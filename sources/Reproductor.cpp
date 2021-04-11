
// Implementaremos ahora nuestro codigo para el Reproductor.

#include <ElectrodeMP/Reproductor.h>

/// @file Reproductor.cpp
/// @brief Definición formal para los metodos de nuestro Reproductor hecho con PortAudio.
/// @author Loubiner
/// @date Friday 16/02/2018

// Utilizaremos el espacio de nombres del Proyecto.

using namespace ElectrodeMP;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// ---------------------------  Reproductor  -----------------------------------

// -----------------------------------------------------------------------------

// --------------------------  Estado Global  ----------------------------------

// Declaramos dos localidades las cuales nos permitiran determinar si nuestro reproductor fue inicializado de manera correcta para poder utilizar los elementos
// de la biblioteca.

// Declaramos el estado de inicialización inicialmente en falso.

bool Reproductor::Inicializado = false;

// Ahora inicializamos el contador de referencias con un valor inicial de 0.

int Reproductor::Referencias = 0;

// -----------------------------------------------------------------------------

// ---------------------------  Constructor  -----------------------------------

// Empezaremos de igual forma con el constructor de nuestra clase el cual inicializa elementos importantes para la biblioteca de portaudio asi como algunos
// valores por defecto para las propiedades.

Reproductor::Reproductor () {
	
	// Lo primero a realizar es la inicialización de nuestra biblioteca de PortAudio , como ya sabemos tenemos el siguiente resultado para poder mostrar
	// mediante la consola.
	
	// Iniciamos nuestro reproductor (biblioteca portaudio) si todavia no lo está.
	
	if (!Is_Initialized ()) {
		
		// Iniciamos la biblioteca y validamos el resultado.
		
		if (!Init ()) {
			
			// Devolvemos la llamada.
			
			return;
			
		}
		
	}
	
	// Intecrementamos el contador de referencias a portaudio.
	
	++ Referencias;
	
	// Ahora mostraremos el siguiente mensaje por la consola.
	
	LOG (" >> PortAudio Inicializado ")
	
	// Validamos cuantos Host's Tenemos en estos momentos.
	
	if (Get_HostCount () <= 0) {
		
		// Mostramos el siguiente mensaje de error.
		
		LOG_E (" >> No Hay Host's Api para Reproducir Audio ")
		
		// Terminamos este metodo.
		
		return;
		
	}
	
	// Obtendremos y asignaremos ahora nuestro Host API por defecto , el cual tiene el siguiente valor.
	
	Indice_Host = Pa_GetDefaultHostApi ();
	
	// Ahora obtendremos la Info para nuestro Host actual por defecto.
	
	const PaHostApiInfo * Informacion_Host = Pa_GetHostApiInfo (Indice_Host);
	
	// Mostramos el siguiente mensaje.
	
	LOG_F (" >> Host Api Asignado : %i [ %s ] (Default) " , Indice_Host , Informacion_Host->name)
	
	// Con está información es posible ahora determinar cual es el valor para nuestro dispositivo de salida por defecto y asignarlo a nuestro indice tambien.
	
	if (Informacion_Host->defaultOutputDevice == paNoDevice) {
		
		// Mostramos el siguiente mensaje de error.
		
		LOG_E (" >> No Hay Devices para Salida de Audio ")
		
		// Terminamos este metodo.
		
		return;
		
	}
	
	// Asignaremos el Device para utilizar.
	
	Indice_Device = Informacion_Host->defaultOutputDevice;
	
	// De igual forma recuperamos la info del device por defecto para presentar su nombre por consola.
	
	const PaDeviceInfo * Informacion_Device = Pa_GetDeviceInfo (Indice_Device);
	
	// Finalmente mostramos este mensaje.
	
	LOG_F (" >> Output Device Asignado : %i [ %s ] (Default) \n" , Indice_Device , Informacion_Device->name)
	
	// Asignamos el valor de Valido a true.
	
	Valido = true;
	
	// Y colocaremos un estado de indeterminado de manera inicial.
	
	Estado_Actual.store (0);
	
	// Mensaje de Aviso para indicar que el reproductor se inicio.
	
	LOG_N (" ::::::::::::::::::::::  Reproductor Creado  ::::::::::::::::::::::: \n")
	
}

// -----------------------------------------------------------------------------

// --------------------------  Init y Terminate  -------------------------------

// Implementaremos el metodo init para inicializar la biblioteca portaudio y con esto comenzar a utilizar sus funciones.

bool Reproductor::Init () {
	
	// Utilizaremos la siguiente llamada para iniciar la biblioteca.
	
	PaError Resultado = Pa_Initialize ();
	
	// Validamos si fue inicializada correctamente.
	
	if (Resultado != paNoError) {
		
		// Error al inicializar PortAudio , mostraremos el siguiente mensaje.
		
		LOG_FE (" >> Error al Inicializar PortAudio : %s " , Pa_GetErrorText (Resultado))
		
		// Terminamos este metodo devolviendo falso.
		
		return false;
		
	}
	
	// Asignamos el estado actual.
	
	Inicializado = true;
	
	// Devolvemos true.
	
	return true;
	
}

// Ahora el metodo contrario , el metodo que finaliza los elementos internos de la biblioteca portaudio de manera segura.

bool Reproductor::Terminate () {
	
	// Por el contrario que con la inicialización , terminaremos la biblioteca invocando a nuestra siguiente función.
	
	PaError Resultado = Pa_Terminate ();
	
	// Validamos si fue finalizado correctamente.
	
	if (Resultado != paNoError) {
		
		// Error al terminar PortAudio , mostraremos el siguiente mensaje.
		
		LOG_FE (" >> Error al Finalizar PortAudio : %s " , Pa_GetErrorText (Resultado))
		
		// Terminamos este metodo devolviendo falso.
		
		return false;
		
	}
	
	// Actualizamos el estado global de reproductor.
	
	Inicializado = false;
	
	// Y devolvemos true.
	
	return true;
	
}

// -----------------------------------------------------------------------------

// -----------------------------  Accesores  -----------------------------------

// Definiremos el metodo para obtener una lista de Nombres de Host's que tenemos en nuestro Sistema Actualmente.

std::vector <std::string> Reproductor::Get_HostList () const {
	
	// Obtendremos la cantidad de Host's Actuales en nuestro dispositivo.
	
	const PaHostApiIndex Cantidad_Hosts = Get_HostCount ();
	
	// Ahora vamos a crear un vector de cadenas las cuales llenaremos con el Nombre de Cada Host.
	
	std::vector <std::string> Lista_Hosts;
	
	// Validamos si tenemos al menos un Host.
	
	if (Cantidad_Hosts > 0) {
		
		// Ahora lo que haremos será iterar por cada Host y obtener su Info para poder recuperar el nombre.
		
		for (PaHostApiIndex Indice = 0 ; Indice < Cantidad_Hosts ; ++ Indice) {
			
			// Obtenemos la Info para este Host.
			
			const PaHostApiInfo * Informacion_Host = Pa_GetHostApiInfo (Indice);
			
			// Ahora asignamos el nombre a nuestra Lista de Hosts.
			
			Lista_Hosts.push_back (std::move (std::string (Informacion_Host->name)));
			
		}
		
	}
	
	// Devolvemos la lista de Hosts.
	
	return std::move (Lista_Hosts);
	
}

// Implementaremos el metodo que recupera un nombre de dispositivo a partir de su valor actual.

std::string Reproductor::Get_HostName (PaHostApiIndex Indice_H) const {
	
	// Primero obtendremos la Info para nuestro Host a consultar.
	
	const PaHostApiInfo * Informacion_Host = Pa_GetHostApiInfo (Indice_H);
	
	// Validamos si tenemos valido el host , de ser asi devolvemos el nombre , o una cadena vacia.
	
	return ((Informacion_Host != nullptr) ? std::string (Informacion_Host->name) : std::string ());
	
}

// Definiremos el metodo para consultar el Indice de Host a partir de su nombre.

PaHostApiIndex Reproductor::Get_HostByName (const std::string & Nombre_H) const {
	
	// Recuperaremos la lista de Host's en el S.O para poder buscar el que necesitamos.
	
	const std::vector <std::string> Lista_Hosts (std::move (Get_HostList ()));
	
	// Tenemos el siguiente valor de Host's Api para iniciar.
	
	PaHostApiIndex Indice_Host_Busqueda = -1;
	
	// Iteramos por cada elemento en la cadena de Nombres y pasaremos a validar si corresponde con el nombre buscado.
	
	for (size_t Indice = 0u ; Indice < Lista_Hosts.size () ; ++ Indice) {
		
		// Comparamos las cadenas de los nombres.
		
		if (Lista_Hosts [Indice] == Nombre_H) {
			
			// Asignamos el valor respectivo de indice.
			
			Indice_Host_Busqueda = static_cast <PaHostApiIndex> (Indice);
			
			// Terminamos el bucle.
			
			break;
			
		}
		
	}
	
	// Devolvemos el resultado.
	
	return Indice_Host_Busqueda;
	
}

// Definiremos el metodo para obtener la cantidad de devices de salida en nuestro S.O.

int Reproductor::Get_DeviceCount () const {
	
	// Primero obtenemos la cantidad de dispositivos en el S.O con la siguiente llamada.
	
        const PaDeviceIndex Cantidad_Devices = Pa_GetDeviceCount ();
	
	// Ahora validaremos cuantos de ellos són de salida usando un contador para acumular el resultado.
	
	int Cantidad_Outputs = 0;
	
	// Iteramos por cada Device y obtenemos sus Informaciones.
	
	for (PaDeviceIndex Indice = 0 ; Indice < Cantidad_Devices ; ++ Indice) {
		
		// Obtenemos la Info de este Device.
		
		const PaDeviceInfo * Informacion_Device = Pa_GetDeviceInfo (Indice);
		
		// Validamos si el dispositivo actual es de salida. Comprobamos si los canales de salida maximos son mayores a 0.
		
		if (Informacion_Device->maxOutputChannels > 0) {
			
			// Incrementamos la cantidad en 1.
			
			++ Cantidad_Outputs;
			
		}
		
	}
	
	// Devolvemos el resultado.
	
	return Cantidad_Outputs;
	
}

// Definiremos está sobrecarga de la anterior función para cuando el Parametro tiene el indice de host.

int Reproductor::Get_DeviceCount (PaHostApiIndex Indice_H) const {
	
	// Primero validaremos cuantos de ellos són de salida usando un contador para acumular el resultado.
	
	int Cantidad_Outputs = 0;
	
	// Lo siguiente es validar que nuestro Indice esté en los limites establecidos.
	
	if ((Indice_H >= 0) && (Indice_H < Get_HostCount ())) {
		
		// Ahora obtenemos la cantidad de dispositivos en el Host actual usando su Info->deviceCount.
		
		const int Cantidad_Devices = Pa_GetHostApiInfo (Indice_H)->deviceCount;
		
		// Iteramos por cada Device y obtenemos sus Informaciones.
		
		for (int Indice = 0 ; Indice < Cantidad_Devices ; ++ Indice) {
			
			// Obtenemos la Info de este Device. Convertimos el valor a PaDeviceIndex con la función especifica.
			
			const PaDeviceInfo * Informacion_Device = Pa_GetDeviceInfo (Pa_HostApiDeviceIndexToDeviceIndex (Indice_H , Indice));
			
			// Validamos si el dispositivo actual es de salida. Comprobamos si los canales de salida maximos son mayores a 0.
			
			if (Informacion_Device->maxOutputChannels > 0) {
				
				// Incrementamos la cantidad en 1.
				
				++ Cantidad_Outputs;
				
			}
			
		}
		
	}
	
	// Devolvemos el resultado.
	
	return Cantidad_Outputs;
	
}

// El siguiente metodo crea y devuelve una lista de Nombres de dispositivos de salida en nuestro S.O.

std::vector <std::string> Reproductor::Get_DeviceList () const {
	
	// Obtendremos la cantidad de Devices Actuales en nuestro S.O.
	
	const PaDeviceIndex Cantidad_Devices = Pa_GetDeviceCount ();
	
	// Ahora vamos a crear un vector de cadenas las cuales llenaremos con el Nombre de Cada Device.
	
	std::vector <std::string> Lista_Devices;
	
	// Validamos si hay devices en nuestro sistema.
	
	if (Cantidad_Devices > 0) {
		
		// Ahora lo que haremos será iterar por cada Device y obtener su Info para poder recuperar el nombre.
		
		for (PaDeviceIndex Indice = 0 ; Indice < Cantidad_Devices ; ++ Indice) {
			
			// Obtenemos la Info para este Device.
			
			const PaDeviceInfo * Informacion_Device = Pa_GetDeviceInfo (Indice);
			
			// Validamos que el Device en cuestión sea de salida contando el numero de canales en el mismo.
			
			if (Informacion_Device->maxOutputChannels > 0) {
				
				// Ahora asignamos el nombre a nuestra Lista de Devices.
				
				Lista_Devices.push_back (std::move (std::string (Informacion_Device->name)));
				
			}
			
		}
		
	}
	
	// Devolvemos la lista de Devices.
	
	return std::move (Lista_Devices);
	
}

// De igual forma que con el host , podemos consultar el nombre de un dispositivo directamente.

std::string Reproductor::Get_DeviceName (PaDeviceIndex Indice_D) const {
	
	// Obtenemos la Info para este Device usando el valor de nuestro parametro.
	
	const PaDeviceInfo * Informacion_Device = Pa_GetDeviceInfo (Indice_D);
	
	// Devolvemos la cadena con el nombre de este dispositivo , si no existe , entonces devolvemos una cadena vacia.
	
	return ((Informacion_Device != nullptr) ? std::string (Informacion_Device->name) : std::string ());
	
}

// Ahora lo mismo pero para cuando especificamos un Indice de Host el cual considerar.

std::vector <std::string> Reproductor::Get_DeviceList (PaHostApiIndex Indice_H) const {
	
	// Vamos a crear un vector de cadenas las cuales llenaremos con el Nombre de Cada Device.
	
	std::vector <std::string> Lista_Devices;
	
	// Lo siguiente es validar que nuestro Indice esté en los limites establecidos.
	
	if ((Indice_H >= 0) && (Indice_H < Get_HostCount ())) {
		
		// Obtendremos la cantidad de Devices para el Host especifico.
		
		const int Cantidad_Devices = Pa_GetHostApiInfo (Indice_H)->deviceCount;
		
		// Validamos si hay devices para este Host.
		
		if (Cantidad_Devices > 0) {
			
			// Ahora lo que haremos será iterar por cada Device asociado a este Host y obtener su Info para poder recuperar el nombre.
			
			for (int Indice = 0 ; Indice < Cantidad_Devices ; ++ Indice) {
				
				// Obtenemos la Info para este Device usando la conversión de por medio.
				
				const PaDeviceInfo * Informacion_Device = Pa_GetDeviceInfo (Pa_HostApiDeviceIndexToDeviceIndex (Indice_H , Indice));
				
				// Validamos que el Device en cuestión sea de salida contando el numero de canales en el mismo.
				
				if (Informacion_Device->maxOutputChannels > 0) {
					
					// Ahora asignamos el nombre a nuestra Lista de Devices.
					
					Lista_Devices.push_back (std::move (std::string (Informacion_Device->name)));
					
				}
				
			}
			
		}
		
	}
	
	// Devolvemos la lista de Devices.
	
	return std::move (Lista_Devices);
	
}

// Ahora para solicitar el nombre de un dispositivo con el Indice de host y el indice local de dispositivo.

std::string Reproductor::Get_DeviceName (PaHostApiIndex Indice_H , int Indice) const {
	
	// Obtenemos la Info para este Device usando la conversión de por medio.
	
	const PaDeviceInfo * Informacion_Device = Pa_GetDeviceInfo (Pa_HostApiDeviceIndexToDeviceIndex (Indice_H , Indice));
	
	// Devolvemos la cadena con el nombre de este dispositivo , si no existe , entonces devolvemos una cadena vacia.
	
	return ((Informacion_Device != nullptr) ? std::string (Informacion_Device->name) : std::string ());
	
}

// Ahora el metodo para recuperar un PaDeviceIndex a partir de un nombre en cuestión.

PaDeviceIndex Reproductor::Get_DeviceByName (const std::string & Nombre_D) const {
	
	// Obtendremos la cantidad de Devices Actuales en nuestro S.O.
	
	const PaDeviceIndex Cantidad_Devices = Pa_GetDeviceCount ();
	
	// Tenemos el siguiente valor de Device para iniciar.
	
	PaDeviceIndex Indice_Device_Busqueda = -1;
	
	// Validamos si hay devices en el sistema operativo.
	
	if (Cantidad_Devices > 0) {
		
		// Ahora lo que haremos será iterar por cada Device y obtener su Info para poder recuperar el nombre.
		
		for (PaDeviceIndex Indice = 0 ; Indice < Cantidad_Devices ; ++ Indice) {
			
			// Obtenemos la Info para este Device.
			
			const PaDeviceInfo * Informacion_Device = Pa_GetDeviceInfo (Indice);
			
			// Comparamos las cadenas de los nombres.
			
			if (Nombre_D == std::string (Informacion_Device->name)) {
				
				// Asignamos el valor respectivo de indice.
				
				Indice_Device_Busqueda = Indice;
				
				// Terminamos el bucle.
				
				break;
				
			}
			
		}
		
	}
	
	// Devolvemos el resultado.
	
	return Indice_Device_Busqueda;
	
}

// -----------------------------------------------------------------------------

// -----------------------------  Setters  -------------------------------------

// Definiremos ahora el metodo para colocar un nuevo Host a nuestro dispositivo.

void Reproductor::Set_Host (PaHostApiIndex Indice_H) {
	
	// Antes de continuar comprobamos que el Indice sea menor a Get_HostCount ().
	
	if ((Indice_H < 0) || (Indice_H >= Get_HostCount ())) {
		
		// Error , el indice no es correcto.
		
		LOG_E (" >> El Indice de Host es Incorrecto ")
		
		// Terminamos este metodo.
		
		return;
		
	}
	
	// Asignaremos el valor en PaHostApiIndex a nuestro reproductor actual.
	
	Indice_Host = Indice_H;
	
	// Ahora obtendremos la Info para nuestro Host actual por defecto.
	
	const PaHostApiInfo * Informacion_Host = Pa_GetHostApiInfo (Indice_Host);
	
	// Mostramos el siguiente mensaje.
	
	LOG_F (" >> Host Api Asignado : %i [ %s ] (Default) " , Indice_Host , Informacion_Host->name)
	
	// Con está información es posible ahora determinar cual es el valor para nuestro dispositivo de salida por defecto y asignarlo a nuestro indice tambien.
	
	if (Informacion_Host->defaultOutputDevice == paNoDevice) {
		
		// Mostramos el siguiente mensaje de error.
		
		LOG_E (" >> No Hay Devices para Salida de Audio ")
		
		// Colocaremos el estado en falso.
		
		Valido = false;
		
		// Terminamos este metodo.
		
		return;
		
	}
	
	// Asignaremos el Device de nuestro Reproductor;
	
	Indice_Device = Informacion_Host->defaultOutputDevice;
	
	// De igual forma recuperamos la info del device por defecto para presentar su nombre por consola.
	
	const PaDeviceInfo * Informacion_Device = Pa_GetDeviceInfo (Indice_Device);
	
	// Finalmente mostramos este mensaje.
	
	LOG_F (" >> Output Device Asignado : %i [ %s ] (Default) " , Indice_Device , Informacion_Device->name)
	
	// El estado cambia o se mantiene en true.
	
	Valido = true;
	
}

// Tenemos el siguiente metodo para colocar un nuevo device en nuestro reproductor.

void Reproductor::Set_Device (PaDeviceIndex Indice_D) {
	
	// Antes de continuar comprobamos que el Indice sea menor al numero de Devices actuales.
	
	if ((Indice_D < 0) || (Indice_D >= Pa_GetDeviceCount ())) {
		
		// Error , el indice no es correcto.
		
		LOG_E (" >> El Indice de Device es Incorrecto ")
		
		// Terminamos este metodo.
		
		return;
		
	}
	
	// Asignaremos nuestro nuevo indice de Device.
	
	Indice_Device = Indice_D;
	
	// De igual forma recuperamos la info del device actual para determinar el nombre.
	
	const PaDeviceInfo * Informacion_Device = Pa_GetDeviceInfo (Indice_Device);
	
	// Validamos antes de continuar que nuestro Device sea de salida.
	
	if (Informacion_Device->maxOutputChannels <= 0) {
		
		// Tenemos este mensaje de aviso.
		
		LOG_E (" >> Dispositivo de Salida no Valido ")
		
		// Colocaremos el estado en falso.
		
		Valido = false;
		
		// Terminamos este metodo.
		
		return;
		
	}
	
	// Finalmente mostramos este mensaje.
	
	LOG_F (" >> Output Device Asignado : %i [ %s ] (Default) " , Indice_Device , Informacion_Device->name)
	
	// El estado cambia o se mantiene en true.
	
	Valido = true;
	
}

// Asignaremos ahora un host y device en el mismo reproductor usando el siguiente metodo.

void Reproductor::Set_Device (PaHostApiIndex Indice_H , int Indice) {
	
	// Antes de continuar comprobamos que el Indice sea menor a Get_HostCount ().
	
	if (Indice_H >= Get_HostCount ()) {
		
		// Error , el indice no es correcto.
		
		LOG_E (" >> El Indice de Host es Incorrecto ")
		
		// Terminamos este metodo.
		
		return;
		
	}
	
	// Asignaremos el valor en PaHostApiIndex a nuestro reproductor actual.
	
	Indice_Host = Indice_H;
	
	// Ahora obtendremos la Info para nuestro Host actual por defecto.
	
	const PaHostApiInfo * Informacion_Host = Pa_GetHostApiInfo (Indice_Host);
	
	// Mostramos el siguiente mensaje.
	
	LOG_F (" >> Host Api Asignado : %i [ %s ] (Default) " , Indice_Host , Informacion_Host->name)
	
	// Validamos ahora que el Indice de Dispositivo no supere la cantidad de devices de nuestro host.
	
	if ((Indice < 0) || (Indice >= Informacion_Host->deviceCount)) {
		
		// Mostramos el siguiente mensaje de error.
		
		LOG_E (" >> El Indice de Device para Host es Incorrecto ")
		
		// Colocaremos el estado en falso.
		
		Valido = false;
		
		// Terminamos este metodo.
		
		return;
		
	}
	
	// Asignaremos nuestro nuevo indice de Device realizando la correcta conversión.
	
	Indice_Device = Pa_HostApiDeviceIndexToDeviceIndex (Indice_Host , Indice);
	
	// De igual forma recuperamos la info del device por defecto para presentar su nombre por consola.
	
	const PaDeviceInfo * Informacion_Device = Pa_GetDeviceInfo (Indice_Device);
	
	// Validamos antes de continuar que nuestro Device sea de salida.
	
	if (Informacion_Device->maxOutputChannels <= 0) {
		
		// Tenemos este mensaje de aviso.
		
		LOG_E (" >> Dispositivo de Salida no Valido ")
		
		// Colocaremos el estado en falso.
		
		Valido = false;
		
		// Terminamos este metodo.
		
		return;
		
	}
	
	// Finalmente mostramos este mensaje.
	
	LOG_F (" >> Output Device Asignado : %i [ %s ] (Default) " , Indice_Device , Informacion_Device->name)
	
	// El estado cambia o se mantiene en true.
	
	Valido = true;
	
}

// -----------------------------------------------------------------------------

// ---------------------------  Reproducción  ----------------------------------

// Definiremos el metodo Open el cual realiza la acción de abrir un archivo de audio para crear el flujo de audio e inicializar la parte del
// demuxer para nuestro decoder , este metodo devuelve true en caso de que no haya ningún tipo de error.

bool Reproductor::Open (const std::string & Filename) {
	
	// Antes de continuar necesitamos validar que nuestro Reproductor tenga un estado valido y nuestro dispositivo de Salida tenga un maximo
	// de canales superior a 0. Para esto tenemos la siguiente validación.
	
	if (Is_Valid () && (Is_Close () || (Estado_Actual.load () == 0)) && Is_Output (Indice_Device)) {
		
		// Asignaremos el Filename de referencia.
		
		Filename_Audio = Filename;
		
		// Registraremos una nueva entrada de audio en el LOG.
		
		LOG (" >>>> Entrada Nueva ... \n\n ----------------------------------------------------------------------------- \n")
		
		// ------------------------------  Tag  --------------------------------
		
		// Comenzaremos con la parte relativa al Etiquetador. Abriremos el archivo nuevamente utilizando el etiquetador para poder leer los metadatos
		// en la entrada y con esto poder generar la Etiqueta del Archivo Actual de audio. Utilizaremos un thread adicional para poder ejecutar todas las
		// acciones pertinentes mientras buscamos la información del stream y demuxeamos por otro lado.
		
		std::future <bool> Generador_Etiqueta = std::async ([this] () -> bool {
			
			// En el metodo asyncrono invocaremos el metodo Open del Etiquetador para poder abrir el archivo y leer toda la información respectiva ,
			// posteriormente si no hay errores entonces pasaremos a generar una Etiqueta.
			
			if (!Tager.Open (Filename_Audio)) {
				
				// Mostraremos el siguiente mensaje de aviso de que no fue posible determinar el tipo de Etiqueta para el archivo actual.
				
				LOG_W (" >> No se pudo Detectar Etiqueta en el Archivo ")
				
				// Asignaremos de cualquier forma una Etiqueta vacia.
				
				Etiqueta_Audio = std::move (Etiqueta ());
				
				// Y devolvemos falso.
				
				return false;
				
			}
			
			// Asignaremos la Etiqueta Generada con todos los datos encontrados colocados.
			
			Etiqueta_Audio = std::move (Tager.Generate ());
			
			// Mostraremos el siguiente mensaje de aviso.
			
			LOG (" >> Etiqueta Asignada Correctamente ")
			
			// Y devolvemos true.
			
			return true;
			
		});
		
		// Retomaremos el resultado posteriormente en la siguiente localidad.
		
		bool Resultado_Etiqueta = false;
		
		// --------------------------  Demuxer  --------------------------------
		
		// El primer paso es determinar los datos de nuestro archivo de Audio actual , para esto necesitamos demuxear el archivo , esté proceso
		// consiste en la separación de todos los flujos que componen al formato de audio comprimido actual en flujos de datos por tipo :
		//		Audio , Texto , Video , Imagen , etc.
		// Nuestro decoder tiene el metodo especifico para realizar dicho proceso y necesitamos utilizarlo para determinar la información
		// de nuestro flujo de audio a reproducir. Enviaremos como argumento el valor 0 especificando que queremos demuxear usando el primer
		// flujo de audio en el archivo actual.
		
		// Comprobamos el resultado para poder continuar.
		
		if (!Decoder.Demuxer (Filename)) {
			
			// Mostraremos el siguiente mensaje de Error por la consola Log.
			
			LOG_E (" >> No se pudo Demuxear el Archivo ")
			
			// Asignaremos los datos de la Etiqueta Generados.
			
			Resultado_Etiqueta = Generador_Etiqueta.get ();
			
			// Devolvemos falso.
			
			return false;
			
		}
		
		// Mostramos el siguiente aviso por la consola LOG.
		
		LOG (" >> Archivo Demuxeado Correctamente ")
		
		// Pasaremos a actualizar el estado del Reproductor a Close (Archivo Cerrado).
		
		Estado_Actual.store (E_Close);
		
		// Ahora pasaremos a verificar cuantos streams de audio tenemos en esté archivo. Usaremos el metodo respectivo para comprobarlo y
		// si tenemos unicamente 1 solo stream de audio , pasaremos a abrirlo inmediatamente.
		
		// Abriremos el stream 0 de Audio y obtendremos el resultado para poderlo validar.
		
		const bool Resultado_Stream = ((Get_StreamCount () == 1u) ? Open_Selection () : true);
		
		// Esperaremos a que los datos de la Etiqueta sean generados y los asignaremos.
		
		Resultado_Etiqueta = Generador_Etiqueta.get ();
		
		// Y finalmente devolvemos el resultado obtenido del stream.
		
		return Resultado_Stream;
		
	}
	
	// Mostraremos el siguiente mensaje de error por nuestra consola LOG.
	
	LOG_W (" >> No se puede Abrir el Archivo , Reproductor No Valido o Device No Aceptable ")
	
	// Devolvemos true.
	
	return true;
	
}

// Implementaremos el metodo para abrir el stream seleccionado de entre todos los posibles. Es necesario invocar esté metodo antes de comenzar la reproducción
// de nuestro audio para poder indicarle al decoder que valores de packet considerar.

bool Reproductor::Open_Selection (size_t Indice) {
	
	// Validamos que Estado sea Cerrado y que el Indice no sea mayor o igual a nuestro contador de streams totales.
	
	if (Is_Close () && (Indice < Get_StreamCount ())) {
		
		// ---------------------------  Open Stream  ---------------------------
		
		// Realizaremos la apertura de nuestro stream indicado y validaremos el resultado obtenido con el parametro del indice.
		
		if (!Open_Stream (Indice)) {
			
			// Mostraremos el siguiente mensaje de error.
			
			LOG_E (" >> Stream de Audio No Abierto ")
			
			// Devolvemos falso.
			
			return false;
			
		}
		
		// Modificaremos el estado de nuestro reproductor actual.
		
		Estado_Actual.store (E_Open);
		
		// Y devolveremos true.
		
		return true;
		
	}
	
	// Mostraremos un mensaje de aviso diciendo lo siguiente.
	
	LOG_W (" >> Stream de Audio No Valido / Stream Abierto Actualmente ")
	
	// Devolvemos true.
	
	return true;
	
}

// Ahora implementaremos el metodo Play el cual como su nombre lo indica inicializa el reproductor y con esto la decodificación de nuestro audio para poder
// escuchar por nuestro dispositivo de salida.

bool Reproductor::Play () {
	
	// ---------------------------  Reiniciando  -------------------------------
	
	// Antes de empezar comprobaremos si el estádo de nuestro reproductor anteriormente fue el de completado ó interrumpido.
	
	if (Decoder.Samples.Is_Complete () || Decoder.Samples.Is_Interrupt ()) {
		
		// Detendremos entonces nuestro stream completamente antes de comenzar una nueva reproducción.
		
		Stop_Stream ();
		
		// Asignaremos el estado de Stop.
		
		Estado_Actual.store (E_Stop);
		
	}
	
	// ---------------------------  Play Stream  -------------------------------
	
	// Comprobaremos que estamos en estado Pause , Stop u Open para poder continuar.
	
	if (Is_Pause () || Is_Stop () || Is_Open ()) {
		
		// Iniciaremos nuestro stream internamente y validaremos el resultado.
		
		if (!Play_Stream ()) {
			
			// Mostramos el siguiente mensaje log.
			
			LOG_E (" >> Error al Iniciar Stream ")
			
			// Devolvemos false.
			
			return false;
			
		}
		
		// Cambiaremos el estado a Play.
		
		Estado_Actual.store (E_Play);
		
		// Mostraremos un mensaje LOG.
		
		LOG_N (" >> Stream Iniciado Correctamente ")
		
		// Y devolvemos true.
		
		return true;
		
	}
	
	// Mostraremos un mensaje de aviso con lo siguiente.
	
	LOG_W (" >> No se Puede Iniciar el Stream en este Estado ")
	
	// Devolvemos false.
	
	return false;
	
}

// Ahora tenemos el metodo Pause el cual detiene por el momento la reproducción y el decodificador , manteniendo el estado de Indice de muestras en la que
// se quedo actualmente nuestro reproductor.

bool Reproductor::Pause () {
	
	// --------------------------  Pause Stream  -------------------------------
	
	// Comprobaremos que estamos en estado Play para poder continuar.
	
	if (Is_Play ()) {
		
		// Pausaremos nuestro stream internamente y validaremos el resultado.
		
		if (!Pause_Stream ()) {
			
			// Mostramos el siguiente mensaje log.
			
			LOG_E (" >> Error al Pausar Stream ")
			
			// Devolvemos false.
			
			return false;
			
		}
		
		// Cambiaremos el estado a Pause.
		
		Estado_Actual.store (E_Pause);
		
		// Mostraremos un mensaje LOG.
		
		LOG_N (" >> Stream Pausado Correctamente ")
		
		// Y devolvemos true.
		
		return true;
		
	}
	
	// Mostraremos un mensaje de aviso con lo siguiente.
	
	LOG_W (" >> No se Puede Pausar el Stream en este Estado ")
	
	// Devolvemos false.
	
	return false;
	
}

// Ahora tenemos el metodo Stop que detiene por completo el flujo de audio y reestablece el indice de muestras procesadas a su valor inicial para que
// se vuelva a reproducir desde el origen.

bool Reproductor::Stop () {
	
	// ---------------------------  Stop Stream  -------------------------------
	
	// Comprobaremos que estamos en estado Play ó Pause para poder continuar.
	
	if (Is_Play () || Is_Pause ()) {
		
		// De manera muy parecida que con pause , detendremos nuestro stream internamente.
		
		if (!Stop_Stream ()) {
			
			// Mostramos el siguiente mensaje log.
			
			LOG_E (" >> Error al Detener Stream ")
			
			// Devolvemos false.
			
			return false;
			
		}
		
		// Cambiaremos el estado a Stop.
		
		Estado_Actual.store (E_Stop);
		
		// Mostraremos un mensaje LOG.
		
		LOG_N (" >> Stream Detenido Correctamente ")
		
		// Y devolvemos true.
		
		return true;
		
	}
	
	// Mostraremos un mensaje de aviso con lo siguiente.
	
	LOG_W (" >> No se Puede Detener el Stream en este Estado ")
	
	// Devolvemos false.
	
	return false;
	
}

// Definiremos finalmente el metodo close el cual realiza la acción de cerrar y liberar los recursos ocupados por portaudio para poder finalizar nuestro archivo
// y esperar a que se abra un nuevo archivo de audio.

bool Reproductor::Close () {
	
	// --------------------------  Close Stream  -------------------------------
	
	// Validamos primero si el estado actual es diferente a Close.
	
	if (!Is_Close ()) {
		
		// Vamos a cerrar nuestro stream de manera ordinaria si tenemos abierto alguno (Estado distinto a indeterminado) y con estó liberar todos los recursos.
		
		if ((Estado_Actual.load () != 0) && !Close_Stream ()) {
			
			// Mostramos el siguiente mensaje log.
			
			LOG_E (" >> Error al Cerrar Stream ")
			
			// Devolvemos falso.
			
			return false;
			
		}
		
		// Ahora cerraremos de igual manera la Etiqueta de Datos.
		
		Tager.Close ();
		
		// Cambiaremos el estado a Close.
		
		Estado_Actual.store (E_Close);
		
		// Mostraremos un mensaje LOG.
		
		LOG_N (" >> Stream Cerrado Correctamente ")
		
		// Registraremos la salida del audio.
		
		LOG ("\n ----------------------------------------------------------------------------- \n")
		
		// Y devolvemos true.
		
		return true;
		
	}
	
	// Mostraremos un mensaje de aviso con lo siguiente.
	
	LOG_W (" >> No se Puede Cerrar el Stream en este Estado ")
	
	// Devolvemos false.
	
	return false;
	
}

// Definiremos ahora nuestro metodo para poder realizar un desplazamiento sobre nuestro audio actualmente procesando y decodificando , tenemos el metodo
// para poder modificar la posición actual de nuestro decodificador y continuar el audio desde cierta posición especificada en segundos.

bool Reproductor::Seek (size_t Second) {
	
	// --------------------------  Desplazando  --------------------------------
	
	// Validamos que tengamos nuestro reproductor en cualquiera de los 4 estados basicos distintos a close.
	
	if (!Is_Close ()) {
		
		// Si el estado actual es play , pausaremos termporalmente el audio para no interferir en los threads.
		
		if (Is_Play () || (Pa_IsStreamActive (Audio) > 0) || Decoder.Samples.Is_Complete () || Decoder.Samples.Is_Interrupt ()) {
			
			// Pausaremos normalmente nuestro stream actual.
			
			Pause_Stream ();
			
		}
		
		// Eliminaremos nuestra cola de Muestras actuales para no reproducir lo anteriormente decodificado.
		
		Decoder.Samples.Clear_Queue ();
		
		// Moveremos nuestro stream a determinada posición , el decoder realizara la actividad actual y desplazara el frame a determinado segundo especificado
		// por nuestro parametro.
		
		Decoder.Seek (Second);
		
		// Finalmente si el stream está en estado play todavia , lo reanudaremos.
		
		if (Is_Play ()) {
			
			// Reanudamos el sonido.
			
			Play_Stream ();
			
		}
		
		// Mostraremos un mensaje LOG.
		
		LOG_FN (" >> Stream Desplazado al Segundo : %i Correctamente " , Second)
		
		// Y devolvemos true.
		
		return true;
		
	}
	
	// Mostraremos un mensaje de aviso con lo siguiente.
	
	LOG_W (" >> No se Puede Desplazar el Stream en este Estado ")
	
	// Devolvemos false.
	
	return false;
	
}

// ---------------------------  Destructor  ------------------------------------

// Implementaremos el destructor de nuestra clase para poder realizar el cierre de nuestro archivo de audio si es ese el caso.

Reproductor::~ Reproductor () {
	
	// Validamos primero si el estado actual es diferente a Close.
	
	if (!Is_Close ()) {
		
		// Vamos a cerrar nuestro stream de manera ordinaria y con estó liberar todos los recursos.
		
		Close_Stream ();
		
	}
	
	// Si esta inicializado portaudio y el numero de referencias a port audio es igual a 1 , entonces continuaremos con lo siguiente.
	
	if (Is_Initialized () && (Referencias == 1)) {
		
		// Ahora finalizaremos la biblioteca portaudio.
		
		Terminate ();
		
	}
	
	// Decrementaremos el contador de referencias en 1.
	
	-- Referencias;
	
}

// -----------------------------------------------------------------------------

// ----------------------------  Privados  -------------------------------------

// Implementaremos el metodo para validar si un device especifico es de salida o de entrada.

bool Reproductor::Is_Output (PaDeviceIndex Indice_D) const {
	
	// Obtenemos la información para nuestro indice de Device Actual.
	
	const PaDeviceInfo * Informacion_Device = Pa_GetDeviceInfo (Indice_D);
	
	// Devolvemos true si los canales de salida són mayores a 0.
	
	return (Informacion_Device->maxOutputChannels > 0);
	
}

// -----------------------------------------------------------------------------

// ----------------------------  Streaming  ------------------------------------

// Implementaremos los metodos para realizar todas las acciones con la biblioteca de PortAudio para poder abrir , reproducir , pausar ,
// detener y cerrar un flujo de audio.

// Ahora implementaremos el metodo para la creación y apertura de un nuevo stream para la reproducción del audio determinado por nuestro
// decoder , asignaremos los valores iniciales a todos los campos necesarios.

bool Reproductor::Open_Stream (size_t Indice) {
	
	// ----------------------  Selecionando Stream  ----------------------------
	
	// Como primer paso tenemos que realizar la selección de stream en el decodificador actual , estó permitira filtrar a todos los packets
	// que soló nos interesán y por ende inicializar el contextó para el decodificador que descomprime el audio seleccionado.
	
	if (!Decoder.Select_Stream (Indice)) {
		
		// Error al seleccionar y abrir decodificador para el flujo respectivo , mostramos el mensaje siguiente.
		
		LOG_E (" >> Error al Seleccionar Stream ")
		
		// Devolvemos falso.
		
		return false;
		
	}
	
	// -----------------------  Creando Parametros  ----------------------------
	
	// El siguiente paso consiste en la creación y comprobación de los Parametros para nuestro flujo a crear e inicializar , usaremos la
	// estructura PaStreamParameters para poder colocar la información de nuestro flujo de audio demuxeado por el Decoder.
	
	// Declaramos la estructura con los parametros vacios por el momento.
	
	PaStreamParameters Parametros_Audio;
	
	// Nuestra estructura cuenta con los siguientes campos los cuales llenaremos con la información obtenida por nuestro Decoder.
	//		device : Dispositivo de Salida a utilizar.
	//		channelCount : Cantidad de Canales de Salida a utilizar.
	//		sampleFormat : Formato de las Muestras a utilizar.
	//		suggestedLatency : Latencia sugerida para nuestro audio.
	//		hostApiSpecificStreamInfo : Información adicional (opcional) para el stream.
	
	// Asignaremos cada uno de estos campos con la información de nuestro decoder. Comenznado por el indice de Dispositivo asignado.
	
	Parametros_Audio.device = Indice_Device; // Dispositivo de Salida actual.
	
	// Numero de canales.
	
	Parametros_Audio.channelCount = ELECTRODEMP_CHANNELS; // Numero de Canales Establecido (Mono = 1 Canal , Estereo = 2 canales , etc).
	
	// Tipo de Muestra a ocupar. Existen numerosos tipos de muestras que PA soporta para la mayoria de implementaciones. Tenemos la siguiente lista
	// con su respectiva cantidad de bytes.
	//		paFloat32 : 4 bytes.
	//		paInt8 : 1 byte.
	//		paInt16 : 2 bytes.
	//		paInt32 : 4 bytes.
	//		paUInt8 : 1 byte.
	
	// Asignaremos el formato que corresponda al tipo especificado en la configuración.
	
	const PaSampleFormat Formato_Audio =
		(std::is_same <ELECTRODEMP_SAMPLE_TYPE , float>::value ? 	paFloat32 :
		(std::is_same <ELECTRODEMP_SAMPLE_TYPE , int8_t>::value ? 	paInt8 :
		(std::is_same <ELECTRODEMP_SAMPLE_TYPE , int16_t>::value ? 	paInt16 :
		(std::is_same <ELECTRODEMP_SAMPLE_TYPE , int32_t>::value ? 	paInt32 :
		(std::is_same <ELECTRODEMP_SAMPLE_TYPE , uint8_t>::value ? 	paUInt8 : paCustomFormat)))));
	
	// Formato de Muestras.
	
	Parametros_Audio.sampleFormat = (Formato_Audio | paNonInterleaved); // Formato de las muestras + planar (no entrelazado).
	
	// Latencia de audio sugerida.
	
	Parametros_Audio.suggestedLatency = Pa_GetDeviceInfo (Indice_Device)->defaultLowOutputLatency; // Latencia por defecto baja.
	
	// Datos adicionales de la API del host : null.
	
	Parametros_Audio.hostApiSpecificStreamInfo = nullptr; // Vacio.
	
	// Ahora vamos a comprobar si nuestro dispositivo de salida actual soporta este formato asignado con la frecuencia de muestreo que tiene
	// el audio actualmente.
	
	PaError Resultado = Pa_IsFormatSupported (nullptr , &Parametros_Audio , static_cast <double> (Decoder.Get_SampleRate ()));
	
	// Validamos esté resultado para saber si continuar.
	
	if (Resultado != paNoError) {
		
		// Error , el formato no es soportado por este dispositivo , mostramos el mensaje siguiente.
		
		LOG_FE (" >> Formato de Audio no Soportado por el Dispositivo : %s " , Pa_GetErrorText (Resultado))
		
		// Devolvemos falso.
		
		return false;
		
	}
	
	// -------------------------  Abrir Stream  ----------------------------
	
	// Ahora continuaremos con la creación de nuestro stream , ya que conocemos que nuestro formato está soportado por el device , queda crear
	// nuestro Stream actual y asignarle las funciones de callback para que se pueda comenzar a procesar el audio.
	
	// Para esto abriremos un nuevo stream de audio con la siguiente información :
	//		stream : Dirección de nuestro stream actual a utilizar.
	//		parametros de entrada : null , solo vamos a reproducir audio.
	//		parametros de salida : Valores de la estructura.
	//		sample rate : Frecuencia de muestreo de nuestro audio actual.
	//		frames por buffer : 0 (portaudio elegira el más conveniente).
	//		stream flags : paNoFlag (sin banderas adicionales).
	//		callback : Función de callback (Callback_Reproduccion).
	//		datos adicionales : this.
	
	Resultado = Pa_OpenStream (&Audio , nullptr , &Parametros_Audio , static_cast <double> (Decoder.Get_SampleRate ()) , 0u , paNoFlag , &Callback_Reproduccion , this);
	
	// Validamos esté resultado para saber si continuar.
	
	if (Resultado != paNoError) {
		
		// Error , no se pudo abrir un flujo de audio nuevo.
		
		LOG_FE (" >> Error al Abrir Stream de Audio : %s " , Pa_GetErrorText (Resultado))
		
		// Devolvemos falso.
		
		return false;
		
	}
	
	// Mostramos el siguiente mensaje.
	
	LOG (" >> Stream de Audio Creado Correctamente ")
	
	// Ahora asignaremos un callback para la finalización de nuestra reproducción de audio , usaremos el metodo especifico para este caso.
	
	Resultado = Pa_SetStreamFinishedCallback (Audio , &Callback_Finalizacion);
	
	// Validamos nuevamente el resultado.
	
	if (Resultado != paNoError) {
		
		// Error , fallo el asignar un callback de finalización.
		
		LOG_FE (" >> Error al Asignar Finish Callback : %s " , Pa_GetErrorText (Resultado))
		
		// Devolvemos falso.
		
		return false;
		
	}
	
	// Mostraremos el mensaje de aviso siguiente.
	
	LOG_N (" >> Stream Abierto Correctamente ")
	
	// ------------------------  Muestreador  ------------------------------
	
	// Iniciaremos el sampler actual.
	
	Decoder.Samples.Muestreador_Init ();
	
	// Asignaremos el total de Muestras a procesar en el Muestreador.
	
	Decoder.Samples.Set_TotalSamples (Decoder.Get_SampleCount ());
	
	// Finalmente devolvemos el valor de true.
	
	return true;
	
}

// Ahora implementaremos el siguiente metodo para realizar la reproducción de Audio con el flujo creado , tenemos que validar si no está
// activo para poder iniciarlo sin errores y validarlos en caso de haberlos.

bool Reproductor::Play_Stream () {
	
	// -------------------------  Iniciar Muestreo  ----------------------------
	
	// Empezaremos con la activación de nuestro Muestreador. Esté elemento permitira syncronizar las acciones que toman lugar en el thread del reproductor
	// , decodificador y el Main.
	
	Decoder.Samples.Muestreador_Play ();
	
	// -----------------------  Iniciar Decodificación  ------------------------
	
	// Lo siguiente es iniciar nuestro Decodificador. Iniciaremos el thread encargado de leer Frames de el archivo de audio actual , decodificarlos y
	// procesarlos para asi anexarlos a la queue de Muestras en el Muestreador.
	
	if (!Decoder.Start_Decode ()) {
		
		// Detendremos el Muestreador actual.
		
		Decoder.Samples.Muestreador_Stop ();
		
		// Devolveremos falso.
		
		return false;
		
	}
	
	// -------------------------  Iniciar Reproducción  ------------------------
	
	// Comenzaremos la reproducción de nuestro flujo de audio si consideramos que el Stream está en estado de stop en estos momentos.
	
	if (Pa_IsStreamStopped (Audio) > 0) {
		
		// Iniciaremos la reproducción de nuestro audio mediante una llamada a Pa_StartStream , lo cual iniciara el thread de reproducción
		// que comenzara a invocar a nuestro metodo de callback para esté evento.
		
		PaError Resultado = Pa_StartStream (Audio);
		
		// Validamos el error para saber si continuar o no.
		
		if (Resultado != paNoError) {
			
			// Detendremos el muestreador.
			
			Decoder.Samples.Muestreador_Stop ();
			
			// Ahora el decodificador iniciado.
			
			Decoder.Stop_Decode ();
			
			// Mostraremos el mensaje de error obtenido y realizaremos la impresión en la consola con el color de fondo Magenta.
			
			LOG_FE (" >> Error al Iniciar Stream : %s " , Pa_GetErrorText (Resultado))
			
			// Devolvemos falso.
			
			return false;
			
		}
		
	}
	
	// Finalmente devolvemos true.
	
	return true;
	
}

// Definiremos el metodo para poder pausar la reproducción del flujo de audio , esté metodo recive como entrada un bool que determina
// la manera en la que se desea detener el flujo de audio mediante una llamada a Pa_StopStream o Pa_AbortStream.

bool Reproductor::Pause_Stream (bool Abort) {
	
	// Validaremos si la opción es para abortar o simplemente detener el video (terminando la reproducción de la ultima muestra). Si además de estó se tiene
	// un estado activo o por lo menos con estado de muestreador en completado o interrumpido , procederemos a detener el stream.
	
	if (Pa_IsStreamActive (Audio) || Decoder.Samples.Is_Complete () || Decoder.Samples.Is_Interrupt ()) {
		
		// ------------------------  Pausar Muestreo  --------------------------
		
		// En primer lugar tenemos que pausar el muestreador actual , de está forma detendremos a nuestros threads de reproducción y decodificación los cuales
		// pasarán a estár liberados del syncronizador y podran terminar su ejecución.
		
		Decoder.Samples.Muestreador_Pause ();
		
		// ------------------------  Pausar Reproducción  ----------------------
		
		// Pausaremos nuestra reproducción de audio actualmente iniciado , usaremos la siguiente llamada y obtendremos el resultado para poder validar que
		// se realizó de manera correcta. Determinamos si abortar o detener.
		
		PaError Resultado = (Abort ? Pa_AbortStream (Audio) : Pa_StopStream (Audio));
		
		// Validamos el error para saber si continuar o no.
		
		if (Resultado != paNoError) {
			
			// Mostraremos el mensaje de error obtenido y realizaremos la impresión en la consola con el color de fondo Magenta.
			
			LOG_FE (" >> Error al Pausar Stream : %s " , Pa_GetErrorText (Resultado))
			
			// Devolvemos falso.
			
			return false;
			
		}
		
	}
	
	// ------------------------  Pausar Decodificación  ------------------------
	
	// Por ultimo lo que haremo es pausar nuestro decodificador actualmente procesado , usamos el siguiente metodo para intentar pausarlo y borrar el thread
	// de manera segura.
	
	Decoder.Pause_Decode ();
	
	// Finalmente devolvemos true.
	
	return true;
	
}

// Definiremos el metodo para detener el flujo de audio actual y resetear las muestras de nuestro decodificador a su estado original para
// que la reproducción pueda comenzar desde el origen nuevamente.

bool Reproductor::Stop_Stream (bool Abort) {
	
	// De igual manera validaremos si el stream está activo o los estados de Completado o Interrumpido fuerón activados desde los threads de 
	// decodificación o procesamiento respectivamente.
	
	if (Pa_IsStreamActive (Audio) || Decoder.Samples.Is_Complete () || Decoder.Samples.Is_Interrupt ()) {
		
		// ------------------------  Detener Muestreo  -------------------------
		
		// Igual que en el caso de pausar el stream, detendremos primero nuestro muestreador para que el thread de reproducción y decodificación
		// puedan terminar correctamente en caso de estar esperando algun evento.
		
		Decoder.Samples.Muestreador_Stop ();
		
		// -----------------------  Detener Reproducción  ----------------------
		
		// Detendremos nuestra reproducción de audio actualmente iniciado , usaremos la siguiente llamada y obtendremos el resultado para poder validar que
		// se realizó de manera correcta. Validamos por medio del parametro si abortar o detener el stream.
		
		PaError Resultado = (Abort ? Pa_AbortStream (Audio) : Pa_StopStream (Audio));
		
		// Validamos el error para saber si continuar o no.
		
		if (Resultado != paNoError) {
			
			// Mostraremos el mensaje de error obtenido y realizaremos la impresión en la consola con el color de fondo Magenta.
			
			LOG_FE (" >> Error al Pausar Stream : %s " , Pa_GetErrorText (Resultado))
			
			// Devolvemos falso.
			
			return false;
			
		}
		
	}
	
	// -----------------------  Detener Decodificación  ------------------------
	
	// Detendremos el decoder para que finalice la ejecución de la decodificación en caso de seguir activa , dejaremos reseteado el seek para que
	// vuelva a estar en el estado de origen.
	
	Decoder.Stop_Decode ();
	
	// Y reiniciaremos nuestro decodificador a su estado init en el momento de ser construido al abrir el flujo de audio.
	
	Decoder.Samples.Muestreador_Reset ();
	
	// Finalmente devolvemos true.
	
	return true;
	
}

// Finalmente tenemos la implementación de nuestro metodo Close_Stream para poder liberar los recursos de Stream ocupados por el archivo
// actual y tener limpio el reproductor antes de abrir otro nuevo archivo/Stream.

bool Reproductor::Close_Stream () {
	
	// ---------------------------  Stop Stream  -------------------------------
	
	// Ahora vamos a realizar una validación antes de cerrar el stream , si esté todavia está en estado play o pausa entonces lo detendremos.
	
	if (Is_Play () || Is_Pause () || Decoder.Samples.Is_Complete () || Decoder.Samples.Is_Interrupt ()) {
		
		// Detenemos nuestro audio actual y validamos el resultado.
		
		if (!Stop_Stream ()) {
			
			// Devolvemos false.
			
			return false;
			
		}
		
	}
	
	// ---------------------------  Free Decoder  ------------------------------
	
	// Ahora procederemos a liberar todos los recursos actuales que ocupamós , para estó primero reiniciaremos nuestro decoder a su estado
	// original.
	
	Decoder.Samples.Muestreador_Reset ();
	
	// Liberaremos nuestros recursos del decoder que se utilizarón (Contexto de Codec y Formato).
	
	Decoder.Free ();
	
	// Y devolvemos true.
	
	return true;
	
}

// -----------------------------------------------------------------------------

// ----------------------------  Callbacks  ------------------------------------

// Definiremos a continuación la función de callback para el Procesador de Audio , está sera invocada cada vez que portaudio necesite más
// muestras de audio , estará completamente syncronizada con el thread de la decodificación asi que tenemos el siguiente codigo para realizar 
// el procesamiento de audio.

int Reproductor::Callback_Reproduccion (const void * Entrada , void * Salida , unsigned long int Cantidad_Frames ,
	const PaStreamCallbackTimeInfo * Informacion_Tiempo , PaStreamCallbackFlags Estado_Stream , void * Datos) {
	
	// Obtendremos primero la referencia a nuestro Reproductor actual con el parametro de Datos.
	
	Reproductor * Player = reinterpret_cast <Reproductor *> (Datos);
	
	// -----------------------------  Reproducción  ----------------------------
	
	// Ahora vamos a recuperar el buffer de salida para cada canal por separado en el formato establecido en la Configuración de la biblioteca + planar. Tenemos
	// las siguientes salidas.
	
	ELECTRODEMP_SAMPLE_TYPE * Salida_I = reinterpret_cast <ELECTRODEMP_SAMPLE_TYPE **> (Salida) [0]; // Canal Izquierdo.
	ELECTRODEMP_SAMPLE_TYPE * Salida_D = reinterpret_cast <ELECTRODEMP_SAMPLE_TYPE **> (Salida) [1]; // Canal Derecho.
	
	// Ahora procederemos a contabilizar las muestras a procesar en la siguiente localidad.
	
	unsigned long int Indice_Frame = 0u;
	
	// Crearemos una Muestra de salida la cual se llenara con los valores de los samples siguientes en la cola de Muestreador del decoder.
	
	Muestreador::Muestras_Audio * Samples_Actuales = nullptr;
	
	// ----------------------------  Query Samples  ----------------------------
	
	// Valos a realizar la asignación de muestras a nuestra localidad de samples y al mismo tiempo solicitaremos un paquete de muestras más , usaremos
	// la siguiente localidad para almacenar el total de muestras que requerimos decodificar (inicialmente 0).
	
	unsigned long int Samples_Query = 0;
	
	// Validaremos la cantidad de muestras a solicitar , usaremos una diferencia para poder calcular las muestras decodificadas actualmente y las
	// que tenemos procesadas hasta ahora.
	
	const int Samples_Diferencia = static_cast <int> (Player->Decoder.Samples.Get_DecodedSamples ()) - static_cast <int> (Player->Decoder.Samples.Get_ProcessedSamples ());
	
	// Validamos esté valor con el total de muestras actual a solicitar * 2 , si la decodificación no se ha completado.
	
	if (!Player->Decoder.Samples.Is_DecodedCompleted () && (Samples_Diferencia < (Cantidad_Frames * 2))) {
		
		// Solicitaremos la cantidad de muestras actual.
		
		Samples_Query = Cantidad_Frames;
		
	}
	
	// Solicitaremos de manera inmediata la adquisisión de nuestra cantidad de Muestras al muestreador del Decodificador. De está forma tendremos
	// listo un paquete de X cantidad de muestras para nuestra proxima iteración sin tener que esperar a que el decoder las obtenga. Ademas pasaremos
	// nuestra localidad para la referencia al siguiente elemento de la cola a procesar.
	
	Player->Decoder.Samples.Query (&Samples_Actuales , nullptr , Samples_Query);
	
	// -----------------------------  Iteración  -------------------------------
	
	// Ahora vamos a iterar hasta asignar todas nuestras muestras actuales.
	
	while (Indice_Frame < Cantidad_Frames) {
		
		// Si nuestras muestras han sido consumidas entonces pasaremos a devolver paComplete.
		
		if (Samples_Actuales != nullptr) {
			
			// Realizaremos el procesamiento por cada paquete de muestras.
			
			if (Player->Funcion_Reproduccion) {
				
				// Procesamos el paquete actual de muestras.
				
				Player->Funcion_Reproduccion (Player , *Samples_Actuales , Player->Datos_Callback_Reproduccion);
				
			}
			
			// Iteramos mientras el muestreador siga activo y hayan mas muestras en el paquete recivido.
			
			while (Samples_Actuales->Indice < Samples_Actuales->Cantidad) {
				
				// Asignaremos el siguiente Valor a los canales I y D.
				
				Salida_I [Indice_Frame] = Samples_Actuales->Datos_L.get () [Samples_Actuales->Indice];
				Salida_D [Indice_Frame ++] = Samples_Actuales->Datos_R.get () [Samples_Actuales->Indice ++];
				
				// Validamos si continuar.
				
				if (Indice_Frame == Cantidad_Frames) {
					
					// Devolvemos paContinue.
					
					return paContinue;
					
				}
				
			}
			
			// Incrementaremos las muestras procesadas en el Muestreador.
			
			Player->Decoder.Samples.Add_ProcessedSamples (Samples_Actuales->Cantidad);
			
			// Eliminaremos este elemento de nuestra cola.
			
			Player->Decoder.Samples.Pop_Queue ();
			
			// Validamos si la cantidad fue consumida y el muestreador sigue activo.
			
			if (Player->Decoder.Samples.Is_Play ()) {
				
				// Validamos el estado de completado.
				
				if (!Player->Decoder.Samples.Is_ProcessedCompleted ()) {
					
					// Asignaremos null a nuestra muestra actual.
					
					Samples_Actuales = nullptr;
					
					// E intentaremos asignar un paquete nuevo de Muestras.
					
					if (!Player->Decoder.Samples.Query (&Samples_Actuales , nullptr)) {
						
						// Entonces simplemente colocaremos el estado de completado en true.
						
						Player->Decoder.Samples.Muestreador_Terminate (false);
						
						// Devolvemos paComplete.
						
						return paComplete;
						
					}
					
					// Continuaremos el siguiente ciclo.
					
					continue;
					
				}
				
			}
			else {
				
				// Resetearemos el contador de muestras actuales.
				
				// Samples_Actuales->Indice = 0u;
				
			}
			
		}
		
		// Rompemos el ciclo.
		
		break;
		
	}
	
	// Validaremos si acabamos de procesar las muestras y seguimos reproduciendo.
	
	if (Player->Decoder.Samples.Is_Play () && !Player->Decoder.Samples.Is_ProcessedCompleted ()) {
		
		// Finalmente devolvemos continue si todavia no acabamos las muestras a procesar.
		
		return paContinue;
		
	}
	else if (Player->Decoder.Samples.Is_ProcessedCompleted ()) {
		
		// Y terminaremos el muestreador por causa de completado.
		
		Player->Decoder.Samples.Muestreador_Terminate (false);
		
	}
	
	// Devolvemos paComplete.
	
	return paComplete;
	
}

// Implementaremos finalmente el callback para la finalización de nuestro audio , invocado cuando se termina de procesar todo el audio y
// tenemos que notificar alguna acción a determinados elementos.

void Reproductor::Callback_Finalizacion (void * Datos) {
	
	// Obtendremos primero la referencia a nuestro Reproductor actual con el parametro de Datos.
	
	Reproductor * Player = reinterpret_cast <Reproductor *> (Datos);
	
	// ---------------------------  Finalización  ------------------------------
	
	// Validaremos la forma en la que realizaremos el termino de nuestro reproductor , si fue por termino de decodificador o Interrumpido.
	
	if (Player->Decoder.Samples.Is_Complete () || Player->Decoder.Samples.Is_Interrupt ()) {
		
		// Detendremos nuestro Decodificador.
		
		Player->Decoder.Stop_Decode ();
		
		// Resetearemos las muestras a nuestro origen.
		
		Player->Decoder.Samples.Muestreador_Reset ();
		
		// Si tenemos el estado complete , entonces invocaremos la función de finalización.
		
		if (Player->Decoder.Samples.Is_Complete ()) {
			
			// Validamos si tenemos la función de callback asignada para poder presentar.
			
			if (Player->Funcion_Finalizacion) {
				
				// Invocaremos está función de termino.
				
				Player->Funcion_Finalizacion (Player , Player->Datos_Callback_Finalizacion);
				
			}
			
			// Mostraremos el mensaje LOG de Stream completado.
			
			LOG_N (" >> Stream Completado Correctamente ");
			
		}
		else {
			
			// Mostraremos el siguiente mensaje.
			
			LOG_W (" >> Stream Interrumpido ");
			
		}
		
	}
	else {
		
		// Mostraremos el siguiente mensaje.
		
		LOG (" >> Stream Pausado / Detenido ");
		
	}
	
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------
