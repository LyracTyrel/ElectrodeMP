
// Implementaremos ahora nuestro codigo para el Dialogo de selección de dispositivo y host.

#include <Dialogo.h>

/// @file Dialogo.cpp
/// @brief Definición formal para nuestro dialogo de selección de Api host y dispositivo de reproducción de audio.
/// 
/// Establecemos el dispositivo de audio una vez se haya seleccionado de la caja de texto respectiva.
/// @author Loubiner
/// @date Wednesday 21/02/2018

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// ---------------------------  Constructor  -----------------------------------

// Comenzaremos como de construmbre implementando el codigo para nuestro constructor del dialogo el cual realiza la asignación de la lista de contenido
// para los combobox de nuestro dialogo actual.

Dialogo::Dialogo (wxWindow * Parent , const std::shared_ptr <Reproductor> & Player_Actual) : DialogoDevice (Parent) , Player (Player_Actual) {
	
	// -------------------------  Recargando Listas  ---------------------------
	
	// Lo primero que haremos sera recargar la lista de selección de Host y de Dispositivos en nuestros combobox , usaremos el valor por defecto que
	// tenemos por portaudio para determinar cual es el host por defecto.
	
	// Limpiaremos ambas listas en primer lugar.
	
	Combo_Host->Clear () , Combo_Device->Clear ();
	
	// Asi que lo primero sera obtener una lista de cadenas que utilizaremos para seleccionar desde el combobox , usamos el metodo respectivo en nuestro
	// reproductor para recuperar las etiquetas.
	
	const std::vector <std::string> Lista_Host (std::move (Player->Get_HostList ()));
	
	// Ahora pasaremos a recuperar el indice de Host actualmente asignado a nuestro reproductor para poderlo seleccionar de la lista de Hosts actuales.
	
	const PaHostApiIndex Indice_Host_Actual = Player->Get_HostIndex ();
	
	// Consultaremos toda la lista de Dispositivo para esté host de igual forma que con la lista de host.
	
	const std::vector <std::string> Lista_Device (std::move (Player->Get_DeviceList (Indice_Host_Actual)));
	
	// Recuperaremos el nombre de nuestro dispositivo actualmente utilizado con el siguiente metodo.
	
	const std::string Nombre_Device_Actual (Player->Get_DeviceName (Player->Get_DeviceIndex ()));
	
	// Finalmente llenaremos nuestros combobox para ambos casos. Iteramos por cada elemento en nuestra lista de Host´s Api's.
	
	for (const std::string & Nombre_Host : Lista_Host) {
		
		// Anexaremos el siguiente elemento en el Combobox.
		
		Combo_Host->Append (wxString (Nombre_Host));
		
	}
	
	// Repetiremos lo mismo para el combobox de nuestros dispositivos.
	
	for (const std::string & Nombre_Device : Lista_Device) {
		
		// Anexaremos el siguiente elemento en el Combobox.
		
		Combo_Device->Append (wxString (Nombre_Device));
		
	}
	
	// Ahora seleccionaremos el Host actualmente utilizado en el reproductor dentro de nuestro combobox.
	
	Combo_Host->Select (static_cast <int> (Indice_Host_Actual));
	
	// Asi mismo seleccionaremos el device utilizado a partir de su nombre.
	
	Combo_Device->SetStringSelection (wxString (Nombre_Device_Actual));
	
}

// -----------------------------  Eventos  -------------------------------------

// Implementaremos el evento de Combo Host para modificar la lista de dispositivos actual de nuestra lista y asignar el host actual a nuestro
// reproductor.

void Dialogo::Evento_Combo_Host (wxCommandEvent & Argumentos) {
	
	// Ahora pasaremos a realizar una pre-asignación de un Host nuevo a nuestro reproductor con base a nuestro indice seleccionado.
	
	const PaHostApiIndex Indice_Seleccionado = static_cast <PaHostApiIndex> (Combo_Host->GetSelection ());
	
	// Consultaremos toda la lista de Dispositivo para esté host de igual forma que con la lista de host.
	
	const std::vector <std::string> Lista_Device (std::move (Player->Get_DeviceList (Indice_Seleccionado)));
	
	// Limpiaremos la lista de dispositivos actualmente mostrada.
	
	Combo_Device->Clear ();
	
	// Anexaremos cada elemento para el combobox de nuestros dispositivos.
	
	for (const std::string & Nombre_Device : Lista_Device) {
		
		// Anexaremos el siguiente elemento en el Combobox.
		
		Combo_Device->Append (wxString (Nombre_Device));
		
	}
	
	// Asi mismo seleccionaremos el primer device en la lista.
	
	Combo_Device->SetSelection (0);
	
}

// Implementaremos el botón de aceptar para poder aplicar el cambio de dispositivo de reproducción actual y modificar con estó el reproductor actual.

void Dialogo::Evento_Boton_Aceptar (wxCommandEvent & Argumentos) {
	
	// -----------------------  Asignando Host/Device  -------------------------
	
	// Consultaremos de nueva cuenta el indice de host seleccionado actualmente.
	
	const PaHostApiIndex Indice_Seleccionado = static_cast <PaHostApiIndex> (Combo_Host->GetSelection ());
	
	// Asignaremos el dispositivo actual de nuestra salida de audio usando una conversión de cadena a indice de esta manera.
	
	const PaDeviceIndex Indice_Dispositivo = Player->Get_DeviceByName (Combo_Device->GetStringSelection ().ToStdString ());
	
	// Antes de continuar nos aseguramos que sean diferentes con los indices de dispositivo actual.
	
	if ((Indice_Seleccionado != Player->Get_HostIndex ()) || (Indice_Dispositivo != Player->Get_DeviceIndex ())) {
		
		// ---------------------------  Validando  -----------------------------
		
		// Determinaremos el estado actual del reproductor , para que podamos aplicar el cambio de dispositivo de salida hace falta que se encuentre
		// en estado pausa o stop. Para esto utilizamos lo siguiente.
		
		if (Player->Is_Play ()) {
			
			// Entonces tendremos que detener la reproducción actual para esto utilizaremos un dialogo de YES/NO/CANCEL para continuar.
			
			wxMessageDialog Dialogo_Aceptar (this , wxString ("La reproducción actual se pausara de momento , continuar ?") , wxString ("Reproducción en curso") ,
				(wxYES_NO | wxCANCEL | wxCENTRE));
			
			// Mostraremos el dialogo modal y obtendremos la respuesta.
			
			if (Dialogo_Aceptar.ShowModal () == wxID_YES) {
				
				// ------------------  Estableciendo Index  --------------------
				
				// Ahora pasaremos a seleccionar nuestra API de Host actual y recargar la lista de hosts disponibles para está misma.
				
				Player->Set_Host (Indice_Seleccionado);
				
				// Asignaremos esté dispositivo a nuestro reproductor actual.
				
				Player->Set_Device (Indice_Dispositivo);
				
				// -----------------------  Reset  -----------------------------
				
				// Ahora para terminar vamos a realizar un reset en el reproductor actual.
				
				Reset_Player ();
				
			}
			
		}
		
	}
	
	// -------------------------------------------------------------------------
	
	// Terminaremos el dialogo modal con el codigo de retorno ID_OK.
	
	this->EndModal (wxID_OK);
	
}

// Ahora implementaremos el evento actual para realizar la cancelación del dialogo actual sin cambio alguno.

void Dialogo::Evento_Boton_Cancelar (wxCommandEvent & Argumentos) {
	
	// Terminaremos el dialogo actual con el id de retorno ID_CANCEL.
	
	this->EndModal (wxID_CANCEL);
	
}

// -----------------------------------------------------------------------------

// -------------------------  Reset Definición  --------------------------------

// Implementaremos el metodo para resetear el estado del reproductor y dejar el audio en el tiempo actualmente establecido.

void Dialogo::Reset_Player () {
	
	// Obtendremos el estado del reproductor actual y lo validaremos.
	
	const int Estado_Actual = Player->Get_State ();
	
	// Validamos el estado del reproductor.
	
	if (Estado_Actual != Reproductor::Estado::E_Close) {
		
		// Lo primero será retomar el Filename del archivo de audio abierto actualmente para poderlo reabrir al momento actual.
		
		const std::string Filename_Actual (Player->Get_Filename ());
		
		// Ahora obtendremos el tiempo en segundos del reproductor donde está la song actualmente sonando.
		
		const size_t Tiempo_Actual (Player->Get_ActualTime ());
		
		// ---------------------------------------------------------------------
		
		// Cerraremos nuestro archivo abierto actualmente si es el estado del reproductor y validamos el resultado.
		
		if (Player->Close ()) {
			
			// Continuaremos entonces abriendo nuevamente el archivo de audio con el nuevo host/device seleccionado.
			
			if (Player->Open (Filename_Actual)) {
				
				// Si el reproductor estaba en estado play o pause moveremos entonces el seek al siguiente valor.
				
				if ((Estado_Actual == Reproductor::Estado::E_Play) || (Estado_Actual == Reproductor::Estado::E_Pause)) {
					
					// Moveremos el seek al tiempo actual.
					
					Player->Seek (Tiempo_Actual);
					
				}
				
			}
			else {
				
				// Problemas al abrir el archivo de audio.
				
				wxMessageBox (wxString ("Error al Abrir archivo de audio") , wxString ("Error al Abrir Audio") , (wxOK | wxCENTRE | wxICON_ERROR));
				
			}
			
		}
		else {
			
			// Problemas al cerrar el archivo de audio.
			
			wxMessageBox (wxString ("Error al Cerrar archivo de audio") , wxString ("Error al Cerrar Audio") , (wxOK | wxCENTRE | wxICON_ERROR));
			
		}
		
	}
	
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------
