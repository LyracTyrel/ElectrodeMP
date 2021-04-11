
/** @mainpage Documentaci&oacute;n de wxThumbFrame

/**
/// ******************************************************************************************
 * @author : Moreno Acero Jos&eacute; Ra&uacute;l               ******
 * @date : 08/06/2016                                           ******
 * Expanci&oacute;n de wxWidgets para el Puerto MSW con los          ******
 * nuevos controles anexados desde windows 7 para                    ******
 * manejar diversas acciones desde la vista previa (thumbnail)       ******
 * de las aplicaciones , tales como barras de progreso , botones     ******
 * , etc , ampliando las posibilidades de las aplicaciones hechas    ******
/// ******************************************************************************************
*/

#ifndef DECLARATION_THUMBNAIL
#define DECLARATION_THUMBNAIL

// Comenzamos incluyendo los componentes mas importantes de wxWidgets

#include <wx\wx.h>

// Incluiremos por separado el manejador de lista de imagenes

#include <wx\imaglist.h>

// Incluiremos la cabecera especifica API win32 para c++ que permite manejar
// la interfaz ITaskBar 4 con la que se desarrollaran los nuevos controles.

#include <shobjidl.h>

// Incluimos tambien el manejador de wide-strings

#include <cwchar>

// Contenedores de lista iniciable , arreglo y vector

#include <initializer_list>
#include <array>
#include <vector>

// Y las utilidades de c++

#include <utility>

// -------------------------------  Namespace Extension  ---------------------------------------

// Contendremos todos los componentes aqui declarados en un espacio de nombres especial para su seguridad dentro
// de lo que tiene wxWidgets.

/** Espacio de nombres de la extensi&oacute;n de wxWidgets.
/// Dentro se tienen todas las clases que conforman las nuevas utilerias para wxWidgets.
 */

namespace jrma_extension {

// -------------------------------  Clase principal de Extension  ------------------------------

// Lo primero que haremos sera declarar la clase que conforma la base para montar las extensiones de thumbnail.

/** Clase que conforma la base para montar las extensiones de thumbnail.
 */

class wxThumbFrame; // Clase base

// -------------------------------  Control : wxThumbButton ------------------------------------

// Este control sera una nueva clase incorporada que nos permitira manejar la informaci&oacute;n mas relevante de
// los controles THUMBBUTTON de win32 , no deriva directamente de wxWindow por que no es un contenedor
// de componentes primario , sino simplemente una estructura de datos que agrupa la informaci&oacute;n de este boton
// pero puede ser usado como un wxObject

/** @page ThumbButton Referencia de ThumbButton

/** Control para los toolbar del thumbnail.
/// Este control sera una nueva clase incorporada que nos permitira manejar la informaci&oacute;n mas relevante de
/// los controles THUMBBUTTON de win32 , no deriva directamente de wxWindow por que no es un contenedor
/// de componentes primario , sino simplemente una estructura de datos que agrupa la informaci&oacute;n de este boton
/// pero puede ser usado como un wxObject
 */

// Declaramos este componente

class wxThumbButton : public wxObject {
	
	// Declaramos el accesorio de clase dinamica para proporcionar informaci&oacute;n de clases en tiempo de ejecuci&oacute;n
	
	wxDECLARE_DYNAMIC_CLASS (wxThumbButton);
	
	/** @section Enumeracion Enum wxThumbButton::
	 */
	
	// Definimos algunos tipos utilizados de manera publica en un THUMBBUTTON
	
	public :
		
		/** Valores de mascara de boton aceptados.
		 */
		
		// Definiremos la enumeraci&oacute;n de clase para las mascaras que utiliza este componente
		
		enum {
			
			// Si el bitmap es valido se utiliza el siguiente valor en la mascara
			
			VALID_BITMAP = THUMBBUTTONMASK::THB_BITMAP , /**< Si el bitmap es valido se utiliza el siguiente valor en la mascara */
			
			// Si el icono es valido se usa este otro
			
			VALID_ICON = THUMBBUTTONMASK::THB_ICON , /**< Si el icono es valido se usa este otro */
			
			// Si el tooltip esta activado en este boton , se utiliza este
			
			VALID_TOOLTIP = THUMBBUTTONMASK::THB_TOOLTIP , /**< Si el tooltip esta activado en este boton , se utiliza este */
			
			// Asi como si los estados son validos en este boton , se usa
			
			VALID_FLAGS = THUMBBUTTONMASK::THB_FLAGS , /**< Asi como si los estados son validos en este boton , se usa */
			
			// Valido OFF para apagar una bandera
			
			VALID_OFF = 0x00000000u /**< Valido OFF para apagar una bandera */
			
		};
		
		// Definiremos otra enumeraci&oacute;n para las banderas de estado
		
		/** Valores de bandera de estado del boton.
		*/
		
		enum {
			
			// Estado de boton habiliado
			
			FLAG_ENABLED = THUMBBUTTONFLAGS::THBF_ENABLED , /**< Estado de boton habiliado */
			
			// Estado de boton desabilitado
			
			FLAG_DISABLED = THUMBBUTTONFLAGS::THBF_DISABLED , /**< Estado de boton desabilitado */
			
			// Estado para que la barra de botones se cierre una vez presionado este boton
			
			FLAG_DISMISSON = THUMBBUTTONFLAGS::THBF_DISMISSONCLICK , /**< Estado para que la barra de botones se cierre una vez presionado este boton */
			
			// Estado para no dibujar el borde del boton
			
			FLAG_NOBORDER = THUMBBUTTONFLAGS::THBF_NOBACKGROUND , /**< Estado para no dibujar el borde del boton */
			
			// Estado para que no sea mostrado el boton
			
			FLAG_HIDDEN = THUMBBUTTONFLAGS::THBF_HIDDEN , /**< Estado para que no sea mostrado el boton */
			
			// Estado para que el boton no sea interactivo con el usuario , es decir que no
			// se vea el efecto de presionado , solamente se quede estatico
			
			FLAG_NOINTERACTIVE = THUMBBUTTONFLAGS::THBF_NONINTERACTIVE , /**< Estado para que el boton no sea interactivo con el usuario , es decir que no se vea el efecto de presionado , solamente se quede estatico */
			
			// Estado vacio
			
			FLAG_OFF = 0x00000000u /**< Estado vacio */
			
		};
		
		// Macros-definiciones para las mascaras y los estados de este control
		
		#define wxTHB_MASK_BITMAP 			wxThumbButton::VALID_BITMAP
		#define wxTHB_MASK_ICON 			wxThumbButton::VALID_ICON
		#define wxTHB_MASK_TOOLTIP 			wxThumbButton::VALID_TOOLTIP
		#define wxTHB_MASK_FLAGS	 		wxThumbButton::VALID_FLAGS
		#define wxTHB_MASK_EMPTY			wxThumbButton::VALID_OFF
		
		#define wxTHB_FLAG_ENABLE 			wxThumbButton::FLAG_ENABLED
		#define wxTHB_FLAG_DISABLE 			wxThumbButton::FLAG_DISABLED
		#define wxTHB_FLAG_DISMISSON 		wxThumbButton::FLAG_DISMISSON
		#define wxTHB_FLAG_NOBORDER	 		wxThumbButton::FLAG_NOBORDER
		#define wxTHB_FLAG_HIDDEN 			wxThumbButton::FLAG_HIDDEN
		#define wxTHB_FLAG_NOINTERACTIV 	wxThumbButton::FLAG_NOINTERACTIVE
		#define wxTHB_FLAG_EMPTY	 		wxThumbButton::FLAG_OFF
		
		// Ahora definimos una mascara por default y un estado por defecto para el boton si no se especifica alguno
		
		/** Valores constantes de estado y mascara que pueden usarse en el boton
		 */
		
		static constexpr const int DEFAULT_MASCARA = (wxTHB_MASK_BITMAP | wxTHB_MASK_ICON | wxTHB_MASK_TOOLTIP | wxTHB_MASK_FLAGS);
		static constexpr const int DEFAULT_ESTADO = wxTHB_FLAG_ENABLE;
	
	// Declaramos ahora los componentes internos de un THUMBBUTTON
	/*
	private :
		
		// Tendremos una estructura propia con los elementos de la estructura THUMBBUTTON que deriva de una referencia a los
		// datos del objeto del que deriva esta clase
		
		struct SThumb {
			
			// Mascara para conocer el comportamiendo del boton actual
			
			int Mascara;
			
			// Identificador del boton
			
			UINT Identificador;
			
			// Indice de un mapa de bits (solo si la mascara lo indica)
			
			UINT IndiceBitmap;
			
			// Manejador de un icono actual del sistema para este boton
			
			HICON Icono;
			
			// Wide string con el Tooltip para el boton (Opcional)
			
			WCHAR ToolTip [260];
			
			// Banderas de estados para el boton
			
			int Estado;
			
		};
		
		// Union que permite la conversi&oacute;n entre THUMBBUTTON y nuestra estructura
		
		union UThumb {
			
			// Estructura THUMBBUTTON
			
			THUMBBUTTON Internal;
			
			// Nuestra estructura del Thumb button
			
			SThumb External;
			
			// Constructor a partir de nuestra estructura
			
			explicit UThumb (const SThumb & Valores) noexcept : External (Valores) {}
			
			// Constructor con la estructura de win32
			
			explicit UThumb (const THUMBBUTTON & Valores) noexcept : Internal (Valores) {}
			
			// Obtencion de los componentes a partir de operadores de conversi&oacute;n
			
			__attribute__ ((__always_inline__)) inline operator SThumb () const noexcept {
				
				// Devolvemos nuestra estructura
				
				return External;
				
			}
			__attribute__ ((__always_inline__)) inline operator THUMBBUTTON () const noexcept {
				
				// Devolvemos la estructura nativa de win32
				
				return Internal;
				
			}
			
		};
		
		// Ahora llega el momento de declarar la clase referenciadora para este control , debido a que wxObject utiliza
		// un modelo de referencias basado en un contador especifico , usamos la clase base wxObjectRefData para crear nuestra
		// propia clase de referencia hacia el objeto de wxThumbButton
		
		class wxThumbButtonReference : public wxObjectRefData {
			
			private :
				
				// Tenemos en primer lugar los datos del ThumButton encapsulados en la estructura respectiva
				
				UThumb Datos; // Toda la informaci&oacute;n esta contenida en esta union
				
				// Asi como un Icono temporal utilizado por este boton , por defecto esta vacio
				
				wxIcon IconoTemporal = wxNullIcon;
			
			public :
				
				// Tenemos la clase del control como una clase amiga de esta subclase
				
				friend class wxThumbButton;
				
				// Ahora tenemos un constructor por defecto que inicia la informaci&oacute;n en 0 de
				// los datos contenidos aqui mismo
				
				wxThumbButtonReference () : 
					Datos (SThumb { wxTHB_MASK_EMPTY , 0u , 0u , wxNullIcon.GetHICON () , { L'\0' } , wxTHB_FLAG_EMPTY }) , IconoTemporal (wxNullIcon) {}
				
				// Tendremos otro constructor con todos los datos para la estructura de un boton THUMBUTTON
				
				explicit wxThumbButtonReference (const SThumb & DatosIniciales , const wxIcon & IconoInicial = wxNullIcon) : 
					Datos (DatosIniciales) , IconoTemporal (IconoInicial) {}
				
				// Constructor de copia
				
				wxThumbButtonReference (const wxThumbButtonReference & Copia) : Datos (Copia.Datos.External) {
					
					// Simplemente copia el icono temporal
					
					IconoTemporal = Copia.IconoTemporal;
					
				}
				
				// Accesores de dato temporal , simplemente sobrecargamos al operador -> para obtener acceso a la estructura external de los datos
				
				__attribute__ ((__always_inline__)) inline SThumb * operator -> () {
					
					// Devolvemos el puntero a la estructura external en los datos de aqui
					
					return &Datos.External;
					
				}
				__attribute__ ((__always_inline__)) inline const SThumb * operator -> () const {
					
					// Lo mismo pero para la versi&oacute;n constante de la referencia
					
					return &Datos.External;
					
				}
			
		};
	*/
	
	// Ahora los componentes publicos al acceso de los usuarios
	
	public :
		
		/** @section Constructor wxThumbButton::Constructor
		 */
		
		// En primer lugar el constructor de la estructura , conlleva una estructura similar a la que
		// tiene un control de wxWidgets pero sin el elemento parent.
		
		// Tenemos el que utiliza un indice de mapa de bits , el que utiliza un
		// icono preconstruido , asi como el que no utiliza ninguno de estos y tiene
		// elementos como Identificador y Estados o solamente el Identificador y un estado por defecto
		
		wxThumbButton () {} // Default (Vacio)
		
		wxThumbButton (wxThumbButton * Referencia) { m_refData = Referencia->m_refData; } // Constructor de Referencia
		wxThumbButton (const wxThumbButton & Copia) = default; // Constructor de Copia
		wxThumbButton (wxThumbButton && Mover) = default; // Constructor de Movimiento
		
		wxThumbButton & operator = (wxThumbButton * Referencia) { m_refData = Referencia->m_refData; } // Referencia Asignable
		wxThumbButton & operator = (const wxThumbButton & Copia) = default; // Copia Asignable
		wxThumbButton & operator = (wxThumbButton && Mover) = default; // Mover Asignable
		
		explicit wxThumbButton (wxWindowID Identificador , bool Estados = true , int EstadoBandera = DEFAULT_ESTADO);
		explicit wxThumbButton (wxWindowID Identificador , const wxString & ToolTip , unsigned int IndiceBitmap , bool Estados = true , int EstadoBandera = DEFAULT_ESTADO);
		explicit wxThumbButton (wxWindowID Identificador , const wxString & ToolTip , const wxIcon & Icono , bool Estados = true , int EstadoBandera = DEFAULT_ESTADO);
		
		// Metodos accesor para cada dato que se le pueda modificar al boton , en el caso de los Setters se llamara a UnShare() para evitar que se comparta la informaci&oacute;n
		// y ademas usaremos el elemento m_refData para hacer todas las operaciones sobre los datos de este control.
		
		// Cambiar el ID
		
		void SetID (wxWindowID IdentificadorNuevo) {
			
			// Cambiamos el ID de la estructura por el nuevo del parametro
			
			UnShare() , (*dynamic_cast <wxThumbButtonReference *> (m_refData))->Identificador = static_cast <UINT> (IdentificadorNuevo);
			
		}
		__attribute__ ((__always_inline__)) inline wxWindowID GetID () const {
			
			// Devolvemos el Identificador
			
			return wxWindowID ((*dynamic_cast <wxThumbButtonReference *> (m_refData))->Identificador);
			
		}
		
		// Cambiar la mascara del boton
		
		void SetMascara (int MascaraNueva) {
			
			// Cambiamos el valor de la mascara por completo
			
			UnShare() , (*dynamic_cast <wxThumbButtonReference *> (m_refData))->Mascara = MascaraNueva;
			
		}
		__attribute__ ((__always_inline__)) inline int GetMascara () const {
			
			// Devolvemos la mascara del boton
			
			return (*dynamic_cast <wxThumbButtonReference *> (m_refData))->Mascara;
			
		}
		
		// Cambiar el indice del mapa de bits a utilizar
		
		void SetBitmap (unsigned int IndiceBitmapNuevo) {
			
			// Cambiamos el indice a usar del mapa de bits
			
			UnShare() , (*dynamic_cast <wxThumbButtonReference *> (m_refData))->IndiceBitmap = IndiceBitmapNuevo;
			
		}
		
		/** @name Grupo Normalizado
		* @{
		* Normal text.
		* \par User defined paragraph:
		* Contents of the paragraph.
		*
		* \par
		* New paragraph under the same heading.
		*
		* \note
		* This note consists of two paragraphs.
		* This is the first paragraph.
		*
		* \par
		* And this is the second paragraph.
		*
		*/
		
		/** 
		* @brief Funcion sensilla.
		* 
		* Funcion que devuelve un bitmap.
		* @param Numero : Indice del numero a buscar.
		* @return <b> unsigned int : </b> numero constante.
		**/
		
		inline unsigned int GetBitmap (int Numero) const {
			
			// Devolvemos el indice del mapa utilizado
			
			return (*dynamic_cast <wxThumbButtonReference *> (m_refData))->IndiceBitmap;
			
		}
		
		/** @}
		**/
		
		// Cambiar el icono a usar en el boton
		
		void SetIcono (const wxIcon & IconoNuevo) {
			
			// Ponemos el nuevo icono al boton , cambiando el icono temporal
			
			UnShare() , (*dynamic_cast <wxThumbButtonReference *> (m_refData))->Icono = (dynamic_cast <wxThumbButtonReference *> (m_refData)->IconoTemporal = IconoNuevo).GetHICON ();
			
		}
		__attribute__ ((__always_inline__)) inline wxIcon GetIcono () const {
			
			// Devolvemos una copia del icono temporal utilizado
			
			return dynamic_cast <wxThumbButtonReference *> (m_refData)->IconoTemporal;
			
		}
		
		// Colocamos el texto a usar como tooltip en la pantalla
		
		void SetToolTip (const wxString & ToolTipNuevo) {
			
			// Usaremos la funci&oacute;n wcscpy de wchar para copiar la representaci&oacute;n en wide char de la cadena del parametro
			// a la cadena del boton interno
			
			UnShare() , std::wcscpy (reinterpret_cast <wchar_t *> ((*dynamic_cast <wxThumbButtonReference *> (m_refData))->ToolTip) , ToolTipNuevo.wc_str ());
			
		}
		__attribute__ ((__always_inline__)) inline wxString GetToolTip () const {
			
			// Devolvemos la representaci&oacute;n en wxString de la cadena del ToolTip del boton actual
			
			return wxString (reinterpret_cast <wchar_t *> ((*dynamic_cast <wxThumbButtonReference *> (m_refData))->ToolTip));
			
		}
		
		// Cambiar el estado actual del boton
		
		void SetEstados (int EstadoBanderaNuevo) {
			
			// Colocaremos el nuevo estado a la variable respectiva en el boton interno
			
			(*dynamic_cast <wxThumbButtonReference *> (m_refData))->Estado = EstadoBanderaNuevo;
			
		}
		__attribute__ ((__always_inline__)) inline int GetEstados () const {
			
			// Devolvemos los estados que tiene este boton
			
			return (*dynamic_cast <wxThumbButtonReference *> (m_refData))->Estado;
			
		}
		
		// Validadores y actualizadores de mascara para saber de manera directa el comportamiento de este boton
		
		__attribute__ ((__always_inline__)) inline bool IsValid () const {
			
			// Si fue creada la referencia , entonces es valido
			
			return (m_refData != NULL);
			
		}
		void SetValidBitmap (bool Valido = true) {
			
			// Colocamos la mascara en estado de VALID_BITMAP
			
			SetMascara ((GetMascara () & ~wxTHB_MASK_BITMAP) | (Valido ? wxTHB_MASK_BITMAP : wxTHB_MASK_EMPTY));
			
		}
		void SetValidIcon (bool Valido = true) {
			
			// Colocamos la mascara en estado de VALID_ICON
			
			SetMascara ((GetMascara () & ~wxTHB_MASK_ICON) | (Valido ? wxTHB_MASK_ICON : wxTHB_MASK_EMPTY));
			
		}
		void SetValidToolTip (bool Valido = true) {
			
			// Colocamos la mascara en estado de VALID_TOOLTIP
			
			SetMascara ((GetMascara () & ~wxTHB_MASK_TOOLTIP) | (Valido ? wxTHB_MASK_TOOLTIP : wxTHB_MASK_EMPTY));
			
		}
		void SetValidFlags (bool Valido = true) {
			
			// Colocamos la mascara en estado de VALID_FLAGS
			
			SetMascara ((GetMascara () & ~wxTHB_MASK_FLAGS) | (Valido ? wxTHB_MASK_FLAGS : wxTHB_MASK_EMPTY));
			
		}
		__attribute__ ((__always_inline__)) inline bool IsValidBitmap () const {
			
			// Comprobamos si la mascara cumple con el valor de VALID_BITMAP
			
			return (bool)(GetMascara () & wxTHB_MASK_BITMAP);
			
		}
		__attribute__ ((__always_inline__)) inline bool IsValidIcon () const {
			
			// Comprobamos si la mascara cumple con el valor de VALID_ICON
			
			return (bool)(GetMascara () & wxTHB_MASK_ICON);
			
		}
		__attribute__ ((__always_inline__)) inline bool IsValidToolTip () const {
			
			// Comprobamos si la mascara cumple con el valor de VALID_TOOLTIP
			
			return (bool)(GetMascara () & wxTHB_MASK_TOOLTIP);
			
		}
		__attribute__ ((__always_inline__)) inline bool IsValidFlags () const {
			
			// Comprobamos si la mascara cumple con el valor de VALID_FLAGS
			
			return (bool)(GetMascara () & wxTHB_MASK_FLAGS);
			
		}
		
		// Conversi&oacute;n al THUMBBUTTON nativo de win32
		
		__attribute__ ((__always_inline__)) inline THUMBBUTTON GetTHUMBBUTTON () const {
			
			// Devolvemos el elemento Internal de los datos de la referencia
			
			return (*dynamic_cast <wxThumbButtonReference *> (m_refData)).Datos.Internal;
			
		}
		
		// Sobreescribimos el destructor de esta clase
		
		virtual ~ wxThumbButton () override {} // Vacio
	
	// Datos protegidos sobreescritos de wxObject
	
	protected :
		
		// Tenemos aqui wxObjectRefData * m_refData
		
		// Para gestionar nuestra propia clase referenciadora usaremos las siguientes funciones
		
		virtual wxObjectRefData * CreateRefData () const override {
			
			// Devolvemos una nueva instancia de la clase referenciadora con los datos vacios
			
			return dynamic_cast <wxObjectRefData *> (new wxThumbButtonReference ());
			
		}
		
		virtual wxObjectRefData * CloneRefData (const wxObjectRefData * Datos) const override {
			
			// Devolvemos una nueva instancia de la clase manejadora de referencias
			// usando los datos del parametro para construir su copia
			
			return dynamic_cast <wxObjectRefData *> (new wxThumbButtonReference (*dynamic_cast <const wxThumbButtonReference *> (Datos)));
			
		}
	
};

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------  Evento : wxThumbEvent  ------------------------------------------------

// Clase especifica para almacenar los datos de un evento ocacionado por los THUMBBUTTONS , es conveniente
// tenerla presente para la programaci&oacute;n de los metodos manejadores de los eventos de los THUMBBUTTONS

class wxThumbEvent : public wxEvent {
	
	// Elementos especificos de este evento solamente para informar sobre el estado y mascara del boton actual
	
	private :
		
		// Mascara del boton
		
		int MascaraActual = wxThumbButton::DEFAULT_MASCARA;
		
		// Estado del boton
		
		int EstadoActual = wxThumbButton::DEFAULT_ESTADO;
	
	// Como elementos publicos tenemos
	
	public :
		
		// Constructor vacio y constructor con parametros de un wxThumbButton
		
		explicit wxThumbEvent () : wxEvent (wxID_ANY) {}
		explicit wxThumbEvent (int Identificador , int Mascara = wxThumbButton::DEFAULT_MASCARA , int Estado = wxThumbButton::DEFAULT_ESTADO);
		
		// Ahora los accesors para los dos datos especificos de este evento
		
		void SetMascara (int MascaraNueva) {
			
			// Cambiamos la mascara usada en el boton
			
			MascaraActual = MascaraNueva;
			
		}
		__attribute__ ((__always_inline__)) inline int GetMascara () const {
			
			// Devolvemos la mascara actual registrada en el evento
			
			return MascaraActual;
			
		}
		void SetEstados (int EstadoNuevo) {
			
			// Cambiamos el estado actual en el evento
			
			EstadoActual = EstadoNuevo;
			
		}
		__attribute__ ((__always_inline__)) inline int GetEstados () const {
			
			// Devolvemos el estado actual
			
			return EstadoActual;
			
		}
		
		// Sobreescribiremos tambien el metodo Clone para duplicar una instancia de este evento
		
		virtual wxEvent * Clone () const override {
			
			return dynamic_cast <wxEvent *> (new wxThumbEvent (*this));
			
		}
	
};

// Declaramos el evento para tenerlo registrado en wxWidgets como un evento accesible

wxDECLARE_EVENT (wxEVT_THUMB_BUTTON , wxThumbEvent); // wxEventType wxEVT_THUMB_BUTTTON

// Ahora una nueva entrada para la tabla de eventos en caso de usar este mecanismo para
// asociar los metodos con los eventos de este tipo

#define EVT_THUMB_BUTTON(ID , Manejador) wx__DECLARE_EVT1 (wxEVT_THUMB_BUTTON , ID , &Manejador)

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------  Control : wxThumbProgress  ------------------------------------------------

// Es un elemento con la funcionalidad de una barra de progreso o wxGauge pero ubicada en la zona de thumbnail de la aplicaci&oacute;n , esta nos permitira
// mostrar el progreso de algun procedimiento que se establezca sin necesidad de abrir el wxFrame principal. Se conforma de diferentes estados
// que cambian el color de la barra para indicar diferentes circunstancias.

// Declaramos este otro control

class wxThumbProgress : public wxObject {
	
	// Usaremos la declaraci&oacute;n de clase dinamica para este componente al igual que se realizo para el wxThumbButton
	
	wxDECLARE_DYNAMIC_CLASS (wxThumbButton);
	
	// Ahora tenemos publicos los estados de los que se puede conformar esta barra de progreso en el thumbnail del wxFrame
	
	public :
		
		// Enumeramos los estados que nos proporciona win32
		
		enum {
			
			// Estado de Sin Progreso , es el estado por defecto que no muestra la barra de progreso y por tanto es bastante util cuando se termina un progreso
			
			FLAG_NOPROGRESS = TBPF_NOPROGRESS ,
			
			// Estado de Indeterminado , este estado provoca que la barra no se llene pero que adquiera un estado de actividad sin indicar que tan lleno
			// esta el proceso
			
			FLAG_INDETERMINATE = TBPF_INDETERMINATE ,
			
			// Estado de Normal , este estado muestra la barra de progreso que se va llenando conforme a los crecimientos en valor establecidos y
			// muestra en general que el proceso esta perfectamente calculado
			
			FLAG_NORMAL = TBPF_NORMAL ,
			
			// Estado de Error , este estado cambia el color de la barra a Rojo , dando a entender que el proceso esta bloqueado o hubo algun tipo de problema
			
			FLAG_ERROR = TBPF_ERROR ,
			
			// Estado de Pause = este estado cambia el color a Amarillo y nos da a entender que el proceso se tuvo que esperar a que el usuario respondiera
			// o simplemente esta calculando algo y luego volvera a resumirse
			
			FLAG_PAUSE = TBPF_PAUSED
			
		};
		
		// Las macros para cada estado a nivel global se tienen a continuaci&oacute;n
		
		#define wxTHP_FLAG_NOPROGRESS 		wxThumbProgress::FLAG_NOPROGRESS
		#define wxTHP_FLAG_INDETERMINATE 	wxThumbProgress::FLAG_INDETERMINATE
		#define wxTHP_FLAG_NORMAL 			wxThumbProgress::FLAG_NORMAL
		#define wxTHP_FLAG_ERROR 			wxThumbProgress::FLAG_ERROR
		#define wxTHP_FLAG_PAUSE 			wxThumbProgress::FLAG_PAUSE
		
		// El estado constante por defecto para todos los progress bar de thumbnail es el siguiente
		
		static constexpr const int DEFAULT_ESTADO = wxTHP_FLAG_NORMAL; // Con barra de progreso por defecto
	
	// Ahora como miembro privado tenemos a la declaraci&oacute;n y definici&oacute;n de la clase referenciadora de este objeto
	
	private :
		
		// Declaramos la clase que nos permitira relacionar los objetos internos con los datos que tendra el componente
		// real de wxThumbProgress
		
		class wxThumbProgressReference : public wxObjectRefData {
			
			// Dentro de los miembros privados tenemos encapsulados la informaci&oacute;n mas relevante de un progress bar de esta naturaleza
			
			private :
				
				// Comenzando por el importantisimo manejador de la ventana thumb a la que esta atendiendo este progress bar
				
				wxThumbFrame * Manejador;
				
				// Segido del valor actual del estado que tiene en este momento el progress bar
				
				int Estado;
				
				// El valor maximo y el valor actual de lleno en la barra de progreso
				
				ULONGLONG Maximo , Completado;
				
			public :
				
				// Tenemos la clase wxThumbProgress como amiga de su propia referencia interna
				
				friend class wxThumbProgress;
				
				// Ahora tenemos un constructor por defecto que llena la informaci&oacute;n con 0
				
				wxThumbProgressReference () noexcept : Manejador (nullptr) , Estado (0) , Maximo (static_cast <ULONGLONG> (0ull)) , Completado (static_cast <ULONGLONG> (0ull)) {}
				
				// Tendremos un segundo cosntructor con los datos iniciales del progress bar
				
				explicit wxThumbProgressReference (wxThumbFrame * ManejadorInicial , int EstadoInicial , ULONGLONG MaximoInicial , ULONGLONG CompletadoInicial) noexcept :
					Manejador (ManejadorInicial) , Estado (EstadoInicial) , Maximo (MaximoInicial) , Completado (CompletadoInicial) {}
				
				// Constructor de copia eliminado
				
				wxThumbProgressReference (const wxThumbProgressReference & Copia) = delete;
			
		};
	
	// Ahora tenemos los elementos publicos de este componente
	
	public :
		
		// Comenzando por definir los constructores y asignadores de esta barra de progreso
		
		wxThumbProgress () {} // Default (Vacio)
		
		wxThumbProgress (wxThumbProgress * Referencia) { m_refData = Referencia->m_refData; } // Constructor de Referencia
		wxThumbProgress (const wxThumbProgress & Copia) = default; // Constructor de Copia
		wxThumbProgress (wxThumbProgress && Mover) = default; // Constructor de Movimiento
		
		wxThumbProgress & operator = (wxThumbProgress * Referencia) { m_refData = Referencia->m_refData; } // Referencia Asignable
		wxThumbProgress & operator = (const wxThumbProgress & Copia) = default; // Copia Asignable
		wxThumbProgress & operator = (wxThumbProgress && Mover) = default; // Mover Asignable
		
		// Ahora el constructor especializado que crea una nueva barra de progreso para una ventana de alto nivel especificada , con un estado inicial indicado
		// y con un valor maximo y completado actual en la barra de progreso
		
		explicit wxThumbProgress (wxThumbFrame * Ventana , int Estado = DEFAULT_ESTADO , unsigned long long int VMaximo = 100ull , unsigned long long int VCompletado = 0ull);
		
		// Ahora definiremos los metodos accesores para cada dato en particular que tiene esta barra y que se deben poder modificar
		
		// Cambiar de ventana y obtener la ventana actualmente asignada para monitorear
		
		void SetVentana (wxThumbFrame * VentanaNueva) {
			
			// Colocamos la nueva ventana como manejador en el dato interno de este elemento
			
			UnShare() , dynamic_cast <wxThumbProgressReference *> (m_refData)->Manejador = VentanaNueva;
			
		}
		__attribute__ ((__always_inline__)) inline wxThumbFrame * GetVentana () {
			
			// Devolvemos la ventana actualmente atendida
			
			return dynamic_cast <wxThumbProgressReference *> (m_refData)->Manejador;
			
		}
		__attribute__ ((__always_inline__)) inline const wxThumbFrame * GetVentana () const {
			
			// Devolvemos la ventana de manera constante
			
			return dynamic_cast <const wxThumbProgressReference *> (m_refData)->Manejador;
			
		}
		
		// Modificar el estado actual de esta barra de progreso
		
		void SetEstado (int EstadoNuevo) {
			
			// Colocamos este estado en los datos internos
			
			UnShare() , dynamic_cast <wxThumbProgressReference *> (m_refData)->Estado = EstadoNuevo;
			
		}
		__attribute__ ((__always_inline__)) inline int GetEstado () const {
			
			// Devolvemos el estado actual de la barra
			
			return dynamic_cast <wxThumbProgressReference *> (m_refData)->Estado;
			
		}
		
		// Modificar el valor maximo al que llega la barra cuando se completa
		
		void SetMaximo (unsigned long long int MaximoNuevo) {
			
			// Cambiamos el valor Maximo en el dato interno
			
			UnShare() , dynamic_cast <wxThumbProgressReference *> (m_refData)->Maximo = MaximoNuevo;
			
		}
		__attribute__ ((__always_inline__)) inline unsigned long long int GetMaximo () const {
			
			// Devolvemos este valor maximo asociado en este momento
			
			dynamic_cast <wxThumbProgressReference *> (m_refData)->Maximo;
			
		}
		
		// Modificar el valor actual de completado en el que se encuentra la barra
		
		void SetCompletado (unsigned long long int CompletadoNuevo) {
			
			// Cambiamos el valor de completado en la barra
			
			UnShare () , dynamic_cast <wxThumbProgressReference *> (m_refData)->Completado = CompletadoNuevo;
			
		}
		__attribute__ ((__always_inline__)) inline unsigned long long int GetCompletado () const {
			
			// Devolvemos el valor de lleno que tiene la barra
			
			return dynamic_cast <wxThumbProgressReference *> (m_refData)->Completado;
			
		}
		
		// Actualizadores , de ventana , de estado y de valores , con estos metodos se veran reflejados los cambios en la
		// ventana asociada al momento de su invocaci&oacute;n
		
		bool RefreshVentana ();
		bool RefreshEstado ();
		bool RefreshValores ();
	
	// Los miembros protegidos se crean por convensi&oacute;n para desarrollar el codigo necesario para crear instancias de referencia a objetos de esta clase
	
	protected :
		
		// Unicamente se implementa el metodo sobreescrito para crear una nueva instancia vacia de los datos internos de esta clase
		
		virtual wxObjectRefData * CreateRefData () const override {
			
			// Devolvemos una nueva instancia de la clase referenciadora con los datos vacios
			
			return dynamic_cast <wxObjectRefData *> (new wxThumbProgressReference ());
			
		}
	
};

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------  Window : wxThumbFrame  ------------------------------------------------

// Una especializaci&oacute;n en el wxFrame que tiene un toolbar thumbnail y permite manejar todos los componentes definidos anterormente con el fin de darle
// una mejor presentaci&oacute;n a la aplicaci&oacute;n y proveer de estas utiles herramientas para su actualizaci&oacute;n.

class wxThumbFrame : public wxFrame {
	
	// Como miembro privado tenemos el importante ITaskBarList que es el que nos permitira anexar una barra de botones al
	// thumbnail del frame actual entre muchas otras cosas con el resto de componentes que conforman a la zona de barra de
	// tareas.
	
	private :
		
		// Taskbar list contenedor de los botones THUMBBUTTON
		
		ITaskbarList4 * Herramienta = NULL;
		
		// Declaramos las funciones que tienen permiso de esta barra en el constrol wxThumbProgress
		
		friend bool wxThumbProgress::RefreshVentana ();
		friend bool wxThumbProgress::RefreshEstado ();
		friend bool wxThumbProgress::RefreshValores ();
		
		// Ahora un vector de elementos wxThumbButton que sera por medio de lo cual organizaremos
		// el contenido de la herramienta
		
		typedef std::vector <wxThumbButton> VectorThumb;
		
		// Declaramos este vector
		
		VectorThumb BotonesThumb;
		
		// Y la lista de imagenes a utilizar para mostrar cada ThumButton
		
		wxImageList * ListaIconos = NULL;
	
	// Elementos publicos de este Frame
	
	public :
		
		// Tenemos tamaños fijos para las imagenes que se pueden usar de icono en los ThumbButtons , estas son delimitadas por el sistema asi :
		
		static constexpr const int SizeIconX = 11;
		static constexpr const int SizeIconY = 12;
		
		// Tenemos otra constante muy importante , el numero maximo de wxThumbButtons que puede tener este Frame y determinado por win32
		
		static constexpr const unsigned int ThumbButtonMax = 7u; // Maximo = 7
		
		// Para manejar Bitmaps con mascaras de bitmaps , usamos una paridad wxBitmap , wxBitmap como se define a continuaci&oacute;n
		
		typedef std::pair <wxBitmap , wxBitmap> wxBitmapMaskIcon;
		
		// El primer constructor sera simplemente el constructor vacio por defecto
		
		wxThumbFrame () {} // Default
		
		// Este segundo constructor es el com&uacute;n y corriente que tenemos para crear la mayor parte
		// de esta estructura.
		
		explicit wxThumbFrame (wxWindow * Parental , wxWindowID Identificador , const wxString & Titulo , const wxPoint & Posicion = wxDefaultPosition , const wxSize & Size = wxDefaultSize ,
			long Estilo = wxDEFAULT_FRAME_STYLE , const wxString & Nombre = wxFrameNameStr);
		
		// Este tercero ya incluye lo que es el arreglo de wxThumbButtons preestablecidos que se quieren integrar al frame
		
		explicit wxThumbFrame (wxWindow * Parental , wxWindowID Identificador , const wxString & Titulo , 
			int Cantidad , const wxThumbButton Botones [] , 
			const wxPoint & Posicion = wxDefaultPosition , const wxSize & Size = wxDefaultSize , long Estilo = wxDEFAULT_FRAME_STYLE , const wxString & Nombre = wxFrameNameStr);
		
		// Anexar o Quitar los ThumbButtons al Thumbnail de este Frame , estas operaciones solamente son validas antes de que se muestre el wxFrame en su constructor
		
		// Comenzamos declarando los metodos para su anexo , diferentes sobrecargas para diferentes datos
		
		void AddThumbButton (wxThumbButton * Boton); // Referencia
		void AddThumbButton (const wxThumbButton & Boton); // Copia
		
		void AddThumbButton (wxWindowID Identificador , const wxBitmap & Icono = wxNullBitmap , const wxBitmap & MascaraIcono = wxNullBitmap , const wxString & ToolTip = wxEmptyString ,
			int Mascara = wxThumbButton::DEFAULT_MASCARA , int Estado = wxThumbButton::DEFAULT_ESTADO);
		
		void AddThumbButton (wxWindowID Identificador , unsigned int IndiceIcono = 0u , const wxString & ToolTip = wxEmptyString ,
			int Mascara = wxThumbButton::DEFAULT_MASCARA , int Estado = wxThumbButton::DEFAULT_ESTADO);
		
		void AddThumbButton (wxWindowID Identificador , const wxIcon & Icono = wxNullIcon , const wxString & ToolTip = wxEmptyString ,
			int Mascara = wxThumbButton::DEFAULT_MASCARA , int Estado = wxThumbButton::DEFAULT_ESTADO);
		
		// Para establecer una nueva instancia de la lista de Bitmaps a travez de una ya creada , podemos usar estos metodos
		
		void SetIconos (const wxImageList & Lista);
		void SetIconos (const std::initializer_list <wxBitmap> & Lista);
		void SetIconos (const std::initializer_list <wxBitmapMaskIcon> & Lista);
		void SetIconos (const std::initializer_list <wxIcon> & Lista);
		
		// Para anexar un icono mas a la lista , podemos usar lo siguiente
		
		size_t AddIcono (const wxBitmap & NuevoIcono);
		size_t AddIcono (const wxBitmapMaskIcon & NuevoIcono);
		size_t AddIcono (const wxIcon & NuevoIcono);
		
		// Para remover los iconos en cierto indice o todos ellos podemos usar esto
		
		// Removedor de Iconos , usando el indice especifico del icono a retirar de la lista
		
		__attribute__ ((__always_inline__)) inline bool RemoveIcono (size_t Indice) {
			
			// Quitamos el icono si la lista esta vacia , si el indice es valido y si la funci&oacute;n acepta remover el icono
			
			return ((ListaIconos != NULL) && (static_cast <int> (Indice) < ListaIconos->GetImageCount ()) && (ListaIconos->Remove (static_cast <int> (Indice))));
			
		}
		
		// Quitar todos los iconos de la lista
		
		__attribute__ ((__always_inline__)) inline bool RemoveIconos () {
			
			// Quitamos todos los iconos
			
			return ((ListaIconos != NULL) && (ListaIconos->RemoveAll ()));
			
		}
		
		// Y para consultarlos usariamos estos metodos inline
		
		__attribute__ ((__always_inline__)) inline wxBitmap GetIconoBitmap (size_t Indice) const {
			
			// Obtenemos el icono directamente de la lista de iconos actuales siempre y cuando el indice no supere el numero de iconos en esta lista
			// y no este vacia obviamente
			
			return (((ListaIconos != NULL) && (static_cast <int> (Indice) < ListaIconos->GetImageCount ())) ? ListaIconos->GetBitmap (static_cast <int> (Indice)) : wxNullBitmap);
			
		}
		__attribute__ ((__always_inline__)) inline wxIcon GetIcono (size_t Indice) const {
			
			// Obtenemos el icono directamente de la lista de iconos actuales siempre y cuando el indice no supere el numero de iconos en esta lista
			// y no este vacia obviamente
			
			return (((ListaIconos != NULL) && (static_cast <int> (Indice) < ListaIconos->GetImageCount ())) ? ListaIconos->GetIcon (static_cast <int> (Indice)) : wxNullIcon);
			
		}
		
		// Ahora para remover un boton o todos los botones del toolbar , estas operaciones tienen efecto hasta que el wxFrame vuelve a crearse
		
		bool RemoveThumbButton (size_t Indice);
		bool RemoveAllThumbButton () noexcept;
		
		// Ahora para acceder a los ThumbButtons actualmente anexados
		
		__attribute__ ((__always_inline__)) inline wxThumbButton & GetThumbButton (size_t Indice) {
			
			// Devolvemos el elemento en el vector de botones con el indice especificado
			
			return BotonesThumb[Indice];
			
		}
		__attribute__ ((__always_inline__)) inline const wxThumbButton & GetThumbButton (size_t Indice) const {
			
			// Versi&oacute;n constante
			
			return BotonesThumb[Indice];
			
		}
		
		// Para consultar los indices de ciertos ThumbButtons por medio de sus ID
		
		int GetThumbButtonIndex (wxWindowID Identificador) const;
		
		bool HaveThumbButton (wxWindowID Identificador) const;
		
		// Iniciadores del Toolbar
		
		// El primer metodo consiste en iniciar la ToolBar del wxFrame actual para que entre en acci&oacute;n y pueda mostrarse
		// en el Thumbnail de este frame con el conjunto de botones e imagenes establecido
		
		void InicializarThumbButtons ();
		
		// Metodo especifico para actualizar todos los ThumbButtons que hay actualmente en este Frame y que pueden ser
		// consultados con los metodos accesores de estos elementos
		
		void ActualizarThumbButtons ();
		
		// Sobreescribiendo al destructor del wxThumbFrame
		
		virtual ~ wxThumbFrame () override;
		
		// Mejorando el callback de WindowsProc de este Frame para adaptarlo al nuevo componente wxThumbButton
		
		virtual WXLRESULT MSWWindowProc (WXUINT Mensaje , WXWPARAM WParam , WXLPARAM LParam) override;
	
	// El unico miembro protegido es el generador del evento wxThumbButton con los parametros especificos de win32
	
	protected :
		
		// Clase manejadora del evento WM_COMMAND de los wxThumbButtons
		
		void SendEventThumbButton (WXWPARAM WParam , WXLPARAM LParam);
	
};

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Terminamos el espacio de nombres

} // jrma_extension

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif
