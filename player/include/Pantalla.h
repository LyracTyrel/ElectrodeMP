
/// @file Pantalla.h
///	@brief Archivo de codigo con la declaración de la clase de Pantalla la cual es la encargada de inicializar todos los
/// componentes de la interfaz de usuario para nuestro reproductor de audio.
///
/// El funcionamiento de esta pantalla es con base a la PantallaBase generada automaticamente con wxFormBuilder y la biblioteca
/// de wxWidgets la cual utiliza los elementos nativos de nuestro sistema operativo para crear los componentes visuales y en el
/// caso de windows es por medio de la API de win32.
/// @author Loubiner
/// @date Monday 2/19/2018

#ifndef PANTALLA_H
#define PANTALLA_H

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

// -----------------------  Modulo : Reproductor  ------------------------------

// El funcionamiento del reproductor reside en el modulo del Reproductor el cual incluye a su vez al decodificador para poder realizar
// la descompresión del formato de audio de entrada y obtener las muestras las cuales son procesadas por el Sampler y finalmente asignadas
// a nuetro reproductor para reproducir el audio necesario.

#include <ElectrodeMP/Reproductor.h>

// Utilizaremos el espacio de nombres de ElectrodeMP.

using namespace ElectrodeMP;

// ----------------------------  wxWidgets  ------------------------------------

// Ahora pasaremos a incluir nuestra cabecera principal para la biblioteca wxWidgets con la mayor parte de los elementos comunes que ocuparemos
// para nuestros elementos graficos de UI.

#include <wx/wx.h>

// -----------------------  Modulo : PantallaBase  -----------------------------

// Además de todo lo anterior incluiremos a nuestra pantalla base la cual fue generada automaticamente con la herramienta de wxFormBuilder
// para la facilitación del codigo y optimización al maximo de la API de wxWidgets.

#include <PantallaBase.h>

// -------------------------  Modulo : Dialogo  --------------------------------

// Para poder elegir entre los diferentes Host y Dispositivos de reproducción que tiene instalado nuestro sistema operativo , contamos con
// un dialogo para realizar la asignación de dispositivo y host.

#include <Dialogo.h>

// ----------------------  Modulo : Dialogo Ayuda ------------------------------

// Para mostrar la información de la aplicación en un simple dialogo tenemos la siguiente cabecera.

#include <DialogoAyuda.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Vamos a declarar entonces nuestra clase de la Pantalla la cual deriva directamente de PantallaBase para implementar sus metodos virtuales
// de cada evento en los controles a inicializar.

/// @defgroup Pantalla
/// @brief Pantalla principal de Interfaz de Usuario para el reproductor de audio.
/// @{

/// @brief
///	Declaramos la clase para nuestra Pantalla principal de la interfaz de usuario la cual presenta los componentes más basicos que cualquier
/// reproductor de audio debe de tener y una pequeña barra de herramientas con algunas opciones adicionales.

class Pantalla : public PantallaBase {
	
	// Comenzaremos con la asignación de los elementos privados para nuestra clase actual.
	
	private :
		
		// ------------------------  Reproductor  ------------------------------
		
		// Tenemos el primer elemento el cual es nuestró reproductor de Audio y Decodificador en la clase Reproductor , esté elemento es nuestra
		// base para el funcionamiento de nuestro programa. Por lo que incluiremos su memoria en este momento.
		
		std::shared_ptr <Reproductor> Player;
		
		// --------------------------  Slider  ---------------------------------
		
		// Declaramos una localidad que nos permitira determinar si estamos desplazando el audio con el seek de nuestro slider o no.
		
		bool Player_Seek = false;
		
		// --------------------------  Resize  ---------------------------------
		
		// Para los eventos de resize necesitamos mantener la misma relación de aspecto que tiene la pantalla inicialmente con el fin de evitar
		// deformaciones a lo alto o ancho , asi que requerimos guardar el aspect ratio al inicio de la aplicación.
		
		float Relacion_Aspecto = 1.0f;
		
		// --------------------------  Thumb  ----------------------------------
		
		// Solamente si la opción está habilitada.
		
		#ifdef ELECTRODEMP_ENABLE_WX_THUMBNAIL
		
		// Como parte adicional , tenemos los componentes para nuestra barra de progreso directamente obtenida de nuestra biblioteca de extensión
		// de wxWidgets la cual tiene como objetivo presentar un progreso de reproducción en nuestra taskbar que se ira llenando conforme el audio
		// vaya progresando de un valor minimo de 0 y maximko de 100.
		
		wxThumbProgress * Barra_Progreso;
		
		#endif
	
	// Ahora pasaremos a la implementación de todos los eventos y construres de nuestra Clase actual.
	
	public :
		
		// ----------------------  ID Thumb Buttons  ---------------------------
		
		/// @brief Ahora vamos a declarar una enumeración para nuestros botones de Thumb de la barra de tareas en nuestra aplicación.
		
		enum IDBotonesThumb : wxWindowID {
			
			/// @brief ID para el wxThumbButton de Play.
			
			Thumb_Boton_Play_ID = 0u ,
			
			/// @brief ID para el wxThumbButton de Pause.
			
			Thumb_Boton_Pause_ID ,
			
			/// @brief ID para el wxThumbButton de Stop.
			
			Thumb_Boton_Stop_ID
			
		};
		
		// -----------------------  Constructores  -----------------------------
		
		/// @name Constructores
		/// @brief Clase constructora de la Pantalla de UI.
		///
		/// @{
		
		/// @brief Constructor por defecto que inicializa todos los elementos en nuestra interfaz de usuario y permite iniciar tambien los componentes
		/// del reproductor de audio y decodificador para su utilización en el resto del programa hasta que esta pantalla se destruya.
		/// @param Titulo String con el titulo de nuestra aplicación.
		
		Pantalla (const std::string & Titulo);
		
		/// @}
		
		// -------------------------  Eventos  ---------------------------------
		
		/// @name Eventos de Pantalla.
		/// @brief Metodos de cada uno de los eventos de nuestra pantalla organizados por categorias de acuerdo
		/// al widget que lo esta efectuando.
		///
		/// Todos estos eventos están conectados a sus respectivos widgets en la superclase PantallaBase la cual declarar los metodos virtuales para
		/// cada uno de los eventos.
		/// @{
		
		/// @name Eventos de Menú.
		/// @brief Implementación de los eventos de los elementos de la Barra de Menu.
		/// @{
		
		/// @brief Metodo para el Evento del Menu Archivo : Abrir.
		/// @param Argumentos Argumentos de Menu Item para nuestro evento.
		
		void Evento_Archivo_Abrir (wxCommandEvent & Argumentos) override;
		
		/// @brief Metodo para el Evento del Menu Archivo : Cerrar.
		/// @param Argumentos Argumentos del Menu Item para nuestro evento.
		
		void Evento_Archivo_Cerrar (wxCommandEvent & Argumentos) override;
		
		/// @brief Metodo para el Evento del Menu Archivo : Salir.
		/// @param Argumentos Argumentos de Menu Item para nuestro evento.
		
		void Evento_Archivo_Salir (wxCommandEvent & Argumentos) override;
		
		/// @brief Metodo para el Evento del Menu Herramientas : Device.
		/// @param Argumentos Argumentos de Menu Item para nuestro evento.
		
		void Evento_Herramientas_Device (wxCommandEvent & Argumentos) override;
		
		/// @brief Metodo para el Evento del Menu Ayuda : Acerca.
		/// @param Argumentos Argumentos de Menu Item para nuestro evento.
		
		void Evento_Ayuda_Acerca (wxCommandEvent & Argumentos) override;
		
		/// @}
		
		/// @name Eventos de ToolBar.
		/// @brief Tenemos la implementación de los eventos de la barra de herramientas de nuestra aplicación.
		/// @{
		
		/// @brief Metodo para el Evento de la Herramienta Abrir.
		/// @param Argumentos Argumentos de nuestro Evento.
		
		void Evento_Tool_Abrir (wxCommandEvent & Argumentos) override;
		
		/// @brief Metodo para el Evento de la Herramienta Cerrar.
		/// @param Argumentos Argumentos de nuestro Evento.
		
		void Evento_Tool_Cerrar (wxCommandEvent & Argumentos) override;
		
		/// @brief Metodo para el Evento de la Herramienta Dispositivo.
		/// @param Argumentos Argumentos de nuestro Evento.
		
		void Evento_Tool_Device (wxCommandEvent & Argumentos) override;
		
		/// @}
		
		/// @name Eventos de Reproducción.
		/// @brief Tenemos la implementación de los Eventos para los Botones de Play , Pause y Stop.
		/// @{
		
		/// @brief Metodo para el Evento de la Acción Play.
		///
		/// Comienza a reproductir inmediatamente el audio actualmente abierto. Debemos tener el estado Open para poder reproducir
		/// un audio y terminarlo con el botón de Stop.
		/// @param Argumentos Argumentos de Comando para el Evento.
		
		void Evento_Play (wxCommandEvent & Argumentos) override;
		
		/// @brief Metodo para el Evento de la Acción Pause.
		///
		/// Pausa la reproducción actual de audio (si es que tenemos el estado correcto) para poder cerrarlo y abrir otro archivo o
		/// para reanudarlo en cualquier momento.
		/// @param Argumentos Argumentos de Comando para el Evento.
		
		void Evento_Pause (wxCommandEvent & Argumentos) override;
		
		/// @brief Metodo para el Evento de la Acción Stop.
		///
		/// Detiene la reproducción de audio actual , estó permite reiniciar el audio a su origen desde el inicio o abrir un archivo
		/// de audio nuevo.
		/// @param Argumentos Argumentos de Comando para el Evento.
		
		void Evento_Stop (wxCommandEvent & Argumentos) override;
		
		// Habilitamos el evento wxThumb para la opción especifica habilitada.
		
		#ifdef ELECTRODEMP_ENABLE_WX_THUMBNAIL
		
		/// @brief Metodo para los Eventos de Thumb Buttons en la TaskBar actual.
		///
		/// Ademas de los controles de la pantalla principal para reproducir , pausar y detener , contamos con los botones de la barra
		/// de tareas de Thumb los cuales son accesos directos a estas mismas funciones cuando el reproductor está minimizado y se quiere
		/// realizar alguna de estas acciones.
		/// @param Argumentos Argumentos de wxThumbEvent para nuestros botones de Thumb.
		
		void Evento_Thumb_Boton (wxThumbEvent & Argumentos);
		
		#endif
		
		/// @}
		
		/// @name Evento de Progreso.
		/// @brief Tenemos algunos metodos para la barra de progreso de nuestro reproductor de Audio.
		/// @{
		
		/// @brief Comenzaremos implementando el metodo para cambio de posición de nuestro audio mediante el Slider.
		///
		/// Mueve el audio a cierta posición en la linea de tiempo representada por esté Slider y efectua la reanudación del audio actual.
		/// @param Argumentos Argumentos del slider para efectuar la acción.
		
		void Evento_Cambio_Seek (ElectrodeProgresoEvent & Argumentos);
		
		/// @brief Metodo para el evento que actualiza los valores de tiempo actual mostrados en la etiqueta respectiva conforme la barra avanza.
		/// @param Argumentos Argumentos del slider para el evento.
		
		void Evento_Slider_Seek (ElectrodeProgresoEvent & Argumentos);
		
		/// @brief Tenemos ahora el evento general que activa o desactiva el cambio de seek para detener temporalmente la actualización
		/// del progreso de la barra.
		/// @param Argumentos Argumentos del slider para el evento.
		
		void Evento_Seek (ElectrodeProgresoEvent & Argumentos);
		
		/// @}
		
		/// @name Evento de Frame.
		/// @brief Tenemos el evento Close para nuestro Frame actual.
		/// @{
		
		/// @brief Metodo para el evento Close de nuestro Frame actual el cual cerrara todos los elementos y terminara de manera
		/// correcta con nuestra biblioteca de reproducción y decodificación de archivos de audio.
		/// @param Argumentos Argumentos para el Evento Close.
		
		void Evento_Close (wxCloseEvent & Argumentos) override;
		
		/// @brief Metodo para el Evento Resize de nuestro Frame Principal , esté simplemente restringe la relación de aspecto
		/// que tiene el reproductor al inicio para evitar deformaciones en el contenido.
		/// @param Argumentos Argumentos para el evento.
		
		void Evento_Size (wxSizeEvent & Argumentos) override;
		
		/// @}
		
		/// @}
		
		// ------------------------  Destructor  -------------------------------
		
		/// @brief Destructor de la Pantalla para finalizar correctamente el programa del reproductor.
		
		virtual ~ Pantalla ();
	
	// Pasaremos a declarar algunos metodos auxiliares privados.
	
	private :
		
		// ------------------------  Progress Bar  -----------------------------
		
		// Definiremos un metodo auxiliar el cual realiza la construcción y configuración de la progress bar personalizada para el reproductor.
		
		void Iniciar_Progress ();
		
		// --------------------------  TaskBar  --------------------------------
		
		// Unicamente si la opción es valida.
		
		#ifdef ELECTRODEMP_ENABLE_WX_THUMBNAIL
		
		// Pasaremos a declarar nuestro metodo para inicializar todos los componentes de nuestra barra de Taskbar.
		
		void Iniciar_Task ();
		
		#endif
		
		// --------------------------  Stream  ---------------------------------
		
		// Declaramos un metodo especifico para poder seleccionar un stream de entre todos los posibles y abrirlo , unicamente valido
		// cuando hay más de un stream de audio.
		
		void Select_Stream ();
		
		// ---------------------------  Tiempo  --------------------------------
		
		// Implementaremos un metodo para actualizar la etiqueta de tiempo con el valor actual del tiempo con respecto al audio y al
		// valor de la barra de progreso.
		
		void Reload_Time ();
		
		// Ahora un metodo para poder recargar el tiempo total o duración de la song actualmente cargada en el media player.
		
		void Reload_Duration ();
		
		// Ahora otra función para resetear la etiqueta de tiempo a su estado inicial.
		
		void Reset_Time ();
		
		// Y una más para poder resetear la etiqueta de duración.
		
		void Reset_Duration ();
		
		// --------------------------  Etiqueta  -------------------------------
		
		// Implementaremos un metodo para recargar toda la información de la Canción que actualmente está sonando , al momento de abrir
		// el archivo de audio se llenarán los campos y la imagen de portada con la información obtenida.
		
		void Reload_Tag ();
		
		// Ahora para resetear el estado de las Etiquetas tenemos el siguiente metodo simple que asigna una cadena vacia a todos
		// los labels presentados.
		
		void Reset_Tag ();
		
		// --------------------------  Progreso  -------------------------------
		
		// Ahora vamos a declarar un evento para actualizar nuestra barra de progreso en función de nuestro reproductor de audio y
		// sus metodos para consultar la posición actual del audio reproduciendo.
		
		void Reload_Progress ();
		
		// Tenemos el siguiente metodo que resetea el progreso actual a su estado original , permitiendo con esto volver a
		// reproducir la misma song.
		
		void Reset_Progress ();
	
};

/// @}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

#endif
