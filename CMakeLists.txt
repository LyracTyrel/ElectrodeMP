
# Archivo de Cmake para la generación del proyecto ElectrodeMP.

# ------------------------------------------------------------------------------

# Usaremos la siguiente versión de CMake como minimo.

cmake_minimum_required (VERSION 3.2.3)

# Ahora daremos el nombre a nuestro proyecto actual.

project (ELECTRODEMP)

# ------------------------------------------------------------------------------

# ------------------------------  Modulos  -------------------------------------

# Incluiremos los modulos respectivos para la busqueda de elementos de bibliotecas y archivos de terceros que ocupa
# electrodemp para su compilación y generación.

# Anexaremos los siguientes directorios a la lista de modulos para CMake.

list (APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_SOURCE_DIR}/cmake/Modules")

# ---------------  ElectrodeMP  ---------------

# Anexaremos nuestro modulo para la busqueda de dependencias y configuración de la misma.

include (ElectrodeMPDependences)

# Anexaremos el modulo para comprobación de elementos.

include (ElectrodeMPChecker)

# Anexaremos el modulo para la configuración de opciones para ElectrodeMP.

include (ElectrodeMPConfigure)

# ------------------------------------------------------------------------------

# ------------------------  Cabeceras y Dependencias  --------------------------

# Tendremos una lista de cabeceras a incluir en nuestra biblioteca y ejecutable del reproductor , las cuales contendran
# todas las direcciones para utilizar asi como dependencias de bibliotecas de terceros que requiere la aplicación.

ElectrodeMPDependences ()

# ------------------------------------------------------------------------------

# -----------------------------  Biblioteca  -----------------------------------

# Comenzaremos anexando el sub-diretorio donde se compilan todas las fuentes de la biblioteca
# y por tanto la generación del archivo .a o .dll de acuerdo a la siguiente opción.

option (ELECTRODEMP_BUILD_SHARED_LIBS "Compila la biblioteca de forma dinamica (*.dll)" OFF)

# Colocaremos la opción en la variable siguiente.
# option (BUILD_SHARED_LIBS "Compila la biblioteca de forma dinamica (*.dll)" OFF)

set (BUILD_SHARED_LIBS "${ELECTRODEMP_BUILD_SHARED_LIBS}")

# Anexaremos el subdirectorio para la generación de la biblioteca con la salida en el mismo directorio bin que esté.

add_subdirectory ("${CMAKE_SOURCE_DIR}/sources" "${CMAKE_BINARY_DIR}/library")

# ------------------------------------------------------------------------------

# ----------------------------  Media Player  ----------------------------------

# Validamos la opción elegida para poder determinar si compilar el programa.

if (ELECTRODEMP_BUILD_MEDIA_PLAYER)

	# Incluiremos el subdirectorio para la compilación del reproductor de audio.
	
	add_subdirectory ("${CMAKE_SOURCE_DIR}/player" "${CMAKE_BINARY_DIR}/player")

endif ()

# ------------------------------------------------------------------------------

# Guardaremos el estado de configurado actual.

set (CONFIGURADO TRUE CACHE INTERNAL "Flag de Configuración")
