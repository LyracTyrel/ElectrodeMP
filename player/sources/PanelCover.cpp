
// Implementaremos ahora nuestro codigo para el widget de Panel de Cover.

#include <PanelCover.h>

/// @file PanelCover.cpp
/// @brief Definición formal para nuestro window personalizado que presenta una Caratula en pantalla.
/// @author Loubiner
/// @date Tuesday 06/03/2018

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------  PanelCover  -----------------------------------

// -----------------------------------------------------------------------------

// ---------------------------  Constructor  -----------------------------------

// Comenzaremos implementando el constructor de nuestro widget personalizado , esté tendra una sintaxis generica de cualquier widget e inicializara el bitmap
// a partir de la caratula predeterminada que tenemos en la aplicación.

PanelCover::PanelCover (wxWindow * Parental , wxWindowID ID , const wxPoint & Position , const wxSize & Size , long Estilo) :
	wxWindow (Parental , ID , Position , Size , Estilo) {
	
	// -------------------------  Iniciando Bitmap  ----------------------------
	
	// Iniciaremos el Bitmap por defecto para la aplicación que tomaremos del siguiente recurso.
	
	Mapa_Default = wxBitmap (wxString ("BITMAP_COVER") , wxBITMAP_TYPE_PNG_RESOURCE);
	
	// Asignaremos de igual forma el Bitmap actual con este mapa y el contexto mediante el metodo clear.
	
	Mapa_Cover = Mapa_Default;
	
	// Y lo colocaremos como fuente para nuestro contexto de memoria.
	
	Contexto_Cover.SelectObjectAsSource (Mapa_Cover);
	
}

// -----------------------------  Bitmaps  -------------------------------------

// Ahora vamos a implementar el metodo para asignar un Nuevo Mapa a nuestro widget mediante la caratula en la entrada.

void PanelCover::Set_Cover (const Caratula & Cover) {
	
	// Antes de continuar nos aseguraremos que tenemos un Cover valido.
	
	if (Cover.Is_Valid ()) {
		
		// Colocaremos nuestro Cover actualmente indicado en el widget establecido.
		
		Cover_Asignado = Cover;
		
		// Liberamos la imagen de Bitmap que tenemos ocupada en estos momentos.
		
		Contexto_Cover.SelectObjectAsSource (wxNullBitmap);
		
		// Ahora vamos a rellenar el Mapa con está información para asi poder establecer el dibujo a mostrar en el widget.
		
		Init_Bitmap ();
		
		// ---------------------------------------------------------------------
		
		// Validamos si tenemos el visualizador activo.
		
		#if defined (_WIN32) && defined (ELECTRODEMP_ENABLE_CIMG)
		
		// Cerraremos un visualizador previo antes de continuar.
		
		Caratula::Close_Preview ();
		
		// Iniciaremos el visualizador con esté cover.
		
		Caratula::Init_Preview (Cover_Asignado);
		
		#endif
		
		// ---------------------------------------------------------------------
		
		// Finalmente asignaremos el objeto de Bitmap del Contexto de Memoria actual.
		
		Contexto_Cover.SelectObjectAsSource (Mapa_Cover);
		
	}
	
}

// Ahora implementaremos el metodo siguiente para limpiar el Bitmap asignado.

void PanelCover::Clear_Cover () {
	
	// Cerraremos primero nuestra Ventana de Previsualización si todavia no lo está (unicamente para windows).
	
	#if defined (_WIN32) && defined (ELECTRODEMP_ENABLE_CIMG)
	
	// Cerramos la ventana.
	
	Caratula::Close_Preview ();
	
	#endif
	
	// ---------------------------------------------------------------------
	
	// Vamos a liberar la imagen de bitmap del contexto actual.
	
	Contexto_Cover.SelectObjectAsSource (wxNullBitmap);
	
	// Ahora asignaremos el Bitmap por Defecto a nuestra Imagen.
	
	Mapa_Cover = Mapa_Default;
	
	// Asignaremos el objeto de Bitmap del Contexto de Memoria actual.
	
	Contexto_Cover.SelectObjectAsSource (Mapa_Cover);
	
	// Y asignaremos una caratula por defecto vacia.
	
	Cover_Asignado = std::move (Caratula ());
	
}

// -----------------------------  Eventos  -------------------------------------

// Definiremos primero la Tabla de Eventos de nuestro widget actual con todos los eventos predeterminados que tenemos.

BEGIN_EVENT_TABLE (PanelCover , wxWindow)
	
	// Evento Paint de Repintado.
	
	EVT_PAINT (PanelCover::Evento_Paint)
	
	// Eventos de Mouse solo para windows.
	
	#if defined (_WIN32) && defined (ELECTRODEMP_ENABLE_CIMG)
	
	EVT_MOUSE_EVENTS (PanelCover::Evento_Mouse)
	
	#endif
	
END_EVENT_TABLE ()

// Implementaremos el Evento Paint para poder realizar el trazado del bitmap actual sobre nuestra Pantalla.
	
void PanelCover::Evento_Paint (wxPaintEvent & Pintador) {
	
	// Lo primero que debemos de hacer es recuperar el contexto de Paint para repintar nuestro panel.
	
	wxPaintDC Contexto (this);
	
	// ---------------------------  Background  --------------------------------
	
	// Empezaremos por limpiar nuestro contexto actual con un color blanco.
	
	Contexto.SetBackground (*wxWHITE_BRUSH);
	
	// Limpiamos el fondo actual.
	
	Contexto.Clear ();
	
	// -----------------------------  Cover  -----------------------------------
	
	// Comenzaremos con un simple ajuste a nuestro bitmap actual antes de pasar a pintar los pixeles de color.
	
	Adjust_Bitmap ();
	
	// Y finalmente pintaremos nuestro mapa de cover utilizando nuestro escalador de bitmap.
	//		Posicion Origen en : 0 , 0.
	//		Posicion Destino en : 0 , (Posicion_Cover.y).
	//		Size Origen : Actual del Mapa.
	//		Size Destino : Actual del Cover.
	//		Utilizaremos la función logica : Copia.
	//		Sin mascara de bits.
	
	Contexto.StretchBlit (Rectangulo_Cover.x , Rectangulo_Cover.y , Rectangulo_Cover.width , Rectangulo_Cover.height , &Contexto_Cover ,
		0 , 0 , Mapa_Cover.GetWidth () , Mapa_Cover.GetHeight ());
	
}

// Para el caso de windows implementaremos el siguiente evento tambien.

#if defined (_WIN32) && defined (ELECTRODEMP_ENABLE_CIMG)

// ---------------------------------------------------------------------

// Implementaremos el evento para poder mostrar la caratula de la song al pulsar sobre nuestro contexto.

void PanelCover::Evento_Mouse (wxMouseEvent & Argumentos) {
	
	// Validamos primero si tenemos una imagen de caratula valida.
	
	if (Cover_Asignado.Is_Valid ()) {
		
		// Validamos que nuestro mouse haya lanzado un evento Left-Up.
		
		if (Argumentos.LeftUp ()) {
			
			// Entonces mostraremmos la imagen de caratula asignada actualmente.
			
			Caratula::Show_Preview (Cover_Asignado);
			
		}
		
		// Si estamos entrando en la ventana el cursor cambiara a una mano.
		
		if (Argumentos.Entering ()) {
			
			// Colocaremos un ToolTip para mostrar el siguiente mensaje.
			
			SetToolTip (wxString ("Click para mostrar Caratula"));
			
			// Entonces pasaremos a asignar el cursor siguiente.
			
			SetCursor (wxCursor (wxCURSOR_HAND));
			
		}
		else if (Argumentos.Leaving ()) {
			
			// Colocaremos el cursor normal de flecha.
			
			SetCursor (wxCursor (wxCURSOR_ARROW));
			
		}
		
	}
	else {
		
		// Quitaremos los tooltips.
		
		UnsetToolTip ();
		
	}
	
}

#endif

// -----------------------------  Destructor  ----------------------------------

// Finalmente implementaremos el metodo virtual para que se encarge de la destrucción de nuestro panel actual correctamente.

PanelCover::~ PanelCover () {
	
	// Cerramos nuestra ventana de visualización del contexto (windows solamente).
	
#if defined (_WIN32) && defined (ELECTRODEMP_ENABLE_CIMG)
	
	// Cerramos la ventana.
	
	Cover_Asignado.Close_Preview ();
	
#endif
	
	// -------------------------------------------------------------------------
	
	// Vamos a liberar la imagen de bitmap del contexto actual.
	
	Contexto_Cover.SelectObjectAsSource (wxNullBitmap);
	
	// Y asignaremos una caratula por defecto vacia para eliminar la que tenemos actualmente asigada.
	
	Cover_Asignado = std::move (Caratula ());
	
}

// -----------------------------------------------------------------------------

// -----------------------------  Privados  ------------------------------------

// Implementaremos el metodo auxiliar para rellenar todos los Pixeles de nuestro Bitmap actual utilizando la caratula asignada.

void PanelCover::Init_Bitmap () {
	
	// Comenzaremos creando un mapa con las medidas actuales de la caratula las cuales son. Ancho de la Imagen.
	
	const int Ancho_Imagen = Cover_Asignado.Get_Width ();
	
	// Asi como el alto de la misma.
	
	const int Alto_Imagen = Cover_Asignado.Get_Height ();
	
	// Crearemos el nuevo bitmap con las medidas establecidas a 24 bits (3 Canales) y validaremos que sea correcta la asignación.
	
	if (Mapa_Cover.Create (Ancho_Imagen , Alto_Imagen , 24)) {
		
		// ---------------------  Rellenando Pixeles  --------------------------
		
		// Determinaremos el total de Pixeles en la Imagen actual multiplicando ambos tamaños.
		
		const size_t Cantidad_Pixeles = (Ancho_Imagen * Alto_Imagen);
		
		// Ahora vamos a crear un simple Iterador de Pixeles para nuestro nuevo bitmap con el que podremos rellenar con el color de cada pixel.
		
		wxNativePixelData Datos_Pixel (Mapa_Cover);
		
		// Validamos que tengamos valido el Iterador.
		
		if (!Datos_Pixel) {
			
			// Devolvemos la llamada.
			
			return;
			
		}
		
		// Ahora tomaremos el iterador al Origen de los Datos de la Imagen actual.
		
		wxNativePixelData::Iterator Pixel = Datos_Pixel.GetPixels ();
		
		// Obtendremos el iterador al Origen de nuestra Imagen de Portada.
		
		Caratula::const_iterator Pixel_Origen = Cover_Asignado.cbegin ();
		
		// Crearemos un iterador para la Imagen de Cover , esté tendra el final de la imagen.
		
		Caratula::const_iterator Pixel_Final = Cover_Asignado.cend ();
		
		// -----------------------  Asignando Pixeles  -------------------------
		
		// Iteraremos desde 0 hasta el total del Alto y Ancho de la Imagen mediante dos bucles , en cada uno realizaremos el desplazamiento de pixeles necesario.
		
		for (int Indice_Y = 0 ; (Indice_Y < Alto_Imagen) && (Pixel_Origen != Pixel_Final) ; ++ Indice_Y) {
			
			// Asignaremos el Origen de la Fila en el siguiente Iterador de respaldo.
			
			wxNativePixelData::Iterator Pixel_Inicio = Pixel;
			
			// Ahora vamos a iterar por todo el Ancho de la Imagen de la misma manera , en cada iteración el iterador de la Portada y el Pixel actual seran desplazados.
			
			for (int Indice_X = 0 ; (Indice_X < Ancho_Imagen) && (Pixel_Origen != Pixel_Final) ; ++ Indice_X , ++ Pixel , ++ Pixel_Origen) {
				
				// Asignaremos el Pixel de Origen en nuestro Bitmap de Salida por cada componente RGB si es valido.
				
				if (Pixel.IsOk ()) {
					
					// Asignamos cada componente de Color.
					
					Pixel.Red () = Pixel_Origen->Red ();
					Pixel.Green () = Pixel_Origen->Green ();
					Pixel.Blue () = Pixel_Origen->Blue ();
					
				}
				else {
					
					// Terminamos la asignación en este punto.
					
					return;
					
				}
				
			}
			
			// Finalmente restauramos el origen de la fila de pixeles que acabamos de asignar para pasar a la siguiente.
			
			Pixel = Pixel_Inicio;
			
			// Desplazamos en 1 la fila actual en el origen.
			
			Pixel.OffsetY (Datos_Pixel , 1);
			
		}
		
	}
	
}

// Definiremos el metodo para poder realizar el ajuste a las medidas y posición del bitmap actual.

void PanelCover::Adjust_Bitmap () {
	
	// ---------------------  Relación de Aspecto  -------------------------
	
	// Calcularemos la relación de aspecto que tiene la Imagen actual y con estó poder determinar el tamaño en alto que ocupara para mantener
	// la misma relación de aspecto.
	
	// Recuperamos el tamaño de nuestra ventana actual.
	
	const wxSize Size_Ventana (GetSize ());
	
	// Ahora vamos a calcular la relación de aspecto de la Imagen que tenemos. Dividimos el ancho entre el alto de nuestro bitmap.
	
	Aspecto_Cover = (static_cast <float> (Mapa_Cover.GetWidth ()) / Mapa_Cover.GetHeight ());
	
	// Con base a esta relación de aspecto calcularemos el Nuevo Alto que debe ocupar nuestra imagen en la pantalla sin perder el aspecto.
	
	const int Alto_Modificado = static_cast <int> (static_cast <float> (Size_Ventana.GetWidth ()) / Aspecto_Cover);
	
	// Asignaremos el tamaño para la Imagen total y la nueva Posición estará localizada en el origen en X (0) , y en (Alto_W / 2) - (Alto_Imagen / 2).
	
	Rectangulo_Cover = wxRect (0 , ((Size_Ventana.GetHeight () / 2) - (Alto_Modificado / 2)) , Size_Ventana.GetWidth () , Alto_Modificado);
	
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------
