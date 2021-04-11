
# ----------------------------  FindPortaudio  ---------------------------------
# --                                                                          --
# --  Archivo de busqueda para la biblioteca PortAudio (PA) a partir de       --
# --  ciertos elementos.                                                      --
# --  Author : Menightmar.                                                    --
# --  Date : 11/03/2018.                                                      --
# --                                                                          --
# --  PORTAUDIO_FOUND                                                         --
# --  PORTAUDIO_INCLUDE_DIR                                                   --
# --  PORTAUDIO_LIBRARY                                                       --
# ------------------------------------------------------------------------------

# Buscaremos para empezar el directorio Filename donde está ubicado el archivo : portaudio.h

find_path (PORTAUDIO_INCLUDE_DIR portaudio.h DOC "Directorio de include donde está el archivo portaudio.h")

# Buscaremos el archivo de biblioteca de PortAudio , usando los nombres para el match :
#		portaudio[.dll]
#		portaudio_static
	
find_library (PORTAUDIO_LIBRARY NAMES portaudio portaudio.dll portaudio_static DOC "Archivo de biblioteca portaudio")

# ------------------------------------------------------------------------------

# Incluiremos el modulo para la busqueda de argumentos estandar de paquete.
	
include (FindPackageHandleStandardArgs)

# Ahora buscaremos los argumentos estandar del paquete Portaudio.

find_package_handle_standard_args (Portaudio DEFAULT_MSG PORTAUDIO_LIBRARY PORTAUDIO_INCLUDE_DIR)

# ------------------------------------------------------------------------------
