///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "PantallaBase.h"

///////////////////////////////////////////////////////////////////////////

PantallaBase::PantallaBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxSPECIFIC_FRAME( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 677,421 ), wxSize( 1015,631 ) );
	
	wxBoxSizer* Sizer_Frame;
	Sizer_Frame = new wxBoxSizer( wxVERTICAL );
	
	Contenedor = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Contenedor->SetBackgroundColour( wxColour( 240, 249, 255 ) );
	
	wxBoxSizer* Sizer_Contenedor;
	Sizer_Contenedor = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* Sizer_Datos;
	Sizer_Datos = new wxBoxSizer( wxHORIZONTAL );
	
	Contenedor_Cover = new PanelCover( Contenedor, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Contenedor_Cover->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	Sizer_Datos->Add( Contenedor_Cover, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND|wxSHAPED, 15 );
	
	Contenedor_Tag = new wxPanel( Contenedor, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Contenedor_Tag->SetBackgroundColour( wxColour( 224, 239, 252 ) );
	
	wxBoxSizer* Sizer_Tag;
	Sizer_Tag = new wxBoxSizer( wxVERTICAL );
	
	Contenedor_Informacion = new wxScrolledWindow( Contenedor_Tag, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTRANSPARENT_WINDOW|wxVSCROLL );
	Contenedor_Informacion->SetScrollRate( 5, 5 );
	wxBoxSizer* Sizer_Tag_Scrolled;
	Sizer_Tag_Scrolled = new wxBoxSizer( wxVERTICAL );
	
	Etiqueta_Title = new wxStaticText( Contenedor_Informacion, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	Etiqueta_Title->Wrap( -1 );
	Etiqueta_Title->SetFont( wxFont( 11, wxFONTFAMILY_ROMAN, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false, wxT("Cambria") ) );
	Etiqueta_Title->SetForegroundColour( wxColour( 6, 35, 83 ) );
	
	Sizer_Tag_Scrolled->Add( Etiqueta_Title, 0, wxALIGN_LEFT|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 7 );
	
	Etiqueta_Artist = new wxStaticText( Contenedor_Informacion, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	Etiqueta_Artist->Wrap( -1 );
	Etiqueta_Artist->SetFont( wxFont( 11, wxFONTFAMILY_ROMAN, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false, wxT("Cambria") ) );
	Etiqueta_Artist->SetForegroundColour( wxColour( 6, 35, 83 ) );
	
	Sizer_Tag_Scrolled->Add( Etiqueta_Artist, 0, wxALIGN_LEFT|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 7 );
	
	Etiqueta_Composer = new wxStaticText( Contenedor_Informacion, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	Etiqueta_Composer->Wrap( -1 );
	Etiqueta_Composer->SetFont( wxFont( 11, wxFONTFAMILY_ROMAN, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false, wxT("Cambria") ) );
	Etiqueta_Composer->SetForegroundColour( wxColour( 6, 35, 83 ) );
	
	Sizer_Tag_Scrolled->Add( Etiqueta_Composer, 0, wxALIGN_LEFT|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 7 );
	
	Etiqueta_Album = new wxStaticText( Contenedor_Informacion, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	Etiqueta_Album->Wrap( -1 );
	Etiqueta_Album->SetFont( wxFont( 11, wxFONTFAMILY_ROMAN, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false, wxT("Cambria") ) );
	Etiqueta_Album->SetForegroundColour( wxColour( 6, 35, 83 ) );
	
	Sizer_Tag_Scrolled->Add( Etiqueta_Album, 0, wxALIGN_LEFT|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 7 );
	
	Etiqueta_Year = new wxStaticText( Contenedor_Informacion, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	Etiqueta_Year->Wrap( -1 );
	Etiqueta_Year->SetFont( wxFont( 11, wxFONTFAMILY_ROMAN, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false, wxT("Cambria") ) );
	Etiqueta_Year->SetForegroundColour( wxColour( 6, 35, 83 ) );
	
	Sizer_Tag_Scrolled->Add( Etiqueta_Year, 0, wxALIGN_LEFT|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 7 );
	
	Etiqueta_Track = new wxStaticText( Contenedor_Informacion, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	Etiqueta_Track->Wrap( -1 );
	Etiqueta_Track->SetFont( wxFont( 11, wxFONTFAMILY_ROMAN, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false, wxT("Cambria") ) );
	Etiqueta_Track->SetForegroundColour( wxColour( 6, 35, 83 ) );
	
	Sizer_Tag_Scrolled->Add( Etiqueta_Track, 0, wxALIGN_LEFT|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 7 );
	
	Etiqueta_Genre = new wxStaticText( Contenedor_Informacion, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	Etiqueta_Genre->Wrap( -1 );
	Etiqueta_Genre->SetFont( wxFont( 11, wxFONTFAMILY_ROMAN, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false, wxT("Cambria") ) );
	Etiqueta_Genre->SetForegroundColour( wxColour( 6, 35, 83 ) );
	
	Sizer_Tag_Scrolled->Add( Etiqueta_Genre, 0, wxALIGN_LEFT|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 7 );
	
	Etiqueta_Date_Original = new wxStaticText( Contenedor_Informacion, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	Etiqueta_Date_Original->Wrap( -1 );
	Etiqueta_Date_Original->SetFont( wxFont( 11, wxFONTFAMILY_ROMAN, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false, wxT("Cambria") ) );
	Etiqueta_Date_Original->SetForegroundColour( wxColour( 6, 35, 83 ) );
	
	Sizer_Tag_Scrolled->Add( Etiqueta_Date_Original, 0, wxALIGN_LEFT|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 7 );
	
	Etiqueta_Date = new wxStaticText( Contenedor_Informacion, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	Etiqueta_Date->Wrap( -1 );
	Etiqueta_Date->SetFont( wxFont( 11, wxFONTFAMILY_ROMAN, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false, wxT("Cambria") ) );
	Etiqueta_Date->SetForegroundColour( wxColour( 6, 35, 83 ) );
	
	Sizer_Tag_Scrolled->Add( Etiqueta_Date, 0, wxALIGN_LEFT|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 7 );
	
	
	Contenedor_Informacion->SetSizer( Sizer_Tag_Scrolled );
	Contenedor_Informacion->Layout();
	Sizer_Tag_Scrolled->Fit( Contenedor_Informacion );
	Sizer_Tag->Add( Contenedor_Informacion, 1, wxEXPAND, 0 );
	
	
	Contenedor_Tag->SetSizer( Sizer_Tag );
	Contenedor_Tag->Layout();
	Sizer_Tag->Fit( Contenedor_Tag );
	Sizer_Datos->Add( Contenedor_Tag, 2, wxEXPAND, 0 );
	
	
	Sizer_Contenedor->Add( Sizer_Datos, 3, wxEXPAND, 5 );
	
	wxBoxSizer* Sizer_Slider;
	Sizer_Slider = new wxBoxSizer( wxVERTICAL );
	
	Gauge_Progreso = new ElectrodeProgreso( Contenedor, wxID_ANY, wxDefaultPosition, wxSize( -1,50 ), wxFULL_REPAINT_ON_RESIZE|wxBORDER_NONE );
	Gauge_Progreso->Enable( false );
	Gauge_Progreso->SetMinSize( wxSize( -1,50 ) );
	Gauge_Progreso->SetMaxSize( wxSize( -1,50 ) );
	
	Sizer_Slider->Add( Gauge_Progreso, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	
	Sizer_Contenedor->Add( Sizer_Slider, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxFIXED_MINSIZE|wxLEFT|wxRIGHT, 24 );
	
	wxBoxSizer* Sizer_Time;
	Sizer_Time = new wxBoxSizer( wxHORIZONTAL );
	
	Etiqueta_Time = new wxStaticText( Contenedor, wxID_ANY, wxT("00:00"), wxDefaultPosition, wxDefaultSize, 0 );
	Etiqueta_Time->Wrap( -1 );
	Etiqueta_Time->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Lucida Console") ) );
	Etiqueta_Time->SetForegroundColour( wxColour( 6, 35, 83 ) );
	
	Sizer_Time->Add( Etiqueta_Time, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 32 );
	
	
	Sizer_Time->Add( 0, 0, 1, wxEXPAND, 5 );
	
	Etiqueta_Duration = new wxStaticText( Contenedor, wxID_ANY, wxT("00:00"), wxDefaultPosition, wxDefaultSize, 0 );
	Etiqueta_Duration->Wrap( -1 );
	Etiqueta_Duration->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Lucida Console") ) );
	Etiqueta_Duration->SetForegroundColour( wxColour( 6, 35, 83 ) );
	
	Sizer_Time->Add( Etiqueta_Duration, 0, wxALIGN_RIGHT|wxRIGHT, 32 );
	
	
	Sizer_Contenedor->Add( Sizer_Time, 0, wxEXPAND, 0 );
	
	wxBoxSizer* Sizer_Botones;
	Sizer_Botones = new wxBoxSizer( wxHORIZONTAL );
	
	Boton_Play = new ElectrodeBoton( Contenedor, BOTON_PLAY_ID, wxNullBitmap, wxDefaultPosition, wxSize( 50,50 ), wxBU_AUTODRAW|0|wxBORDER_NONE );
	
	Boton_Play->SetBitmap( wxBitmap( wxT("BITMAP_BUTTON_PLAY"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	Boton_Play->SetBitmapDisabled( wxBitmap( wxT("BITMAP_BUTTON_PLAY_DISABLE"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	Boton_Play->SetBitmapPressed( wxBitmap( wxT("BITMAP_BUTTON_PLAY_PRESSED"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	Boton_Play->SetBitmapCurrent( wxBitmap( wxT("BITMAP_BUTTON_PLAY_SELECTED"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	Boton_Play->SetBackgroundColour( wxColour( 240, 249, 255 ) );
	
	Sizer_Botones->Add( Boton_Play, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	Boton_Pause = new ElectrodeBoton( Contenedor, BOTON_PAUSE_ID, wxNullBitmap, wxDefaultPosition, wxSize( 50,50 ), wxBU_AUTODRAW|0|wxBORDER_NONE );
	
	Boton_Pause->SetBitmap( wxBitmap( wxT("BITMAP_BUTTON_PAUSE"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	Boton_Pause->SetBitmapDisabled( wxBitmap( wxT("BITMAP_BUTTON_PAUSE_DISABLE"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	Boton_Pause->SetBitmapPressed( wxBitmap( wxT("BITMAP_BUTTON_PAUSE_PRESSED"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	Boton_Pause->SetBitmapCurrent( wxBitmap( wxT("BITMAP_BUTTON_PAUSE_SELECTED"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	Boton_Pause->SetBackgroundColour( wxColour( 240, 249, 255 ) );
	
	Sizer_Botones->Add( Boton_Pause, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	Boton_Stop = new ElectrodeBoton( Contenedor, BOTON_STOP_ID, wxNullBitmap, wxDefaultPosition, wxSize( 50,50 ), wxBU_AUTODRAW|0|wxBORDER_NONE );
	
	Boton_Stop->SetBitmap( wxBitmap( wxT("BITMAP_BUTTON_STOP"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	Boton_Stop->SetBitmapDisabled( wxBitmap( wxT("BITMAP_BUTTON_STOP_DISABLE"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	Boton_Stop->SetBitmapPressed( wxBitmap( wxT("BITMAP_BUTTON_STOP_PRESSED"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	Boton_Stop->SetBitmapCurrent( wxBitmap( wxT("BITMAP_BUTTON_STOP_SELECTED"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	Boton_Stop->SetBackgroundColour( wxColour( 240, 249, 255 ) );
	
	Sizer_Botones->Add( Boton_Stop, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	Sizer_Contenedor->Add( Sizer_Botones, 0, wxALIGN_CENTER, 0 );
	
	
	Contenedor->SetSizer( Sizer_Contenedor );
	Contenedor->Layout();
	Sizer_Contenedor->Fit( Contenedor );
	Sizer_Frame->Add( Contenedor, 1, wxEXPAND, 0 );
	
	
	this->SetSizer( Sizer_Frame );
	this->Layout();
	Barra_Estado = this->CreateStatusBar( 2, wxSTB_SIZEGRIP, wxID_ANY );
	Barra_Menu = new wxMenuBar( 0 );
	Menu_Archivo = new wxMenu();
	wxMenuItem* Menu_Archivo_Abrir;
	Menu_Archivo_Abrir = new wxMenuItem( Menu_Archivo, MENU_ARCHIVO_ABRIR_ID, wxString( wxT("Abrir") ) + wxT('\t') + wxT("Ctrl+O"), wxT("Abrir un archivo de Audio"), wxITEM_NORMAL );
	Menu_Archivo->Append( Menu_Archivo_Abrir );
	
	wxMenuItem* Menu_Archivo_Cerrar;
	Menu_Archivo_Cerrar = new wxMenuItem( Menu_Archivo, MENU_ARCHIVO_CERRAR_ID, wxString( wxT("Cerrar") ) + wxT('\t') + wxT("Ctrl+W"), wxT("Cerrar archivo de Audio Actual"), wxITEM_NORMAL );
	Menu_Archivo->Append( Menu_Archivo_Cerrar );
	
	Menu_Archivo->AppendSeparator();
	
	wxMenuItem* Menu_Archivo_Salir;
	Menu_Archivo_Salir = new wxMenuItem( Menu_Archivo, MENU_ARCHIVO_SALIR_ID, wxString( wxT("Salir") ) + wxT('\t') + wxT("Alt+F4"), wxString ("Salir de la Aplicación"), wxITEM_NORMAL );
	Menu_Archivo->Append( Menu_Archivo_Salir );
	
	Barra_Menu->Append( Menu_Archivo, wxT("Archivo") );
	
	Menu_Herramientas = new wxMenu();
	wxMenuItem* Menu_Herramientas_Device;
	Menu_Herramientas_Device = new wxMenuItem( Menu_Herramientas, MENU_HERRAMIENTAS_DEVICE_ID, wxString( wxT("Seleccionar API - Device") ) + wxT('\t') + wxT("Ctrl+D"), wxT("Seleccionar API y Dispositivo de Salida"), wxITEM_NORMAL );
	Menu_Herramientas->Append( Menu_Herramientas_Device );
	
	Barra_Menu->Append( Menu_Herramientas, wxT("Herramientas") );
	
	Menu_Ayuda = new wxMenu();
	wxMenuItem* Menu_Ayuda_Acerca;
	Menu_Ayuda_Acerca = new wxMenuItem( Menu_Ayuda, MENU_AYUDA_ACERCA_ID, wxString( wxT("Acerca de...") ) + wxT('\t') + wxT("F1"), wxString ("Información sobre la aplicación"), wxITEM_NORMAL );
	Menu_Ayuda->Append( Menu_Ayuda_Acerca );
	
	Barra_Menu->Append( Menu_Ayuda, wxT("Ayuda") );
	
	this->SetMenuBar( Barra_Menu );
	
	Barra_Herramientas = this->CreateToolBar( wxTB_HORIZONTAL|wxTB_HORZ_LAYOUT|wxTB_TEXT, wxID_ANY );
	Barra_Herramientas->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	Barra_Herramientas->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNHIGHLIGHT ) );
	Barra_Herramientas->SetBackgroundColour( wxColour( 174, 210, 251 ) );
	
	Herramienta_Abrir = Barra_Herramientas->AddTool( HERRAMIENTA_ABRIR_ID, wxT("Abrir"), wxBitmap( wxT("BITMAP_TOOL_OPEN"), wxBITMAP_TYPE_PNG_RESOURCE ), wxNullBitmap, wxITEM_NORMAL, wxT("Abrir archivo de Audio"), wxEmptyString, NULL );
	
	Herramienta_Cerrar = Barra_Herramientas->AddTool( HERRAMIENTA_CERRAR_ID, wxT("Cerrar"), wxBitmap( wxT("BITMAP_TOOL_CLOSE"), wxBITMAP_TYPE_PNG_RESOURCE ), wxNullBitmap, wxITEM_NORMAL, wxT("Cerrar archivo de Audio Actual"), wxEmptyString, NULL );
	
	Herramienta_Device = Barra_Herramientas->AddTool( HERRAMIENTA_DEVICE_ID, wxT("Seleccionar API/Dispositivo"), wxBitmap( wxT("BITMAP_TOOL_DEVICE"), wxBITMAP_TYPE_PNG_RESOURCE ), wxNullBitmap, wxITEM_NORMAL, wxT("Cerrar archivo de Audio Actual"), wxEmptyString, NULL );
	
	Barra_Herramientas->Realize();
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PantallaBase::Evento_Close ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( PantallaBase::Evento_Size ) );
	Boton_Play->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PantallaBase::Evento_Play ), NULL, this );
	Boton_Pause->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PantallaBase::Evento_Pause ), NULL, this );
	Boton_Stop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PantallaBase::Evento_Stop ), NULL, this );
	Menu_Archivo->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PantallaBase::Evento_Archivo_Abrir ), this, Menu_Archivo_Abrir->GetId());
	Menu_Archivo->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PantallaBase::Evento_Archivo_Cerrar ), this, Menu_Archivo_Cerrar->GetId());
	Menu_Archivo->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PantallaBase::Evento_Archivo_Salir ), this, Menu_Archivo_Salir->GetId());
	Menu_Herramientas->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PantallaBase::Evento_Herramientas_Device ), this, Menu_Herramientas_Device->GetId());
	Menu_Ayuda->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PantallaBase::Evento_Ayuda_Acerca ), this, Menu_Ayuda_Acerca->GetId());
	this->Connect( Herramienta_Abrir->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( PantallaBase::Evento_Tool_Abrir ) );
	this->Connect( Herramienta_Cerrar->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( PantallaBase::Evento_Tool_Cerrar ) );
	this->Connect( Herramienta_Device->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( PantallaBase::Evento_Tool_Device ) );
}

PantallaBase::~PantallaBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PantallaBase::Evento_Close ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( PantallaBase::Evento_Size ) );
	Boton_Play->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PantallaBase::Evento_Play ), NULL, this );
	Boton_Pause->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PantallaBase::Evento_Pause ), NULL, this );
	Boton_Stop->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PantallaBase::Evento_Stop ), NULL, this );
	this->Disconnect( Herramienta_Abrir->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( PantallaBase::Evento_Tool_Abrir ) );
	this->Disconnect( Herramienta_Cerrar->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( PantallaBase::Evento_Tool_Cerrar ) );
	this->Disconnect( Herramienta_Device->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( PantallaBase::Evento_Tool_Device ) );
	
}

DialogoDevice::DialogoDevice( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* Sizer_Dialogo;
	Sizer_Dialogo = new wxBoxSizer( wxVERTICAL );

	Contenedor = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* Sizer_Contenedor;
	Sizer_Contenedor = new wxBoxSizer( wxVERTICAL );

	Etiqueta_Host = new wxStaticText( Contenedor, wxID_ANY, wxT("Selecciona una API de salida para el Reproductor"), wxDefaultPosition, wxDefaultSize, 0 );
	Etiqueta_Host->Wrap( -1 );
	Etiqueta_Host->SetFont( wxFont( 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	Sizer_Contenedor->Add( Etiqueta_Host, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 12 );

	Combo_Host = new wxComboBox( Contenedor, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, wxCB_READONLY );
	Combo_Host->SetFont( wxFont( 10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	Sizer_Contenedor->Add( Combo_Host, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 12 );

	Etiqueta_Dipositivo = new wxStaticText( Contenedor, wxID_ANY, wxT("Selecciona un Dispositivo para la salida de Audio"), wxDefaultPosition, wxDefaultSize, 0 );
	Etiqueta_Dipositivo->Wrap( -1 );
	Etiqueta_Dipositivo->SetFont( wxFont( 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	Sizer_Contenedor->Add( Etiqueta_Dipositivo, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 12 );

	Combo_Device = new wxComboBox( Contenedor, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, wxCB_READONLY );
	Combo_Device->SetFont( wxFont( 10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	Sizer_Contenedor->Add( Combo_Device, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 12 );

	wxBoxSizer* Sizer_Botones;
	Sizer_Botones = new wxBoxSizer( wxHORIZONTAL );

	Boton_Aceptar = new wxButton( Contenedor, wxID_ANY, wxT("Aceptar"), wxDefaultPosition, wxDefaultSize, 0 );

	Boton_Aceptar->SetDefault();
	Boton_Aceptar->SetFont( wxFont( 10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	Sizer_Botones->Add( Boton_Aceptar, 0, wxEXPAND|wxRIGHT, 12 );

	Boton_Cancelar = new wxButton( Contenedor, wxID_ANY, wxT("Cancelar"), wxDefaultPosition, wxDefaultSize, 0 );
	Boton_Cancelar->SetFont( wxFont( 10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	Sizer_Botones->Add( Boton_Cancelar, 0, wxEXPAND, 0 );


	Sizer_Contenedor->Add( Sizer_Botones, 1, wxALIGN_RIGHT|wxALL, 12 );


	Contenedor->SetSizer( Sizer_Contenedor );
	Contenedor->Layout();
	Sizer_Contenedor->Fit( Contenedor );
	Sizer_Dialogo->Add( Contenedor, 1, wxEXPAND, 0 );


	this->SetSizer( Sizer_Dialogo );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	Combo_Host->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DialogoDevice::Evento_Combo_Host ), NULL, this );
	Boton_Aceptar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoDevice::Evento_Boton_Aceptar ), NULL, this );
	Boton_Cancelar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoDevice::Evento_Boton_Cancelar ), NULL, this );
}

DialogoDevice::~DialogoDevice()
{
	// Disconnect Events
	Combo_Host->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DialogoDevice::Evento_Combo_Host ), NULL, this );
	Boton_Aceptar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoDevice::Evento_Boton_Aceptar ), NULL, this );
	Boton_Cancelar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoDevice::Evento_Boton_Cancelar ), NULL, this );

}

///////////////////////////////////////////////////////////////////////////

DialogoAcerca::DialogoAcerca( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* Sizer_Dialogo;
	Sizer_Dialogo = new wxBoxSizer( wxVERTICAL );
	
	Contenedor = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Contenedor->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	wxBoxSizer* Sizer_Contenedor;
	Sizer_Contenedor = new wxBoxSizer( wxVERTICAL );
	
	Bitmap_Creator = new wxStaticBitmap( Contenedor, wxID_ANY, wxBitmap( wxT("BITMAP_CREATOR"), wxBITMAP_TYPE_PNG_RESOURCE ), wxDefaultPosition, wxSize( 170,170 ), 0 );
	Sizer_Contenedor->Add( Bitmap_Creator, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	Etiqueta_Autor = new wxStaticText( Contenedor, wxID_ANY, wxString("Desarrollado por : José Raúl Moreno Acero (Tyrel)"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	Etiqueta_Autor->Wrap( -1 );
	Etiqueta_Autor->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );
	
	Sizer_Contenedor->Add( Etiqueta_Autor, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* Sizer_Logos;
	Sizer_Logos = new wxBoxSizer( wxHORIZONTAL );
	
	Bitmap_Logotipo = new wxStaticBitmap( Contenedor, wxID_ANY, wxBitmap( wxT("BITMAP_LOGOTIPO"), wxBITMAP_TYPE_PNG_RESOURCE ), wxDefaultPosition, wxSize( 100,100 ), 0 );
	Sizer_Logos->Add( Bitmap_Logotipo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	Etiqueta_Aplicacion = new wxStaticText( Contenedor, wxID_ANY, wxString("<b>ElectrodeMP - Audio Media Player Alpha</b>\n\nUn simple reproductor que se expandirá con nuevas funciones sacadas de la imaginación. Lo importante es divertirse y disfrutar acompañado de las mejores personas.\n"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	Etiqueta_Aplicacion->SetLabelMarkup( wxString("<b>ElectrodeMP - Audio Media Player Alpha</b>\n\nUn simple reproductor que se expandirá con nuevas funciones sacadas de la imaginación. Lo importante es divertirse y disfrutar acompañado de las mejores personas.\n") );
	Etiqueta_Aplicacion->Wrap( -1 );
	Etiqueta_Aplicacion->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	Sizer_Logos->Add( Etiqueta_Aplicacion, 0, wxALL|wxEXPAND, 12 );
	
	
	Sizer_Contenedor->Add( Sizer_Logos, 1, wxALL|wxEXPAND, 10 );
	
	Etiqueta_Contacto = new wxStaticText( Contenedor, wxID_ANY, wxString("Para cualquier duda o aclaración puede hacerlo\nen la siguiente dirección : <b>jrma_tyrel@hotmail.com</b>"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	Etiqueta_Contacto->SetLabelMarkup( wxString("Para cualquier duda o aclaración puede hacerlo\nen la siguiente dirección : <b>jrma_tyrel@hotmail.com</b>") );
	Etiqueta_Contacto->Wrap( -1 );
	Etiqueta_Contacto->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	Sizer_Contenedor->Add( Etiqueta_Contacto, 0, wxALL|wxEXPAND, 5 );
	
	Boton_Cerrar = new wxButton( Contenedor, wxID_ANY, wxT("Cerrar"), wxDefaultPosition, wxDefaultSize, 0 );
	Sizer_Contenedor->Add( Boton_Cerrar, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	
	Contenedor->SetSizer( Sizer_Contenedor );
	Contenedor->Layout();
	Sizer_Contenedor->Fit( Contenedor );
	Sizer_Dialogo->Add( Contenedor, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( Sizer_Dialogo );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	Boton_Cerrar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoAcerca::Evento_Cerrar ), NULL, this );
}

DialogoAcerca::~DialogoAcerca()
{
	// Disconnect Events
	Boton_Cerrar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogoAcerca::Evento_Cerrar ), NULL, this );
	
}
