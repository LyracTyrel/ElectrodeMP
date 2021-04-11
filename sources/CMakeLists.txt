
# Sub-Directorio CMake de ElectrodeMP para la compilación de la biblioteca [Dinamica ó Estatica].

# ------------------------------------------------------------------------------

# -----------------------  Biblioteca Electrodemp  -----------------------------

# Tenemos la siguiente lista de fuentes de la biblioteca principal.
# 	Reproductor.cpp
#	Decodificador.cpp
#	Muestreador.cpp
#	Etiquetador.cpp
#	LOG.cpp

set (ELECTRODEMP_LIBRARY_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/Reproductor.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Decodificador.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Muestreador.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Etiquetador.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/LOG.cpp"
)

# Tenemos que anexar todos los directorios de include para las cabeceras de ElectrodeMP.

include_directories ("${ELECTRODEMP_HEADERS}")

# Anexaremos la biblioteca con el nombre de "ElectrodeMP" y todos los archivos fuente.

add_library (ElectrodeMP "${ELECTRODEMP_LIBRARY_SOURCES}")

# ------------------------------------------------------------------------------
