
# Modulo para la configuración de elementos y propiedades de ElectrodeMP.

# ------------------------------------------------------------------------------

# ---------------------------  Configuraciones  --------------------------------

# Empezaremos por declarar algunos elementos configurables para la biblioteca que el usuario
# puede habilitar o desabilitar deacuerdo a lo que necesite.

# -------------  LOG  --------------

# Para habilitar la salida del LOG por consola ademas del archivo LOG.txt generado por la aplicación.
# Se utiliza la siguiente bandera de estado (por defecto está desabilitada).

option (ELECTRODEMP_ENABLE_CONSOLE_LOG "Habilita la salida LOG por consola del programa" ON)

# Si la consola fue habilitada , entonces anexaremos lo siguiente.

if (ELECTRODEMP_ENABLE_CONSOLE_LOG)

	# Colocaremos la siguiente flag para el compilador de los ejecutables.
	
	set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mconsole")

else ()

	# Colocaremos la siguiente flag para el compilador de los ejecutables.
	
	set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")

endif ()

# --------------  Sample Type  ---------------

# Estableceremos ahora una lista de tipos de datos para elegir entre cual utilizar como tipo de muestra en el programa.

set (ELECTRODEMP_SAMPLE_TYPE "float" CACHE STRING "Tipo de dato de las Muestras de audio utilizadas")

# Colocaremos el resto de las opciones de está forma :
#		float (32 bits)
#		signed char (8 bits)
#		unsigned char (8 bits)
#		short (16 bits)
#		int (32 bits)

set_property (CACHE ELECTRODEMP_SAMPLE_TYPE PROPERTY STRINGS
	"float"
	"signed char"
	"unsigned char"
	"short"
	"int"
)

# Mostraremos el mensaje con el tipo de muestra seleccionado.

message (STATUS "Seleccionado el tipo de Sample : ${ELECTRODEMP_SAMPLE_TYPE}")

# --------------  Pixel Type  --------------

# Establecemos de igual forma una lista con tipos de datos para los pixeles de la imagen de caratula. Asi como el procesador
# de imagenes de CImg , OpenCV.

set (ELECTRODEMP_PIXEL_TYPE "unsigned char" CACHE STRING "Tipo de dato de los Pixeles de Imagen")

# Colocaremos los siguientes tipos de datos , para seleccionar de una lista.
#		unsigned char (1 byte por canal).
#		float (4 bytes por canal).

set_property (CACHE ELECTRODEMP_PIXEL_TYPE PROPERTY STRINGS
	"unsigned char"
	"float"
)

# Mostraremos otro mensaje para indicar el tipo de pixel que fue seleccionado.

message (STATUS "Seleccionado el tipo de Pixel : ${ELECTRODEMP_PIXEL_TYPE}")

# -------------  Media Player --------------

# Adicionalmente se puede compilar el programa Media Player hecho con wxWidgets para mostrar claramente
# el funcionamiento de ElectrodeMP en una aplicación de reproducción basica.

option (ELECTRODEMP_BUILD_MEDIA_PLAYER "Compila el reproductor de Audio (Media Player) de ElectrodeMP" OFF)

# ------------------------------------------------------------------------------
