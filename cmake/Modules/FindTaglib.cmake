
# ----------------------------  FindTagLib  ------------------------------------
# --                                                                          --
# --  Archivo de busqueda para la biblioteca PortAudio (PA) a partir de       --
# --  ciertos elementos.                                                      --
# --  Author : Menightmar.                                                    --
# --  Date : 11/03/2018.                                                      --
# --                                                                          --
# --  TAGLIB_FOUND                                                            --
# --  TAGLIB_INCLUDE_DIRS                                                     --
# --  TAGLIB_STATIC                                                           --
# --  TAGLIB_LIBRARY                                                          --
# ------------------------------------------------------------------------------

# Buscaremos para empezar el directorio Filename donde está ubicado el archivo : tag.h

find_path (TAGLIB_INCLUDE_DIR tag.h DOC "Directorio donde se encuentra tag.h")

# Buscaremos el path para el directorio donde está el archivo config.h.

find_path (TAGLIB_INCLUDE_CONFIG_DIR config.h DOC "Directorio donde se encuentra config.h")

# Validamos si tenemos el directorio de Include.

if (NOT ("${TAGLIB_INCLUDE_DIR}" STREQUAL "TAGLIB_INCLUDE_DIR-NOTFOUND"))
	
	# Anexaremos estos path a la lista de directorios.
	
	list (APPEND TAGLIB_INCLUDE_DIRS "${TAGLIB_INCLUDE_DIR}" "${TAGLIB_INCLUDE_CONFIG_DIR}")

	# Ahora anexaremos el resto de sub-directorios :
	#    /toolkit
	#    /mpeg
	#    /mpeg/id3v2
	#    /riff
	#    /riff/wav
	#    /mp4
	#    /ogg
	#    /ogg/vorbis
	#    /ogg/flac
	#    /flac
	
	list (APPEND TAGLIB_INCLUDE_DIRS
		"${TAGLIB_INCLUDE_DIR}/toolkit"
		"${TAGLIB_INCLUDE_DIR}/mpeg"
		"${TAGLIB_INCLUDE_DIR}/mpeg/id3v2"
		"${TAGLIB_INCLUDE_DIR}/riff"
		"${TAGLIB_INCLUDE_DIR}/riff/wav"
		"${TAGLIB_INCLUDE_DIR}/mp4"
		"${TAGLIB_INCLUDE_DIR}/ogg"
		"${TAGLIB_INCLUDE_DIR}/ogg/vorbis"
		"${TAGLIB_INCLUDE_DIR}/ogg/flac"
		"${TAGLIB_INCLUDE_DIR}/flac"
	)
	
endif ()

# Si la biblioteca TagLib a utilizar es estática , entonces tendra el usuario que indicarlo para poder
# anexar las flags requeridas.

option (TAGLIB_STATIC "Indica si utilizar la biblioteca estatica" ON)

# Buscaremos el archivo de biblioteca de TagLib , usando el siguiente nombre para el match :
#	libtag.a

find_library (TAGLIB_LIBRARY NAMES tag DOC "Archivo de biblioteca TagLib")

# ------------------------------------------------------------------------------

# Incluiremos el modulo para la busqueda de argumentos estandar de paquete.
	
include (FindPackageHandleStandardArgs)

# Ahora buscaremos los argumentos estandar del paquete Taglib.

find_package_handle_standard_args (Taglib DEFAULT_MSG TAGLIB_LIBRARY TAGLIB_INCLUDE_DIR TAGLIB_INCLUDE_CONFIG_DIR)

# ------------------------------------------------------------------------------
