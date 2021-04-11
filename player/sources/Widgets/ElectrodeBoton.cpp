
// Implementaremos ahora nuestro codigo para el botón personalizado del media player.

#include <Widgets/ElectrodeBoton.h>

/// @file ElectrodeBoton.cpp
/// @brief Definición formal para los metodos del widget de Botón Personalizado en el media player.
/// @author Menightmar
/// @date Tuesday 10/04/2018

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------  ElectrodeBoton  ---------------------------------

// -----------------------------------------------------------------------------

// ---------------------------  Constructor  -----------------------------------

// Implementaremos el constructor vacio por defecto que tenemos en el botón.

ElectrodeBoton::ElectrodeBoton () {
	
	// Dejaremos el recuadro del botón iniciado con sus valores por defecto.
	
	Recuadro_Boton = wxRect (wxDefaultPosition , wxDefaultSize);
	
	// Y asignaremos un nullbitmap al mapa de la zona habil.
	
	Zona_Boton = wxNullBitmap;
	
}

// Definiremos ahora nuestro constructor completo con todos los elementos del botón de bitmap del cual deriva. Iniciaremos asi mismo los
// elementos internos del botón personalizado.

ElectrodeBoton::ElectrodeBoton (wxWindow * Parent , wxWindowID ID , const wxBitmap & Mapa, const wxPoint & Position , const wxSize & Size , long Estilo) :
	wxBitmapButton (Parent , ID , Mapa , Position , Size , Estilo) {
	
	// Asignaremos el recuadro de progreso con los valores iniciales para nuestró botón actual.
	
	Recuadro_Boton = wxRect (Position , Size);
	
	// Asignaremos un mapa null a la zona habil.
	
	Zona_Boton = wxNullBitmap;
	
}

// -----------------------------------------------------------------------------

// ------------------------------  Zona  ---------------------------------------

// Implementaremos el siguiente metodo para colocar un mapa de zona habil a nuestro botón personalizado.

void ElectrodeBoton::Set_Enable_Zone (const wxBitmap & Zona) {
	
	// Asignaremso el mapa especificado a la zona habil de nuestro botón.
	
	Zona_Boton = Zona;
	
}

// -----------------------------------------------------------------------------

// ----------------------------  Privados  -------------------------------------

// -----------------------------------------------------------------------------

// -----------------------------  Mouse  ---------------------------------------

// Definiremos el siguiente metodo para determinar si la posición relativa del cursor está dentro de la zona habil para esté botón.

bool ElectrodeBoton::Boton_Mouse_Inside (const wxPoint & Posicion) {
	
	// Lo primero será validar que tengamos un mapa de zona valido.
	/*
	if (Zona_Boton.IsOk ()) {
		
		// Declaramos un resultado por defecto en true.
		
		bool Resultado = true;
		
		// Crearemos un DC para el bitmap de está zona.
		
		wxMemoryDC Contexto_Zona;
		
		// Seleccionamos el objeto fuente sobre esté bitmap.
		
		Contexto_Zona.SelectObjectAsSource (Zona_Boton);
		
		// Ahora vamos a determinar si el pixel en la posición actual del Mouse corresponde a un pixel blanco o negro.
		
		wxColour Color_Pixel (*wxBLACK);
		
		// Validamos si el pixel es asignado.
		
		if (Contexto_Zona.GetPixel (Posicion.x , Posicion.y , &Color_Pixel)) {
			
			// Si es negro entonces asignamos falso.
			
			Resultado = (Color_Pixel == (*wxWHITE));
			
		}
		
		// Desagsignamos el objeto fuente.
		
		Contexto_Zona.SelectObjectAsSource (wxNullBitmap);
		
		return Resultado;
		
	}
	*/
	// Entonces devolvemos true por defecto.
	
	return true;
	
}

// -----------------------------  Eventos  -------------------------------------

// Ahora vamos a definir el siguiente metodo para poder atender el evento Click de nuestro botón , esté mismo sera solamente aplicable si
// el mouse se encuentra en la zona valida de nuestro botón.

void ElectrodeBoton::Boton_Click_Event (wxCommandEvent & Argumentos) {
	
	// Para continuar procesando el evento necesitamos entonces validar el cursor del mouse está dentro de la zona valida.
	
	const wxPoint Posicion_Actual (ScreenToClient (wxGetMousePosition ()));
	
	// Validamos si está posición está dentro.
	
	if (Boton_Mouse_Inside (Posicion_Actual)) {
		
		// Colocaremos el Skip en el argumento.
		
		Argumentos.Skip ();
		
	}
	
}

// Implementaremos ahora el siguiente metodo para poder atender los eventos de mouse en el botón personalizado.

void ElectrodeBoton::Boton_Mouse_Event (wxMouseEvent & Argumentos) {
	
	// Validamos si está posición está dentro.
	
	if (Boton_Mouse_Inside (Argumentos.GetPosition ())) {
		
		// Colocaremos el Skip en el argumento.
		
		Argumentos.Skip ();
		
	}
	
}

// Ahora vamos a definir la tabla de eventos para esté botón.

BEGIN_EVENT_TABLE (ElectrodeBoton , wxBitmapButton)
	
	// Asignaremos primero el evento Size sobre nuestra barra de progreso.
	
	// EVT_SIZE (ElectrodeProgreso::Progreso_Size_Event)
	
	// Asignaremos el evento Mouse Enter sobre nuestro metodo actual.
	
	EVT_ENTER_WINDOW (ElectrodeBoton::Boton_Mouse_Event)
	
	// Asi mismo tenemos el evento Mouse Leave sobre este elemento.
	
	EVT_LEAVE_WINDOW (ElectrodeBoton::Boton_Mouse_Event)
	
	// Activaremos el evento Mouse Left Down para esté metodo.
	
	EVT_LEFT_DOWN (ElectrodeBoton::Boton_Mouse_Event)
	
	// Asi mismo tenemos el evento Mouse Left Up para esté mismo metodo.
	
	EVT_LEFT_UP (ElectrodeBoton::Boton_Mouse_Event)
	
	// Ahora el evento Motión para el mouse.
	
	EVT_MOTION (ElectrodeBoton::Boton_Mouse_Event)
	
	// Implementaremos el evento Draw para nuestra barra de progreso.
	
	// EVT_PAINT (ElectrodeProgreso::Progreso_Draw)
	
	// EVT_BUTTON (wxID_ANY , ElectrodeBoton::Boton_Click_Event)
	
END_EVENT_TABLE ()

// ----------------------------------------------------------------------------------------------------------------------------------------------------------
