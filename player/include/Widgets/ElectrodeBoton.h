
/// @file ElectrodeBoton.h
///	@brief Cabecera para el widget personalizado de Botón con fondo especifico para presentar en la pantalla de la aplicación
/// dandole una mejor presentación.
///
/// El botón se realizara utilizando las clases wxWindow de wxWidgets , de está forma podremos anexarlo como cualquier otro botón y
/// podra ser modificado y personalizado en diferentes contextos.
/// @author Menightmar
/// @date Lunes 19/03/2018

#ifndef ELECTRODEMP_BOTON_H
#define ELECTRODEMP_BOTON_H

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------  Bases  ---------------------------------------

// Incluiremos para empezar nuestras cabeceras de C.

#include <stdio.h>
#include <stdlib.h>

// Tenemos las funciones matematicas de C/C++.

#include <cmath>

// Ahora tenemos las cadenas de C++.

#include <string>

// ----------------------------  wxWidgets  ------------------------------------

// Incluiremos la cabecera para wxWidgets de donde ocuparemos principalmente al contenedor de wxWindow principal para poder derivar de
// esté nuestro Botón personalizado.

#include <wx/wx.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
	
/// @defgroup ElectrodeBoton
/// @brief Botón personalizado para la aplicación.
/// @{

/// @brief Clase wxWindow que provee de un simple botón el cual puede ser personalizado utilizando elementos adicionales como un fondo
/// para trazar el mismo.
///
/// El botón está conformado principalmente por las dimensiones del mismo , la posición , el estilo de wxWindow , tiene ademas el conjunto
/// de imagenes wxBitmapButton , adicionalmente permite especificar un mapa para la mascara de selección la cual permitira limitar el espacio donde el botón
/// puede ser presionado.

class ElectrodeBoton : public wxBitmapButton {
	
	// Tenemos las siguientes propiedades para nuestro botón personalizado.
	
	private :
		
		// ---------------------  Elementos Visuales  --------------------------
		
		// Tenemos como primer elemento el propio recuadro de nuestro botón actual el cual determina la posición y el tamaño que tiene con respecto al
		// contenedor donde está ubicado.
		
		wxRect Recuadro_Boton;
		
		// Ahora vamos a declarar un Bitmap que representa la zona habilitada para presionar el botón , esté es propiamente un bitmap monocromatico donde
		// la zona blanca representa la parte del botón que es presionable (clickeable) y la negra la zona donde no sucede nada.
		
		wxBitmap Zona_Boton;
	
	// Ahora vamos con los metodos publicos de la clase Botón.
	
	public :
		
		// ------------------------  Constructor  ------------------------------
		
		/// @name Constructores.
		/// @brief Constructores del Botón personalizado del media player.
		/// @{
		
		/// @brief Constructor por defecto vacio que inicia los elementos base del boton a valores por defecto.
		
		ElectrodeBoton ();
		
		/// @brief Constructor para iniciar el botón de bitmap con el mapa por defecto , estilo y propiedades como posición y tamaño inicial.
		/// @param Parent Window contenedor para el elemento.
		/// @param ID Identificador unico para el botón.
		/// @param Mapa Bitmap inicial por defecto para el botón.
		/// @param Position Posición asignada a nuestra ventana relativa al control contenedor Parent.
		/// @param Size Tamaño de la barra de progreso.
		/// @param Estilo Flags de estilo de wxWindow.
		
		explicit ElectrodeBoton (wxWindow * Parent , wxWindowID ID , const wxBitmap & Mapa, const wxPoint & Position = wxDefaultPosition ,
			const wxSize & Size = wxDefaultSize , long Estilo = 0);
		
		/// @}
		
		// ---------------------------  Zona  ----------------------------------
		
		/// @name Zona Hábil.
		/// @brief Metodos para asignar la zona habil de esté botón presonalizado.
		/// @{
		
		/// @brief Metodo para asignar un Bitmap monocromatico que representa la zona habil de esté botón.
		///
		/// La zona habil en un botón es basicamente la mascara que representa que parte del botón es pulsable y que parte no es tomada en cuenta como
		/// si el botón no existiera en esa zona (transparente). Si no se asigna un bitmap , por defecto todo el recuadro del cursor es considerado
		/// como si fuera zona hábil.
		/// @param Zona Bitmap monocromatico donde el color blanco representa la zona habil y el negro la zona invalida.
		
		void Set_Enable_Zone (const wxBitmap & Zona);
		
		/// @}
	
	// Ahora tenemos los metodos privados auxiliares de esté botón personalizado.
	
	private :
		
		// ----------------------------  Mouse  --------------------------------
		
		// Empezaremos por declarar un metodo autilixar para determinar si el mouse está posicionado por encima de la zona habil del botón actual.
		
		bool Boton_Mouse_Inside (const wxPoint & Posicion);
		
		// ---------------------------  Eventos  -------------------------------
		
		// Implementaremos los eventos internos que gestiona nuestro botón personalizado para ateder acciones tales como pulsar con el mouse , 
		// realizar cambio de tamaño , etc.
		
		// Empezaremos por implementar el metodo Click el cual simplemente realiza una pre-validación antes de efectuar el evento del super-metodo.
		
		void Boton_Click_Event (wxCommandEvent & Argumentos);
		
		// Ahora tenemos el metodo Mouse para implementar un codigo que de igual forma que en el anterior caso solamente valide cuando el mouse está
		// dentro de la zona especificada.
		
		void Boton_Mouse_Event (wxMouseEvent & Argumentos);
		
		// Por ultimo dejamos declarada la tabla de eventos para está clase.
		
		DECLARE_EVENT_TABLE ()
	
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/// @}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ELECTRODEMP_BOTON_H
