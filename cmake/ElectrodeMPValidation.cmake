
# Modulo para la funci�n de Comprobaci�n de Bibliotecas de dependencias opcionales.

# ------------------------------------------------------------------------------

# ---------------------------------  Bases  ------------------------------------

# Incluiremos el modulo para la comprobaci�n de archivos include.



# -------------------------------  Validation  ---------------------------------

# Definiremos la siguiente funci�n cuya labor consiste en comprobar que los directorios y archivos de bibliotecas
# para determinado packete est�n asignados y en caso de no estarlo ,  se colocara una cadena de directorio actual.

# Est� lleva como parametros el valor PACKAGE_FOUND , PACKAGE_INCLUDES , PACKAGE_LIBRARIES.
# Devuelve un valor TRUE si el paquete es valido.

function (ElectrodeMPValidation PACKAGE_FOUND PACKAGE_INCLUDES PACKAGE_LIBRARIES)

	# Tenemos el siguiente nombre del paquete actual.
	
	string (REPLACE "_FOUND" "" PACKAGE_NAME "${PACKAGE_FOUND}")
	
	# ----------------------------  Comprobaci�n  ------------------------------
	
	# Empezaremos comprobando que el paquete fue encontrado , si no es asi entonces validaremos lo siguiente.
	
	if (NOT PACKAGE_FOUND)
		
		message (STATUS "Paquete opcional (${PACKAGE_NAME}) no encontrado")
		
		# Devolvemos falso.
		
		return (FALSE)
		
	endif ()
	
	# Mostraremos el siguiente mensaje apropiado.
	
	message (STATUS "Paquete opcional (${PACKAGE_NAME}) encontrado") 
	
	# Devolvemos true en est� caso.
	
	return (TRUE)

# Terminamos la funci�n de validaci�n.

endfunction (ElectrodeMPValidation)

# ------------------------------------------------------------------------------
