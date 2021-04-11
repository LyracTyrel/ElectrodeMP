
/// @file Decodificador.h
///	@brief Archivo de cabecera donde tenemos declarados la clase de nuestro decodificador de factoria el 
/// cual realiza las acciones pertinentes con los archivos de audio leidos y devuelve arreglos de muestras los cuales
/// estan normalizados para su procesamiento en el programa de nuestro reproductor.
///
/// La base para nuestro decodificador la encontramos en la biblioteca de FFmpeg la cual nos servira para realizar
///	la lectura y decoder de nuestros archivos de audio , por lo que necesitaremos registrar los codecs permitidos antes
///	de poder realizar cualquier acción.
/// @author Loubiner
/// @date Wednesday 2/14/2018

#ifndef ELECTRODEMP_DECODIFICADOR_H
#define ELECTRODEMP_DECODIFICADOR_H

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------  Bases  ---------------------------------------

// Incluiremos para empezar nuestras cabeceras de C.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Tenemos las funciones matematicas de C/C++.

#include <cmath>

// Los flujos estandar de C++.

#include <iostream>

// Ahora tenemos las cadenas de C++.

#include <string>

// Elementos de contenedores como el vector , deque y la cola.

#include <vector>
#include <deque>
#include <queue>

// Incluiremos nuestros threads y nuestros datos atomicos.

#include <atomic>
#include <thread>

// Funciones lambda de C++ 11.

#include <functional>

// Tipos de datos validados.

#include <type_traits>

// Limites numericos para el Resample.

#include <limits>

// Asi como las utilerias para trabajar con memorias y elementos de C++ 17.

#include <memory>
#include <utility>

// -----------------------  Modulo : Muestreador  ------------------------------

// Incluiremos el modulo del Muestreador el cual tiene como elementos las clases necesarias para las Muestras , los Resamplers y nuestro
// muestreador que comunica con el thread de Reproducción y Decodificación.

#include <ElectrodeMP/Muestreador.h>

// -----------------------------------------------------------------------------

// ----------------------------  FFmpeg  ---------------------------------------

// Ahora pasaremos con lo importante , la biblioteca de FFmpeg la cual nos dara las herramientas necesarias para poder comenzar a trabajar
// con muchos diferentes tipos de formatos de audio ademas del Wav que fue el que utilizamos para el ejercicio anterior. Esta biblioteca
// tiene muchos elementos asi que es necesario organizar.

// Incluiremos de esta las bibliotecas de :
//		libavformat : Manejador de Demuxers para decodificar audio.
//		libavcodec : Conjunto de codecs para decodificación.
//		libavutility : Utilerias de FFmpeg.

extern "C" {

// Comenzaremos por incluir la configuración para nuestra biblioteca.

#include <config.h>

// Incluiremos la biblioteca de formatos.
	
#include <libavformat/avformat.h>
	
// Incluiremos la biblioteca de codecs.
	
#include <libavcodec/avcodec.h>
	
// Incluiremos la biblioteca de utilerias.
	
#include <libavutil/avutil.h>
	
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Incluiremos todo el codigo dentro del siguiente espacio de nombres.

namespace ElectrodeMP {

// Vamos a declarar nuestra clase para nuestra factoria de formatos de audio la cual realizara la tarea de decodificar archivos de audio para su pro-
// sesamiento en nuestra aplicación de reproductor.

/// @defgroup Decodificador
/// @brief Decodificador de Audio hecho con FFmpeg.
/// @{

/// @brief
///	Declaramos la clase para nuestro decodificador , la cual realiza la tarea de decodificar un archivo de audio en la entrada y procesar las
/// muestras obtenidas.

class Decodificador {
	
	// Tenemos la siguiente lista de elementos en nuestra clase.
	
	private :
		
		// -----------------------  Contexto FFmpeg  ---------------------------
		
		// Tenemos en primer lugar nuestro contexto para la realización de nuestra decodificación de un archivo de audio.
		
		AVFormatContext * Contexto_Formato = nullptr;
		
		// Asi como nuestro Contexto para la decodificación de nuestro audio mediante el uso de parametros de Codec.
		
		AVCodecContext * Contexto_Codec = nullptr;
		
		// -----------------------  Streams FFmpeg  ----------------------------
		
		// En segundo lugar tenemos las referencias a todos los streams de Audio disponibles en un archivo multimedia abierto. Demuxeado
		// anteriormente con el metodo especifico.
		
		std::vector <AVStream *> Streams;
		
		// Tenemos la localidad para guardar el indice de stream seleccionado actualmente en un valor de 0 hasta Streams.size () - 1.
		
		size_t Indice_Audio = 0u;
		
		// La referencia para nuestro Stream de Audio el cual seleccionaremos utilizando nuestro metodo especifico para realizarlo.
		
		AVStream * Stream_Audio = nullptr;
		
		// Tenemos nuestro Codec de Auio para el Flujo de Audio actual configurado con los parametros especificos.
		
		AVCodec * Codec_Audio = nullptr;
		
		// -------------------------  Seeking  ---------------------------------
		
		// Para desplazar nuestro audio actual a determinado tiempo de manera asyncrona necesitamos de un valor actual el cual vaya cambiando
		// de false a true para poder desplazar el audio.
		
		std::atomic <bool> Desplazar;
		
		// Ahora tenemos un valor actual que representa el tiempo en segundos al cual desplazar nuestro audio.
		
		std::atomic <int64_t> Tiempo_Desplazar;
		
		// Y las flags de dirección para nuestro Desplazamiento.
		
		std::atomic <int> Flag_Desplazar;
		
		// ---------------------  Datos de Callback  ---------------------------
		
		// Nuestro decodificador maneja dos callbacks , uno para cuando estamos procesando los Frames de audio y otro para cuando se termina
		// la decodificación del audio. Guardaremos entonces los datos a ocupar para cada callback en las siguientes localidades.
		
		// Datos para el Callback de Procesamiento.
		
		void * Datos_Callback_Procesamiento = nullptr;
		
		// Datos para el Callback de Finalización.
		
		void * Datos_Callback_Finalizacion = nullptr;
		
		// Declaramos nuestra función para el Callback de Procesamiento con el formato siguiente :
		//		Decodificador * Objeto , AVFrame * Frame , void * Datos : void.
		
		std::function <void (Decodificador * , AVFrame * , void *)> Funcion_Procesamiento;
		
		// Declaramos nuestra función para el Callback de Finalización.
		//		Decodificador * Objeto , void * Datos : void.
		
		std::function <void (Decodificador * , void *)> Funcion_Finalizacion;
		
		// ------------------------  Threading  --------------------------------
		
		// Tenemos ahora un Thread el cual realizara todo el proceso de la decodificación en el background. Y una clase personalizada para remuestrear nuestros
		// frames obtenidos por el decoder.
		
		// Declaramos nuestro Thread siguiente el cual sera el encargado de procesar nuestro audio en la entrada y llenar asyncronamente nuestro
		// contenido en las muestras para que el reproductor las pueda leer de igual forma.
		
		std::thread * Nucleo_Decodificar = nullptr;
	
	// Tenemos ahora la declaración formal de nuestro Muestreador de Datos y estado de nuestro decodificador.
	
	private :
		
		// ------------------------  Muestreador  ------------------------------
		
		/// @brief
		/// El muestreador de samplers es la unidad de coordinación entre el Decodificador y el Reproductor.
		///
		/// Tiene en sus datos una Cola de Muestras del tipo de Sample especificado en el archivo de cabecera.
		/// Comparte elementos para ambos threads tanto la reproducción y la decodificación y permite sincronizar
		/// acciones entre ambos threads.
		
		Muestreador Samples;
		
		// ---------------------------  Estado  --------------------------------
		
		/// @brief
		/// Para poder indicar en que momento detener el thread necesitamos de un elemento adicional que detenga de manera directa el proceso de
		/// decodificación y con esto terminar la ejecución del thread del trabajo. Usaremos el siguiente estado para esto.
		
		std::atomic <bool> Activo;
		
		// ------------------------  Clase Reproductor  ------------------------
		
		/// @brief Compartiremos la información del decodificador con el Reproductor.
		
		friend class Reproductor;
	
	// Tenemos ahora nuestros metodos publicos para la clase del Decodificador.
	
	public :
		
		/// @name Constructores
		/// @brief Constructores para nuestro programa
		/// @{
		
		// --------------------------  Constructores  --------------------------
		
		/// @brief
		///	Tenemos el primer constructor , el constructor por defecto el cual simplemente inicializa la biblioteca y los valores por defecto.
		
		Decodificador ();
		
		/// @}
		
		// ---------------------------  Iniciador  -----------------------------
		
		/// @name Inicializadores
		/// @brief Tenemos ahora el metodo inicializador de nuestra biblioteca el cual nos permitira registrar todos los codecs , parsers y formatos
		/// de audio permitidos por nuestro programa.
		/// @{
		
		/// @brief Initializador de la biblioteca del Decoder (FFmpeg).
		/// @return true en caso de No haber error , falso en caso contrario.
		
		bool Inicializar ();
		
		/// @}
		
		// ----------------------------  Callback  -----------------------------
		
		/// @name Metodos de Callback
		/// @brief Tenemos algunos metodos para asignar callbacks de tipo Functor los cuales se ejecuten en determinados momentos de nuestro decodificador ,
		/// para realizar acciones adicionales.
		/// @{
		
		/// @brief El tipo de Función para el Callback de Procesamiento.
		
		typedef void (* Tipo_Callback_Procesamiento) (Decodificador * , AVFrame * , void *);
		
		/// @brief El tipo de Función para el Callback de Finalización.
		
		typedef void (* Tipo_Callback_Finalizacion) (Decodificador * , void *);
		
		/// @brief Metodo para la asignación de una función de callback de Procesamiento la cual se invoca por cada Frame decodificado.
		/// @tparam TypeFuncion Objeto funcional ó puntero a función de tipo @ref Tipo_Callback_Procesamiento.
		/// @param Funcion Callback a invocar durante el procesamiento.
		/// @param Datos Datos adicionales para nuestra función. Pueden ser Null si no se necesitan.
		/// @see Tipo_Callback_Procesamiento
		
		template <typename TypeFuncion>
		void Set_Callback_Procesamiento (TypeFuncion Funcion , void * Datos = nullptr) {
			
			// Asignaremos nuestra función actual al callback de nuestro decodificador.
			
			Funcion_Procesamiento = std::forward <typename std::decay <TypeFuncion>::type> (Funcion);
			
			// Asignaremos nuestros datos adicionales.
			
			Datos_Callback_Procesamiento = Datos;
			
		}
		
		/// @brief Metodo para la asignación de una función de callback de Finalización la cual es invocada al terminar la decodificación.
		/// @tparam TypeFuncion Objeto funcional ó puntero a función de tipo @ref Tipo_Callback_Finalizacion.
		/// @param Funcion Callback a invocar al terminar de decodificar.
		/// @param Datos Datos adicionales para nuestra función. Pueden ser Null si no se necesitan.
		/// @see Tipo_Callback_Finalizacion
		
		template <typename TypeFuncion>
		void Set_Callback_Finalizacion (TypeFuncion Funcion , void * Datos = nullptr) {
			
			// Asignaremos nuestra función actual al callback de nuestro decodificador.
			
			Funcion_Finalizacion = std::forward <typename std::decay <TypeFuncion>::type> (Funcion);
			
			// Asignaremos nuestros datos adicionales.
			
			Datos_Callback_Finalizacion = Datos;
			
		}
		
		/// @}
		
		// ------------------------  Demuxer  ----------------------------------
		
		/// @name Demuxer
		///	@brief
		/// Ahora tenemos los elementos más importantes de nuestra clase , los cuales són el propio demuxer el cual se encarga de realizar la separación
		/// de nuestros flujos de audio en streams los cuales contienen toda la información sobre el audio guardado en el archivo multimedia y comprimido.
		///
		/// Está información sera completada una vez se seleccióne un stream de audio de entre todo los posibles , teniendo en cuenta la info de cada stream
		/// se procedera a repartir memoria para nuestro codec. Seleccionar stream antes de comenzar la decodificación con @ref Select_Stream.
		/// @{
		
		/// @brief Tenemos el metodo para cargar nuestro archivo de audio y demuxear su contenido en flujos , llenando un vector de todos los streams de audio
		/// validos en el archivo.
		/// @param Filename El archivo de audio a decodificar (demuxer primero).
		/// @return True en caso de no haber error.
		
		bool Demuxer (const std::string & Filename);
		
		/// @brief Para poder obtener la cantidad de streams de audio validos , tenemos el siguiente metodo de acceso rapido que devuelve cuandos streams
		/// fuerón anexados a la lista.
		/// @return Cantidad de streams de audio en el archivo demuxeado o 0 si no hay streams o no se ha demuxeado un archivo.
		
		ELECTRODEMP_FORCE_INLINE size_t Get_StreamCount () const {
			
			// Devolvemos la cantidad de streams que tiene nuestra lista.
			
			return Streams.size ();
			
		}
		
		/// @brief Tenemos el metodo para seleccionar un stream de audio a decodificar en el proceso. Tenemos que haber Demuxeado el archivo de entrada
		/// antes de poder seleccionar alguno de los streams posibles.
		///
		/// @note El metodo inicializa el decoder apropiado para el stream de audio seleccionado y devuelve un valor que indica si la operación realizada
		/// fue correctamente hecha.
		/// @see Demuxer , Get_StreamCount.
		/// @param Indice Indice del flujo de audio a utilizar en el rango de 0 a (Get_StreamCount - 1).
		/// @return True si fue correctamente asignado el stream a decodificar.
		
		bool Select_Stream (size_t Indice);
		
		/// @}
		
		// ----------------------------  Decoder  ------------------------------
		
		/// @name Decoder
		/// @brief
		/// Ahora tenemos el conjunto de metodos para la acción del decoder en especifico ,el cual es el encargado de realizar la decodificación del 
		/// formato de codec utilizado para determinado flujo de audio el cual se puede especificar mediante el metodo especifico.
		/// 
		/// Tenemos las acciones basicas para iniciar la decodificación , pausarla , detenerla y finalmente liberar los recursos del decoder utilizado.
		/// @{
		
		/// @brief
		/// Ahora tenemos el metodo para la decodificación de nuestro flujo de audio previamente inicializado y configurado , el resto sera abrir nuestro
		/// codec de audio para decodificar y asignar las muestras a nuestra cola de muestras.
		/// @return True en caso de no presentar error.
		
		bool Start_Decode ();
		
		/// @brief
		/// Tenemos el siguiente metodo para pausar el proceso de decodificación de audio , no elimina los elementos de la cola actualmente asignados ,
		/// simplemente detiene el thread de la decodificación temporalmente.
		
		void Pause_Decode ();
		
		/// @brief
		/// Tenemos el siguiente metodo para detener el proceso de decodificación de audio , asignando un valor de Actividad en falso , destruyendo de
		/// manera segura nuestro thread ejecutado y limpiando la cola de Muestras en el Muestreador.
		
		void Stop_Decode ();
		
		/// @brief
		/// Tenemos ahora un metodo el cual permite desplazar nuestro flujo de audio actual a determinado codigo de tiempo en segundos , pasaremos por
		/// una conversión a la unidad de medida de nuestro flujo actual para poderlo desplazar.
		/// @param Second Segundo al cual desplazar el stream.
		
		void Seek (size_t Second);
		
		/// @brief
		/// Como ultimo metodo tenemos el que libera y destruye todos los elementos de Contexto creados para poder abrir un nuevo archivo de audio
		/// o terminar la aplicación de manera segura. Nos aseguramos de limpiar todos los elementos repartidos y dejar sus valores de vuela en null.
		
		void Free ();
		
		/// @}
		
		// -----------------------  Propiedades  -------------------------------
		
		/// @name Propiedades del Audio
		/// @brief Tenemos ahora un conjunto de propiedades las cuales pueden ser de utilidad para el reproductor de audio.
		/// Entre las que encontramos principalmente a :
		///		Frecuencia de Muestreo del Audio.
		///		Bit Rate (Bits / Segundo).
		///		Numero de Canales
		///		Duración total de nuestro audio en segundos.
		///		Muestras Totales Decodificadas.
		/// @{
		
		/// @brief
		/// Metodo para acceder a nuestra frecuencia de muestreo de nuestro audio actual.
		/// @note Si no tenemos audio demuxeado entonces devuelve 0.
		/// @return Frecuencia de muestreo ó muestras por segundo (Hz)
		
		ELECTRODEMP_FORCE_INLINE size_t Get_SampleRate () const {
			
			// Devolvemos la frecuencia de muestreo de nuestro audio si tenemos un flujo actualmente.
			
			return ((Stream_Audio != nullptr) ? Stream_Audio->codecpar->sample_rate : 0u);
			
		}
		
		/// @brief
		/// Metodo para obtener nuestro bit-rate o bits por segundo del audio decodificado.
		/// @note Si no hay audio devuelve 0.
		/// @return La frecuencia de bits de nuestro audio.
		
		ELECTRODEMP_FORCE_INLINE size_t Get_BitRate () const {
			
			// Devolvemos el bit rate para el audio actual.
			
			return ((Stream_Audio != nullptr) ? Stream_Audio->codecpar->bit_rate : 0u);
			
		}
		
		/// @brief
		/// Metodo apra obtener la cantidad de canales de nuestro audio actual.
		/// @note Si no tenemos audio devuelve 0.
		/// @return Cantidad de Canales de nuestro flujo de audio (1 = Mono , 2 = Estereo , ..).
		
		ELECTRODEMP_FORCE_INLINE size_t Get_Channels () const {
			
			// Devolvemos la cantidad de canales actual.
			
			return ((Stream_Audio != nullptr) ? Stream_Audio->codecpar->channels : 0u);
			
		}
		
		/// @brief
		/// Metodo para obtener la duración total de nuetro audio en s.
		/// @note Si no tenemos audio devuelve 0 segundos.
		/// @return Duración del flujo de audio en segundos.
		
		ELECTRODEMP_FORCE_INLINE size_t Get_Duration () const {
			
			// Para devolver la duración en segundos tenemos que hacer una conversión de nuestra duración multiplicada por
			// el tiempo base de nuestro stream el cual es un valor Rational (Numerador / Denominador).
			
			return ((Stream_Audio != nullptr) ? 
				static_cast <size_t> ((static_cast <float> (Stream_Audio->time_base.num) / Stream_Audio->time_base.den) * Stream_Audio->duration) : 0u);
			
		}
		
		/// @brief
		/// Tenemos el siguiente metodo para facilitar el acceso a codigo de tiempo en segundos conforme el muestreo actual.
		/// @param Sample Muestra actual a convertir a unidad de tiempo del Stream.
		/// @return Tiempo en segundos de este sample.
		
		ELECTRODEMP_FORCE_INLINE size_t Get_TimeSample (size_t Sample) const {
			
			// Para obtener la cantida de Tiempo actual necesitamos realizar un ajuste de escala para el Flujo de tiempo actual , de esta forma
			// podremos pasar las unidades de muestra a tiempo. Haremos una regla de tres para determinar a que tiempo corresponde determinada muestra.
			
			return static_cast <size_t> ((static_cast <float> (Sample) * Get_Duration ()) / Get_SampleCount ());
			
		}
		
		/// @brief
		/// Tenemos el siguiente metodo que determina la muestra que corresponde a determinado tiempo en segundos , usaremos este
		/// metodo cuando queramos obtener la muestra para cierto tiempo.
		/// @param Tiempo Tiempo en segundos a pasar a muestras.
		/// @return La muestra respectiva a determinada tasa de muestreo en el audio actual en el intervalo de : [0 - @ref Get_SampleCount ()].
		
		ELECTRODEMP_FORCE_INLINE size_t Get_SampleTime (size_t Tiempo) const {
			
			// Simplemente multiplicaremos nuestro tiempo en segundos por la tasa de muestras por segundo en el stream actual.
			
			return std::min ((Tiempo * Get_SampleRate ()) , Get_SampleCount ());
			
		}
		
		/// @brief
		/// Metodo para recuperar la cantidad de muestras totales que tiene nuestro audio actual.
		/// @note Si no hay audio devolvemos entonces 0.
		/// @return La cantidad de muestras totales en el flujo de audio.
		
		ELECTRODEMP_FORCE_INLINE size_t Get_SampleCount () const {
			
			// Devolvemos la duración del audio multiplicada por nuestro sample-rate.
			
			return ((Stream_Audio != nullptr) ? (Get_SampleRate () * Get_Duration ()) : 0u);
			
		}
		
		/// @}
		
		// ----------------------------  Destructor  ---------------------------
		
		/// @brief Declaramos el destructor para cerrar el decodificador de manera segura antes de liberar recursos.
		
		virtual ~ Decodificador ();
	
	// Tenemos los siguientes metodos privados para uso personal de nuestros metodos principales.
	
	private :
		
		// Tenemos el siguiente metodo el cual se encarga de la asignación del Codec especifico para nuestros parametros de Codec especificos. Devolviendo
		// true en caso de no haber error.
		
		bool Alloc_Decoder (const AVCodecParameters * Parametros);
		
		// El siguiente metodo permite procesar el audio de un Frame y devolver nuestra Muestra respectiva la cual puede ser reproducida por el reproductor
		// especifico en formato float de 32 bits donde el rango de valores varia de [-1.0 a 1.0].
		
		Muestreador::Muestras_Audio Process_Frame (const AVFrame * Frame);
		
		// ---------------------------  Reset  ---------------------------------
		
		// Ahora vamos a declarar un metodo para resetear el estado de nuestro decodificador , moviendo la posición actual de frame al origen y con estó
		// comenzando el audio desde 0.
		
		void Reset ();
	
};

/// @}

} // namespace ElectrodeMP.

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ELECTRODEMP_DECODIFICADOR_H
