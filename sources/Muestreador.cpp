
// Implementaremos ahora nuestro codigo para las clases y estructuras de nuestro Muestreador.

#include <ElectrodeMP/Muestreador.h>

/// @file Muestreador.cpp
/// @brief Definición formal para nuestros metodos y propiedades de las clases contenidas en este modulo.
/// @author Loubiner
/// @date Saturday 17/02/2018

// Utilizaremos el espacio de nombres del Proyecto.

using namespace ElectrodeMP;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------  Muestreador  ----------------------------------

// -----------------------------------------------------------------------------

// ---------------------------  Constructores  ---------------------------------

// Comenzaremos con la implementación del Constructor por defecto que tiene nuestro Muestreador para iniciar todos los elementos
// que esté contiene a sus valores por defecto.

Muestreador::Muestreador () {
	
	// Comenzaremos iniciando la Queue de muestras a un valor nullptr.
	
	Queue.store (nullptr);
	
	// Ahora vamos a realizar la asignación de todos los valores de nuestros contadores de muestras decodificadas y procesadas en
	// valores iniciales de 0 usando el siguiente metodo.
	
	Muestreador_Reset ();
	
	// Y finalmente colocaremos un estado de indeterminado (0) en nuestro muestreador.
	
	Estado_Actual.store (0);
	
}

// -----------------------------------------------------------------------------

// -----------------------------  Get y Set  -----------------------------------

// Definiremos el metodo Set para el Total de Muestras a contabilizar en nuestro procesó actual de reproducción y decodificación.

void Muestreador::Set_TotalSamples (size_t Cantidad) {
	
	// Asignaremos este nuevo valor a nuestra localidad respectiva.
	
	Total_Muestras.store (Cantidad);
	
	// Asi mismo reiniciaremos el contador de Muestras a 0.
	
	Cantidad_Muestras_Procesadas.store (0u);
	
}

// Implementaremos el metodo para colocar una cantidad determinada de muestras en el contador actual.

void Muestreador::Set_ProcessedSamples (size_t Cantidad) {
	
	// Asignaremos el valor siguiente en la Cantidad de Muestras procesadas.
	
	Cantidad_Muestras_Procesadas.store (Cantidad);
	
}

// Ahora definiremos el metodo para sumar determinada cantidad de muestras a nuestro contador de Muestras actuales.

void Muestreador::Add_ProcessedSamples (size_t Cantidad) {
	
	// Sumaremos la cantidad en el contador de muestras procesadas.
	
	Cantidad_Muestras_Procesadas += Cantidad;
	
}

// Definiremos el metodo para colocar una cantidad de muestras decodificadas actualmente.

void Muestreador::Set_DecodedSamples (size_t Cantidad) {
	
	// Asignaremos el valor siguiente en la Cantidad de Muestras decodificadas.
	
	Cantidad_Muestras_Decodificadas.store (Cantidad);
	
	// Notificaremos a todos los threads en esperá de cualquier cambio.
	
	Condicion_Sincronizacion.notify_all ();
	
}

// Ahora definiremos el metodo para sumar determinada cantidad de muestras a nuestro contador de Muestras decodificadas.

void Muestreador::Add_DecodedSamples (size_t Cantidad) {
	
	// Sumaremos la cantidad en el contador de muestras decodificadas.
	
	Cantidad_Muestras_Decodificadas += Cantidad;
	
	// Notificaremos a todos los threads en esperá de cualquier cambio.
	
	Condicion_Sincronizacion.notify_all ();
	
}

// Definiremos el metodo para poder asignar el estado de completado en la parte del decoder.

void Muestreador::Set_DecodedCompleted (bool Completado) {
	
	// Asignaremos el valor actual sin importar el total de muestras decodificadas.
	
	Decodificacion_Completa.store (Completado);
	
	// Notificaremos a todos los threads.
	
	Condicion_Sincronizacion.notify_all ();
	
}

// -----------------------------------------------------------------------------

// -------------------------------  Queue  -------------------------------------

// Definiremos ahora el metodo de la Queue que inicializa el espacio en memoria a utilizar.

void Muestreador::New_Queue () {
	
	// Si tenemos una memoria valida asignada , entonces pasaremos a limpiar y liberar la memoria.
	
	if (Valid_Queue ()) {
		
		// Eliminaremos todos los elementos y luego liberamos el espacio.
		
		Clear_Queue () , Delete_Queue ();
		
	}
	
	// Asignaremos un nuevo espacio en memoria para la queue de muestras , con 0 elementos.
	
	Queue.store (new Queue_Muestras ());
	
}

// Anexaremos un nuevo elemento por copia en nuestra queue de Muestras actual.

void Muestreador::Push_Queue (const Muestras_Audio & Entrada) {
	
	// Colocamos el siguiente elemento en la queue.
	
	Queue.load ()->push (Entrada);
	
	// Notificaremos a nuestro thread de reproducción que hay una nueva muestra en la queue.
	
	Condicion_Sincronizacion.notify_all ();
	
}

// Anexaremos un elemento en la cola de Muestras actuales por movimiento.

void Muestreador::Push_Queue (Muestras_Audio && Entrada) {
	
	// Movemos el elemento de entrada a nuestra queue.
	
	Queue.load ()->push (std::move (Entrada));
	
	// Notificaremos a nuestro thread de reproducción que hay una nueva muestra en la queue.
	
	Condicion_Sincronizacion.notify_all ();
	
}

// Eliminaremos un elemento en la cola de muestras mediante el siguiente metodo.

void Muestreador::Pop_Queue () {
	
	// Quitaremos el siguiente elemento en la Cola de Muestras.
	
	Queue.load ()->pop ();
	
	// Notificaremos a nuestro thread de decodificación.
	
	Condicion_Sincronizacion.notify_all ();
	
}

// Y para limpiar la cola de elementos tenemos este otro metodo.

void Muestreador::Clear_Queue () {
	
	// Iteramos mientras la cola siga teniendo elementos.
	
	while (!Queue.load ()->empty ()) {
		
		// Quitaremos el siguiente elemento.
		
		Queue.load ()->pop ();
		
	}
	
}

// Ahora tenemos el metodo para poder liberar el espacio asignado para la queue de muestras.

void Muestreador::Delete_Queue () {
	
	// Liberamos la memoria de la queue y dejamos asignado un elemento nullptr.
	
	delete (Queue.load ());
	
	// Asignaremos nullptr.
	
	Queue.store (nullptr);
	
}

// -----------------------------------------------------------------------------

// --------------------------   Coordinadores  ---------------------------------

// Implementaremos el codigo para cada uno de los metodos coordinadores entre los Threads de Reproducción y Decodificación respectivamente , tenemos 
// el primer metodo el cual es invocado por el Reproductor para la obtención de nuevas muestras de audio desde la cola de elementos.

bool Muestreador::Extract (Muestras_Audio ** Salida_Muestras , std::unique_lock <std::mutex> & Bloqueo) {
	
	// Validamos si estamos activos o por lo menos completados antes de continuar.
	
	if (Is_Play ()) {
		
		// ---------------------------  Extracción  --------------------------------
		
		// Realizaremos el ciclo while mientras la cola de Muestras actuales este vacia , a la espera de que el decoder termine su procesamiento y notifique
		// la llegada de nuevas muestras.
		
		while (Empty_Queue () && Is_Play () && !Is_DecodedCompleted ()) {
			
			// Esperaremos a que el decoder notifique la llegada de más muestras en la cola.
			
			Condicion_Sincronizacion.wait (Bloqueo);
			
		}
		
		// Si estamos activos todavia o por lo menos con el decodificador completado y hay muestras en la cola , continuaremos reproduciendo.
		
		if (!Empty_Queue ()) {
			
			// Extraemos la siguiente muestra de nuestra cola y la asignaremos por movimiento a nuestra salida del parametro.
			
			*(Salida_Muestras) = std::addressof (Queue.load ()->front ());
			
			// Devolvemos true.
			
			return true;
			
		}
		
		// Devolvemos falso.
		
		return false;
		
	}
	
	// Devolvemos falso.
	
	return false;
	
}

// Definiremos ahora nuestro metodo para solicitar una cantidad determinada de muestras en la cola de Elementos actual , tenemos esté metodo para poder
// notificaar al nucleo de decodificación que procese cierta cantidad de muestras como minimo.

bool Muestreador::Request (unsigned long int Cantidad) {
	
	// Validamos si estamos activos antes de continuar.
	
	if (Estado_Actual.load () == E_Play) {
		
		// ---------------------------- Solicitud  ---------------------------------
		
		// Ajustaremos la cantidad de muestras a solicitar a un intervalo entre [0 , [Total - Procesadas]]. Validaremos antes que nuestro indice siga siendo
		// menor que la cantidad total. 
		
		if (!Is_ProcessedCompleted () && !Is_DecodedCompleted ()) {
			
			// LOG (" DDDDDDD ")
			
			// Ajustaremos la cantidad de Muestras actuales contabilizadas en 0 para poder comenzar nuevamente a decodificar.
			
			Muestras_Obtenidas.store (0u);
			
			// Tenemoss la diferencia de Muestras siguiente.
			
			const size_t Limite_Solicitud = (Total_Muestras.load () - Cantidad_Muestras_Procesadas.load ());
			
			// Ajustaremos la cantidad total de Muestras solicitadas a la cantidad especifica limitada al intervalo especifico.
			
			Muestras_Solicitadas.store (std::min (static_cast <size_t> (Cantidad) , Limite_Solicitud));
			
			// Daremos aviso a los threads.
			
			Condicion_Sincronizacion.notify_all ();
			
		}
		
		// Devolvemos true.
		
		return true;
		
	}
	
	// Devolvemos falso.
	
	return false;
	
}

// Ahora pasaremos a implementar el metodo de colocación de muestras , esté realiza en efecto la operación contraria al metodo Extract , coloca un paquete
// de nuevas muestras en la cola y espera a que se necesiten más muestras desde el decoder.

bool Muestreador::Emplace (Muestras_Audio * Entrada_Muestras , std::unique_lock <std::mutex> & Bloqueo) {
	
	// Validamos si estamos activos antes de continuar.
	
	if (Estado_Actual.load () == E_Play) {
		
		// --------------------------  Colocación  ---------------------------------
		
		// Sumaremos la cantidad de Muestras Obtenidas.
		
		Muestras_Obtenidas += Entrada_Muestras->Cantidad;
		
		// Colocaremos la entrada de Muestras en la Cola de Muestras actual y continuaremos el proceso de decodificación.
		
		Queue.load ()->push (std::move (*Entrada_Muestras));
		
		// Notificaremos a nuestro thread de que hay una muestra más en la queue.
		
		Condicion_Sincronizacion.notify_all ();
		
		// Realizaremos un bucle while mientras la cantidad de muestras obtenidas sea mayor o igual a la de las solicitadas para poder esperar por una nueva
		// solicitud de muestras por parte del reproductor.
		
		while ((Estado_Actual.load () == E_Play) && !Queue.load ()->empty () && (Muestras_Obtenidas.load () >= Muestras_Solicitadas.load ())) {
			
			// Esperaremos a la notificación de solicitud nueva.
			
			Condicion_Sincronizacion.wait (Bloqueo);
			
		}
		
		// Devolvemos true.
		
		return true;
		
	}
	
	// Devolvemos falso.
	
	return false;
	
}

// Finalmente pasaremos con la definición formal de nuestro metodo coordinador , query , el cual permite la colocación y la extracción de elementos
// en la cola , permitiendo solamente realizar una acción a la vez en ambos threads , de esta forma aseguramos un orden en la ejecución de las consultas.

bool Muestreador::Query (Muestras_Audio ** Salida , Muestras_Audio * Entrada , unsigned long int Cantidad) {
	
	// Realizaremos un bloqueo actual usando nuestro mutex de manera unica , hasta terminar con la ejecución de este metodo solo un thread podra ejecutarlo.
	
	std::unique_lock <std::mutex> Bloqueo (Mutex_Sincronizacion);
	
	// ----------------------------  Consulta  ---------------------------------
	
	// Validaremos si estamos activos en el muestreador para poder continuar.
	
	if (Estado_Actual.load () == E_Play) {
		
		// Validaremos que clase de acción de consulta realizar , extracción , colocación , o requisito para muestras. Validaremos si tenemos una entrada
		// de cantidad para requisito de muestras nuevas , en caso de no ser asi pasaremos a la siguiente parte. Obtendremos el resultado.
		
		bool Resultado = ((Cantidad > 0) ? Request (Cantidad) : true);
		
		// Validamos si nuestra Salida es diferente a nullptr para realizar una operación de extracción.
		
		if (Salida != nullptr) {
			
			// Vamos a realizar una operación de Salida y devolvemos el resultado anexado al anterior resultado.
			
			return (Resultado && Extract (Salida , Bloqueo));
			
		}
		
		// Validamos si nuestra Entrada es diferente a nullptr para realizar una operación de colocación.
		
		if (Entrada != nullptr) {
			
			// Vamos a realizar una colocación de muestras , devolvemos el resultado anexado al anterior.
			
			return (Resultado && Emplace (Entrada , Bloqueo));
			
		}
		
		// Devolvemos el resultado solamente.
		
		return Resultado;
		
	}
	
	// Devolvemos falso.
	
	return false;
	
}

// -----------------------------------------------------------------------------

// ---------------------------  Muestreación  ----------------------------------

// Implementaremos el metodo Init para iniciarlizar todos los elementos privados de nuestro reprouctor de manera correcta antes de pasar a iniciar la
// reproducción y decodificación.

void Muestreador::Muestreador_Init () {
	
	// -------------------------  Cola de Muestras  ----------------------------
	
	// Comenzando por colocar nuestra memoria para la queue de muestras que será el elemento principal para el intercambio de valores entre el
	// decodificador y el stream del reproductor.
	
	New_Queue ();
	
	// Resetearemos los contadores de las muestras actuales a sus valores iniciales en 0.
	
	Muestreador_Reset ();
	
	// Colocaremos el estado actual de initializado.
	
	Estado_Actual.store (E_Initialized);
	
}

// Tenemos el siguiente metodo para iniciar los procesos de Reproducción y Decodificación de manera correcta.

void Muestreador::Muestreador_Play () {
	
	// Activaremos el estado Play en el muestreador actual.
	
	Estado_Actual.store (E_Play);
	
	// Notificaremos a todos los threads.
	
	Condicion_Sincronizacion.notify_all ();
	
}

// Ahora vamos a implementar el metodo pause para poder detener de manera correcta nuestros procesos mediante los elementos de sincronización.

void Muestreador::Muestreador_Pause () {
	
	// Colocaremos el estado pausa en el muestreador.
	
	Estado_Actual.store (E_Pause);
	
	// Notificaremos a todos los threads.
	
	Condicion_Sincronizacion.notify_all ();
	
}

// Tenemos la implementación del metodo para detener nuestro muestreador actual.

void Muestreador::Muestreador_Stop () {
	
	// Desactivaremos el estado en el muestreador actual.
	
	Estado_Actual.store (E_Stop);
	
	// Notificaremos a todos los threads.
	
	Condicion_Sincronizacion.notify_all ();
	
}

// Ahora para terminar con el proceso de muestreador de manera correcta.

void Muestreador::Muestreador_Terminate (bool Interrupcion) {
	
	// Colocaremos el estado E_Interrupt o E_Completado según el Caso.
	
	Estado_Actual.store (Interrupcion ? E_Interrupt : E_Complete);
	
	// Notificaremos a todos los threads.
	
	Condicion_Sincronizacion.notify_all ();
	
}

// Implementaremos el metodo reset para el muestreador.

void Muestreador::Muestreador_Reset () {
	
	// Reiniciaremos los contadores actuales para nuestro muestreador.
	
	Cantidad_Muestras_Procesadas.store (0u) , Cantidad_Muestras_Decodificadas.store (0u);
	
	// Asi como la cantidad obtenida y solicitada.
	
	Muestras_Obtenidas.store (0u) , Muestras_Solicitadas.store (0u);
	
}

// -----------------------------------------------------------------------------

// ----------------------------  Destructor  -----------------------------------

// Tenemos el metodo destructor de nuestro Muestreador el cual simplemente libera la memoria ocupada por la cola de Muestras actual.

Muestreador:: ~ Muestreador () {
	
	// Si la cola es valida en estos momentos.
	
	if (Valid_Queue ()) {
		
		// Eliminaremos todos los elementos de la queue.
		
		Clear_Queue ();
		
		// Y liberaremos el espacio de la misma.
		
		Delete_Queue ();
		
	}
	
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------
