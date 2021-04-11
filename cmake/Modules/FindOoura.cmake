
# ----------------------------  FindOoura  -------------------------------------
# --                                                                          --
# --  Archivo de busqueda para la biblioteca Ooura a partir de elementos.     --
# --  Author : Menightmar.                                                    --
# --  Date : 11/03/2018.                                                      --
# --                                                                          --
# --  AQUILA_OOURA_FOUND                                                      --
# --  AQUILA_OOURA_LIBRARY                                                    --
# ------------------------------------------------------------------------------

# Ooura está conformado por diversas versiones de la FFT.

# Por defecto tenemos el valor en falso.

set (AQUILA_OOURA_FOUND FALSE)

# Buscaremos el archivo de biblioteca de la siguiente forma.
#	libOoura_fft
#	libOoura_fft4g
#	libOoura_fft4g_h
#	libOoura_fft8g
#	libOoura_fft8g_h
#	libOoura_fftsg
#	libOoura_fftsg_h

find_library (AQUILA_OOURA_LIBRARY
	
	NAMES Ooura_fft Ooura_fft4g Ooura_fft4g_h Ooura_fft8g Ooura_fft8g_h Ooura_fftsg Ooura_fftsg_h
	DOC "Biblioteca de Ooura para la FFT a utilizar (opcional)"
	
)

# Validamos si encontró la biblioteca

if (NOT "${AQUILA_OOURA_LIBRARY}" STREQUAL "AQUILA_OOURA_LIBRARY-NOTFOUND")

	set (AQUILA_OOURA_FOUND TRUE)

endif ()

# ------------------------------------------------------------------------------

# Incluiremos el modulo para la busqueda de argumentos estandar de paquete.
	
include (FindPackageHandleStandardArgs)

# Ahora buscaremos los argumentos estandar del paquete Ooura.

find_package_handle_standard_args (Ooura DEFAULT_MSG AQUILA_OOURA_LIBRARY)

# ------------------------------------------------------------------------------
