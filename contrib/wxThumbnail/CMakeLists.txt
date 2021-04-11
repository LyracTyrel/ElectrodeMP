
# Archivo de compilación CMake de la extensión wxThumbFrame y los demas componentes de thumbnail.h

# ------------------------------------------------------------------------------

# -----------------------  Biblioteca wxThumbnail  -----------------------------

# Tenemos el siguiente codigo fuente para la biblioteca de extensión
# 	source/
#		thumbnail.cpp

set (wxThumbnail_LIBRARY_SOURCES "source/thumbnail.cpp")

# Anexaremos el directorio de cabecera de wx/wxthumbnail y el resto de ElectrodeMP.

include_directories ("include" "${ELECTRODEMP_HEADERS}")

# Anexaremos la biblioteca con el nombre de "wxthumbnail" y todos los archivos fuente.

add_library (wxthumbnail STATIC "${wxThumbnail_LIBRARY_SOURCES}")

# ------------------------------------------------------------------------------
