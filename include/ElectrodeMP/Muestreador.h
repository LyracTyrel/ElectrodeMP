
/// @file Muestreador.h
///	@brief Tenemos el siguiente archivo de codigo para nuestras clases :  Muestras y Resampler.
///
/// Tiene como finalidad la coordinación entre el decodificador y el reproductor de Audio al proporcionar una simple interfaz
/// de intercambio entre muestras decodificadas y muestras a reproducir en un contenedor de queue para su colocación al momento
/// de ser solicitadas y decodificadas.
/// @author Loubiner
/// @date Saturday 2/17/2018

#ifndef ELECTRODEMP_MUESTREADOR_H
#define ELECTRODEMP_MUESTREADOR_H

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

// Elementos de contenedores como el deque y la cola.

#include <deque>
#include <queue>

// Incluiremos nuestros threads , mutex , condiciones variables y nuestros datos atomicos.

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

// Funciones lambda de C++ 11.

#include <functional>

// Tipos de datos validados.

#include <type_traits>

// Limites numericos para el Resample.

#include <limits>

// Algoritmos numericos de C++.

#include <algorithm>

// Asi como las utilerias para trabajar con memorias y elementos de C++ 17.

#include <memory>
#include <utility>

// -------------------------  Configuracion  -----------------------------------

// Empezaremos a incluir nuestra configuración para todo el proyecto en general.

#include <ElectrodeMP/Configuracion.h>

// -----------------------------  LOG  -----------------------------------------

// Incluiremos el LOG para registrar los procesos por los que pasa ElectrodeMP.

#include <ElectrodeMP/LOG.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Incluiremos todo el codigo dentro del siguiente espacio de nombres.

namespace ElectrodeMP {

// Vamos a declarar un conjunto de clases y estructuras las cuales serviran como elementos intermedios entre el decodificador y el procesador de audio
// para la correcta reproducción del audio por nuestro dispositivo elegido.

/// @defgroup Muestreador
/// @brief Clases de utilidad para la syncronización de Muestras entre el @ref Decodificador y el @ref Reproductor de Audio.
/// @{

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Empezaremos declarando y definiendo la clase Generica de un Resampler sensillo hecho con valores de Limites especificos para los tipos de Entrada y
// de salida.

// ----------------------  Resampler Generico   --------------------------------

/*
/// @brief Vamos a declarar una interfaz de Resmuestreador que permita remuestrear un paquete de datos de entrada a datos de salida en otro formato y
/// permita interpolar los valores a otro rango especificado por los limites del tipo de salida y entrada respectivamente.

struct Generic_Resampler {
	
	// --------------------------  Constructor  --------------------------------
	
	/// @brief Tenemos el constructor symbolico vacio por defecto para nuestra estructura actual.
	
	Generic_Resampler () = default;
	
	// -------------------------  Remuestreador  -------------------------------
	
	/// @brief Metodo remuestreador implementado en la subclase especializada @ref Resampler con los tipos de datos indicados.
	/// @param Salida Buffer de Samples de Salida.
	/// @param Entrada Buffer de Samples de Entrada.
	/// @param Cantidad Numero de Samples en ambos Buffers.
	
	virtual void Remuestrear (void * Salida , const void * Entrada , size_t Cantidad) const;
	
};
*/

// --------------------------  Resampler  --------------------------------------

/// @brief Declararemos una clase de plantilla la cual nos permita realizar la correcta asignación de valores de muestra a nuestro programa mediante el
/// uso de limites numericos. 
///	
/// Usaremos como parametros de plantilla nuestros tipos de entrada y salida para el remuestreo de un buffer de datos
/// determinado.
/// @tparam TypeInputSample Tipo de Muestra de Entrada.
/// @tparam TypeOutputSample Tipo de Muestra de Salida.

template <typename TypeInputSample , typename TypeOutputSample>
struct Resampler {
	
	/// @brief Comenzaremos declarando los Limites en expresión constante para nuestros tipos de datos de cada muestra. Usaremos el valor minimo -1,0 y
	/// maximo 1.0 para los datos de tipo punto flotante , y para el resto el resultado devuelto por numeric_limits.
	/// 
	/// Determinaremos en la siguiente localidad constante y estatica si el tipo de Dato de Entrada y Salida es de tipo punto flotante.
	
	static constexpr bool Input_Floating = std::is_floating_point <TypeInputSample>::value;
	static constexpr bool Output_Floating = std::is_floating_point <TypeOutputSample>::value;
	
	/// @brief Ahora determinamos el valor Numerico Maximo y Minimo para el tipo de dato de Entrada.
	
	static constexpr TypeInputSample Input_Max = (Input_Floating ? static_cast <TypeInputSample> (1.0) : std::numeric_limits <TypeInputSample>::max ());
	static constexpr TypeInputSample Input_Min = (Input_Floating ? static_cast <TypeInputSample> (-1.0) : std::numeric_limits <TypeInputSample>::min ());
	
	/// @brief Valor Numerico Maximo y Minimo para el tipo de dato de Salida.
	
	static constexpr TypeOutputSample Output_Max = (Output_Floating ? static_cast <TypeOutputSample> (1.0) : std::numeric_limits <TypeOutputSample>::max ());
	static constexpr TypeOutputSample Output_Min = (Output_Floating ? static_cast <TypeOutputSample> (-1.0) : std::numeric_limits <TypeOutputSample>::min ());
	
	/// @brief A partir de estos valores podemos determinar cual es el Rango de Entrada y Salida respectivamente con la diferencia entre el valor maximo y el minimo.
	/// 
	/// Estos valores nos serviran para realizar nuestro remuestreo.
	
	static constexpr long double Input_Range = (static_cast <long double> (Input_Max) - Input_Min);
	static constexpr long double Output_Range = (static_cast <long double> (Output_Max) - Output_Min);
	
	/*
	
	// --------------------------  Constructor  --------------------------------
	
	/// @brief Tenemos el constructor symbolico vacio por defecto para nuestra estructura actual.
	
	Resampler () = default;
	
	*/
	
	// -------------------------  Remuestreador  -------------------------------
	
	/// @brief Ahora vamos a declarar nuestro metodo estatico el cual realiza el cambio de Muestra de un tipo de Entrada a uno de Salida usando los valores de limites
	/// actualmente asignados para un buffer de Entrada y asignando el resultado en un buffer de salida.
	/// 
	/// Tenemos a la entrada la cantidad de Muestras a remuestrear y devolvemos true en caso de no haber error , asi que.
	/// @param Salida Buffer de Samples de Salida.
	/// @param Entrada Buffer de Samples de Entrada.
	/// @param Cantidad Numero de Samples en ambos Buffers.
	
	static void Remuestrear (TypeOutputSample * Salida , const TypeInputSample * Entrada , size_t Cantidad);
	
	/// @brief Implementaremos de manera inline una función que simplemente limite nuestro valor especifico a un intervalo de Min a Max para nuestra estructura actual
	/// devolviendo el valor resultante con el tipo de dato de salida.
	/// @param Entrada Muestra a limitar en los rangos de este Resampler.
	/// @return La muestra en los rangos [Output_Min , Output_Max]
	
	static ELECTRODEMP_FORCE_INLINE TypeOutputSample Clamp (const TypeOutputSample Entrada) {
		
		// Devolvemos el resultado de aplicar : max (min (Entrada , Max) , Min).
		
		return std::max <TypeOutputSample> (std::min <TypeOutputSample> (Entrada , Output_Max) , Output_Min);
		
	}
	
};

// ------------------------  Resampler Definición ------------------------------

// Definiremos el codigo para nuestra estructura auxiliar de Resample la cual se encarga de cambiar los valores de un buffer de muestras de Entrada y asignarlos
// a uno de salida con la conversión entre los tipos de Entrada y Salida respectivos.

template <typename TypeInputSample , typename TypeOutputSample>
void Resampler <TypeInputSample , TypeOutputSample>::Remuestrear (TypeOutputSample * Salida , const TypeInputSample * Entrada , size_t Cantidad) {
	
	// Vamos a realizar una iteración por el buffer de Entrada y el de Salida usando el valor de Cantidad como limite.
	
	for (size_t Indice_Muestra = 0u ; Indice_Muestra < Cantidad ; ++ Indice_Muestra) {
		
		// Obtendremos primero el valor de nuestra Entrada constante en un tipo long double.
		
		const long double Valor_Entrada = static_cast <long double> (Entrada [Indice_Muestra]);
		
		// Ahora pasaremos a convertir este valor del rango [I_Min , I_Max] al rango [O_Min , O_Max] usando la siguiente formula.
		//		Valor_Salida = (((Valor_Entrada - Input_Min) * Output_Range) / Input_Range) + Output_Min
		
		const long double Valor_Salida = (((Valor_Entrada - Input_Min) * Output_Range) / Input_Range) + Output_Min;
		
		// Finalmente el valor de salida lo convertiremos al tipo de dato de salida y lo limitaremos a su intervalo [Min , Max] con nuestro
		// metodo Clamp.
		
		Salida [Indice_Muestra] = Clamp (static_cast <TypeOutputSample> (Valor_Salida));
		
	}
	
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// ------------------------------  Muestras  -----------------------------------

/// @brief Estructura de Datos de Muestras para su Reproducción.
/// Nuestra información decodificada sera almacenada en paquetes de Muestras de un Tamaño dinamico el cual sera repartido
/// al momento de seleccionar el decodificador. Usaremos esta estructura para contener la información que servira como
/// datos para nuestro reproductor.

template <typename TypeSampleBase>
struct Muestras {
	
	/// @brief Definición del Tipo de Dato de Cada Muestra.
	
	typedef TypeSampleBase TypeSample;
	
	/// @brief
	/// Tenemos contenido en primer lugar nuestro buffer de Datos de tamaño dinamico. Cada muestra está normalizada al intervalo de
	/// -1.0f a 1.0f como minimo y maximo respectivamente. Este primero corresponde a las muestras del Canal Izquierdo.
	
	std::shared_ptr <TypeSample> Datos_L;
	
	/// @brief
	/// Tenemos ahora nuestro buffer para los Datos de nuestro Canal Derecho , de igual forma está normalizada.
	
	std::shared_ptr <TypeSample> Datos_R;
	
	/// @brief
	/// El tamaño en (TypeSample) de nuestros Datos. O la cantidad de Muestras contenidas.
	
	size_t Cantidad = 0u;
	
	/// @brief
	/// El indice de Datos para su consumó (de uso para el reproductor)
	
	size_t Indice = 0u;
	
	// ----------------------------  Constructor  ------------------------------
	
	/// @brief
	/// Tenemos el constructor por defecto para nuestras Muestras , vacio.
	
	Muestras () = default;
	
	/// @brief Constructor para iniciar los datos de las muestras.
	///
	/// Tenemos el constructor con los Datos y la cantidad de ellos.
	/// @param Datos_M Muestras para nuestro paquete.
	/// @param Cantidad_M Cantidad total de Muestras en el paquete.
	/// @note Tomaremos poseción de la memoria de Datos_M.
	
	explicit Muestras (TypeSample * Datos_ML , TypeSample * Datos_MR , size_t Cantidad_M) : 
		Datos_L (Datos_ML , [] (TypeSample * Samples) -> void { delete [] (Samples); }) ,
		Datos_R (Datos_MR , [] (TypeSample * Samples) -> void { delete [] (Samples); }) , Cantidad (Cantidad_M) {}
	
	// -------------------------  Procesador  ----------------------------------
	
	/// @brief Metodo para realizar el procesamiento de los Samples de Entradas a Samples de Salida utilizando el Sampler especifico para las
	/// muestras dependiendo de nuestro tipo de Muestra seleccionada.
	///
	/// Tomaremos los buffers de entrada y los pasaremos a separar en canales Izquierdo y Derecho los cuales seran normalizados a valores respectivos conforme
	/// el Tipo de Sample elegido para nuestros datos.
	/// @tparam TypeSampleInput Tipo de los samples de entrada en el buffer.
	/// @param Samples Samples o muestras decodificadas por un decoder seleccionado.
	/// @param Cantidad Numero de Samples en el buffer de entrada por cada Canal.
	/// @param Size_Buffer Tamaño del Buffer en Bytes por cada Canal.
	/// @param Entrelazado Indica si el buffer de entrada es entrelazado (valores para cada canal intercalados) o planar (separados cada canal).
	/// @return Muestras procesadas y normalizadas al intervalo de los limites de nuestro Sampler.
	
	template <typename TypeSampleInput>
	static Muestras <TypeSample> Procesar (const TypeSampleInput * const * Samples , size_t Cantidad , size_t Canales , size_t Size_Buffer , bool Entrelazado);
	
};

// --------------------------  Muestras Definición  ----------------------------

// Definiremos ahora nuestro metodo privado auxiliar para realizar el remuestreo de nuestros Samples que tenemos en determinado Frame , este metodo tambien
// ayuda a desentrelazar el audio si es el caso de acuerdo al parametro especificado.

template <typename TypeSampleBase>
template <typename TypeSampleInput>
Muestras <TypeSampleBase> Muestras <TypeSampleBase>::Procesar (const TypeSampleInput * const * Samples , size_t Cantidad , size_t Canales ,
	size_t Size_Buffer , bool Entrelazado) {
	
	// Lo primero a realizar será la repartición de Muestras para nuestra Salida en cada canal , estos tendran una capacidad de muestras igual a la cantidad
	// de Samples de entrada que tenemos.
	
	TypeSample * Muestras_I = new TypeSample [Cantidad];
	TypeSample * Muestras_D = new TypeSample [Cantidad];
	
	// Ahora vamos a declarar nuestro par de Array de Samples para la Entrada de datos , de igual forma tenemos el del canal izquierdo y derecho , estos
	// seran utilizados para realizar el resampling de nuestro audio.
	
	TypeSampleInput * Samples_I = nullptr;
	TypeSampleInput * Samples_D = nullptr;
	
	// Ahora pasaremos a validar si nuestras muestras de Entrada son entrelazadas o no , y con esto poder continuar con el resto de codigo.
	
	if (Entrelazado) {
		
		// ------------------------  Samples Entrelazados  ---------------------
		
		// Esto quiere decir que los datos de cada canal están alineadas en un mismo paquete : IIIIDDDDDIIIIIDDDD , por lo que deberemos asignar en ambos
		// casos , la salida sobre cada buffer respectivo y leer de un mismo buffer cuyo tamaño esta determinado por el Numero de Canales multiplicado por el
		// tamaño de cada muestra y multiplicado por la cantidad de muestras.
		
		// Tenemos ahora nuestro arreglo de samples de la siguiente manera.
		
		const TypeSampleInput * const Samples_Data = Samples [0]; // Solo tenemos el primer plano valido.
		
		// El limite de Muestras en este array de datos es igual a : Canales * Cantidad.
		
		const size_t Limite_Samples = (Canales * Cantidad);
		
		// Empezaremos entonces creando un par de buffers temporales para desentrelazar nuestras muestras en la entrada , por lo que crearemos el siguiente array
		// para cada canal.
		
		Samples_I = new TypeSampleInput [Cantidad];
		Samples_D = new TypeSampleInput [Cantidad];
		
		// En el caso de tener un solo canal pasaremos a realizar lo siguiente.
		
		if (Canales == 1u) {
			
			// Iteramos por cada muestra en nuestros datos de entrada hasta alcanzar el limite de samples y cón esto lograr desentrelazar el audio.
			
			for (size_t Indice_Sample = 0u ; Indice_Sample < Limite_Samples ; ++ Indice_Sample) {
				
				// Asignaremos el mismo valor en el canal izquierdo y derecho.
				
				Samples_I [Indice_Sample] = Samples_Data [Indice_Sample];
				Samples_D [Indice_Sample] = Samples_Data [Indice_Sample];
				
			}
			
		}
		else {
			
			// Iteramos por cada muestra en nuestros datos de entrada hasta alcanzar el limite de samples y cón esto lograr desentrelazar el audio.
			
			for (size_t Indice_Sample = 0u ; Indice_Sample < (Limite_Samples - 1u) ; Indice_Sample += Canales) {
				
				// Asignaremos el primer valor en el canal izquierdo y el segundo en el derecho.
				
				Samples_I [Indice_Sample / Canales] = Samples_Data [Indice_Sample];
				Samples_D [Indice_Sample / Canales] = Samples_Data [Indice_Sample + 1u];
				
			}
			
		}
		
	}
	else {
		
		// ------------------------  Samples Planares  -------------------------
		
		// Aqui solamente queda asignar a nuestras muestras de entrada los canales respectivos de nuestros datos conforme lo establecido en FFmpeg. Si
		// es el caso de canal monoestereo entonces :
		
		Samples_I = const_cast <TypeSampleInput *> (Samples [0]);
		Samples_D = const_cast <TypeSampleInput *> (Samples [((Canales > 1) ? 1 : 0)]);
		
	}
	
	// ---------------------------  Remuestreo  --------------------------------
	
	// Finalmente realizaremos el remuestreo de nuestros datos de Entrada y Salida respectivamente , asignando el resultado en nuestros datos de buffer
	// los cuales pasaremos a una estructura Muestra antes de Terminar. El remuestreó unicamente aplica cuando los tipos de las muestras de entrada y salida
	// son diferentes.
	
	if (!std::is_same <TypeSampleInput , TypeSample>::value) {
		
		// Realizaremos el remuestreo para el lado izquierdo y derecho.
		
		Resampler <TypeSampleInput , TypeSample>::Remuestrear (Muestras_I , Samples_I , Cantidad);
		Resampler <TypeSampleInput , TypeSample>::Remuestrear (Muestras_D , Samples_D , Cantidad);
		
	}
	else {
		
		// Copiaremos todos los elementos de la entrada en la nueva salida.
		
		std::copy (Samples_I , (Samples_I + Cantidad) , Muestras_I);
		std::copy (Samples_D , (Samples_D + Cantidad) , Muestras_D);
		
	}
	
	// Eliminaremos la memoria repartida para los entrelazados si es el caso.
	
	if (Entrelazado) {
		
		// Borramos nuestros arrays temporales.
		
		delete [] (Samples_I);
		delete [] (Samples_D);
		
	}
	
	// Y devolvemos finalmente una Muestra con nuestros datos de Samples asignados.
	
	return std::move (Muestras <TypeSample> (Muestras_I , Muestras_D , Cantidad));
	
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// ------------------------------  Muestreador  --------------------------------

/// @brief Clase especial para realizar el flujo de muestras entre el @ref Decodificador y el @ref Reproductor , tenemos incluida en está
/// los campos necesarios para coordinar los threads lanzados en ambos elementos. La mayoria de campos en está clase son atomicos por lo cual es
/// segura para compartir entre los threads del Reproductor y el Decodificador.
///
/// Compuesta principalmente por un mutex y una condición variable la cual dependera de en que modulo nos encontremos y una cola de @ref Muestras
/// la cual sera compartida tanto en el programa del Reproductor como en el programa de el Decodificador.

class Muestreador {
	
	// Tenemos el tipo de Muestras publico para nuestros modulos.
	
	public :
		
		/// @brief Definiremos el tipo de Muestra a utilizar.
		
		typedef Muestras <ELECTRODEMP_SAMPLE_TYPE> Muestras_Audio;
		
		/// @brief Ahora vamos a definir el tipo de queue a ocupar en nuestro muestreador actual.
		
		typedef std::queue <Muestras_Audio> Queue_Muestras;
	
	// Tenemos los siguientes objetos privados.
	
	private :
		
		// --------------------  Cola de  Muestras  ----------------------------
		
		// Tenemos la cola de muestras la cual sera nuestro contenedor principal para todos los valores a reproductir en el audio y los valores
		// de salida de nuestro decodificador despues de pasar por la etapa de Pre-Procesamiento. Declaramos este contenedor con base a una deque
		// de elementos variables.
		
		// Ahora pasaremos a declarar una Cola de Muestras la cual sera la base para nuestro reproductor. Será de este lugar de donde tome datos
		// para la reproducción asi como en donde el decodificador asigne paquetes de muestras para su reproducción. Tenemos que tener este dato
		// syncronozado entre el thread del reproductor y el thread del decodificador para que puedan realizar el trabajo adecuadamente.
		
		std::atomic <Queue_Muestras *> Queue;
		
		// Declaramos ahora un valor constante para la reproducción y decodificación , esté sera el total de Muestras que tenemos en nuestro muestreador
		// de manera volatil , de esta forma podremos contabilizar hasta que punto terminar el muestreo de datos.
		
		std::atomic <size_t> Total_Muestras;
		
		// Para llevar la cuenta del total de muestras que llevamos hasta este momento procesadas por el reproductor de audio y asignadas al
		// dispositivo de salida.
		
		std::atomic <size_t> Cantidad_Muestras_Procesadas;
		
		// Ahora para tener la cuenta de cuantas muestras tenemos decodificadas por el decoder está este otro valor.
		
		std::atomic <size_t> Cantidad_Muestras_Decodificadas;
		
		// Y para validar que nuestro decodificador terminó su decodificación tenemos el siguiente estado.
		
		std::atomic <bool> Decodificacion_Completa;
		
		// ---------------------  Syncronización  ------------------------------
		
		// La reproducción de nuestro audio se ejecuta por automatico en un Thread separado al igual que con nuestro decodificador , PortAudio lanza
		// esté thread al momento de iniciar nuestra reproducción. Este debe syncronizarse con nuestro thread de la decodificación para permitirle
		// iniciarse hasta que existan muestras en la cola de muestras.
		
		// Vamos a requerir por lo tanto un Mutex que nos ayude a mantener el estado actual de nuestro bloqueo.
		
		std::mutex Mutex_Sincronizacion;
		
		// Ahora vamos a declarar una condition variable la cual nos ayude a detener el reproductor hasta que el decodificador haya terminado de procesar
		// por lo menos un paquete de Muestras para su reproducción.
		
		std::condition_variable Condicion_Sincronizacion;
		
		// -------------------------  Requisitos  ------------------------------
		
		// Para poder establecer un requisito de muestras sobre el decoder , tenemos entonces que utilizar algunos elementos adicionales que le permitan
		// especificar hasta que cantidad de muestras decodificar y luego esperar a ser llamado nuevamente ,  estó con el fin de evitar decodificar
		// todas las muestras de audio en un inicio.
		
		// Requerimos utilizar un valor actual que represente la cantidad de Muestras a decodificar por el decoder para el proximo llamado al metodo
		// Extract , este valor sera del tipo siguiente.
		
		std::atomic <unsigned long int> Muestras_Solicitadas;
		
		// Ahora vamos a declarar un contador para tener el control sobre estás muestras solicitadas.
		
		std::atomic <unsigned long int> Muestras_Obtenidas;
	
	// Tenemos los siguientes estados para nuestro muestreador actual.
	
	public :
		
		/// # ----------------------------  Estado  -------------------------------
		
		/// @brief Nuestro muestreador cuenta principalmente con 6 estados , los cuales representan diferentes acciones que hay que considerar en
		/// el reproductor.
		/// 
		/// Contamos con los estados de :
		/// @li @b Iniciado : El muestreador fue iniciado a sus valores originales para iniciar una reproducción nueva.
		///	@li @b Play : El muestreador está activo de manera ordinaria por lo que el reproductor y el decodificador están funcionando correctamente.
		/// @li @b Pause : El decodificador y el reproductor pasarón a estado de pausa por lo que no hay flujo de muestras en el queue.
		/// @li @b Stop : El decodificador y el reproductor pasarón a estado de stop , el contador de muestras se reinicio a 0 y por tanto al reanudar
		///	la song , se comenzara desde la muestra en 0 a menos que se desplace por acciones de seek.
		/// @li @b Interrumpido : El muestreador está interrumpido por acciones del decodificador o reproductor por lo que hay que finalizar los threads 
		/// respectivos y detener la song.
		/// @li @b Completado : El muestreador fue completado , todas las muestras fuerón decodificadas/procesadas.
		/// 
		/// Enumeración de los estados por los que pasa nuestro Muestreador.
		
		enum Estado : short int {
			
			/// @brief Estao de Iniciado.
			
			E_Initialized = 1 ,
			
			/// @brief Estado de Play.
			
			E_Play ,
			
			/// @brief Estado de Pausa.
			
			E_Pause ,
			
			/// @brief Estado de Stop.
			
			E_Stop ,
			
			/// @brief Estado de Interrupción.
			
			E_Interrupt ,
			
			/// @brief Estado de Completado.
			
			E_Complete
			
		};
		
		/// @brief Para determinar en que estado está el muestreador , tenemos la siguiente flag atomica para comprobarlo.
		
		std::atomic <short int> Estado_Actual;
	
	// Ahora pasaremos a declarar los metodos publicos los cuales serán la base del funcionamiento de nuestra clase.
	
	public :
		
		// ------------------------  Constructores  ----------------------------
		
		/// @name Constructores
		/// @brief Metodos para la construcción del muestreador.
		/// @{
		
		/// @brief Tenemos el constructor por defecto el cual simplemente inicializa los valores por defecto de cada atributo.
		
		Muestreador ();
		
		/// @}
		
		// ---------------------------  Queue  ---------------------------------
		
		/// @name Metodos de la Queue.
		/// @brief Metodos para anexar o quitar elementos de Muestras en la Cola de Muestras actuales.
		/// @{
		
		/// @brief Tennemos primero el metodo para saber si la Queue actual es completamente valida , es decir , tiene un espacio
		/// de memoria asignado.
		/// @return True si la cola tiene memoria asignada.
		
		ELECTRODEMP_FORCE_INLINE bool Valid_Queue () const {
			
			// Devolvemos el estado de la queue actual.
			
			return (Queue.load () != nullptr);
			
		}
		
		/// @brief Tenemos el siguiente metodo para determinar si la queue está vacia , asi que pasaremos a realizar esto.
		/// @return True si la cola está actualmente vacia.
		
		ELECTRODEMP_FORCE_INLINE bool Empty_Queue () const {
			
			// Devolvemos el estado de la queue actual si es diferente a nullptr (valida).
			
			return (Valid_Queue () ? Queue.load ()->empty () : false);
			
		}
		
		/// @brief Metodo para poder iniciar la memoria de la Queue actual , asignando un espacio en memoria nuevo.
		
		void New_Queue ();
		
		/// @brief Anexaremos un nuevo elemento a la cola de muestras por copia.
		/// @param Entrada Nuevo conjunto de Samples a anexar al final de la cola.
		
		void Push_Queue (const Muestras_Audio & Entrada);
		
		/// @brief Anexaremos un nuevo elemento a la cola de muestras por copia.
		/// @param Entrada Nuevo conjunto de Samples a anexar al final de la cola por movimiento.
		
		void Push_Queue (Muestras_Audio && Entrada);
		
		/// @brief Eliminaremos el siguiente elemento en la cola de Muestras.
		
		void Pop_Queue ();
		
		/// @brief Limpiaremos toda nuestra cola de Elementos actuales.
		
		void Clear_Queue ();
		
		/// @brief Liberaremos el espacio de la cola de Elementos repartida hasta esté momento.
		
		void Delete_Queue ();
		
		/// @}
		
		// --------------------------  Get y Set  ------------------------------
		
		/// @name Getters y Setters.
		/// @brief Pequeño conjunto de metodos para acceder a los miembros más importantes en el muestreador.
		/// @{
		
		/// @brief Metodo para acceder a la Cantidad Total de Muestras en el Decodificador para el flujo de audio a reproductir.
		/// @return Cantidad de muestras totales a reproducir/decodificar.
		
		ELECTRODEMP_FORCE_INLINE size_t Get_TotalSamples () const {
			
			// Devolvemos el total de muestras asignado.
			
			return Total_Muestras.load ();
			
		}
		
		/// @brief Para obtener la cantidad de Muestras procesadas actualmente con el fín de determinar en que parte de nuestro procesamiento estamos
		/// actualmente.
		/// @return Cantidad de Muestras procesadas hasta este momento.
		
		ELECTRODEMP_FORCE_INLINE size_t Get_ProcessedSamples () const {
			
			// Devolvemos el total de muestras procesadas hasta ahora.
			
			return Cantidad_Muestras_Procesadas.load ();
			
		}
		
		/// @brief Para obtener la cantidad de Muestras decodificadas actualmente con el fín de determinar en que parte de nuestra decodificación estamos
		/// actualmente.
		/// @return Cantidad de Muestras decodificadas hasta este momento.
		
		ELECTRODEMP_FORCE_INLINE size_t Get_DecodedSamples () const {
			
			// Devolvemos el total de muestras decodificadas hasta ahora.
			
			return Cantidad_Muestras_Decodificadas.load ();
			
		}
		
		/// @brief Metodo para colocar una cantidad total de muestras a procesar en los threads respectivos.
		/// @param Cantidad Total de muestras a procesar para tener como una referencia para las validaciones.
		
		void Set_TotalSamples (size_t Cantidad);
		
		/// @brief Para colocar la cantidad actual de Muestras procesadas tenemos este metodo.
		/// @param Cantidad Numero de Muestras procesadas.
		
		void Set_ProcessedSamples (size_t Cantidad);
		
		/// @brief Adicionalmente tenemos el metodo que permite sumar una cantidad determinada a nuestras muestras
		/// actualmente procesadas.
		/// @param Cantidad Cantidad a sumar a las muestras procesadas actuales.
		
		void Add_ProcessedSamples (size_t Cantidad);
		
		/// @brief Para colocar la cantidad actual de Muestras decodificadas tenemos este metodo.
		/// @param Cantidad Numero de Muestras decodificadas.
		
		void Set_DecodedSamples (size_t Cantidad);
		
		/// @brief Asi tambien tenemos el metodo que permite sumar una cantidad determinada a nuestras muestras
		/// actualmente decodificadas.
		/// @param Cantidad Cantidad a sumar a las muestras decodificadas actuales.
		
		void Add_DecodedSamples (size_t Cantidad);
		
		/// @brief Establecemos el estado de completado en el decodificador para poder comprobar si termino el audio
		/// de ser decodificado o todavia no más alla del numero de muestras decodificadas y el total de ellas.
		
		void Set_DecodedCompleted (bool Completado = true);
		
		/// @}
		
		// --------------------------  Estado  ---------------------------------
		
		/// @name Estado Muestreador.
		/// @brief Metodos para conocer el Estado actual del Muestreador.
		/// @{
		
		/// @brief Validamos si el estado actual del Muestreador es Initialized (E_Initialized).
		/// @return True si es el estado actual.
		
		ELECTRODEMP_FORCE_INLINE bool Is_Initialized () const {
			
			// Devolvemos la comparación de nuestro estado.
			
			return (Estado_Actual.load () == E_Initialized);
			
		}
		
		/// @brief Validamos si el estado actual del Muestreador es Play (E_Play).
		/// @return True si es el estado actual.
		
		ELECTRODEMP_FORCE_INLINE bool Is_Play () const {
			
			// Devolvemos la comparación de nuestro estado.
			
			return (Estado_Actual.load () == E_Play);
			
		}
		
		/// @brief Validamos si el estado actual del Muestreador es Pause (E_Pause).
		/// @return True si es el estado actual.
		
		ELECTRODEMP_FORCE_INLINE bool Is_Pause () const {
			
			// Devolvemos la comparación de nuestro estado.
			
			return (Estado_Actual.load () == E_Pause);
			
		}
		
		/// @brief Validamos si el estado actual del Muestreador es Stop (E_Stop).
		/// @return True si es el estado actual.
		
		ELECTRODEMP_FORCE_INLINE bool Is_Stop () const {
			
			// Devolvemos la comparación de nuestro estado.
			
			return (Estado_Actual.load () == E_Stop);
			
		}
		
		/// @brief Validamos si el estado actual del Muestreador es Interrupt (E_Interrupt).
		/// @return True si es el estado actual.
		
		ELECTRODEMP_FORCE_INLINE bool Is_Interrupt () const {
			
			// Devolvemos la comparación de nuestro estado.
			
			return (Estado_Actual.load () == E_Interrupt);
			
		}
		
		/// @brief Validamos si el estado actual del Muestreador es Complete (E_Complete).
		/// @return True si es el estado actual.
		
		ELECTRODEMP_FORCE_INLINE bool Is_Complete () const {
			
			// Devolvemos la comparación de nuestro estado.
			
			return (Estado_Actual.load () == E_Complete);
			
		}
		
		/// @brief Determinamos si nuestro decodificador terminó de decodificar las muestras actuales validando lo siguiente.
		/// @return True si el total de muestras decodificadas es igual o mayor al total de samples ó el estado pasó a
		/// true a causa del decodificador.
		
		ELECTRODEMP_FORCE_INLINE bool Is_DecodedCompleted () const {
			
			// Validamos las dos cuestiones antes mencionadas.
			
			return ((Cantidad_Muestras_Decodificadas.load () >= Total_Muestras.load ()) || Decodificacion_Completa);
			
		}
		
		/// @brief Determinamos si el procesador de audio ha terminado de procesar todas las muestras actuales con
		/// la siguiente comparación.
		/// 
		/// @note Determinamos la condición en true si :
		///		@li @b A : El thread decodificador ha terminado de decodificar todas las muetras y el total de muestras procesadas
		///		equivale a la cantidad decodificada o más.
		///		@li @b B : El total de samples procesadas es igual o mayor al total establecido.
		///		@li @b C : El decodificador está completo y no hay más muestras en la cola.
		/// @return True si se han procesado todas las muestras.
		
		ELECTRODEMP_FORCE_INLINE bool Is_ProcessedCompleted () const {
			
			// Validamos con estos tres opciones posibles.
			
			return (Is_DecodedCompleted () ?
				((Cantidad_Muestras_Procesadas.load () >= Cantidad_Muestras_Decodificadas.load ()) || Empty_Queue ()) :
				(Cantidad_Muestras_Procesadas.load () >= Total_Muestras.load ()));
			
		}
		
		/// @}
		
		// ------------------------  Coordinadores  ----------------------------
		
		/// @name Coordinadores de Muestras.
		/// @brief Tenemos un conjunto de metodos publicos los cuales están sincronizados sobre el mismo mutex para la realización de procesos entre el
		/// Reproductor y el Decodificador.
		/// 
		/// Estos metodos proporcionan una interfaz de comunicación entre los threads que ejecutan cada actividad por separado para poder coordinar las
		/// acciones que tienen lugar en la reproducción de nuestro audio.
		/// @{
		
		/// @brief Tenemos el metodo Extract , esté tiene como finalidad la de obtener un paquete de Muestras actuales desde la cola de Muestras que tiene
		/// nuestro muestreador.
		///
		/// Devolvera y asignara el elemento siguiente en la cola de Muestras en nuestro parametro de salida , la función devolvera true si no hay error o falso
		/// en caso de haber problema desde el decodificador o haber terminado de procesar el audio.
		/// @param[out] Salida_Muestras Salida de la Muestra de Samples siguiente en la cola.
		/// @param Bloqueo Mutex unico para desbloquear en caso de espera.
		/// @return True en caso de no presentar error , falso en caso de haber terminado de decodificar los elementos.
		
		bool Extract (Muestras_Audio ** Salida_Muestras , std::unique_lock <std::mutex> & Bloqueo);
		
		/// @brief Metodo para solicitar un conjunto nuevo de Muestras en nuestra cola de Muestras , esté metodo sera el encargado de solicitar muestras al
		/// decodificador actual.
		///
		/// De acuerdo al numero de muestras el decodificador podra establecer hasta un minimo de estás , notificara a la condición variable cuando estén listas
		/// las muestras solicitadas.
		/// @param Cantidad Numero de muestras a obtener del decoder @b como @b minimo.
		/// @return True en caso de no haber error.
		
		bool Request (unsigned long int Cantidad);
		
		/// @brief Metodo para asignar nuevas muestras a la cola de Muestras usando el move constructor de la muestra a colocar y devolviendo true en caso afirmativo.
		///
		/// El valor de retorno dependera de la condición actual de nuestras muestras.
		/// @param Entrada_Muestras Paquete de muestras a colocar en el final de la Cola de Muestras actual.
		/// @param Bloqueo Mutex unico para desbloquear en caso de espera.
		/// @return True en caso de no haber error en el proceso.
		
		bool Emplace (Muestras_Audio * Entrada_Muestras , std::unique_lock <std::mutex> & Bloqueo);
		
		/// @brief Finalmente tenemos el metodo coordinador de ambos threads el cual permite la colocación de un nuevo elemento en la cola y la asignación de otro
		/// elemento a nuestra reproducción. Este metodo tiene que estar sincronizado en ambos threads para colocar y extraer elementos.
		/// @param[out] Salida Buffer para la salida de muestras de la queue en el caso de solicitar elemento. Dejar en null si vamos a colocar elementos.
		/// @param[in] Entrada Buffer para la entrada de muestras a la queue. Dejar en null si vamos a extraer elementos.
		/// @param Cantidad Solicitud nueva de muestras para el decoder. Dejar en 0 si no requerimos más muestras.
		/// @return True en caso de no presentar ningun error.
		
		bool Query (Muestras_Audio ** Salida , Muestras_Audio * Entrada = nullptr , unsigned long int Cantidad = 0u);
		
		/// @}
		
		// -----------------------  Muestreación  ------------------------------
		
		/// @name Muestración.
		/// @brief Tenemos un conjunto de metodos para modificar el estado de nuestro coordinador de threads actual.
		/// 
		/// Incluiremos metodos como Muestreador_Play , Muestreador_Stop , Muestreador_Pause , Muestreador_Init , Muestreador_Terminate para poder iniciar
		/// ciertos elementos que nuestro coordinador tiene que establecer para mantener comunicados de manera correcta a nuestro Reproductor y Decodificador.
		/// @{
		
		/// @brief Tenemos el metodo Init para iniciar todos los valores de nuestro Muestreador.
		
		void Muestreador_Init ();
		
		/// @brief Tenemos el metodo Play para iniciar el proceso del muestreador actual dando oportunidad al decoder y reproductor que comiencen sus procesamientos
		/// al notificar la continuación para ambos threads.
		
		void Muestreador_Play ();
		
		/// @brief Tenemos el siguiente metodo para Pausar el Proceso de Muestreador actual y con esto detener a los threads respectivos.
		
		void Muestreador_Pause ();
		
		/// @brief El siguiente metodo tiene como objetivo detener el prOceso de decodificación y reproducción de nuestro audio actualmente establecido de manera
		/// correcta.
		
		void Muestreador_Stop ();
		
		/// @brief Tenemos ahora el metodo Terminate para finalizar todos los valores que se utilizarón en el proceso de muestreación.
		/// @param Interrupcion Indica si fue terminado por causa de una interrupción (Por ejemplo un error de Frame) o de manera normal.
		
		void Muestreador_Terminate (bool Interrupcion = false);
		
		/// @brief Ahora un metodo para resetear el contador de muestras en nuestro muestreador , esto permitira retomar el estado original que
		/// teniamos antes de comenzar a reproducir el audio desde el origen.
		
		void Muestreador_Reset ();
		
		/// @}
		
		// ------------------  Destructor del Muestreador  ---------------------
		
		/// @brief Implementaremos asi mismo nuestro destructor para esté muestreador.
		
		~ Muestreador ();
	
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/// @}

} // namespace ElectrodeMP.

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ELECTRODEMP_MUESTREADOR_H
