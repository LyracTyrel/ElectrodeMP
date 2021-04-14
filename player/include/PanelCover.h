
/// @file PanelCover.h
///	@brief Archivo de cabecera donde declaramos el componente (wxWindow) para presentar la Imagen de Portada de cualquier
/// audio que contenga alguno. Tambien tendremos un Bitmap por defecto para presentar por la pantalla cuando el audio no tiene
/// ningun cover que presentar.
///
/// Nuestro Reproductor es el encargado de abrir el archivo de audio para decodificar y leer los metadatos de la musica , mediante
/// estó ultimo es posible recuperar el Tag del audio donde encontramos entre la información la portada o caratula del album.
/// @author Loubiner
/// @date Tuesday 06/03/2018

#ifndef PANELCOVER_H
#define PANELCOVER_H

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------  Bases  ---------------------------------------

// Incluiremos para empezar nuestras cabeceras de C.

#include <stdio.h>
#include <stdlib.h>

// Tenemos las funciones matematicas de C/C++.

#include <cmath>

// Los flujos estandar de C++.

#include <iostream>

// Ahora tenemos las cadenas de C++.

#include <string>

// Funciones lambda de C++ 11.

#include <functional>

// Asi como las utilerias para trabajar con memorias y elementos de C++ 17.

#include <memory>
#include <utility>

// ----------------------------  wxWidgets  ------------------------------------

// Incluiremos los elementos de la biblioteca wxWidgets para poder trabar con wxBitmaps y wxWindow que es lo que requerimos para crear nuestro
// propio widget personalizado.

#include <wx/wx.h>

// Incluiremos el modulo de datos de Pixeles.

#include <wx/rawbmp.h>

// -----------------------  Modulo : Etiquetador  ------------------------------

// Adicionalmente incluiremos la referencia al Etiquetador donde contenemos la clase Caratula a utilizar como base para la generación del Bitmap
// que finalmente dibujara nuestro widget personalizado.

#include <ElectrodeMP/Etiquetador.h>

// Incluiremos el espacio de nombres de ElectrodeMP.

using namespace ElectrodeMP;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/// @defgroup Panel Cover
/// @brief wxWidget personalizado para presentar Imagenes de Caratula.
/// @{

/// @brief Declaramos la clase para mostrar la imagen de portada de un audio determinado.
///
/// Está clase simplemente implementa los elementos de un wxWidget y anexa un elemento adicional del tipo wxBitmap el cual realizara la correcta
/// presentación de la imagen por el contenedor.

class PanelCover : public wxWindow {
	
	// Comenzaremos declarando los elementos privados para este elemento.
	
	private :
		
		// ---------------------------  Cover  ---------------------------------
		
		// Tenemos en primer lugar el Bitmap de la Imagen a colocar a nuestro wxWindow , esté sera el objeto principal para dibujar.
		
		wxBitmap Mapa_Cover;
		
		// Tenemos un Mapa por defecto el cual es inicializado al momento de crear la ventana.
		
		wxBitmap Mapa_Default;
		
		// Ahora tenemos una referencia de copia a nuestra Caratula o Cover de la Song especificada.
		
		Caratula Cover_Asignado;
		
		// -------------------------  Contexto  --------------------------------
		
		// Asi mismo tenemos el contexto de memoria para poder trazar el mapa de cover actualmente asignado.
		
		wxMemoryDC Contexto_Cover;
		
		// Tenemos la relación de aspecto de nuestra imagen de entrada que usaremos para determinar la posicion de salida del mapa.
		
		float Aspecto_Cover = 1.0f;
		
		// El rectangulo que representa nuestro cover en la pantalla actual.
		
		wxRect Rectangulo_Cover;
	
	// Los elementos publicos de nuestro componente actual son los siguientes.
	
	public :
		
		// -------------------------  Constructor  -----------------------------
		
		/// @brief Tenemos el constructor por defecto de nuestro widget el cual realiza la simple construcción de un wxWidget por defecto , usaremos este constructor
		/// para poder iniciar los elementos base de cualquier widget.
		/// @param Parental Ventana o wxWidget superior donde estará contenido esté elemento.
		/// @param ID Identificador unico para el widget.
		/// @param Position Posicion donde se ubicará el widget.
		/// @param Size Tamaño de nuestro widget actual.
		/// @param Estilo Flags para definir el estilo del widget.
		
		PanelCover (wxWindow * Parental , wxWindowID ID = wxID_ANY , const wxPoint & Position = wxDefaultPosition , const wxSize & Size = wxDefaultSize ,
			long Estilo = wxTAB_TRAVERSAL);
		
		// --------------------------  Bitmaps  --------------------------------
		
		/// @name Mapas de Caratula.
		/// @brief Tenemos ahora la declaración de algunos metodos para realizar la asignación u obtención del Mapa a utilizar como portada.
		/// @{
		
		/// @brief Metodo para seleccionar una Caratula y asignarla a nuestra Ventana actual para mostrarla.
		/// @param Cover Caratula a utilizar para presentar en imagen sobre nuestro widget actual.
		
		void Set_Cover (const Caratula & Cover);
		
		/// @brief Metodo para devolver la Caratula asignada en estos momentos y poder modificar algunas propiedades o simplemente dejarla
		/// sin modificaciones.
		/// @return Referencia a la Caratula asignada a este Widget.
		
		ELECTRODEMP_FORCE_INLINE Caratula & Get_Cover () {
			
			// Devolvemos la caratula que tiene nuestro widget.
			
			return Cover_Asignado;
			
		}
		
		/// @brief Metodo para devolver la Caratula asignada en estos momentos y poder modificar algunas propiedades o simplemente dejarla
		/// sin modificaciones.
		/// @return Referencia constante a la Caratula asignada a este Widget.
		
		ELECTRODEMP_FORCE_INLINE const Caratula & Get_Cover () const {
			
			// Devolvemos la caratula que tiene nuestro widget.
			
			return Cover_Asignado;
			
		}
		
		/// @brief Metodo para limpiar la portada actualmente asignada y dejarla en la portada o caratula por defecto.
		
		void Clear_Cover ();
		
		/// @}
		
		// --------------------------  Eventos  --------------------------------
		
		/// @name Eventos de Cover.
		/// @brief Finalmente tenemos la declaración formal de algunos eventos predeterminados por el widget para poder pintar nuestro cover y mostrarlo
		/// directamente en su visualizador incluido de GDI32 para la plataforma windows.
		/// @{
		
		/// @brief Comenzaremos con el evento paint el cual realiza la presentación del Bitmap asignado actualmente en la pantalla.
		/// 
		/// Metodo para el evento Paint del Contenedor de Tag de Etiquetas para el audio reproducido , tenemos esté metodo el cual
		/// realiza el pintado sobre nuestro objeto actual.
		/// @param Argumentos Parametros del evento para utilizar.
		
		void Evento_Paint (wxPaintEvent & Argumentos);
		
#if defined (_WIN32) && defined (ELECTRODEMP_ENABLE_CIMG)
		
		/// @brief Metodo de evento para el caso de pulsar sobre el Cover actual y mostrar la ventana de Visualizaación (Solamente Windows).
		/// @param Argumentos Argumento para el evento.
		
		void Evento_Mouse (wxMouseEvent & Argumentos);
		
#endif
		
		/// @}
		
		/// @brief Declaramos la tabla de eventos para el widget actual.
		
		DECLARE_EVENT_TABLE ()
		
		// -------------------------  Destructor  ------------------------------
		
		/// @brief Tenemos la implementación de nuestro destructor para la clase actual.
		
		virtual ~ PanelCover ();
	
	// Algunos metodos auxiliares y privados.

	private :
		
		// Tenemos un metodo simple cuyo trabajo es inicializar el Mapa actual con la Caratula asignada a esté contexto , de igual manera establece el
		// contexto de memoria para poder trabajar al momento de repintar.
		
		void Init_Bitmap ();
		
		// Tenemos un metodo para poder ajustar el bitmap a mostrar en pantalla usando las medidas actuales de la ventana y la imagen de caratula.
		
		void Adjust_Bitmap ();
	
};

/// @}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

#endif
