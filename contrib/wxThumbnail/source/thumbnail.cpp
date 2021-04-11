
// Definición formal para los metodos usados en las clases proporcionadas por el thumbnail

#include <wx/thumbnail.h>

// --------------------------  Espacio de Nombres Utilizado  --------------------------------------

// Utilizamos el espacio de nombres de extension

namespace jrma_extension {

// -------------------------  Constructores del wxThumbButton  ------------------------------------

// Implementaremos cada uno de los metodos y constructores del boton wxThumbButton , comenzando por dejar la implementación de la clase dinamica

wxIMPLEMENT_DYNAMIC_CLASS (wxThumbButton , wxObject);

// Definimos el constructor con los datos base para el wxThumbButton

wxThumbButton::wxThumbButton (wxWindowID Identificador , bool Estados , int EstadoBandera) {
	
	// Lo primero que haremos sera crear la instancia de la referencia de objeto con una nueva instancia de
	// la clase de wxThumbButtonReference
	
	m_refData = dynamic_cast <wxObjectRefData *> (new wxThumbButtonReference ());
	
	// Ahora que esta iniciada la referencia , podemos colocar los datos
	
	// Comenzando por colocar el ID que es necesario para el THUMBBUTTON
	
	(*dynamic_cast <wxThumbButtonReference *> (m_refData))->Identificador = static_cast <UINT> (Identificador);
	
	// Si los estados son TRUE , se asignan de lo contrario se dejan en default
	
	(*dynamic_cast <wxThumbButtonReference *> (m_refData))->Estado = (Estados ? EstadoBandera : DEFAULT_ESTADO);
	
	// Ahora la mascara se determina tomando el valor actual del booleano de estados y asignando la bandera VALID_FLAGS si es true
	
	(*dynamic_cast <wxThumbButtonReference *> (m_refData))->Mascara = wxTHB_MASK_FLAGS;
	
}

// El siguiente constructor incluye lo que es el ToolTip y el indice de bitmap , los demas datos se siguen construyendo por el
// primer constructor.

wxThumbButton::wxThumbButton (wxWindowID Identificador , const wxString & ToolTip , unsigned int IndiceBitmap , bool Estados , int EstadoBandera) :
	wxThumbButton (Identificador , Estados , EstadoBandera) {
	
	// Asignamos a la referencia el tooltip del THUMBBUTTON , pero esta vez asignando con la función de copia de wide-strings
	
	std::wcscpy (reinterpret_cast <wchar_t *> ((*dynamic_cast <wxThumbButtonReference *> (m_refData))->ToolTip) , ToolTip.wc_str ());
	
	// Ponemos tambien el indice del bitmap que se va a presentar en este Boton
	
	(*dynamic_cast <wxThumbButtonReference *> (m_refData))->IndiceBitmap = static_cast <UINT> (IndiceBitmap);
	
	// La mascara debera contener las banderas de VALID_TOOLTIP y VALID_BITMAP activadas en este caso , se anexa a lo que ya tenia usando |
	
	(*dynamic_cast <wxThumbButtonReference *> (m_refData))->Mascara = ((wxTHB_MASK_TOOLTIP | wxTHB_MASK_BITMAP) | GetMascara ());
	
}

// Este ultimo tiene la unica diferencia con el anterior de usar un Icono en lugar de un indice para un Bitmap

wxThumbButton::wxThumbButton (wxWindowID Identificador , const wxString & ToolTip , const wxIcon & Icono , bool Estados , int EstadoBandera) :
	wxThumbButton (Identificador , Estados , EstadoBandera) {
	
	// Asignamos a la referencia el tooltip del THUMBBUTTON
	
	std::wcscpy (reinterpret_cast <wchar_t *> ((*dynamic_cast <wxThumbButtonReference *> (m_refData))->ToolTip) , ToolTip.wc_str ());
	
	// Copiaremos una referencia al icono en los datos de referencia
	
	(*dynamic_cast <wxThumbButtonReference *> (m_refData)).IconoTemporal = Icono;
	
	// Ahora pondremos el manejador de este icono en los datos de la referencia del THUMBBUTTON
	
	(*dynamic_cast <wxThumbButtonReference *> (m_refData))->Icono = static_cast <HICON> ((*dynamic_cast <wxThumbButtonReference *> (m_refData)).IconoTemporal.GetHICON ());
	
	// La mascara debera contener las banderas de VALID_TOOLTIP y VALID_ICON activadas en este otro caso
	
	(*dynamic_cast <wxThumbButtonReference *> (m_refData))->Mascara = ((wxTHB_MASK_TOOLTIP | wxTHB_MASK_ICON) | GetMascara ());
	
}

// -------------------------  Metodos miembros del wxThumbProgress  ------------------------------

// Comenzamos por definir el objeto dinamico de la clase del wxThumbProgress

wxIMPLEMENT_DYNAMIC_CLASS (wxThumbProgress , wxObject);

// Ahora implementaremos el constructor explicito que se tiene para este objeto en particular

wxThumbProgress::wxThumbProgress (wxThumbFrame * Ventana , int Estado , unsigned long long int VMaximo , unsigned long long int VCompletado) {
	
	// Lo primero que tenemos que realizar es crear la instancia de los datos internos de referencia para este objeto
	
	m_refData = dynamic_cast <wxObjectRefData *> (new wxThumbProgressReference ());
	
	// Ahora pasaremos a colocar los datos que requiere esta instancia
	
	// Manejador de la ventana
	
	dynamic_cast <wxThumbProgressReference *> (m_refData)->Manejador = Ventana;
	
	// Estado actual de la barra
	
	dynamic_cast <wxThumbProgressReference *> (m_refData)->Estado = Estado;
	
	// Valores maximo y actual de la barra de progreso
	
	dynamic_cast <wxThumbProgressReference *> (m_refData)->Maximo = static_cast <ULONGLONG> (VMaximo);
	dynamic_cast <wxThumbProgressReference *> (m_refData)->Completado = static_cast <ULONGLONG> (VCompletado);
	
	// Ahora solamente hay que actualizar la herramienta interna de la ventana para que se pueda mostrar la barra de progreso
	
	RefreshVentana ();
	
}

// Ahora los actualizadores de datos que se encargan de asociar la información interna a la ventana manejadora por medio de la API de win32

// Actualizador de Ventana

bool wxThumbProgress::RefreshVentana () {
	
	// Lo primero que hay que hacer es obtener el manejador nativo de win32 para la ventana asociada
	
	HWND ManejadorNativo = GetVentana ()->GetHWND ();
	
	// Colocaremos ahora toda la información registrada para tener los cambios en la nueva ventana asociada
	// Usaremos la herramienta interna de la ventana para modificar sus valores
	
	ITaskbarList4 * Herramienta = GetVentana ()->Herramienta;
	
	// Colocaremos tanto el estado como los valores actuales de la barra de progreso y devolveremos el resultado
	// de estas operaciones
	
	return (RefreshEstado () && RefreshValores ());
	
}

// Actualizador de Estados

bool wxThumbProgress::RefreshEstado () {
	
	// Determinamos el manejador de la ventana actualmente asociada
	
	HWND ManejadorNativo = GetVentana ()->GetHWND ();
	
	// Usando este manejador podemos actualizar el estado de la barra de progreso actualmente mostrada para este wxThumbFrame y devolver el resultado
	// de la operación interna en la herramienta del Frame
	
	return SUCCEEDED (GetVentana ()->Herramienta->SetProgressState (ManejadorNativo , static_cast <TBPFLAG> (GetEstado ())));
	
}

// Actualizador de los valores

bool wxThumbProgress::RefreshValores () {
	
	// Obtenemos primero el manejador de la ventana actual
	
	HWND ManejadorNativo = GetVentana ()->GetHWND ();
	
	// Ahora podemos actualizar los valores de la propia barra de progreso usando las medidas internas de los datos y la herramienta interna del propio
	// Frame para utilizar su metodo de win32
	
	return SUCCEEDED (GetVentana ()->Herramienta->SetProgressValue (ManejadorNativo ,
		dynamic_cast <wxThumbProgressReference *> (m_refData)->Completado ,
		dynamic_cast <wxThumbProgressReference *> (m_refData)->Maximo));
	
}

// -------------------------  Constructor del wxThumbEvent  ------------------------------------

// Definiremos el constructor por defecto que unicamente inicializa la parte del tipo de evento actual.

wxThumbEvent::wxThumbEvent () : wxCommandEvent (wxEVT_THUMB_BUTTON) {}

// Definiremos el constructor que toma todos los valores para un wxThumbEvent y que pueda ser enviado por el wxThumbFrame para su manejo
// en los eventos EVT_THUMB_BUTTON de los wxThumbButtons

wxThumbEvent::wxThumbEvent (int Identificador , int Mascara , int Estado) : 
	wxCommandEvent (wxEVT_THUMB_BUTTON , Identificador) {
	
	// Copiamos los valores a las propiedades
	
	MascaraActual = Mascara;
	EstadoActual = Estado;
	
}

// Terminamos la implementación de este evento , definiendo la siguiente macro

wxDEFINE_EVENT (wxEVT_THUMB_BUTTON , wxThumbEvent);

// -------------------------  Metodos miembros del wxThumbFrame  ------------------------------------

// Ahora por ultimo el wxThumbFrame , la clase principal para manejar los toolbar thumbnails en las aplicaciones que deriven de esta.

// El constructor que define toda la información del wxFrame , queda implementado de la siguiente manera

wxThumbFrame::wxThumbFrame (wxWindow * Parental , wxWindowID Identificador , const wxString & Titulo , const wxPoint & Posicion , const wxSize & Size , long Estilo , const wxString & Nombre) : 
	wxFrame (Parental , Identificador , Titulo , Posicion , Size , Estilo , Nombre) {
	
	// Registraremos el mensaje de TaskBarCreated de win32
	
	RegisterWindowMessage (TEXT ("TaskbarButtonCreated"));
	
	// Ahora tenemos que crear una istancia de C en el taskbar de la herramienta interna de esta ventana , obtendremos el resultado de la operación
	
	HRESULT Resultado = CoCreateInstance (CLSID_TaskbarList , NULL , CLSCTX_INPROC_SERVER , IID_ITaskbarList4 , reinterpret_cast <void **> (&Herramienta)); // Herramienta = new ITaskBarList4
	
	// Iniciaremos la barra de herramientas TaskBar que acabamos de crear
	
	Resultado &= Herramienta->HrInit ();
	
	// Ahora solamente validamos que el resultado sea correcto
	
	if (!SUCCEEDED (Resultado)) {
		
		// Error , modo debud de wxWidgets
		
		wxASSERT_MSG (false , wxT (" : Error al crear la instancia de ITaskbarList4 : "));
		
	}
	
}

// Ahora el constructor especializado para crear los ThumbButtons del parametro en un inicio , se implementa de la siguiente forma

wxThumbFrame::wxThumbFrame (wxWindow * Parental , wxWindowID Identificador , const wxString & Titulo , int Cantidad , const wxThumbButton Botones [] , const wxPoint & Posicion , const wxSize & Size , 
	long Estilo , const wxString & Nombre) : wxThumbFrame (Parental , Identificador , Titulo , Posicion , Size , Estilo , Nombre) {
	
	// Colocaremos cada elemento en el Arreglo de botones , dentro del vector de ThumbButton
	// por medio del metodo AddThumbButton , validando primero que el arreglo de botones no este vacio
	
	if (Botones != NULL) {
		
		// Pasamos todos los botones hasta la cantidad especificada al vector interno en este frame
		
		for (int Indice = 0 ; Indice < Cantidad ; Indice ++) {
			
			// Anexamos el wxThumbButton en este indice
			
			AddThumbButton (Botones[Indice]);
			
		}
		
	}
	else {
		
		// Error , modo debug de wxWidgets
		
		wxASSERT_MSG (false , wxT (" : Error en el arreglo de wxThumButtons del argumento , probablemente este vacio : "));
		
	}
	
}

// Ahora los metodos para anexar los wxThumbButton al vector de botones

// Definimos el metodo para anexar un wxThumbButton como una referencia del original

void wxThumbFrame::AddThumbButton (wxThumbButton * Boton) {
	
	// Colocamos en el vector una instancia por referencia del wxThumbButton del parametro
	
	BotonesThumb.push_back (std::move (wxThumbButton (Boton)));
	
}

// Definimos el metodo para anexar un simple wxThumbButton al vector como una nueva copia del original

void wxThumbFrame::AddThumbButton (const wxThumbButton & Boton) {
	
	// Colocamos el nuevo boton hasta el final del vector de botones
	
	BotonesThumb.push_back (Boton);
	
}

// Definición del metodo para crear y anexar un nuevo wxThumbButton al vector de botones en este frame

void wxThumbFrame::AddThumbButton (wxWindowID Identificador , const wxBitmap & Icono , const wxBitmap & MascaraIcono , const wxString & ToolTip , int Mascara , int Estado) {
	
	// Primero creamos un nuevo wxThumbButton con el ID y los estados proporcionados
	
	wxThumbButton BotonNuevo (Identificador , true , Estado);
	
	// Ahora colocamos los demas datos a este boton
	
	BotonNuevo.SetToolTip (ToolTip);
	BotonNuevo.SetMascara (Mascara);
	
	// Para colocar el Icono de bitmap , lo primero que haremos sera anexarlo a la lista actual
	
	if (ListaIconos == NULL) {
		
		// Creamos la lista si no ha sido creada , usamos los tamaños predeterminados para los iconos
		
		ListaIconos = new wxImageList (SizeIconX , SizeIconY , true);
		
	}
	
	// Ahora anexamos el bitmap y su mascara a esta lista , guardando el indice donde se quedo almacenado
	
	int Indice = ListaIconos->Add (Icono , MascaraIcono);
	
	// Ahora ponemos este indice a los datos del boton
	
	BotonNuevo.SetBitmap (static_cast <unsigned int> (Indice));
	
	// Solamente resta anexar el boton al vector de botones actuales para el frame
	
	BotonesThumb.push_back (BotonNuevo);
	
}

// Definición para el metodo que anexa el botón con los datos proporcionados , pero a diferencia del anterior , usa un indice de un ImageList ya cargado anteriormente

void wxThumbFrame::AddThumbButton (wxWindowID Identificador , unsigned int IndiceIcono , const wxString & ToolTip , int Mascara , int Estado) {
	
	// Repetiremos los pasos del metodo anterior
	
	wxThumbButton BotonNuevo (Identificador , true , Estado);
	
	// Ahora colocamos los demas datos
	
	BotonNuevo.SetToolTip (ToolTip);
	BotonNuevo.SetMascara (Mascara);
	BotonNuevo.SetBitmap (IndiceIcono); // El indice tiene que ser valido en el wxImageList de este frame
	
	// Anexamos el boton al vector de botones thumb
	
	BotonesThumb.push_back (BotonNuevo);
	
}

// Implementación de este otro anexador , pero con un wxIcon en lugar de un Bitmap

void wxThumbFrame::AddThumbButton (wxWindowID Identificador , const wxIcon & Icono , const wxString & ToolTip , int Mascara , int Estado) {
	
	// Repetiremos los mismos pasos que el anterior
	
	wxThumbButton BotonNuevo (Identificador , true , Estado);
	
	// Ahora colocamos los demas datos
	
	BotonNuevo.SetToolTip (ToolTip);
	BotonNuevo.SetMascara (Mascara);
	BotonNuevo.SetIcono (Icono);
	
	// Anexamos el boton al vector de botones thumb
	
	BotonesThumb.push_back (BotonNuevo);
	
}

// Inicializadores de la lista de iconos actual en el Frame

// Definimos este primero que toma una instancia de un wxImageList para iniciar la lista de iconos en este frame

void wxThumbFrame::SetIconos (const wxImageList & Lista) {
	
	// Si hay alguna lista cargada , la borraremos
	
	if (ListaIconos != NULL) {
		
		// Borramos la lista
		
		delete (ListaIconos);
		
	}
	
	// Creamos la nueva instancia de la lista de imagenes con los tamaños especificados
	
	ListaIconos = new wxImageList (SizeIconX , SizeIconY , false); // Sin mascaras para los bitmaps
	
	// Colocamos la nueva lista con los datos de la lista del parametro elemento por elemento usando el siguiente contador
	
	int Contador = 0;
	
	// Desde el primer elemento hasta el ultimo de la lista del parametro
	
	for (Contador ; Contador < Lista.GetImageCount () ; Contador ++) {
		
		// Colocamos el bitmap respectivo de la lista del parametro a la nueva lista
		
		ListaIconos->Add (Lista.GetBitmap (Contador));
		
	}
	
}

// Definimos este otro que utiliza un inicializador para crear la lista con todos los bitmaps del conjunto enviado

void wxThumbFrame::SetIconos (const std::initializer_list <wxBitmap> & Lista) {
	
	// Si hay alguna lista cargada , la borraremos
	
	if (ListaIconos != NULL) {
		
		// Borramos la lista
		
		delete (ListaIconos);
		
	}
	
	// Anexaremos cada uno de los bitmaps a la lista que a continuación instanciaremos
	
	ListaIconos = new wxImageList (SizeIconX , SizeIconY , false); // Bitmaps sin mascaras
	
	// Anexaremos cada uno de los bitmaps
	
	for (const wxBitmap & Icono : Lista) {
		
		// Ponemos este bitmap en la lista
		
		ListaIconos->Add (Icono);
		
	}
	
}

// Definiremos uno que acepta bitmap y mascara del bitmap

void wxThumbFrame::SetIconos (const std::initializer_list <wxBitmapMaskIcon> & Lista) {
	
	// Si hay alguna lista cargada , la borraremos
	
	if (ListaIconos != NULL) {
		
		// Borramos la lista
		
		delete (ListaIconos);
		
	}
	
	// Anexaremos cada uno de los bitmaps a la lista que a continuación instanciaremos
	
	ListaIconos = new wxImageList (SizeIconX , SizeIconY , true);
	
	// Anexaremos cada uno de los bitmaps incluyendo a su mascara
	
	for (const wxBitmapMaskIcon & Icono : Lista) {
		
		// Ponemos este bitmap en la lista
		
		ListaIconos->Add (Icono.first , Icono.second);
		
	}
	
}

// Definición para el metodo sobrecargado con el wxIcon para crear la lista de iconos

void wxThumbFrame::SetIconos (const std::initializer_list <wxIcon> & Lista) {
	
	// Si hay alguna lista cargada , la borraremos
	
	if (ListaIconos != NULL) {
		
		// Borramos la lista
		
		delete (ListaIconos);
		
	}
	
	// Anexaremos cada uno de los iconos en la lista del parametro
	
	ListaIconos = new wxImageList (SizeIconX , SizeIconY , false); // Bitmaps sin mascaras
	
	// Anexaremos cada uno de los icons
	
	for (const wxIcon & Icono : Lista) {
		
		// Ponemos este icono en la lista
		
		ListaIconos->Add (Icono);
		
	}
	
}

// Anexadores de iconos a la lista actual

// Implementación para anexar un nuevo icono a partir de bitmap sin mascara de bits y devolver el indice que tomo este nuevo icono

size_t wxThumbFrame::AddIcono (const wxBitmap & NuevoIcono) {
	
	// Anexaremos el nuevo icono a la lista de iconos actual , si no hay una lista entonces la creamos
	
	if (ListaIconos == NULL) {
		
		// Creamos la lista con las medidas por defecto y sin mascara de bits
		
		ListaIconos = new wxImageList (SizeIconX , SizeIconY , false);
		
	}
	
	// Anexamos este icono a la lista y devolvemos el indice que ocupo
	
	return static_cast <size_t> (ListaIconos->Add (NuevoIcono));
	
}

// Anexar icono pero con una pareja de bitmaps , uno para el dibujo y otro para la mascara de bits

size_t wxThumbFrame::AddIcono (const wxBitmapMaskIcon & NuevoIcono) {
	
	// Creamos la lista si esta vacia
	
	if (ListaIconos == NULL) {
		
		// Usamos las medidas respectivas y con mascara de bits habilitada
		
		ListaIconos = new wxImageList (SizeIconX , SizeIconY , true);
		
	}
	
	// Colocamos el nuevo icono en esta lista
	
	return static_cast <size_t> (ListaIconos->Add (NuevoIcono.first , NuevoIcono.second));
	
}

// Ahora el siguiente es para anexar un wxIcon a la lista

size_t wxThumbFrame::AddIcono (const wxIcon & NuevoIcono) {
	
	// Creamos la lista si esta vacia
	
	if (ListaIconos == NULL) {
		
		// Ponemos las medidas necesarias
		
		ListaIconos = new wxImageList (SizeIconX , SizeIconY);
		
	}
	
	// Anexamos el icono a esta lista
	
	return static_cast <size_t> (ListaIconos->Add (NuevoIcono));
	
}

// Removedores de ThumButtons

// Definiremos el primer metodo para remover los ThumbButtons en el vector actual , este
// unicamente recive el indice en el que va a eliminar el ThumButton

bool wxThumbFrame::RemoveThumbButton (size_t Indice) {
	
	// Buscaremos en el vector el indice actual y devolveremos true si fue exitosamente removido
	
	bool Resultado = false;
	
	// Primero validamos que el indice sea < que el numero de elementos en el vector
	
	if (Indice < BotonesThumb.size ()) {
		
		// Recuperamos el iterador al inicio del vector
		
		decltype (BotonesThumb)::iterator Posicion = BotonesThumb.begin ();
		
		// Ahora incrementamos el valor del indice a este iterador
		
		Posicion += Indice; // &Vector[0] + Indice
		
		// Ahora invocamos la función para quitar este elemento en el vector
		
		BotonesThumb.erase (Posicion);
		
		// Cambiamos el estado a true
		
		Resultado = true;
		
	}
	
	// Devolvemos el resultado general
	
	return Resultado;
	
}

// Ahora el metodo para quitar todos los ThumbButtons en el vector del Frame

bool wxThumbFrame::RemoveAllThumbButton () noexcept {
	
	// Quitamos todos los elementos del vector
	
	BotonesThumb.clear ();
	
	// Devolvemos el estado actual del vector , si esta vacio o no
	
	return BotonesThumb.empty ();
	
}

// Consultar indices y si un ThumbButton esta dentro o no del vector del Frame

// Este primero permite determinar el indice en el que esta un ThumButton con cierto
// Identificador con el que fue construido

int wxThumbFrame::GetThumbButtonIndex (wxWindowID Identificador) const {
	
	// Recuperaremos el indice especificado y lo dejaremos almacenado en la siguiente variable
	// Si no hay ningun elemento en el vector con Dicho Identificador , entonces se devuelve -1
	
	int Indice = -1;
	
	// Declaramos el contador de botones comparados
	
	size_t Contador = 0u;
	
	// Buscaremos desde el primero hasta el ultimo elemento
	
	for (decltype (BotonesThumb)::const_iterator Boton = BotonesThumb.cbegin () ; Boton < BotonesThumb.cend () ; Boton ++ , Contador ++) {
		
		// Validamos que el ID en el actual boton sea igual al que se tiene como parametro
		
		if (Boton->IsValid () && Boton->GetID () == Identificador) {
			
			// Ponemos el contador actual en el indice de retorno
			
			Indice = static_cast <int> (Contador);
			
			// Terminamos de buscar los elementos
			
			break;
			
		}
		
	}
	
	// Devolvemos el valor actual del indice
	
	return Indice;
	
}

// Metodo para saber si un wxThumbButton identificado con cierto ID esta contenido dentro del vector del Frame

bool wxThumbFrame::HaveThumbButton (wxWindowID Identificador) const {
	
	// Buscaremos de manera similar el elemento con el ID en el vector de botones
	
	for (decltype (BotonesThumb)::const_iterator Boton = BotonesThumb.cbegin () ; Boton < BotonesThumb.cend () ; Boton ++) {
		
		// Validamos que el ID en el actual boton sea igual al que se tiene como parametro
		
		if (Boton->IsValid () && Boton->GetID () == Identificador) {
			
			// Devolvemos true
			
			return true;
			
		}
		
	}
	
	// Devolvemos false por defecto si no se encontro una coincidencia
	
	return false;
	
}

// ---------------------------------------  Inicializadores de toolbar thumbnail win32  -------------------------------------------

// Ahora definiremos los metodos mas importantes , los de inicialización y actualización del toolbar que permiten tener disponibles estos
// controles en el wxThumbFrame actual.

// Es importante saber que el siguiente metodo Inicializa la barra de toolbar con todos los wxThumbButtons que existen actualmente
// en el vector de botones de referencia y usando los bitmaps de la lista de iconos actualmente establecida (si es que hay).

// Para inicializar usamos una simple interfaz con el win32 y el objeto ITaskBarList4 que nos permitira realizar la incorporación de la barra
// de herramientas en el Thumbnail de nuestro wxFrame actual.

void wxThumbFrame::InicializarThumbButtons () {
	
	// Si la lista de Imagenes no fue creada todavia , entonces creamos una de referencia solamente , y no debera ocuparse en ninguno de los
	// wxThumbButtons
	
	if (ListaIconos == NULL) {
		
		// Creamos una lista vacia
		
		ListaIconos = new wxImageList (SizeIconX , SizeIconY , false);
		
	}
	
	// Ahora que tenemos todo preparado , verificamos que el vector de botones no este vacio , de lo contrario no tendra caso continuar
	// con la incorporación del toolbar.
	
	if (!BotonesThumb.empty ()) {
		
		// Ahora resta asociar la lista de iconos y el arreglo de THUMBBUTTONS a esta toolbar para que aparezcan en la barra del Thumbnail actualmente en este wxFrame
		
		// Comenzaremos por definir nuestro arreglo de wxThumbButtons usando el wrapper array con un maximo de Max buttons que puede tener el Frame
		
		std::array <THUMBBUTTON , ThumbButtonMax> ArregloButtons;
		
		// Colocaremos unicamente los wxThumbButtons validos del vector actual en el arreglo , usaremos el siguiente contador para tener la cuenta
		// del numero de botones que se anexaran al frame
		
		size_t CantidadBotones = 0u;
		
		// Recorremos todo el vector de botones
		
		for (wxThumbButton & Boton : BotonesThumb) {
			
			// Validamos que sea aceptado el boton
			
			if (Boton.IsValid ()) {
				
				// Entonces ponemos su version THUMBBUTTON en el arreglo actual de botones final
				
				ArregloButtons[CantidadBotones ++] = Boton.GetTHUMBBUTTON (); // Se incrementa el contador
				
			}
			
		}
		
		// Para continuar y asociar la lista de imagenes y el arreglo de botones a la barra , tenemos que tener presente el Manejador nativo de win32
		// de este frame
		
		HWND Manejador = this->GetHWND ();
		
		// Ahora colocaremos la lista de imagenes al task bar list creado y obtendremos el resultado , usaremos tanto el manejador de este frame
		// para asociarlo en este lugar y un cast de la lista de imagenes para su version nativa de win32
		
		HRESULT Resultado = Herramienta->ThumbBarSetImageList (Manejador , reinterpret_cast <HIMAGELIST> (ListaIconos->GetHIMAGELIST ()));
		
		// Si el resultado es correcto continuamos
		
		if (SUCCEEDED (Resultado)) {
			
			// Ahora solamente queda colocar los botones en esta misma cadena , por lo tanto solamente llamamos a la siguiente función
			// con el manejador , el numero de botones a anexar y el origen del arreglo de botones THUMBBUTTON anteriormente determinados.
			
			Herramienta->ThumbBarAddButtons (Manejador , static_cast <UINT> (CantidadBotones) , reinterpret_cast <LPTHUMBBUTTON> (ArregloButtons.data ()));
			
		}
		
	}
	
}

// Se puede usar convenientemente la siguiente función para actualizar solamente los botones que estan en el Taskbar creado anteriormente con su constructor
// permitiendo asi cambiar algunas de sus propiedades como por ejemplo ocultar o cambiar el efecto que tienen los botones en el thumbnail

void wxThumbFrame::ActualizarThumbButtons () {
	
	// Pasaremos a verificar que el vector no este vacio para poder continuar
	
	if (!BotonesThumb.empty ()) {
		
		// Ahora actualizaremos cada boton insertado en el vector , pasandolos al siguiente arreglo
		// Repetiremos la asignación de datos desde el vector hacia el arreglo de THUMBBUTTONS nativos
		
		std::array <THUMBBUTTON , ThumbButtonMax> ArregloButtons;
		
		// Colocaremos unicamente los wxThumbButtons validos del vector actual en el arreglo
		
		size_t CantidadBotones = 0u;
		
		// Recorremos todo el vector de botones
		
		for (wxThumbButton & Boton : BotonesThumb) {
			
			// Validamos que sea aceptado el boton
			
			if (Boton.IsValid ()) {
				
				// Entonces ponemos su version THUMBBUTTON en el arreglo actual de botones final
				
				ArregloButtons[CantidadBotones ++] = Boton.GetTHUMBBUTTON (); // Se incrementa el contador
				
			}
			
		}
		
		// Recuperamos el manejador de ventana actual
		
		HWND Manejador = this->GetHWND ();
		
		// Ahora procederemos a actualizar los botones que tuvieron cambios en la re-asignación de botones en el arreglo
		
		Herramienta->ThumbBarUpdateButtons (Manejador , static_cast <UINT> (CantidadBotones) , reinterpret_cast <LPTHUMBBUTTON> (ArregloButtons.data ()));
		
	}
	
}

// Sobreescribiremos el destructor para que pueda terminar correctamente todos sus recursos que utilizo este frame ademas de los que ya tenia

wxThumbFrame::~ wxThumbFrame () {
	
	// Eliminaremos la barra de herramientas tool bar si es diferente a NULL
	
	if (Herramienta != NULL) {
		
		// Eliminamos esta instancia usando una llamada a Release
		
		Herramienta->Release ();
		
	}
	
	// Eliminamos tambien la lista de imagenes que se utilizo para los botones
	
	if (ListaIconos != NULL) {
		
		// Borramos la lista
		
		delete (ListaIconos);
		
	}
	
}

// ------------------------------------------  WinProc y Captador de Eventos  -----------------------------

// Ahora lo que tenemos que hacer es definir otro metodo muy importante por que es el encargado de recivir los eventos notificados por
// la API de win32 y convertirlos a un evento valido para wxWidgets y que pueda ser atendido por el manejador(es) que se haya especificado.

// Para esto se tiene que sobreescribir el metodo siguiente , ampliando las posibilidades de atender eventos

WXLRESULT wxThumbFrame::MSWWindowProc (WXUINT Mensaje , WXWPARAM WParam , WXLPARAM LParam) {
	
	// En primer lugar queda determinar el tipo de mensaje enviado , sabemos que los THUMBBUTTON de win32 producen WM_COMMAND events cuanto
	// son presionados por el usuario , por lo que este sera nuestro identificador del mensaje que queremos atender en este wxThumbFrame
	
	if (Mensaje == WM_COMMAND) {
		
		// Si es del tipo que requerimos , entonces pasaremos a crear el evento y lanzarlo para que sea atendido por medio
		// de nuestro metodo SendEventThumbButton con los parametros que identifican al propio THUMBBUTTON
		
		SendEventThumbButton (WParam , LParam);
		
	}
	
	// dejamos que wxFrame haga el resto que tiene programado en la cadena de eventos y devolvemos el resultado que esto genera
	
	return this->wxFrame::MSWWindowProc (Mensaje , WParam , LParam);
	
}

// Ahora implementamos el enviador de eventos de tipo wxThumbEvent asociado a este frame

void wxThumbFrame::SendEventThumbButton (WXWPARAM WParam , WXLPARAM LParam) {
	
	// En primer lugar queda determinar el ID del boton THUMBBUTTON que fue presionado , para esto usaremos
	// el nibble bajo de la palabra de WParam
	
	WORD IdentificadorThumb = LOWORD (WParam);
	
	// Ahora buscaremos el boton que fue presionado en el repositorio vectorial de este Frame , usando este Identificador
	// en su version de wxWindowID y dejando su posición de iterador almacenado en la siguiente variable para poderlo consultar
	// una vez encontrado
	
	decltype (BotonesThumb)::const_iterator ButtonPresionado = BotonesThumb.cbegin (); // Comienza con el origen
	
	// Ahora buscamos donde el elemento coincida con el Id establecido
	
	for (ButtonPresionado ; ((ButtonPresionado != BotonesThumb.cend ()) && (ButtonPresionado->GetID () != static_cast <wxWindowID> (IdentificadorThumb))) ; ButtonPresionado ++);
	
	// Si fue encontrado el iterador debe ser diferente a cend del vector
	
	if (ButtonPresionado != BotonesThumb.cend ()) {
		
		// Usamos este boton para crear el evento wxThumbEvent con la información que solicita , comenzando por crearlo usando el ID actual
		
		wxThumbEvent EventoEnviar (static_cast <UINT> (IdentificadorThumb));
		
		// Colocamos ahora la mascara del boton que ocasiono el evento
		
		EventoEnviar.SetMascara (ButtonPresionado->GetMascara ());
		
		// Ahora colocamos el estado de este boton
		
		EventoEnviar.SetEstados (ButtonPresionado->GetEstados ());
		
		// Y ponemos el propio ThumButton como parte del estado del evento derivado de wxEvent
		
		EventoEnviar.SetEventObject (dynamic_cast <wxObject *> (&const_cast <wxThumbButton &> (*ButtonPresionado)));
		
		// Ya solo queda procesar este evento por el manejador de eventos respectivo , usando esto
		
		ProcessWindowEvent (dynamic_cast <wxEvent &> (EventoEnviar));
		
	}
	
	// Si no encontro este ID en los thumbbuttons , entonces significa que fue eliminado y no se actualizo el wxThumbFrame o que simplemente
	// fue un evento externo de otro control
	
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// Cerramos el espacio de nombres

} // jrma_extension

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
