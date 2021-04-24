
// Implementaremos ahora nuestro codigo para la barra de progreso del media player.

#include <Widgets/ElectrodeProgreso.h>

/// @file ElectrodeProgreso.cpp
/// @brief Definición formal para los metodos del widget de Barra de progreso de tiempo en el media player.
/// @author Menightmar
/// @date Thursday 29/03/2018

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------  ElectrodeProgreso  --------------------------------

// -----------------------------------------------------------------------------

// ---------------------------  Constructor  -----------------------------------

// Implementaremos para comenzar el constructor por defecto sin parametros que inicia solamente algunos elementos en nuestra barra a sus valores
// originales.

ElectrodeProgreso::ElectrodeProgreso () {
	
	// Iniciaremos el fondo del progreso en un nullbitmap.
	
	Fondo_Progreso = wxNullBitmap;
	
	// Ahora un color para el progreso por defecto de color negro.
	
	Color_Progreso = (*wxBLACK);
	
	// Dejaremos seleccionado el fondo con el memory_dc que tenemos.
	
	Contexto_Fondo.SelectObjectAsSource (Fondo_Progreso);
	
}

// Ahora vamos a implementar el constructor formal con los elementos para iniciar el wxWindow.

ElectrodeProgreso::ElectrodeProgreso (wxWindow * Parent , wxWindowID ID , const wxPoint & Position , const wxSize & Size , long Estilo) :
	wxWindow (Parent , ID , Position , Size , Estilo) {
	
	// Iniciaremos el fondo del progreso y el cursor en un nullbitmap.
	
	Cursor_Progreso = Fondo_Progreso = wxNullBitmap;
	
	// Ahora un color para el progreso por defecto de color negro.
	
	Color_Progreso = (*wxBLACK);
	
	// Dejaremos seleccionado el fondo con el memory_dc que tenemos.
	
	Contexto_Fondo.SelectObjectAsSource (Fondo_Progreso);
	
	// Dejaremos seleccionado el mismo bitmap para el cursor.
	
	Contexto_Cursor.SelectObjectAsSource (Cursor_Progreso);
	
	// Crearemos el recuadro de progreso actual utilizando la posición y el tamaño asignado a está ventana.
	
	Recuadro_Progreso = wxRect (Position , Size);
	
	// Colocamos el estado inicial del cursor deacuerdo al estado de nuestro componente.
	
	Cursor_State = (IsEnabled () ? STATE_CURSOR::CURSOR_ENABLE : STATE_CURSOR::CURSOR_DISABLE);
	
}

// ------------------------------  Bitmap  -------------------------------------

// Tenemos implementado el siguiente metodo para asignar un mapa de bits de fondo a nuestra barra de progreso.

void ElectrodeProgreso::Set_Background (const wxBitmap & Fondo) {
	
	// Validamos que el mapa sea valido.
	
	if (Fondo.IsOk ()) {
		
		// Primero deseleccionaremos lo que está actualmente apuntando el Contexto de Dibujo.
		
		Contexto_Fondo.SelectObjectAsSource (wxNullBitmap);
		
		// Asignaremos entonces el nuevo fondo a nuestra propiedad.
		
		Fondo_Progreso = Fondo;
		
		// Finalmente seleccionaremos el nuevo objeto fuente del contexto de fondo.
		
		Contexto_Fondo.SelectObjectAsSource (Fondo_Progreso);
		
	}
	
}

// Definiremos el siguiente metodo para asignar nuestro nuevo bitmap en estado disable de la barra de progreso.

void ElectrodeProgreso::Set_Background_Disable (const wxBitmap & Fondo) {
	
	// Validamos nuevamente que el bitmap está en estado de ok.
	
	if (Fondo.IsOk ()) {
		
		// Asignaremos el nuevo fondo de progreso.
		
		Fondo_Progreso_Disable = Fondo;
		
	}
	
}

// Tenemos ahora la otra implementación para el bitmap a utilizar cuando la barra se está llenando.

void ElectrodeProgreso::Set_Background_Filled (const wxBitmap & Relleno , wxRasterOperationMode Operacion) {
	
	// Validamos nuevamente que el bitmap está en estado de ok.
	
	if (Relleno.IsOk ()) {
		
		// Asignaremos el nuevo fondo de progreso.
		
		Fondo_Progreso_Filled = Relleno;
		
		// Asi como el modo de Operación.
		
		Fondo_Progreso_Operacion = Operacion;
		
	}
	
}

// Implementaremos el siguiente metodo para la asignación de un Bitmap para establecer la zona presionable de la barra.

void ElectrodeProgreso::Set_Background_Line (const wxBitmap & Linea) {
	
	// Validamos en primer lugar que el bitmap sea valido.
	
	if (Linea.IsOk ()) {
		
		// Asignaremos entonces esté bitmap.
		
		Fondo_Progreso_Line = Linea;
		
	}
	
}

// Implementaremos el color de fondo para el relleno en caso de no usar un bitmap.

void ElectrodeProgreso::Set_Progress_Colour (const wxColour & Color_Relleno , wxRasterOperationMode Operacion) {
	
	// Asignaremos el nuevo color de fondo de progreso.
	
	Color_Progreso = Color_Relleno;
	
	// Asi como el modo de Operación.
	
	Fondo_Progreso_Operacion = Operacion;
	
}

// Implementaremos el codigo para colocar el cursor y su mascara respectiva en la barra de progreso.

void ElectrodeProgreso::Set_Cursor (const wxBitmap & Bitmap_Cursor , const wxBitmap & Bitmap_Mascara) {
	
	// Deseleccionaremos de la misma forma que con el fondo el cursor.
	
	Contexto_Cursor.SelectObjectAsSource (wxNullBitmap);
	
	// Colocaremos ahora el bitmap para el cursor de la barra de progreso.
	
	Cursor_Progreso = Bitmap_Cursor;
	
	// Comprobamos que el bitmap del cursor es valido.
	
	if (Bitmap_Cursor.IsOk ()) {
		
		// Validamos si la mascara del cursor es valido.
		
		if (Bitmap_Mascara.IsOk ()) {
			
			// Tenemos una mascara de bits opcional para ocultar parte del bitmap del cursor.
			
			wxMask * Cursor_Mascara;
			
			// Asignaremos el bitmap a la mascara de nuestro cursor.
			
			Cursor_Mascara = new wxMask (Bitmap_Mascara , (*wxBLACK));
			
			// Establecemos la mascara del bitmap.
			
			Cursor_Progreso.SetMask (Cursor_Mascara);
			
		}
		
		// Finalmente seleccionaremos el nuevo bitmap.
		
		Contexto_Cursor.SelectObjectAsSource (Cursor_Progreso);
		
	}
	
}

// Implementamos ahora el bitmap para el cursor cuando la barra está en estado disable.

void ElectrodeProgreso::Set_Cursor_Disable (const wxBitmap & Bitmap_Cursor , const wxBitmap & Bitmap_Mascara) {
	
	// Colocaremos primero el bitmap para el cursor de la barra de progreso.
	
	Cursor_Progreso_Disable = Bitmap_Cursor;
	
	// Comprobamos que el bitmap del cursor es valido.
	
	if (Bitmap_Cursor.IsOk ()) {
		
		// Validamos si la mascara del cursor es valido.
		
		if (Bitmap_Mascara.IsOk ()) {
			
			// Tenemos una mascara de bits opcional para ocultar parte del bitmap del cursor.
			
			wxMask * Cursor_Mascara;
			
			// Asignaremos el bitmap a la mascara de nuestro cursor.
			
			Cursor_Mascara = new wxMask (Bitmap_Mascara , (*wxBLACK));
			
			// Establecemos la mascara del bitmap.
			
			Cursor_Progreso_Disable.SetMask (Cursor_Mascara);
			
		}
		
	}
	
}

// Implementaremos el bitmap para el cursor cuando esté está siendo seleccionado.

void ElectrodeProgreso::Set_Cursor_Selected (const wxBitmap & Bitmap_Cursor , const wxBitmap & Bitmap_Mascara) {
	
	// Colocaremos primero el bitmap para el cursor de la barra de progreso.
	
	Cursor_Progreso_Selected = Bitmap_Cursor;
	
	// Comprobamos que el bitmap del cursor es valido.
	
	if (Bitmap_Cursor.IsOk ()) {
		
		// Validamos si la mascara del cursor es valido.
		
		if (Bitmap_Mascara.IsOk ()) {
			
			// Tenemos una mascara de bits opcional para ocultar parte del bitmap del cursor.
			
			wxMask * Cursor_Mascara;
			
			// Asignaremos el bitmap a la mascara de nuestro cursor.
			
			Cursor_Mascara = new wxMask (Bitmap_Mascara , (*wxBLACK));
			
			// Establecemos la mascara del bitmap.
			
			Cursor_Progreso_Selected.SetMask (Cursor_Mascara);
			
		}
		
	}
	
}

// Implementaremos el bitmap para el cursor cuando esté está siendo presionado.

void ElectrodeProgreso::Set_Cursor_Pressed (const wxBitmap & Bitmap_Cursor , const wxBitmap & Bitmap_Mascara) {
	
	// Colocaremos primero el bitmap para el cursor de la barra de progreso.
	
	Cursor_Progreso_Pressed = Bitmap_Cursor;
	
	// Comprobamos que el bitmap del cursor es valido.
	
	if (Bitmap_Cursor.IsOk ()) {
		
		// Validamos si la mascara del cursor es valido.
		
		if (Bitmap_Mascara.IsOk ()) {
			
			// Tenemos una mascara de bits opcional para ocultar parte del bitmap del cursor.
			
			wxMask * Cursor_Mascara;
			
			// Asignaremos el bitmap a la mascara de nuestro cursor.
			
			Cursor_Mascara = new wxMask (Bitmap_Mascara , (*wxBLACK));
			
			// Establecemos la mascara del bitmap.
			
			Cursor_Progreso_Pressed.SetMask (Cursor_Mascara);
			
		}
		
	}
	
}

// Implementaremos el siguiente metodo para asignar el tamaño del borde del cursor.

void ElectrodeProgreso::Set_Cursor_Border (int Border) {
	
	// Asignaremos el siguiente valor al borde.
	
	Cursor_Border = Border;
	
}

// ---------------------------  State Cursor  ----------------------------------

// Implementaremos el metodo Enable de la clase wxWindow.

bool ElectrodeProgreso::Enable (bool Habilitar) {
	
	// Asignaremos el estado del cursor deacuerdo a esté valor.
	
	Cursor_State = (Habilitar ? STATE_CURSOR::CURSOR_ENABLE : STATE_CURSOR::CURSOR_DISABLE);
	
	// Realizamos la llamada al super metodo con el parametro y devolvemos el resultado.
	
	return wxWindow::Enable (Habilitar);
	
}

// -----------------------------  Valores  -------------------------------------

// Implementaremos ahora el metodo siguiente para la asignación del limite min de la barra de progreso.

ElectrodeProgreso & ElectrodeProgreso::Set_Value_Min (long int Limite_Min) {
	
	// Asignaremos el nuevo valor de limite minimo a la barra.
	
	Progreso_Min = Limite_Min;
	
	// Actualizaremos el valor , si sobrepasa esté limite entonces lo asignaremos a esté valor.
	
	Progreso_Actual = ((Progreso_Actual < Progreso_Min) ? Progreso_Min : Progreso_Actual);
	
	// Actualizamos el recuadro del cursor.
	
	Progreso_Cursor_Rect ();
	
	// Actualizaremos el cursor repintando la zona especifica.
	
	Refresh ();
	
	// Finalmente devolvemos la referencia actual.
	
	return (*this);
	
}

// Implementaremos ahora el metodo siguiente para la asignación del limite max de la barra de progreso.

ElectrodeProgreso & ElectrodeProgreso::Set_Value_Max (long int Limite_Max) {
	
	// Asignaremos el nuevo valor de limite maximo a la barra.
	
	Progreso_Max = Limite_Max;
	
	// Actualizaremos el valor , si sobrepasa esté limite entonces lo asignaremos a esté valor.
	
	Progreso_Actual = ((Progreso_Actual > Progreso_Max) ? Progreso_Max : Progreso_Actual);
	
	// Actualizamos el recuadro del cursor.
	
	Progreso_Cursor_Rect ();
	
	// Actualizaremos el cursor repintando la zona especifica.
	
	Refresh ();
	
	// Finalmente devolvemos la referencia actual.
	
	return (*this);
	
}

// Ahora vamos a implementar el metodo para asignar un valor actual a está barra de progreso y modificar con estó el
// cursor de la misma.

ElectrodeProgreso & ElectrodeProgreso::Set_Value (long int Valor_Actual) {
	
	// Asignaremos el nuevo valor al progreso actual recortando a los limites max y min.
	
	Progreso_Actual = std::max (std::min (Progreso_Max , Valor_Actual) , Progreso_Min);
	
	// Vamos a actualizar la posición del cursor para poderlo localizar en la zona que le corresponda ocupar con base al valor de la barra.
	
	Progreso_Cursor_Position ();
	
	// Actualizaremos el cursor repintando la zona especifica.
	
	Refresh ();
	
	// Finalmente devolvemos la referencia actual.
	
	return (*this);
	
}

// ----------------------------  Destructor  -----------------------------------

// Implementaremos el destructor de nuestra barra de progreso.

ElectrodeProgreso:: ~ ElectrodeProgreso () {
	
	// Liberamos el mapa seleccionado por los contextos para que pueda ser liberado.
	
	Contexto_Fondo.SelectObjectAsSource (wxNullBitmap) , Contexto_Cursor.SelectObjectAsSource (wxNullBitmap);
	
}

// -----------------------------------------------------------------------------

// ----------------------------  Privados  -------------------------------------

// -----------------------------------------------------------------------------

// ------------------------------  Draw  ---------------------------------------

// Implementaremos ahora el metodo para dibujar nuestra barra de progreso de acuerdo al nivel o posición de la misma.

void ElectrodeProgreso::Progreso_Draw (wxPaintEvent & Argumentos) {
	
	// Crearemos ahora nuestro contexto para pintar sobre nuestra ventana.
	
	wxPaintDC Contexto_Ventana (this);
	
	// Estableceremos una pluma transparente para evitar contorneos.
	
	Contexto_Ventana.SetPen (*wxTRANSPARENT_PEN);
	
	// ---------------------------  Changue Cursor  ----------------------------
	
	// Si no estamos en el modo de desplazamiento (left click). Vamos a validar si el mouse está posicionado encima del cursor.
	
	if (!Progreso_Left_Click) {
		
		// ----------------------------  Mouse  --------------------------------
		
		// Ahora vamos a recuperar las coordenadas de cliente del mouse para validar lo siguiente.
		
		const wxPoint Posicion_Actual (ScreenToClient (wxGetMousePosition ()));
		
		// Si el mouse está dentro del area del cursor en estos momentos.
		
		if (Progreso_Cursor_Inside (Posicion_Actual)) {
			
			// Si no estamos seleccionados o presionados entonces.
			
			if (Is_Cursor_Enable ()) {
				
				// Asignaremos el siguiente estado al cursor actual.
				
				Cursor_State = STATE_CURSOR::CURSOR_SELECTED;
				
				// Actualizaremos el cursor de la ventana a uno del tipo hand.
				
				SetCursor (wxCURSOR_HAND);
				
			}
			
		}
		else {
			
			// Si no estamos habilitados entonces.
			
			if (!Is_Cursor_Enable () && IsEnabled ()) {
				
				// Dejaremos el cursor en estado de no seleccionado.
				
				Cursor_State = STATE_CURSOR::CURSOR_ENABLE;
				
				// Actualizaremos el cursor a uno de tipo Normal.
				
				SetCursor (wxCURSOR_ARROW);
				
			}
			
		}
		
	}
	
	// ------------------------------  Fondo  ----------------------------------
	
	// Validamos si el fondo del bitmap está asignado.
	
	if (Fondo_Progreso.IsOk ()) {
		
		/*
		// --------------------  Iterador de Regiones  -------------------------
		
		// Ahora obtenemos un iterador para todas las regiones que necesitan ser repintadas de nuestra ventana y con esto poder dibujar usando
		// el codigo respectivo.
		
		wxRegionIterator Regiones (GetUpdateRegion ());
		
		// LOG_FN (" >> Size : %i , %i \n" , Recuadro_Progreso.width , Recuadro_Progreso.height)
		
		// Iteramos por cada región en forma wxRect que nos devuelve.
		
		while (Regiones) {
			
			// Obtenemos el rectangulo de la región actual.
			
			const wxRect Region_Actual (Regiones.GetRect ());
			
			// Determinaremos entonces el origen en X y en Y , asi como el ancho y alto del mapa a dibujar con base a la siguiente formula :
			//		N_X = X_Ventana / Relacion_X.
			//		N_Y = Y_Ventana / Relacion_Y.
			
			const int Origen_Fondo_X = static_cast <int> (static_cast <float> (Region_Actual.x) / Relacion_Fondo_X);
			const int Origen_Fondo_Y = static_cast <int> (static_cast <float> (Region_Actual.y) / Relacion_Fondo_Y);
			
			// De igual forma para el ancho y alto a ocupar.
			
			const int Origen_Fondo_Ancho = static_cast <int> (static_cast <float> (Region_Actual.width) / Relacion_Fondo_X);
			const int Origen_Fondo_Alto = static_cast <int> (static_cast <float> (Region_Actual.height) / Relacion_Fondo_Y);
			
			// Pintaremos el bitmap de fondo en nuestra ventana ajustandolo.
			
			Contexto_Ventana.StretchBlit (Region_Actual.x , Region_Actual.y , Region_Actual.width , Region_Actual.height , &Contexto_Fondo
				, Origen_Fondo_X , Origen_Fondo_Y , Origen_Fondo_Ancho , Origen_Fondo_Alto);
			
			// Incrementamos el iterador de Regiones en 1.
			
			++ Regiones;
			
		}
		*/
		
		// Si tenemos valido nuestro bitmap disable realizaremos lo siguiente.
		
		if (Fondo_Progreso_Disable.IsOk ()) {
			
			// Seleccionaremos el bitmap que corresponda de acuerdo al estado en la barra de progreso.
			
			Contexto_Fondo.SelectObjectAsSource ((IsEnabled () || Is_Cursor_Enable ()) ? Fondo_Progreso : Fondo_Progreso_Disable);
			
		}
		
		// Dibujaremos el bitmap del fondo acomplandolo al tamaño actual.
		
		Contexto_Ventana.Blit (0 , 0 , Recuadro_Progreso.width , Recuadro_Progreso.height , &Contexto_Fondo , 0 , 0);
		
	}
	else {
		
		// Seleccionaremos la brocha de fondo con el color de fondo de la ventana.
		
		Contexto_Ventana.SetBackground (wxBrush (GetBackgroundColour ()));
		
		// Limpiamos la ventana con esté color.
		
		Contexto_Ventana.Clear ();
		
	}
	
	// -----------------------------  Progreso  --------------------------------
	
	// Dibujaremos a continuación el progreso actual de la barra utilizando el bitmap o el color de relleno para hacerlo.
	
	Progreso_Draw_Progress (Contexto_Ventana);
	
	// ------------------------------  Cursor  ---------------------------------
	
	// Dibujaremos por ultimo el cursor de nuestra barra de progreso para su posición actual , usaremos el siguiente metodo.
	
	Progreso_Draw_Cursor (Contexto_Ventana);
	
}

// Tenemos la implementación del metodo para dibujar el progreso de la barra actual.

void ElectrodeProgreso::Progreso_Draw_Progress (wxPaintDC & Contexto) {
	
	// Colocaremos el modo de operación para el fondo del relleno.
	
	Contexto.SetLogicalFunction (Fondo_Progreso_Operacion);
	
	// Validamos si tenemos el fondo de bitmap para el relleno activo.
	
	if (Fondo_Progreso_Filled.IsOk ()) {
		
		// Seleccionaremos el fondo fill como fuente para el contexto.
		
		Contexto_Fondo.SelectObjectAsSource (Fondo_Progreso_Filled);
		
		// Teniendo en cuenta la posición actual del cursor , tomaremos la mitad y sumaremos estó a su posición en X para determinar el limite a dibujar del fondo
		// de relleno o color ´, según el estado.
		
		const wxCoord Relleno_Limite = (Recuadro_Cursor.x + (Recuadro_Cursor.width / 2));
		
		// Dibujaremos entonces el fondo para el relleno de la progress.
		
		Contexto.Blit (0 , 0 , Relleno_Limite , Recuadro_Progreso.height , &Contexto_Fondo , 0 , 0);
		
		// Dejaremos nuevamente seleccionado el fondo normal.
		
		Contexto_Fondo.SelectObjectAsSource (Fondo_Progreso);
		
	}
	else {
		
		// Determinaremos el valor de Origen para la posición inicial del relleno , tomando la mitad del ancho del cursor.
		
		const wxCoord Relleno_Origen_X = (Cursor_X_Min + (Recuadro_Cursor.width / 2));
		
		// Calcularemos el relleno limite a partir del recuadro del cursor y el valor del Origen en X.
		
		const wxCoord Relleno_Limite_X = Recuadro_Cursor.x;
		
		// Dibujaremos una linea de grosor (Recuadro_Progreso.Alto / 4) para indicar el nivel de progreso actual.
		
		const int Alto_Linea = (Recuadro_Progreso.height / 4);
		
		// Determinamos el rectangulo de la linea de progreso.
		
		const wxRect Rectangle_Progress (wxPoint (Relleno_Origen_X , ((Recuadro_Progreso.height / 2) - (Alto_Linea / 2))) , wxSize (Relleno_Limite_X , Alto_Linea));
		
		// Colocaremos una brocha del color de relleno colocado.
		
		Contexto.SetBrush (Color_Progreso);
		
		// Dibujaremos el rectangulo con este rectangulo definido.
		
		Contexto.DrawRectangle (Rectangle_Progress);
		
	}
	
}

// Definiremos el siguiente metodo para dibujar nuestro cursor dentro de la pantalla actual.

void ElectrodeProgreso::Progreso_Draw_Cursor (wxPaintDC & Contexto) {
	
	// Validaremos si tenemos valido el bitmap del cursor.
	
	if (Cursor_Progreso.IsOk ()) {
		
		// ----------------------------  Cursor  -------------------------------
		
		// Determinaremos el estado actual del cursor para seleccionar el bitmap que corresponda.
		
		switch (Cursor_State) {
			
			// Estado Activo (Default) :
			
			case STATE_CURSOR::CURSOR_ENABLE :
				
				// Seleccionamos el bitmap por defecto.
				
				Contexto_Cursor.SelectObjectAsSource (Cursor_Progreso);
				break;
			
			// Estado Desactivado :
			
			case STATE_CURSOR::CURSOR_DISABLE :
				
				// Seleccionamos el bitmap para el estado desabilitado o el bitmap por defecto si no hay alguno asignado.
				
				Contexto_Cursor.SelectObjectAsSource (Cursor_Progreso_Disable.IsOk () ? Cursor_Progreso_Disable : Cursor_Progreso);
				break;
			
			// Estado Seleccionado :
			
			case STATE_CURSOR::CURSOR_SELECTED :
				
				// Seleccionamos el bitmap para el estado seleccionado o el bitmap por defecto si no hay alguno asignado.
				
				Contexto_Cursor.SelectObjectAsSource (Cursor_Progreso_Selected.IsOk () ? Cursor_Progreso_Selected : Cursor_Progreso);
				break;
			
			// Estado Presionado :
			
			case STATE_CURSOR::CURSOR_PRESSED :
				
				// Seleccionamos el bitmap para el estado presionado o el bitmap por defecto si no hay alguno asignado.
				
				Contexto_Cursor.SelectObjectAsSource (Cursor_Progreso_Pressed.IsOk () ? Cursor_Progreso_Pressed : Cursor_Progreso);
				break;
			
		}
		
		// Pintaremos el bitmap del cursor alargandolo si es necesario al espacio determinado por su recuadro , habilitaremos la mascara asignada.
		
		Contexto.StretchBlit (Recuadro_Cursor.x , Recuadro_Cursor.y , Recuadro_Cursor.width , Recuadro_Cursor.height , &Contexto_Cursor
			, 0 , 0 , Cursor_Progreso.GetWidth () , Cursor_Progreso.GetHeight () , wxCOPY , true);
		
		// Dejaremos seleccionado el cursor por defecto.
		
		Contexto_Cursor.SelectObjectAsSource (Cursor_Progreso);
		
	}
	else {
		
		// ---------------------------  Default  -------------------------------
		
		// Colocaremos una brocha de color blanco para pintar el circulo del cursor.
		
		Contexto.SetBrush (*wxWHITE_BRUSH);
		
		// Determinaremos el origen en X , Y para nuestro circulo de está manera :
		//		X : Cursor.x + (Cursor.Ancho / 2).
		//		Y : Cursor.y + (Cursor.Alto / 2).
		
		const wxCoord Cursor_Origen_X = (Recuadro_Cursor.x + (Recuadro_Cursor.width / 2));
		const wxCoord Cursor_Origen_Y = (Recuadro_Cursor.y + (Recuadro_Cursor.height / 2));
		
		// Dibujaremos el circulo usando los tamaños del recuadro como radio.
		
		Contexto.DrawCircle (Cursor_Origen_X , Cursor_Origen_Y , Recuadro_Cursor.width);
		
	}
	
}

// -----------------------------  Recuadros  -----------------------------------

// Implementaremos el metodo para recalcular el tamaño del recuadro que compone a está barra de progreso.

void ElectrodeProgreso::Progreso_Progress_Rect (const wxSize & Size_Ventana) {
	
	// -----------------------------  Recuadro  --------------------------------
	
	// Antes de realizar el dibujado debemos actualizar nuestro rectangulo de progreso con la posición y tamaño actuales de la ventana.
	
	Recuadro_Progreso = wxRect (GetPosition () , Size_Ventana);
	
	// Determinaremos la posición Origen X , Y de nuestro bitmap donde comenzaremos a dibujar. Usaremos el tamaño original en X y en Y
	// para el mapa.
	
	const int Fondo_Ancho_Origen = Fondo_Progreso.GetWidth ();
	const int Fondo_Alto_Origen = Fondo_Progreso.GetHeight ();
	
	// Actualizaremos la relación que hay entre el ancho y alto origen de nuestro fondo , con rececto al tamaño de nuestra ventana.
	
	Relacion_Fondo_X = (static_cast <float> (Recuadro_Progreso.width) / Fondo_Ancho_Origen);
	Relacion_Fondo_Y = (static_cast <float> (Recuadro_Progreso.height) / Fondo_Alto_Origen);
	
}

// Definiremos el siguiente metodo para poder calcular el tamaño del recuadro del cursor actual y su posición.

void ElectrodeProgreso::Progreso_Cursor_Rect () {
	
	// -------------------------  Recuadro Cursor  -----------------------------
	
	// Calcularemos el ancho y alto del cursor utilizando la formula : Recuadro_Progreso.Alto - Borde.
	
	const wxSize Cursor_Size ((Recuadro_Progreso.height - Cursor_Border) , (Recuadro_Progreso.height - Cursor_Border));
	
	// Determinaremos el min y max de posición en X del cursor usando está formula :
	//		Min : 0.
	//		Max : (Recuadro_Progreso.Ancho - Cursor.Ancho).
	
	Cursor_X_Min = 0;
	Cursor_X_Max = (Recuadro_Progreso.GetWidth () - Cursor_Size.GetWidth ());
	
	// Calcularemos la diferencia de las posiciones X del cursor.
	
	const float Cursor_X_Diferencia = static_cast <float> (Cursor_X_Max - Cursor_X_Min);
	
	// Actualizaremos la cantidad de espacio en pixeles que hay por cada unidad de progreso en la barra.
	
	Progreso_Diff = (Cursor_X_Diferencia / (Progreso_Max - Progreso_Min));
	
	// Calcularemos la posición X , Y de está forma :
	//		X : (Cursor_X_Max - Cursor_X_Min) * ((Valor - Min) / (Max - Min)).
	//		Y : Borde / 2.
	
	// Ahora calcularemos el resultado de la siguiente división de valores.
	
	// const float Cursor_X_Valores = (static_cast <float> (Progreso_Actual - Progreso_Min) / (Progreso_Max - Progreso_Min));
	
	// Calcularemos la nueva posicion X del cursor multiplicando está diferencia con el valor actual de progreso.
	
	const int Cursor_X = std::max (std::min (static_cast <int> (std::nearbyint (Progreso_Diff * Progreso_Actual)) , Cursor_X_Max) , Cursor_X_Min);
	
	// Ahora calculamos la posición Y de está forma.
	
	const int Cursor_Y = static_cast <int> (Cursor_Border / 2);
	
	// Asignaremos el recuadro del cursor.
	
	Recuadro_Cursor = wxRect (wxPoint (Cursor_X , Cursor_Y) , Cursor_Size);
	
}

// Definiremos ahora el metodo para recalcular el recuadro del cursor actualmente posicionado de la barra de progreso.

void ElectrodeProgreso::Progreso_Cursor_Position () {
	
	// --------------------------  Posicion Cursor  ----------------------------
	
	// Tomando en cuenta la diferencia en pixeles de cada valor unitario en la barra de progreso. Calculamos la posicion X actual del cursor con
	// una simple multiplicación y redondeando al valor entero más proximo el resultado.
	
	Recuadro_Cursor.x = std::max (std::min (static_cast <int> (std::nearbyint (Progreso_Diff * Progreso_Actual)) , Cursor_X_Max) , Cursor_X_Min);
	
}

// ------------------------------  Cursor  -------------------------------------

// Definiremos el metodo que permite indicarnos si el mouse está sobre la linea de progreso la cual está ubicada en la linea establecida mediante la
// propiedad de bitmap Linea_Progress.

bool ElectrodeProgreso::Progreso_Line_Inside (const wxPoint & Posicion) {
	
	// Crearemos el resultado siguiente. Por defecto estámos en la zona de efecto.
	
	bool Resultado = true;
	
	// Ahora validaremos si tenemos un Bitmap asignado a la linea de progreso.
	
	if (Fondo_Progreso_Line.IsOk ()) {
		
		// Crearemos un DC para el bitmap de está linea.
		
		wxMemoryDC Contexto_Linea;
		
		// Seleccionamos el objeto fuente sobre esté bitmap actual.
		
		Contexto_Linea.SelectObjectAsSource (Fondo_Progreso_Line);
		
		// Ahora vamos a determinar si el pixel en la posición actual del Mouse corresponde a un pixel blanco o negro.
		
		wxColour Color_Pixel (*wxBLACK);
		
		// Validamos si el pixel es asignado.
		
		if (Contexto_Linea.GetPixel (Posicion.x , Posicion.y , &Color_Pixel)) {
			
			// Si es negro entonces asignamos falso.
			
			Resultado = (Color_Pixel == (*wxWHITE));
			
		}
		
		// Desagsignamos el objeto fuente actual.
		
		Contexto_Linea.SelectObjectAsSource (wxNullBitmap);
		
	}
	
	// Y devolvemos el resultado actual.
	
	return Resultado;
	
}

// Implementaremos el siguiente metodo para determinar si la posición especificada está dentro de los limites del cursor y ademas esta en el area
// blanca en la mascara del bitmap si es el caso.

bool ElectrodeProgreso::Progreso_Cursor_Inside (const wxPoint & Posicion) {
	
	// Empezaremos por validar si la posición está dentro del rectangulo del cursor.
	
	if (Recuadro_Cursor.Contains (Posicion.x , Posicion.y)) {
		
		// Tendremos el siguiente resultado.
		
		bool Resultado = false;
		
		// Determinamos si el bitmap del cursor tiene mascara , si es asi entonces validaremos esté caso.
		
		if (Cursor_Progreso.IsOk ()) {
			
			// Obtenemos la mascara de referencia del bitmap.
			
			wxMask * Mascara_Cursor = Cursor_Progreso.GetMask ();
			
			// Si es diferente de null continuaremos.
			
			if (Mascara_Cursor != nullptr) {
				
				// Obtenemos el bitmap monocromatico de la mascara.
				
				const wxBitmap Bitmap_Mascara_Cursor (Mascara_Cursor->GetBitmap ());
				
				// Determinaremos la relación entre el tamaño original de la mascara y el recuadro del cursor.
				
				const float Relacion_Bitmap_Mascara_X = (static_cast <float> (Recuadro_Cursor.width) / Bitmap_Mascara_Cursor.GetWidth ());
				const float Relacion_Bitmap_Mascara_Y = (static_cast <float> (Recuadro_Cursor.height) / Bitmap_Mascara_Cursor.GetHeight ());
				
				// Crearemos un DC para la mascara del bitmap actual.
				
				wxMemoryDC Contexto_Mascara;
				
				// Seleccionamos el objeto fuente de la mascara.
				
				Contexto_Mascara.SelectObjectAsSource (Bitmap_Mascara_Cursor);
				
				// Convertiremos la posición de coordenadas de la barra entera a una relativa , solo en el area del cursor.
				
				wxPoint Posicion_Relativa (Posicion - Recuadro_Cursor.GetPosition ());
				
				// Ahora a estás posiciones las dividiremos por su relación con el bitmap original.
				
				Posicion_Relativa.x /= Relacion_Bitmap_Mascara_X;
				Posicion_Relativa.y /= Relacion_Bitmap_Mascara_Y;
				
				// Ahora vamos a determinar si el pixel en está posición corresponde a un pixel blanco o negro.
				
				wxColour Color_Pixel (*wxBLACK);
				
				// Validamos si el pixel es asignado.
				
				if (Contexto_Mascara.GetPixel (Posicion_Relativa.x , Posicion_Relativa.y , &Color_Pixel)) {
					
					// Si es negro entonces asignamos falso.
					
					Resultado = (Color_Pixel == (*wxWHITE));
					
				}
				
				// Desagsignamos el objeto fuente.
				
				Contexto_Mascara.SelectObjectAsSource (wxNullBitmap);
				
			}
			else {
				
				// Entonces con el simple hecho de estar en el area del cursor es valido.
				
				Resultado = true;
				
			}
			
		}
		
		// Devolvemos el resultado.
		
		return Resultado;
		
	}
	
	// Devolvemos falso.
	
	return false;
	
}

// Definiremos ahora el metodo para realizar las acciones que toman lugar cuando el mouse se posiciona dentro de la barra de progreso.

void ElectrodeProgreso::Progreso_Mouse_Inside (wxMouseEvent & Argumentos , bool Line_Inside , bool Cursor_Inside) {
	
	// Si el mouse está entrando , saliendo o entrando a la zona del cursor de la ventana entonces.
	
	if (Argumentos.Moving () || Argumentos.Entering () || Argumentos.Leaving ()) {
		
		// ----------------------  Enter  ------------------------------
		
		// Si el mouse está siendo movido por la barra de progreso o entrando/saliendo entonces validaremos estó.
		
		if (Cursor_Inside) {
			
			// Si no estamos seleccionados entonces.
			
			if (!Is_Cursor_Selected ()) {
				
				// Asignaremos el siguiente estado al cursor actual.
				
				Cursor_State = STATE_CURSOR::CURSOR_SELECTED;
				
				// Actualizaremos el cursor de la ventana a uno del tipo hand.
				
				SetCursor (wxCURSOR_HAND);
				
				// Refrescaremos entonces el area del cursor.
				
				RefreshRect (Recuadro_Cursor);
				
			}
			
		}
		else {
			
			// Si no estamos habilitados entonces.
			
			if (!Is_Cursor_Enable ()) {
				
				// Dejaremos el cursor en estado de no seleccionado.
				
				Cursor_State = STATE_CURSOR::CURSOR_ENABLE;
				
				// Actualizaremos el cursor a uno de tipo Normal.
				
				SetCursor (wxCURSOR_ARROW);
				
				// Refrescaremos entonces el area del cursor.
				
				RefreshRect (Recuadro_Cursor);
				
			}
			
			// Ademas si estamos dentro de la zona del cursor entonces el cursor cambia.
			
			if (Line_Inside) {
				
				// Si estamos en la zona entonces modificaremos el cursor.
				
				SetCursor (wxCURSOR_HAND);
				
				// Y ademas asignaremos el valor en true.
				
				Progreso_Line_Mouse = true;
				
			}
			else {
				
				// Dejaremos el cursor por defecto.
				
				SetCursor (wxCURSOR_ARROW);
				
				// Dejaremos el valor del cursor en falso.
				
				Progreso_Line_Mouse = false;
				
			}
			
		}
		
	}
	else if (Argumentos.LeftDown ()) {
		
		// Si el mouse tiene el click izquierdo presionado sobre el cursor o sobre la linea de progreso entonces pasará lo siguiente.
		
		if (Cursor_Inside || Line_Inside) {
			
			// Activaremos el modo de selección con left click para poder mover el cursor libremente.
			
			Progreso_Left_Click = true;
			
			// Colocaremos el cursor en estado presionado.
			
			Cursor_State = STATE_CURSOR::CURSOR_PRESSED;
			
			// Usaremos el metodo siguiente para desplazar el cursor a la posición del mouse. Habilitaremos la captura del mouse.
			
			Progreso_Seek_Event (Argumentos) , CaptureMouse ();
			
			// Refrescamos el recuadro del cursor.
			
			RefreshRect (Recuadro_Cursor);
			
			// --------------------  Evento Cursor Down  -----------------------
			
			// Lanzaremos un evento de tipo wxEVT_ELECTRODE_PROGRESS_CURSOR_DOWN con los argumentos siguientes.
			
			ElectrodeProgresoEvent Argumentos_Evento_Down (true , Progreso_Actual , wxEVT_ELECTRODE_PROGRESS_CURSOR_DOWN , Argumentos.GetId ());
			
			// Procesamos esté evento.
			
			HandleWindowEvent (Argumentos_Evento_Down);
			
		}
		
	}
	
}

// Por otro lado tenemos el metodo para cuando el mouse presiona el cursor de la barra de progreso y realiza cambios en su posición.

void ElectrodeProgreso::Progreso_Mouse_Pressed (wxMouseEvent & Argumentos , bool Line_Inside , bool Cursor_Inside) {
	
	// Validamos ahora si estamos arrastrando el cursor con el mouse.
	
	if (Argumentos.Dragging ()) {
		
		// Procesaremos el evento de seek en esté caso.
		
		Progreso_Seek_Event (Argumentos);
		
		// -------------------------  Evento Slider  ---------------------------
		
		// Lanzaremos ahora un evento de tipo wxEVT_ELECTRODE_PROGRESS_SLIDER para notificar los cambios en la barra de progreso.
		
		ElectrodeProgresoEvent Argumentos_Evento_Slider (true , Progreso_Actual , wxEVT_ELECTRODE_PROGRESS_SLIDER , Argumentos.GetId ());
		
		// Procesamos esté evento.
		
		HandleWindowEvent (Argumentos_Evento_Slider);
		
	}
	else if (Argumentos.LeftUp ()) {
		
		// Colocaremos en falso ese estado.
		
		Progreso_Left_Click = false;
		
		// Validamos si el cursor sigue dentro de la zona.
		
		if (!Cursor_Inside) {
			
			// Dejaremos el cursor en estado de no seleccionado.
			
			Cursor_State = STATE_CURSOR::CURSOR_ENABLE;
			
			// Actualizaremos el cursor a uno de tipo Normal.
			
			SetCursor (wxCURSOR_ARROW);
			
		}
		else {
			
			// Dejaremos el cursor en estado de seleccionado.
			
			Cursor_State = STATE_CURSOR::CURSOR_SELECTED;
			
		}
		
		// Refrescamos la barra y liberamos la captura externa del Mouse.
		
		Refresh () , ReleaseMouse ();
		
		// -----------------------  Evento Cursor Up  --------------------------
		
		// Lanzaremos ahora un evento de tipo wxEVT_ELECTRODE_PROGRESS_CURSOR_UP para notificar.
		
		ElectrodeProgresoEvent Argumentos_Evento_Up (false , Progreso_Actual , wxEVT_ELECTRODE_PROGRESS_CURSOR_UP , Argumentos.GetId ());
		
		// Procesamos esté evento.
		
		HandleWindowEvent (Argumentos_Evento_Up);
		
		// ------------------------  Evento Changued  --------------------------
		
		// Lanzaremos otro evento de tipo wxEVT_ELECTRODE_PROGRESS_CHANGUED para notificar esté ultimo cambio en la barra de progreso.
		
		ElectrodeProgresoEvent Argumentos_Evento_Changued (true , Progreso_Actual , wxEVT_ELECTRODE_PROGRESS_CHANGUED , Argumentos.GetId ());
		
		// Procesamos esté evento tambien.
		
		HandleWindowEvent (Argumentos_Evento_Changued);
		
	}
	
}

// -----------------------------  Eventos  -------------------------------------

// Comenzaremos implementando el codigo para el evento size que actualiza los recuadros para la barra y el cursor.

void ElectrodeProgreso::Progreso_Size_Event (wxSizeEvent & Argumentos) {
	
	// Actualizaremos primero el recuadro de la barra de progreso con base al tamaño especificado en los argumentos.
	
	Progreso_Progress_Rect (Argumentos.GetSize ());
	
	// Ahora recalcularemos el recuadro del cursor con el valor actual de la barra.
	
	Progreso_Cursor_Rect ();
	
	// Y saltaremos el evento para seguir el procesamiento.
	
	Argumentos.Skip ();
	
}

// Implementaremos el evento de mouse para poder cambiar los estados de la barra de progreso asi como desplazar la linea de tiempo a determinados
// valores.

void ElectrodeProgreso::Progreso_Mouse_Event (wxMouseEvent & Argumentos) {
	
	// Validamos si la barra de progreso está habilitada.
	
	if (IsEnabled ()) {
		
		// Empezaremos determinando si el mouse está posicionado dentro del area de la linea de progreso.
		
		const bool Line_Inside = Progreso_Line_Inside (Argumentos.GetPosition ());
		
		// Determinamos si el cursor del mouse está dentro o fuera del area del cursor de la barra.
		
		const bool Cursor_Inside = Progreso_Cursor_Inside (Argumentos.GetPosition ());
		
		// Lo siguiente será saber si el mouse está presionando el botón izquierdo para ocasionar otro tipo de eventos.
		
		if (Progreso_Left_Click) {
			
			// -----------------------  Mouse Pressed  -------------------------
			
			// Si el mouse está presionando el cursor , entonces estaremos ocasionando eventos de arrastrar y soltar la posición.
			
			Progreso_Mouse_Pressed (Argumentos , Line_Inside , Cursor_Inside);
			
		}
		else {
			
			// ----------------------- Mouse Inside  ---------------------------
			
			// En otro caso estáremos atendiendo eventos de mouse para modificar los estados del mismo.
			
			Progreso_Mouse_Inside (Argumentos , Line_Inside , Cursor_Inside);
			
		}
		
	}
	else {
		
		// Dejamos el cursor en estado de disable.
		
		Cursor_State = STATE_CURSOR::CURSOR_DISABLE;
		
		// Dejamos el estado de presionado en falso tambien.
		
		Progreso_Left_Click = false;
		
	}
	
}

// Implementaremos el evento de Seek de la barra de progreso actual.

void ElectrodeProgreso::Progreso_Seek_Event (wxMouseEvent & Argumentos) {
	
	// Obtenemos la posición actual del mouse.
	
	const wxPoint Posicion_Actual (Argumentos.GetPosition ());
	
	// Calcularemos la nueva posición en X , lo asignaremos limitando al intervalo de min y max sobre X y esta a su vez sera el nuevo punto de partida.
	
	Progreso_Left_X = std::max (std::min ((Posicion_Actual.x - (Recuadro_Cursor.width / 2)) , Cursor_X_Max) , Cursor_X_Min);
	
	// Validaremos si esté valor es distinto al que tenemos en nuestro recuadro.
	
	if (Recuadro_Cursor.x != Progreso_Left_X) {
		
		// Determinaremos la posición float para poder calcular el nuevo valor del progreso.
		
		const float Nueva_Posicion_X = static_cast <float> (Progreso_Left_X);
		
		/*
		
		// printf (" >> Min : %i , Max : %i , Pos X : %.2f \n" , Cursor_X_Min , Cursor_X_Max , Nueva_Posicion_X);
		
		// Determinaremos entonces la posición del valor respectivo al cual hay que mover la barra actual. Usaremos la siguiente formula :
		//		Valor = ((X / (Cursor_X_Max - Cursor_X_Min)) * (Max - Min) + Min)
		
		const float Valor_Nuevo = (Nueva_Posicion_X / Progreso_Diff);
		
		// printf (" >> Diff : %.2f , Valor_Nuevo : %.2f \n" , Progreso_Diff , Valor_Nuevo);
		
		// Calcularemos primero el valor de cociente que hay entre la posicion actual en X y la diferencia de los cursores.
		
		const float Valor_X_Actual = (Nueva_Posicion_X / (Cursor_X_Max - Cursor_X_Min));
		
		// Ahora continuaremos con el valor de la diferencia de los progresos.
		
		const float Valor_X_Diferencia = (Progreso_Max - Progreso_Min);
		
		// Finalmente calcularemos el valor que corresponda , acercando al valor entero proximo.
		
		// long int Nuevo_Valor = static_cast <long int> (std::nearbyint ((Valor_X_Actual * Valor_X_Diferencia) + Progreso_Min));
		
		long int Nuevo_Valor = static_cast <long int> (std::nearbyint (Valor_Nuevo));
		
		// long int Nuevo_Valor = static_cast <long int> (std::nearbyint (Nueva_Posicion_X / Progreso_Diff));
		
		// Recortaremos (clamp) esté valor a los limites de la barra.
		
		Nuevo_Valor = std::max (std::min (Progreso_Max , Nuevo_Valor) , Progreso_Min);
		
		*/
		
		// Calcularemos el nuevo valor de la barra de progreso con base a la posición del cursor. Dividiremos este valor con la diferencia
		// de posiciones entre cada valor y el resultado lo redondearemos al entero mas proximo.
		
		const long int Nuevo_Valor = std::max (std::min (static_cast <long int> (std::nearbyint (Nueva_Posicion_X / Progreso_Diff)) , Progreso_Max) , Progreso_Min);
		
		// Validamos si es diferente que el valor actual.
		
		if (Nuevo_Valor != Progreso_Actual) {
			
			// Asignaremos la nueva posición en X al recuadro del cursor.
			
			// Recuadro_Cursor.x = Progreso_Left_X;
			
			// const int Posicion_X_Cursor = std::max (std::min (static_cast <int> (Progreso_Diff * Nuevo_Valor) , Cursor_X_Max) , Cursor_X_Min);
			
			// printf (" X : %i \n" , Posicion_X_Cursor);
			
			// Recuadro_Cursor.x = Posicion_X_Cursor;
			
			// Asignaremos esté nuevo valor a la barra.
			
			Progreso_Actual = Nuevo_Valor;
			
			// Recargaremos la posicion actual de nuestro cursor.
			
			Progreso_Cursor_Position ();
			
			// Refrescamos la ventana.
			
			Refresh ();
			
		}
		
	}
	
}

// Ahora vamos a implementar la tabla de eventos asociada a nuestra clase.

BEGIN_EVENT_TABLE (ElectrodeProgreso , wxWindow)
	
	// Asignaremos primero el evento Size sobre nuestra barra de progreso.
	
	EVT_SIZE (ElectrodeProgreso::Progreso_Size_Event)
	
	// Asignaremos el evento Mouse Enter sobre nuestro metodo actual.
	
	EVT_ENTER_WINDOW (ElectrodeProgreso::Progreso_Mouse_Event)
	
	// Asi mismo tenemos el evento Mouse Leave sobre este elemento.
	
	EVT_LEAVE_WINDOW (ElectrodeProgreso::Progreso_Mouse_Event)
	
	// Activaremos el evento Mouse Left Down para esté metodo.
	
	EVT_LEFT_DOWN (ElectrodeProgreso::Progreso_Mouse_Event)
	
	// Asi mismo tenemos el evento Mouse Left Up para esté mismo metodo.
	
	EVT_LEFT_UP (ElectrodeProgreso::Progreso_Mouse_Event)
	
	// Ahora el evento Motión para el mouse.
	
	EVT_MOTION (ElectrodeProgreso::Progreso_Mouse_Event)
	
	// Implementaremos el evento Draw para nuestra barra de progreso.
	
	EVT_PAINT (ElectrodeProgreso::Progreso_Draw)
	
END_EVENT_TABLE ()
	
// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------  Evento Progress  --------------------------------

// -----------------------------------------------------------------------------

// ----------------------------  Constructor  ----------------------------------

// Implementaremos ahora el constructor por defecto para nuestra clase del Evento de la barra de progreso.

ElectrodeProgresoEvent::ElectrodeProgresoEvent () : wxCommandEvent (wxEVT_NULL) {
	
	// Dejaremos los valores por defecto para el estado.
	
	Progreso_Click = false;
	
	// Y el valor en 0 para la barra de progreso.
	
	Progreso_Valor = 0;
	
}

// Definiremos el constructor con los datos para nuestra barra de progreso y el tipo de evento lanzado.

ElectrodeProgresoEvent::ElectrodeProgresoEvent (bool Progreso_Estado_Click , long int Progreso_Valor_Actual , wxEventType Progreso_Tipo_Evento , int Progreso_ID) :
	wxCommandEvent (Progreso_Tipo_Evento , Progreso_ID) {
	
	// Asignaremos el valor para el estado del progreso.
	
	Progreso_Click = Progreso_Estado_Click;
	
	// Asi como el valor de la barra de progreso.
	
	Progreso_Valor = Progreso_Valor_Actual;
	
}

// -----------------------------  Accesores  -----------------------------------

// Definiremos el metodo para asignar un valor al estado del progreso actual.

void ElectrodeProgresoEvent::Set_Progress_State (bool Progreso_Estado_Click) {
	
	// Asignaremos esté valor a nuestro estado.
	
	Progreso_Click = Progreso_Estado_Click;
	
}

// Y el metodo para asignar un valor a la barra de progreso en el evento.

void ElectrodeProgresoEvent::Set_Progress_Value (long int Progreso_Valor_Actual) {
	
	// Asignaremos esté valor a nuestra propiedad.
	
	Progreso_Valor = Progreso_Valor_Actual;
	
}

// ----------------------------  Definición  -----------------------------------

// Definiremos ya solamente los tres tipos de Eventos que tiene la barra de progreso.

wxDEFINE_EVENT (wxEVT_ELECTRODE_PROGRESS_CURSOR_DOWN , ElectrodeProgresoEvent);
wxDEFINE_EVENT (wxEVT_ELECTRODE_PROGRESS_CURSOR_UP , ElectrodeProgresoEvent);
wxDEFINE_EVENT (wxEVT_ELECTRODE_PROGRESS_SLIDER , ElectrodeProgresoEvent);
wxDEFINE_EVENT (wxEVT_ELECTRODE_PROGRESS_CHANGUED , ElectrodeProgresoEvent);

// ----------------------------------------------------------------------------------------------------------------------------------------------------------
