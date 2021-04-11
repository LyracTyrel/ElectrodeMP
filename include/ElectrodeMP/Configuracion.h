
// -----------------------  Archivo de Configuración  --------------------------

/// @file Configuracion.h
///	@brief Archivo de configuración para el Proyecto.
///
/// El presente archivo fue realizado con la finalidad de asignar algunas macros y definir algunas constantes en general para todo el
/// reproductor de Audio , estas permitiran mantener igualados por cada modulo los elementos a utilizar.
/// @author Loubiner
/// @date Sunday 24/02/2018

#ifndef ELECTRODEMP_CONFIGURACION_H
#define ELECTRODEMP_CONFIGURACION_H

// ----------------------------  Configuración  --------------------------------

// Comenzaremos por incluir el archivo de electrodemp_config.h base donde se establecen todas las macro-definiciones que se asignarón conforme
// el sistema y algunas busquedas y configuraciones establecidas desde el CMake.

#include <electrodemp_config.h>

// -----------------------------------------------------------------------------

// ---------------------  Configuraciones Adicionales  -------------------------

/// @brief
/// Vamos a definir entonces la Macro constante para determinar si usar el atributo always_inline o dejarlo solo en inline de acuerdo
/// al compilador que estemos utilizando.

#if defined (_MSC_VER)
	
	/// Macro de Force inline para Visual C++.
	
	#define ELECTRODEMP_FORCE_INLINE __forceinline

#elif defined (__MINGW32__) || defined (__GNUC__) || defined (__clang__)
	
	/// Macro de Force inline para GNU (MinGW) ó Clang.

	#define ELECTRODEMP_FORCE_INLINE __attribute__ ((__always_inline__)) inline
	
#else

	/// Macro de Force inline para el resto de compiladores.
	
	#define ELECTRODEMP_FORCE_INLINE inline

#endif

/// @brief Tenemos la definición del char SEPARADOR de paths , dependiendo el sistema operativo actual definiremos esté simmbolo.

#ifdef _WIN32

	/// Definiremos el caracter '\' para la separación de paths en la Plataforma Windows.

	#define ELECTRODEMP_PATH_SEPARATOR '\\'

#else

	/// Definiremos el caracter '/' para la separación de paths en el resto de Plataformas.

	#define ELECTRODEMP_PATH_SEPARATOR '/'

#endif

// -----------------------------------------------------------------------------

// -----------------------------  Samples  -------------------------------------

/// @brief Tenemos la siguiente macro para declarar el total de Canales a utilizar en nuestro programa.

#define ELECTRODEMP_CHANNELS 2

/// @brief Tenemos el tipo de dato para cada Sample que utilizaremos en los programas del reproductor y el decodificador.
///
/// Usaremos el tipo de dato especificado en la interfaz GUI de configuración en Cmake , en caso de no contar con esté se asignara un
/// tipo float por defecto.

#ifndef ELECTRODEMP_SAMPLE_TYPE

	/// @brief Usaremos un tipo de dato float de 32 bists para realizar el intercambio de muestra a muestra en valores que van de un rango de -1.0 a 1.0.

	#define ELECTRODEMP_SAMPLE_TYPE float

#endif

// -----------------------------------------------------------------------------

// -----------------------------  Pixeles  -------------------------------------

/// @brief Tipo de dato para guardar los pixeles de las imagenes de Caratula de las etiquetas en los archivos de audio.
///
/// Utilizaremos el tipo de pixel establecido en el Cmake o un tipo de pixel unsigned char por defecto.

#ifndef ELECTRODEMP_PIXEL_TYPE

	/// @brief Usaremos el tipo de pixel tradicional , unsigned char (1 byte para Mono , 3 bytes para RGB , 4 para RGBA).
	
	#define ELECTRODEMP_PIXEL_TYPE unsigned char

#endif

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ELECTRODEMP_CONFIGURACION_H
