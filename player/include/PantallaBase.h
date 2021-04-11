///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2017)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __PANTALLABASE_H__
#define __PANTALLABASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include "PanelCover.h"
#include "Widgets/ElectrodeBoton.h"
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include <Widgets/ElectrodeProgreso.h>
#include <wx/bmpbuttn.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/statusbr.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/frame.h>
#include <wx/combobox.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define BOTON_PLAY_ID 1000
#define BOTON_PAUSE_ID 1001
#define BOTON_STOP_ID 1002
#define MENU_ARCHIVO_ABRIR_ID 1003
#define MENU_ARCHIVO_CERRAR_ID 1004
#define MENU_ARCHIVO_SALIR_ID 1005
#define MENU_HERRAMIENTAS_DEVICE_ID 1006
#define MENU_AYUDA_ACERCA_ID 1007
#define HERRAMIENTA_ABRIR_ID 1008
#define HERRAMIENTA_CERRAR_ID 1009
#define HERRAMIENTA_DEVICE_ID 1010

///////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------

// ------------------------------  ELECTRODEMP  --------------------------------

// Incluiremos la configuración de ElectrodeMP.

#include <ElectrodeMP/Configuracion.h>

// Si tenemos habilitado el uso de wxThumbFrame , entonces pasaremos a realizar la siguiente acción.

#ifdef ELECTRODEMP_ENABLE_WX_THUMBNAIL

// Incluiremos la cabecera para la extensión de wxThumbnail.

#include "wx/thumbnail.h"

// Utilizaremos el siguiente espacio de nombres.

using namespace jrma_extension;

// definiremos la clase wxSPECIFIC_FRAME con un wxThumbFrame.

#define wxSPECIFIC_FRAME wxThumbFrame

#else

// Definiremos la clase wxSPECIFIC_FRAME con un wxFrame.

#define wxSPECIFIC_FRAME wxFrame

#endif

// -----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
/// Class PantallaBase
///////////////////////////////////////////////////////////////////////////////
class PantallaBase : public wxSPECIFIC_FRAME
{
	
	protected:
		
		wxPanel* Contenedor;
		PanelCover* Contenedor_Cover;
		wxPanel* Contenedor_Tag;
		wxScrolledWindow* Contenedor_Informacion;
		wxStaticText* Etiqueta_Title;
		wxStaticText* Etiqueta_Artist;
		wxStaticText* Etiqueta_Composer;
		wxStaticText* Etiqueta_Album;
		wxStaticText* Etiqueta_Year;
		wxStaticText* Etiqueta_Track;
		wxStaticText* Etiqueta_Genre;
		wxStaticText* Etiqueta_Date_Original;
		wxStaticText* Etiqueta_Date;
		ElectrodeProgreso* Gauge_Progreso;
		wxStaticText* Etiqueta_Time;
		wxStaticText* Etiqueta_Duration;
		ElectrodeBoton* Boton_Play;
		ElectrodeBoton* Boton_Pause;
		ElectrodeBoton* Boton_Stop;
		wxStatusBar* Barra_Estado;
		wxMenuBar* Barra_Menu;
		wxMenu* Menu_Archivo;
		wxMenu* Menu_Herramientas;
		wxMenu* Menu_Ayuda;
		wxToolBar* Barra_Herramientas;
		wxToolBarToolBase* Herramienta_Abrir;
		wxToolBarToolBase* Herramienta_Cerrar;
		wxToolBarToolBase* Herramienta_Device;
		
		// Virtual event handlers, overide them in your derived class
		virtual void Evento_Close( wxCloseEvent& event ) { event.Skip(); }
		virtual void Evento_Size( wxSizeEvent& event ) { event.Skip(); }
		virtual void Evento_Play( wxCommandEvent& event ) { event.Skip(); }
		virtual void Evento_Pause( wxCommandEvent& event ) { event.Skip(); }
		virtual void Evento_Stop( wxCommandEvent& event ) { event.Skip(); }
		virtual void Evento_Archivo_Abrir( wxCommandEvent& event ) { event.Skip(); }
		virtual void Evento_Archivo_Cerrar( wxCommandEvent& event ) { event.Skip(); }
		virtual void Evento_Archivo_Salir( wxCommandEvent& event ) { event.Skip(); }
		virtual void Evento_Herramientas_Device( wxCommandEvent& event ) { event.Skip(); }
		virtual void Evento_Ayuda_Acerca( wxCommandEvent& event ) { event.Skip(); }
		virtual void Evento_Tool_Abrir( wxCommandEvent& event ) { event.Skip(); }
		virtual void Evento_Tool_Cerrar( wxCommandEvent& event ) { event.Skip(); }
		virtual void Evento_Tool_Device( wxCommandEvent& event ) { event.Skip(); }
	
	
	public:
		
		PantallaBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 761,500 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
		
		~PantallaBase();
		
};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogoDevice
///////////////////////////////////////////////////////////////////////////////
class DialogoDevice : public wxDialog
{
	
	protected:
		
		wxPanel* Contenedor;
		wxStaticText* Etiqueta_Host;
		wxComboBox* Combo_Host;
		wxStaticText* Etiqueta_Dipositivo;
		wxComboBox* Combo_Device;
		wxButton* Boton_Aceptar;
		wxButton* Boton_Cancelar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void Evento_Combo_Host( wxCommandEvent& event ) { event.Skip(); }
		virtual void Evento_Boton_Aceptar( wxCommandEvent& event ) { event.Skip(); }
		virtual void Evento_Boton_Cancelar( wxCommandEvent& event ) { event.Skip(); }
	
	
	public:
		
		DialogoDevice( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Seleccionar Dispositivo de Salida"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 406,212 ), long style = wxDEFAULT_DIALOG_STYLE );
		~DialogoDevice();
	
};

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class DialogoAcerca
///////////////////////////////////////////////////////////////////////////////
class DialogoAcerca : public wxDialog
{
	
	protected:
		
		wxPanel* Contenedor;
		wxStaticBitmap* Bitmap_Creator;
		wxStaticText* Etiqueta_Autor;
		wxStaticBitmap* Bitmap_Logotipo;
		wxStaticText* Etiqueta_Aplicacion;
		wxStaticText* Etiqueta_Contacto;
		wxButton* Boton_Cerrar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void Evento_Cerrar( wxCommandEvent& event ) { event.Skip(); }
	
	
	public:
		
		DialogoAcerca( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Acerca de..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 438,445 ), long style = wxDEFAULT_DIALOG_STYLE );
		
		~DialogoAcerca();
	
};

#endif //__PANTALLABASE_H__
