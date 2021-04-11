
# Modulo para la busqueda y generación de headers y dependencias del Proyecto ElectrodeMP.

# ------------------------------------------------------------------------------

# ------------------------------  Modulos  -------------------------------------

# Incluiremos el siguiente modulo para la comprobación de paquetes.

include (ElectrodeMPValidation)

# ----------------------------  Bibliotecas  -----------------------------------

# Ahora vamos a declarar una función que se encargue de buscar y establecer la lista de Headers asi como la lista de
# libraries que tiene que contener ElectrodeMP para su funcionamiento.

macro (ElectrodeMPDependences)
	
	# -------------------------  Dependencias  ---------------------------------
	
	# Empezaremos por las dependencias más importantes , las cuales son : PortAudio , FFmpeg y TagLib. Buscaremos los
	# archivos de cabecera para cada una asi como los archivos de biblioteca.
	
	# --------------------------------------------------------------------------

	# --------------------------  Port Audio  ----------------------------------

	# Comenzaremos por incluir nuestros elementos para la biblioteca de Port Audio como parte principal para el streaming
	# dentro de electrodemp.

	# Buscaremos el paquete en el sistema.

	find_package (Portaudio REQUIRED)

	# --------------------------------------------------------------------------

	# ---------------------------  FFmpeg  -------------------------------------

	# Para poder decodificar nuestros audios de diferentes formatos tenemos a la biblioteca FFmpeg la cual se
	# encarga de la parte de leer y decodificar un audio especifico. Usaremos para esto sus actuales bibliotecas
	# de formatos : libavformat , libavcodec y libavutil.

	# Buscaremos todas estas bibliotecas usando nuestro buscador.

	find_package (FFmpeg REQUIRED)

	# --------------------------------------------------------------------------

	# ---------------------------  TabLib  -------------------------------------

	# En este proyecto realizaremos la obtención de los metadatos y tag de una song en formatos ID3v2 ,
	# por lo que requerimos de la biblioteca TagLib la cual provee un conjunto muy completo de clases para poder
	# recuperar toda la información posible de una song.

	# Buscaremos el paquete de la biblioteca.

	find_package (Taglib REQUIRED)
	
	# Anexaremos el siguiente flag para el uso estatico de la biblioteca si es el caso.
	
	if (TAGLIB_STATIC)
		
		# Anexaremos la definición siguiente al proyecto.
		
		add_definitions (-DTAGLIB_STATIC)
		
	endif ()
	
	# --------------------------  Adicionales  ---------------------------------
	
	# Contenemos adicionalmente algunas dependencias para modificar la manera de trabajar de ElectrodeMP , entre
	# estás podemos mencionar a :
	#		Aquila : 	Biblioteca para el Procesamiento de Audio (proximamente).
	#		CImg : 		Biblioteca para el procesamiento digital de imagenes y visualización de las mismas. (C++).
	#		OpenCV : 	Biblioteca para procesamiento digital de imagenes digitales , video entre otros. (Alto rendmiento).
	#		TermColor : Biblioteca para gestionar colores de consola en multiples plataformas con una simple interfaz.
	
	# --------------------------------------------------------------------------

	# ----------------------------  Aquila  ------------------------------------

	# //////////////////////////////////////////////////////////////////////////
	# //////////////////////////////////////////////////////////////////////////

	# Biblioteca de procesamiento de audio para proximas actualizaciones.
	
	# Ahora vamos a incluir nuestra biblioteca de Aquila para poder realizar la generación de tonos de manera
	# simple usando funciones de c++.

	# Buscaremos el paquete de Aquila.

	# find_package (Aquila)
	
	# Validaremos el siguiente paquete.
	
	# ElectrodeMPValidation (Aquila_FOUND AQUILA_INCLUDE_DIR AQUILA_LIBRARIES)
	
	# //////////////////////////////////////////////////////////////////////////
	# //////////////////////////////////////////////////////////////////////////

	# --------------------------------------------------------------------------

	# ----------------------------  CImg  --------------------------------------

	# Para poder presentar la portada o caratula de song en una ventana especifica requerimos de la biblioteca
	# CImg la cual nos provee de algunos metodos para la representación de la imagen especificada.

	# Buscaremos el paquete para CImg.

	find_package (CImg)
	
	# Si el paquete fue encontrado entonces.
	
	if (CImg_FOUND)
		
		# Configuramos el siguiente flag para poder establecer la opción.
		
		set (ELECTRODEMP_ENABLE_CIMG TRUE)
		
	endif ()
	
	# --------------------------------------------------------------------------

	# ---------------------------  OpenCV  -------------------------------------

	# Ahora incluiremos la biblioteca de procesamiento de Imagen OpenCV la cual nos ofrece una interfaz simple
	# y amigable para la carga de Imagenes y procesamiento de las mismas. Usaremos esta para dar lugar a la
	# caratula y algunos procesamientos menores en las imagenes de cada song.
	
	# Buscaremos el paquete de OpenCV.

	find_package (OpenCV)
	
	# Si el paquete fue encontrado entonces.
	
	if (OpenCV_FOUND)
		
		# Configuramos el siguiente flag para poder establecer la opción.
		
		set (ELECTRODEMP_ENABLE_OPENCV TRUE)
		
	endif ()

	# --------------------------------------------------------------------------

	# ---------------------------  Term Color  ---------------------------------

	# Para decorar nuestra consola con colores especificos tenemos la biblioteca de term color la cual nos permite
	# rapidamente realizar la asignación de colores a nuestro texto en la terminal de consola (Opcional).

	# Pero antes validamos si la opción de LOG por consola está habilitada.

	if (ELECTRODEMP_ENABLE_CONSOLE_LOG)
		
		# Establecemos una Opción para utilizar la consola de color o no.
		
		option (TERMCOLOR_ENABLE "Habilita la consola de LOG de Colores utilizando TermColor (Opcional)" OFF)
		
		# Buscaremos la ruta donde se encuentra el archivo : termcolor/termcolor.hpp
		
		find_path (TERMCOLOR_INCLUDE_DIR termcolor/termcolor.hpp)
		
		# Si la encontramos o el usuario la localizó , pasaremos a incluirla al proyecto.
		
		if (TERMCOLOR_ENABLE AND NOT ("${TERMCOLOR_INCLUDE_DIR}" STREQUAL "TERMCOLOR_INCLUDE_DIR-NOTFOUND"))
			
			# Habilitaremos la siguiente opción
			
			set (ELECTRODEMP_ENABLE_TERMCOLOR TRUE)
			
		endif ()
		
	endif ()
	
	# -------------------------------  Outputs  --------------------------------
	
	# Declaramos la siguiente localidad para guardar todas las cabeceras que se han encontrado conforme el cmake se fue
	# configurando. Anexaremos la cabecera principal del proyecto como primera entrada.
	# Anexamos los directorios de include para formatos MP3 , WAV , MP4 , OGG y FLAC.
	# Incluiremos los directorios siguientes para los modulos a ocupar.
	
	set (ELECTRODEMP_HEADERS
		
		"${CMAKE_SOURCE_DIR}/include"
		
		"${PORTAUDIO_INCLUDE_DIR}" "${FFMPEG_INCLUDE_DIR}" "${TAGLIB_INCLUDE_DIRS}"
		"${CIMG_INCLUDE_DIR}" "${OPENCV_INCLUDE_DIRS}"
		"${TERMCOLOR_INCLUDE_DIR}"
		
	)
	
	# Declaramos la localidad para guardar todas las bibliotecas y dependencias de terceros que requiere nuestra biblioteca
	# para su compilación.
	
	set (ELECTRODEMP_LIBRARY_DEPENDENCES
		
		"${PORTAUDIO_LIBRARY}" "${FFMPEG_LIBRARIES}" "${TAGLIB_LIBRARY}"
		"${OPENCV_LIBRARIES}"
		
		"${ELECTRODEMP_SYSTEM_DEPENDENCES}"
		
	)
	
	# --------------------------------------------------------------------------
	
	# -------------------------  Media Player  ---------------------------------
	
	# Tenemos ahora las dependencias para el media player que puede compilarse tambien junto a la biblioteca. Esté programa está basado
	# en wxWidgets para su diseño de GUI asi como de algunas otras dependencias de sistema.
	
	# --------------------------------------------------------------------------
	
	# Ahora vamos con la compilación y generación de la aplicación , buscaremos las dependencias adicionales si la opción está marcada.
	
	if (ELECTRODEMP_BUILD_MEDIA_PLAYER)
		
		# ----------------------------------------------------------------------
		
		# --------------------------  wxWidgets  -------------------------------
		
		# Ahora vamos con la biblioteca de GUI multiplataforma wxWidgets , para esto la opción de compilar el ejecutable debe estar
		# asignada.
		
		# Buscaremos entonces el paquete para wxWidgets requiriendo como componentes el nucleo y la base.
		
		find_package (wxWidgets REQUIRED core base)
		
		# Tenemos como una opción utilizar versión unicode del codigo wxWidgetes.
		
		option (wxWidgets_USE_UNICODE "Utilizar la versión unicode de biblioteca" ON)
		
		# Ademas tambien podemos usar la extensión de wxThumb en la carpeta del proyecto para poder presentar la barra de botones
		# en la taskbar (opcional).
		
		option (wxWidgets_USE_THUMBNAIL_EXTENSION "Indica si utilizar la extensión propia para manejar thumbnail en el programa" ON)
		
		# Validamos la opción de Unicode para anexar la definición.
		
		if (wxWidgets_USE_UNICODE)
			
			# Anexaremos la definición de UNICODE y _UNICODE.
			
			add_definitions (-D_UNICODE -DUNICODE)
			
		endif ()
		
		# Si fue encontrado entonces pasaremos a anexar sus directorios y bibliotecas a nuestra lista.
		
		if (wxWidgets_FOUND)
			
			# Anexaremos a la lista de cabeceras las cabeceras para wxWidgets.
			
			list (APPEND ELECTRODEMP_HEADERS "${wxWidgets_INCLUDE_DIRS}")
			
			# Validamos el resultado de wxThumbnail y buscamos la biblioteca.
			
			if (wxWidgets_USE_THUMBNAIL_EXTENSION)
				
				# Buscaremos el paquete con está extensión para wxWidgets.
				
				find_package (wxThumbnail)
				
				# Si lo encontramos y es valido entonces lo anexaremos al programa para utilizarlo.
				
				if (wxThumbnail_FOUND)
					
					# Anexaremos las cabeceras para está biblioteca.
					
					list (APPEND ELECTRODEMP_HEADERS "${wxThumbnail_INCLUDE_DIR}")
					
					# Anexaremos a la lista de dependencias de bibliotecas tambien los archivos de thumbnail.
					
					list (APPEND ELECTRODEMP_MEDIA_PLAYER_DEPENDENCES "${wxThumbnail_LIBRARY}")
					
					# Configuramos la siguiente var de habilitación para el config.h.
					
					set (ELECTRODEMP_ENABLE_WX_THUMBNAIL TRUE)
					
				endif ()
				
			endif ()
			
			# Establecemos las dependencias de bibliotecas para el media player.
			
			list (APPEND ELECTRODEMP_MEDIA_PLAYER_DEPENDENCES "${wxWidgets_LIBRARIES}")
			
		endif ()
		
		# ----------------------------------------------------------------------
		
		# ----------------------------  ZLIB  ----------------------------------
		
		# Requerimos ZLIB como una base para la aplicación final en ejecutable.
		
		find_package (ZLIB REQUIRED)
		
		# Anexaremos su directorio de includes.
		
		list (APPEND ELECTRODEMP_HEADERS "${ZLIB_INCLUDE_DIRS}")
		
		# Anexaremos la dependencia a la lista de dependencias de bibliotecas.
		
		list (APPEND ELECTRODEMP_LIBRARY_DEPENDENCES "${ZLIB_LIBRARIES}")
		
		# ----------------------------------------------------------------------
		
		# ---------------------------- Systema  --------------------------------
		
		# Algunas dependencias de nuestro propio sistema que tambien debemos de utilizar para enlazar en el ejecutable son :
		#	API winmm , setupapi , ole32 entre muchas otras.
		
		list (APPEND ELECTRODEMP_LIBRARY_DEPENDENCES
			
			-lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lcomctl32 -lwinmm -lshell32 -lshlwapi -lsetupapi -lole32 
			-liconv -lWs2_32 -lSecur32 -lws2_32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lversion -luxtheme
			
		)
		
	endif ()
	
	# -----------------------  Definiciones Iniciales  -------------------------
	
	# Anexaremos las siguientes macro-defs y configuraciones para todos los compiladores en general.
	
	# Desabilitaremmos el uso de las macros max , min.
	
	add_definitions (-DNOMINMAX)
	
	# --------------------------------------------------------------------------
	
	# --------------------------  Configuración  -------------------------------
	
	# Por ultimo vamos a configurar el archivo de plantilla electrodemp_config.h.in para guardar en la salida
	# actual de nuestro programa , de está manera tendremos todos los valores de macro-defs que se
	# asignarón durante la configuración del cmake en un archivo .h.
	
	configure_file ("${CMAKE_SOURCE_DIR}/cmake/Templates/electrodemp_config.h.in" "${CMAKE_BINARY_DIR}/electrodemp_config.h")
	
	# Incluiremos ademas el directorio para la salida del programa donde está localizado esté
	# archivo config.
	
	list (APPEND ELECTRODEMP_HEADERS "${CMAKE_BINARY_DIR}")

# Finalizamos la creación de la función.

endmacro (ElectrodeMPDependences)

# ------------------------------------------------------------------------------
