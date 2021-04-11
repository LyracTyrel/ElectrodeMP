
# Sub-Directorio CMake para la compilación del Programa Media Player de ElectrodeMP.

# ------------------------------------------------------------------------------

# ----------------------------  Media Player  ----------------------------------

# A continuación vamos a  declarar algunas opciones que permiten configurar ciertos elementos de la
# aplicación al momento de compilar.

# Tenemos la siguiente lista de fuentes de nuestro programa actual relativos al directorio actual.
#	Main.cpp
#	sources/
# 		Pantalla.cpp
#		Dialogo.cpp
#		DialogoAyuda.cpp
#		PantallaBase.cpp
#		PanelCover.cpp
#	Widgets/
#			ElectrodeBoton.cpp
#			ElectrodeProgreso.cpp
#	resources/
#		Resources.rc (solo windows)

set (ELECTRODEMP_PLAYER_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/sources/Pantalla.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/sources/Dialogo.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/sources/DialogoAyuda.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/sources/PantallaBase.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/sources/PanelCover.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/sources/Widgets/ElectrodeBoton.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/sources/Widgets/ElectrodeProgreso.cpp"
)

# Si el sistema operativo destino es windows , incluiremos los recursos (*.rc) del programa.

if (DEFINED WIN32)

	# Anexaremos el siguiente rc a los sources.
	
	list (APPEND ELECTRODEMP_PLAYER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/resources/Resources.rc")

endif ()

# Tenemos que anexar todos los directorios de include para las cabeceras de ElectrodeMP ademas del
# directorio include del Media Player.

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include" "${ELECTRODEMP_HEADERS}")

# Anexaremos el siguiente target ejecutable y todos los archivos fuente.

add_executable (ElectrodeMP_MediaPlayer "${ELECTRODEMP_PLAYER_SOURCES}")

# Ahora vamos a configurar el nombre de salida de nuestro programa ejecutable.

set_target_properties (ElectrodeMP_MediaPlayer PROPERTIES RUNTIME_OUTPUT_NAME "ElectrodeMP - Audio Media Player Alpha")

# El directorio para la salida será el directorio "bin" en la raiz del proyecto.

set_target_properties (ElectrodeMP_MediaPlayer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Crearemos la lista de dependencias menores para wxWidgets.

set (ElectrodeMP_MediaPlayer_SUB_DEPENDENCES -loleacc)

# Y vincularemos todas las bibliotecas de dependencia del ejecutable final incluyendo a ElectrodeMP.

target_link_libraries (ElectrodeMP_MediaPlayer
	"${ELECTRODEMP_MEDIA_PLAYER_DEPENDENCES}"
	ElectrodeMP
	"${ELECTRODEMP_LIBRARY_DEPENDENCES}"
	"${ElectrodeMP_MediaPlayer_SUB_DEPENDENCES}"
)

# ------------------------------------------------------------------------------
