
# ----------------------------  FindAquila  ------------------------------------
# --                                                                          --
# --  Archivo de busqueda para la biblioteca Aquila a partir de elementos.    --
# --  Author : Menightmar.                                                    --
# --  Date : 11/03/2018.                                                      --
# --                                                                          --
# --  AQUILA_FOUND                                                            --
# --  AQUILA_INCLUDE_DIR                                                      --
# --  AQUILA_LIBRARIES                                                        --
# ------------------------------------------------------------------------------

# Buscaremos para empezar el directorio Filename donde está ubicado el archivo : aquila/aquila.h

find_path (AQUILA_INCLUDE_DIR aquila/aquila.h DOC "Directorio include de la carpeta de Aquila")

# Buscaremos el archivo de biblioteca de Aquila usando el siguiente nombre :
#		Aquila
	
find_library (AQUILA_LIBRARY NAMES Aquila DOC "Archivo de biblioteca de Aquila")

# Anexaremos la lista de bibliotecas de Aquila.

list (APPEND AQUILA_LIBRARIES "${AQUILA_LIBRARY}")

# ------------------------------------------------------------------------------

# ------------------------------  Ooura FFT  -----------------------------------

# Aquila puede utilizar las transformadas de fourier de Ooura , de manera opcional buscaremos
# esta dependencia.

find_package (Ooura)

# Si encontramos la biblioteca entonces la anexaremos.

if (AQUILA_OOURA_FOUND)

	# Anexaremos a la lista de bibliotecas.
	
	list (APPEND AQUILA_LIBRARIES "${AQUILA_OOURA_LIBRARY}")

endif ()

# ------------------------------------------------------------------------------

# Incluiremos el modulo para la busqueda de argumentos estandar de paquete.
	
include (FindPackageHandleStandardArgs)

# Ahora buscaremos los argumentos estandar del paquete Aquila.

find_package_handle_standard_args (Aquila DEFAULT_MSG AQUILA_LIBRARY AQUILA_INCLUDE_DIR)

# ------------------------------------------------------------------------------
