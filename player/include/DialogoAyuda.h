
/// @file DialogoAyuda.h
///	@brief Archivo de cabecera para la implementaci�n del dialogo de ayuda de nuestra aplicaci�n.
/// 
/// @author Tyrel
/// @date Thursday 09/04/2021

#ifndef DIALOGO_AYUDA_H
#define DIALOGO_AYUDA_H

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------  Bases  ---------------------------------------

// Incluiremos para empezar nuestras cabeceras de C.

#include <stdio.h>
#include <stdlib.h>

// Los flujos estandar de C++.

#include <iostream>

// Ahora tenemos las cadenas de C++.

#include <string>

// ----------------------------  wxWidgets  ------------------------------------

// Incluiremos la cabecera de wxWidgets principal donde encontraremos todas las bases para poder trabajar nuestro programa , tenemos por
// consiguiente la siguiente cabecera.

#include <wx/wx.h>

// -----------------------  Modulo : PantallaBase ------------------------------

// Incluiremos el modulo para nuestra pantalla base donde tenemos contenida la declaraci�n del dialogo de dispositivo el cual usaremos
// para implementar cada evento.

#include <PantallaBase.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Empezaremos nuevamente por la declaraci�n de nuestra clase para el dialogo de ayuda derivando de la clase base DialogoAacerca.

/// @defgroup DialogoAyuda
/// @brief Pantalla de DIalogo para mostrar una selecci�n de informaci�n referente a la aplicaci�n y su(s) autor(es).
/// @{

/// @brief
///	Declaramos la clase que toma como base la declaraci�n del DialogoAcerca e implementa los eventos dentro de la misma asi como
/// inicializa todo el resto de componentes.

class DialogoAyuda : public DialogoAcerca {
	
	// Implementaremos la parte publica de la aplicaci�n.
	
	public :
		
		// ------------------------  Constructor  ------------------------------
		
		/// @name Constructor
		/// @brief Constructor del dialogo de ayuda mediante la ventana principal.
		/// @{
		
		/// @brief
		/// Declaramos un constructor que tome como parametro la referencia a nuestro reproductor de audio que actualmente estamos utilizando.
		/// @param Parent Pantalla Principal desde la cual fue invocado el dialogo.
		
		DialogoAyuda (wxWindow * Parent);
		
		/// @}
		
		// -------------------------  Evento  ----------------------------------
		
		/// @name Eventos
		/// @brief Pasaremos a declarar el evento para el bot�n de cerrar en la ventana de dialogo.
		/// @{
		
		/// @brief Evento para cerrar la ventana de dialogo actual.
		/// @param Argumentos Argumentos para nuestro evento para cerrar la ventana.
		
		void Evento_Cerrar (wxCommandEvent & Argumentos) override;
		
		/// @}
	
};

/// @}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // DIALOGO_AYUDA_H
