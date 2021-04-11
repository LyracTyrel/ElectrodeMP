
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
	
	// Recuperaremos el nombre de nuestro dispositivo actualmente utilizado con el siguiente metodo.
	
	const std::string Nombre_Device_Actual (Player->Get_DeviceName (Player->Get_DeviceIndex ()));
	
	// Limpiaremos la lista de dispositivos actualmente mostrada.
	
	Combo_Device->Clear ();
	
	// Anexaremos cada elemento para el combobox de nuestros dispositivos.
	
	for (const std::string & Nombre_Device : Lista_Device) {
		
		// Anexaremos el siguiente elemento en el Combobox.
		
		Combo_Device->Append (wxString (Nombre_Device));
		
	}
	
	// Asi mismo seleccionaremos el device utilizado a partir de su nombre.
	
	Combo_Device->SetStringSelection (wxString (Nombre_Device_Actual));
	
}

// Implementaremos el botón de aceptar para poder aplicar el cambio de dispositivo de reproducción actual y modificar con estó el reproductor.

void Dialogo::Evento_Boton_Aceptar (wxCommandEvent & Argumentos) {
	
	// Consultaremos de nueva cuenta el indice de host seleccionado.
	
	const PaHostApiIndex Indice_Seleccionado = static_cast <PaHostApiIndex> (Combo_Host->GetSelection ());
	
	// Asignaremos el dispositivo actual de nuestra salida de audio usando una conversión de cadena a indice de esta manera.
	
	const PaDeviceIndex Indice_Dispositivo = Player->Get_DeviceByName (Combo_Device->GetStringSelection ().ToStdString ());
	
	// Ahora pasaremos a seleccionar nuestra API de Host y recargar la lista de hosts disponibles para está misma.
	
	Player->Set_Host (Indice_Seleccionado);
	
	// Asignaremos esté dispositivo a nuestro reproductor.
	
	Player->Set_Device (Indice_Dispositivo);
	
	// Terminaremos el dialogo modal con el codigo de retorno ID_OK.
	
	this->EndModal (wxID_OK);
	
}

// Ahora implementaremos el evento actual para realizar la cancelación del dialogo actual sin cambio alguno.

void Dialogo::Evento_Boton_Cancelar (wxCommandEvent & Argumentos) {
	
	// Terminaremos el dialogo actual con el id de retorno ID_CANCEL.
	
	this->EndModal (wxID_CANCEL);
	
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------
