
/// @file Dialogo.h
///	@brief Archivo de cabecera para la declaraci�n de nuestro dialogo para la selecci�n de una API y dispositivo para la reproducci�n
/// de audio en nuestra pc.
/// 
/// Obtendremos el funcionamiento base de nuestro dialogo base e implementaremos los codigos para cada uno de los eventos que
/// est� ultimo tiene declarados.
/// @author Loubiner
/// @date Wednesday 2/21/2018

#ifndef DIALOGO_H
#define DIALOGO_H

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------  Bases  ---------------------------------------

// Incluiremos para empezar nuestras cabeceras de C.

#include <stdio.h>
#include <stdlib.h>

// Los flujos estandar de C++.

#include <iostream>

// Ahora tenemos las cadenas de C++.

#include <string>

// El vector para la lista de elementos.

#include <vector>

// Incluiremos algunas utilerias de C++ y memorias.

#include <memory>
#include <utility>

// -----------------------  Modulo : Reproductor  ------------------------------

// Incluiremos el funcionamiento base de nuestro reproductor el cual tiene los elementos necesarios para modificar el dispositivo y api
// de salida para la reproducci�n de audio. Asi mismo nos permite recuperar listas de dispositivo y host api's para nuestra facilidad
// de uso.

#include <ElectrodeMP/Reproductor.h>

// Importaremos su espacio de nombres.

using namespace ElectrodeMP;

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

// Empezaremos nuevamente por la declaraci�n de nuestra clase para el dialogo principal para cambiar entre Host  y dispositivo de salida.

/// @defgroup Dialogo
/// @brief Pantalla de DIalogo para mostrar una selecci�n de API y dispositivo de reproducci�n para el reproductor.
/// @{

/// @brief
///	Declaramos la clase que toma como base la declaraci�n del DialogoDevice para poder implementar los eventos que suceden en el mismo y
/// construir las listas de selecci�n con la informaci�n obtenida por el reproductor de nuestro constructor.

class Dialogo : public DialogoDevice {
	
	// Comenzaremos por la declaraci�n de nuestros elementos privados.
	
	private :
		
		// Declaramos el reproductor de audio actualmente utilizado para la implementaci�n de nuestro programa. Est� tendra que ser
		// referenciado desd� la aplicaci�n principal o la pantalla que invoque a este dialogo.
		
		std::shared_ptr <Reproductor> Player = std::shared_ptr <Reproductor> (nullptr);
	
	// Ahora vamos a implementar todos los eventos y constructor de nuestro dialogo actual.
	
	public :
		
		// ------------------------  Constructor  ------------------------------
		
		/// @name Constructor
		/// @brief Constructor del dialogo de dispositivo con el parametro especifico.
		/// @{
		
		/// @brief
		/// Declaramos un constructor que tome como parametro la referencia a nuestro reproductor de audio que actualmente estamos utilizando.
		/// @param Parent Pantalla Principal desde la cual fue invocado el dialogo.
		/// @param Player_Actual Referencia al reproductor de audio actualmente utilizado.
		
		Dialogo (wxWindow * Parent , const std::shared_ptr <Reproductor> & Player_Actual);
		
		/// @}
		
		// -------------------------  Eventos  ---------------------------------
		
		/// @name Eventos
		/// @brief Pasaremos a declarar alguna serie de eventos los cuales provienen de la superclase de DIalogo para poder interactuar con
		/// la aplicaci�n.
		/// @{
		
		/// @brief Evento para el cambio de API Host de nuestro reproductor , usaremos el siguiente metodo.
		/// @param Argumentos Argumentos para nuestro evento de seleecci�n de host.
		
		void Evento_Combo_Host (wxCommandEvent & Argumentos) override;
		
		/// @brief Evento para la aceptaci�n y aplicaci�n de nuestro dispositivo de salida de audio a nuestro reproductor actual.
		/// @param Argumentos Argumentos para el comando de nuestro evento.
		
		void Evento_Boton_Aceptar (wxCommandEvent & Argumentos) override;
		
		/// @brief Evento para la cancelaci�n de la operaci�n actual y volver a la pantalla original sin modificar nada.
		/// @param Argumentos Argumentos para el comando de nuestro evento.
		
		void Evento_Boton_Cancelar (wxCommandEvent & Argumentos) override;
		
		/// @}
	
	// Metodo para resetear nuestro audio abierto actualmente ya sea ese el caso.
	
	private :
		
		// -------------------------  Reset  -----------------------------------
		
		// Reseteamos el estado de nuestra reproducci�n para reabrir el stream con el nuevo Host/Device seleccionado.
		
		void Reset_Player ();
	
};

/// @}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

#endif
