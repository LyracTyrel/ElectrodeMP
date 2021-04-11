
# ----------------------------  FindCImg  --------------------------------------
# --                                                                          --
# --  Archivo de busqueda para la biblioteca CImg a partir de elementos.      --
# --  Author : Menightmar.                                                    --
# --  Date : 11/03/2018.                                                      --
# --                                                                          --
# --  CIMG_FOUND                                                              --
# --  CIMG_INCLUDE_DIR                                                        --
# ------------------------------------------------------------------------------

# Buscaremos simplemente la cabecera de CImg que conforma toda la biblioteca : CImg.h

find_path (CIMG_INCLUDE_DIR CImg.h DOC "Directorio donde se encuentra CImg.h")

# ------------------------------------------------------------------------------

# Incluiremos el modulo para la busqueda de argumentos estandar de paquete.
	
include (FindPackageHandleStandardArgs)

# Ahora buscaremos los argumentos estandar del paquete CImg.

find_package_handle_standard_args (CImg DEFAULT_MSG CIMG_INCLUDE_DIR)

# ------------------------------------------------------------------------------
