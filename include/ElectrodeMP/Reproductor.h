
/// @file Reproductor.h
///	@brief Archivo de cabecera donde declaramos la clase para nuestro Reproductor de Audio sensillo el cual tiene como
/// base el decodificador de audio hecho con FFmpeg y su reproducción en llamadas a las funciones de PortAudio.
///
/// Nuestro Reproductor contiene diferentes elementos base como los streams de portaudio los cuales son creados con base a
/// una serie de atributos definidos por el decodificador y asignados a nuestra salida de audio. Tenemos un thread dedicado
/// al procesamiento de nuestras muestras de audio obtenidas de nuestro decodificador.
/// @author Loubiner
/// @date Thursday 2/15/2018

#ifndef ELECTRODEMP_REPRODUCTOR_H
#define ELECTRODEMP_REPRODUCTOR_H

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

// Algoritmos matematicos y de rangos en C++.

#include <algorithm>

// Vector para listas de host.

#include <vector>

// Elementos de contenedores como el deque y la cola.

#include <deque>
#include <queue>

// Manejador de tipos de datos.

#include <type_traits>

// El mapa de elementos desordenado.

#include <unordered_map>

// Incluiremos nuestros threads , datos atomicos , future , mutex y condiciones variables.

#include <atomic>
#include <mutex>
#include <future>
#include <condition_variable>
#include <thread>

// Funciones lambda de C++ 11.

#include <functional>

// Asi como las utilerias para trabajar con memorias y elementos de C++ 17.

#include <memory>
#include <utility>

// -----------------------  Modulo : Decodificador  ----------------------------

// Una dependencia directa de nuestro reproductor de audio es el Decodificador , el cual es una clase con los metodos para decodificar
// y obtener una cola de muestras la cual puede ser interpretada por nuestro reproductor para la salida de audio por nuestra PC.

#include <ElectrodeMP/Decodificador.h>

// -----------------------  Modulo : Etiquetador  ------------------------------

// La otra dependencia del reprodutor es la clase Etiquetador , misma que nos devuelve estructuras de Etiqueta las cuales contienen información
// adicional de la song actualmente reproducida asi como su caratula de portada para presentar en la aplicación grafica GUI.

#include <ElectrodeMP/Etiquetador.h>

// -----------------------------------------------------------------------------

// ---------------------------  PortAudio  -------------------------------------

// En este archivo vamos a trabajar completamente con la biblioteca de portaudio para la reproducción de nuestro audio. Está nos proporciona
// las funciones y estructuras necesarias para realizar la colocación de un dispositivo de salida , una API de host y un flujo de audio el cual
// puede ser detenido en cualquier momento.

#include <portaudio.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Incluiremos todo el codigo dentro del siguiente espacio de nombres.

namespace ElectrodeMP {

/// @defgroup Reproductor
/// @brief Reproductor de Audio hecho con PortAudio.
/// @{

/// @brief Ahora vamos a declarar la clase para la reproducción de nuestro Audio.
///
/// La clase contiene un Decodificador de Audio el cual es su principal herramienta para la obtención de muestras a reproducir , tiene tambien
/// algunos elementos para gestionar el reproductor como su estado actual y su indice de muestra.

class Reproductor {
	
	// Los siguientes son los elementos privados para nuestro reproductor.
	
	private :
		
		// ----------------------  Decodificador  ------------------------------
		
		// En primer lugar el elemento más importante es nuestro decodificador , esté nos provee de los metoods necesarios para demuxear ,
		// decodificar y generar las muestras de audio para un determinado archivo , asi que de esta forma tenemos la siguiente declaración.
		
		Decodificador Decoder;
		
		// Guardaremos la referencia a nuestro filename de audio actualmente reproducido el cual puede ser consultado con su metodo respectivo.
		
		std::string Filename_Audio;
		
		// ----------------------  Etiquetador  --------------------------------
		
		// El siguiente elemento importante en el reproductor es el analizador y generador de etiquetas TAG para los archivos de audio , de
		// está manera podremos leer los metadatos de los archivos de audio de entrada y recuperar información sobre la canción asi como su portada
		// para presentar en cualquier lugar.
		
		Etiquetador Tager;
		
		// Tenemos la Etiqueta actualmente obtenida por el Tager para el ultimo filename abierto correctamente.
		
		Etiqueta Etiqueta_Audio;
		
		// ----------------------  Streaming  ----------------------------------
		
		// Ahora pasaremos a la parte de PortAudio , comenzaremos entonces declarando nuestro Indice de Host como lo usamos el ejercicio
		// pasado , este determinara la API a utilizar para la reproducción de nuestro audio.
		
		PaHostApiIndex Indice_Host;
		
		// Ahora el indice del dispositivo GLOBAL el cual estamos utilizando para nuestra salida de audio , nuestra clase proporciona un metodo para
		// seleccionar un elemento dentro de una lista de posibilidades , asi que declaramos el indice primero aqui.
		
		PaDeviceIndex Indice_Device;
		
		// Ahora vamos a declarar un mapa de dispositivos el cual nos ayude a recuperar el valor de nuestro Indice de dispositivo a partir del nombre
		// del mismo , declaramos el siguiente mapa.
		
		std::unordered_map <std::string , PaDeviceIndex> Mapa_Devices;
		
		// Ahora pasaremos a declarar los Elementos para el Flujo de Audio actual , comenzando por el Stream el cual se utilizará para la reproducción
		// de nuestro audio actual.
		
		PaStream * Audio = nullptr;
		
		// -----------------------  Estado Global  ------------------------------
		
		// Tenemos la siguiente propiedad global estatica la cual determina si el reproductor de audio fue correctamente inicializado o todavia no
		// se ha inicializado.
		
		static bool Inicializado;
		
		// Para tener un conteo de las referencias a la biblioteca portaudio , usaremos el siguiente contador , este valor permitira determinar en que
		// momento destruir correctamente un reproductor de audio finalizando todos los valores de la biblioteca.
		
		static int Referencias;
		
		// Declaramos una localidad que indica si el estado de nuestro Reproductor es valido , cuando tenemos asignado un device y host api para
		// la reproducción de audio correctamente y la biblioteca fue inicializada.
		
		bool Valido = false;
		
		// ------------------------  Callbacks  --------------------------------
		
		// Tenemos la posibilidad de ejecutar eventos durante la llamada al metodo principal de callback del reproductor , esto permitira realizar codigo
		// adicional ademas de la simple reproducción.
		
		// Tenemos la función de callback para la reproducción del audio la cual tiene el formato siguiente :
		//		void (Reproductor * Objeto , Muestreador::Muestras_Audio & , void * Datos)
		
		std::function <void (Reproductor * , Muestreador::Muestras_Audio & , void *)> Funcion_Reproduccion;
		
		// Ahora los datos adicionales para está función de callback.
		
		void * Datos_Callback_Reproduccion = nullptr;
		
		// Tenemos la función de callback para cuando se termina de procesar todo el audio :
		//		void (Reproductor * Objeto , void * Datos)
		
		std::function <void (Reproductor * , void *)> Funcion_Finalizacion;
		
		// Ahora los datos adicionales para está función de callback.
		
		void * Datos_Callback_Finalizacion = nullptr;
	
	// Ahora pasarmos con los estados de nuestro reproductor actual.
	
	public :
		
		// -------------------------  Estado  ----------------------------------
		
		/// @brief Tenemos la parte de los estados del Reproductor.
		/// Para determinar la condición actual del reproductor presentaremos una enumeración la cual determinara en que estado se encuentra nuestro
		/// reproductor en estos momentos.
		
		enum Estado : short int {
			
			/// @brief Estado de Abierto.
			
			E_Open = 1 ,
			
			/// @brief Estado de Play.
			
			E_Play ,
			
			/// @brief Estado de Pausa.
			
			E_Pause ,
			
			/// @brief Estado de Stop.
			
			E_Stop ,
			
			/// @brief Estado de Cerrado.
			
			E_Close
			
		};
		
		/// @brief Tenemos una variable atomica la cual modificara y nos presentara el estado actual de nuestro reproductor.
		
		std::atomic <short int> Estado_Actual;
	
	// Lo siguiente va a ser la declaración de los metodos publicos y constructores para nuestro reproductor.
	
	public :
		
		// ------------------------  Constructores  ----------------------------
		
		/// @name Constructores
		/// @brief Constructores de nuestro Reproductor.
		/// @{
		
		/// @brief Constructor por defecto vacio.
		/// Este inicializara tanto al decoder como a la parte principal de PortAudio para comenzar a inicializar algunos valores antes de la decodificación
		/// y reproducción de nuestro audio.
		
		Reproductor ();
		
		/// @}
		
		// ---------------------  Init y Terminate  ----------------------------
		
		/// @name Inicializador y Terminador.
		/// @brief Esté par de metodos de clase permiten como sus nombres lo indican , inicializar el contexto de portaudio ó terminarlo para el funcionamiento
		/// actual del reproductor.
		/// @{
		
		/// @brief Inicializador de la biblioteca de dependencia en el Reproductor.
		/// @return True en caso de ser inicializado correctamente.
		
		static bool Init ();
		
		/// @brief Terminador de la biblioteca de depencencia del Reproductor.
		/// @return True en caso de ser finalizado correctamente.
		
		static bool Terminate ();
		
		/// @}
		
		// ------------------------  Accesores  --------------------------------
		
		/// @name Accesores de Datos
		/// @brief Tenemos un conjunto de metodos para el acceso a algunas propiedades de nuestro reproductor actual asi como al Tag Generado.
		/// @{
		
		/// @brief Acceso a nuestro Filename de Audio actualmente abierto.
		/// @return Filename del ultimo audio abierto de forma correcta en esté reproductor.
		
		ELECTRODEMP_FORCE_INLINE std::string Get_Filename () const {
			
			// Devolvemos la referencia a nuestro archivo de audio abierto.
			
			return Filename_Audio;
			
		}
		
		/// @brief Acceso a la Etiqueta de Datos del archivo de Audio abierto.
		///
		/// Automaticamente se genera la Etiqueta y llena lo más que se pueda con los metadatos del archivo de audio al momento de abrir un nuevo
		/// file en el reproductor.
		/// @return Referencia a la Etiqueta de Datos generada.
		
		ELECTRODEMP_FORCE_INLINE const Etiqueta & Get_Tag () const {
			
			// Devolvemos la referencia constante a nuestra etiqueta del audio abierto.
			
			return Etiqueta_Audio;
			
		}
		
		/// @brief Acceso a la lista de Streams de Audios disponibles en nuestro archivo abierto.
		/// @return Cantidad de audios (streams) en el archivo abierto o 0 si no hay archivo de audio abierto.
		
		ELECTRODEMP_FORCE_INLINE size_t Get_StreamCount () const {
			
			// Devolvemos la cantidad de streams de audios en el decoder.
			
			return Decoder.Get_StreamCount ();
			
		}
		
		/// @brief Acceso a nuestro Indice de Host actualmente asignado.
		/// @return Indice de Host de PortAudio.
		
		ELECTRODEMP_FORCE_INLINE PaHostApiIndex Get_HostIndex () const {
			
			// Devolvemos nuestro indice de host actual.
			
			return Indice_Host;
			
		}
		
		/// @brief Accesp a la cantidad de Host Disponibles en nuestro S.O.
		/// @return La cantidad de Host API en nuestro sistema.
		
		ELECTRODEMP_FORCE_INLINE PaHostApiIndex Get_HostCount () const {
			
			// Devolvemos la cantidad de Host en nuestro dispositivo.
			
			return Pa_GetHostApiCount ();
			
		}
		
		/// @brief Acceso a una Lista de Strings de Host para nuestro programa.
		/// @return Una lista de host utilizables por nuestro reproductor.
		
		std::vector <std::string> Get_HostList () const;
		
		/// @brief Acceso a un nombre de Host a partir de su indice.
		/// @param Indice_H Indice de host a utilizar para obtener el nombre del mismo.
		/// @return Nombrel del host especificado.
		
		std::string Get_HostName (PaHostApiIndex Indice_H) const;
		
		/// @brief Tenemos ahora un metodo para recuperar el Index de determinado Host por medio de su nombre.
		/// @param Nombre_H Cadena con el Nombre del Host.
		/// @return Indice de Host utilizable por portaudio.
		
		PaHostApiIndex Get_HostByName (const std::string & Nombre_H) const;
		
		/// @brief Acceso a nuestro Indice de Dispositivo de portaudio.
		/// @return Indice de Device de PortAudio.
		
		ELECTRODEMP_FORCE_INLINE PaDeviceIndex Get_DeviceIndex () const {
			
			// Devolvemos nuestro indice de dispositivo actual.
			
			return Indice_Device;
			
		}
		
		/// @brief Acceso a la cantidad de Dispositivos de salida en nuestro S.O.
		/// @return La cantidad de dispositivos de salida totales en nuestro sistema.
		
		int Get_DeviceCount () const;
		
		/// @brief Acceso a la cantidad de Dispositivos de salida para determinado Host.
		/// @param Indice_H Host Api del cual contar sus dispositivos en el rango [0 , @ref Get_HostCount].
		/// @return La cantidad de dispositivos de saliuda de determinado Host.
		
		int Get_DeviceCount (PaHostApiIndex Indice_H) const;
		
		/// @brief Acceso a toda una Lista de Dispositivos de Salida en nuestro Sistema.
		/// @return Una lista de Strings con el nombre de cada Device de Output.
		
		std::vector <std::string> Get_DeviceList () const;
		
		/// @brief Acceso a un nombre de Dispositivo a partir de su indice.
		/// @param Indice_D Indice de dispositivo a utilizar para obtener el nombre del mismo.
		/// @return Nombrel del dispositivo especificado.
		
		std::string Get_DeviceName (PaDeviceIndex Indice_D) const;
		
		/// @brief Acceso a una Lista de Nombres de Dispositivos de Salida para cierto Host.
		/// @param Indice_H Indice de Host a utilizar.
		/// @return Una lista de dispositivos de salida utilizables por nuestro Host.
		
		std::vector <std::string> Get_DeviceList (PaHostApiIndex Indice_H) const;
		
		/// @brief Acceso a un nombre de Dispositivo a partir de su indice de host e indice de dispositivo.
		/// @param Indice_H Indice de host a utilizar.
		/// @param Indice Indice de dispositivo de host a utilizar para obtener el nombre del mismo.
		/// @return Nombrel del dispositivo especificado.
		
		std::string Get_DeviceName (PaHostApiIndex Indice_H , int Indice) const;
		
		/// @brief Metodo para recuperar el Indice de Device a partir de su Nombre.
		/// @param Nombre_D Nombre del Device a consultar su Indice.
		/// @return Indice de Device utilizable para portaudio.
		
		PaDeviceIndex Get_DeviceByName (const std::string & Nombre_D) const;
		
		/// @brief Metodo para obtener el valor del Estado actual de nuestro reproductor de manera
		/// directa.
		/// @return Estado actual del Reproductor.
		/// @see Estado
		
		ELECTRODEMP_FORCE_INLINE int Get_State () const {
			
			// Devolvemos el valor de estado de nuestro reproductor.
			
			return Estado_Actual.load ();
			
		}
		
		/// @}
		
		// ----------------------  Estado Global  ------------------------------
		
		/// @name Estado Global.
		/// @brief Tenemos algunos metodos estaticos para determinar el estado actual de nuestro reproductor en general (todas las instancias).
		/// @{
		
		/// @brief Metodo estatico para determinar si el reproductor de audio fue inicializado correctamente ,
		/// esté es un estado compartido para todas las instancias de reproductor , por lo que solamente es requerido
		/// tener iniciada la biblioteca una sola vez para la aplicación.
		/// @return True si fue inicializada la biblioteca y todos sus componentes.
		
		static ELECTRODEMP_FORCE_INLINE bool Is_Initialized () {
			
			// Devolvemos el valor de la propiedad especifica.
			
			return Inicializado;
			
		}
		
		/// @brief Metodo para recuperar el numero de instancias del reproductor actualmente activas.
		/// @return Cantidad de reproductores inicializados correctamente.
		
		static ELECTRODEMP_FORCE_INLINE int Get_ReferenceCount () {
			
			// Devolvemos el total de referencias.
			
			return Referencias;
			
		}
		
		/// @}
		
		// ---------------------------  Estado  --------------------------------
		
		/// @name Estado.
		/// @brief Algunos metodos auxiliares para conocer el estado actual de esté reproductor inicializado.
		/// Usaremos algunas combinaciones de propiedades para determinar cada estado de manera correcta.
		/// @{
		
		/// @brief Metodo para determinar si nuestro reproductor es valido.
		/// El reproductor tiene estado de valido (true) si todos los elementos de la biblioteca fuerón
		/// correctamente inicializados , tiene asignado un Host Api y un Device para la salida de Audio y 
		/// el @ref Decodificador fue iniciado correctamente.
		/// @return True en caso de ser Valido o falso en caso contrario.
		
		ELECTRODEMP_FORCE_INLINE bool Is_Valid () const {
			
			// Devolvemos el valor de nuestra localidad y el campo inicializado.
			
			return (Inicializado && Valido);
			
		}
		
		/// @brief Metodo de acceso para determinar si tenemos el reproductor en el estado de Abierto. @ref E_Open.
		///
		/// @note El reproductor debe estar en el estado Abierto antes de poder reproducir un Audio.
		/// @return Devolvemos true si es el estado actual.
		
		ELECTRODEMP_FORCE_INLINE bool Is_Open () const {
			
			// Devolvemos true si es el estado actual.
			
			return (Estado_Actual.load () == E_Open);
			
		}
		
		/// @brief Metodo de acceso para determinar si tenemos el reproductor en el estado de Play. @ref E_Play.
		///
		/// @note El reproductor está actualmente @b activo y reproduciendo una song.
		/// @return Devolvemos true si es el estado actual.
		
		ELECTRODEMP_FORCE_INLINE bool Is_Play () const {
			
			// Devolvemos true si es el estado actual.
			
			return (Estado_Actual.load () == E_Play);
			
		}
		
		/// @brief Metodo de acceso para determinar si tenemos el reproductor en el estado de Pause. @ref E_Pause.
		///
		/// @note El reproductor está actualmente @b inactivo y por lo tanto podemos solamente realizar acciones como
		/// reanudar la canción ( @ref Play ) , detener ( @ref Stop) ó Cerrar al audio actual ( @ref Close ).
		/// @return Devolvemos true si es el estado actual.
		
		ELECTRODEMP_FORCE_INLINE bool Is_Pause () const {
			
			// Devolvemos true si es el estado actual.
			
			return (Estado_Actual.load () == E_Pause);
			
		}
		
		/// @brief Metodo de acceso para determinar si tenemos el reproductor en el estado de Stop. @ref E_Stop.
		///
		/// @note El reproductor está actualmente @b inactivo y reseteado en el origen , podemos solamente realizar acciones como
		/// comenzar la canción ( @ref Play ) ó Cerrar al audio actual ( @ref Close ).
		/// @return Devolvemos true si es el estado actual.
		
		ELECTRODEMP_FORCE_INLINE bool Is_Stop () const {
			
			// Devolvemos true si es el estado actual.
			
			return (Estado_Actual.load () == E_Stop);
			
		}
		
		/// @brief Metodo de acceso para determinar si tenemos el reproductor en el estado de Close. @ref E_Close.
		///
		/// @note El reproductor está actualmente cerrado , por lo que no podemos realizar ninguna otra acción que no sea
		/// abrir un audio nuevo ( @ref Open , @ref Open_Selection ).
		/// @return Devolvemos true si es el estado actual.
		
		ELECTRODEMP_FORCE_INLINE bool Is_Close () const {
			
			// Devolvemos true si es el estado actual.
			
			return (Estado_Actual.load () == E_Close);
			
		}
		
		/// @}
		
		// ------------------------  Setters  ----------------------------------
		
		/// @name Colocadores de Datos.
		/// @brief Conjunto de metodos para modificar los datos de nuestro reproductor.
		/// @{
		
		/// @brief Metodo para colocar un Host especifico a nuestro reproductor.
		/// @param Indice_H Indice de nuestro Host en el rango [0 , @ref Get_HostCount ()].
		
		void Set_Host (PaHostApiIndex Indice_H);
		
		/// @brief Metodo para colocar un Device especifico en nuestro reproductor.
		/// @param Indice_D Indice de nuestro Dispositivo en el rango [0 , @ref Get_DeviceCount () - 1].
		
		void Set_Device (PaDeviceIndex Indice_D);
		
		/// @brief Metodo para colocar un Host y Device determinado en el reproductor.
		/// @param Indice_H Indice de Host a utilizar.
		/// @param Indice_D Indice de Device en el rango [0 , @ref Host->deviceCount - 1]
		
		void Set_Device (PaHostApiIndex Indice_H , int Indice);
		
		/// @}
		
		// -----------------------  Reproducción  ------------------------------
		
		/// @name Reproducción
		/// @brief Elementos para la reproducción de nuestro Audio a partir de un archivo.
		/// @{
		
		/// @brief Metodo para abrir un nuevo audio desde un archivo de musica/video en los siguientes formatos posibles.
		/// 
		///	Los formatos que acepta el reproductor son los siguientes :
		///	@li @b Audio AAC (*.aac , *.m4a).
		/// @li @b Audio AC3 (*.ac3).
		/// @li @b Audio OGG (*.ogg , *.oga , *.ogv).
		///	@li @b Audio FLAC (*.flac)
		/// @li @b Audio MP3 (*.mp3).
		/// @li @b Audio WAV (*.wav).
		/// @li @b Video (*.mp4 , *.mov , *.3gp)
		/// Todos los formatos que tengan soporte para multiples flujos de audio (streams) seran separados por cada flujo y se tendra que
		/// seleccionar alguno de todos los posibles con el metodo @ref Open_Selection (indice) antes de reproducir el audio.
		/// @note Si el audio solamente cuenta cón 1 stream , automaticamente se abrira para poder comenzar su reproducción.
		/// @param Filename Archivo de Audio en alguno de los formatos soportados.
		/// @return True en caso de ser correcta la apertura de audio o falso en caso de error.
		
		bool Open (const std::string & Filename);
		
		/// @brief Ademas del metodo Open , tenemos el siguiente metodó complementario para poder abrir un stream de audio en especifico
		/// si el formato de archivo cuentá con más de uno.
		///
		/// Para saber cuantos streams tiene el archivo de audio abierto tenemos el metodo @ref Get_StreamCount.
		/// @param Indice Indice de stream a seleccionar para reproducir en el rango de 0 a (Get_StreamCount () - 1).
		/// @return El resultado de está operación.
		
		bool Open_Selection (size_t Indice = 0u);
		
		/// @brief Metodo para Iniciar la Reproducción de nuestro audio previamente abierto.
		///
		/// En caso de haber iniciado correctamente nuestro audio abierto , el estado del reproductor cambia a @ref Estado::E_Play.
		/// @return True en caso de tener un resultado correcto y el reproductor comenzara la reproducción del audio en segundo
		/// plano , falso en caso de error o el reproductor no es valido en estos momentos.
		/// @see Open
		
		bool Play ();
		
		/// @brief Metodo para Pausar la Reproducción de nuestro audio previamente iniciado con @ref Play.
		///
		/// En caso de haber pausado correctamente nuestro audio reproducido , el estado del reproductor cambia a @ref Estado::E_Pause.
		/// @return True en caso de tener ó no presentar error en está operación , falso en caso contrario o por algún motivo que
		/// puede ser causa de nuestro archivo de entrada o el dispositivo seleccionado.
		
		bool Pause ();
		
		/// @brief Metodo para Detener la Reproducción de nuestro audio previamente iniciado con @ref Play.
		///
		/// En caso de haber detenido correctamente nuestro audio , el estado del reproductor cambia a @ref Estado::E_Stop.
		/// @return True en caso de tener error al realizar está acción , falso en caso de haber algún error al intentar detener
		/// el stream de audio actual.
		
		bool Stop ();
		
		/// @brief Metodo para realizar la terminación del archivo previamente abierto.
		/// 
		/// Es necesario llamar esté metodo siempre que se desee abrir un nuevo archivo de audio para poder finalizar correctamente
		/// la información de portaudio , liberar recursos ocupados por el audio anterior (si es que teniamos alguno abierto)
		/// y reiniciar el contador de samples entre muchos otros elementos.
		/// @return True para indicar que fue cerrado exitosamente nuestro archivo de audio , falso para el caso contrario.
		
		bool Close ();
		
		/// @brief Metodo para desplazar el reproductor de audio a determinado segundo en el rango de tiempo valido.
		///
		/// Tenemos este metodo para realizar un desplazamiento de nuestro audio a determinado tiempo en segundos , usaremos este metodo
		/// en conjunto con una barra de progreso para movernos por el audio actual en estado de pausa , play o stop.
		/// @param Second Tiempo en segundos a mover el audio.
		/// @return True en caso de no haber error.
		/// @see Play , Pause , Stop.
		
		bool Seek (size_t Second);
		
		/// @}
		
		// ---------------------------  Samples  -------------------------------
		
		/// @name Metodos para determinar Samples.
		/// @brief Contamos con algunos metodos para obtener información sobre nuestro flujo de audio abierto actualmente , tal como el
		/// total de muestras a procesar y la muestra actualmente en procesamiento , duración en segundos , cambiar de muestra a segundo , etc.
		/// @{
		
		/// @brief Metodo para recuperar el total de Muestras del audio abierto actualmente.
		/// @return Total de muestras en archivo de audio abierto ó 0 en caso de no tener alguno abierto.
		
		ELECTRODEMP_FORCE_INLINE size_t Get_SampleCount () const {
			
			// Devolvemos la cantidad de muestras que indica el decoder.
			
			return ((Estado_Actual != E_Close) ? Decoder.Get_SampleCount () : 0u);
			
		}
		
		/// @brief Tenemos el siguiente metodo que devuelve la muestra actualmente procesando , usaremos la llamada de esta forma para poder
		/// determinar en que instante de tiempo estamos.
		/// @return Devuelve el indice de muestra actualmente procesando.
		
		ELECTRODEMP_FORCE_INLINE size_t Get_ActualSample () const {
			
			// Devolvemos la muestra actual que estamos procesando.
			
			return ((Estado_Actual != E_Close) ? Decoder.Samples.Get_ProcessedSamples () : 0u);
			
		}
		
		/// @brief Metodo para recuperar la duración del audio actual en segundos.
		/// @return Duración del audio en segundos o 0 si no hay audio abierto.
		
		ELECTRODEMP_FORCE_INLINE size_t Get_Duration () const {
			
			// Devolvemos la duración de nuestro audio.
			
			return ((Estado_Actual != E_Close) ? Decoder.Get_Duration () : 0u);
			
		}
		
		/// @brief Metodo para recuperar el tiempo en segundos que le corresponde a determinada muestra de audio.
		/// @param Sample Muestra la cual pasar a tiempo en segundos.
		/// @return Tiempo en segundos que corresponde a la muestra indicada.
		
		ELECTRODEMP_FORCE_INLINE size_t Get_TimeSample (size_t Sample) const {
			
			// Devolvemos la duración de nuestro audio.
			
			return ((Estado_Actual != E_Close) ? Decoder.Get_TimeSample (Sample) : 0u);
			
		}
		
		/// @brief Metodo para recuperar el segundo que estamos procesando actualmente de nuestro audio con base a la muestra actualmente
		/// procesando , usaremos una conversión de muestra a tiempo.
		/// @return El segundo actualmente procesando o 0 si no hay audio reproduciendo.
		
		ELECTRODEMP_FORCE_INLINE size_t Get_ActualTime () const {
			
			// Devolvemos la duración de nuestro audio.
			
			return ((Estado_Actual != E_Close) ? Decoder.Get_TimeSample (Decoder.Samples.Get_ProcessedSamples ()) : 0u);
			
		}
		
		/// @brief Metodo para obtener la muestra que corresponde a determinado tiempo en segundos
		/// @param Tiempo Tiempo especificado en segundos a pasar a muestra.
		/// @return Muestra del rango de 0 a Get_SampleCount () para determinado tiempo.
		
		ELECTRODEMP_FORCE_INLINE size_t Get_SampleTime (size_t Tiempo) const {
			
			// Devolvemos la duración de nuestro audio.
			
			return ((Estado_Actual != E_Close) ? Decoder.Get_SampleTime (Tiempo) : 0u);
			
		}
		
		/// @}
		
		// ----------------------------  Callback  -----------------------------
		
		/// @name Metodos de Callback
		/// @brief Contamos con un par de metodos para realizar acciones determinadas durante la reproducción de nuestro audio en el callback
		/// interno de portaudio , asi como para cuando se termina de reproducir todo el audio.
		/// @{
		
		/// @brief El tipo de Función para el Callback de Reproduccion.
		
		typedef void (* Tipo_Callback_Reproduccion) (Reproductor * , Muestreador::Muestras_Audio & , void *);
		
		/// @brief El tipo de Función para el Callback de Finalización.
		
		typedef void (* Tipo_Callback_Finalizacion) (Reproductor * , void *);
		
		/// @brief Metodo para la asignación de una función de callback de Reproducción la cual se invoca por cada paquete de @ref Decodificador::Muestras a reproducir.
		/// @tparam TypeFuncion Objeto funcional ó puntero a función de tipo @ref Tipo_Callback_Reproduccion.
		/// @param Funcion Callback a invocar durante la reproducción.
		/// @param Datos Datos adicionales para nuestra función. Pueden ser Null si no se necesitan.
		/// @see Tipo_Callback_Reproduccion
		
		template <typename TypeFuncion>
		void Set_Callback_Reproduccion (TypeFuncion Funcion , void * Datos = nullptr) {
			
			// Asignaremos nuestra función actual al callback de nuestro reproductor.
			
			Funcion_Reproduccion = std::forward <typename std::decay <TypeFuncion>::type> (Funcion);
			
			// Asignaremos nuestros datos adicionales.
			
			Datos_Callback_Reproduccion = Datos;
			
		}
		
		/// @brief Metodo para la asignación de una función de callback de Finalización la cual es invocada al terminar la reproducción.
		/// @tparam TypeFuncion Objeto funcional ó puntero a función de tipo @ref Tipo_Callback_Finalizacion.
		/// @param Funcion Callback a invocar al terminar de reprucir el audio.
		/// @param Datos Datos adicionales para nuestra función. Pueden ser Null si no se necesitan.
		/// @see Tipo_Callback_Finalizacion
		
		template <typename TypeFuncion>
		void Set_Callback_Finalizacion (TypeFuncion Funcion , void * Datos = nullptr) {
			
			// Asignaremos nuestra función actual al callback de nuestro reproductor.
			
			Funcion_Finalizacion = std::forward <typename std::decay <TypeFuncion>::type> (Funcion);
			
			// Asignaremos nuestros datos adicionales.
			
			Datos_Callback_Finalizacion = Datos;
			
		}
		
		/// @}
		
		// --------------------------  Destructor  -----------------------------
		
		/// @brief Nos aseguramos de eliminar todos los elementos y cerrar el archivo de audio actual.
		
		virtual ~ Reproductor ();
	
	// Tenemos los siguientes metodos privados para el uso interno del reproductor.
	
	private :
		
		// Tenemos el siguiente metodo el cual determina si un Device determinado es de salida o solamente de
		// entrada para validar antes de asignar.
		
		bool Is_Output (PaDeviceIndex Indice_D) const;
		
		// --------------------------  Streaming  ------------------------------
		
		// Vamos a declarar todos los metodos internos para realizar las diferentes etapas de la reproducción del audio , desde la apertura
		// del stream hasta el cierre del mismo de manera privada para poder reutilizar cada vez que se requiera.
		
		// Tenemos el siguiente metodo el cual realiza la creación y apertura de un nuevo stream de audio para el stream de audio actualmente
		// indado con el valor especifico.
		
		bool Open_Stream (size_t Indice);
		
		// Implementaremos el metodo para inicializar el flujo de audio y con estó poner en marcha la reproducción de audio actualmente abierto
		// con el metodo de apertura.
		
		bool Play_Stream ();
		
		// Implementaremos el metodo para pausar el flujo de audio actualmente iniciado , esté detendra el stream de audio de dos maneras posibles.
		// Ya sea abortando el audio (detiene el audio de manera rapida) o deteniendolo de manera normal para que termine de procesar la muestra
		// actual. Por defecto se detiene el audio.
		
		bool Pause_Stream (bool Abort = false);
		
		// Ahora implementaremos el metodo para detener el audio por completo , esté liberara algunos recursos utilizados y dejara el contador
		// de muestras reseteado para comenzar el flujo nuevamente desde el origen del audio. Igual que el caso anterior presenta dos formas
		// de detener el flujo.
		
		bool Stop_Stream (bool Abort = false);
		
		// Implementaremos el metodo para Cerrar el flujo de audio actual , es la contraparte de Open_Stream y libera todos los recursos que
		// ya no se requieran para poder terminar con el archivo actual y cambiarlo por otro o cerrar la aplicación.
		
		bool Close_Stream ();
		
		// -------------------------  Callbacks  -------------------------------
		
		// Ahora vamos a realizar la declaración formal de nuestra función de callback estatica la cual sera llamada cada vez que portaudio
		// necesite orequiera procesar más muestras de audio , hasta terminar con estás , tenemos entonces el siguiente metodo el cual sera
		// el encargado de realizar todo el procesamiento en background para nuestro audio asi como el que invoca a nuestro functor de callback
		// para realizar acciones adicionales.
		
		// El metodo tiene el siguiente formato :
		// (const void * input , void * output , unsigned long frameCount ,
		// const PaStreamCallbackTimeInfo * timeInfo , PaStreamCallbackFlags statusFlags , void *userData) : int
		
		static int Callback_Reproduccion (const void * Entrada , void * Salida , unsigned long int Cantidad_Frames ,
			const PaStreamCallbackTimeInfo * Informacion_Tiempo , PaStreamCallbackFlags Estado_Stream , void * Datos);
		
		// El siguiente será el metodo de callback para cuando se termine de procesar nuestro audio actual y tengamos que realizar determinadas
		// acciones adicionales , tambien tenemos presente el callback a invocar para poder realizar procesamiento adicional al momento de finalizar
		// por lo que requerimos este metodo estatico.
		
		// El formato es simplemente : void (void * Datos) donde Datos es el mismo valor pasado a la función de callback anterior.
		
		static void Callback_Finalizacion (void * Datos);
	
};

/// @}

} // namespace ElectrodeMP.

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ELECTRODEMP_REPRODUCTOR_H
