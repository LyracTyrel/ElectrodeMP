
// Implementaremos ahora nuestro codigo para las funciones de LOG.

#include <ElectrodeMP/LOG.h>

/// @file LOG.cpp
/// @brief Definición formal de todas las funciones que proporciona ElectrodeMP para registrar
/// procesos en el sistema mediante texto (LOG).
/// @author Loubiner
/// @date Tuesday 13/03/2018

// Utilizaremos el espacio de nombres del Proyecto.

namespace ElectrodeMP {

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// -------------------------------  LOG  ---------------------------------------

// Comenzaremos entonces con la declaración de nuestro elemento primordial para la generación de la salida LOG
// mediante un archivo de texto plano. Declaramos un std::filebuf para la salidad de nuestro flujo de texto utilizando
// un buffer intermedio de stream.

static std::filebuf LOG_Stream;

// Para sincronizar el envio de notificaciones de manera ordenada en todos los threads de trabajo que maneja
// electrodeMP. Utilizaremos el siguiente mutex para poder bloquear la ejecución de ambos threads en un mismo metodo.

static std::mutex LOG_Lock;

// Vamos entonces a implementar la función que inicializa nuestro LOG_Stream actual , está solamente debera ser llamada la
// primera vez para inicializar el buffer stream del archivo LOG.

bool LOG_Init (const std::string & Filename_LOG) {
	
	// Empezaremos ocasionando un bloqueo hasta que el thread actual termine de iniciar esté log.
	
	std::unique_lock <std::mutex> Bloqueo (LOG_Lock);
	
	// Si el LOG_Stream no está abierto todavia , entonces abriremos un nuevo flujo para la salida de texto en el stream
	// actual con el archivo de entrada y el modo de apertura.
	
	if (!LOG_Stream.is_open ()) {
		
		// Abriremos el nuevo flujo para el archivo actual en modo de solo salida.
		
		if (!LOG_Stream.open (Filename_LOG.c_str () , std::ios_base::out)) {
			
			// Tenemos un error con la creación del log , entonces pasaremos a devolver falso.
			
			return false;
			
		}
		
	}
	
	// Escribiremos el mensaje de inicio siguiente en la salida de nuestro LOG.
	
	const std::string Mensaje_Init ("\n ----------------  ElectrodeMP : Reproductor de Audio y Editor de Etiquetas  --------------- \n\n");
	
	// Si la consola está habilitada entonces pasaremos a mostrar el mensaje en está misma.
	
	#ifdef ELECTRODEMP_ENABLE_CONSOLE_LOG
	
	// Asi que comenzaremos cambiando el color para nuestra pantalla de Consola.
	
	system ("COLOR 70");
	
	// Mostraremos el mensaje de Inicio.
	
	puts (Mensaje_Init.c_str ());
	
	#endif
	
	// Escribiremos el mensaje de inicio en la salida del LOG mediante el uso del stream buffer abierto actualmente.
	
	LOG_Stream.sputn (Mensaje_Init.c_str () , Mensaje_Init.size ());
	
	// Devolvemos true.
	
	return true;
	
}

// Implementaremos ahora la función para realizar la escritura de nuestro texto dentro del archivo LOG iniciado anteriormente
// asi como por medio de la consola del sistema si es el caso y la opción está habilitada.

void LOG_Write (const std::string & Cadena , LOG_Type Tipo) {
	
	// Comenzaremos primero bloqueando primero nuestros threads de ejecución actual para que solamente un solo thread a la
	// vez registre el texto en el registro LOG actual.
	
	std::unique_lock <std::mutex> Bloqueo (LOG_Lock);
	
	// ----------------------------  Write  ------------------------------------
	
	// Realizaremos la escritura de nuestra cadena en el archivo LOG mediante el siguiente codigo que utiliza el flujo anteriormente abierto.
	
	if (LOG_Stream.is_open ()) {
		
		// Cadena a imprimir con el salto de linea.
		
		const std::string Cadena_Console (Cadena + "\n");
		
		// Escribiremos en el archivo la cadena.
		
		LOG_Stream.sputn (Cadena_Console.c_str () , Cadena_Console.size ());
		
	}
	
	// Si la consola de salida está habilitada entonces pasaremos a realizar la impresión de la misma cadena.
	
	#ifdef ELECTRODEMP_ENABLE_CONSOLE_LOG
	
	// Si tenemos habilitada la salida de colores mediante el uso de termcolor , validaremos lo siguiente.
	
	#ifdef ELECTRODEMP_ENABLE_TERMCOLOR
		
		// Seleccionaremos un color especifico para presentar por la salida estandar de la consola.
		
		switch (Tipo) {
			
			// Tipo Nota.
			
			case LOG_Type::LOG_Note :
				
				// Asignaremos el siguiente color para la letra y el texto.
				
				std::cout << termcolor::on_green << termcolor::white;
				break;
			
			// Tipo Warning.
			
			case LOG_Type::LOG_Warning :
				
				// Asignaremos el siguiente color para la letra y el texto.
				
				std::cout << termcolor::on_yellow << termcolor::white;
				break;
			
			// Tipo Caution.
			
			case LOG_Type::LOG_Caution :
				
				// Asignaremos el siguiente color para la letra y el texto.
				
				std::cout << termcolor::on_yellow << termcolor::red;
				break;
			
			// Tipo Error.
			
			case LOG_Type::LOG_Error :
				
				// Asignaremos el siguiente color para la letra y el texto.
				
				std::cout << termcolor::on_grey << termcolor::white;
				break;
			
		}
		
	#endif // ELECTRODEMP_ENABLE_TERMCOLOR
	
	// Imprimiremos la cadena por la consola de está forma.
	
	std::cout << Cadena;
	
	#ifdef ELECTRODEMP_ENABLE_TERMCOLOR
		
		// Resetearemos el color utilizado.
		
		std::cout << termcolor::reset;
		
	#endif
	
	// Daremos un salto de linea.
	
	std::cout << std::endl;
	
	#endif // ELECTRODEMP_ENABLE_CONSOLE_LOG
	
}

// Ahora implementaremos el terminador de registros LOG mediante la llamada al metodo para cerrar el registro.

void LOG_Terminate () {
	
	// Tenemos un bloqueo para proteger el codigo siguiente y permitir solo entrar a un thread a la vez.
	
	std::unique_lock <std::mutex> Bloqueo (LOG_Lock);
	
	// Escribiremos el mensaje de finalización siguiente en la salida de nuestro LOG.
	
	const std::string Mensaje_Terminate (
		"\n -------------------------  ElectrodeMP : Finalizado  ------------------------"
		"\n -----------------------------------------------------------------------------\n");
	
	// Validaremos para comenzar que tengamos un archivo LOG valido antes de continuar.
	
	if (LOG_Stream.is_open ()) {
		
		// Escribiremos el registro del mensaje de finalización.
		
		LOG_Stream.sputn (Mensaje_Terminate.c_str () , Mensaje_Terminate.size ());
		
		// Y finalmente liberaremos el registro LOG.
		
		LOG_Stream.close ();
		
	}
	
	// De igual forma si tenemos habilitado la salida por consola.
	
	#ifdef ELECTRODEMP_ENABLE_CONSOLE_LOG
	
	// Mostraremos el mensaje de terminación.
	
	puts (Mensaje_Terminate.c_str ());
	
	#endif
	
}

// -----------------------------------------------------------------------------

} // namespace ElectrodeMP.

// ----------------------------------------------------------------------------------------------------------------------------------------------------------
