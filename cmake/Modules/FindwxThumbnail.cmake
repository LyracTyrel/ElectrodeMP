
# ---------------------------  FindwxThumbnail  --------------------------------
# --                                                                          --
# --  Archivo de busqueda para la biblioteca de extensión de wxWidgets :      --
# --  wxThumbnail.                                                            --
# --  Author : Menightmar.                                                    --
# --  Date : 15/03/2018.                                                      --
# --                                                                          --
# --  wxThumbnail_FOUND                                                       --
# --  wxThumbnail_INCLUDE_DIR                                                 --
# --  wxThumbnail_LIBRARY                                                     --
# ------------------------------------------------------------------------------

# Tenemos dos opciones , permitirle al asuario elegir entre compilar el modulo de extensión dentro del mismo
# ElectrodeMP o utilizar una versión que el usuario ya tenga.

option (wxThumbnail_COMPILE_SOURCE "Compilar wxthumbnail utilizando el codigo de contrib" ON)

# Validamos la opción para saber como continuar.

if (wxThumbnail_COMPILE_SOURCE)

	# Tenemos el siguiente directorio fuente para la extensión.
	
	set (wxThumbnail_DIR "${CMAKE_SOURCE_DIR}/contrib/wxThumbnail")

	# Anexaremos el subdirectorio siguiente para compilar el codigo fuente de wxthumbnail.
	
	add_subdirectory ("${wxThumbnail_DIR}" "${CMAKE_BINARY_DIR}/wxThumbnail")
	
	# Estableceremos el valor actual para el output filename de la biblioteca.
	
	set (wxThumbnail_LIBRARY_FILE "$<TARGET_FILE:wxthumbnail>")
	
	# Colocaremos las direcciones a los archivos include.
	
	set (wxThumbnail_INCLUDE_DIR "${wxThumbnail_DIR}/include")
	
	# Colocaremos la ubicación de la biblioteca de salida.
	
	set (wxThumbnail_LIBRARY "${wxThumbnail_LIBRARY_FILE}")

else ()

	# Buscaremos para empezar el directorio Filename donde está ubicado el archivo : portaudio.h

	find_path (wxThumbnail_INCLUDE_DIR "wx/thumbnail.h" 
		DOC "Directorio de include donde está el archivo wxthumbnail.h"
		HINTS "${CMAKE_SOURCE_DIR}/contrib/wxThumbnail/include"
	)

	# Buscaremos el archivo de biblioteca de wxthumbnail , usando el siguiente nombre :
	#		wxthumbnail
		
	find_library (wxThumbnail_LIBRARY NAMES wxthumbnail
		DOC "Archivo de biblioteca wxthumbnail para wxWidgets"
		HINTS "${CMAKE_SOURCE_DIR}/contrib/wxThumbnail/lib"
	)

endif ()

# ------------------------------------------------------------------------------

# Incluiremos el modulo para la busqueda de argumentos estandar de paquete.
	
include (FindPackageHandleStandardArgs)

# Ahora buscaremos los argumentos estandar del paquete wxThumbnail.

find_package_handle_standard_args (wxThumbnail DEFAULT_MSG wxThumbnail_LIBRARY wxThumbnail_INCLUDE_DIR)

# ------------------------------------------------------------------------------
