
/// @file ElectrodeProgreso.h
///	@brief Cabecera para el widget de la barra de progreso personalizada del reproductor de ElectrodeMP , está barra sera hecha a partir
/// de algunos elementos graficos visuales personalizados desde los recursos de la aplicación.
///
/// La barra tendra los eventos para poder lanzar eventos cuando es presionada en cualquier parte , y devolvera un evento con la posición
/// relativa dentro de un intervalo en la que fue presionada. Con base a un simple slider.
/// @author Menightmar
/// @date Lunes 19/03/2018

#ifndef ELECTRODEMP_PROGRESO_H
#define ELECTRODEMP_PROGRESO_H

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------  Bases  ---------------------------------------

// Incluiremos para empezar nuestras cabeceras de C.

#include <stdio.h>
#include <stdlib.h>

// Tenemos las funciones matematicas de y algoritmos C/C++.

#include <algorithm>
#include <cmath>

// Ahora tenemos las cadenas de C++.

#include <string>

// -------------------------  Configuracion  -----------------------------------

// Importaremos el modulo de configuraciones de la biblioteca ElectrodeMP.

#include <ElectrodeMP/Configuracion.h>

// ----------------------------  wxWidgets  ------------------------------------

// Incluiremos la cabecera para wxWidgets de donde ocuparemos principalmente al contenedor de wxWindow principal para poder derivar de
// esté nuestro Botón personalizado.

#include <wx/wx.h>

// Ademas contendremos tamebien la clase wxMemoryDC para poder trazar los contextos de Bitmap en la pantalla.

#include <wx/dcmemory.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/// @defgroup ElectrodeProgreso
/// @brief Barra de progreso (TimeLine) del reproductor de ElectrodeMP.
/// @{

/// @brief Clase wxWindow que provee de un simple progress bar o gauge que detecta los clicks en todo su ancho y permite devolver la
/// posición del ultimo click mediante eventos especificos.
///
/// La barra deriva directamente de la clase wxWindow de wxWidgets , teniendo acceso a las funciones de la API que nos proveen de la mayoria de
/// funcionalidades para trabajar con controles independientes del contexto.

class ElectrodeProgreso : public wxWindow {
	
	// Tenemos los siguientes elementos privados.
	
	private :
		
		// -----------------------  Elementos Visuales  ------------------------
		
		// Tenemos ahora como primer elemento el rectangulo que representa la localización y tamaño exacto de nuestra barra de Progreso la
		// cual tenemos definida con esté control.
		
		wxRect Recuadro_Progreso;
		
		// Ahora vamos a definir los bitmap de fondo que tendra la barra de progreso. Estos sera dibujado al principio como fondo para nuestra
		// barra dandole un aspecto bastante personalizado.
		
		wxBitmap Fondo_Progreso , Fondo_Progreso_Disable , Fondo_Progreso_Filled;
		
		// Tenemos adicionalmente un Fondo de Mascara (monocromatico) para establecere el area de progreso en esté fondo que es valida para
		// mover el cursor.
		
		wxBitmap Fondo_Progreso_Line;
		
		// Tenemos el modo de operación para sobreponer el fondo de relleno y el fondo natural de la barra.
		
		wxRasterOperationMode Fondo_Progreso_Operacion = wxCOPY;
		
		// Determinaremos la relación entre el ancho y alto de ambos fondos para fines de trazados.
		
		float Relacion_Fondo_X = 1.0f , Relacion_Fondo_Y = 1.0f;
		
		// El color para poder iluminar la linea de progreso actual (mediante una linea gruesa) conforme la barra incrementé su valor actual.
		
		wxColour Color_Progreso;
		
		// Tenemos asi mismo el contexto de dibujado para el mapa del fondo , esté sera asignado cuando se establezca un fondo de progreso y
		// en caso de no contar con fondo solamente se pintara en el mismo color de fondo que la ventana contenedora.
		
		wxMemoryDC Contexto_Fondo;
		
		// -----------------------------  Cursor  ------------------------------
		
		// Ademas del fondo de la barra , tenemos tambien un elemento visual para el cursor el cual puede ser personalizado utilizando un bitmap
		// para el elemento grafico y otro para la mascara. O bien podemos dejar el cursor por defecto representado por un circulo blanco y un
		// circulo interno del color del progreso.
		
		// El cursor actual pasa por diferentes estados , siendo cada uno representado por un bitmap distinto.
		
		enum STATE_CURSOR {
			
			// Estado Activo (Default).
			
			CURSOR_ENABLE = 0 ,
			
			// Estado Disactivado.
			
			CURSOR_DISABLE ,
			
			// Estado Seleccionado.
			
			CURSOR_SELECTED ,
			
			// Estado Presionado.
			
			CURSOR_PRESSED
			
		} Cursor_State = STATE_CURSOR::CURSOR_ENABLE;
		
		// Tenemos ahora los bitmaps para el Cursor a dibujar encima de la barra conforme su posición o valor de la misma. Tenemos en general uno
		// para cada uno de los estados.
		
		wxBitmap Cursor_Progreso , Cursor_Progreso_Disable , Cursor_Progreso_Selected , Cursor_Progreso_Pressed;
		
		// Asi mismo tenemos el recuadro que está representando nuestro cursor dentro del espacio asignado a la barra de progreso.
		
		wxRect Recuadro_Cursor;
		
		// La separación entre el recuadro de la barra y el cursor será determinado por la siguiente propiedad.
		
		int Cursor_Border = 5;
		
		// Tendremos como referencia las posiciones X_Max y X_Min que el cursor puede moverse por el Recuadro de está barra.
		
		int Cursor_X_Min = 0 , Cursor_X_Max = 0;
		
		// Ahora el contexto para poder retomar los pixeles del Bitmap del cursor y utilizarlos para trazar el cursor.
		
		wxMemoryDC Contexto_Cursor;
		
		// -----------------------------  Eventos  -----------------------------
		
		// Ahora tenemos algunos elementos adicionales que realizan acciones sobre la barra con el fin de detectar los eventos asociados a
		// la progress bar.
		
		// Var que indica si el botón izquierdo del mouse toco la barra actual , por lo que debera comenzar a detectar eventos.
		
		bool Progreso_Left_Click = false;
		
		// Var que indica si el mouse está sobre la linea de la barra de progreso valida.
		
		bool Progreso_Line_Mouse = false;
		
		// Tenemos el siguiente valor que indica la posición X del mouse registrada al momento de hacer click.
		
		int Progreso_Left_X = 0;
		
		// ----------------------------  Propiedad  ----------------------------
		
		// Como propiedades tenemos primero a la regla de valores la cual determinara las posiciones de nuestro cursor de mouse con respecto
		// al tamaño de la barra y a esté intervalo de valores. Tenemos el minimo inicial en 0 y el maximo en 100.
		
		long int Progreso_Min = 0 , Progreso_Max = 100;
		
		// Y tenemos asi mismo el progreso actual de la barra que commienza en 0.
		
		long int Progreso_Actual = 0;
		
		// Ahora vamos a colocar un valor float que representa los segmentos de espacio que hay por cada unidad en el progreso actual en pixeles.
		
		float Progreso_Diff = 0.0f;
	
	// Tenemos a continuación la declaración formal de todos los metodos publicos.
	
	public :
		
		// -----------------------  Constructores  -----------------------------
		
		/// @name Constructores.
		/// @brief Constructores de la ElectrodeProgreso widget.
		/// @{
		
		/// @brief Constructor por defecto.
		
		ElectrodeProgreso ();
		
		/// @brief Constructor para iniciar el wxWindow con la posición , tamaño , parent window , etc.
		///
		/// Esté constructor creara del elemento de barra de progreso en la ventana asignada en Parent y a partir de estó podemos realizar la
		/// asignación de eventos mediante el uso de tables estaticas o de forma dinamica. Utilizando un rango por defecto de 0 a 100 en la
		/// linea de tiempo.
		/// @param Parent Window contenedor para el elemento.
		/// @param ID Identificador unico para la barra.
		/// @param Position Posición asignada a nuestra ventana relativa al control contenedor Parent.
		/// @param Size Tamaño de la barra de progreso.
		/// @param Estilo Flags de estilo de wxWindow.
		
		explicit ElectrodeProgreso (wxWindow * Parent , wxWindowID ID , const wxPoint & Position = wxDefaultPosition 
			, const wxSize & Size = wxDefaultSize , long Estilo = 0);
		
		/// @}
		
		// ---------------------------  Bitmap  --------------------------------
		
		/// @name Bitmaps.
		/// @brief La barra de progreso permite personalizar un poco mas su aspecto utilizando bitmaps para el fondo y el cursor respectivamente.
		/// @{
		
		/// @brief Metodo para asignar un nuevo mapa de fondo a nuestra barra de progreso.
		/// @param Fondo Nuevo bitmap a utilizar como fondo para la barra de progreso.
		
		void Set_Background (const wxBitmap & Fondo);
		
		/// @brief Metodo para asignar un bitmap que represente la barra de progreso en estado desactivado.
		/// @brief Fondo Bitmap a aplicar a la barra de progreso como fondo cuando este en estado @b disable.
		
		void Set_Background_Disable (const wxBitmap & Fondo);
		
		/// @brief Metodo para asignar un bitmap de relleno cuando la barra de progreso se va llenando.
		/// @param Relleno Bitmap que se ira mostrando a medida que el valor de la barra de progreso se aproxime más al limite max.
		/// @param Operacion Modo de operación a trabajar del tipo @ref wxRasterOperationMode.
		
		void Set_Background_Filled (const wxBitmap & Relleno , wxRasterOperationMode Operacion = wxCOPY);
		
		/// @brief Metodo para asignar un bitmap monocromatico que señale con un color @b Blanco las partes del fondo que són presionables con el
		/// cursor.
		/// Esté area permite determinar la zona del background de la barra de progreso que es pulsable y por ende el cursor se movera a la posición
		/// en X relativa del cursor.
		/// @param Linea Bitmap que señala con @b Blanco la zona presionable de la barra y con @b Negro el resto que no es señalable.
		
		void Set_Background_Line (const wxBitmap & Linea);
		
		/// @brief Si lo que se desea es solamente utilizar una linea de un color solido para marcar el relleno de la barra , usamos este metodo.
		/// @param Color_Relleno Color para rellenar la barra de progreso.
		/// @param Operacion Modo de operación a trabajar del tipo @ref wxRasterOperationMode.
		
		void Set_Progress_Colour (const wxColour & Color_Relleno , wxRasterOperationMode Operacion = wxCOPY);
		
		/// @brief Metodo para asignar un mapa de bits para el cursor en estado activo asi como una mascara de bits a utilizar.
		/// @param Bitmap_Cursor Bitmap para el cursor de la barra de progreso.
		/// @param Bitmap_Mask Bitmap de mascara para el cursor (opcional).
		/// @note Este cursor sera asignado al resto de estados del cursor (para modificarlo @see Set_Cursor_Disable @see Set_Cursor_Selected @see Set_Cursor_Pressed).
		/// Ademas se puede quitar el cursor utilizando un @b wxNullBitmap como parametro.
		
		void Set_Cursor (const wxBitmap & Bitmap_Cursor , const wxBitmap & Bitmap_Mascara = wxNullBitmap);
		
		/// @brief Metodo para asignar un mapa de bits para el cursor en estado disable de la barra de igual forma que el anterior caso.
		/// @param Bitmap_Cursor Bitmap para el cursor cuando la barra está en estado disable.
		/// @param Bitmap_Mask Bitmap de mascara para el cursor desabilitado (opcional).
		/// Se puede quitar el cursor utilizando un @b wxNullBitmap como parametro , en este caso se usara el bitmap por defecto.
		
		void Set_Cursor_Disable (const wxBitmap & Bitmap_Cursor , const wxBitmap & Bitmap_Mascara = wxNullBitmap);
		
		/// @brief Metodo para asignar un mapa de bits para el cursor en estado seleccionado de igual forma que el anterior caso.
		/// @param Bitmap_Cursor Bitmap para el cursor cuando se tiene seleccionado.
		/// @param Bitmap_Mask Bitmap de mascara para el cursor seleccionado (opcional).
		/// Se puede quitar el cursor utilizando un @b wxNullBitmap como parametro , en este caso se usara el bitmap por defecto.
		
		void Set_Cursor_Selected (const wxBitmap & Bitmap_Cursor , const wxBitmap & Bitmap_Mascara = wxNullBitmap);
		
		/// @brief Metodo para asignar un mapa de bits para el cursor en estado presionado de igual forma que el anterior caso.
		/// @param Bitmap_Cursor Bitmap para el cursor cuando se tiene presionado.
		/// @param Bitmap_Mask Bitmap de mascara para el cursor presionado (opcional).
		/// Se puede quitar el cursor utilizando un @b wxNullBitmap como parametro , en este caso se usara el bitmap por defecto.
		
		void Set_Cursor_Pressed (const wxBitmap & Bitmap_Cursor , const wxBitmap & Bitmap_Mascara = wxNullBitmap);
		
		/// @brief Metodo para asignar el tamaño del borde del cursor actualmente asignado.
		/// @param Border Cantidad en pixeles de separación del borde del recuadro y el cursor.
		
		void Set_Cursor_Border (int Border);
		
		/// @}
		
		// -------------------------  State Cursor  ----------------------------
		
		/// @name Estado del Cursor.
		/// @brief Metodos para conocer el estado actual del cursor en la barra de progreso.
		/// @{
		
		/// @brief Empezaremos por colocar una sobrecarga para el metodo Enable de wxWindow.
		/// @param Habilitar Indica si activar/desactivar la barra de progreso actual.
		/// @return True si el control fue habilitado , falso en caso contrario.
		
		bool Enable (bool Habilitar = true) override;
		
		/// @brief Metodo para obtener la posición X relativa a está barra donde está colocado el recuadro del cursor actual.
		///
		///	El recuadro del cursor es un wxRect que define la posición y el tamaño del Bitmap del Cursor de está barra de progreso. Este metodo devuelve
		/// lo que es la posición X de la esquina superior izquierda de este recuadro.
		/// @return Posicion X relativa del recuadro del cursor.
		
		ELECTRODEMP_FORCE_INLINE int Get_Cursor_X () const {
			
			// Devolvemos la posición x actual del recuadro.
			
			return Recuadro_Cursor.x;
			
		}
		
		/// @brief Metodo para obtener la posición X minima que el cursor puede tener.
		/// @return Posicion X relativa minima a la que el cursor puede moverse.
		
		ELECTRODEMP_FORCE_INLINE int Get_Cursor_X_Min () const {
			
			// Devolvemos la posición x minima del cursor fijada.
			
			return Cursor_X_Min;
			
		}
		
		/// @brief Metodo para obtener la posición X maxima que el cursor puede tener.
		/// @return Posicion X relativa maxima a la que el cursor puede moverse.
		
		ELECTRODEMP_FORCE_INLINE int Get_Cursor_X_Max () const {
			
			// Devolvemos la posición x maxima del cursor fijada.
			
			return Cursor_X_Max;
			
		}
		
		/// @brief Para determinar si el cursor está activo o (default) tenemos estó.
		/// @return True si el cursor está en estado enable (default).
		
		ELECTRODEMP_FORCE_INLINE bool Is_Cursor_Enable () const {
			
			// Devolvemos true si el estado es asi.
			
			return (Cursor_State == STATE_CURSOR::CURSOR_ENABLE);
			
		}
		
		/// @brief Para determinar si el cursor está desabilitado tenemos estó.
		/// @return True si el cursor está en estado disable.
		
		ELECTRODEMP_FORCE_INLINE bool Is_Cursor_Disable () const {
			
			// Devolvemos true si el estado es asi.
			
			return (Cursor_State == STATE_CURSOR::CURSOR_DISABLE);
			
		}
		
		/// @brief Para determinar si el cursor está seleccionado tenemos estó.
		/// @return True si el cursor está en estado selected.
		
		ELECTRODEMP_FORCE_INLINE bool Is_Cursor_Selected () const {
			
			// Devolvemos true si el estado es asi.
			
			return (Cursor_State == STATE_CURSOR::CURSOR_SELECTED);
			
		}
		
		/// @brief Para determinar si el cursor está presionado tenemos estó.
		/// @return True si el cursor está en estado pressed.
		
		ELECTRODEMP_FORCE_INLINE bool Is_Cursor_Pressed () const {
			
			// Devolvemos true si el estado es asi.
			
			return (Cursor_State == STATE_CURSOR::CURSOR_PRESSED);
			
		}
		
		/// @}
		
		// ---------------------------  Values  --------------------------------
		
		/// @name Valores.
		/// @brief Conjunto de metodos para asignar el rango de valores de la barra de progreso asi como el valor actualmente mostrando.
		/// @{
		
		/// @brief Metodo para asignar el valor minimo de la barra de progreso , efectuando un ajuste al valor mostrado actualmente.
		///
		/// Estó hará que la barra actualice su representación de progreso actual y por tanto cambie la posición del cursor.
		/// @param Limite_Min Valor minimo de la barra de progreso.
		/// @return Referencia a esté objeto.
		
		ElectrodeProgreso & Set_Value_Min (long int Limite_Min);
		
		/// @brief Metodo para obtener el valor Minimo que tiene la barra de progreso en estos momentos.
		/// @return valor minimo que tiene la barra de progreso.
		
		ELECTRODEMP_FORCE_INLINE long int Get_Value_Min () const {
			
			// Devolvemos el valor minimo.
			
			return Progreso_Min;
			
		}
		
		/// @brief Metodo para asignar el valor maximo de la barra de progreso , efectuando un ajuste al valor mostrado actualmente.
		///
		/// Estó hará que la barra actualice su representación de progreso actual y por tanto cambie la posición del cursor.
		/// @param Limite_Max Valor maximo de la barra de progreso.
		/// @return Referencia a esté objeto.
		
		ElectrodeProgreso & Set_Value_Max (long int Limite_Max);
		
		/// @brief Metodo para obtener el valor Maximo que tiene la barra de progreso en estos momentos.
		/// @return valor maximo que tiene la barra de progreso.
		
		ELECTRODEMP_FORCE_INLINE long int Get_Value_Max () const {
			
			// Devolvemos el valor maximo.
			
			return Progreso_Max;
			
		}
		
		/// @brief Metodo para asignar el valor actualmente representando en la barra mediante el cursor utilizado.
		///
		/// @note Utilizamos esté metodo para desplazar la barra a diferentes posiciones en el rango [Min , Max]. Si el valor supera estos
		/// limites entonces sera recortado @b clampeado.
		/// @param Valor_Actual Valor a mover el cursor de la barra actual.
		/// @return Referencia a esté objeto.
		
		ElectrodeProgreso & Set_Value (long int Valor_Actual);
		
		/// @brief Metodo para obtener el valor actual que tiene la barra de progreso en estos momentos.
		/// @return valor actual que tiene la barra de progreso.
		
		ELECTRODEMP_FORCE_INLINE long int Get_Value () const {
			
			// Devolvemos el valor actual de la barra.
			
			return Progreso_Actual;
			
		}
		
		/// @}
		
		// -------------------------  Destructor  ------------------------------
		
		/// @brief Tenemos el destructor de nuestra ElectrodeProgreso para eliminar todos los elementos de la jerarquia de wxWindow.
		
		virtual ~ ElectrodeProgreso ();
	
	// Tenemos los siguientes metodos internos para pintar nuestra barra.
	
	private :
		
		// ---------------------------  Draw  ----------------------------------
		
		// Vamos a implementar el siguiente metodo para realizar el pintado de nuestra barra de contexto sobre la superficie de dibujo asignada.
		
		void Progreso_Draw (wxPaintEvent & Argumentos);
		
		// Tenemos otro metodo el cual realiza el dibujado de la representación del progreso de la barra actual utilizando el valor actual con
		// respecto al maximo y minimo establecidos. Tiene como parametro el contexto a utilizar para dibujar sobre la ventana.
		
		void Progreso_Draw_Progress (wxPaintDC & Contexto);
		
		// Asi mismo tenemos el metodo para pintar el cursor actual de la barra de progreso en el valor asignado en estos momentos y utilizando 
		// los limites como referencia para poderlo localizar.
		
		void Progreso_Draw_Cursor (wxPaintDC & Contexto);
		
		// -------------------------  Recuadros  -------------------------------
		
		// Declaramos un metodo auxiliar cuya función es la de recargar el tamaño del recuadro de la barra de progreso con base a un tamaño especificado.
		
		void Progreso_Progress_Rect (const wxSize & Size_Ventana);
		
		// Declaramos un metodo para calcular el tamaño (recuadro) del cursor con base al tamaño actual del recuadro de progreso asignado.
		
		void Progreso_Cursor_Rect ();
		
		// Por otro lado tenemos el metodo para recalcular la posición del cursor actual , con base al valor actual de la barra.
		
		void Progreso_Cursor_Position ();
		
		// --------------------------  Cursor  ---------------------------------
		
		// Para determinar si el mouse está dentro del area de progreso (linea intermedia en la barra) , usaremos el siguient emetodo.
		
		bool Progreso_Line_Inside (const wxPoint & Posicion);
		
		// Para determinar si determinado punto está dentro del area especifica de dibujo del cursor actual , usaremo esté metodo.
		
		bool Progreso_Cursor_Inside (const wxPoint & Posicion);
		
		// Tenemos el siguiente metodo interno para realizar todo el procesamiento cuando el mouse está dentro de la barra de progreso y no está
		// presionando el cursor.
		
		void Progreso_Mouse_Inside (wxMouseEvent & Argumentos , bool Line_Inside = false , bool Cursor_Inside = false);
		
		// Tenemos otro metodo para realizar todos los procesos necesarios en la barra de progreso cuando el cursor esta seleccionado y ademas
		// se está presionando el botón izquierdo del mouse.
		
		void Progreso_Mouse_Pressed (wxMouseEvent & Argumentos , bool Line_Inside = false , bool Cursor_Inside = false);
		
		// --------------------------  Eventos  --------------------------------
		
		// Comenzamos por declarar un evento para cuando el control cambia de tamaño y por tanto hay que actualizar los recuadros.
		
		void Progreso_Size_Event (wxSizeEvent & Argumentos);
		
		// Tenemos el siguiente metodo para atender todos los eventos de mouse que entren en la barra de progreso actual.
		
		void Progreso_Mouse_Event (wxMouseEvent & Argumentos);
		
		// Tenemos el evento desplazamiento de la barra para cuando el mouse está presionando el cursor y movemos el mouse por la barra.
		
		void Progreso_Seek_Event (wxMouseEvent & Argumentos);
		
		// Finalmente declaramos la tabla de eventos internos para nuestra clase de Barra de Progreso.
		
		DECLARE_EVENT_TABLE ()
	
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// --------------------------  Evento Progress  --------------------------------

/// @brief Clase derivada de un wxCommandEvent para la atención de los eventos ocasionados por la barra de progreso
/// 
/// Los cuales pueden ser basicamente por efectos de click , soltar y arrastrar el cursor de la barra de progreso cambiando con esto los valores
/// actuales para el contador interno.

class ElectrodeProgresoEvent : public wxCommandEvent {
	
	// Elementos especificos de este evento solamente para informar sobre el valor actual de la barra de progreso.
	
	private :
		
		// Tenemos el estado actual de la barra , si está siendo presionada o soltada.
		
		bool Progreso_Click = false;
		
		// Tenemos el valor actual de la barra de progreso.
		
		long int Progreso_Valor = 0;
	
	// Como elementos publicos tenemos los siguientes.
	
	public :
		
		// -------------------------  Constructores  ---------------------------
		
		/// @name Constructores.
		/// @brief Constructores para el evento de la barra de progreso.
		/// @{
		
		/// @brief Constructor vacio por defecto para iniciar con los valores iniciales.
		
		ElectrodeProgresoEvent ();
		
		/// @brief Constructor con el estado de click en la barra de progreso y su valor actualmente presentado.
		/// @param Progreso_Estado_Click Bool que indica si está siendo presionado el cursor o no.
		/// @param Progreso_Valor_Actual Valor entero actualmente marcado por la barra de progreso.
		/// @param Progreso_Tipo_Evento Tipo de evento ocasionado.
		/// @param Progreso_ID Identificador del control de barra de progreso.
		
		explicit ElectrodeProgresoEvent (bool Progreso_Estado_Click , long int Progreso_Valor_Actual , wxEventType Progreso_Tipo_Evento , int Progreso_ID = 0);
		
		/// @brief Constructor de copia por defecto para esté evento.
		/// @param Copia Evento a copiar sus elementos.
		
		ElectrodeProgresoEvent (const ElectrodeProgresoEvent & Copia) = default;
		
		/// @}
		
		// ---------------------------  Accesores  -----------------------------
		
		/// @name Accesores.
		/// @brief Metodos accesores de datos del Evento especificamente.
		/// @{
		
		/// @brief Metodo para asignar el estado de la barra de progreso.
		/// @param Progreso_Estado_Click Indica si está siendo presionada la barra de progreso o no.
		
		void Set_Progress_State (bool Progreso_Estado_Click);
		
		/// @brief Metodo para obtener el estado actual de la barra de progreso.
		/// @return Estado actual (presionado o no).
		
		ELECTRODEMP_FORCE_INLINE bool Get_Progress_State () const {
			
			// Devolvemos el valor que tenemos asignado.
			
			return Progreso_Click;
			
		}
		
		/// @brief Metodo para asignar el valor actual marcado por la barra de progreso.
		/// @param Progreso_Valor_Actual Valor entero que especifica la posición de la barra de progreso.
		
		void Set_Progress_Value (long int Progreso_Valor_Actual);
		
		/// @brief Metodo para obtener el valor actual de la barra de progreso asignado a esté evento.
		/// @return Valor marcado por la barra de progreso.
		
		ELECTRODEMP_FORCE_INLINE long int Get_Progress_Value () const {
			
			// Devolvemos el valor marcado por la barra.
			
			return Progreso_Valor;
			
		}
		
		/// @}
		
		// ----------------------------  Clonación  ----------------------------
 		
		/// @brief Sobreescribiremos tambien el metodo Clone para duplicar una instancia de este evento.
		/// @return Devuelve una instancia nueva (copia) de esté evento.
		
		virtual wxEvent * Clone () const override {
			
			// Devolvemos la siguiente instancia.
			
			return dynamic_cast <wxEvent *> (new ElectrodeProgresoEvent (*this));
			
		}
	
};

// -------------------------  Tipos de Eventos  --------------------------------

// Declaramos el tipo de Evento para cuando el cursor de la barra esta siendo presionado.

wxDECLARE_EVENT (wxEVT_ELECTRODE_PROGRESS_CURSOR_DOWN , ElectrodeProgresoEvent);

// Declaramos el tipo de Evento para cuando el cursor está siendo soltado.

wxDECLARE_EVENT (wxEVT_ELECTRODE_PROGRESS_CURSOR_UP , ElectrodeProgresoEvent);

// Asi como el tipo de Evento para cuando desplazamos el cursor (slider).

wxDECLARE_EVENT (wxEVT_ELECTRODE_PROGRESS_SLIDER , ElectrodeProgresoEvent);

// Y un evento para cuando se termina de reposicionar el Cursor (slider) y el valor ha cambiado.

wxDECLARE_EVENT (wxEVT_ELECTRODE_PROGRESS_CHANGUED , ElectrodeProgresoEvent);

// Ahora una nueva entrada para la tabla de eventos en caso de usar este mecanismo para cada tipo de Evento.

#define EVT_ELECTRODE_PROGRESS_CURSOR_DOWN(ID , Manejador) 	wx__DECLARE_EVT1 (wxEVT_ELECTRODE_PROGRESS_CURSOR_DOWN , ID , &Manejador)
#define EVT_ELECTRODE_PROGRESS_CURSOR_UP(ID , Manejador) 	wx__DECLARE_EVT1 (wxEVT_ELECTRODE_PROGRESS_CURSOR_UP , ID , &Manejador)
#define EVT_ELECTRODE_PROGRESS_SLIDER(ID , Manejador) 		wx__DECLARE_EVT1 (wxEVT_ELECTRODE_PROGRESS_SLIDER , ID , &Manejador)
#define EVT_ELECTRODE_PROGRESS_CHANGUED(ID , Manejador) 	wx__DECLARE_EVT1 (wxEVT_ELECTRODE_PROGRESS_CHANGUED , ID , &Manejador)

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/// @}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ELECTRODEMP_PROGRESO_H
