
// ---------------------------  Archivo de LOG  --------------------------------

/// @file LOG.h
///	@brief Archivo para la declaración del sistema LOG de ElectrodeMP.
///
/// Contenemos todas las macro y funciones para generar y recuperar la cadena de texto LOG que va a ser generada conforme el programa
/// vaya registrando diferentes entradas de Texto LOG.
/// @author Loubiner
/// @date Monday 12/03/2018

#ifndef ELECTRODEMP_LOG_H
#define ELECTRODEMP_LOG_H

// -----------------------------------------------------------------------------

// -----------------------------  Bases  ---------------------------------------

// Incluiremos para empezar nuestras cabeceras de C.

#include <stdio.h>
#include <stdint.h>

// Incluiremos ademas las cadenas de c/c++.

#include <string.h>
#include <string>

// Requerimos la salida estandar , buffers y de flujo para los archivos de texto.

#include <iostream>
#include <streambuf>
#include <fstream>

// Utilizaremos tambien la sincronización de threads mediante mutex y locks.

#include <mutex>

// -------------------------  Configuracion  -----------------------------------

// Vamos a incluir nuestra configuración para todo el proyecto en general.

#include <ElectrodeMP/Configuracion.h>

// ---------------------------  TermColor  -------------------------------------

// Habilitaremos el uso de Termcolor si está especificado en la configuración.

#ifdef ELECTRODEMP_ENABLE_TERMCOLOR

// Para la presentación de mensajes de LOG por la consola tenemos a nuestra biblioteca de colores personalizados por consola. Estó le dara un poco más
// de vista a nuestras impresiones de cadenas. Incluiremos la cabecera de termcolor si la opción está establecida.

#include <termcolor/termcolor.hpp>

#endif

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Incluiremos todo el codigo dentro del siguiente espacio de nombres.

namespace ElectrodeMP {

// -----------------------------------------------------------------------------

// -----------------------------  Funciones  -----------------------------------

// Comenzaremos con la declaración de todas las funciones para el procesamiento de texto LOG en un archivo de texto
// asi como en la consola del sistema si está habilitada.
	
/// @defgroup LOG
/// @brief Funciones de log para registrar las acciones del sistema en ElectrodeMP..
/// @{

/// @brief Función para la inicialización del sistema LOG en ElectrodeMP especificando una ruta de Filename para el archivo
/// de texto plano a generar.
///
/// Durante todo el procedimiento de ElectrodeMP , las acciones registran lo que va pasando a medida que se ejecuta el programa
/// y van escribiendo estas acciones en el archivo especificado.
/// @param Filename_LOG Ruta al archivo (.txt) generado en ElectrodeMP.
/// @return True si fue iniciado LOG de manera correcta.

bool LOG_Init (const std::string & Filename_LOG);

/// @brief Enumeración de los diferentes niveles de Aviso para los registros LOG.
///
/// La finalidad de estó es proveer una diferencia entre los avisos normales , los avisos de errores , las advertencias , etc.
/// que permitan ubicar de manera directa los diferentes estados en el que está pasando ElectrodeMP.

enum LOG_Type {
	
	/// @brief Nivel de Aviso Simple : Para notificar una acción o un hecho simple (sensillo) realizado de forma normal.
	
	LOG_Simple = 0 ,
	
	/// @brief Nivel de Aviso Note : Para resaltar un hecho importante realizado de manera correcta.
	
	LOG_Note ,
	
	/// @brief Nivel de Aviso Warning : Para resaltar una acción o un hecho simple que pudiera no haberse realizado correctamente
	/// sin afectar el sistema.
	
	LOG_Warning ,
	
	/// @brief Nivel de Aviso Caution : Para resaltar un hecho importante que pudiera no haberse realizado de manera correcta y
	/// puede o no afectar al sistema.
	
	LOG_Caution ,
	
	/// @brief Nivel de Aviso Error : Para resaltar un hecho importante que causo un problema en el sistema de ElectrodeMP y
	/// necesita ser atendido.
	
	LOG_Error
	
};

/// @brief Función para anexar un registro LOG al archivo y/o consola del sistema. Utilizando la cadena de entrada a registrar
/// en dicha salida y el nivel de Aviso a utilizar.
///
/// @note Si el sistema LOG no se ha iniciado correctamente , entonces se ignorará está llamada.
/// @param Cadena Cadena a imprimir (escribir) en el archivo LOG y en la consola si la opción fue habilitada.
/// @param Tipo Enumeración de @ref LOG_Type para determinar el tipo de salida.

void LOG_Write (const std::string & Cadena , LOG_Type Tipo);

/// @brief Función para anexar un registro LOG al archivo y/o consola del sistema. Utilizando la cadena de formato para imprimir
/// en dicha salida , el nivel de Aviso a utilizar y los argumentos (params) para utilizar - estilo printf -.
///
/// @tparam Argumentos Tipos de datos variables para imprimir con formato.
/// @note Si el sistema LOG no se ha iniciado correctamente , entonces se ignorará está llamada.
/// @param Formato Cadena de formato (igual que con stdio.h) en el archivo LOG y en la consola si la opción fue habilitada.
/// @param Tipo Enumeración de @ref LOG_Type para determinar el tipo de salida.
/// @param Args Parametros variables con los datos a imprimir usando el formato.

template <typename ... Argumentos>
void LOG_Printf (const std::string & Formato , LOG_Type Tipo , Argumentos ... Args) {
	
	// Imprimiremos la cadena de Salida utilizando el formato y los argumentos  variables en el siguiente buffer char.
	
	char Buffer_Cadena [500] = { '\0' };
	
	// Imprimiremos utilizando sprintf en el buffer.
	
	sprintf (Buffer_Cadena , Formato.c_str () , Args ...); // Expandimos el variadic.
	
	// Ahora invocaremos a la función LOG_Write para terminar de escribir el registro.
	
	LOG_Write (std::string (Buffer_Cadena) , Tipo);
	
}

/// @brief Función para finalizar la escritura textual de LOG en el sistema ElectrodeMP.
///
/// Está función realiza la liberación del archivo de texto LOG y la finalización del mismo escribiendo el termino de
/// la salida en LOG.
/// @note No tendra efecto si no se ha iniciado un LOG anteriormente.

void LOG_Terminate ();

// -----------------------------------------------------------------------------

/// @}

} // namespace ElectrodeMP.

// ----------------------------  Macros LOG  -----------------------------------

// Tenemos ahora un conjunto de Macros que permiten realizar las acciones de LOG para los distintos niveles de advertencia
// o notificaciones que tenemos.

/// @brief Macro def de Mensaje LOG de tipo SIMPLE (cón o sin formato).

#define LOG(Cadena) 			ElectrodeMP::LOG_Write (Cadena , 	ElectrodeMP::LOG_Type::LOG_Simple);
#define LOG_F(Formato , ...) 	ElectrodeMP::LOG_Printf (Formato , 	ElectrodeMP::LOG_Type::LOG_Simple , __VA_ARGS__);

/// @brief Macro def de Mensaje LOG de tipo NOTE (cón o sin formato).

#define LOG_N(Cadena) 			ElectrodeMP::LOG_Write (Cadena , 	ElectrodeMP::LOG_Type::LOG_Note);
#define LOG_FN(Formato , ...) 	ElectrodeMP::LOG_Printf (Formato , 	ElectrodeMP::LOG_Type::LOG_Note , __VA_ARGS__);

/// @brief Macro def de Mensaje LOG de tipo WARNING (cón o sin formato).

#define LOG_W(Cadena) 			ElectrodeMP::LOG_Write (Cadena , 	ElectrodeMP::LOG_Type::LOG_Warning);
#define LOG_FW(Formato , ...) 	ElectrodeMP::LOG_Printf (Formato , 	ElectrodeMP::LOG_Type::LOG_Warning , __VA_ARGS__);

/// @brief Macro def de Mensaje LOG de tipo CAUTION (cón o sin formato).

#define LOG_C(Cadena) 			ElectrodeMP::LOG_Write (Cadena , 	ElectrodeMP::LOG_Type::LOG_Caution);
#define LOG_FC(Formato , ...) 	ElectrodeMP::LOG_Printf (Formato , 	ElectrodeMP::LOG_Type::LOG_Caution , __VA_ARGS__);

/// @brief Macro def de Mensaje LOG de tipo ERROR con (cón o sin formato).

#define LOG_E(Cadena) 			ElectrodeMP::LOG_Write (Cadena , 	ElectrodeMP::LOG_Type::LOG_Error);
#define LOG_FE(Formato , ...) 	ElectrodeMP::LOG_Printf (Formato , 	ElectrodeMP::LOG_Type::LOG_Error , __VA_ARGS__);

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ELECTRODEMP_LOG_H
