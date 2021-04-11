
# ----------------------------  FindOpenCV  ------------------------------------
# --                                                                          --
# --  Archivo de busqueda para la biblioteca OpenCV a partir de elementos.    --
# --  Author : Menightmar.                                                    --
# --  Date : 11/03/2018.                                                      --
# --                                                                          --
# --  OPENCV_FOUND                                                            --
# --  OPENCV_INCLUDE_DIRS                                                     --
# --  OPENCV_LIBRARIES                                                        --
# ------------------------------------------------------------------------------

# Buscaremos para empezar el directorio donde está la raiz de la biblioteca.

find_path (OPENCV_ROOT_DIR include/opencv2/opencv.h DOC "Directorio raiz de la carpeta OpenCV")

# Buscaremos el path para el directorio donde están todos los archivos de salida de la biblioteca (*.dll , *.a , *.lib)

find_path (OPENCV_LIB_DIR cvconfig.h DOC "Directorio de los archivos de la biblioteca (*.dll , *.a , *.lib)")

# Validamos si fue encontrado el path de root.

if (NOT ("${OPENCV_ROOT_DIR}" STREQUAL "OPENCV_ROOT_DIR-NOTFOUND"))

	# Buscaremos el directorio Filename donde está ubicado el archivo : opencv2/opencv.hpp
	
	find_path (OPENCV_INCLUDE_DIR opencv2/opencv.hpp
		
		DOC 	"Directorio de include para OpenCV donde está opencv2/opencv.hpp"
		PAHTS 	"${OPENCV_ROOT_DIR}/include"
		
	)
	
	# Ahora buscaremos el resto de sub-directorios para los modulos que necesitamos :
	#     modules/core/include"
	#     modules/imgcodecs/include"
	#     modules/imgproc/include"

	find_path (OPENCV_INCLUDE_CORE_DIR opencv2/core.hpp
		
		DOC 	"Directorio de include para OpenCV donde está opencv2/core.hpp"
		PATHS 	"${OPENCV_ROOT_DIR}/modules/core/include"
		
	)
	find_path (OPENCV_INCLUDE_IMGCODECS_DIR opencv2/imgcodecs.hpp
		
		DOC 	"Directorio de include para OpenCV donde está opencv2/imgcodecs.hpp"
		PATHS 	"${OPENCV_ROOT_DIR}/modules/imgcodecs/include"
		
	)
	find_path (OPENCV_INCLUDE_IMGPROC_DIR opencv2/imgproc.hpp
		
		DOC 	"Directorio de include para OpenCV donde está opencv2/imgproc.hpp"
		PATHS 	"${OPENCV_ROOT_DIR}/modules/imgproc/include"
		
	)
	
	# Anexaremos estos path a la lista de directorios.
	
	list (APPEND OPENCV_INCLUDE_DIRS
		"${OPENCV_INCLUDE_DIR}"
		"${OPENCV_INCLUDE_IMGCODECS_DIR}"
		"${OPENCV_INCLUDE_IMGPROC_DIR}"
		"${OPENCV_INCLUDE_CORE_DIR}"
	)

endif ()

# Ahora vamos a comprobar si fue escrito el directorio de bibliotecas de OpenCV.

if (NOT ("${OPENCV_LIB_DIR}" STREQUAL "OPENCV_LIB_DIR-NOTFOUND"))

	# Buscaremos ahora el directorio del archivo de configuración de OpenCV usando el siguiente nombre : cvconfig.h
	
	find_path (OPENCV_INCLUDE_CONFIG_DIR cvconfig.h
		
		DOC 	"Directorio del archivo de configuración (lib) para OpenCV donde está cvconfig.h"
		PATHS 	"${OPENCV_LIB_DIR}"
		
	)
	
	# Anexaremos esté directorio a la lista.
	
	list (APPEND OPENCV_INCLUDE_DIRS "${OPENCV_INCLUDE_CONFIG_DIR}")
	
	# Buscaremos los archivos de biblioteca de OPENCV , usando los nombres que tenemos a continuación para el match :
	#    libopencv_imgcodecs.a
	#    libopencv_imgproc.a
	#    libopencv_core.a
	#    liblibtiff.a
	#    liblibpng.a
	#    liblibjpeg.a
	
	# Buscaremos las siguientes versiones :
	#	341 , 340 , 331 , 330 , 320 , 310 , 300
	
	find_library (OPENCV_IMGCODECS_LIBRARY
		
		NAMES 	opencv_imgcodecs341 opencv_imgcodecs340 opencv_imgcodecs331 opencv_imgcodecs330 opencv_imgcodecs320 opencv_imgcodecs310 opencv_imgcodecs300
		DOC 	"Archivo de biblioteca de opencv imgcodecs"
		PATHS 	"${OPENCV_LIB_DIR}"
		
	)
	find_library (OPENCV_IMGPROC_LIBRARY
		
		NAMES 	opencv_imgproc341 opencv_imgproc340 opencv_imgproc331 opencv_imgproc330 opencv_imgproc320 opencv_imgproc310 opencv_imgproc300
		DOC 	"Archivo de biblioteca de opencv imgproc"
		PATHS 	"${OPENCV_LIB_DIR}"
		
	)
	find_library (OPENCV_CORE_LIBRARY 		
		
		NAMES 	opencv_core341 opencv_core340 opencv_core331 opencv_core330 opencv_core320 opencv_core310 opencv_core300
		DOC 	"Archivo de biblioteca de opencv core"
		PATHS 	"${OPENCV_LIB_DIR}"
		
	)
	find_library (OPENCV_LIBTIFF_LIBRARY 	
		
		NAMES 	libtiff
		DOC 	"Archivo de biblioteca de libtiff"
		PATHS 	"${OPENCV_LIB_DIR}"
		
	)
	find_library (OPENCV_LIBPNG_LIBRARY 	
		
		NAMES 	libpng
		DOC 	"Archivo de biblioteca de libpng"
		PATHS 	"${OPENCV_LIB_DIR}"
		
	)
	find_library (OPENCV_LIBJPEG_LIBRARY 	
		
		NAMES 	libjpeg
		DOC 	"Archivo de biblioteca de libjpeg"
		PATHS 	"${OPENCV_LIB_DIR}"
		
	)
	
	# Establecemos la lista de bibliotecas que utilizamos.
	
	list (APPEND OPENCV_LIBRARIES
		"${OPENCV_IMGCODECS_LIBRARY}"
		"${OPENCV_IMGPROC_LIBRARY}"
		"${OPENCV_CORE_LIBRARY}"
		"${OPENCV_LIBTIFF_LIBRARY}"
		"${OPENCV_LIBPNG_LIBRARY}"
		"${OPENCV_LIBJPEG_LIBRARY}"
	)
	
endif ()

# ------------------------------------------------------------------------------

# Incluiremos el modulo para la busqueda de argumentos estandar de paquete.
	
include (FindPackageHandleStandardArgs)

# Ahora buscaremos los argumentos estandar del paquete OpenCV.

find_package_handle_standard_args (OpenCV DEFAULT_MSG
	OPENCV_CORE_LIBRARY
	OPENCV_IMGCODECS_LIBRARY
	OPENCV_IMGPROC_LIBRARY
	OPENCV_LIBTIFF_LIBRARY
	OPENCV_LIBPNG_LIBRARY
	OPENCV_LIBJPEG_LIBRARY
	OPENCV_INCLUDE_DIR
	OPENCV_INCLUDE_CONFIG_DIR
	OPENCV_INCLUDE_CORE_DIR
	OPENCV_INCLUDE_IMGCODECS_DIR
)

# ------------------------------------------------------------------------------
