
// Implementaremos ahora nuestro codigo para la Pantalla de UI.

#include <Pantalla.h>

/// @file Pantalla.cpp
/// @brief Definición formal para la  clase de la Pantalla Principal de nuestra aplicación la cual realiza la.
/// 
/// Configuración y creación de todos los componentes graficos de la interfaz de usuario para nuestro reproductor
/// de audio.
/// @author Loubiner
/// @date Monday 19/02/2018

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------  Pantalla  -------------------------------------

// -----------------------------------------------------------------------------

// ---------------------------  Constructor  -----------------------------------

// Implementaremos en primer lugar nuestro constructor por defecto con el titulo de nuestra ventana , esté sera el encargado
// de inicializar los elementos de la interfaz base y nuestro reproductor de audio y decodificador.

Pantalla::Pantalla (const std::string & Titulo) : PantallaBase (nullptr , wxID_ANY , wxString (Titulo)) {
	
	// --------------------------  Reproductor  --------------------------------
	
	// Lo primero que realizaremos será la creación de Nuestro Reproductor de Audio , lo cual inicializara los elementos de portaudio y FFmpeg para el
	// decodificador de formatos. Teniendo la asignación siguiente a nuestro campo de memoria para el reproductor.
	
	Player = std::move (std::shared_ptr <Reproductor> (new Reproductor ()));
	
	// ---------------------------  Callbacks  ---------------------------------
	
	// Nuestro reproductor de audio tiene dos callbacks que nos permitiran ejecutar un codigo adicional por cada muestra procesada de audio , tenemos
	// entonces un functor que servira para realizar actualizaciones a nuestra barra de progreso actual.
	
	std::function <void (Reproductor * , Muestreador::Muestras_Audio & , void *)> Funcion_Procesamiento_Barra = [this]
		(Reproductor * Player , Muestreador::Muestras_Audio & Samples , void * Datos) -> void {
		
		// Realizaremos la actualización de nuestro slider mediante una llamada al metodo respectivo en la cola de eventos actual.
		
		this->CallAfter (&Pantalla::Reload_Progress);
		
		// De igual actualizaremos la etiqueta de tiempo.
		
		this->CallAfter (&Pantalla::Reload_Time);
		
	};
	
	// De igual forma contamos con el siguiente metodo de callback el cual invocara de manera asyncrona a nuestro metodo Reset_Progress para reiniciar
	// las barras de progreso y el estado del reproductor.
	
	std::function <void (Reproductor * , void *)> Funcion_Finalizacion_Barra = [this] (Reproductor * Player , void * Datos) -> void {
		
		// Realizaremos el reseteo de nuestro slider y barra de progreso con la siguiente llamada al metodo especifico.
		
		this->CallAfter ([Player , this] () -> void {
			
			// Detendremos el reproductor de audio si está en el Estado de Play ó Pausa.
			
			if (Player->Is_Play () || Player->Is_Pause ()) {
				
				// Detenemos el audio actual.
				
				Player->Stop ();
				
			}
			
			// Resetearemos el estado de nuestras barras.
			
			this->Reset_Progress ();
			
			// Asi mismo con la etiqueta de tiempo.
			
			this->Reset_Time ();
			
			// Colocaremos el estado actual del Reprodutor en la barra de estado.
			
			this->Barra_Estado->SetStatusText (wxString ("Detenido"));
			
		});
		
	};
	
	// Colocaremos estos callbacks a nuestro Reproductor de audio para que los invoque cuando sea necesario.
	
	Player->Set_Callback_Reproduccion (std::move (Funcion_Procesamiento_Barra));
	Player->Set_Callback_Finalizacion (std::move (Funcion_Finalizacion_Barra));
	
	// ------------------------  Zona de Botones  ------------------------------
	
	// Ahora asignaremos un Bitmap para cada zona de botón especifico.
	
	Boton_Play->Set_Enable_Zone (wxBitmap (wxString ("BITMAP_BUTTON_PLAY_MASK") , wxBITMAP_TYPE_PNG_RESOURCE));
	Boton_Pause->Set_Enable_Zone (wxBitmap (wxString ("BITMAP_BUTTON_PAUSE_MASK") , wxBITMAP_TYPE_PNG_RESOURCE));
	Boton_Stop->Set_Enable_Zone (wxBitmap (wxString ("BITMAP_BUTTON_STOP_MASK") , wxBITMAP_TYPE_PNG_RESOURCE));
	
	// ----------------------------  Resize  -----------------------------------
	
	// Guardaremos la relación de aspecto de nuestro contenedor principal , usaremos sus medidas actuales para determinarla.
	
	Relacion_Aspecto = (static_cast <float> (Contenedor->GetSize ().GetWidth ()) / Contenedor->GetSize ().GetHeight ());
	
	// -----------------------------  Time  ------------------------------------
	
	// Resetearemos el tiempo y duración mostrado en la etiqueta.
	
	Reset_Time () , Reset_Duration ();
	
	// ---------------------------  Controls -----------------------------------
	
	// Desabilitamos los controles hasta que un archivo de audio se abra.
	
	Boton_Play->Enable (false) , Boton_Pause->Enable (false) , Boton_Stop->Enable (false);
	
	// ---------------------------  Progress  ----------------------------------
	
	// Vamos a configurar la barra de progreso de la aplicación.
	
	Iniciar_Progress ();
	
	// ----------------------------  TaskBar  ----------------------------------
	
	// Validamos la opción de wxThumbFrame.
	
	#ifdef ELECTRODEMP_ENABLE_WX_THUMBNAIL
	
	// Ahora procederemos  con la inicialización de los componentes de la barra de Tareas (Barra de Progreso y Botones de Comandos) para nuestro
	// programa actual.
	
	Iniciar_Task ();
	
	#endif
	
	// -----------------------------  Frame  -----------------------------------
	
	// Colocaremos el icono de nuestra aplicación antes de presentarla.
	
	SetIcon (wxIcon (wxString ("ICON_FRAME") , wxBITMAP_TYPE_ICO_RESOURCE));
	
	// Lo siguiente es centrar la pantalla y presentarla.
	
	Center () , Show (true);
	
	// De igual forma solamente para thumbnail.
	
	#ifdef ELECTRODEMP_ENABLE_WX_THUMBNAIL
	
	// Finalmente inicializaremos los componentes de TasBar para mostrarlos en la aplicación.
	
	InicializarThumbButtons ();
	
	#endif
	
}

// -----------------------------------------------------------------------------

// ----------------------------  Eventos  --------------------------------------

// Ahora vamos a implementar los eventos de cada categoria en nuestra pantalla para realizar las determinadas acciones que tienen
// destinadas para cada componente o widget.

// --------------  Menu Archivo  -------------

// Tenemos los eventos del Menu Archivo de nuestra barra de Menu. Esté comprende los siguientes metodos.

// Implementaremos primero el metodo para el evento del Menu Abrir el cual realiza una de las tareas más importantes , la de abrir un archivo de
// audio en cualquiera de los formatos soportados , estó permitira iniciar nuestro reproductor de audio , demuxear y preparar el archivo para su
// reproducción.

void Pantalla::Evento_Archivo_Abrir (wxCommandEvent & Argumentos) {
	
	// ---------------------  Seleccionador de Archivo  ------------------------
	
	// Comenzaremos con el paso para seleccionar nuestro archivo de audio el cual debe tener alguna de las siguientes extensiones.
	//		Audio AC3 : (*.ac3).
	//		Audio AAC : (*.aac).
	//		Audio OGG : (*.ogg , *.oga , *.flac).
	//		Audio MP3 : (*.mp3).
	//		Audio WAV : (*.wav).
	//		Video : (*.mp4 , *.m4a , *.mov , *.3gp).
	
	// Crearemos nuestro dialogo de apertura de Archivo para seleccionar el archivo de audio a reproducir usando los wildcard necesarios.
	
	wxFileDialog Dialogo_Apertura (this , wxString ("Selecciona un archivo de Audio") , wxEmptyString , wxEmptyString ,
		wxString (
			"Todos los Audios/Videos Soportados|*.mp3;*.aac;*.m4a;*.ac3;*.ogg;*.oga;*.ogv;*.flac;*.wav;*.mp4;*.mov;*.3gp"
			"|Audio MP3 (*.mp3)|*.mp3"
			"|Audio AAC (*.aac , *.m4a)|*.aac;*.m4a"
			"|Audio AC3 (*.ac3)|*.ac3"
			"|Audio OGG (*.ogg , *.oga , *.ogv)|*.ogg;*.oga;*.ogv"
			"|Audio FLAC (*.flac)|*.flac"
			"|Audio WAV (*.wav)|*.wav"
			"|Video (*.mp4 , *.mov , *.3gp)|*.mp4;*.mov;*.3gp"
			"|Todos los Archivos|*.*"
		) , wxFD_OPEN);
	
	// Mostraremos el dialogo para seleccionar un archivo de nuestro S.O y obtendremos el resultado.
	
	if (Dialogo_Apertura.ShowModal () == wxID_OK) {
		
		// Obtenemos el Filename para nuestro archivo actual en UTF-8.
		
		const std::string Filename (static_cast <const char *> (Dialogo_Apertura.GetPath ().utf8_str ()));
		
		// Si el audio actual no está en estado de cerrado , pasaremos a cerrarlo primero antes de abrir uno nuevo.
		
		if (!Player->Is_Close ()) {
			
			// Invocaremos al metodo siguiente para poderlo cerrar correctamente.
			
			Evento_Archivo_Cerrar (Argumentos);
			
		}
		
		// Realizaremos la Apertura de nuestro archivo de Audio actualmente seleccionado. Validamos el resultado.
		
		if (!Player->Open (Filename)) {
			
			// Mensaje de error.
			
			wxMessageBox (wxString ("Formato de Archivo ó Codec no Soportado") , wxString ("Error al Abrir Archivo de Audio") , (wxOK | wxCENTRE | wxICON_ERROR));
			
			// Devolvemos la llamada.
			
			return;
			
		}
		
		// Validamos si hay más de un stream en el archivo abierto.
		
		if (Player->Get_StreamCount () > 1u) {
			
			// Seleccionaremos un stream de audio de entre todos los totales.
			
			Select_Stream ();
			
		}
		
		// Si ya está abierto el archivo , pasaremos a recargar la Etiqueta y habilitar la barra de progreso.
		
		if (Player->Is_Open ()) {
			
			// Recargamos los datos de nuestro audio en la pantalla.
			
			Reload_Tag ();
			
			// Habilitamos los botones de control.
			
			Boton_Play->Enable (true) , Boton_Pause->Enable (true) , Boton_Stop->Enable (true);
			
			// Habilitamos la barra de progreso actual y actualizamos el valor maximo.
			
			Gauge_Progreso->Enable (true) , Gauge_Progreso->Set_Value_Max (Player->Get_Duration ());
			
			// Opción de thumbnail.
			
			#ifdef ELECTRODEMP_ENABLE_WX_THUMBNAIL
			
			// Actualizamos el valor maximo del Progress de thumbnail.
			
			Barra_Progreso->SetMaximo (Player->Get_Duration ());
			
			#endif
			
			// Asi mismo actualizaremos la etiqueta de tiempo y duración.
			
			Reload_Time () , Reload_Duration ();
			
			// Colocaremos el estado actual del Reprodutor en la barra de estado.
			
			Barra_Estado->SetStatusText (wxString ("Abierto"));
			
		}
		
	}
	
}

// Definiremos el metodo para el Menu de Cerrar el cual realiza la correcta terminación de un archivo de audio previamente abierto y
// libera los recursos ocupados por este mismo.

void Pantalla::Evento_Archivo_Cerrar (wxCommandEvent & Argumentos) {
	
	// Cerraremos nuestro archivo actualmente abierto si el estado es Open , Play , Pause o Stop.
	
	if (Player->Is_Open () || Player->Is_Play () || Player->Is_Pause () || Player->Is_Stop ()) {
		
		// Para el caso de CIMG.
		
		// ---------------------------------------------------------------------
		
		#if defined (_WIN32) && defined (ELECTRODEMP_ENABLE_CIMG)
		
		// Cerraremos nuestra previsualización del Cover.
		
		Player->Get_Tag ().Cover.Close_Preview ();
		
		#endif
		
		// ---------------------------------------------------------------------
		
		// Cerramos nuestro archivo de Audio actualmente abierto.
		
		if (!Player->Close ()) {
			
			// Mensaje de error.
			
			wxMessageBox (wxString ("Error al Cerrar Archivo Actual") , wxString ("Error al Cerrar Audio") , (wxOK | wxCENTRE | wxICON_ERROR));
			
			// Retornamos la llamada.
			
			return;
			
		}
		
		// Invocaremos a nuestro metodo para reiniciar la barra de progreso.
		
		Reset_Progress ();
		
		// Lo mismo para la etiqueta de tiempo y duración.
		
		Reset_Time () , Reset_Duration ();
		
		// Desabilitamos los controles hasta que un archivo de audio se abra nuevamente.
		
		Boton_Play->Enable (false) , Boton_Pause->Enable (false) , Boton_Stop->Enable (false);
		
		// Desabilitaremos la barra.
		
		Gauge_Progreso->Enable (false);
		
		// Reseteamos las Etiquetas.
		
		Reset_Tag ();
		
		// Colocaremos el estado actual del Reprodutor en la barra de estado.
		
		Barra_Estado->SetStatusText (wxString ("Cerrado"));
		
	}
	
}

// Ahora implementaremos el evento Menu Salir para terminar con nuestra aplicación de manera segura , tenemos entonces el siguiente
// metodo para realizarlo.

void Pantalla::Evento_Archivo_Salir (wxCommandEvent & Argumentos) {
	
	// Cerraremos ahora nuestra pantalla actual para terminar la aplicación.
	
	this->Close ();
	
}

// Implementaremos el metodo que nos permite cambiar el host de dispositivo y dispositivo de salida para la reproducción de nuestro audio ,
// teniendo en cuenta la lista de disponibilidad que presentaremos en un dialogo especifico.

void Pantalla::Evento_Herramientas_Device (wxCommandEvent & Argumentos) {
	
	// Crearemos un dialogo para la selección de dispositivo y lo mostraremos de manera modal.
	
	Dialogo Dialogo_Dispositivo (this , Player);
	
	// Mostraremkos el dialogo actual para seleccionar device nuevo de reproducción.
	
	Dialogo_Dispositivo.ShowModal ();
	
}

// Implementaremos el codigo de nuestro evento para mostrar el dialogo de Acerca de...

void Pantalla::Evento_Ayuda_Acerca (wxCommandEvent & Argumentos) {
	
	// Crearemos una instancia de nuestro dialogo de ayuda a presentar.
	
	DialogoAyuda Dialogo_Acerca (this);
	
	// Mostraremos el dialogo modal.
	
	Dialogo_Acerca.ShowModal ();
	
}

// --------------  Barra Herramientas  -------------

// Definiremos ahora el evento para cada uno de los botones presentes en la barra de herramientas los cuales son accesos directos para los eventos
// más comunes de nuestros respectivos menus.

// Implementaremos el evento para el Tool Abrir el cual realiza la llamada al evento del Menu Archivo respectivo.

void Pantalla::Evento_Tool_Abrir (wxCommandEvent & Argumentos) {
	
	// Realizaremos la llamada respectiva a nuestro evento de Menu Archivo.
	
	Evento_Archivo_Abrir (Argumentos);
	
}

// Ahora pasaremos a implementar el metodo para el Evento del Tool Cerrar el cual simplemente realiza la terminación del archivo actual.

void Pantalla::Evento_Tool_Cerrar (wxCommandEvent & Argumentos) {
	
	// De igual forma realizamos la llamada respectiva de nuestro menu archivo.
	
	Evento_Archivo_Cerrar (Argumentos);
	
}

// Ahora para el evento de la barra de herramientas de selección de dispositivo tenemos el siguiente codigo.

void Pantalla::Evento_Tool_Device (wxCommandEvent & Argumentos) {
	
	// Realizaremos la llamada al evento de la herramienta de menu especifica.
	
	Evento_Herramientas_Device (Argumentos);
	
}

// ---------------  Reproducción  -------------------

// Implementaremos el codigo para los eventos de nuestros botones de reproducción los cuales realizan las tareas de iniciar , pausar y detener
// la reproducción de audio en nuestro programa.

// Implementaremos el evento Play para iniciar la reproducción del audio actualmente abierto.

void Pantalla::Evento_Play (wxCommandEvent & Argumentos) {
	
	// Validaremos si el estado actual del reproductor es igual a Open , Pause ó Stop.
	
	if (Player->Is_Open () || Player->Is_Pause () || Player->Is_Stop ()) {
		
		// Iniciaremos la reproducción de Audio actual.
		
		if (!Player->Play ()) {
			
			// Mensaje de error.
			
			wxMessageBox (wxString ("Error al Reproducir el Audio Actual") , wxString ("Error al Iniciar Audio") , (wxOK | wxCENTRE | wxICON_ERROR));
			
			// Devolvemos la llamada.
			
			return;
			
		}
		
		// Opción de thumbnail.
		
		#ifdef ELECTRODEMP_ENABLE_WX_THUMBNAIL
		
		// Colocaremos el estado normal en la barra de progreso y lo actualizamos.
		
		Barra_Progreso->SetEstado (wxThumbProgress::FLAG_NORMAL) , Barra_Progreso->RefreshEstado ();
		
		#endif
		
		// Colocaremos el estado actual del Reprodutor en la barra de estado.
		
		Barra_Estado->SetStatusText (wxString ("Reproduciendo"));
		
	}
	
}

// Implementaremos el evento Pause para detener la reproducción de audio actual.

void Pantalla::Evento_Pause (wxCommandEvent & Argumentos) {
	
	// Validaremos si el estado actual del reproductor es igual a Play.
	
	if (Player->Is_Play ()) {
		
		// Pausaremo la reproducción de Audio actual.
		
		if (!Player->Pause ()) {
			
			// Mensaje de error.
			
			wxMessageBox (wxString ("Error al Pausar el Audio Actual") , wxString ("Error al Pausar Audio") , (wxOK | wxCENTRE | wxICON_ERROR));
			
			// Devolvemos la llamada.
			
			return;
			
		}
		
		// Opción de thumbnail.
		
		#ifdef ELECTRODEMP_ENABLE_WX_THUMBNAIL
		
		// Colocaremos el estado pause en la barra de progreso y lo actualizamos.
		
		Barra_Progreso->SetEstado (wxThumbProgress::FLAG_PAUSE) , Barra_Progreso->RefreshEstado ();
		
		#endif
		
		// Colocaremos el estado actual del Reprodutor en la barra de estado.
		
		Barra_Estado->SetStatusText (wxString ("Pausado"));
		
	}
	
}

// Para detener y reiniciar el audio tenemos el metodo Stop.

void Pantalla::Evento_Stop (wxCommandEvent & Argumentos) {
	
	// Validaremos que el estado actual del Reproductor sea igual a Play ó Pause.
	
	if (Player->Is_Play () || Player->Is_Pause ()) {
		
		// Detendremos el archivo de audio antes de cerrarlo.
		
		if (!Player->Stop ()) {
			
			// Mensaje de error.
			
			wxMessageBox (wxString ("Error al Detener el Audio Actual") , wxString ("Error al Detener Audio") , (wxOK | wxCENTRE | wxICON_ERROR));
			
			// Devolvemos la llamada.
			
			return;
			
		}
		
		// Resetearemos el estado no progress en la barra de progreso y lo actualizamos.
		
		Reset_Progress ();
		
		// Dejaremos la etiqueta de tiempo en su valor original.
		
		Reload_Time ();
		
		// Colocaremos el estado actual del Reprodutor en la barra de estado.
		
		Barra_Estado->SetStatusText (wxString ("Detenido"));
		
	}
	
}

// Solo si la opción está habilitada.

#ifdef ELECTRODEMP_ENABLE_WX_THUMBNAIL

// ----------------  Thumb Buttón  ------------------

void Pantalla::Evento_Thumb_Boton (wxThumbEvent & Argumentos) {
	
	// Validaremos el Id de nuestro argumento para saber a que metodo invocar con los parametros especificos.
	
	switch (Argumentos.GetId ()) {
		
		// Play de Audio.
		
		case Thumb_Boton_Play_ID :
			
			// Reproducir el audio. Con el evento Play.
			
			Evento_Play (Argumentos);
			break;
		
		// Pausa de Audio.
		
		case Thumb_Boton_Pause_ID :
			
			// Pausamos el audio. Con el evento Pause.
			
			Evento_Pause (Argumentos);
			break;
		
		// Stop de Audio.
		
		case Thumb_Boton_Stop_ID :
			
			// Detenemos el audio. Con el evento Stop.
			
			Evento_Stop (Argumentos);
			break;
		
	}
	
}

#endif // ELECTRODEMP_ENABLE_WX_THUMBNAIL

// --------------------  Seek  ----------------------

// Tenemos ademas un metodo para el evento de la barra de desplazamiento la cual realiza la tarea de desplazar el audio a cualquier sample
// especificado mediante una barra contada de 0 a 100.

void Pantalla::Evento_Cambio_Seek (ElectrodeProgresoEvent & Argumentos) {
	
	// Validamos si estamos cambiando nuestro slider.
	
	if (!Player->Is_Close () && Player_Seek) {
		
		// Moveremos nuestro reproductor de audio a la posición determinada. Obtenemos el valor actual para el slider.
		
		const int Valor_Actual = Argumentos.Get_Progress_Value ();
		
		// Pasaremos este valor a un intervalo de 0 - 1.0f.
		
		const float Valor_Normalizado = std::min ((static_cast <float> (Valor_Actual) / Gauge_Progreso->Get_Value_Max ()) , 1.0f);
		
		// Determinaremos el segundo actual al cual desplazarnos con la formula : Segundo = Valor * Duracion.
		
		const size_t Segundo_Desplazar = (static_cast <size_t> (Valor_Normalizado * Player->Get_Duration ()));
		
		// Desplazaremos nuestro audio a esa posición.
		
		Player->Seek (Segundo_Desplazar);
		
		// Y desactivaremos el seek.
		
		Player_Seek = false;
		
	}
	
}

// Definiremos el metodo para actualizar los valores de tiempo actualmente mostrados en la etiqueta conforme el tiempo se desplace.

void Pantalla::Evento_Slider_Seek (ElectrodeProgresoEvent & Argumentos) {
	
	// Recargaremos la etiqueta de tiempo.
	
	Reload_Time ();
	
}

// Implementaremos un metodo para activar o desactivar nuestro desplazamiento de audio conforme el slider se desplace.

void Pantalla::Evento_Seek (ElectrodeProgresoEvent & Argumentos) {
	
	// Validamos que estémos actualmente en estado distinto a close.
	
	if (!Player->Is_Close ()) {
		
		// Activamos el slider de desplazamiento para poder mover libremente el slider.
		
		Player_Seek = true;
		
	}
	
}

// -------------------  Frame  ----------------------

// Implementaremos el evento Close para nuestro Frame actual el cual realiza la liberación de todos los recursos ocupados por nuestro reproductor y
// finaliza con las bibliotecas que iniciamos al principio.

void Pantalla::Evento_Close (wxCloseEvent & Argumentos) {
	
	// Ocultaremos la pantalla.
	
	this->Show (false);
	
	// -------------------------------------------------------------------------
	
	// Si tenemos habilitado CImg entonces cerramos esto.
	
	#if defined (_WIN32) && defined (ELECTRODEMP_ENABLE_CIMG)
	
	// Cerraremos nuestra previsualización del Cover antes de terminar.
	
	Player->Get_Tag ().Cover.Close_Preview ();
	
	#endif
	
	// -------------------------------------------------------------------------
	
	// Cerraremos el archivo de audio actual si no lo está todavia.
	
	if (!Player->Is_Close ()) {
		
		// Cerramos el audio actualmente abierto.
		
		Player->Close ();
		
	}
	
	// Esperaremos 1 segundo para cerra la aplicación.
	
	wxThread::This ()->Sleep (1000);
	
	// Terminaremos la aplicación actual con Destroy.
	
	this->Destroy ();
	
}

// Definiremos el evento para el resizer del contenedor de nuestra pantalla actual , tenemos entonces el siguiente codigo para poder ajustar el tamaño
// del contenedor a la relación de aspecto fija.

void Pantalla::Evento_Size (wxSizeEvent & Argumentos) {
	
	// Actualizamos el layout.
	
	this->Layout ();
	
	// Repintaremos el cover.
	
	Contenedor_Cover->Refresh ();
	
}

// -----------------------------------------------------------------------------

// -----------------------------  Destructor  ----------------------------------

// Definiremos el destructor de la Pantalla principal.

Pantalla::~ Pantalla () {
	
	// Primero finalizaremos el reproductor de audio.
	
	Reproductor::Terminate ();
	
	// Por ultimo finalizamos el LOG de ElectrodeMP.
	
	ElectrodeMP::LOG_Terminate ();
	
}

// -----------------------------------------------------------------------------

// -----------------------------  Privados  ------------------------------------

// Ahora pasaremos a definir nuestros metodos privados para nuestra pantalla principal , comenzando por el metodo para iniciar los componentes de
// la barra de progreso en nuestra aplicación.

void Pantalla::Iniciar_Progress () {
	
	// Asignaremos el siguiente bitmap de fondo a nuestra barra de progreso del reproductor.
	
	Gauge_Progreso->Set_Background (wxBitmap (wxString ("BITMAP_PROGRESS") , wxBITMAP_TYPE_PNG_RESOURCE));
	
	// Gauge_Progreso->Set_Background_Filled (wxBitmap (wxString ("BITMAP_PROGRESS_FILL") , wxBITMAP_TYPE_PNG_RESOURCE));
	
	// Asignaremos ahora el background para cuando la barra está en estado disable.
	
	Gauge_Progreso->Set_Background_Disable (wxBitmap (wxString ("BITMAP_PROGRESS_DISABLE") , wxBITMAP_TYPE_PNG_RESOURCE));
	
	// Asi mismo colocaremos el bitmap monocromatico para especificar la zona valida para realizar eventos de seek.
	
	Gauge_Progreso->Set_Background_Line (wxBitmap (wxString ("BITMAP_PROGRESS_LINE") , wxBITMAP_TYPE_PNG_RESOURCE));
	
	// Colocaremos el color para el progreso de está forma.
	
	Gauge_Progreso->Set_Progress_Colour (wxColour (27 , 95 , 179));
	
	// Cargaremos ahora la mascara en común para todos los bitmaps del cursor exepto el disable.
	
	const wxBitmap Bitmap_Cursor_Mask (wxString ("BITMAP_PROGRESS_CUR_MASK") , wxBITMAP_TYPE_PNG_RESOURCE);
	
	// Ahora la mascara para nuestro cursor disable.
	
	const wxBitmap Bitmap_Cursor_Disable_Mask (wxString ("BITMAP_PROGRESS_CUR_DISABLE_MASK") , wxBITMAP_TYPE_PNG_RESOURCE);
	
	// Asignaremos el siguiente bitmap para el cursor de la barra de progreso.
	
	Gauge_Progreso->Set_Cursor (wxBitmap (wxString ("BITMAP_PROGRESS_CUR") , wxBITMAP_TYPE_PNG_RESOURCE) , Bitmap_Cursor_Mask);
	
	// Asignaremos el bitmap para el cursor en estado desabilitado.
	
	Gauge_Progreso->Set_Cursor_Disable (wxBitmap (wxString ("BITMAP_PROGRESS_CUR_DISABLE") , wxBITMAP_TYPE_PNG_RESOURCE) , Bitmap_Cursor_Disable_Mask);
	
	// Asignaremos el bitmap para el cursor en estado seleccionado.
	
	Gauge_Progreso->Set_Cursor_Selected (wxBitmap (wxString ("BITMAP_PROGRESS_CUR_SELECTED") , wxBITMAP_TYPE_PNG_RESOURCE) , Bitmap_Cursor_Mask);
	
	// Asignaremos el bitmap para el cursor en estado presionado.
	
	Gauge_Progreso->Set_Cursor_Pressed (wxBitmap (wxString ("BITMAP_PROGRESS_CUR_PRESSED") , wxBITMAP_TYPE_PNG_RESOURCE) , Bitmap_Cursor_Mask);
	
	// Colocaremos un borde para el cursor de 4 pixeles.
	
	Gauge_Progreso->Set_Cursor_Border (4);
	
	// Activaremos el doble buffer sobre está barra de progreso.
	
	Gauge_Progreso->SetDoubleBuffered (true);
	
	// ------------------------------  Eventos  --------------------------------
	
	// Conectaremos ahora el evento wxEVT_ELECTRODE_PROGRESS_CURSOR_DOWN a un metodo especifico en nuestra pantalla para cuando se presiona el cursor.
	
	Bind (wxEVT_ELECTRODE_PROGRESS_CURSOR_DOWN , &Pantalla::Evento_Seek , this , Gauge_Progreso->GetId () , Gauge_Progreso->GetId ());
	
	// Conectaremos el evento SLIDER con el metodo siguiente para poder actualizar los valores de etiqueta que tiene asignados.
	
	Bind (wxEVT_ELECTRODE_PROGRESS_SLIDER , &Pantalla::Evento_Slider_Seek , this , Gauge_Progreso->GetId () , Gauge_Progreso->GetId ());
	
	// Asi como el evento CHANGUED el cual realizara el procesado del Seek para cambiar la posición del audio.
	
	Bind (wxEVT_ELECTRODE_PROGRESS_CHANGUED , &Pantalla::Evento_Cambio_Seek , this , Gauge_Progreso->GetId () , Gauge_Progreso->GetId ());
	
}

// Tenemos el siguiente codigo si la opción dw wxThumbnail fue marcada en true.

#ifdef ELECTRODEMP_ENABLE_WX_THUMBNAIL

// Implementaremos nuestro metodo para iniciar la taskbar que está incluida para poder realizar acciones desde la vista previa de la aplicación.

void Pantalla::Iniciar_Task () {
	
	// Para crear nuestros elementos de botones y barra de progreso en la barra de tareas de nuestro programa , requerimos establecerlos mediante la llamada
	// a alguno de los metodos AddThumbButton con los siguientes parametros :
	//		ID de nuestro Botón a anexar en la thumb task.
	//		Bitmap de icono para presentar.
	//		Bitmap de mascara (opcional) a utilizar.
	//		Tooltop a presentar cuando se coloca el mouse sobre el botón.
	
	// Basicamente podemos crear y anexar estos botones especiales con tan solo estos 4 metodos , tenemos entonces por lo tanto tres botones en la barra de
	// tareas :
	//		Botón de Play.
	//		Botón de Pausa.
	//		Botón de Stop.
	
	// Anexaremos cada botón a nuestro ThumbFrame con el metodo antes mencionado y los parametros que corresponde a cada uno.
	
	AddThumbButton (Thumb_Boton_Play_ID , wxBitmap (wxString ("BITMAP_THUMB_PLAY") , wxBITMAP_TYPE_PNG_RESOURCE) , wxNullBitmap , wxString ("Reproducir"));
	AddThumbButton (Thumb_Boton_Pause_ID , wxBitmap (wxString ("BITMAP_THUMB_PAUSE") , wxBITMAP_TYPE_PNG_RESOURCE) , wxNullBitmap , wxString ("Pausar"));
	AddThumbButton (Thumb_Boton_Stop_ID , wxBitmap (wxString ("BITMAP_THUMB_STOP") , wxBITMAP_TYPE_PNG_RESOURCE) , wxNullBitmap , wxString ("Detener"));
	
	// ------------------------------  Eventos  --------------------------------
	
	// Conectaremos los tres botones con el respectivo metodo bind a nuestro evento de Thumb Botón para que realicen las acciones determinadas.
	
	Bind (wxEVT_THUMB_BUTTON , &Pantalla::Evento_Thumb_Boton , this , Thumb_Boton_Play_ID , Thumb_Boton_Stop_ID);
	
	// Ahora vamos a crear la barra de progreso de la barra de tareas de thumb mediante el constructor siguiente con la referencia a nuestra pantalla actual.
	
	Barra_Progreso = new wxThumbProgress (this); // De 0 a 100.
	
}

#endif // ELECTRODEMP_ENABLE_WX_THUMBNAIL

// -----------------------------------------------------------------------------

// -------------------------------  Stream  ------------------------------------

// Definiremos el metodo para la selección de stream mediante un dialogo simple de selección que permita al usuario indicar el flujo de audio a reproducir.

void Pantalla::Select_Stream () {
	
	// Primero crearemos un dialogo de selección simple con todos los valores de flujo a colocar.
	
	wxArrayString Arreglo_Seleccion;
	
	// Anexamos cada elemento de la lista de Streams numericos con el formato especifico.
	
	for (size_t Indice_Elemento = 0u ; Indice_Elemento < Player->Get_StreamCount () ; ++ Indice_Elemento) {
		
		// Anexamos el siguiente string al array.
		
		Arreglo_Seleccion.Add (wxString::Format (wxString ("Stream de Audio : %i") , Indice_Elemento));
		
	}
	
	// Creamos el dialogo para la selección de alguna de todas las opciones de flujo de audio.
	
	wxSingleChoiceDialog Dialogo_Seleccion (this , wxString ("Seleccióna un Flujo de Audio a Reproducir") , wxString ("Seleccionar Stream") , Arreglo_Seleccion);
	
	// Seleccionaremos por defecto el indice 0.
	
	Dialogo_Seleccion.SetSelection (0);
	
	// Mostraremos el dialogo para la selección y validaremos lo siguiente.
	
	if (Dialogo_Seleccion.ShowModal () == wxID_OK) {
		
		// Seleccionaremos el stream de audio actual y validamos nuevamente.
		
		if (!Player->Open_Selection (Dialogo_Seleccion.GetSelection ())) {
			
			// Mensaje de error.
			
			wxMessageBox (wxString ("Error al Abrir Stream de Audio") , wxString ("Error al Abrir Stream") , (wxOK | wxCENTRE | wxICON_ERROR));
			
		}
		
	}
	
}

// -----------------------------------------------------------------------------

// -------------------------------  Tiempo  ------------------------------------

// Definiremos el metodo para recargar el valor de la etiqueta de tiempo actual.

void Pantalla::Reload_Time () {
	
	// Lo primero que haremos sera obtener el tiempo actual del reproductor en segundos para poder convertir a minutos.
	
	const int Tiempo_Actual = static_cast <int> (Gauge_Progreso->Get_Value ());
	
	// Ahora vamos a determinar el valor de minutos para esté tiempo dividiendo entre 60 (60 segundos por minuto).
	
	const int Tiempo_Actual_Min = (Tiempo_Actual / 60);
	
	// Ahora el residuo nos indicara los segundos restantes.
	
	const int Tiempo_Actual_Seg = (Tiempo_Actual % 60);
	
	// Tenemos la siguiente cadena de formato para construir el tiempo actual : MM:SS.
	
	const wxString Cadena_Formato ("%02i:%02i");
	
	// Finalmente calcularemos la cadena resultante del tiempo actual con el formato especifico.
	
	const wxString Cadena_Tiempo (wxString::Format (Cadena_Formato , Tiempo_Actual_Min , Tiempo_Actual_Seg));
	
	// Si la cadena de la etiqueta es diferente que está , entonces la actualizaremos.
	
	if (Etiqueta_Time->GetLabel () != Cadena_Tiempo) {
		
		// Asignaremos la nueva cadena.
		
		Etiqueta_Time->SetLabel (Cadena_Tiempo);
		
	}
	
}

// Implementaremos el metodo para recargar el valor de la etiqueta de duración.

void Pantalla::Reload_Duration () {
	
	// Repetiremos lo mismo que con el tiempo pero para la duración total del audio actual.
	
	const int Duracion = static_cast <int> (Player->Get_Duration ());
	
	// Calcularemos la duración en minutos primero.
	
	const int Duracion_Min = (Duracion / 60);
	
	// Ahora la duración total en segundos restantes.
	
	const int Duracion_Seg = (Duracion % 60);
	
	// Tenemos la siguiente cadena de formato para construir la duración actual : MM:SS.
	
	const wxString Cadena_Formato ("%02i:%02i");
	
	// Ahora para la cadena del tiempo de la duración.
	
	const wxString Cadena_Duracion (wxString::Format (Cadena_Formato , Duracion_Min , Duracion_Seg));
	
	// De igual forma si la cadena de la duración es diferente , entonces la cambiamos.
	
	if (Etiqueta_Duration->GetLabel () != Cadena_Duracion) {
		
		// Asignaremos la nueva cadena de la duración.
		
		Etiqueta_Duration->SetLabel (Cadena_Duracion);
		
	}
	
}

// Ahora el metodo para resetear el tiempo a su valor inicial.

void Pantalla::Reset_Time () {
	
	// Asignaremos a la cadena de tiempo un valor de 00:00.
	
	Etiqueta_Time->SetLabel (wxString ("00:00"));
	
}

// Tenemos el metodo para resetear el valor de la duración.

void Pantalla::Reset_Duration () {
	
	// Asignaremos de igual forma la duración en 00:00.
	
	Etiqueta_Duration->SetLabel (wxString ("00:00"));
	
}

// -----------------------------------------------------------------------------

// ------------------------------  Etiqueta  -----------------------------------

// Implementaremos el metodo para recargar toda la información de la canción abierta actualmente utilizando la estructura de Etiqueta.

void Pantalla::Reload_Tag () {
	
	// Actualizaremos todas las Etiquetas de nuestro contenedor de Información. Usaremos la información contenida en la Caratula Generada por nuestro
	// reproductor y Etiquetador , asi que obtenemos la referencia.
	
	const Etiqueta & Audio_Tag = Player->Get_Tag ();
	
	// ----------------------  Actualizando Labels  ----------------------------
	
	// Actualizaremos primero la Cadena de String de la Etiqueta de Titulo.
	
	Etiqueta_Title->SetLabel 	(wxString ("Titulo :    ") + (!Audio_Tag.Title.empty () ? wxString::FromUTF8 (Audio_Tag.Title.c_str ()) : wxString ("Desconocido")));
	
	// Actualizaremos la Cadena del Artista.
	
	Etiqueta_Artist->SetLabel 	(wxString ("Artista :   ") + (!Audio_Tag.Artist.empty () ? wxString::FromUTF8 (Audio_Tag.Artist.c_str ()) : wxString ("Desconocido")));
	
	// Actualizaremos la Cadena del Compositor.
	
	Etiqueta_Composer->SetLabel (wxString ("Compositor : ") + (!Audio_Tag.Composer.empty () ? wxString::FromUTF8 (Audio_Tag.Composer.c_str ()) : wxString ("Desconocido")));
	
	// Actualizaremos la Cadena del Album.
	
	Etiqueta_Album->SetLabel 	(wxString ("Albun :    ") + (!Audio_Tag.Album.empty () ? wxString::FromUTF8 (Audio_Tag.Album.c_str ()) : wxString ("Desconocido")));
	
	// Actualizaremos la Cadena del Año.
	
	Etiqueta_Year->SetLabel 	(wxString ("Año :     ") + (!Audio_Tag.Year.empty () ? wxString::FromUTF8 (Audio_Tag.Year.c_str ()) : wxString ("Desconocido")));
	
	// Actualizaremos la Cadena del Numero de Pista.
	
	Etiqueta_Track->SetLabel 	(wxString ("Pista :    ") + (!Audio_Tag.Track.empty () ? wxString::FromUTF8 (Audio_Tag.Track.c_str ()) : wxString ("Desconocido")));
	
	// Actualizaremos la Cadena del Genero.
	
	Etiqueta_Genre->SetLabel 	(wxString ("Genero :    ") + (!Audio_Tag.Genre.empty () ? wxString::FromUTF8 (Audio_Tag.Genre.c_str ()) : wxString ("Desconocido")));
	
	// Actualizarmeos la Cadena de Fecha Original.
	
	Etiqueta_Date_Original->SetLabel (wxString ("Fecha de creación : ") + (!Audio_Tag.Date_Original.empty () ?
		wxString::FromUTF8 (Audio_Tag.Date_Original.c_str ()) : wxString ("Desconocido")));
	
	// Actualizarmeos la Cadena de Fecha Actual.
	
	Etiqueta_Date->SetLabel (wxString ("Fecha de modificación : ") + (!Audio_Tag.Date.empty () ?
		wxString::FromUTF8 (Audio_Tag.Date.c_str ()) : wxString ("Desconocido")));
	
	// Actualizamos el layout para el panel de las Etiquetas y Refrescamos.
	
	Contenedor_Tag->Layout () , Contenedor_Tag->Refresh ();
	
	// Finalmente si tenemos una Imagen de Portada valida , la asignaremos entonces a nuestro componente.
	
	if (Audio_Tag.Cover.Is_Valid ()) {
		/*
		Caratula Cover_Nuevo (Audio_Tag.Cover);
		
		Caratula::iterator ll1 = Cover_Nuevo.begin ();
		
		Caratula::const_iterator ll (ll1);
		
		for (Caratula::CColor & Color_Actual : Cover_Nuevo) {
			
			Color_Actual.Red () = ll->Blue ();
			
			Color_Actual.Blue () = ll->Red ();
			
			ll++;
			
		}
		*/
		// Asignaremos la caratula al contenedor del cover.
		
		Contenedor_Cover->Set_Cover (Audio_Tag.Cover);
		
		// Repintaremos el cover.
		
		Contenedor_Cover->Refresh ();
		
	}
	
}

// Ahora el metodo para regresar el estado de las etiquetas a un texto vacio.

void Pantalla::Reset_Tag () {
	
	// Actualizaremos primero la Cadena de String de la Etiqueta de Titulo.
	
	Etiqueta_Title->SetLabel 	(wxEmptyString);
	
	// Actualizaremos la Cadena del Artista.
	
	Etiqueta_Artist->SetLabel 	(wxEmptyString);
	
	// Actualizaremos la Cadena del Compositor.
	
	Etiqueta_Composer->SetLabel (wxEmptyString);
	
	// Actualizaremos la Cadena del Album.
	
	Etiqueta_Album->SetLabel 	(wxEmptyString);
	
	// Actualizaremos la Cadena del Año.
	
	Etiqueta_Year->SetLabel 	(wxEmptyString);
	
	// Actualizaremos la Cadena del Numero de Pista.
	
	Etiqueta_Track->SetLabel 	(wxEmptyString);
	
	// Actualizaremos la Cadena del Genero.
	
	Etiqueta_Genre->SetLabel 	(wxEmptyString);
	
	// Actualizamos la cadena de creación original.
	
	Etiqueta_Date_Original->SetLabel (wxEmptyString);
	
	// Actualizamos la cadena de fecha de modificacion.
	
	Etiqueta_Date->SetLabel (wxEmptyString);
	
	// Repintaremos el contenedor de las Etiquetas.
	
	Contenedor_Tag->Refresh ();
	
	// Actualizaremos la Caratula de Portada.
	
	Contenedor_Cover->Clear_Cover ();
	
	// Repintaremos el cover.
	
	Contenedor_Cover->Refresh ();
	
}

// -----------------------------------------------------------------------------

// ------------------------------  Progreso  -----------------------------------

// Ahora implementaremos el metodo para recargar nuestra barra de desplazamiento conforme el audio se va procesando.

void Pantalla::Reload_Progress () {
	
	// Obtendremos la cantidad actual de segundos en el audio.
	
	const size_t Duracion = Player->Get_Duration ();
	
	// Ahora vamos a recuperar el segundo de procesamiento actual. Limitando el resultado a un valor max en la barra de progreso.
	
	const int Tiempo_Actual = std::min (static_cast <int> (Player->Get_ActualTime ()) , static_cast <int> (Gauge_Progreso->Get_Value_Max ()));
	
	// LOG_FN (" >> T ACTUAL : %.2f " , Tiempo_Actual)
	
	// Si no estamos en el evento de Seek , desplazamos el slider.
	
	if (!Player_Seek) {
		
		// Asignaremos este valor a nuestra barra de progreso.
		
		Gauge_Progreso->Set_Value (Tiempo_Actual);
		
	}
	
	// Opción de thumbnail.
	
	#ifdef ELECTRODEMP_ENABLE_WX_THUMBNAIL
	
	// Y lo asignaremos asi mismo a nuestro progress de ThumbBar.
	
	Barra_Progreso->SetCompletado (Tiempo_Actual);
	
	// Recacargamos los valores.
	
	Barra_Progreso->RefreshValores ();
	
	#endif
	
}

// Definiremos asi mismo nuestro metodo para reiniciar el estado de nuestras barras de progreso y slider.

void Pantalla::Reset_Progress () {
	
	// Colocaremos el valor siguiente en el slider.
	
	Gauge_Progreso->Set_Value (Gauge_Progreso->Get_Value_Min ());

	// Opción de thumbnail.
	
	#ifdef ELECTRODEMP_ENABLE_WX_THUMBNAIL
	
	// Ahora pondremos un valor de 0 en nuestro progreso de thumbbar.
	
	Barra_Progreso->SetCompletado (0ull);
	
	// Colocaremos el estado de noprogress en nuestro thumb.
	
	Barra_Progreso->SetEstado (wxThumbProgress::FLAG_NOPROGRESS);
	
	// Y refrescaremos el estado y los valores.
	
	Barra_Progreso->RefreshEstado () , Barra_Progreso->RefreshValores ();
	
	#endif
	
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------
