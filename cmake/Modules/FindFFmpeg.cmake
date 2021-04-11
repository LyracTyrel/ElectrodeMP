
# ----------------------------  FindFFmpeg  ------------------------------------
# --                                                                          --
# --  Archivo de busqueda para la biblioteca FFmpeg a partir de               --
# --  ciertos elementos.                                                      --
# --  Author : Menightmar.                                                    --
# --  Date : 11/03/2018.                                                      --
# --                                                                          --
# --  FFMPEG_FOUND                                                            --
# --  FFMPEG_INCLUDE_DIR                                                      --
# --  FFMPEG_LIBRARIES                                                        --
# ------------------------------------------------------------------------------

# Buscaremos para empezar el directorio Filename donde está ubicado el archivo : fftools/ffmpeg.h

find_path (FFMPEG_ROOT_DIR fftools/ffmpeg.h DOC "Directorio raiz de FFmpeg")

# Validamos si encontramos el directorio raiz.

if (NOT ("${FFMPEG_ROOT_DIR}" STREQUAL "FFMPEG_ROOT_DIR-NOTFOUND"))
	
	# Establecemos el directorio para los includes de ffmpeg.
	
	set (FFMPEG_INCLUDE_DIR "${FFMPEG_ROOT_DIR}")

	# Buscaremos todos los archivos de biblioteca de ffmpeg en modulos de format , codecs y utils.
	#     libavformat.a
	#     libavcodec.a
	#     libavutil.a
	#     libswresample.a
	#     libswscale.a

	find_library (FFMPEG_AVFORMAT_LIBRARY
		
		NAMES avformat
		DOC "Archivo de biblioteca avformat"
		PATHS "${FFMPEG_ROOT_DIR}/libavformat"
		
	)
	find_library (FFMPEG_AVCODEC_LIBRARY
		
		NAMES avcodec
		DOC "Archivo de biblioteca avcodec"
		PATHS "${FFMPEG_ROOT_DIR}/libavcodec"
		
	)
	find_library (FFMPEG_AVUTIL_LIBRARY
		
		NAMES avutil
		DOC "Archivo de biblioteca avutil"
		PATHS "${FFMPEG_ROOT_DIR}/libavutil"
		
	)
	find_library (FFMPEG_SWRESAMPLE_LIBRARY
		
		NAMES swresample
		DOC "Archivo de biblioteca swresample"
		PATHS "${FFMPEG_ROOT_DIR}/libswresample"
		
	)
	find_library (FFMPEG_SWSCALE_LIBRARY
		
		NAMES swscale
		DOC "Archivo de biblioteca swscale"
		PATHS "${FFMPEG_ROOT_DIR}/libswscale"
		
	)

	# Establecemos la lista de bibliotecas.

	set (FFMPEG_LIBRARIES
		"${FFMPEG_AVFORMAT_LIBRARY}"
		"${FFMPEG_AVCODEC_LIBRARY}"
		"${FFMPEG_AVUTIL_LIBRARY}"
		"${FFMPEG_SWRESAMPLE_LIBRARY}"
		"${FFMPEG_SWSCALE_LIBRARY}"
	)

endif ()

# ------------------------------------------------------------------------------

# Incluiremos el modulo para la busqueda de argumentos estandar de paquete.

include (FindPackageHandleStandardArgs)

# Ahora buscaremos los argumentos estandar del paquete FFmpeg.

find_package_handle_standard_args (FFmpeg DEFAULT_MSG
	FFMPEG_AVFORMAT_LIBRARY
	FFMPEG_AVCODEC_LIBRARY
	FFMPEG_AVUTIL_LIBRARY
	FFMPEG_SWRESAMPLE_LIBRARY
	FFMPEG_SWSCALE_LIBRARY
	FFMPEG_INCLUDE_DIR
)

# ------------------------------------------------------------------------------
