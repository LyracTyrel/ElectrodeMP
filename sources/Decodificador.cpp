
// Implementaremos ahora nuestro codigo para el Decodificador.

#include <ElectrodeMP/Decodificador.h>

/// @file Decodificador.cpp
/// @brief Definición formal para nuestro decodificador de formatos de audio a muestras reproducibles.
/// @author Loubiner
/// @date Wednesday 14/02/2018

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// ---------------------  Formats , Decoders & Parsers  ------------------------

// Vamos a declarar algunas macro-definiciones para permitirnos registrar todos los formatos , codecs y parsers a utilizar en ElectrodeMP.

#define REGISTER_DEMUXER(X , x)                                         \
{                                                                   	\
	extern AVInputFormat ff_##x##_demuxer;                          	\
		if (CONFIG_##X##_DEMUXER)                                       \
			av_register_input_format (&ff_##x##_demuxer);               \
}

#define REGISTER_DECODER(X , x)                                         \
{                                                                   	\
	extern AVCodec ff_##x##_decoder;                                	\
		if (CONFIG_##X##_DECODER)                                       \
			avcodec_register (&ff_##x##_decoder);                       \
}

#define REGISTER_PARSER(X , x)                                          \
{                                                                   	\
	extern AVCodecParser ff_##x##_parser;                           	\
		if (CONFIG_##X##_PARSER)                                        \
			av_register_codec_parser (&ff_##x##_parser);                \
}

// Declaración formal de la función para iniciar y registrar los formatos soportados por ElectrodeMP.

static void Register_Formats () {
	
	// Registraremos entonces la siguiente lista de formatos para su Demuxer.
	//		Formato : AC3
	//		Formato : AAC
	//		Formato : OGG
	//		Formato : FLAC
	//		Formato : MP3
	//		Formato : WAV
	//		Formato : MOV , MP4 , M4A , 3GP.
	
	REGISTER_DEMUXER (AC3 , ac3);
	REGISTER_DEMUXER (AAC , aac);
	REGISTER_DEMUXER (OGG , ogg);
	REGISTER_DEMUXER (FLAC , flac);
	REGISTER_DEMUXER (MP3 , mp3);
	REGISTER_DEMUXER (WAV , wav);
	REGISTER_DEMUXER (MOV , mov);
	
}

// Declaración formal de la función para iniciar y registrar los decoders soportados por ElectrodeMP.

static void Register_Decoders () {
	
	// Registraremos nuestros decoders para los audios especificados.
	//		Decoder : ac3 , ac3_fixed
	//		Decoder : aac , aac_fixed , aac_latm
	//		Decoder : vorbis , theora
	//		Decoder : flac
	//		Decoder : mp3 , mp3float
	//		Decoder : PCM_DVD , PCM_F16LE , PCM_S16LE , PCM_S16BE , PCM_F32LE , PCM_F32BE , PCM_S32LE , PCM_S32BE.
	
	REGISTER_DECODER (AC3 , ac3);
	REGISTER_DECODER (AC3_FIXED , ac3_fixed);
	REGISTER_DECODER (AAC , aac);
	REGISTER_DECODER (AAC_FIXED , aac_fixed);
	REGISTER_DECODER (AAC_LATM , aac_latm);
	REGISTER_DECODER (VORBIS , vorbis);
	REGISTER_DECODER (THEORA , theora);
	REGISTER_DECODER (FLAC , flac);
	REGISTER_DECODER (MP3 , mp3);
	REGISTER_DECODER (MP3FLOAT , mp3float);
	REGISTER_DECODER (PCM_DVD , pcm_dvd);
	REGISTER_DECODER (PCM_F16LE , pcm_f16le);
	REGISTER_DECODER (PCM_S16LE , pcm_s16le);
	REGISTER_DECODER (PCM_S16BE , pcm_s16be);
	REGISTER_DECODER (PCM_F32LE , pcm_f32le);
	REGISTER_DECODER (PCM_F32BE , pcm_f32be);
	REGISTER_DECODER (PCM_S32LE , pcm_s32le);
	REGISTER_DECODER (PCM_S32BE , pcm_s32be);
	
}

// Declaración formal de la función para iniciar y registrar los parsers soportados por ElectrodeMP.

static void Register_Parsers () {
	
	// Registraremos tambien nuestros parsers para algunos formatos.
	//		Parser : ac3
	//		Parser : aac , aac_latm
	//		Parser : vorbis
	//		Parser : FLAC
	//		Parser : mpegaudio
	
	REGISTER_PARSER (AC3 , ac3);
	REGISTER_PARSER (AAC , aac);
	REGISTER_PARSER (AAC_LATM , aac_latm);
	REGISTER_PARSER (VORBIS , vorbis);
	REGISTER_PARSER (FLAC , flac);
	REGISTER_PARSER (MPEGAUDIO , mpegaudio);
	
}

// Utilizaremos el espacio de nombres del Proyecto de aqui en adelante.

using namespace ElectrodeMP;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------  Decodificador  ----------------------------------

// -----------------------------------------------------------------------------

// ---------------------------  Constructor  -----------------------------------

// Empezaremos por el constructor de nuestro decodificador para iniciar todos los valores a sus respectivos defaults.

Decodificador::Decodificador () {
	
	// Lo que realizaremos sera la correcta initialización de la biblioteca de FFmpeg , por lo cual requerimos entonces llamar a nuestro metodo siguiente y
	// realizar la comprobación del resultado.
	
	if (!Inicializar ()) {
		
		// Mostraremos por la consola el siguiente mensaje.
		
		LOG_E (" >> Error al Inicializar FFmpeg ")
		
	}
	
	// Mensaje de aviso LOG.
	
	LOG (" >> FFmpeg Inicializado ")
	
	// ------------------------------  Seeking  --------------------------------
	
	// Ahora vamos a colocar los valores de nuestros atributos para el desplazamiento (seek) por el audio actual en valores por defecto de falso y 0.
	
	Desplazar.store (false);
	
	// Colocaremos el tiempo a desplazar y la flag en 0.
	
	Tiempo_Desplazar.store (0) , Flag_Desplazar.store (0);
	
	// -------------------------------  Estado  --------------------------------
	
	// Ahora colocaremos el estado de actividad privado del decoder en falso.
	
	Activo.store (false);
	
	// Mostraremos el siguiente mensaje LOG para avisar.
	
	LOG_N (" :::::::::::::::::::::  Decodificador Creado  :::::::::::::::::::::: \n")
	
}

// ----------------------------  Initializador  --------------------------------

// Ahora vamos a implementar el codigo para nuestra función de inicialización de la bibliotec de FFmpeg la cual registrara todos los formatos de audio disponibles
// para nuestra aplicación.

// Definiremos el metodo initializer el cual devuelve true en caso de no presentar error.

bool Decodificador::Inicializar () {
	
	// Empezaremos por registrar todos los formatos para Demuxear archivos de audio.
	
	Register_Formats ();
	
	// Mostramos el mensaje de aviso siguiente.
	
	LOG (" >> Formato AC3 Registrado ")
	LOG (" >> Formato AAC Registrado ")
	LOG (" >> Formato OGG Registrado ")
	LOG (" >> Formato FLAC Registrado ")
	LOG (" >> Formato MP3 Registrado ")
	LOG (" >> Formato WAV Registrado ")
	LOG (" >> Formato MP4 , MOV y 3GP Registrados ")
	
	// Ahora vamos a registrar todos los decoders que utilizan estos formatos para descromprimir el audio.
	
	Register_Decoders ();
	
	// Mostramos ahora los mensajes siguientes.
	
	LOG (" >> Decoders AC3 , AC3_FIXED  Registrados ")
	LOG (" >> Decoders AAC , AAC_FIXED , AAC_LATM Registrados ")
	LOG (" >> Decoders VORBIS , THEORA Registrados ")
	LOG (" >> Decoder FLAC Registrado ")
	LOG (" >> Decoders MP3 , MP3FLOAT Registrados ")
	LOG (" >> Decoders PCM_DVD , PCM_F16LE , PCM_F32LE Registrados ")
	
	// Finalmente registraremos todos los parsers para ciertos archivos de audio.
	
	Register_Parsers ();
	
	// Mostramos por ultimo estos mensajes.
	
	LOG (" >> Parser AC3  Registrado ")
	LOG (" >> Parsers AAC , AAC_LATM Registrados ")
	LOG (" >> Parser VORBIS Registrado ")
	LOG (" >> Parser FLAC Registrado ")
	LOG (" >> Parser MPEGAUDIO Registrado ")
	
	// Devolvemos true.
	
	return true;
	
}

// -----------------------------------------------------------------------------

// ------------------------------  Demuxer  ------------------------------------

// Implementaremos el codigo para nuestro demuxer del decodificador el cual realiza la separación de nuestros flujos y con este podremos asignar el n-esimo
// flujo de audio a nuestro contexto actual.

bool Decodificador::Demuxer (const std::string & Filename) {
	
	// ------------------------------  Demuxer  --------------------------------
	
	// Validamos si tenemos nuestro contexto repartido en estos momentos para poderlo liberar de manera segura.
	
	if (Contexto_Formato != nullptr) {
		
		// Cerramos el contexto , liberamos el Formato repartido y todos sus streams , dejando su localida en null.
		
		avformat_close_input (&Contexto_Formato);
		
		// Asignaremos el stream de audio de vuelta a null.
		
		Stream_Audio = nullptr;
		
	}
	
	// Pasaremos a realizar la iniciación entonces de un Contexto de formato para poder abrir nuestro nuevo audio.
	
	Contexto_Formato = avformat_alloc_context ();
	
	// Lo primero que debemos de realizar es la apertura de nuestro archivo de audio actual , usaremos para esto nuestra función siguiente la cual se encarga de
	// leer nuestro archivo y determinar con su cabecera el contenido de nuestro audio , dejando iniciado los campos del contexto de Formato previamente
	// repartido.
	
	int Resultado = avformat_open_input (&Contexto_Formato , Filename.c_str () , nullptr , nullptr);
	
	// Validaremos si el resultado es diferente de 0.
	
	if (Resultado != 0) {
		
		// Error al abrir archivo : crearemos un buffer para guardar la cadena del error.
		
		char Error [300] = { '\0' };
		
		// Llenaremos el buffer de cadena con el error y mostraremos el siguiente mensaje.
		
		LOG_FE (" >> Error al Demuxear Archivo : %s " , av_make_error_string (Error , 299u , Resultado))
		
		// Devolvemos falso.
		
		return false;
		
	}
	
	// En caso de no presentar error , mostraremos el siguiente mensaje por la consola de LOG.
	
	LOG_F (" >> Archivo [ %s ] Demuxeado Correctamente " , Filename)
	
	// --------------------------  Streams  ------------------------------------
	
	// Ahora que ya tenemos nuestro archivo demuxeado en flujos separados , tenemos entonces que iterar por cada uno de ellos desde nuestro contexto de
	// formato e ir llenando nuestro vector de Streams validos de audio por cada uno de estós , el resto de streams diferentes de audio seran ignorados
	// para que no se pierda tiempo en buscar su información adicional.
	
	// Limpiaremos primero el vector de streams actuales.
	
	Streams.clear ();
	
	// Iteramos por todos los flujos contenidos en nuestro Formato actualmente demuxeado.
	
	for (unsigned int Indice_Flujo_Actual = 0u ; Indice_Flujo_Actual < Contexto_Formato->nb_streams ; Indice_Flujo_Actual ++) {
		
		// Obtendremos la referencia a nuestro Stream actual de esta manera.
		
		AVStream * Flujo_Actual = Contexto_Formato->streams [Indice_Flujo_Actual];
		
		// Dentro del flujo tenemos los parametros de Codec [codecpar] y dentro está contenido el ID de tipo de codec especifico , validaremos si el
		// actual corresponde con el tipo de Audio.
		
		if (Flujo_Actual->codecpar->codec_type == AVMediaType::AVMEDIA_TYPE_AUDIO) {
			
			// Anexaremos esté flujo de audio a nuestra lista de streams actual.
			
			Streams.push_back (Flujo_Actual);
			
		}
		else {
			
			// Vamos a descartar esté flujo para no considerarlo posteriormente , para estó tenemos un campo en el stream actual que permite indicar
			// que paquetes descargar , en nuestro caso , descartaremos todos los packets por lo que.
			
			Flujo_Actual->discard = AVDISCARD_ALL; // Indicamos a descartar todos los paquetes de esté stream.
			
		}
		
	}
	
	// Mostraremos el mensaje siguiente con la cantidad de flujos encontrados.
	
	LOG_F (" >> Flujos de Audio Disponibles : %i " , Streams.size ())
	
	// Devolvemos true.
	
	return true;
	
}

// Definiremos el metodo de asignación de stream de audio a decodificar , una vez hecha la separación de todos los streams de audio validos es momento
// de asignar uno en especifico a reproducir en el reproductor actual y decodificar en este decoder.

bool Decodificador::Select_Stream (size_t Indice) {
	
	// Antes que nada validamos que nuestro contexto esté inicializado y nuestro indice de flujo sea menor a la cantidad actual.
	
	if ((Contexto_Formato != nullptr) && (Indice < Streams.size ())) {
		
		// Asignaremos el Stream de audio actual de entre la lista que tenemos.
		
		Stream_Audio = Streams [Indice];
		
		// Ahora vamos a descartar el resto de Streams.
		
		for (AVStream * Stream_Actual : Streams) {
			
			// Si no són el que actualmente seleccionamos.
			
			if (Stream_Actual != Stream_Audio) {
				
				// Descartamos todos sus packets.
				
				Stream_Actual->discard = AVDISCARD_ALL;
				
			}
			
		}
		
		// Y asignaremos el modo discard en NONE.
		
		Stream_Audio->discard = AVDISCARD_NONE;
		
		// Mostraremos el siguiente mensaje.
		
		LOG_F (" >> Stream de Audio [ %i ] Seleccionado " , Indice)
		
		// ----------------------  Asignar Codec  ------------------------------
		
		// El siguiente paso es buscar por el archivo los parametros de formato especificos , para el caso de que la cabecera no tenga la suficiente información
		// , tenemos la siguiente llamada a función para ello. Retomaremos el resultado para validar.
		
		int Resultado = avformat_find_stream_info (Contexto_Formato , nullptr);
		
		// Validaremos que no haya errores aqui tampoco para poder continuar al siguiente paso.
		
		if (Resultado < 0) {
			
			// Error al buscar información en el archivo.
			
			char Error [300] = { '\0' };
			
			// Llenaremos el buffer de cadena con el error y mostraremos el siguiente mensaje.
			
			LOG_FE (" >> Error al Buscar Info de Stream : %s " , av_make_error_string (Error , 299u , Resultado))
			
			// Dejaremos el stream sin asignar en null.
			
			Stream_Audio = nullptr;
			
			// Devolvemos falso.
			
			return false;
			
		}
		
		// Mostraremos el siguiente resultado para poder continuar.
		
		LOG (" >> Informacion de Flujo Encontrada ")
		
		// Amtes de repartir memoria y asignar un contexto de codec , requerimos liberar el que tuviesemos actualmente asignado.
		
		if (Contexto_Codec != nullptr) {
			
			// Cerramos el codec , liberamos todos sus recursos utilizados y dejaremos la referencia devuelta a null.
			
			avcodec_free_context (&Contexto_Codec);
			
			// Dejaremos la referencia al codec en null.
			
			Codec_Audio = nullptr;
			
		}
		
		// Finalmente buscaremos el Codec y parametros para nuestro ID de codec actual en los parametros de codec. Usaremos el siguiente metodo privado
		// para realizar la acción y devolvemos el resultado.
		
		return Alloc_Decoder (Stream_Audio->codecpar);
		
	}
	
	// Mostraremos el mensaje de aviso siguiente.
	
	LOG_C (" >> Flujo de Audio No Valido ")
	
	// Devolvemos falso.
	
	return false;
	
}

// -------------------------------  Decoder  -----------------------------------

// Ahora toca implementar el siguiente metodo , el decoder , el cual como su nombre lo indica se encarga de la decodificación en segundo-plano de nuestro
// archivo de audio con todos los contextos llenos y preparados para iniciar en un thread separado. Tenemos entonces el siguiente codigo.

bool Decodificador::Start_Decode () {
	
	// Solicitaremos por lo mientras un total de 0.25 segundos de adelanto.
	
	Samples.Query (nullptr , nullptr , static_cast <unsigned long int> (Get_SampleRate () / 4u));
	
	// ----------------------  Iniciando Decodificación  -----------------------
	
	// Inicializaremos nuestro Packet con valores por defecto. Este paquet sera nuestro almacen de datos comprimidos el cual se enviara a nuestro Decoder para
	// poder realizar nuestra decodificación y recuperar el Frame de los datos RAW de audio decodificados los cuales pasaran por una etapa de procesamiento y
	// normalización antes de ser colocados en la cola de Muestras.
	
	AVPacket * Paquete = av_packet_alloc ();
	
	// Validamos que nuestro paquete se haya creado correctamente.
	
	if (Paquete == nullptr) {
		
		// Error al repartir memoria para el packet , mostramos el siguiente mensaje.
		
		LOG_E (" >> Error al Repartir Packet ")
		
		// Terminaremos el muestreador con interrupción.
		
		Samples.Muestreador_Terminate (true);
		
		// Devolvemos falso.
		
		return false;
		
	}
	
	// Mostramos un mensaje de aviso siguiente.
	
	LOG (" >> Packet Repartido ")
	
	// Asi que comenzaremos con un ciclo while el cual terminara cuando se encuentre el final del archivo , cuando surga un error o cuando se detenga la actividad
	// de decodificación por parte de otro thread.
	
	// Establecemos el decodificación completada en falso para poder validar en todo momento.
	
	Samples.Set_DecodedCompleted (false);
	
	// Asignaremos el estado de decodificación en true y comenzamos con la iteración.
	
	Activo.store (true);
	
	// ----------------------------  Iterador  ---------------------------------
	
	// Para la iteración requerimos usar un Thread especifico que realiza la lectura del archivo , decodifique y procese nuestros frames de salida del decodificador
	// , asi mismo colocara las muestras como vayan saliendo en la cola de muestras global del decodificador.
	
	// Implementaremos el codigo de la función de nuestro Thread en el siguiente functor lambda.
	
	std::function <void (AVPacket *)> Funcion_Iterador = [this] (AVPacket * Paquete) -> void {
		
		// Lo primero sera crear nuestro Frame de resultado donde colocaremos toda la información de cada paquete decodificado una vez que haya pasado por el
		// decodificador , repartiremos su memoria una sola vez y la borraremos al terminar el ciclo de decodificación.
		
		AVFrame * Frame = av_frame_alloc ();
		
		// Validamos si el Frame fue creado correctamente.
		
		if (Frame == nullptr) {
			
			// Error al repartir memoria para el frame , mostramos el siguiente mensaje.
			
			LOG_E (" >> Error al Repartir Frame ")
			
			// Liberamos la memoria de nuestro Paquete.
			
			av_packet_free (&Paquete);
			
			// Asi mismo terminaremos el muestreador con interrupción.
			
			Samples.Muestreador_Terminate (true);
			
			// Terminamos nuestro proceso de decodificación asignando este valor en falso.
			
			Activo.store (false);
			
			// Terminamos la función.
			
			return;
			
		}
		
		// Mostramos el siguiente mensaje de aviso.
		
		LOG (" >> Frame Repartido ")
		
		// Comenzaremos con un desplazmiento de Seek si asi es el valor actualmente asignado.
		
		if (Desplazar.load ()) {
			
			// Moveremos nuestro flujo de audio actual a la posición especificada.
			
			if (av_seek_frame (Contexto_Formato , Stream_Audio->index , Tiempo_Desplazar.load () , Flag_Desplazar.load ()) < 0) {
				
				// Mostraremos simplemente un mensaje de aviso.
				
				LOG_C (" >> Error al Desplazar Frame ")
				
			}
			
			// Asignaremos el valor a falso.
			
			Desplazar.store (false);
			
		}
		
		// Para poder guardar los valores obtenidos por el proceso de Lectura , Send/Receive Packets y Frames , tenemos las siguientes localidades.
		
		int Resultado = 0 , Resultado_S = 0 , Resultado_R = 0;
		
		// Limpiaremos el buffer del codec antes de comenzar a leer el primer packet.
		
		avcodec_flush_buffers (Contexto_Codec);
		
		// Realizaremos la tarea de leer Packet de nuestro archivo de entrada y con esto ir decodificando cada paquete en Frame los cuales procesaremos usando
		// nuestro metodo especifico. El siguiente bucle permitira continuar iterando hasta terminar con el archivo o hasta que se valide en falso la localidad
		// de activo.
		
		while (Activo.load ()) {
			
			// Realizaremos la lectura de nuestro siguiente packet en el stream de audio actual , obtenemos el resultado para validar solamente si el Resultado
			// de Paquete es igual a 0 (no error).
			
			if (Resultado_S == 0) {
				
				// Realizamos la lectura a un nuevo packet de datos a ser decodificado.
				
				Resultado = av_read_frame (Contexto_Formato , Paquete);
				
				// Validamos el resultado para determinar si continuar o no.
				
				if (Resultado < 0) {
					
					// Recolocaremos el estado del decodificador a completado.
					
					Samples.Set_DecodedCompleted (true);
					
					// Rompemos el ciclo actual.
					
					break;
					
				}
				
			}
			
			// Antes de pasar a la decodificación debemos saber si el Packet leido corresponde a un Packet del flujo de audio seleccionado actualmente , para esto
			// comparamos sus indices de stream antes de decodificarlo.
			
			if (Paquete->stream_index == Stream_Audio->index) {
				
				// -------------------  Decodificador  -------------------------
				
				// Ahora que tenemos la información de nuestro paquete , pasaremos a decodificar el paquete usando el decoder actual. Asi que enviaremos el paquete
				// usando la siguiente llamada a función y obtenemos el resultado.
				
				Resultado_S = avcodec_send_packet (Contexto_Codec , Paquete);
				
				// Validamos si fue correctamente pasado el paquete de datos al decodificador.Si el error fue debido a un estado actual , entonces
				// pasaremos a continuar con la siguiente parte (recivir el frame).
				
				if ((Resultado_S != 0) && (Resultado_S != AVERROR (EAGAIN))) {
					
					// Si el error fue por causa de EOF , entonces solo terminaremos.
					
					if (Resultado_S != AVERROR_EOF) {
						
						// Error al decodificar datos del paquete. Tenemos el siguiente buffer para el error.
						
						char Error [300] = { '\0' };
						
						// Llenaremos el buffer de cadena con el error y mostraremos el siguiente mensaje.
						
						LOG_FE (" >> Error de Packet : %s " , av_make_error_string (Error , 299u , Resultado_S))
						
						// Asi mismo terminaremos el muestreador con interrupción.
						
						Samples.Muestreador_Terminate (true);
						
					}
					
					// Entonces pasaremos a terminar nuestro ciclo actual.
					
					break;
					
				}
				
				// Si el resultado del envio del packet hacia el decoder fue 0 (SUCCESS) , entonces desreferenciaremos el buffer interno.
				
				if (Resultado_S == 0) {
					
					// Quitamos la referencia del buffer del paquete.
					
					av_packet_unref (Paquete);
					
				}
				
				// Ahora que ya tenemos nuestros datos decodificados por el decoder usando los parametros de Contexto , obtendremos la data de nuestro audio
				// RAW lista para ser procesada y encolada en nuestras muestras de audio para el reproductor. Usamos la siguiente llamada a funcíón.
				
				Resultado_R = avcodec_receive_frame (Contexto_Codec , Frame);
				
				// Validamos si el resultado tuvo errores o no , Si el error fue por causa de EAGAIN , entonces solo continuaremos al siguiente ciclo.
				
				if (Resultado_R == AVERROR (EAGAIN)) {
					
					// Continuamos con el siguiente ciclo para colocar nuevo paquete de datos.
					
					continue;
					
				}
				else if (Resultado_R != 0) {
					
					// Si fue por algo diferente a terminación de archivo , mostraremos el siguiente mensaje.
					
					if (Resultado_R != AVERROR_EOF) {
						
						// Error al recivir frame de nuestros datos decodificados.
						
						char Error [300] = { '\0' };
						
						// Llenaremos el buffer de cadena con el error y mostraremos el siguiente mensaje.
						
						LOG_FE (" >> Error de Frame : %s " , av_make_error_string (Error , 299u , Resultado_R))
						
						// Terminamos el muestreador.
						
						Samples.Muestreador_Terminate (true);
						
					}
					
					// Terminamos el ciclo.
					
					break;
					
				}
				
				// ---------------------  Callback  ----------------------------
				
				// Realizaremos la llamada a nuestra función de callback si es que la tenemos asignada para este decodificador.
				
				if (Funcion_Procesamiento) {
					
					// Invocamos la función de procesamiento adicional con la info de este Frame actual y con los datos opcionales.
					
					Funcion_Procesamiento (this , Frame , Datos_Callback_Procesamiento);
					
				}
				
				// ----------------------  Procesando  -------------------------
				
				// Como parte final tenemos al procesador del Frame el cual realizara toda la normalización y procesamiento digital de nuestras muestras
				// de audio RAW decodificadas.
				
				Muestreador::Muestras_Audio Samples_Colocar (std::move (Process_Frame (Frame)));
				
				// Sumaremos la cantidad decodificada de muestras actuales.
				
				Samples.Add_DecodedSamples (Samples_Colocar.Cantidad);
				
				// Ahora vamos a realizar una consulta formal de colocación a nuestro muestreador con la dirección de esta localidad.
				
				if (!Samples.Query (nullptr , &Samples_Colocar)) {
					
					// Finalizamos la colocación de muestras , por lo que rompemos el ciclo.
					
					break;
					
				}
				
			}
			
		}
		
		// Para terminar solamente resta liberar la memoria de nuestro Frame y Packet actuales.
		
		av_frame_free (&Frame) , av_packet_free (&Paquete);
		
		// ---------------------------  Callback  ------------------------------
		
		// Si tenemos asignada una función para llamar cuando se termine de procesar/decodificar nuestro audio , la invocaremos en este momento.
		
		if (Funcion_Finalizacion) {
			
			// Invocamos está función con los parametros establecidos y sus datos opcionales.
			
			Funcion_Finalizacion (this , Datos_Callback_Finalizacion);
			
		}
		
		// ----------------------  Finalizado  ---------------------------------
		
		// Terminamos nuestro proceso de decodificación asignando este valor en falso.
		
		Activo.store (false);
		
	};
	
	// Mostramos el siguiente mensaje de aviso.
	
	LOG (" >> Decodificacion Iniciada ")
	
	// Y crearemos y lanzaremos nuestro nuevo thread , por lo cual necesitamos pasar como parametro la función y el Paquete repartido.
	
	Nucleo_Decodificar = new std::thread (Funcion_Iterador , Paquete);
	
	// Devolvemos true.
	
	return true;
	
}

// Tenemos el metodo que simplemente pausa la decodificación actualmente en curso para darle lugar a otro evento en el programa.

void Decodificador::Pause_Decode () {
	
	// Primero asignaremos un valor de Actividad en falso para que el Thread termine su procesamiento.
	
	Activo.store (false);
	
	// Ahora validaremos que nuestro Thread este iniciado y para esto tenemos la siguiente comprobación.
	
	if (Nucleo_Decodificar != nullptr) {
		
		// Ahora esperaremos a que nuestro thread termine o si ya termino entonces continuaremos.
		
		Nucleo_Decodificar->join ();
		
		// Borraremos este thread.
		
		delete (Nucleo_Decodificar);
		
		// Lo asignaremos a null.
		
		Nucleo_Decodificar = nullptr;
		
	}
	
}

// Tenemos el siguiente metodo para detener la decodificación actualmente iniciada y cerrar el Thread especifico que está decodificando el audio actualmente
// si es que todavia está activo.

void Decodificador::Stop_Decode () {
	
	// Primero asignaremos un valor de Actividad en falso para que el Thread termine su procesamiento.
	
	Activo.store (false);
	
	// Ahora validaremos que nuestro Thread este iniciado y para esto tenemos la siguiente comprobación.
	
	if (Nucleo_Decodificar != nullptr) {
		
		// Ahora esperaremos a que nuestro thread termine o si ya termino entonces continuaremos.
		
		Nucleo_Decodificar->join ();
		
		// Borraremos este thread.
		
		delete (Nucleo_Decodificar);
		
		// Lo asignaremos a null.
		
		Nucleo_Decodificar = nullptr;
		
	}
	
	// Si la queue es valida , entonces la limpiaremos.
	
	if (Samples.Valid_Queue ()) {
		
		// Eliminaremos nuestra cola de Muestras actuales.
		
		Samples.Clear_Queue ();
		
		// Y resetearemos el stream de audio a su origen.
		
		Reset ();
		
	}
	
}

// Implementaremos nuestro metodo Seek para desplazar nuestro audio a determinado segundo para continuar la decodificación desde ese momento. Realizaremos
// entonces el siguiente metodo.

void Decodificador::Seek (size_t Second) {
	
	// Desplazaremos al segundo especificado si el valor no supera la duración actual.
	
	if (Second > Get_Duration ()) {
		
		// Limitaremos el valor de tiempo al total del audio.
		
		Second = Get_Duration ();
		
	}
	
	// Ahora vamos a realizar un escalamiento con factor para nuestro valor de tiempo actual de esta manera. Usando las utilerias de FFmpeg podemos convertir
	// nuestro valor de tiempo en segundos a un valor de timestamp que podamos utilizar con base a nuestro stream de audio actual.
	
	// Asignaremos el valor siguiente convertido a la base de tiempo de nuestro stream.
	
	const int64_t Second_TimeEstamp = av_rescale_q (static_cast <int64_t> (Second * AV_TIME_BASE) , AVRational { 1 , AV_TIME_BASE } , Stream_Audio->time_base);
	
	// Ahora vamos a realizar el desplazamiento de nuestro stream actual de audio , asignaremos a nuestra localidad de valor el tiempo de timestamp.
	
	Tiempo_Desplazar.store (Second_TimeEstamp);
	
	// Calcularemos la cantidad de muestras a avanzar hasta llegar al segundo especificado.
	
	const size_t Second_Sample = Get_SampleTime (Second);
	
	// La flag para desplazar dependera de nuestro segundo actual y muestra actual. Si el segundo corresponde a una muestra menor entonces sera hacia atras.
	
	Flag_Desplazar.store ((Second_Sample < Samples.Get_DecodedSamples ()) ? AVSEEK_FLAG_BACKWARD : 0);
	
	// Asignaremos el valor de la muestras decodificadas y procesadas actualmente en el valor indice.
	
	Samples.Set_ProcessedSamples (Second_Sample) , Samples.Set_DecodedSamples (Second_Sample);
	
	// Y asignaremos el valor de true en nuestro seek.
	
	Desplazar.store (true);
	
}

// Definiremos el metodo para liberar y cerrar el contexto de FFmpeg creado hasta el momento para poder generar un nuevo contexto a partir de otro flujo
// de audio nuevamente.

void Decodificador::Free () {
	
	// Comenzaremos primero limpiando todas las muestras de la queue de samples y liberando su espacio en memoria.
	
	if (Samples.Valid_Queue ()) {
		
		// Limpiaremos la queue.
		
		Samples.Clear_Queue () , Samples.Delete_Queue ();
		
	}
	
	// Validamos si tenemos nuestro contexto repartido en estos momentos para poderlo liberar de manera segura.
	
	if (Contexto_Formato != nullptr) {
		
		// Cerramos el contexto , liberamos el Formato repartido y todos sus streams , dejando su localida en null.
		
		avformat_close_input (&Contexto_Formato);
		
		// Asignaremos el stream de audio de vuelta a null.
		
		Stream_Audio = nullptr;
		
	}
	
	// Ahora validaremos que el condec actual haya sido inicializado para eliminarlo tambien.
	
	if (Contexto_Codec != nullptr) {
		
		// Cerramos el codec , liberamos todos sus recursos utilizados y dejaremos la referencia devuelta a null.
		
		avcodec_free_context (&Contexto_Codec);
		
		// Dejaremos la referencia al codec en null.
		
		Codec_Audio = nullptr;
		
	}
	
	// Y dejaremos la lista de streams vacia.
	
	Streams.clear ();
	
}

// ----------------------------  Destructor  -----------------------------------

// Definiremos nuestro destructor para detener el decoder en caso de no estarlo , liberar recursos y terminar de manera segura.

Decodificador::~ Decodificador () {
	
	// Si el estado del decodificador es activo , entonces lo detendremos.
	
	if (Activo.load ()) {
		
		// Detenemos el decodificador actual.
		
		Stop_Decode ();
		
	}
	
	// Ahora liberaremos los recursos ocupados.
	
	Free ();
	
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------  Privados  ------------------------------------

// Entonces pasaremos a definir nuestro metodo para repartir memoria para nuestro Contexto de Decoder y con estó tener preparado nuestro decodificador
// para iniciar cuando sea el momento.

bool Decodificador::Alloc_Decoder (const AVCodecParameters * Parametros) {
	
	// Lo primero que debemos de hacer es buscar el decoder y asignarlo a nuestra propiedad , para esto tenemos el codec_id de nuestros parametros , asi
	// entonces tenemos la siguiente asignación.
	
	Codec_Audio = avcodec_find_decoder (Parametros->codec_id);
	
	// Validamos que tengamos asignado un codec correcto.
	
	if (Codec_Audio == nullptr) {
		
		// Error al buscar decoder apropiado , mostraremos el siguiente mensaje de error.
		
		LOG_E (" >> Error , no se encontró Decoder ")
		
		// Devolvemos falso.
		
		return false;
		
	}
	
	// Asi que ya tenemos el Decoder a utilizar , mostramos el siguiente mensaje de aviso.
	
	LOG_F (" >> Decoder [ %s ] Asignado " , Codec_Audio->name)
	
	// El siguiente paso es la asignación de nuestro contexto de codec con valores por defecto a partir del Decoder que obtuvimos. Primero validamos si ya
	// tenemos un contexto asignado.
	
	if (Contexto_Codec != nullptr) {
		
		// Entonces liberamos este contexto antes de asignar otro.
		
		avcodec_free_context (&Contexto_Codec);
		
	}
	
	// Asignaremos un nuevo contexto a partir del Decoder , inicializando sus valores por defecto.
	
	Contexto_Codec = avcodec_alloc_context3 (Codec_Audio);
	
	// Asi que validaremos si fue correctamente creado nuestro nuevo COntexto de Codec.
	
	if (Contexto_Codec == nullptr) {
		
		// Error al repartir nuevo Contexto de Codec , mostramos el siguiente mensaje LOG.
		
		LOG_E (" >> Error al Crear Contexto del Codec ")
		
		// Devolvemos falso.
		
		return false;
		
	}
	
	// Mostramos un mensaje de aviso siguiente.
	
	LOG (" >> Contexto del Codec Creado e Inicializado ")
	
	// Lo siguiente que queda es copiar los parametros de nuestro Flujo en la entrada de este metodo a nuestro nuevo Contexto de Codec repartido para que pueda determinar
	// la manera de trabajar con nuestro archivo de audio. Copiamos usando la siguiente función y retomaremos el resultado para validar.
	
	int Resultado = avcodec_parameters_to_context (Contexto_Codec , Parametros);
	
	// Validamos si el resultado fue correcto.
	
	if (Resultado < 0) {
		
		// Error al copiar parametros del flujo al contexto de codec , tenemos el siguiente buffer para el error.
		
		char Error [300] = { '\0' };
		
		// Llenaremos el buffer de cadena con el error y mostraremos el siguiente mensaje.
		
		LOG_FE (" >> Error al Copiar Parametros de Flujo al Contexto : %s " , av_make_error_string (Error , 299u , Resultado))
		
		// Devolvemos falso.
		
		return false;
		
	}
	
	// Ahora mostramos el siguiente mensaje de aviso.
	
	LOG (" >> Parametros del Flujo de Audio Copiados Correctamente ")
	
	// Finalmente vamos a abrir nuestro Codec y asignarlo a nuestro contexto de Codec para que lo utilice , realizamos entonces la siguiente llamada y obtenemos el
	// resultado para determinar si fue correctamente abierto.
	
	Resultado = avcodec_open2 (Contexto_Codec , Codec_Audio , nullptr);
	
	// Validamos que el resultado sea correcto antes de continuar.
	
	if (Resultado < 0) {
		
		// Error al abrir nuestro codec para la decodificación. Usamos el buffer de error siguiente.
		
		char Error [300] = { '\0' };
		
		// Llenaremos el buffer de cadena con el error y mostraremos el siguiente mensaje.
		
		LOG_FE (" >> Error al Abrir Codec para el Contexto : %s " , av_make_error_string (Error , 299u , Resultado))
		
		// Devolvemos falso.
		
		return false;
		
	}
	
	// Mostramos el mensaje siguiente para confirmar.
	
	LOG (" >> Codec Asignado a nuestro Contexto ")
	
	// Y devolvemos true.
	
	return true;
	
}

// Definiremos el codigo para el Procesador de Frames el cual su función es la de normalizar los valores que tenemos en el Frame del parametro a un intervalo
// valido para la reproducción de este audio y aplicar algunos efectos. Devolverá la Muestra de Audio para su reproducción.

Muestreador::Muestras_Audio Decodificador::Process_Frame (const AVFrame * Frame) {
	
	// Ahora que tenemos nuestro Frame queda realizar el procesamiento del mismo para generar nuestro resultado en dos buffers de salida , uno para el canal
	// izquierdo y otro para el canal derecho , ambos del tipo de Sample especificado en nuestra estructura de la Muestra.
	
	// Pero antes de esto necesitamos determinar el tipo de Sample que estamos utilizando , a partir de su formato determinaremos la manera de continuar , por lo que
	// requerimos saber antes que nada si nuestro formato es de tipo entrelazado o empaquetado.
	
	// ---------------------------  Formato de Muestra  ------------------------
	
	// La estructura de nuestro Frame viene con muchos campos llenados para nuestro audio , entre el que encontramos el formato de nuestro arreglo de Samples ,
	// este formato es un valor enumerado de AVSampleFormat para nuestro caso de audio. O un valor de -1 si no es valido. Tenemos en FFmpeg la siguiente lista
	// de formatos de muestras de Audio :
	// 		AV_SAMPLE_FMT_U8 	unsigned 8 bits
	// 		AV_SAMPLE_FMT_S16 	signed 16 bits
	// 		AV_SAMPLE_FMT_S32 	signed 32 bits
	// 		AV_SAMPLE_FMT_FLT 	float
	// 		AV_SAMPLE_FMT_DBL 	double
	// 		AV_SAMPLE_FMT_U8P 	unsigned 8 bits, planar
	// 		AV_SAMPLE_FMT_S16P 	signed 16 bits, planar
	// 		AV_SAMPLE_FMT_S32P 	signed 32 bits, planar
	// 		AV_SAMPLE_FMT_FLTP 	float, planar
	// 		AV_SAMPLE_FMT_DBLP 	double, planar
	// 		AV_SAMPLE_FMT_S64 	signed 64 bits
	// 		AV_SAMPLE_FMT_S64P 	signed 64 bits, planar
	
	// Ahora validaremos que tipo de Sample tenemos y continuaremos con el procesamiento de los Samples para determinado caso.
	
	switch (Frame->format) {
		
		// Formato : unsigned 8 bits [interleaved o planar].
		
		case AVSampleFormat::AV_SAMPLE_FMT_U8 :
		case AVSampleFormat::AV_SAMPLE_FMT_U8P :
			
			// Procesaremos los samples con este tipo de dato. Devolvemos el resultado.
			
			return std::move (Muestreador::Muestras_Audio::Procesar (reinterpret_cast <const uint8_t * const *> (Frame->data) ,
				Frame->nb_samples , Frame->channels , Frame->linesize [0] , (Frame->format == AVSampleFormat::AV_SAMPLE_FMT_U8)));
			break;
		
		// Formato : signed 16 bits [interleaved o planar].
		
		case AVSampleFormat::AV_SAMPLE_FMT_S16 :
		case AVSampleFormat::AV_SAMPLE_FMT_S16P :
			
			// Procesaremos los samples con este tipo de dato.
			
			return std::move (Muestreador::Muestras_Audio::Procesar (reinterpret_cast <const int16_t * const *> (Frame->data) ,
				Frame->nb_samples , Frame->channels , Frame->linesize [0] , (Frame->format == AVSampleFormat::AV_SAMPLE_FMT_S16)));
			break;
		
		// Formato : signed 32 bits [interleaved o planar].
		
		case AVSampleFormat::AV_SAMPLE_FMT_S32 :
		case AVSampleFormat::AV_SAMPLE_FMT_S32P :
			
			// Procesaremos los samples con este tipo de dato.
			
			return std::move (Muestreador::Muestras_Audio::Procesar (reinterpret_cast <const int32_t * const *> (Frame->data) ,
				Frame->nb_samples , Frame->channels , Frame->linesize [0] , (Frame->format == AVSampleFormat::AV_SAMPLE_FMT_S32)));
			break;
		
		// Formato : float [interleaved o planar].
		
		case AVSampleFormat::AV_SAMPLE_FMT_FLT :
		case AVSampleFormat::AV_SAMPLE_FMT_FLTP :
			
			// Procesaremos los samples con este tipo de dato.
			
			return std::move (Muestreador::Muestras_Audio::Procesar (reinterpret_cast <const float * const *> (Frame->data) ,
				Frame->nb_samples , Frame->channels , Frame->linesize [0] , (Frame->format == AVSampleFormat::AV_SAMPLE_FMT_FLT)));
			break;
		
		// Formato : double [interleaved o planar].
		
		case AVSampleFormat::AV_SAMPLE_FMT_DBL :
		case AVSampleFormat::AV_SAMPLE_FMT_DBLP :
			
			// Procesaremos los samples con este tipo de dato.
			
			return std::move (Muestreador::Muestras_Audio::Procesar (reinterpret_cast <const double * const *> (Frame->data) ,
				Frame->nb_samples , Frame->channels , Frame->linesize [0] , (Frame->format == AVSampleFormat::AV_SAMPLE_FMT_DBL)));
			break;
		
		// Formato : signed 64 bits [interleaved o planar].
		
		case AVSampleFormat::AV_SAMPLE_FMT_S64 :
		case AVSampleFormat::AV_SAMPLE_FMT_S64P :
			
			// Procesaremos los samples con este tipo de dato.
			
			return std::move (Muestreador::Muestras_Audio::Procesar (reinterpret_cast <const int64_t * const *> (Frame->data) ,
				Frame->nb_samples , Frame->channels , Frame->linesize [0] , (Frame->format == AVSampleFormat::AV_SAMPLE_FMT_S64)));
			break;
		
	}
	
	// Devolvemos una muestra vacia.
	
	return std::move (Muestreador::Muestras_Audio ());
	
}

// -----------------------------------------------------------------------------

// --------------------------------  Reset  ------------------------------------

// Tenemos la  definición de nuestro metodo Reset para reiniciar nuestro decodificador al origen del archivo , usaremos este metodo para
// los eventos de Stop y de completado.

void Decodificador::Reset () {
	
	// Validamos que nuestro contexto esté correctamente creado asi como nuestro stream esté asignado.
	
	if ((Contexto_Formato != nullptr) && (Stream_Audio != nullptr)) {
		
		// Obtendremos el tiempo de timestamp para el inicio en el segundo 0.
		
		const int64_t Second_TimeEstamp = av_rescale_q (static_cast <int64_t> (0 * AV_TIME_BASE) , AVRational { 1 , AV_TIME_BASE } , Stream_Audio->time_base);
		
		// Ahora vamos a realizar el desplazamiento de nuestro stream actual de audio , asignaremos a nuestra localidad de valor el tiempo de timestamp.
		
		Tiempo_Desplazar.store (Second_TimeEstamp);
		
		// Tenemos la flag de Backward para retroceder nuestro stream.
		
		Flag_Desplazar.store (AVSEEK_FLAG_BACKWARD);
		
		// Resetearemos los contadores de muestras procesadas y decodificadas.
		
		Samples.Set_ProcessedSamples (0u) , Samples.Set_DecodedSamples (0u);
		
		// Colocaremos el valor de desplazamiento en true.
		
		Desplazar.store (true);
		
	}
	
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------
