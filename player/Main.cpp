
/**
* @author Menightmar
* @date Domingo 18/03/2018
**/

/// @mainpage ElectrodeMP - Reproductor de Audio y Editor de Etiquetas.
/// @brief
/// 
/// ElectrodeMP es una biblioteca hecha para crear un reproductor de Audio basico utilizando los elementos ofrecidos por : PortAudio , FFmpeg , TagLib , entre otros.
/// Tiene las funciones basicas de cualquier reproductor tales como una interfaz simple y facil de manejar que solamente requiere de pocas lineas de codigo para poder
/// iniciar la reproducci�n de un audio en alguno de los formatos soportados.
///
/// @section introduccion Introducci�n a la biblioteca de electrodemp.
/// 
/// De manera simple y r�pida uno puede simplemente tomar las funciones que aqu� est�n hechas y comenzar a construir su propio reproductor , lo unico que lo limit� es
/// la propia imaginaci�n.
///
/// Veamos un simple codigo de ejemplo donde podemos nosotros crear en unos cuantos pasos nuestro primer reproductor :
/// 
/// @code
/// 
/// #include <stdio.h>
/// #include <stdlib.h>
/// 
/// ......
/// 
/// int main () {
/// 
/// 	// Lo primero es crear nuestra base de reproductor. Para esto tenemos la clase principal de toda la biblioteca (Reproductor)
/// 	
/// 	Reproductor MediaPlayer; // Esto inicializa toda la parte interna (decoders , filters , etc...)
///		
/// 	// Ahora lo siguiente es abrir un archivo de audio. Para esto usamos el metodo Open (filename) , pasamos un ejemplo de audio que tenemos en
/// 	// nuestro escritorio de esta forma.
/// 	
/// 	MediaPlayer.Open ("ejemplo.mp3");
/// 
/// 	// Todo bien hasta ahora , solamente queda un simple paso y es reproductir nuestro audio. Para esto tenemos el conjunto t�pico de instrucciones
///		// Play , Pause , Stop.
/// 
/// 	// Reproducimos el audio.
/// 	
///		MediaPlayer.Play (); // Inicia la reproducci�n en segundo plano del audio.
///
///		// Podemos seguir procesando en el programa mientras la m�sica se reproduce. En este ejemplo solamente pausamos la consola.
///		
/// 	system ("pause");
///	
///		// Para terminar solamente hay que detener la reproducci�n y cerrar el reproductor.
/// 
///		MediaPlayer.Stop () , MediaPlayer.Close ();
///		
///		// As� de sensillo.
///		
///		return EXIT_SUCCESS;
///		
/// }
/// 
/// @endcode
/// 

// -----------------------------------------------------------------------------

// -----------------------------  Bases  ---------------------------------------

// Incluiremos entonces nuestros elementos base para poder trabajar mejor.

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// Incluiremos los flujos de iostream.

#include <iostream>

// Cadenas de C++.

#include <string>

// Y ademas algunas utilerias adicionales.

#include <memory>
#include <utility>

// ------------------------  Modulo : Pantalla  --------------------------------

// Nuestro programa ahora est� organizado en modulos , cada modulo tiene su objetivo especifico para trabajar todos en conjunto en un modulo general que tenemos en
// este cpp y con esto separar los codigos de cada biblioteca para mejorar nuestro codigo. Todas las cabeceras de los modulos estan contenidas en la carpeta "include"
// en la carpeta raiz de nuestro proyecto actual.

// Ahora vamos a incluir el modulo de la Pantalla el cual realiza la inclusi�n del resto de modulos para trabajar con nuestro reproductor.

#include <Pantalla.h>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// ---------------------------  Aplicaci�n  ------------------------------------

// Incluiremos nuestros elementos de la biblioteca wxWidgets para poder realizar la implementaci�n de nuestra aplicaci�n.

/// @defgroup Principal
/// @brief Clase de la Aplicaci�n principal para el reproductor de Audio.
///	@{

///	@brief
/// Asi que ahora el resto para nuestro modulo principal es la definici�n de nuestra wxApp la cual ejecutara nuestra pantalla principal con todos los elementos que
/// esta contiene. De esta forma tenemos a :

class Aplicacion : public wxApp {
	
	// Declaramos la pantalla Principal de nuestra aplicaci�n.
	
	private :
		
		// Tenemos la referencia a :
		
		Pantalla * Programa;
		
		// Ahora nuestro metodo OnInit el cual crea la pantalla de nuestra aplicaci�n.
		
		public :
			
			/// @brief
			/// Sobreescribiremos el metodo de inicio de nuestro programa.
			///	@return Devolvemos el resultado de nuestro programa (true == EXIT_SUCCESS).
			
			bool OnInit () override {
				
				// Para empezar iniciaremos el LOG de ElectrodeMP en el archivo txt (LOG.txt).
				
				ElectrodeMP::LOG_Init ("LOG.txt");
				
				// Registraremos el manejador de wxImage para formato PNG.
				
				wxImage::AddHandler (new wxPNGHandler ());
				
				// Ahora vamos a crear nuestra pantalla principal.
				
				Programa = new Pantalla ("ElectrodeMP - Audio Media Player Alpha");
				
				// Finalmente devolvemos true.
				
				return true;
				
			}
};

/// @brief Implementamos el wxApp y con esto terminariamos.

wxIMPLEMENT_APP (Aplicacion);

/// @}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------
