
# Modulo para la comprobaci�n de funciones y elementos como tama�os , variables , estados , etc.

# ------------------------------------------------------------------------------

# --------------------------------  Bases  -------------------------------------

# Anexaremos el modulo para la macro de busqueda de Funciones en el sistema actual.

include (CheckCXXSymbolExists)

# Asignaremos las banderas para la comprobaci�n en C++11.

set (CMAKE_REQUIRED_FLAGS "-std=c++11")

# Configuramos los archivos de include para verificar la existencias de funciones.

list (APPEND CMAKE_REQUIRED_INCLUDES "string")

# ------------------------  String  To Numeric  --------------------------------

# Buscaremos la funci�n de nombre std::to_string (int) en el sistema para utilizarla.

CHECK_CXX_SYMBOL_EXISTS ("static_cast <std::string (&) (int)> (std::to_string)" "string" ELECTRODEMP_ENABLE_FUNC_TO_STRING)

# ------------------------------------------------------------------------------
