
// Implementaremos ahora nuestro codigo para el Dialogo de ayuda.

#include <DialogoAyuda.h>

/// @file DialogoAyuda.cpp
/// @brief Definición formal para nuestro dialogo que muestra la información de nuestra aplicación.
/// @author Tyrel
/// @date Viernes 09/04/2021

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// ---------------------------  Constructor  -----------------------------------

// Comenzaremos como de construmbre implementando el codigo para nuestro constructor del dialogo actual iniciando todos sus componentes.

DialogoAyuda::DialogoAyuda (wxWindow * Parent) : DialogoAcerca (Parent) {
	
	// Se establecen todas las bases del dialogo.
	
}

// -----------------------------  Eventos  -------------------------------------

// Implementaremos el evento

void DialogoAyuda::Evento_Cerrar (wxCommandEvent & Argumentos) {
	
	// Terminaremos el dialogo actual con el id de retorno ID_CANCEL.
	
	this->EndModal (wxID_CANCEL);
	
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------
