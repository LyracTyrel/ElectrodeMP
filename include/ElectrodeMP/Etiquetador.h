
/// @file Etiquetador.h
///	@brief Este archivo de cabecera incluye los elementos para todo lo relacionado a los TAG de audio.
///
/// Tenemos clases para la construcción y obtención de Etiquetas Tag , a partir de un archivo de audio en alguno de los
/// formatos posibles. Permite la decodificación de caratulas de Songs que asi lo contengan en imagenes RGB que pueden 
/// set procesadas para realizar cualquier cosa.
/// @author Loubiner
/// @date Sunday 25/02/2018

#ifndef ELECTRODEMP_ETIQUETADOR_H
#define ELECTRODEMP_ETIQUETADOR_H

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------  Bases  ---------------------------------------

// Incluiremos para empezar nuestras cabeceras de C.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Tenemos las funciones matematicas de C/C++.

#include <cmath>

// Algunos algoritmos de busqueda.

#include <algorithm>

// Los flujos estandar de C++.

#include <iostream>

// Incluiremos el contenedor de vector.

#include <vector>

// Algunos iteradores para el manejo del Color.

#include <iterator>

// Ahora tenemos las cadenas de C++.

#include <string>

// Incluiremos nuestros threads y nuestros datos atomicos.

#include <thread>
#include <atomic>

// Funciones lambda de C++ 11.

#include <functional>

// Incluiremos el tratamiento de tipos.

#include <type_traits>

// Asi como las utilerias para trabajar con memorias y elementos de C++ 17.

#include <memory>
#include <utility>

// -------------------------  Configuracion  -----------------------------------

// Vamos a incluir nuestra configuración para todo el proyecto en general.

#include <ElectrodeMP/Configuracion.h>

// -----------------------------  LOG  -----------------------------------------

// Incluiremos el LOG para registrar todos los procesos por los que pasa ElectrodeMP.

#include <ElectrodeMP/LOG.h>

// ----------------------------  Color  ----------------------------------------

// Incluiremos ademas nuestra clase Color y el Iterador para la manipulación de las imagenes.

#include <ElectrodeMP/Color.h>

// -----------------------------------------------------------------------------

// ----------------------------  TagLib  ---------------------------------------

// Para nuestro modulo actual requerimos de la biblioteca de etiquetas ID3v2 entre otros formatos , TagLib. Esta nos proporciona todos los elementos
// necesarios para obtener la mayor cantidad de información posibles sobre un archivo de audio. Usaremos algunas de sus clases para poder recuperar
// los tags de archivos de audio en varios formatos.

// Empezaremos por incluir el importador de Archivos general de referencia.

#include <fileref.h>

// Incluiremos los importadores de archivos especificos : mp3 , wav , mp4 , ogg y flac.

#include <mpeg/mpegfile.h>
#include <wav/wavfile.h>
#include <mp4/mp4file.h>
#include <ogg/oggfile.h>
#include <ogg/vorbis/vorbisfile.h>
#include <ogg/flac/oggflacfile.h>
#include <flac/flacfile.h>

// Incluiremos ahora el mapa de propiedades (declaración) para los datos más especificos del audio.

#include <tpropertymap.h>

// Incluiremos las propiedades tecnicas del audio.

#include <audioproperties.h>

// Anexaremos asi mismo la cabecera que tiene la declaración formal de la clase TagID3v2 de donde obtendremos la información.

#include <mpeg/id3v2/id3v2tag.h>

// Utilizaremos la clase base para todos los frames.

#include <mpeg/id3v2/id3v2frame.h>

// Y por supuesto necesitamos ahora el tipo de Frame especifico con el que trabajaremos el cual es de tipo AttachedPictureFrame
// y lo podemos encontrar en la siguiente cabecera.

#include <mpeg/id3v2/frames/attachedpictureframe.h>

// Importaremos el espacio de nombres de TagLib para acortar un poco las declaraciones.

using namespace TagLib;

// Incluiremos la biblioteca de CImg si la tenemos activada para poder realizar la asignación de los datos de la imagen.

#if defined (ELECTRODEMP_ENABLE_CIMG)

// -------------------------------  CImg  --------------------------------------

// Cimage es una biblioteca conformada unicamente por un archivo de cabecera , en este tenemos la declaración de una clase para contener los datos de
// una imagen , podemos realizar procesamiento basico con la misma. Usaremos esta clase para poder contener la imagen de caratula de las songs.

#include <CImg.h>

// Importaremos el espació de nombres para la biblioteca.

using namespace cimg_library;

// -----------------------------------------------------------------------------

#endif

// Incluiremos ademas la biblioteca de OpenCV para realizar procesamiento (y/o decodificación de la Imagen).

#if defined (ELECTRODEMP_ENABLE_OPENCV)

// ------------------------------  OpenCV  -------------------------------------

// Finalmente incluiremos la biblioteca de procesamiento de imagen y decodificación de archivos de Imagen. OpenCV , está nos porporciona una interfaz
// sensilla y facil de utilizar para realizar algoritmos de procesamiento simples hasta avanzados , incluiremos en general solo dos de sus modulos.

// Incluiremos el modulo de procesamiento de imagenes.

#include <opencv2/imgproc.hpp>

// Y la parte relacionada a los decodificadores de formatos de imagen.

#include <opencv2/imgcodecs.hpp>

// -----------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Incluiremos todo el codigo dentro del siguiente espacio de nombres.

namespace ElectrodeMP {

// Vamos a realizar declaraciónes de varios elementos que conforman parte de la Etiqueta TAG general para todos los archivos , comenzando por el cover
// o imagen de caratula. Guardaremos todos los elementos siguientes a declarar en el grupo apropiado.

/// @defgroup Etiquetador
/// @brief Clases de utilidad para la obtención de metadatos de los archivos de audio , tales como :
///		@li @b Titulo
///		@li @b Año
///		@li @b Compositor
///		@li @b Artista
///		@li @b Album
///		@li @b Imagen de Caratula.
///
/// Todos estos elementos seran encapsulados en una estructura la cual contiene todos los campos que repreentan la @ref Etiqueta de un archivo de
/// audio abierto por el reproductor.
/// @{

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// ------------------------------  Caratula  -----------------------------------

/// @brief Estructura para la contención de los metadatos relacionados a la imagen de portada o cover de nuestro archivo de audio. Algunos archivos de audio
/// como los MP3 pueden tener incluido una imagen de portada a mostrar.

struct Caratula {
	
	// Tenemos el modo de pixel para el caso de CImg habilitado.
	
#if defined (ELECTRODEMP_ENABLE_CIMG)
	
	// -----------------------------  CImg  ------------------------------------
	
	/// @brief Definiremos el tipo de CImg a utilizar para nuestro programa.
	
	typedef CImg <ELECTRODEMP_PIXEL_TYPE> CImagen;
	
	// -------------------------------------------------------------------------
	
	// Ahora vamos a declarar los elementos de la imagen para el caso de contar con OpenCV.

#elif defined (ELECTRODEMP_ENABLE_OPENCV)
	
	// ----------------------------  OpenCV  -----------------------------------
	
	/// @brief Definiremos el tipo de Matriz de OpenCV a utilizar para nuestro programa.
	
	typedef cv::Mat_ <ELECTRODEMP_PIXEL_TYPE> CImagen;
	
	// -------------------------------------------------------------------------
	
	// En caso de no contar con estás bibliotecas , usaremos la manera tradicional.
	
#else
	
	// ----------------------------  Native  -----------------------------------
	
	/// @brief Crearemos una estructura basica de plantilla que contenga la información sobre la imagen tal como sus datos de pixeles , ancho , alto , canales.
	
	template <typename TypePixel>
	struct Native_Imagen {
		
		// ----------------------------  Tipos  --------------------------------
		
		/// @brief Tipo de Pixel utilizado para la imagen.
		
		typedef TypePixel value_type;
		
		/// @brief Tipo de Contenedor utilizado para los datos.
		
		typedef std::vector <value_type> container_type;
		
		/// @brief Tipo de Iterador utilizado para los datos.
		
		typedef container_type::iterator iterator;
		
		/// @brief Tipo de Iterador Constante utilizado para los datos.
		
		typedef container_type::const_iterator const_iterator;
		
		// ---------------------------  Datos  ---------------------------------
		
		/// @brief Definiremos el tipo de array de pixeles nativo y compartido para el caso normal en forma de un vector.
		
		container_type Datos;
		
		/// @brief Ancho de la Imagen en Pixeles.
		
		int Ancho = 0;
		
		/// @brief Alto de la Imagen en Pixeles.
		
		int Alto = 0;
		
		/// @brief Numero de Canales de la Imagen.
		
		int Canales = 0;
		
		// ------------------------  Constructores  ----------------------------
		
		/// @brief Constructor por defecto vacio , inicia los elementos por defecto.
		
		Native_Imagen () = default;
		
	};
	
	/// @brief Definiremos el tipo de Imagen nativa a utilizar de acuerdo al tipo de pixel elegido.
	
	typedef Native_Imagen <ELECTRODEMP_PIXEL_TYPE> CImagen;
	
#endif
	
	// -----------------------------  Tipos  -----------------------------------
	
	/// @brief Definiremos el tipo de Pixel ocupado nuevamente para tenerlo como referencia.
	
	typedef CImagen::value_type CComponente;
	
	/// @brief Definición del tipo de Iterador de Color interno (convertible a const_iterator).
	
	typedef Color_Iterator <CComponente , CImagen::iterator> iterator;
	
	/// @brief Definición del tipo de Color (Color Iterador) para la componente de Color y el Iterador de la CImagen.
	
	typedef typename iterator::value_type CColor;
	
	/// @brief Definición del tipo de Iterador de Color constante interno.
	
	typedef Color_Iterator <const CComponente , CImagen::const_iterator> const_iterator;
	
	/// @brief Definición del tipo de Color (Color Iterador) constante para la componente de Color y el Iterador de la CImagen.
	
	typedef typename const_iterator::value_type CConstColor;
	
	// ---------------------------  Elementos  ---------------------------------
	
	/// @brief Declaración formal de la Imagen interna contenida para la caratula.
	/// 
	/// @note Si contamos con @b CImg : Declaramos el elemento principal para la caratula. Es decir , la imagen de CImg que se está presentando en este momento ,
	/// está tendra la información de todos los pixeles de la imagen procesados por opencv. Tendremos una referencia a la memoria ocupada por esta caratula para evitar
	/// fugas de memoria y poder compartirla cuando sea necesario. Iniciada en null por defecto.
	/// @note Si contamos con @b OpenCV : Declaramos ahora el elemento para el caso de OpenCV , una instancia de la clase CImagen bastara para poder realizar la asignación
	/// correcta y la destrucción de la imagen cuando el objeto no se necesite más.
	/// @note Si no contamos con alguna de estás bibliotecas entonces : Declaramos unicamente el elemento de Imagen , el array de pixeles organizados de la manera tradicional
	/// es decir , utilizando el modo RGBRGBRGB que nos provee las bibliotecas decodificadoras para contener la info de la imagen.
	
	CImagen Imagen;
	
	/// @brief Tamaño de la Caratula actual a lo ancho.
	/// @note Este tamaño puede diferir dependiendo la implementación de CImagen que se haya seleccionado.
	
	int Imagen_Ancho = 0;
	
	/// @brief Tamaño de la Caratula actual a lo alto.
	/// @note Este tamaño puede diferir dependiendo la implementación de CImagen que se haya seleccionado.
	
	int Imagen_Alto = 0;
	
	/// @brief Número de canales que tiene nuestra caratula.
	/// @note Este tamaño puede diferir dependiendo la implementación de CImagen que se haya seleccionado.
	
	int Imagen_Canales = 0;
	
	// -------------------------  Constructores  -------------------------------
	
	/// @name Constructores por Defecto.
	/// @brief Declaramos los cinco tipos de constructores que tiene nuestra Caratula para su creación.
	/// @{
	
	/// @brief Empezando por la declaración y definición del constructor por defecto el cual crea una caratula vacia.
	
	Caratula () = default;
	
	/// @brief Constructor con los pixeles de la imagen del tipo de PIXEL apropiado en formato RGB.
	/// @param Pixeles_Imagen Datos rgb organizados en la forma RGBRGBRGB ...... RNGNBN.
	/// @param Ancho Ancho de la imagen de entrada.
	/// @param Alto Alto de la imagen de entrada.
	/// @param Canales Numero de canales de la imagen de entrada.
	/// @note El numero de elementos en Pixeles_Imagen debe ser igual o mayor a (Ancho * Alto * Canales) del tipo de Pixel elegido.
	
	explicit Caratula (const CComponente * Pixeles_Imagen , size_t Ancho , size_t Alto , size_t Canales);
	
#if defined (ELECTRODEMP_ENABLE_OPENCV)
	
	/// @brief Constructor mediante copia por referencia de cv::Mat la cual tiene los pixeles asignados así como la información de la imagen
	/// que fue decodificada.
	/// @param Imagen_Referencia Imagen cv::Mat de origen para crear una copia de referencia.
	
	explicit Caratula (const cv::Mat & Imagen_Referencia);
	
#endif
	
	/// @brief Constructor de Copia por defecto.
	/// @param Copia Referencia a una Caratula a copiar.
	
	Caratula (const Caratula & Copia) = default;
	
	/// @brief Constructor de Movimiento por defecto.
	/// @param Mover Referencia a un r-value de Caratula.
	
	Caratula (Caratula && Mover) = default;
	
	/// @}
	
	/// @name Asignadores.
	/// @brief Asi mismmo contamos con las asignaciones por copia y movimiento por defecto.
	/// @{
	
	/// @brief Asignación por Copia. Devuelve al referencia a esté Objeto.
	/// @param Copia Caratula a copiar.
	/// @param Esté Objeto.
	
	Caratula & operator = (const Caratula & Copia) = default;
	
	/// @brief Asignación por Movimiento. Devuelve al referencia a esté Objeto.
	/// @param Mover Caratula a mover.
	/// @param Esté Objeto.
	
	Caratula & operator = (Caratula && Mover) = default;
	
	/// @}
	
	// -----------------------------  Accesores  -------------------------------
	
	/// @name Metodos de Acceso.
	/// @brief Contamos con algunos metodos miembro que devuelven información sobre la imagen como sus medidas entre algunos otros valores para poder
	/// utilizar en las aplicaciones.
	/// @{
	
	/// @brief Comenzaremos con un simple metodo que nos permite determinar si tenemos una caratula valida en estos momentos utilizando la CImage
	/// actualmente asignada.
	/// @return True si la imagen de cover actual es valida.
	
	ELECTRODEMP_FORCE_INLINE bool Is_Valid () const {
		
		// Validaremos que la referencia sea correcta y la imagen contenida no esté vacia.
		
		return 
			
			#if defined (ELECTRODEMP_ENABLE_CIMG)
			
			!Imagen.is_empty ();
			
			#elif defined (ELECTRODEMP_ENABLE_OPENCV)
			
			!Imagen.empty ();
			
			#else
			
			!Imagen.Datos.empty ();
			
			#endif
		
	}
	
	/// @brief Metodo para obtener el Ancho en pixeles de nuestra Imagen actualmente asignada.
	/// @return El ancho de la imagen o 0 si no hay imagen valida.
	
	ELECTRODEMP_FORCE_INLINE int Get_Width () const {
		
		// Devolvemos el ancho que tiene la Imagen actualmente asignada.
		
		return (Is_Valid () ? Imagen_Ancho : 0);
		
	}
	
	/// @brief Metodo para obtener el Alto de la Imagen en pixeles.
	/// @return El alto de la Imagen o 0 si no hay imagen valida.
	
	ELECTRODEMP_FORCE_INLINE int Get_Height () const {
		
		// Devolvemos el alto de la imagen actualmente inicializada.
		
		return (Is_Valid () ? Imagen_Alto : 0);
		
	}
	
	/// @brief Metodo para obtener el Número de Canales en la Imagen actualmente asignada.
	/// @return El número de Canales en la Imagen (1 - Monocromatica , 3 - RGB , 4 - GBA).
	
	ELECTRODEMP_FORCE_INLINE int Get_Channels () const {
		
		// Devolvemos la cantidad de canales en la Imagen o lo que es igual , el spectrum de la misma.
		
		return (Is_Valid () ? Imagen_Canales : 0);
		
	}
	
	/// @brief Obtenemos la Cantidad de Pixeles en la Imagen , considerando el ancho y alto de la misma.
	/// @note El tamaño de la imagen es igual al (Ancho * Alto) de la misma.
	/// @return El numero de Pixeles en la Imagen de Caratula.
	
	ELECTRODEMP_FORCE_INLINE int Get_PixelCount () const {
		
		// Devolvemos el producto de Ancho * Alto.
		
		return (Is_Valid () ? (Imagen_Ancho * Imagen_Alto) : 0);
		
	}
	
	/// @}
	
	// ---------------------------  Iteradores  --------------------------------
	
	/// @name Iteradores.
	/// @brief Tenemos ahora algunos metodos para obtener y recuperar iteradores de Color para poder recorrer la Imagen actual en un formato de
	/// pixel RGB , siendo de esta manera.
	/// 
	/// Los pixeles de la Imagen actual estan organizados de la forma RRRRRR......GGGG.......BBBBB...... , cada plano o canal está separado uno del otro
	/// por lo que deberiamos iterar por cada uno de estos por separado para poder obtener la información que deseamos. Usando la clase @ref Caratula::Color es
	/// posible obtener la información en cada canal (R , G , B) de manera conjunta e iterar por toda la imagen.
	/// @{
	
	/// @brief Obtenemos un iterador al origen de los Pixeles , la esquina superior izquierda de la Imagen donde podemos comenzar a iterar.
	///
	/// La iteración se lleva a cabo en sentido de izquierda a derecha y de arriba hacia abajo de la imagen.
	/// @return Iterador al inicio de la imagen.
	
	ELECTRODEMP_FORCE_INLINE iterator begin () {
		
		// Construiremos un iterador al origen de la Imagen (Origen de canales R , G , B) , de lo contrario un iterador por defecto.
		
		return (Is_Valid () ? iterator (
			
			#if defined (ELECTRODEMP_ENABLE_CIMG)	
			
			// -----------------------  CImg  ----------------------------------
			
			Imagen.data () ,					// Origen en R.
			Imagen.data (0 , 0 , 0 , 1) ,		// Origen en G.
			Imagen.data (0 , 0 , 0 , 2)			// Origen en B.
			
			// -----------------------------------------------------------------
			
			#elif defined (ELECTRODEMP_ENABLE_OPENCV)
			
			// ----------------------  OpenCV  ---------------------------------
			
			(Imagen.begin () + 2u) ,	// Origen en R.
			(Imagen.begin () + 1u) ,	// Origen en G.
			Imagen.begin () , 			// Origen en B.
			Imagen_Canales				// Incremento en cantidad de canales.
			
			// -----------------------------------------------------------------
			
			#else
			
			// ----------------------  Native  ---------------------------------
			
			Imagen.Datos.begin () ,												// Origen en R.
			(Imagen.Datos.begin () + (Get_PixelCount () * Get_Channels ()))	,	// Origen en G.
			(Imagen.Datos.begin () + (Get_PixelCount () * Get_Channels () * 2))	// Origen en B.
			
			// -----------------------------------------------------------------
			
			#endif
			
			) : iterator ());
		
	}
	
	/// @brief Obtenemos un iterador al origen de los Pixeles.
	/// @return Iterador constante al inicio de la imagen.
	
	ELECTRODEMP_FORCE_INLINE const_iterator begin () const {
		
		// Construiremos un iterador al origen de la Imagen (Origen de canales R , G , B) , de lo contrario un iterador por defecto.
		
		return (Is_Valid () ? const_iterator (
			
			#if defined (ELECTRODEMP_ENABLE_CIMG)	
			
			// -----------------------  CImg  ----------------------------------
			
			Imagen.data () ,					// Origen en R.
			Imagen.data (0 , 0 , 0 , 1) ,		// Origen en G.
			Imagen.data (0 , 0 , 0 , 2)			// Origen en B.
			
			// -----------------------------------------------------------------
			
			#elif defined (ELECTRODEMP_ENABLE_OPENCV)
			
			// ----------------------  OpenCV  ---------------------------------
			
			(Imagen.begin () + 2u) ,	// Origen en R.
			(Imagen.begin () + 1u) ,	// Origen en G.
			Imagen.begin () ,			// Origen en B.
			Imagen_Canales				// Incremento en cantidad de canales.
			
			// -----------------------------------------------------------------
			
			#else
			
			// ----------------------  Native  ---------------------------------
			
			Imagen.Datos.begin () ,												// Origen en R.
			(Imagen.Datos.begin () + (Get_PixelCount () * Get_Channels ()))	,	// Origen en G.
			(Imagen.Datos.begin () + (Get_PixelCount () * Get_Channels () * 2))	// Origen en B.
			
			// -----------------------------------------------------------------
			
			#endif
			
			) : const_iterator ());
		
	}
	
	/// @brief Obtenemos un iterador al origen de los Pixeles.
	/// @return Iterador constante al inicio de la imagen.
	
	ELECTRODEMP_FORCE_INLINE const_iterator cbegin () const {
		
		// Construiremos un iterador al origen de la Imagen (Origen de canales R , G , B) , de lo contrario un iterador por defecto.
		
		return (Is_Valid () ? const_iterator (
			
			#if defined (ELECTRODEMP_ENABLE_CIMG)	
			
			// -----------------------  CImg  ----------------------------------
			
			Imagen.data () ,					// Origen en R.
			Imagen.data (0 , 0 , 0 , 1) ,		// Origen en G.
			Imagen.data (0 , 0 , 0 , 2)			// Origen en B.
			
			// -----------------------------------------------------------------
			
			#elif defined (ELECTRODEMP_ENABLE_OPENCV)
			
			// ----------------------  OpenCV  ---------------------------------
			
			(Imagen.begin () + 2u) ,	// Origen en R.
			(Imagen.begin () + 1u) ,	// Origen en G.
			Imagen.begin () ,			// Origen en B.
			Imagen_Canales				// Incremento en cantidad de canales.
			
			// -----------------------------------------------------------------
			
			#else
			
			// ----------------------  Native  ---------------------------------
			
			Imagen.Datos.begin () ,												// Origen en R.
			(Imagen.Datos.begin () + (Get_PixelCount () * Get_Channels ()))	,	// Origen en G.
			(Imagen.Datos.begin () + (Get_PixelCount () * Get_Channels () * 2))	// Origen en B.
			
			// -----------------------------------------------------------------
			
			#endif
			
			) : const_iterator ());
		
	}
	
	/// @brief Obtenemos un iterador al post-final de los Pixeles.
	/// @return Iterador constante al final de la imagen.
	
	ELECTRODEMP_FORCE_INLINE iterator end () {
		
		// Construiremos un iterador al final de la Imagen (Origen de canales R , G , B) , de lo contrario un iterador por defecto.
		
		return (Is_Valid () ? iterator (
			
			#if defined (ELECTRODEMP_ENABLE_CIMG)	
			
			// -----------------------  CImg  ----------------------------------
			
			(Imagen.data () + Get_PixelCount ()) ,					// Final en R.
			(Imagen.data (0 , 0 , 0 , 1) + Get_PixelCount ()) ,		// Final en G.
			(Imagen.data (0 , 0 , 0 , 2) + Get_PixelCount ())		// Final en B.
			
			// -----------------------------------------------------------------
			
			#elif defined (ELECTRODEMP_ENABLE_OPENCV)
			
			// ----------------------  OpenCV  ---------------------------------
			
			(Imagen.begin () + (Get_PixelCount () * Get_Channels () + 2u)) ,	// Origen en R.
			(Imagen.begin () + (Get_PixelCount () * Get_Channels () + 1u)) ,	// Origen en G.
			(Imagen.begin () + (Get_PixelCount () * Get_Channels ())) ,			// Origen en B.
			Imagen_Canales														// Incremento.
			
			// -----------------------------------------------------------------
			
			#else
			
			// ----------------------  Native  ---------------------------------
			
			(Imagen.Datos.begin () + (Get_PixelCount () * Get_Channels ())) ,		// Origen en R.
			(Imagen.Datos.begin () + (Get_PixelCount () * Get_Channels () * 2))	,	// Origen en G.
			(Imagen.Datos.begin () + (Get_PixelCount () * Get_Channels () * 3))		// Origen en B.
			
			// -----------------------------------------------------------------
			
			#endif
			
			) : iterator ());
		
	}
	
	/// @brief Obtenemos un iterador al post-final de los Pixeles.
	/// @return Iterador constante al final de la imagen.
	
	ELECTRODEMP_FORCE_INLINE const_iterator end () const {
		
		// Construiremos un iterador al final de la Imagen (Origen de canales R , G , B) , de lo contrario un iterador por defecto.
		
		return (Is_Valid () ? const_iterator (
			
			#if defined (ELECTRODEMP_ENABLE_CIMG)	
			
			// -----------------------  CImg  ----------------------------------
			
			(Imagen.data () + Get_PixelCount ()) ,					// Final en R.
			(Imagen.data (0 , 0 , 0 , 1) + Get_PixelCount ()) ,		// Final en G.
			(Imagen.data (0 , 0 , 0 , 2) + Get_PixelCount ())		// Final en B.
			
			// -----------------------------------------------------------------
			
			#elif defined (ELECTRODEMP_ENABLE_OPENCV)
			
			// ----------------------  OpenCV  ---------------------------------
			
			(Imagen.begin () + (Get_PixelCount () * Get_Channels () + 2u)) ,	// Origen en R.
			(Imagen.begin () + (Get_PixelCount () * Get_Channels () + 1u)) ,	// Origen en G.
			(Imagen.begin () + (Get_PixelCount () * Get_Channels ())) ,			// Origen en B.
			Imagen_Canales														// Incremento.
			
			// -----------------------------------------------------------------
			
			#else
			
			// ----------------------  Native  ---------------------------------
			
			(Imagen.Datos.begin () + (Get_PixelCount () * Get_Channels ())) ,		// Origen en R.
			(Imagen.Datos.begin () + (Get_PixelCount () * Get_Channels () * 2))	,	// Origen en G.
			(Imagen.Datos.begin () + (Get_PixelCount () * Get_Channels () * 3))		// Origen en B.
			
			// -----------------------------------------------------------------
			
			#endif
			
			) : const_iterator ());
		
	}
	
	/// @brief Obtenemos un iterador al post-final de los Pixeles.
	/// @return Iterador constante al final de la imagen.
	
	ELECTRODEMP_FORCE_INLINE const_iterator cend () const {
		
		// Construiremos un iterador al final de la Imagen (Origen de canales R , G , B) , de lo contrario un iterador por defecto.
		
		return (Is_Valid () ? const_iterator (
			
			#if defined (ELECTRODEMP_ENABLE_CIMG)	
			
			// -----------------------  CImg  ----------------------------------
			
			(Imagen.data () + Get_PixelCount ()) ,					// Final en R.
			(Imagen.data (0 , 0 , 0 , 1) + Get_PixelCount ()) ,		// Final en G.
			(Imagen.data (0 , 0 , 0 , 2) + Get_PixelCount ())		// Final en B.
			
			// -----------------------------------------------------------------
			
			#elif defined (ELECTRODEMP_ENABLE_OPENCV)
			
			// ----------------------  OpenCV  ---------------------------------
			
			(Imagen.begin () + (Get_PixelCount () * Get_Channels () + 2u)) ,	// Origen en R.
			(Imagen.begin () + (Get_PixelCount () * Get_Channels () + 1u)) ,	// Origen en G.
			(Imagen.begin () + (Get_PixelCount () * Get_Channels ())) ,			// Origen en B.
			Imagen_Canales														// Incremento.
			
			// -----------------------------------------------------------------
			
			#else
			
			// ----------------------  Native  ---------------------------------
			
			(Imagen.Datos.begin () + (Get_PixelCount () * Get_Channels ())) ,		// Origen en R.
			(Imagen.Datos.begin () + (Get_PixelCount () * Get_Channels () * 2))	,	// Origen en G.
			(Imagen.Datos.begin () + (Get_PixelCount () * Get_Channels () * 3))		// Origen en B.
			
			// -----------------------------------------------------------------
			
			#endif
			
			) : const_iterator ());
		
	}
	
	/// @}
	
	// -----------------------  Presentar (Win32)  -----------------------------
	
	/// Limitaremos la presentación de una Ventana de Previsualización en el SO Windows.
	
#if defined (_WIN32) && defined (ELECTRODEMP_ENABLE_CIMG)
	
	/// @name Presentación.
	/// @brief Podemos realizar la previsualización de la Imagen de Caratula con un visualizador integrado en CImg.
	/// @{
	
	/// @brief Tenemos el inicador de la Ventana de Preview a presentar. Utilizamos esté metodo antes de llamar a @ref Show_Preview.
	/// @param Cover Caratula a presentar en la ventana , utilizaremos las medidas originales de la Imagen para crear el viewer.
	/// @warning Asegurate de llamar a @ref Close_Preview antes de iniciar un nuevo Visualizador.
	
	static void Init_Preview (const Caratula & Cover);
	
	/// @brief Nuestra caratula puede ser presentada en Pantalla utilizando una simple ventana de CImg que tiene incluido algunos elementos adicionales como
	/// los cursores para poder moverse y enfocar la imagen en determinados puntos. Usaremos entonces un metodo especifico para presentar la imagen actual.
	/// @param Cover Caratula a presentar en la ventana de previsualización.
	/// @param Asincrono Indica si nuestra ventana debe ser gestionada por un thread independiente o en el thread principal que la invoca.
	
	static void Show_Preview (const Caratula & Cover , bool Asincrono = true);
	
	/// @brief Tenemos el siguiente metodo que detiene la previsualización de la imagen de caratula desde el thread en caso de que esté se haya lanzado
	/// de manera asincrona.
	/// @note No tiene efecto si la imagen fue visualizada con el thread principal (sincrono).
	
	static void Close_Preview ();
	
	/// @}
	
#endif
	
	// -------------------------------------------------------------------------
	
	// Ahora vamos a implementar un metodo exclusivo para cuando tenemos habilitada CImg.
	
#if defined (ELECTRODEMP_ENABLE_CIMG)
	
	// -------------------------  Organizador CImg  ----------------------------
	
	/// @name Organizador de Pixeles
	/// @brief Está estructura cuenta con algunos metodo estaticos que permiten ajustar los pixeles RGB o BGR (OpenCV) a un Formato determinado y devolver una Caratula
	/// hecha con esté mismo arreglo de pixeles organizados.
	/// @{
	
	/// @brief Metodo para organizar los pixeles de Entrada en un array de pixeles legible para los pixeles de CImg.
	/// @tparam TypeInputPixel Tipo de los datos de entrada.
	/// @param Pixeles_Entrada Datos de pixel organizados nativamente o de la manera de OpenCV , es decir , entrelazados de la forma BGA :
	///	
	///	Pixeles : B0G0R0B1G1R1B2G2R2 ..... BNGNRN , con N igual al numero de Pixeles en orden de fila mayor.
	/// @param Salida Imagen de CImg de salida , se repartira memoria suficiente para contener todos los pixeles.
	/// @param Ancho Ancho (numero de pixeles a lo ancho) de la imagen de entrada.
	/// @param Alto Alto (numero de pixeles a lo alto) de la imagen de entrada.
	/// @param Canales Numero de Canales en la imagen de entrada (1 para MONOCROMATICO , 3 para RGB ó 4 para RGBA).
	/// @warning Pixeles_Entrada debe tener (Ancho * Alto * Canales) elementos en el array. En caso de ser necesario los valores de los
	/// pixeles serán recortados al tipo de pixel utilizado.
	
	template <typename TypeInputPixel>
	static void Organize_Pixels_ToCImg (CImg <CComponente> & Salida , const TypeInputPixel * Pixeles_Entrada , size_t Ancho , size_t Alto , size_t Canales = 3u);
	
	/// @}
	
#endif
	
	// -------------------------------------------------------------------------
	
	// Lo siguiente es para organizar pixeles de acuerdo a la matriz de opencv.
	
#if defined (ELECTRODEMP_ENABLE_OPENCV)
	
	// ------------------------  Organizador OpenCV ----------------------------
	
	/// @name Organizador de Pixeles para OpenCV mat.
	/// @brief Para poder organizar los pixeles en la imagen naturalmente requerimos copiar todos los valores de cada pixel de muestra en la matriz original.
	/// @{
	
	/// @brief Metodo para organizar los pixeles de Entrada a una matriz de pixeles de un tipo de dato especificado.
	/// @tparam TypeInputPixel Tipo de los datos de entrada.
	/// @param Pixeles_Entrada Datos de pixel organizados nativamente o de la manera de OpenCV , es decir , entrelazados de la forma BGA :
	///	
	///	Pixeles : B0G0R0B1G1R1B2G2R2 ..... BNGNRN , con N igual al numero de Pixeles en orden de fila mayor.
	/// @param Salida Imagen de CImg de salida , se repartira memoria suficiente para contener todos los pixeles.
	/// @param Ancho Ancho (numero de pixeles a lo ancho) de la imagen de entrada.
	/// @param Alto Alto (numero de pixeles a lo alto) de la imagen de entrada.
	/// @param Canales Numero de Canales en la imagen de entrada (1 para MONOCROMATICO , 3 para RGB ó 4 para RGBA).
	/// @warning Pixeles_Entrada debe tener (Ancho * Alto * Canales) elementos en el array. En caso de ser necesario los valores de los
	/// pixeles serán recortados al tipo de pixel utilizado.
	
	template <typename TypeInputPixel>
	static void Organize_Pixels_ToMat (cv::Mat_ <CComponente> & Salida , const TypeInputPixel * Pixeles_Entrada , size_t Ancho , size_t Alto , size_t Canales = 3u);
	
	/// @brief Metodo que permite la generación de una matriz de N canales, con las medidas de ancho y alto especificadas mediante la matriz de entrada unicanal
	/// esto con el fin de poder realizar operaciones sobre la matriz general.
	/// @param Entrada Matriz de entrada con los elementos de pixeles del tipo de componente especificado.
	/// @param Ancho Ancho de la imagen de entrada/salida.
	/// @param Alto Alto de la imagen de entrada/salida.
	/// @param Canales Número de canales que tendrá la imagen de salida multicanal de cv::Mat.
	/// @return Matriz Multicanal de OpenCV con los pixeles organizados para su procesamiento.
	
	static cv::Mat Convert_ToMat (const cv::Mat_ <CComponente> & Entrada , int Ancho_Imagen , int Alto_Imagen , int Canales_Imagen = 3u);
	
	/// @}
	
#endif
	
	// -------------------------------  Utilerías  -----------------------------
	
	// Tenemos un metodo estatico que permite obtener la versión escalada de la imagen de caratula indicada para propositos de presentar en algún
	// otro lienzo.
	
	static Caratula Get_Picture_Scaled (const Caratula & Imagen_Cover , size_t Nuevo_Ancho , size_t Nuevo_Alto);
	
};

// -----------------------------------------------------------------------------

// ------------------------  Caratula Definición -------------------------------

// -----------------------------------------------------------------------------

// Para el caso de CImg tenemos un metodo que nos ayuda a realizar la organización de los pixeles de acuerdo a esta biblioteca.

#if defined (ELECTRODEMP_ENABLE_CIMG)

// Ahora vamos a implementar el metodo para organizar un array de pixeles de entrada a la forma utilizada por CImg para el procesamiento de sus imagenes , devuelve
// una memoria nueva con los pixeles reorganizados.

template <typename TypeInputPixel>
void Caratula::Organize_Pixels_ToCImg (CImg <CComponente> & Salida , const TypeInputPixel * Pixeles_Entrada , size_t Ancho , size_t Alto , size_t Canales) {
	
	// Primero asignaremos el tamaño de nuestra imagen de salida con un total de 3 canales (RGB).
	
	Salida.assign (Ancho , Alto , 1 , 3);
	
	// -------------------------------------------------------------------------
	
	// Primero determinaremmos el total de pixeles en la imagen de entrada.
	
	const size_t Cantidad = (Ancho * Alto);
	
	// Ahora determinaremos el total de datos a repartir para nuestro array de salida. Multiplicamos la cantidad de pixeles por los canales de entrada.
	
	const size_t Cantidad_Elementos_Entrada = (Cantidad * Canales);
	
	// De igual forma tendremos la cantidad de datos en el array de salida con los tres canales a utilizar.
	
	const size_t Cantidad_Elementos_Salida = (Cantidad * 3);
	
	// Para poder colocar los valores en el formato RRR...GGG...BBB... usaremos tres iteradores que nos permitiran ir llenando las respectivas
	// partes del vector para cada canal , es decir que dividiremos nuestro array en tres secciones de (Ancho * Alto) para cada uno de los canales.
	
	CImg <CComponente>::iterator Iterador_R = Salida.begin (); 				// Comienza en el origen.
	CImg <CComponente>::iterator Iterador_G = (Iterador_R + Cantidad); 		// Offset igual al numero de pixeles
	CImg <CComponente>::iterator Iterador_B = (Iterador_G + Cantidad); 		// Offset igual al numero de pixeles * 2
	
	// Ahora que tenemos los iteradores para el vector , pasaremos a asignarles los valores desde los datos de la imagen en la memoria , este arreglo
	// tiene el formato estandar de valores : RGBRGBRGB , por lo que para cada indice , el color rojo se establece como datos(indice) , el color
	// verde como datos(indice + 1) y el azul como datos(indice + 2). Nuestros incrementos seran de 3 en 3 que es el numero de componentes
	// para el RGB asignado a la constante de "Imagen_Canales" y el total de datos en el arreglo original de datos es igual al numero de pixeles
	// por el numero de canales (Pixeles * 3).
	
	// De acuerdo al numero de Canales en nuestra Imagen de entrada validaremos como continuar.
	
	if (Canales == 1u) {
		
		// Realizaremos la iteración por los canales de Entrada y asignaremos a todos los canales de Salida : RGB el mismo valor.
		
		for (size_t Indice_Pixel = 0u ; Indice_Pixel < Cantidad_Elementos_Entrada ; ++ Indice_Pixel) {
			
			// Tomaremos la Componente de Color actual del pixel de entrada monocromatico. Convertimos al formato especifico si es necesario.
			
			const CComponente Componente_Actual = (std::is_same <CComponente , TypeInputPixel>::value ?
				Pixeles_Entrada [Indice_Pixel] : static_cast <CComponente> (Pixeles_Entrada [Indice_Pixel]));
			
			// Asignaremos cada componente de color segun lo establecido en su respectivo iterador y lo incrementaremos en una unidad
			// para que pase a la siguiente posición.
			
			*(Iterador_R ++) = Componente_Actual;	// Componente
			*(Iterador_G ++) = Componente_Actual; 	// Componente
			*(Iterador_B ++) = Componente_Actual; 	// Componente
			
		}
		
	}
	else if (Canales >= 3u) {
		
		// Por lo que ahora podemos asignar los valores para reorganizar los datos de la imagen y que CImg pueda trabajar con esto.
		
		for (size_t Indice_Entrada = 0u , Indice_Salida = 0u ;
			(Indice_Entrada < Cantidad_Elementos_Entrada) && (Indice_Salida < Cantidad_Elementos_Salida) ;
			Indice_Entrada += Canales , Indice_Salida += 3u) {
			
			// Asignaremos cada componente de color segun lo establecido en su respectivo iterador y lo incrementaremos en una unidad
			// para que pase a la siguiente posición. Recordemos que el Orden de Componentes en OpenCV es BGA.
			
			// Con OpenCV habilitado entonces invertiremos el orden en Red y Blue.
			
			#if defined (ELECTRODEMP_ENABLE_OPENCV)
			
			// ------------------------  OpenCV  -------------------------------
			
			*(Iterador_B ++) = (std::is_same <CComponente , TypeInputPixel>::value ?
				Pixeles_Entrada [Indice_Entrada]		: static_cast <CComponente> (Pixeles_Entrada [Indice_Entrada])); 		// Componente Azul (Red)
			*(Iterador_G ++) = (std::is_same <CComponente , TypeInputPixel>::value ?
				Pixeles_Entrada [Indice_Entrada + 1u] 	: static_cast <CComponente> (Pixeles_Entrada [Indice_Entrada + 1u])); 	// Componente Verde
			*(Iterador_R ++) = (std::is_same <CComponente , TypeInputPixel>::value ?
				Pixeles_Entrada [Indice_Entrada + 2u] 	: static_cast <CComponente> (Pixeles_Entrada [Indice_Entrada + 2u])); 	// Componente Rojo (Blue)
			
			// -----------------------------------------------------------------
			
			#else
			
			// -------------------------  Native  ------------------------------
			
			*(Iterador_R ++) = (std::is_same <CComponente , TypeInputPixel>::value ?
				Pixeles_Entrada [Indice_Entrada]		: static_cast <CComponente> (Pixeles_Entrada [Indice_Entrada])); 		// Componente Rojo
			*(Iterador_G ++) = (std::is_same <CComponente , TypeInputPixel>::value ?
				Pixeles_Entrada [Indice_Entrada + 1u] 	: static_cast <CComponente> (Pixeles_Entrada [Indice_Entrada + 1u])); 	// Componente Verde
			*(Iterador_B ++) = (std::is_same <CComponente , TypeInputPixel>::value ?
				Pixeles_Entrada [Indice_Entrada + 2u] 	: static_cast <CComponente> (Pixeles_Entrada [Indice_Entrada + 2u])); 	// Componente Azul
			
			// -----------------------------------------------------------------
			
			#endif
			
		}
		
	}
	
}

#endif // defined (ELECTRODEMP_ENABLE_CIMG)

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Para el caso de OpenCV tenemos la siguiente organización de pixeles.

#if defined (ELECTRODEMP_ENABLE_OPENCV)

// Ahora vamos a implementar el metodo para organizar un array de pixeles de entrada a la forma utilizada por CImg para el procesamiento de sus imagenes , devuelve
// una memoria nueva con los pixeles reorganizados.

template <typename TypeInputPixel>
void Caratula::Organize_Pixels_ToMat (cv::Mat_ <CComponente> & Salida , const TypeInputPixel * Pixeles_Entrada , size_t Ancho , size_t Alto , size_t Canales) {
	
	// Para empezar primero debemos establecer el tamaño fijo de columnas para la salida , está debera ser igual al numero de canales * ancho.
	
	const size_t Columnas = (Ancho * Canales);
	
	// Establecemos el tamaño de la matriz de pixeles para la salida (Filas , Columnas).
	
	Salida.create (Alto , Columnas);
	
	// -------------------------------------------------------------------------
	
	// Determinaremos el total de pixeles que tiene la imagen.
	
	const size_t Cantidad = (Ancho * Alto);
	
	// Ahora determinaremos el total de datos a repartir para nuestro array de salida. Multiplicamos la cantidad de pixeles por los canales de entrada.
	
	const size_t Cantidad_Elementos_Entrada = (Cantidad * Canales);
	
	// De igual forma tendremos la cantidad de datos en el array de salida con los tres canales a utilizar.
	
	const size_t Cantidad_Elementos_Salida = (Cantidad * Canales);
	
	// Para poder colocar los valores en el formato BGRBGRBGR.....BGR usaremos tres iteradores que nos permitiran ir llenando las respectivas
	// partes del vector para cada canal e irlos incrementando en la cantidad de canales.
	
	cv::Mat_ <CComponente>::iterator Iterador_B = Salida.begin (); 		// Comienza en el origen.
	cv::Mat_ <CComponente>::iterator Iterador_G = (Iterador_B + 1u); 		// Offset igual al 1
	cv::Mat_ <CComponente>::iterator Iterador_R = (Iterador_B + 2u); 		// Offset igual al 2
	
	// Ahora que tenemos los iteradores para el vector , pasaremos a asignarles los valores desde los datos de la imagen en la memoria , este arreglo
	// tiene el formato estandar de valores : RGBRGBRGB , por lo que para cada indice , el color rojo se establece como datos(indice) , el color
	// verde como datos(indice + 1) y el azul como datos(indice + 2). Nuestros incrementos seran de 3 en 3 que es el numero de componentes
	// para el RGB asignado a la constante de "Imagen_Canales" y el total de datos en el arreglo original de datos es igual al numero de pixeles
	// por el numero de canales (Pixeles * 3).
	
	// De acuerdo al numero de Canales en nuestra Imagen de entrada validaremos como continuar.
	
	if (Canales == 1u) {
		
		// Realizaremos la iteración por los canales de Entrada y asignaremos a todos los canales de Salida : RGB el mismo valor.
		
		for (size_t Indice_Pixel = 0u ; Indice_Pixel < Cantidad_Elementos_Entrada ; ++ Indice_Pixel) {
			
			// Tomaremos la Componente de Color actual del pixel de entrada monocromatico. Convertimos al formato especifico si es necesario.
			
			const CComponente Componente_Actual = (std::is_same <CComponente , TypeInputPixel>::value ?
				Pixeles_Entrada [Indice_Pixel] : static_cast <CComponente> (Pixeles_Entrada [Indice_Pixel]));
			
			// Asignaremos cada componente de color segun lo establecido en su respectivo iterador.
			
			(*Iterador_R) = Componente_Actual;	// Componente
			(*Iterador_G) = Componente_Actual; 	// Componente
			(*Iterador_B) = Componente_Actual; 	// Componente
			
			// Incrementaremos los iteradores en el numero de canales cada uno.
			
			Iterador_R = (Iterador_R + Canales);
			Iterador_G = (Iterador_G + Canales);
			Iterador_B = (Iterador_B + Canales);
			
		}
		
	}
	else if (Canales >= 3u) {
		
		// Por lo que ahora podemos asignar los valores para reorganizar los datos de la imagen y que CImg pueda trabajar con esto.
		
		for (size_t Indice_Entrada = 0u , Indice_Salida = 0u ;
			(Indice_Entrada < Cantidad_Elementos_Entrada) && (Indice_Salida < Cantidad_Elementos_Salida) ;
			Indice_Entrada += Canales , Indice_Salida += 3u) {
			
			// Asignaremos cada componente de color segun lo establecido en su respectivo iterador y lo incrementaremos en una unidad
			// para que pase a la siguiente posición. Recordemos que el Orden de Componentes en OpenCV es BGA.
			
			(*Iterador_B) = (std::is_same <CComponente , TypeInputPixel>::value ?
				Pixeles_Entrada [Indice_Entrada]		: static_cast <CComponente> (Pixeles_Entrada [Indice_Entrada])); 		// Componente Azul (Red)
			(*Iterador_G) = (std::is_same <CComponente , TypeInputPixel>::value ?
				Pixeles_Entrada [Indice_Entrada + 1u] 	: static_cast <CComponente> (Pixeles_Entrada [Indice_Entrada + 1u])); 	// Componente Verde
			(*Iterador_R) = (std::is_same <CComponente , TypeInputPixel>::value ?
				Pixeles_Entrada [Indice_Entrada + 2u] 	: static_cast <CComponente> (Pixeles_Entrada [Indice_Entrada + 2u])); 	// Componente Rojo (Blue)
			
			// Incrementaremos los iteradores en el numero de canales cada uno.
			
			Iterador_R = (Iterador_R + Canales);
			Iterador_G = (Iterador_G + Canales);
			Iterador_B = (Iterador_B + Canales);
			
		}
		
	}
	
}

#endif // defined (ELECTRODEMP_ENABLE_OPENCV)

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// -------------------------------  Etiqueta  ----------------------------------

/// @brief Ahora vamos a realizar la declaración de la Estructura Etiqueta la cual contiene todos los elementos y metadados de un archivo de audio
/// más importantes que se utilizan para presentar información adicional.
///
/// #Entre los elementos tenemos a :
/// @li <b> Titulo. </b>
///	@li <b> Artista. </b>
/// @li <b> Album. </b>
/// @li <b> Compositor. </b>
/// @li <b> Año del Album. </b>
/// @li <b> Numero de Pista. </b>
/// @li <b> Genero. </b>

struct Etiqueta {
	
	/// @brief Primer lugar tenemos el Filename de nuestro archivo de audio de referencia.
	
	std::string Filename;
	
	/// @brief El titulo de nuestro audio reproducido.
	
	std::string Title;
	
	/// @brief El Artista de nuestro album donde está el audio.
	
	std::string Artist;
	
	/// @brief El compositor del audio en caso de tenerlo.
	
	std::string Composer;
	
	/// @brief El nombre del Album.
	
	std::string Album;
	
	/// @brief El año de publicación del Album.
	
	std::string Year;
	
	/// @brief El numero de Pista en el Album.
	
	std::string Track;
	
	/// @brief El Generó de el Audio.
	
	std::string Genre;
	
	// ------------------------------  Fechas  ---------------------------------
	
	/// @brief Fecha de creación del audio original (dd/MM/YYYY).
	
	std::string Date_Original;
	
	/// @brief Fecha de modificación del audio (dd/MM/YYYY).
	
	std::string Date;
	
	// -------------------------------  Cover  ---------------------------------
	
	/// @brief Y finalmente la Caratula de nuestro Audio.
	
	Caratula Cover;
	
	// ---------------------------  Constructores  -----------------------------
	
	/// @name Constructores.
	/// @brief Contamos con algunos constructores por defecto para crear el Objeto de está estructura con datos por defecto.
	/// @{
	
	/// @brief Constructor por defecto , simplemente inicializa todos los campos en estado @b empty.
	
	Etiqueta () = default;
	
	/// @brief Constructor de Copia. Copia todos los datos de nuestro objeto de referencia.
	/// @param Copia Etiqueta a copiar.
	
	Etiqueta (const Etiqueta & Copia) = default;
	
	/// @brief Construcor de Movimiento. Traspasa todos los datos de la Etiqueta de entrada a está etiqueta.
	/// @param Mover Etiqueta a Mover.
	
	Etiqueta (Etiqueta && Mover) = default;
	
	/// @}
	
	/// @name Asignadores.
	/// @brief Asi mismmo contamos con las asignaciones por copia y movimiento por defecto.
	/// @{
	
	/// @brief Asignación por Copia. Devuelve al referencia a esté Objeto.
	/// @param Copia Etiqueta a copiar.
	/// @param Esté Objeto.
	
	Etiqueta & operator = (const Etiqueta & Copia) = default;
	
	/// @brief Asignación por Movimiento. Devuelve al referencia a esté Objeto.
	/// @param Mover Etiqueta a mover.
	/// @param Esté Objeto.
	
	Etiqueta & operator = (Etiqueta && Mover) = default;
	
	/// @}
	
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// -----------------------------  Etiquetador  ---------------------------------

// Ahora pasaremos a la declaración de la Clase principal para nuestro modulo. El etiquetador es la clase encargada de realizar la lectura y generación de Etiquetas
// a partir de los metadatos en los archivos de audio que tenemos. Permite abrir un archivo de audio , recuperar toda la información posible en la estructura
// Etiqueta , realizar cambios a la información y guardar los cambios en la Etiqueta de la song.

/// @brief Clase generadora de Etiquetas con información a partir de Archivos de Audio.
/// 
/// Está clase es la encargada de leer archivos de audio , obtener sus metadatos y recuperar la información en objetos de etiqueta.
/// Permite realizar cambios a la información y finalmente guardar los cambios en el archivo de audio original. La lista de formatos de audio soportados y los tipos
/// de Etiquetas se puede ver a continuación :
///	## Formatos Soportados
///		+ Audio MP3.
///		+ Audio OGG (Vorbis , Flac).
///		+ Audio FLAC.
///		+ Audio WAV.
///		+ Audio de Video MP4.
///
/// ## Etiquetas Soportadas.
///		+ Etiquetas ID3v1 , ID3v2

class Etiquetador {
	
	// Empezaremos con la declaración de todos los elementos privados para el Etiquetador.
	
	private :
		
		// ---------------------  Propiedades de Audio  ------------------------
		
		// En primer lugar tenemos el Filename del Archivo de Audio abierto recientemente , como referencia para realizar cambios sobre su etiqueta.
		
		std::string Filename;
		
		// Tenemos como siguiente elemento el super importante File , una interfaz generica que manipula todos los atributos , frames , propiedades , etc , de un
		// archivo de audio en particular. Tiene una subclase para cada tipo de archivo en especifico.
		
		std::shared_ptr <TagLib::File> Audio_File;
		
		// Tenemos ahora el elemento más interesante , el propio Tag de nuestro archivo de audio. Esta interfaz guarda la información más general de cada archivo
		// de audio. Dejando abierta la posibilidad de extender sus opciones mediante el uso de las subclases que derivan de la misma.
		
		TagLib::Tag * Audio_Tag = nullptr;
		
		// Asi mismo contendremos el Mapa de Propiedades más en detalle de nuestro Audio abierto actualmente. Con esté mapa tendremos acceso a muchos elementos
		// de información especifica sobre el audio de los cuales podemos sacar provecho.
		
		TagLib::PropertyMap Audio_Map;
		
		// El siguiente serán las Propiedades Genericas del Audio , está es una simple interfaz que nos proporciona información tecnica sobre el archivo de audio que
		// pueden utilizarse en conjunto con el Decodificador para determinar la manera de trabajar del Audio en cuestión.
		
		TagLib::AudioProperties * Audio_Properties = nullptr;
		
		// Ahora tenemos un elemento para determinar el estado actual de nuestro archivo. Si está abierto o cerrado de manera syncrona.
		
		std::atomic <bool> Opened;
	
	// Tenemos la declaración de todos los tipos de Formatos soportados , esto ayudara con la gestión especifica por cada tipo de archivo.
	
	public :
		
		// -------------------------  Formatos  --------------------------------
		
		/// @brief Enumeración de formatos de Audio soportados por el Etiquetador.
		
		enum Formato : short int {
			
			/// @brief Formato de Audio MP3 (*.mp3).
			
			F_MP3 = 1 ,
			
			/// @brief Formato de Audio OGG-Vorbis (*.ogg , *.oga).
			
			F_OGG ,
			
			/// @brief Formato de Audio OGG-Flac (*.oga , *.flac).
			
			F_FLAC ,
			
			/// @brief Formato de Audio WAV (*.wav).
			
			F_WAV ,
			
			/// @brief Formato de Audio para video MP4 (*.mp4).
			
			F_MP4
			
		};
		
		/// @brief Formato de audio asignado para el filename actual.
		
		std::atomic <short int> Formato_Actual;
	
	// Lo siguiente será la declaración formal de nuestros metodos publicos para la generación de Etiquetas en los metadatos de archivos de audio.
	
	public :
		
		// ------------------------  Constructor  ------------------------------
		
		/// @name Constructor.
		/// @brief Contamos con un simple constructor por defecto que inicializa todos los componentes de nuestras bibliotecas a utilizar y establece los
		/// estados iniciales para configurar de manera inicial.
		
		Etiquetador ();
		
		// ------------------------  Accesores  --------------------------------
		
		/// @name Accesores.
		/// @brief Algunos metodos getters para obtener información sobre nuestro archivo de audio que tenemos actualmente abierto.
		/// @{
		
		/// @brief Metodo para recuperar el Filename actualmente abierto en nuestro etiquetador.
		/// @return Cadena con el Filename actualmente abierto o una cadena vacia si no hay alguno abierto.
		
		ELECTRODEMP_FORCE_INLINE std::string Get_Filename () const {
			
			// Devolvemos el filename abierto actualmente.
			
			return Filename;
			
		}
		
		/// @}
		
		// --------------------------  Estado  ---------------------------------
		
		/// @brief Tenemos el metodo que nos determina el estado actual de nuestro Archivo de audio , si está abierto o cerrado.
		/// @return True si tenemos un archivo de audio abierto y podemos realizar acciones de Generate y Assign.
		
		ELECTRODEMP_FORCE_INLINE bool Is_Open () const {
			
			// Devolvemos el valor actual de nuestra localidad.
			
			return Opened.load ();
			
		}
		
		// -------------------------  Formato  ---------------------------------
		
		/// @name Formato del Audio.
		/// @brief Tenemos algunos metodos que indican de manera rapida si el formato de audio actualmente asignado es en alguno de los distintos formatos
		/// posibles.
		/// @{
		
		/// @brief Verifica si el archivo de audio abierto está en formato MP3.
		/// @return True si el archivo es de formato mp3.
		
		ELECTRODEMP_FORCE_INLINE bool Is_MP3 () const {
			
			// Devolvemos el valor de nuestro campo Formato comparandolo con el enum.
			
			return (Formato_Actual.load () == F_MP3);
			
		}
		
		/// @brief Verifica si el archivo de audio abierto está en formato OGG (Vorbis).
		/// @return True si el archivo es de formato ogg.
		
		ELECTRODEMP_FORCE_INLINE bool Is_OGG () const {
			
			// Devolvemos el valor de nuestro campo Formato comparandolo con el enum.
			
			return (Formato_Actual.load () == F_OGG);
			
		}
		
		/// @brief Verifica si el archivo de audio abierto está en formato FLAC.
		/// @return True si el archivo es de formato flac.
		
		ELECTRODEMP_FORCE_INLINE bool Is_FLAC () const {
			
			// Devolvemos el valor de nuestro campo Formato comparandolo con el enum.
			
			return (Formato_Actual.load () == F_FLAC);
			
		}
		
		/// @brief Verifica si el archivo de audio abierto está en formato WAV.
		/// @return True si el archivo es de formato wav.
		
		ELECTRODEMP_FORCE_INLINE bool Is_WAV () const {
			
			// Devolvemos el valor de nuestro campo Formato comparandolo con el enum.
			
			return (Formato_Actual.load () == F_WAV);
			
		}
		
		/// @brief Verifica si el archivo de audio abierto está en formato MP4.
		/// @return True si el archivo es de formato mp4.
		
		ELECTRODEMP_FORCE_INLINE bool Is_MP4 () const {
			
			// Devolvemos el valor de nuestro campo Formato comparandolo con el enum.
			
			return (Formato_Actual.load () == F_MP4);
			
		}
		
		/// @}
		
		// -------------------------  Acciones  --------------------------------
		
		/// @name Acciones del Etiquetador.
		/// @brief Tenemos ahora los metodos principales con los que el reproductor interactua directamente para poder realizar la apertura , generación del tag ,
		/// asignación de tag , cerrado de archivo , guardado de cambios , etc.
		/// @{
		
		/// @brief Metodo principal. Realiza la apertura de un archivo de Audio y automaticamente lee toda la información respectiva del Audio.
		///
		/// Se realiza toda la lectura de metadatos en el archivo de audio y se generán las estructuras de datos con la información proporcionada.
		/// @param Filename Archivo de Audio en cualquiera de los siguientes formatos soportados :
		///		+ Archivo MP3
		///		+ Archivo OGG (Vorbis , FLAC)
		///		+ Archivo MP4
		///		+ Archivo WAV
		///
		///	@note Los diferentes tipos de archivo contienen diferentes tipos de información , por lo que algunos formatos y tipos de etiqueta pueden o no presentar
		/// determinada información.
		/// @return True en caso de no haber error.
		
		bool Open (const std::string & Filename);
		
		/// @brief Tenemos el metodo para la generación de la Etiqueta a partir de los metadatos de nuestro archivo de audio previamente abierto.
		///
		/// Este metodo llena la estructura con toda la información encontrada en el archivo de audio , al final resulta una estructura Etiqueta con la información
		/// que se haya podido localizar.
		/// @return Etiqueta de datos del archivo de Audio.
		
		Etiqueta Generate ();
		
		/// @brief Tenemos ahora el metodo siguiente para poder realizar la asignación de propiedades a la Etiqueta actual de la song.
		///
		/// @warning Se modifican los valores establecidos en el archivo de audio actual y guardan los cambios en el tag , por lo que es necesario respaldar el
		/// tag anterior si se quiere recuperar a como estaba anteriormente.
		/// @param Datos Etiqueta con los datos a sobreescribir en el Tag del Audio actualmente abierto.
		
		void Assign (const Etiqueta & Datos);
		
		/// @brief Tenemos ahora un metodo para finalizar el trabajo con el archivo de audio actual y cerrar el filename establecido.
		///
		/// De esta forma se liberan los recursos que puedierán estar siendo ocupados.
		/// @return True en caso de no haber errores.
		
		bool Close ();
		
		/// @}
		
		// ---------------------------  Destructor  ----------------------------
		
		/// @brief Nos aseguramos de cerrar nuestro archivo de audio que pudiera estar abierto antes de destruir el objeto.
		
		virtual ~ Etiquetador ();
	
	// Ahora vamos a declarar todos los metodos auxiliares privados que utiliza el Etiquetador para realizar todas las acciones pertinentes.
	
	private :
		
		// -------------------------  Metadatos  -------------------------------
		
		// Tenemos a continuación un conjunto de metodos auxiliares para los metodos principales del Etiquetador cuyas funciones son las de obtener la información
		// respectiva de un archivo de audio para la generación de la etiqueta respectiva con los datos insertados.
		
		// El primer metodo es el que nos permite realizar la asignación de un tipo especifico de archivo de TagLib conforme la extensión que tenga el archivo ,
		// validaremos el tipo de extensión a partir del filename de entrada y devolveremos una instancia a File * desde la clase derivada para el tipo de archivo
		// de audio. Devolveremos null si no hay file apropiado.
		
		TagLib::File * File_Audio (const std::string & Filename);
		
		// Para poder realizar la busqueda de información en nuestro archivo de audio , tenemos tres formas diferentes de intentar encontrar la información :
		//		1 : Mediante los Frames de etiqueta ID3v2.
		//		2 : Mediante el Mapa de Propiedades de Audio.
		//		3 : Usando la interfaz de Tag.
		// El siguiente metodo tiene como finalidad encontrar la información de cadena usando cualquiera de los tres metodos anteriores para estó el metodo recive
		// tres elementos importantes : El key de Frame a buscar , El ID de Mapa a utilizar y el Metodo de propiedad de la Etiqueta Tag o null si no hay alguno.
		
		// Definiremos los tipos de Metodo a utilizar. Primero el metodo para recivir un String.
		
		using Metodo_String = TagLib::String (TagLib::Tag:: *) () const;
		
		// Ahora el tipo de metodo para recivir un entero sin signo.
		
		using Metodo_Integer = unsigned int (TagLib::Tag:: *) () const;
		
		// El metodo devuelve la cadena de busqueda en cualquiera de estos metodos o una cadena vacia.
		
		std::string Search_String (const char * const Frame_Key = nullptr , const char * const Map_ID = nullptr , Metodo_String Campo_S = nullptr , Metodo_Integer Campo_I = nullptr);
		
		// El siguiente metodo tiene como finalidad recupeestructurasrar el nombre o titulo de nuestro audio actualmente abierto a partir de sus propiedades , frames ó metadatos.
		// De no ser posible encontrarlo en estas  , usaremos el filename para determinarlo.
		
		std::string Title_Audio ();
		
		// De igual forma tenemos el metodo que recupera el nombre de nuestro Artista para el audio.
		
		std::string Artist_Audio ();
		
		// Metodo para recuperar el compositor del audio.
		
		std::string Composer_Audio ();
		
		// Metodo para recuperar el nombre del Album.
		
		std::string Album_Audio ();
		
		// Metodo para la recuperación de el año de publicación del Album.
		
		std::string Year_Audio ();
		
		// Metodo para reecuperar el numero de pista.
		
		std::string Track_Audio ();
		
		// Metodo para recuperar el genero de la canción.
		
		std::string Genre_Audio ();
		
		// Metodo para recuperar la fecha de creación (Original) de la canción.
		
		std::string Date_Original_Audio ();
		
		// Metodo para obtener la fecha de la ultima modificación de la song.
		
		std::string Date_Audio ();
		
		// Y el metodo para decodificar y recuperar la Imagen de caratula insertada (embebida) en el Archivo de Audio si es que tiene alguna.
		
		Caratula Cover_Audio ();
		
		// --------------------------  Frames  ---------------------------------
		
		// Las etiquetas ID3v2 contienen a diferencia de las demas , un mapa de Frames con información muy especifica sobre el archivo de audio , entre los datos
		// que encontramos más interesantes está la caratula de la song. Usaremos entonces un metodo especifico para recuperar la información de cadena de un
		// frame especifico.
		
		// Este primer metodo  devuelve la cadena que contiene determinado Frame por medio de su Key (string de 4 caracteres) , buscará alguna coincidencia o si no
		// devolvera una cadena vacia.
		
		std::string Get_Frame (const TagLib::ID3v2::Tag * ID3v2_Tag , const char * const ID3v2_Key) const;
		
		// -------------------------  Properties  ------------------------------
		
		// Ademas de contar con los Frames para el caso de ID3v2 , contamos tambien con nuestro propio mapa de propiedades que funciona de una manera similar ,
		// conteniendo listas de cadenas con los valores respectivos para determinado ID , TagLib define estos ID's y podemos usar el mapa para buscar la
		// información que requerimos.
		
		// Implementaremos otro metodo el cual realice la busqueda del ID especifico en el mapa de propiedades que indiquemos en el parametro. Devuelve la cadena
		// respectiva o una cadena vacia si no se encontro información.
		
		std::string Get_MapItem (const TagLib::PropertyMap & Mapa_Propiedades , const char * const Map_ID) const;
		
		// -------------------------  Picture  ---------------------------------
		
		// El siguiente metodo auxiliar permite realizar la decodificación y lectura de una Imagen en la etiqueta actualmente leida del audio. De esta forma
		// utilizaremos la biblioteca opencv y cimg para poder realizar la interpretación de los pixeles de la Imagen.
		
		// La entrada del metodo es propiamente la etiqueta de tipo Picture de la cual leeremos los datos de imagen para que puedan ser procesados y asignados
		// a una Caratula la cual devolveremos al terminar de asignar los pixeles y organizarlos de manera correcta.
		
		Caratula Get_Picture (const TagLib::ID3v2::AttachedPictureFrame * ID3v2_Picture) const;
	
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/// @}

} // namespace ElectrodeMP.

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ELECTRODEMP_ETIQUETADOR_H
