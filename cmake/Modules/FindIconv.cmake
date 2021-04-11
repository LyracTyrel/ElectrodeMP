
# -----------------------------  FindIconv  ------------------------------------
# --                                                                          --
# --  Archivo de busqueda para la biblioteca estandar de Iconv (GNU GCC)      --
# --  ciertos elementos.                                                      --
# --  Author : Menightmar.                                                    --
# --  Date : 11/03/2018.                                                      --
# --                                                                          --
# --  ICONV_FOUND                                                             --
# --  ICONV_INCLUDE_DIR                                                       --
# --  ICONV_LIBRARY                                                           --
# ------------------------------------------------------------------------------

# Buscaremos para empezar el directorio Filename donde está ubicado el archivo : iconv.h

find_path (ICONV_INCLUDE_DIR iconv.h DOC "Directorio donde se encuentra iconv.h")

# Buscaremos el archivo de biblioteca de ICONV , usando el siguiente nombre para el match :
#	libiconv.a

find_library (ICONV_LIBRARY NAMES iconv DOC "Archivo de biblioteca ICONV")

# ------------------------------------------------------------------------------

# Incluiremos el modulo para la busqueda de argumentos estandar de paquete.
	
include (FindPackageHandleStandardArgs)

# Ahora buscaremos los argumentos estandar del paquete ICONV.

find_package_handle_standard_args (Iconv DEFAULT_MSG ICONV_LIBRARY ICONV_INCLUDE_DIR)

# ------------------------------------------------------------------------------
