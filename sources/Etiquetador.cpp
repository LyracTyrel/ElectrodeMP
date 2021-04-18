
// Implementaremos ahora nuestro codigo para las clases y estructuras de nuestro Etiquetador.

#include <ElectrodeMP/Etiquetador.h>

/// @file Etiquetador.cpp
/// @brief Definición formal para nuestros metodos y propiedades de las clases del Etiquetador contenidas en este modulo.
/// @author Loubiner
/// @date Sunday 25/02/2018

// Utilizaremos el espacio de nombres del Proyecto.

using namespace ElectrodeMP;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------  Caratula  -----------------------------------

// -----------------------------------------------------------------------------

// ----------------------------  Constructores  --------------------------------

// Implementaremos el constructor de la Caratula con el parametro de nuestro array de pixeles en la imagen,

Caratula::Caratula (const CComponente * Pixeles_Imagen , size_t Ancho , size_t Alto , size_t Canales) {
	
	// Realizaremos la asignación e inicialización correcta de nuestra imagen interna realizando una reorganización de pixeles para el caso de tener
	// la biblioteca de Cimg.
	
	// Asignaremos las medidas de la imagen.
	
	Imagen_Ancho = Ancho , Imagen_Alto = Alto , Imagen_Canales = Canales;
	
	// En caso de CImg habilitado tenemos lo siguiente.
	
	// -------------------------------  CImg  ----------------------------------
	
	#if defined (ELECTRODEMP_ENABLE_CIMG)
	
	// Reorganizaremos los Pixeles de nuestra Imagen a la forma en que CImg define su estructura.
	
	Organize_Pixels_ToCImg (Imagen , Pixeles_Imagen , Ancho , Alto , Canales);
	
	// -------------------------------------------------------------------------
	
	// Si tenemos habilitado la biblioteca de OpenCV entonces tenemos el siguiente proceso.
	
	#elif defined (ELECTRODEMP_ENABLE_OPENCV)
	
	// ------------------------------  OpenCV  ---------------------------------
	
	// Reorganizaremos los pixeles a la manera en la que deben estar en OpenCV.
	
	Organize_Pixels_ToMat (Imagen , Pixeles_Imagen , Ancho , Alto , Canales);
	
	// -------------------------------------------------------------------------
	
	#else
	
	// ------------------------------  Native  ---------------------------------
	
	
	
	// -------------------------------------------------------------------------
	
	#endif
	
}

// -----------------------------------------------------------------------------

// ---------------------------  Create OpenCV  ---------------------------------

// Para el caso de OpenCV.

#if defined (ELECTRODEMP_ENABLE_OPENCV)

// Definiremos el constructor por referencia.

Caratula::Caratula (const cv::Mat & Imagen_Referencia) {
	
	// Primero asignaremos los datos de medida de la imagen.
	
	Imagen_Ancho = Imagen_Referencia.cols , Imagen_Alto = Imagen_Referencia.rows , Imagen_Canales = Imagen_Referencia.channels ();
	
	// Si tenemos CImg habilitado entonces.
	
	#if defined (ELECTRODEMP_ENABLE_CIMG)
	
	// ------------------------  Pixeles de CImg  ------------------------------
	
	// Obtenemos la referencia a los pixeles.
	
	const CComponente * const Pixeles_Imagen = Imagen_Referencia.ptr <CComponente> ();
	
	// Asignaremos una nueva caratula a nuestra imagen con los pixeles reorganizados mediante la siguiente función.
	
	Organize_Pixels_ToCImg (Imagen , Pixeles_Imagen , Imagen_Ancho , Imagen_Alto , Imagen_Canales);
	
	#else
	
	// -------------------------------------------------------------------------
	
	// Simplemente creamos una copia por referencia y conversión de la imagen de referencia.
	
	Imagen = Imagen_Referencia;
	
	// -------------------------------------------------------------------------
	
	#endif
	
}

#endif

// -----------------------------------------------------------------------------

// -----------------------------  Presentar  -----------------------------------

// Implementaremos los metodos para mostrar y ocultar la preview de la Caratula en el caso de Win32.

#if defined (_WIN32) && defined (ELECTRODEMP_ENABLE_CIMG)

// Crearemos ahora una ventana para poder presentar la Imagen en una interfaz interactiva hecha con la API nativa de win32.

static std::shared_ptr <CImgDisplay> Ventana_Preview = std::shared_ptr <CImgDisplay> (nullptr);

// Crearemos un thread el cual realizara la tarea de mostrar la imagen de manera asincrona y atender los eventos en la ventana.

static std::thread * Nucleo_Preview = nullptr;

// Ahora declaramos el estado actual de la Ventana , si está iniciada o desabilitada.

static std::atomic <bool> Estado_Preview;

// Definiremos el metodo Init para la ventana de Previsualización.

void Caratula::Init_Preview (const Caratula & Cover) {
	
	// Primero asignaremos una ventana con el tamaño de nuestra Imagen actual.
	
	Ventana_Preview.reset (new CImgDisplay (Cover.Get_Width () , Cover.Get_Height () , "Caratula (Cover) del Audio" , 0 , false , true));
	
	// Dejaremos el nucleo preview en estado de nullptr.
	
	Nucleo_Preview = nullptr;
	
	// Iniciaremos su estado en falso.
	
	Estado_Preview.store (false);
	
}

// Implementaremos el metodo para mostrar la Imagen actual , usaremos algunas utilerias de CImg para realizar este proceso.

void Caratula::Show_Preview (const Caratula & Cover , bool Asincrono) {
	
	// Validaremos que nuestra Imagen actual sea correcta y no estémos en estado de activo (previsualizado en thread secundario).
	
	if (Cover.Is_Valid () && !Estado_Preview) {
		
		// Mostramos el siguiente mensaje de aviso.
		
		LOG_N (" >> Visualizacion de Caratula Iniciada ")
		
		// Ahora validaremos la manera en la que ejecutaremos el metodo actual , si de forma asincrona o en el thread principal.
		
		if (Asincrono) {
			
			// -----------------------  Lanzando Thread  -----------------------
			
			// Si el thread actual es diferente de null , entonces vamos a eliminar el thread.
			
			if (Nucleo_Preview != nullptr) {
				
				// Esperamos a que se termine de ejecutar el thread.
				
				Nucleo_Preview->join ();
				
				// Eliminaremos el thread actual.
				
				delete (Nucleo_Preview);
				
			}
			
			// Colocaremos el estado en true.
			
			Estado_Preview.store (true);
			
			// Asignaremos un nuevo thread y lo lanzaremos de manera asincrona para que muestre la imagen en la ventana.
			
			Nucleo_Preview = new std::thread ([] (const CImagen & Imagen) -> void {
				
				// Crearemos una referencia a la Ventana creada para poder presentar la imagen.
				
				std::shared_ptr <CImgDisplay> Ventana_Preview_Actual (Ventana_Preview);
				
				// Crearemos una Imagen de reserva (referencia) por si la original sufre modificaciones o es eliminada.
				
				const CImagen Imagen_Preview (Imagen , true);
				
				// Mostraremos nuestra Imagen en la Ventana actual de manera interactiva hasta que se efectue la tecla ESP , se cierre la ventana o se invoque
				// el metodo Close_Preview.
				
				Imagen_Preview.display ((*Ventana_Preview_Actual) , true);
				
				// Si el estado es diferente a closed entonces lo cerraremmos.
				
				if (!Ventana_Preview_Actual->is_closed ()) {
					
					// Cerramos la ventana.
					
					Ventana_Preview_Actual->close ();
					
					// Lanzaremos los metodos pendientes.
					
					Ventana_Preview_Actual->flush ();
					
				}
				
				// Y ajustaremos el estado en falso.
				
				Estado_Preview.store (false);
				
				// Mostramos el mensaje siguiente.
				
				LOG_N (" >> Visualizacion de Caratula Terminada ")
				
			} , Cover.Imagen);
			
		}
		else {
			
			// Presentaremos en el thread actual la imagen , mostramos la ventana y esperamos un evento.
			
			Cover.Imagen.display ((*Ventana_Preview) , true);
			
			// Si el estado es diferente a closed entonces lo cerraremmos.
			
			if (!Ventana_Preview->is_closed ()) {
				
				// Cerramos la ventana.
				
				Ventana_Preview->close ();
				
				// Lanzaremos los metodos pendientes.
				
				Ventana_Preview->flush ();
				
			}
			
			// Ahora borraremos la ventana , asignando un valor vacio.
			
			Ventana_Preview->assign ();
			
			// Mostramos el mensaje siguiente.
			
			LOG_N (" >> Visualizacion de Caratula Terminada ")
			
		}
		
	}
	
}

// Ahora el metodo que cierra la ventana actualmente presentada , está ventana puede volver a mostrarse ocupando el metodo anterior.

void Caratula::Close_Preview () {
	
	// Validaremos que la ventana sea valida y el estado es igual a true.
	
	if (Ventana_Preview && Estado_Preview) {
		
		// Cerraremos la ventana actual y esperaremos a que el thread termine.
		
		Ventana_Preview->close ();
		
		// Efectuaremos la liberación de todos los eventos pendientes en la ventana.
		
		Ventana_Preview->flush ();
		
		// Si el thread es diferente a null.
		
		if (Nucleo_Preview != nullptr) {
			
			// Ahora vamos a esperar a que el thread termine su ejecución.
			
			Nucleo_Preview->join ();
			
			// Borraremos la memoria de nuestro thread de manera segura.
			
			delete (Nucleo_Preview);
			
			// Ahora asignamos null.
			
			Nucleo_Preview = nullptr;
			
		}
		
		// Ahora borraremos la ventana , asignando un valor vacio.
		
		Ventana_Preview->assign ();
		
		// El estado pasa a falso.
		
		Estado_Preview.store (false);
		
	}
	
}

#endif // defined (_WIN32) && defined (ELECTRODEMP_ENABLE_CIMG)

// -----------------------------------------------------------------------------

// -------------------------  Utilerías Definición  ----------------------------

// Vamos a realizar la definición de nuestro metodo para crear una caratula con un nuevo tamaño especificado a partir de la información de la caratula original.

Caratula Caratula::Get_Picture_Scaled (const Caratula & Imagen_Cover , size_t Nuevo_Ancho , size_t Nuevo_Alto) {
	
	// Tenemos la caratula de resultado vacía por el momento.
	
	Caratula Resultado;
	
	// Asignaremos la información de los tamaños al resultado.
	
	Resultado.Imagen_Ancho = static_cast <int> (Nuevo_Ancho) , Resultado.Imagen_Alto = static_cast <int> (Nuevo_Alto);
	
	// Primero que nada vamos a validar si la imágen de entrada es valida.
	
	if (Imagen_Cover.Is_Valid ()) {
		
		// Asignaremos la cantidad de canales de la imagen de origen.
		
		Resultado.Imagen_Canales = Imagen_Cover.Imagen_Canales;
		
		// Para el caso de tener CIMG habilitado tenemos el siguiente procedimiento.
		
		#if defined (ELECTRODEMP_ENABLE_CIMG)
		
		// ---------------------------------------------------------------------
		
		// Utilizaremos la función get_resized la cual recibe como información las nuevas medidas (ancho , alto , profundidad) , la cantidad de canales
		// y el metodo de interpolación el cuál será un valor entero de la siguiente manera:
		//  	-1 = no interpolation: raw memory resizing.
		//		0 = no interpolation: additional space is filled according to boundary_conditions.
		//		1 = nearest-neighbor interpolation.
		//		2 = moving average interpolation.
		//		3 = linear interpolation.
		//		4 = grid interpolation.
		//		5 = cubic interpolation.
		//		6 = lanczos interpolation.
		
		// Asignaremos el resultado de una interpolación lineal a nuestra Imagen en la caratula resultante.
		
		Resultado.Imagen = std::move (Imagen_Cover.Imagen.get_resize (Resultado.Imagen_Ancho , Resultado.Imagen_Alto , -100 , Resultado.Imagen_Canales , 3));
		
		// ---------------------------------------------------------------------
		
		// Si por otro lado tenemos OpenCV habilitado entonces procederemos con lo siguiente.
		
		#elif defined (ELECTRODEMP_ENABLE_OPENCV)
		
		// ---------------------------------------------------------------------
		
		// Para el caso de OpenCV tenemos una utilería en la biblioteca de imgproc para la redimensión de imágenes mediante algoritmos de procesamiento
		// los cuales pueden estar optimizados.
		
		// Para esto requerimos una matriz de pixeles de salida como referencia con el nuevo tamaño de pixeles resultante y el tipo de datos de la imágen
		// de origen utilizando una utileria de OpenCV. De la siguiente manera.
		
		cv::Mat Imagen_Resultado = cv::Mat (Resultado.Imagen_Alto , Resultado.Imagen_Ancho , CV_MAKETYPE (cv::traits::Depth <CComponente>::value , Resultado.Imagen_Canales));
		
		// Ahora el algoritmo resize para OpenCV el cual opera sobre la matriz de pixeles de la imagen de entrada , la imagen resultado , el nuevo tamaño
		// para la salida , posiciones de referencia (0 originalmente) y el metodo de interpolación el cual es un valor de la siguiente forma :
		//  	INTER_NEAREST 		: nearest neighbor interpolation
		//  	INTER_LINEAR 		: bilinear interpolation
		//  	INTER_CUBIC 		: bicubic interpolation
		//  	INTER_AREA 			: resampling using pixel area relation.
		//  	INTER_LANCZOS4 		: Lanczos interpolation over 8x8 neighborhood
		//  	INTER_MAX 			: mask for interpolation codes
		//  	WARP_FILL_OUTLIERS 	: flag, fills all of the destination image pixels.
		//  	WARP_INVERSE_MAP 	: flag, inverse transformation
		
		// Conversión de matriz interna.s
		
		// cv::Mat Imagen_Original (std::move (Convert_ToMat (Imagen_Cover.Imagen , Imagen_Cover.Imagen_Ancho , Imagen_Cover.Imagen_Alto ,  Imagen_Cover.Imagen_Canales)));
		
		// Aplicamos un reshape a la Imagen de entrada para ajustarla al numero de canales a presentar.
		
		cv::Mat Imagen_Original (Imagen_Cover.Imagen.reshape (Resultado.Imagen_Canales));
		
		// En este caso aplicremos una interpolación Lineal para mejores resultados.
		
		cv::resize (Imagen_Original , Imagen_Resultado , Imagen_Resultado.size () , 0 , 0 , cv::INTER_AREA);
		
		// Asignaremos la matriz resultante a nuestra imagen resultado.
		
		Resultado.Imagen = Imagen_Resultado;
		
		// Liberamos la matriz de pixeles asignada para disminuir su contador de referencia.
		
		Imagen_Resultado.release ();
		
		// ---------------------------------------------------------------------
		
		#endif
		
		// ---------------------------------------------------------------------
		
	}
	
	// Devolveremos la caratula resultante.
	
	return std::move (Resultado);
	
}

// -----------------------------------------------------------------------------

// Para el caso de OpenCV contamos con lo siguiente.

#if defined (ELECTRODEMP_ENABLE_OPENCV)

// -----------------------------------------------------------------------------

// Ahora definiremos el metodo para poder convertir la matriz de pixeles del tipo especifico a una matriz multicanal principal para su procesamiento.

cv::Mat Caratula::Convert_ToMat (const cv::Mat_ <CComponente> & Entrada , int Ancho_Imagen , int Alto_Imagen , int Canales_Imagen) {
	
	// Primero vamos a crear una matriz de salida para N canales de tipo especifico con el alto y ancho especificado.

	cv::Mat Matriz_Resultado (Alto_Imagen , Ancho_Imagen , CV_MAKETYPE (cv::traits::Depth <CComponente>::value , Canales_Imagen));
	
	// Obtendremos una referencia al primer elemento en la matriz para iterar sobre esta de la siguiente manera.
	
	CComponente * Pixeles_Resultado = Matriz_Resultado.ptr <CComponente> ();
	
	// De la misma manera obtenemos una referencia a los pixeles de entrada para iterar.
	
	const CComponente * Pixeles_Entrada = Entrada.ptr <CComponente> ();
	
	// -------------------------------------------------------------------------
	
	// Determinaremos el total de pixeles que tiene la imagen de salida/entrada.
	
	const size_t Cantidad = (Ancho_Imagen * Alto_Imagen);
	
	// Ahora determinaremos el total de datos a repartir para nuestro array de salida. Multiplicamos la cantidad de pixeles por los canales de entrada.
	
	const size_t Cantidad_Elementos_Entrada = (Cantidad * Canales_Imagen);
	
	// De igual forma tendremos la cantidad de datos en el array de salida con los canales a utilizar.
	
	const size_t Cantidad_Elementos_Salida = (Cantidad * Canales_Imagen);
	
	// Ahora que tenemos los iteradores para el vector , pasaremos a asignarles los valores desde los datos de la imagen en la memoria , este arreglo
	// tiene el formato estandar de valores : RGBRGBRGB , por lo que para cada indice , el color rojo se establece como datos(indice) , el color
	// verde como datos(indice + 1) y el azul como datos(indice + 2). Nuestros incrementos seran de 3 en 3 que es el numero de componentes
	// para el RGB asignado a la constante de "Imagen_Canales" y el total de datos en el arreglo original de datos es igual al numero de pixeles
	// por el numero de canales (Pixeles * 3).
	
	// Por lo que ahora podemos asignar los valores para reorganizar los datos de la imagen y que CImg pueda trabajar con esto.
	
	for (size_t Indice_Entrada = 0u , Indice_Salida = 0u ;
		(Indice_Entrada < Cantidad_Elementos_Entrada) && (Indice_Salida < Cantidad_Elementos_Salida) ;
		Indice_Entrada += Canales_Imagen , Indice_Salida += Canales_Imagen) {
		
		// Asignaremos cada componente de color segun lo establecido en su respectivo iterador y lo incrementaremos en una unidad
		// para que pase a la siguiente posición. Recordemos que el Orden de Componentes en OpenCV es BGA.
		
		Pixeles_Resultado [Indice_Salida] = Pixeles_Entrada [Indice_Entrada];				// Color B
		Pixeles_Resultado [Indice_Salida + 1u] = Pixeles_Entrada [Indice_Entrada + 1u];		// Color G
		Pixeles_Resultado [Indice_Salida + 2u] = Pixeles_Entrada [Indice_Entrada + 2u];		// Color R
		
	}
	
	// Devolvemos la referencia a nuestra matriz resultante
	
	return std::move (Matriz_Resultado);
	
}

#endif

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------  Etiquetador  ----------------------------------

// -----------------------------------------------------------------------------

// ----------------------------  Constructor  ----------------------------------

// Implementaremos en primer lugar el constructor que inicializa todos los componentes necesarios para el Etiquetador.

Etiquetador::Etiquetador () {
	
	// Comenzaremos colocando los valores por defecto para nuestros atributos atomic. Empezando por el estado actual del archivo en falso.
	
	Opened.store (false);
	
	// Y ahora vamos a colocar el formato de Audio en 0 de Indeterminado.
	
	Formato_Actual.store (0);
	
	// Mensaje de aviso de que fue creado el Etiquetador.
	
	LOG_N (" :::::::::::::::::::::  Etiquetador Creado  :::::::::::::::::::::::: \n")
	
}

// -----------------------------  Acciones  ------------------------------------

// Implementaremos en primer lugar nuestro metodó para realizar la apertura de un nuevo archivo de audio para poder obtener la información del mismo.

bool Etiquetador::Open (const std::string & Filename) {
	
	// ----------------------  Determinando Formato  ---------------------------
	
	// El primer paso antes de comenzar la extracción de los metadatos , es saber el formato del archivo con el que estamos trabajando , tenemos diferentes
	// tipos de Files para cada archivo soportado por el reproductor , asi que para determinar el formato a aplicar usaremos simplemente la extensión del archivo.
	
	// Obtenemos la ubicación del ultimo '.' en el filename,
	
	size_t Indice_Dot = Filename.find_last_of ('.');
	
	// Ahora vamos a declarar la extensión del archivo en un String de TagLib.
	
	const TagLib::String Extension (TagLib::String (Filename.substr (Indice_Dot + 1u).c_str ()).upper ());
	
	// Validaremos la extensión del archivo para poder asignar el formato adecuado.
	
	if (Extension == "MP3") {
		
		// Asignaremos la extensión MP3 y mostraremos un mensaje LOG.
		
		Formato_Actual.store (F_MP3); LOG (" >> Formato de Audio MP3 para Etiquetador ")
		
	}
	else if ((Extension == "OGG") || (Extension == "OGA")) {
		
		// Asignaremos la extensión OGG y mostraremos un mensaje LOG.
		
		Formato_Actual.store (F_OGG); LOG (" >> Formato de Audio OGG para Etiquetador ")
		
	}
	else if (Extension == "FLAC") {
		
		// Asignaremos la extensión FLAC y mostraremos un mensaje LOG.
		
		Formato_Actual.store (F_FLAC); LOG (" >> Formato de Audio FLAC para Etiquetador ")
		
	}
	else if (Extension == "WAV") {
		
		// Asignaremos la extensión WAV y mostraremos un mensaje LOG.
		
		Formato_Actual.store (F_WAV); LOG (" >> Formato de Audio WAV para Etiquetador ")
		
	}
	else if (Extension == "MP4") {
		
		// Asignaremos la extensión MP4 y mostraremos un mensaje LOG.
		
		Formato_Actual.store (F_MP4); LOG (" >> Formato de Audio en MP4 para Etiquetador ")
		
	}
	else {
		
		// No está soportado el formato , por lo que mostraremos lo siguiente.
		
		LOG_W (" >> Formato de Audio para Etiquetador No Soportado ")
		
		// Devolvemos falso.
		
		return false;
		
	}
	
	// Asignaremos el Filename a nuestro campo especifico.
	
	this->Filename = Filename;
	
	// --------------------------  Abriendo Archivo  ---------------------------
	
	// Pasaremos a asignar nuestro Audio_File respectivo de acuerdo al formato actualmente establecido , para estó tenemos el siguiente metodo que
	// realiza esté proceso. Asignaremos la referencia de File a nuestra propiedad actual adquiriendo la propiedad.
	
	Audio_File = std::move (std::shared_ptr <TagLib::File> (File_Audio (Filename)));
	
	// Ahora que tenemos el archivo generado y abierto pasaremos a comprobar el estado antes de continuar.
	
	if (!Audio_File) {
		
		// Mostraremos un mensaje de error LOG para poder notificar.
		
		LOG_E (" >> Error al Abrir Etiqueta de Archivo ")
		
		// Y devolvemos false.
		
		return false;
		
	}
	else if (!Audio_File->isValid ()) {
		
		// Mostraremos el siguiente mensaje LOG de error para poder presentar.
		
		LOG_E (" >> Error , Etiqueta de Archivo No Valida ")
		
		// Y devolvemos false.
		
		return false;
		
	}
	
	// Ahora pasaremos a retomar entonces los valores de Propiedades en los campos especficos. Comenzaremos por retomar la referencia al Tag actual en el
	// archivo de audio.
	
	Audio_Tag = Audio_File->tag ();
	
	// Ahora moveremos todas las propiedades del mapa de propiedades actualmente en el archivo de audio.
	
	Audio_Map = std::move (Audio_File->properties ());
	
	// Y para complementar la información obtendremos la referencia a las propiedades tecnicas del Audio.
	
	Audio_Properties = Audio_File->audioProperties ();
	
	// Asignaremos el estado de abierto para poder finalizar.
	
	Opened.store (true);
	
	// Presentaremos el siguiente mensaje LOG de aviso de que fue abierto el archivo correctamente.
	
	LOG (" >> Etiqueta de Archivo Abierta Correctamente ")
	
	// Finalmente devolvemos true.
	
	return true;
	
}

// Implementaremos el metodo para generar la Etiqueta con los datos de nuestro archivo de audio abierto , y con esto poder mostrarlos por cualquier
// lugar donde se desee.

Etiqueta Etiquetador::Generate () {
	
	// Para empezar validaremos que tengamos un archivo abierto y el File respectivo sea valido.
	
	if (Is_Open () && Audio_File->isValid ()) {
		
		// Crearemos ahora nuestra Etiqueta vacia por el momento para irla llenando poco a poco de manera secuencial con ayuda de los metodos auxiliares.
		
		Etiqueta Resultado;
		
		// ---------------------  Anexando Filename  ---------------------------
		
		// Colocaremos una copia del Filename de está song codificado en UTF-8.
		
		Resultado.Filename = Filename;
		
		// ----------------------  Anexando Titulo  ----------------------------
		
		// Primero estableceremos el titulo de nuestro audio , para estó tenemos que llamar al siguiente metodo y asignar el resultado al campo respectivo.
		
		Resultado.Title = std::move (Title_Audio ());
		
		// ----------------------  Anexando Artista  ---------------------------
		
		// Anexaremos el Artista de esté audio.
		
		Resultado.Artist = std::move (Artist_Audio ());
		
		// ----------------------  Anexando Compositor  ------------------------
		
		// Anexaremos el Compositor Oficial del Audio.
		
		Resultado.Composer = std::move (Composer_Audio ());
		
		// ----------------------  Anexando Album  -----------------------------
		
		// Anexaremos el Album del Disco al Resultado.
		
		Resultado.Album = std::move (Album_Audio ());
		
		// ----------------------  Anexando Año --------------------------------
		
		// El Año de publicación de esté archivo.
		
		Resultado.Year = std::move (Year_Audio ());
		
		// ----------------------  Anexando Pista ------------------------------
		
		// El Numero de pista de esté Audio.
		
		Resultado.Track = std::move (Track_Audio ());
		
		// ----------------------  Anexando Genero  ----------------------------
		
		// Anexaremos el Genero del Audio.
		
		Resultado.Genre = std::move (Genre_Audio ());
		
		// ------------------  Anexando Fecha Original  ------------------------
		
		// Anexaremos la fecha de creación del Audio.
		
		Resultado.Date_Original = std::move (Date_Original_Audio ());
		
		// ----------------------  Anexando Fecha  -----------------------------
		
		// Anexaremos la fecha de modificación del Audio.
		
		Resultado.Date = std::move (Date_Audio ());
		
		// ---------------------  Anexando Caratula  ---------------------------
		
		// Y anexaremos la Imagen de Caratula que tiene el Audio.
		
		Resultado.Cover = std::move (Cover_Audio ());
		
		// ------------------------  Finalizando  ------------------------------
		
		// Finalizamos imprimiendo el siguiente mensaje de aviso.
		
		LOG (" >> Etiqueta Generada Correctamente ")
		
		// Y devolvemos la etiqueta con todos los campos asignados.
		
		return std::move (Resultado);
		
	}
	
	// Mostramos el mensaje LOG de advertencia siguiente.
	
	LOG_W (" >> Error al Generar Etiqueta , No hay Archivo Abierto ")
	
	// Devolvemos una Etiqueta Vacia.
	
	return Etiqueta ();
	
}

// Definiremos el metodo para realizar la asignación de los datos de una Etiqueta a nuestros metadatos para el archivo de audio abierto actualmente.

void Etiquetador::Assign (const Etiqueta & Datos) {
	
	
	
}

// Definiremos el metodo para cerrar nuestro archivo de audio y con esto poder liberarlo para su uso en otras aplicaciones.

bool Etiquetador::Close () {
	
	// Resetearemos el archivo actualmente asignado y liberaremos todos los recursos utilizados.
	
	Audio_File.reset ();
	
	// Asignaremos el estado de cerrrado para poder terminar.
	
	Opened.store (false);
	
	// Presentaremos el siguiente mensaje LOG de aviso de que fue cerrado el archivo correctamente.
	
	LOG (" >> Etiqueta de Archivo Cerrada Correctamente ")
	
	// Y devolvemos true.
	
	return true;
	
}

// ------------------------------  Destructor  ---------------------------------

// Implementaremos el destructor de nuestra clase actual.

Etiquetador::~ Etiquetador () {
	
	// Si tenemos un archivo de audio abierto en esté momento , lo cerraremos para liberar los recursos.
	
	if (Is_Open ()) {
		
		// Cerramos el archivo de audio actual.
		
		Close ();
		
	}
	
	// Eliminando elementos ..............
	
}

// -----------------------------------------------------------------------------

// -------------------------------  Privados  ----------------------------------

// -----------------------------------------------------------------------------

// ------------------------------  Metadatos  ----------------------------------

// Implementaremos primero el metodo para realizar la obtención de nuestro file respectivo conforme el filename y su extensión. Abriremos el nuevo archivo y
// devolveremos la referencia al mismo o null si no hay un file valido.

TagLib::File * Etiquetador::File_Audio (const std::string & Filename) {
	
	// Para poder abrir y leer los metadatos de nuestro filename requerimos la construcción de un Objeto derivado de la clase File , estó con el fin de asignar
	// la etiqueta y los datos correctos de acuerdo al formato de entrada. Si tenemos asignado un formato actualmente entonces pasaremos a construir el objeto
	// en cuestión  y abrirlo.
	
	switch (Formato_Actual.load ()) {
		
		// Formato MP3.
		
		case F_MP3 :
			
			// Construiremos y abriremos un nuevo MPEG::File de esta manera.
			
			return dynamic_cast <TagLib::File *> (new TagLib::MPEG::File (Filename.c_str ()));
			break;
		
		// Formato OGG.
		
		case F_OGG :
			
			// Construiremos y abriremos un nuevo Ogg::Vorbis::File de esta manera.
			
			return dynamic_cast <TagLib::File *> (new TagLib::Ogg::Vorbis::File (Filename.c_str ()));
			break;
		
		// Formato FLAC.
		
		case F_FLAC :
			
			// Construiremos y abriremos un nuevo FLAC::File de esta manera.
			
			return dynamic_cast <TagLib::File *> (new TagLib::FLAC::File (Filename.c_str ()));
			break;
		
		// Formato WAV.
		
		case F_WAV :
			
			// Construiremos y abriremos un nuevo RIFF::WAV::File de esta manera.
			
			return dynamic_cast <TagLib::File *> (new TagLib::RIFF::WAV::File (Filename.c_str ()));
			break;
		
		// Formato MP4.
		
		case F_MP4 :
			
			// Construiremos y abriremos un nuevo MP4::File de esta manera.
			
			return dynamic_cast <TagLib::File *> (new TagLib::MP4::File (Filename.c_str ()));
			break;
		
	}
	
	// En caso de no tener un formato asignado , devolvemos null.
	
	return nullptr;
	
}

// Implementaremos el metodo para la busqueda de todas las cadenas utilizando los campos establecidos en la interfaz , los frames de las etiquetas ID3v2 o
// el mapa de propiedades general que tiene asignado.

std::string Etiquetador::Search_String (const char * const Frame_Key , const char * const Map_ID , Metodo_String Campo_S , Metodo_Integer Campo_I) {
	
	// -----------------------  Buscando Información  --------------------------
	
	// Vamos a realizar la busqueda intensiva de la información solicitada , usaremos un vector de elementos de tipo string donde iremos colocando los diferentes
	// strings que encontramos en la información del archivo de audio obtenida por TagLib , al final devolveremos la cadena que sea más conveniente.
	
	std::vector <std::string> Lista_Cadenas;
	
	// ------------------------  Buscando en Frames  ---------------------------
	
	// Buscaremos en primer lugar el String de Titulo en los Frames para Etiquetas ID3v2 , asi que validaremos primeró esta condición.
	
	if ((Frame_Key != nullptr) && Is_MP3 ()) {
		
		// Obtenemos la referencia en formato MP3 de nuestro Archivo Actual para que podamos acceder a los miembros especificos de esté formato.
		
		std::shared_ptr <TagLib::MPEG::File> Audio_MP3 = std::dynamic_pointer_cast <TagLib::MPEG::File> (Audio_File);
		
		// Validaremos si nuestro formato tiene Etiqueta ID3v2 , continuaremos en ese caso.
		
		if (Audio_MP3->hasID3v2Tag ()) {
			
			// En general contamos con varios tipos de Frames pero en este caso no nos especializaremos en algunos en particular sino unicamente usaremos
			// este mapa para acceder a los más habituales los cuales los encontraremos en la doc oficial del estandar.
			
			// Para este metodo obtendremos la información de los Frames siguientes (todos de la doc oficial). Cada uno conformado por 4 caracteres
			//		Pares de : Clave : Descripción.
			//		TIT2 : Title/songname/content description
			//		TALB : Album/Movie/Show title
			//		TCOM : Composer
			//		TMED : Media type
			//		TBPM : BPM (beats per minute)
			//		TRCK : Track number/Position in set
			
			// Usaremos nuestro Metodo especifico para obtener la cadena especifica en formato UTF-8 y la colocaremos en la lista de cadenas posibles.
			
			Lista_Cadenas.push_back (std::move (Get_Frame (Audio_MP3->ID3v2Tag () , Frame_Key)));
			
		}
		
	}
	
	// ------------------------  Buscando en el Mapa  --------------------------
	
	// Una vez que ya tehemos obtenido el string de etiqueta ID3v2 , pasaremos a buscar la cadena en el mapa de propiedades , de esta manera tendremos una
	// posibilidad aun mayor de encontrar la información que necesitamos.
	
	if (Map_ID != nullptr) {
		
		// Primero obtendremos el Map <Key , T> que define TagLib para las propiedades de este archivo donde :
		//		Key : Es un String que define la KeyWord para identificar al elemento actual del mapa.
		//		T : Es una lista de Strings (StringList) que tiene toda la información sobre este tag en especifico.
		
		// Buscaremos en el Mapa el primer string que corresponda al ID de Mapa indicado en el parametro establecido , anexaremos la cadena convertida a nuestra
		// lista de salida.
		
		Lista_Cadenas.push_back (std::move (Get_MapItem (Audio_Map , Map_ID)));
		
	}
	
	// -----------------------  Buscando en el Tag  ----------------------------
	
	// Ademas del FrameList y el Mapa de Propiedades , contamos con algunos metodos en nuestro Tag de Audio , usaremos esté tag para poder acceder a determinados
	// elementos de información que queremos obtener. Y para esto usaremos las referencias a metodos miembro de la clase Tag , de esta manera tenemos lo siguiente.
	
	if (Campo_S != nullptr) {
		
		// Obtendremos la Cadena respectiva aplicando el metodo indicado sobre nuestra referencia al Tag y lo convertiremos a utf-8 para poderlo colocar en la lista
		// de cadenas resultantes.
		
		Lista_Cadenas.push_back (std::move ((Audio_Tag->*Campo_S) ().to8Bit (true)));
		
	}
	else if (Campo_I != nullptr) {
		
		// Crearemos un buffer temporal para almacenar el valor numerico en cadena.
		
		char Cadena_Int [10] = { '\0' };
		
		// Recuperaremos el valor numerico que nos devuelve nuestro metodo y lo converetiremos a una cadena terminada en '\0' que pasaremos a convertir a std::string.
		
		#ifdef ELECTRODEMP_ENABLE_FUNC_TO_STRING
		
		Lista_Cadenas.push_back (std::move (std::to_string (static_cast <int> ((Audio_Tag->*Campo_I) ()))));
		
		#else
		
		Lista_Cadenas.push_back (std::move (std::string (itoa (static_cast <int> ((Audio_Tag->*Campo_I) ()) , Cadena_Int , 10))));
		
		#endif
		
	}
	
	// --------------------------  Buscando Cadena  ----------------------------
	
	// Por ultimo realizaremos la busqueda de la mejor cadena o simplemente la primera que no esté vacia en nuestro vector de strings , validamos que no esté vacia.
	
	if (!Lista_Cadenas.empty ()) {
		
		// Obtenemmos el iterador al primer elemento que cumpla la siguiente condición :
		//		No debe estar vacio.
		
		typename decltype (Lista_Cadenas)::iterator Elemento_Busqueda = std::find_if (Lista_Cadenas.begin () , Lista_Cadenas.end () ,
			[] (std::string & Cadena) -> bool {
			
			// Devolvemos true si la cadena no está vacia.
			
			return !Cadena.empty ();
			
		});
		
		// Si el iterador es diferente que el ultimo elemento entonces hemos encontrado la cadena.
		
		if (Elemento_Busqueda != Lista_Cadenas.end ()) {
			
			// Devolvemos la cadena que encontramos.
			
			return std::move (*Elemento_Busqueda);
			
		}
		
	}
	
	// De lo contrario devolvemos una cadena vacia.
	
	return std::string ();
	
}

// Implementaremos ahora el metodo que nos permite recuperar el nombre de nuestro audio actual.

std::string Etiquetador::Title_Audio () {
	
	// Obtendremos el Titulo del Audio utilizando los siguientes campos : Frame [TIT2] , ID [TITLE] , Campo [title].
	
	std::string Cadena_Titulo (std::move (Search_String ("TIT2" , "TITLE" , &TagLib::Tag::title)));
	
	// Si la cadena está vacia , entonces utilizaremos el filename de nuestro archivo para determinarlo.
	
	if (Cadena_Titulo.empty () && !Filename.empty ()) {
		
		// Recortaremos la cadena del Filename hasta el ultimo separador de archivo. Obtenemos la posición del ultimo simbolo separador.
		
		const size_t Indice_Inicio = Filename.find_last_of (ELECTRODEMP_PATH_SEPARATOR);
		
		// Ahora recortaremos la cadena desde la posición inicio + 1 , hasta el final de la misma.
		
		std::string Cadena_Titulo_Filename (Filename.substr ((Indice_Inicio != std::string::npos) ? (Indice_Inicio + 1u) : 0u));
		
		// Quitaremos la extensión de nuestra cadena recortando hasta el iterador al ultimo '.' en la cadena y devolvemos está cadena.
		
		return std::move (std::string (Cadena_Titulo_Filename.begin () , std::find (Cadena_Titulo_Filename.begin () , Cadena_Titulo_Filename.end () , '.')));
		
	}
	
	// Devolvemos la cadena encontrada en los metadatos.
	
	return std::move (Cadena_Titulo);
	
}

// Implementaremos el codigo para el metodo que realiza la extracción del nombre del artista con los datos actuales del audio.

std::string Etiquetador::Artist_Audio () {
	
	// Obtendremos la cadena de Artista de la misma manera : Frame [TPE1] , ID [ARTIST] , Campo (artist).
	
	return std::move (Search_String ("TPE1" , "ARTIST" , &TagLib::Tag::artist));
	
}

// Implementaremos el codigo para el metodo que realiza la extracción del compositor con los datos actuales del audio.

std::string Etiquetador::Composer_Audio () {
	
	// Obtendremos la cadena del Compositor : Frame [TCOM] , ID [COMPOSER] , Campo (null).
	
	return std::move (Search_String ("TCOM" , "COMPOSER"));
	
}

// Implementaremos el codigo para el metodo que realiza la extracción del album con los datos actuales del audio.

std::string Etiquetador::Album_Audio () {
	
	// Obtendremos la cadena del Album : Frame [TALB] , ID [ALBUM] , Campo (album).
	
	std::string Cadena_Album (std::move (Search_String ("TALB" , "ALBUM" , &TagLib::Tag::album)));
	
	// Si la cadena está vacia , entonces utilizaremos la carpeta donde está contenida nuestra song para asignarla.
	
	if (Cadena_Album.empty () && !Filename.empty ()) {
		
		// Recortaremos la cadena del Filename hasta el ultimo separador de archivo. Obtenemos la posición del ultimo simbolo separador.
		
		const size_t Indice_Inicio = Filename.find_last_of (ELECTRODEMP_PATH_SEPARATOR);
		
		// Ahora recortaremos la cadena desde la posición inicio + 1 , hasta el final de la misma.
		
		std::string Cadena_Titulo_Filename (Filename.substr ((Indice_Inicio != std::string::npos) ? (Indice_Inicio + 1u) : 0u));
		
		// Quitaremos la extensión de nuestra cadena recortando hasta el iterador al ultimo '.' en la cadena y devolvemos está cadena.
		
		return std::move (std::string (Cadena_Titulo_Filename.begin () , std::find (Cadena_Titulo_Filename.begin () , Cadena_Titulo_Filename.end () , '.')));
		
	}
	
	// Devolvemos la cadena encontrada en los metadatos.
	
	return std::move (Cadena_Album);
	
}

// Implementaremos el codigo para el metodo que realiza la extracción del año de publicación con los datos actuales del audio.

std::string Etiquetador::Year_Audio () {
	
	// Obtendremos la cadena del Año de Publicación : Frame [null] , ID [nullptr] , Campo (nullptr , year).
	
	return std::move (Search_String (nullptr , nullptr , nullptr , &TagLib::Tag::year));
	
}

// Implementaremos el codigo para el metodo que realiza la extracción del numero de pista con los datos actuales del audio.

std::string Etiquetador::Track_Audio () {
	
	// Obtendremos la cadena del Número de Pista : Frame [TRCK] , ID [TRACKNUMBER] , Campo (nullptr , track).
	
	return std::move (Search_String ("TRCK" , "TRACKNUMBER" , nullptr , &TagLib::Tag::track));
	
}

// Implementaremos el codigo para el metodo que realiza la extracción del genero del audio con los datos actuales del audio.

std::string Etiquetador::Genre_Audio () {
	
	// Obtendremos la cadena del Genero del Audio : Frame [null] , ID [GENRE] , Campo (genre).
	
	return std::move (Search_String (nullptr , "GENRE" , &TagLib::Tag::genre));
	
}

// Implementaremos ahora el metodo para recuperar la fecha de creación original del audio.

std::string Etiquetador::Date_Original_Audio () {
	
	// Obtendremos el valor de la fecha para el audio original utilizando : Frame [TDOR] , ID [ORIGINALDATE] , Campo (nullptr , nullptr)
	
	return std::move (Search_String ("TDOR" , "ORIGINALDATE"));
	
}

// Ahora para recuperar la info sobre la fecha de modificación del audio.

std::string Etiquetador::Date_Audio () {
	
	// Obtendremos el valor de la fecha para el audio utilizando : Frame [TDRL] , ID [DATE] , Campo (nullptr , nullptr)
	
	return std::move (Search_String ("TDRL" , "DATE"));
	
}

// Implementaremos el codigo para el metodo que realiza la extracción de la caratula de nuestro audio con los datos actuales del audio.

Caratula Etiquetador::Cover_Audio () {
	
	// ------------------------  Buscando en Frames  ---------------------------
	
	// Vamos a realizar la busqueda de nuestra portada o caratula para el album del disco en el audio actual. Para poderlo realizar antes debemos asegurarnos que
	// nuestro audio está en formato MP3 y ademas que tiene una etiqueta ID3v2 valida ya que de otro modo no presentá un cover embebido en el audio.
	
	// Determinaremos si nuestro audio actual está en formato MP3 para poder comenzar la busqueda.
	
	if (Is_MP3 ()) {
		
		// Obtenemos la referencia en formato MP3 de nuestro Archivo Actual para que podamos acceder a los miembros especificos de esté formato.
		
		std::shared_ptr <TagLib::MPEG::File> Audio_MP3 = std::dynamic_pointer_cast <TagLib::MPEG::File> (Audio_File);
		
		// Validaremos si nuestro formato tiene Etiqueta ID3v2 , continuaremos en ese caso.
		
		if (Audio_MP3->hasID3v2Tag ()) {
			
			// Ahora que sabemos que tiene esta etiqueta vamos a recuperar la referencia interna a la misma y almacenarla en la siguiente localidad.
			
			const TagLib::ID3v2::Tag * ID3v2_Tag = Audio_MP3->ID3v2Tag ();
			
			// Obtendremos entonces la referencia a dicho mapa de Frames con la siguiente llamada al metodo de acceso respectivo.
			
			const TagLib::ID3v2::FrameListMap & Mapa_Frames = ID3v2_Tag->frameListMap ();
			
			// ---------------  Decodificando y Presentando APIC  --------------
			
			// Ahora que ya tenemos el mapa de Listas de Frames asignado , pasaremos entonces a ubicar el Frame que tiene por ID APIC de Attached Picture.
			// Este frame tiene la información relacionada con una imagen embebida en el Tag de nuestro audio para utilizarla como cubierta del CD de la
			// parte frontal o trasera. De acuerdo a este estandar en ID3v2 oficial.
			
			// Entonces validaremos que nuestro mapa contenga el frame siguiente : Frame [APIC].
			
			if (Mapa_Frames.contains ("APIC")) {
				
				// Ahora que determinamos que efectivamente , el mapa tiene la clave indicada pasaremos a retomar el valor de dicho mapa , una lista de Frames donde
				// cada uno establece información del mismo tipo.
				
				const TagLib::ID3v2::FrameList & Lista_Frames = Mapa_Frames ["APIC"];
				
				// Para continuar validaremos que la lista de Frames no se encuentre vacia.
				
				if (!Lista_Frames.isEmpty ()) {
					
					// Vamos a decodificar y procesar la Imagen , por lo que convertiremmos el primer Frame en un Tipo Picture Frame para que podamos acceder
					// a su información. Devolvemos el resultado obtenido.
					
					return std::move (Get_Picture (dynamic_cast <const TagLib::ID3v2::AttachedPictureFrame *> (Lista_Frames.front ())));
					
				}
				
			}
			
		}
		
	}
	
	// EN caso de no contener una caratula valida , devolvemos una Vacia.
	
	return Caratula ();
	
}

// -----------------------------------------------------------------------------

// -------------------------------  Frames  ------------------------------------

// Definiremos el metodo para acceder a la información contenida en una Etiqueta ID3v2 mediante la lista de Frames actuales. Validaremos que el mapa contenga
// el elemento principal a buscar.

std::string Etiquetador::Get_Frame (const TagLib::ID3v2::Tag * ID3v2_Tag , const char * const ID3v2_Key) const {
	
	// Obtendremos la lista de Frames que tiene nuestro Tag actual , de esta manera tenemos la siguiente referencia.
	
	const TagLib::ID3v2::FrameListMap & Mapa_Frames = ID3v2_Tag->frameListMap ();
	
	// Ahora vamos a validar que el mapa contenga la clave especificada en el parametro antes de continuar.
	
	if (Mapa_Frames.contains (ID3v2_Key)) {
		
		// Ahora que determinamos que efectivamente , el mapa tiene la clave indicada pasaremos a retomar el valor de dicho mapa , una lista de Frames donde
		// cada uno establece información del mismo tipo.
		
		const TagLib::ID3v2::FrameList & Lista_Frames = Mapa_Frames [ID3v2_Key];
		
		// Para terminar validaremos que la lista de Frames no se encuentre vacia.
		
		if (!Lista_Frames.isEmpty ()) {
			
			// Entonces tomaremos el primer elemento en la lista y lo convertiremos en un std::string codificada con UTF-8. Devolvemos el resultado.
			
			return std::move (Lista_Frames.front ()->toString ().to8Bit (true));
			
		}
		
	}
	
	// De lo contrario vamos a devolver la cadena vacia.
	
	return std::string ();
	
}

// -----------------------------------------------------------------------------

// -----------------------------  Properties  ----------------------------------

// Definiremos el metodo para obtener el valor de string de un ID en el Mapa de Propiedades del archivo de audio actual. Devolvemos una cadena vacia si
// no hay una cadena que corresponda al id indicado.

std::string Etiquetador::Get_MapItem (const TagLib::PropertyMap & Mapa_Propiedades , const char * const Map_ID) const {
	
	// Validaremos en primer lugar que nuestro mapa de propiedades contenga la clave ID indicada en el parametro antes de continuar.
	
	if (Mapa_Propiedades.contains (Map_ID)) {
		
		// Entonces vamos a obtener el StringList que corresponde a nuestra clave especificada , de esta forma tenemos la referencia a esta lista.
		
		const TagLib::StringList & Lista_Propiedades = Mapa_Propiedades [Map_ID];
		
		// Validaremos que la lista no se encuentre vacia para poder tomar el elemento que necesitamos.
		
		if (!Lista_Propiedades.isEmpty ()) {
			
			// Tomaremos el primer elemento de la lista y devolveremos su representación en cadena UTF-8 de c++.
			
			return std::move (Lista_Propiedades.front ().to8Bit (true));
			
		}
		
	}
	
	// De lo contrario vamos a devolver la cadena vacia.
	
	return std::string ();
	
}

// ------------------------------  Picture  ------------------------------------

// Implmentaremos el metodo para la decodificación y organización de los pixeles en la Imagen embebida en la etiqueta actual. Devolveremos la caratula con los
// pixeles de la imagen procesados y organizados o una caratula vacia en caso de haber errores.

Caratula Etiquetador::Get_Picture (const TagLib::ID3v2::AttachedPictureFrame * ID3v2_Picture) const {
	
	// La clase AttachedPictureFrame cuenta con varios metodos especializados para conocer la información sobre la Imagen actual tal como su codificación ,
	// tipo de imagen , MIME type , etc. Pero en particular tenemos el metodo picture el cual nos devuelve un ByteVector con el array de datos crudos o binarios
	// de la Imagen embebida en el Tag del Audio.
	
	// No podemos simplemente mostrar la imagen embebida asi como asi , tenemos antes que decodificarla usando nuestra biblioteca de opencv y luego ajustar los
	// pixeles para que su alineamiento sea deacuerdo a lo establecido en la biblioteca CImg , finalmente se crea la instancia de CImg con el formato de pixel
	// establecido en las configuraciones del programa (unsigned char).
	
	// Ahora vamos a retomar el ByteVector que contiene los datos de la Imagen a decodificar.
	
	const ByteVector Datos_Imagen = ID3v2_Picture->picture ();
	
	// Las medidas de la imagen estarán en nuestras variables siguientes.
	
	size_t Ancho_Imagen = 0u , Alto_Imagen = 0u , Canales_Imagen = 0u;
	
	// Los pixeles de la imagen decodificados serán estos :
	
	const Caratula::CComponente * Datos_Pixeles = nullptr;
	
	// -------------------------------------------------------------------------
	
	// Tenemos la decodificación ofrecida por OpenCV.
	
	#if defined (ELECTRODEMP_ENABLE_OPENCV)
	
	// --------------------  Decodificando OpenCV ------------------------------
	
	// Para decodificar la Imagen utilizaremos la biblioteca de OpenCV , misma que nos provee de muchos metodos para el procesamiento de imagenes y algoritmos
	// para realizar cambios en la misma. Usaremos el modulo de imcodecs para poder decodificar los datos de la imagen que tenemos actualmente asignada y
	// recuperaremos la Matrix de pixeles que nos devuelve.
	
	// Para realizar la decodificación de la Imagen usaremos el metodo cv::imdecode (InputArray , Flags) donde usaremos el input array como el array de datos
	// siguientes que tenemos.
	
	const cv::_InputArray Datos (Datos_Imagen.data () , Datos_Imagen.size ());
	
	// Decodificaremos esta información de la Imagen y obtendremos la matriz Mat de opencv con los pixeles de la Imagen decodificada. Usaremos la flag siguiente
	// para poder realizar la decodificación :
	//		Flag : cv::ImreadModes::IMREAD_UNCHANGED
	
	cv::Mat Imagen = cv::imdecode (Datos , cv::ImreadModes::IMREAD_UNCHANGED);
	
	// Obtenemos entonces las Medidas de Nuestra Imagen leida usando los campos y metodos de la clase Mat. Determinaremos el Ancho , Alto y Canales de la
	// Imagen de entrada.
	
	Ancho_Imagen = Imagen.cols , Alto_Imagen =  Imagen.rows , Canales_Imagen = Imagen.channels ();
	
	// LOG_F (" Size Decode = Ancho : %i , Alto : %i , Canales : %i , Tipo : %i " , Ancho_Imagen , Alto_Imagen , Canales_Imagen , Imagen.type ());
	
	// Asignaremos los datos de pixel decodificados.
	
	Datos_Pixeles = Imagen.ptr <Caratula::CComponente> ();
	
	// -------------------------------------------------------------------------
	
	#endif
	
	// ---------------------------  Creando Cover  -----------------------------
	
	// -------------------------------------------------------------------------
	
	// Validamos que la imagen fue decodificada correctamente mirando el array interno en la Matriz obtenida.
	
	if (Datos_Pixeles != nullptr) {
		
		// Para el caso de CImg tenemos que realizar una reorganización de pixeles de esta manera.
		
		#if defined (ELECTRODEMP_ENABLE_CIMG)
		
		// ---------------------  Reorganizando Pixeles  -----------------------
		
		// La siguiente parte una vez que la matrix de pixeles fue llenada por completo , tenemos que organizar los pixeles en una forma planar donde cada canal
		// esté completamente separado uno de otro , por lo que el acceso a cada componente de color será independiente.
		
		// Una vez hecho estó podemos entonces reorganizar todos los pixeles en la data interna de la Imagen , para estó usaremos el metodo estatico de la
		// Caratula que aplica ael algoritmo para reorganizar todos los pixeles devolviendo a la salida un array de pixeles nuevo para construir la caratula.
		
		// Pasaremos al metodo el ptr de datos reinterpretado conforme el Tipo de Componente y las medidas de la imagen.
		
		Caratula Imagen_Cover (Datos_Pixeles , Ancho_Imagen , Alto_Imagen , Canales_Imagen);
		
		// ---------------------------------------------------------------------
		
		#elif defined (ELECTRODEMP_ENABLE_OPENCV)
		
		// ---------------------  Copiando Referencia  -------------------------
		
		// Si es el caso de OpenCV entonces solamente copiaremos la referencia de la matriz de pixeles ya asignada.
		
		Caratula Imagen_Cover (Imagen);
		
		#else
		
		// ---------------------------------------------------------------------
		
		// Decodificación nativa de imagen .....
		
		Caratula Imagen_Cover;
		
		// ---------------------------------------------------------------------
		
		#endif
		
		// -------------------  Liberación de Memoria  -------------------------
		
		// Parael caso de OpenCV podemos liberar la memoria utilizada de la siguiente manera.
		
		#if defined (ELECTRODEMP_ENABLE_OPENCV)
		
		// Liberamos memoria.
		
		//Imagen.release ();
		
		#endif
		
		// ---------------------------------------------------------------------
		
		// ---------------------------------------------------------------------
		
		// Y finalmente devolvemos la nueva Caratula creada con el array de Pixeles generado y las medidas de la Imagen.
		
		return std::move (Imagen_Cover);
		
	}
	
	// Devolvemos en todo caso una caratula vacia.
	
	return Caratula ();
	
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------
