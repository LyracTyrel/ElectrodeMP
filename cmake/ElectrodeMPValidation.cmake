
# Modulo para la función de Comprobación de Bibliotecas de dependencias opcionales.

# ------------------------------------------------------------------------------

# ---------------------------------  Bases  ------------------------------------

# Incluiremos el modulo para la comprobación de archivos include.



# -------------------------------  Validation  ---------------------------------

# Definiremos la siguiente función cuya labor consiste en comprobar que los directorios y archivos de bibliotecas
# para determinado packete estén asignados y en caso de no estarlo ,  se colocara una cadena de directorio actual.

# Está lleva como parametros el valor PACKAGE_FOUND , PACKAGE_INCLUDES , PACKAGE_LIBRARIES.
# Devuelve un valor TRUE si el paquete es valido.

function (ElectrodeMPValidation PACKAGE_FOUND PACKAGE_INCLUDES PACKAGE_LIBRARIES)

	# Tenemos el siguiente nombre del paquete actual.
	
	string (REPLACE "_FOUND" "" PACKAGE_NAME "${PACKAGE_FOUND}")
	
	# ----------------------------  Comprobación  ------------------------------
	
	# Empezaremos comprobando que el paquete fue encontrado , si no es asi entonces validaremos lo siguiente.
	
	if (NOT PACKAGE_FOUND)
		
		message (STATUS "Paquete opcional (${PACKAGE_NAME}) no encontrado")
		
		# Devolvemos falso.
		
		return (FALSE)
		
	endif ()
	
	# Mostraremos el siguiente mensaje apropiado.
	
	message (STATUS "Paquete opcional (${PACKAGE_NAME}) encontrado") 
	
	# Devolvemos true en esté caso.
	
	return (TRUE)

# Terminamos la función de validación.

endfunction (ElectrodeMPValidation)

# ------------------------------------------------------------------------------
