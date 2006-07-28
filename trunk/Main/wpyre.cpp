/////////////////////////////////////////////////////////////////////////////
// Modified form minimal.cpp form Julian Smart
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources and even
// though we could still include the XPM here it would be unused)
#if !defined(__WXMSW__) && !defined(__WXPM__)
    #include "../sample.xpm"
#endif

#include <string.h>
#include "expat.h"
#include <wx/image.h>
#include <wx/dcbuffer.h>

char xml_Buff[8192];
int xml_Depth;
FILE *xml_fp;
char xml_temp[256];
int xml_temp_num=0;
int resx=0,resy=0;
int mousex, mousey;

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
	void OnMouseLeftDoubleClick(wxMouseEvent& event);
	void OnMouseLeftDown(wxMouseEvent& event);
private:
    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title);

    // event handlers (these functions should _not_ be virtual)
	void OnActivate(wxActivateEvent& event);
	void OnPageChanged(wxBookCtrlEvent &event);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnPaint( wxPaintEvent &WXUNUSED(event) );
	void OnMouseLeftDoubleClick(wxMouseEvent& event);
	void OnMouseLeftDown(wxMouseEvent& event);
	
	void Needer_OnButtonOpen(wxCommandEvent& WXUNUSED(event));
    void Needer_OnButtonUpload(wxCommandEvent& WXUNUSED(event));
    void Needer_OnButtonResult(wxCommandEvent& WXUNUSED(event));
    void Needer_OnButtonQuit(wxCommandEvent& WXUNUSED(event));
	void Needer_ReDo(int x, int y);

	int Waiter_OnButtonRun_Sub();
    void Waiter_OnButtonRun(wxCommandEvent& WXUNUSED(event));
    void Waiter_OnButtonResult(wxCommandEvent& WXUNUSED(event));
    void Waiter_OnButtonQuit(wxCommandEvent& WXUNUSED(event));
	
	void ExeCmdline(char *cmd);
    void HttpAddress(char *http_addr, char *str);
	void deal_xml();

private:
    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
private:
    //Do not add custom control declarations
    //wxDev-C++ will remove them. Add custom code after the block.
    ////GUI Control Declaration Start
    wxBookCtrl *m_book;
	
	wxStaticText *Needer_WxStaticTextFilename;
    wxStaticText *Needer_WxStaticTextID;
    wxButton *Needer_WxButtonOpen;
    wxButton *Needer_WxButtonUpload;
    wxButton *Needer_WxButtonResult;
    wxButton *Needer_WxButtonQuit;
    wxTextCtrl *Needer_WxTextFilename;
    wxTextCtrl *Needer_WxTextID;
    wxTextCtrl *Needer_WxTextLog;

    wxStaticText *Waiter_WxStaticTextID;
    wxButton *Waiter_WxButtonRun;
    wxButton *Waiter_WxButtonResult;
    wxButton *Waiter_WxButtonQuit;
    wxTextCtrl *Waiter_WxTextID;
    wxTextCtrl *Waiter_WxTextLog;
    wxCheckBox *Waiter_WxCheckAuto;

	wxImage Needer_image;
	wxImage Waiter_image;

	wxPanel *Needer_Panel;
	wxPanel *Waiter_Panel;
			
	char TextFilename[512];
    char TextID[16];

	char NeederImage[24];
	char WaiterImage[24];
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Minimal_About = wxID_ABOUT,

    ID_BOOK = 500,

	ID_Needer_StaticTextFilename = 1000,
    ID_Needer_StaticTextID = 1001,
    ID_Needer_ButtonOpen = 1002,
    ID_Needer_ButtonUpload = 1003,
    ID_Needer_ButtonResult = 1004,
    ID_Needer_ButtonQuit = 1005,
    ID_Needer_TextFilename = 1006,
    ID_Needer_TextID = 1007,
    ID_Waiter_StaticTextID = 2000,
    ID_Waiter_ButtonRun = 2001,
    ID_Waiter_ButtonResult = 2002,
    ID_Waiter_ButtonQuit = 2003,
    ID_Waiter_TextID = 2004,
    ID_Waiter_CheckAuto = 2005,

    ////GUI Enum Control ID End
    ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values

};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.

BEGIN_EVENT_TABLE(MyApp, wxApp)
	EVT_LEFT_DCLICK(MyApp::OnMouseLeftDoubleClick)
	EVT_LEFT_DOWN(MyApp::OnMouseLeftDown)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_ACTIVATE(MyFrame::OnActivate)
	EVT_BOOKCTRL_PAGE_CHANGED(ID_BOOK, MyFrame::OnPageChanged)

	EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
	EVT_MENU(Minimal_About, MyFrame::OnAbout)

	EVT_PAINT(MyFrame::OnPaint)
	EVT_LEFT_DCLICK(MyFrame::OnMouseLeftDoubleClick)
	EVT_LEFT_DOWN(MyFrame::OnMouseLeftDown)

	EVT_BUTTON(ID_Needer_ButtonOpen, MyFrame::Needer_OnButtonOpen)
	EVT_BUTTON(ID_Needer_ButtonUpload, MyFrame::Needer_OnButtonUpload)
	EVT_BUTTON(ID_Needer_ButtonResult, MyFrame::Needer_OnButtonResult)
	EVT_BUTTON(ID_Needer_ButtonQuit, MyFrame::Needer_OnButtonQuit)

	EVT_BUTTON(ID_Waiter_ButtonRun, MyFrame::Waiter_OnButtonRun)
	EVT_BUTTON(ID_Waiter_ButtonResult, MyFrame::Waiter_OnButtonResult)
	EVT_BUTTON(ID_Waiter_ButtonQuit, MyFrame::Waiter_OnButtonQuit)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------
    MyFrame *frame;
// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // create the main application window
	frame = new MyFrame(_T("Web based Parallel Rendering Environment"));

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

void MyFrame::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
	wxPaintDC dc_Needer( Needer_Panel );
	wxPaintDC dc_Waiter( Waiter_Panel );
	wxPaintDC dc_MyFrame( this );

 	Waiter_image.Rescale(279* Needer_image.GetWidth()/Needer_image.GetHeight(), 279);
 	Needer_image.Rescale(279* Needer_image.GetWidth()/Needer_image.GetHeight(), 279);
	dc_Waiter.DrawBitmap( Waiter_image, 357, 8);
	dc_Needer.DrawBitmap( Needer_image, 357, 8);

}

void MyFrame::OnMouseLeftDown(wxMouseEvent& event)
{
	wxClientDC dc_Waiter(Waiter_Panel);
	dc_Waiter.DrawBitmap( Waiter_image, 357, 8);


	event.GetPosition(&mousex, &mousey);

	int box_width=279*Needer_image.GetWidth()/Needer_image.GetHeight()/10;
	int box_height=279/10;

	int blank_x=21;
	int blank_y=8;
	wxClientDC dc_Needer(Needer_Panel);
    if( (mousex-blank_x)/box_width+1<20 && (mousex-blank_x)/box_width+1>9 && (mousey-blank_y)/box_height+1 > 0&& (mousey-blank_y)/box_height+1 < 11)
	{
//	*Needer_WxTextLog << "Grid (";
//	*Needer_WxTextLog << (mousex-blank_x)/box_width-8;
//	*Needer_WxTextLog << ",";
//	*Needer_WxTextLog << (mousey-blank_y)/box_height+1;
//	*Needer_WxTextLog << ") is selected.\n";
		dc_Needer.DrawBitmap( Needer_image, 357, 8);
		mousey=(int)((mousey-blank_y)/box_height)*box_height+blank_y;
		mousex=(int)((float)(int)((float)(mousex-blank_x)/box_width)/10*Needer_image.GetWidth())+blank_x;
		dc_Needer.SetPen( wxPen(*wxLIGHT_GREY, 1, wxDOT_DASH) );
		dc_Needer.DrawLine(mousex,				mousey,				mousex,				mousey+box_height);
		dc_Needer.DrawLine(mousex,				mousey,				mousex+box_width,	mousey);
		dc_Needer.DrawLine(mousex+box_width-1,	mousey+box_height-1,	mousex,				mousey+box_height-1);
		dc_Needer.DrawLine(mousex+box_width-1,	mousey+box_height-1,	mousex+box_width-1,	mousey-1);
	}
}

void MyFrame::OnMouseLeftDoubleClick(wxMouseEvent& event)
{
	event.GetPosition(&mousex, &mousey);


	int box_width=279*Needer_image.GetWidth()/Needer_image.GetHeight()/10;
	int box_height=279/10;

	int blank_x=21;
	int blank_y=8;
	wxClientDC dc_Needer(Needer_Panel);
	wxClientDC dc_Waiter(Waiter_Panel);

    if( (mousex-blank_x)/box_width+1<20 && (mousex-blank_x)/box_width+1>9 && (mousey-blank_y)/box_height+1 > 0&& (mousey-blank_y)/box_height+1 < 11)
	{
    *Needer_WxTextLog << "Redo grid (";
	*Needer_WxTextLog << (mousex-blank_x)/box_width-8;
    *Needer_WxTextLog << ",";
	*Needer_WxTextLog << (mousey-blank_y)/box_height+1;
    *Needer_WxTextLog << ").\n";
		dc_Needer.DrawBitmap( Needer_image, 357, 8);
		mousey=(int)((mousey-blank_y)/box_height)*box_height+blank_y;
		mousex=(int)((float)(int)((float)(mousex-blank_x)/box_width)/10*Needer_image.GetWidth())+blank_x;
		dc_Needer.SetPen( wxPen(*wxLIGHT_GREY, 1, wxDOT_DASH) );
		dc_Needer.DrawLine(mousex,				mousey,				mousex,				mousey+box_height);
		dc_Needer.DrawLine(mousex,				mousey,				mousex+box_width,	mousey);
		dc_Needer.DrawLine(mousex+box_width-1,	mousey+box_height-1,	mousex,				mousey+box_height-1);
		dc_Needer.DrawLine(mousex+box_width-1,	mousey+box_height-1,	mousex+box_width-1,	mousey-1);

	MyFrame::Needer_ReDo((mousex-blank_x)/box_width-8, (mousey-blank_y)/box_height+1);
	}
}

void MyApp::OnMouseLeftDown(wxMouseEvent& event)
{
	frame->OnMouseLeftDown(event);
	event.Skip();
}

void MyApp::OnMouseLeftDoubleClick(wxMouseEvent& event)
{
	frame->OnMouseLeftDoubleClick(event);
	event.Skip();
}
// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title)
{
    // set the frame icon
    SetIcon(wxICON(sample));
    SetSize(755,388);
    Center();

	m_book = new wxBookCtrl(this, ID_BOOK);
    Needer_Panel = new wxPanel(m_book);
		Needer_WxStaticTextFilename = new wxStaticText(Needer_Panel, ID_Needer_StaticTextFilename, wxT("File name:"), wxPoint(7,8), wxDefaultSize, 0, wxT("WxStaticTextID"));
		Needer_WxStaticTextID = new wxStaticText(Needer_Panel, ID_Needer_StaticTextID,  wxT("ID:"), wxPoint(7,40), wxDefaultSize, 0, wxT("WxStaticTextFilename"));
		Needer_WxTextFilename = new wxTextCtrl(Needer_Panel, ID_Needer_TextFilename, wxT("..\\Test\\source.7z"), wxPoint(80,8), wxSize(265,21), 0, wxDefaultValidator, wxT("WxTextFilename"));
		Needer_WxTextID = new wxTextCtrl(Needer_Panel, ID_Needer_TextID, wxT(""), wxPoint(80,40), wxSize(265,21), 0, wxDefaultValidator, wxT("WxTextID"));
		Needer_WxButtonOpen = new wxButton(Needer_Panel, ID_Needer_ButtonOpen, wxT("Select a file"), wxPoint(7,72), wxSize(75,21), 0, wxDefaultValidator, wxT("WxButtonOpen"));
		Needer_WxButtonUpload = new wxButton(Needer_Panel, ID_Needer_ButtonUpload, wxT("Upload"), wxPoint(95,72), wxSize(75,21), 0, wxDefaultValidator, wxT("WxButtonUpload"));
		Needer_WxButtonResult = new wxButton(Needer_Panel, ID_Needer_ButtonResult, wxT("Result"), wxPoint(183,72), wxSize(75,21), 0, wxDefaultValidator, wxT("WxButtonResult"));
		Needer_WxButtonQuit = new wxButton(Needer_Panel, ID_Needer_ButtonQuit, wxT("Quit"), wxPoint(271,72), wxSize(75,21), 0, wxDefaultValidator, wxT("WxButtonQuit"));
		Needer_WxTextLog = new wxTextCtrl( Needer_Panel, wxID_ANY, _T("Pannel started.\n"), wxPoint(6,100), wxSize(339,188), wxTE_MULTILINE | wxTE_READONLY /* | wxTE_RICH */);
	m_book->AddPage(Needer_Panel, _T("Needer"), true, -1);

    Waiter_Panel = new wxPanel(m_book);
	    Waiter_WxStaticTextID = new wxStaticText(Waiter_Panel, ID_Waiter_StaticTextID,  wxT("ID:"), wxPoint(7,8), wxDefaultSize, 0, wxT("WxStaticTextFilename"));
	    Waiter_WxTextID = new wxTextCtrl(Waiter_Panel, ID_Waiter_TextID, wxT(""), wxPoint(80,8), wxSize(265,21), 0, wxDefaultValidator, wxT("WxTextID"));
		Waiter_WxCheckAuto = new wxCheckBox(Waiter_Panel, ID_Waiter_CheckAuto, wxT("Auto"), wxPoint(7,42), wxDefaultSize, 0, wxDefaultValidator, wxT("checkBox"));
	    Waiter_WxButtonRun = new wxButton(Waiter_Panel, ID_Waiter_ButtonRun, wxT("Run"), wxPoint(95,40), wxSize(75,21), 0, wxDefaultValidator, wxT("WxButtonRun"));
		Waiter_WxButtonResult = new wxButton(Waiter_Panel, ID_Waiter_ButtonResult, wxT("Result"), wxPoint(183,40), wxSize(75,21), 0, wxDefaultValidator, wxT("WxButtonResult"));
		Waiter_WxButtonQuit = new wxButton(Waiter_Panel, ID_Waiter_ButtonQuit, wxT("Quit"), wxPoint(271,40), wxSize(75,21), 0, wxDefaultValidator, wxT("WxButtonQuit"));
		Waiter_WxTextLog = new wxTextCtrl( Waiter_Panel, wxID_ANY, _T("Pannel started.\n"), wxPoint(6,70), wxSize(339,218), wxTE_MULTILINE | wxTE_READONLY /* | wxTE_RICH */);
	m_book->AddPage(Waiter_Panel, _T("Waiter"), true, -1);

	strcpy(NeederImage, "blank.png");
	strcpy(WaiterImage, "blank.png");
	wxInitAllImageHandlers();
 	Needer_image.LoadFile( NeederImage );
 	Needer_image.Rescale(279* Needer_image.GetWidth()/Needer_image.GetHeight(), 279);
 	Waiter_image.LoadFile( WaiterImage );
 	Waiter_image.Rescale(279* Waiter_image.GetWidth()/Waiter_image.GetHeight(), 279);

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *fileMenu = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, _T("&About...\tF1"), _T("Show about dialog"));

    fileMenu->Append(Minimal_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, _T("&File"));
    menuBar->Append(helpMenu, _T("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText(_T("Web based Parallel Rendering Environment started."));
#endif // wxUSE_STATUSBAR
}


// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnActivate(wxActivateEvent& event)
{
    // true is to force the frame to close
	wxPaintDC dc_Needer( Needer_Panel );
	wxPaintDC dc_Waiter( Waiter_Panel );
	dc_Waiter.DrawBitmap( Waiter_image, 357, 8);
	dc_Needer.DrawBitmap( Needer_image, 357, 8);

}
void MyFrame::OnPageChanged(wxBookCtrlEvent &event)
{
    // true is to force the frame to close
	wxPaintDC dc_Needer( Needer_Panel );
	wxPaintDC dc_Waiter( Waiter_Panel );
	dc_Waiter.DrawBitmap( Waiter_image, 357, 8);
	dc_Needer.DrawBitmap( Needer_image, 357, 8);

}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( _T("Web based Parallel Rendering Environment.\n")
                _T("Homepage: http://projects.blender.org/projects/wpyre\n\n")
                _T("Version: 0.01"));

    wxMessageBox(msg, _T("About Minimal"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::Needer_OnButtonOpen(wxCommandEvent& WXUNUSED(event))
{
    wxString Filename = wxFileSelector("Select a file", "", "", "", "*.7z");
    //  *WxTextLog << _T(Filename);
	if(strlen(Filename))
	{
		Needer_WxTextFilename->Clear();
		*Needer_WxTextFilename << _T(Filename);
		strncpy(TextFilename, Filename.mb_str(wxConvUTF8), 512);
	}
    //	ExeCmdline("dir");
}

void xml_start(void *data, const char *el, const char **attr)
{
    int i;

	//deal with 'outfile' tag
		if(!strcmp("outfile", el))
		return;

    for (i = 0; i < xml_Depth; i++)
        fprintf(xml_fp, "\t");
        //printf("  ");
    //printf("<%s>", el);
    fprintf(xml_fp, "<%s", el);
    strcpy(xml_temp, el);
    xml_temp_num=(int)attr[0];
    if(attr[0])
    {
        for (i = 0; attr[i]; i += 2)
        {
			//deal with 'cache' tag
			if(!strcmp("filename", el))
				if(!strcmp("value", attr[i]))
					strcpy((char *)attr[i + 1], strrchr(attr[i + 1], '/')+1);
			
			//deal with 'cache' tag
			if(!strcmp("cache", attr[i]))
				fprintf(xml_fp, " cache=\"off\"");
			else
				fprintf(xml_fp, " %s=\"%s\"", attr[i], attr[i + 1]);

			//deal with 'resx' tag
			if(!strcmp("resx", attr[i]))
			{
				printf("%s", attr[i + 1]);
				resx=atoi(attr[i+1]);
			}
			//deal with 'resx' tag
			if(!strcmp("resy", attr[i]))
			{
				printf(" * %s\n", attr[i + 1]);
				resy=atoi(attr[i+1]);
			}
		}
        fprintf(xml_fp, ">\n");
    }
    else
        fprintf(xml_fp, ">\n");

    //printf("\n");
    //fprintf(xml_fp, "\n");
    xml_Depth++;
}  /* End of start handler */

void xml_end(void *data, const char *el)
{
    int i;

	//deal with 'outfile' tag
	if(!strcmp("outfile", el))
		return;

    if(strcmp(xml_temp, el) || !xml_temp_num)
    {
        for (i = 0; i < xml_Depth-1; i++)
            //printf("  ");
            fprintf(xml_fp, "\t");

        //printf("</%s>", el);
        fprintf(xml_fp, "</%s>", el);
    }
    else
    {
		if(xml_Depth==2)
		{
			for (i = 0; i < xml_Depth-1; i++)
				//printf("  ");
				fprintf(xml_fp, "\t");

			//printf("</%s>", el);
			fprintf(xml_fp, "</%s>", el);
		}
		else
		{
		fseek(xml_fp, -3, SEEK_CUR);
        fprintf(xml_fp, "/>");
		}
    }
    //printf("\n");
    fprintf(xml_fp, "\n");

    xml_Depth--;
}  /* End of end handler */

void MyFrame::deal_xml()
{
    FILE *fp_in;

    XML_Parser xml_p = XML_ParserCreate(NULL);

    if((fp_in = fopen("temp\\source.xml","r"))==NULL)
	{
		*Needer_WxTextLog << "source";
		//printf("source");
		return;
	}
	if((xml_fp=fopen("temp\\output.xml","w"))==NULL)
	{
		*Needer_WxTextLog << "output";
		//printf("output");
		return;
	}

	if (! xml_p)
    {
		*Needer_WxTextLog << "Couldn't allocate memory for parser\n";
//        fprintf(stderr, "Couldn't allocate memory for parser\n");
        exit(-1);
    }

    XML_SetElementHandler(xml_p, xml_start, xml_end);

    for (;;)
    {
        int xml_done;
        int xml_len;

        xml_len = fread(xml_Buff, 1, 8192, fp_in);
        if (ferror(fp_in))
        {
			*Needer_WxTextLog << "Read error\n";
//            fprintf(stderr, "Read error\n");
            exit(-1);
        }
        xml_done = feof(fp_in);

        if (! XML_Parse(xml_p, xml_Buff, xml_len, xml_done))
        {
			*Needer_WxTextLog << "Parse error at line ";
            fprintf(stderr, "Parse error at line %d:\n%s\n",
                    XML_GetCurrentLineNumber(xml_p),
                    XML_ErrorString(XML_GetErrorCode(xml_p)));
            exit(-1);
        }

        if (xml_done)
        {
            fclose(fp_in);
            fclose(xml_fp);
            break;
        }
    }
}

void MyFrame::Needer_OnButtonUpload(wxCommandEvent& WXUNUSED(event))
{
    FILE *fp;
    //char filename[256];
    char upresult[8];
    char id[16];
    char web[256];
    char line[256];

	strncpy(TextFilename, Needer_WxTextFilename->GetValue().mb_str(wxConvUTF8), 512);
    if((fp=fopen("neederconf.txt", "r"))==NULL)
    {
        *Needer_WxTextLog << "neederconf.txt does not exist.\n";
        return;
    }
    else
    {
        fgets(web, 64, fp);
        *Needer_WxTextLog << "Web site: ";
        *Needer_WxTextLog << web;
        *Needer_WxTextLog << "\n";
        fclose(fp);
    }

    if((fp=fopen(TextFilename, "r"))==NULL)
    {
        *Needer_WxTextLog << TextFilename;
		*Needer_WxTextLog << " does not exist.\n";
		return;
    }
    else
    {
        fclose(fp);
		//1. deal-xml.bat
		if((fp=fopen("extract-source.bat", "w+"))==NULL)
        {
            *Needer_WxTextLog << "Creating upload script failed.\n";
        }
        else
        {
            fprintf(fp, "mkdir temp\n");
            fprintf(fp, "del temp /q\n");
			fprintf(fp, "7z x -y -otemp \"%s\"\n", TextFilename);
            fclose(fp);
            ExeCmdline("extract-source.bat");
			
			MyFrame::deal_xml();
			if(resx==0)
				resx=1024;
			if(resy==0)
				resy=768;
            *Needer_WxTextLog << "Resolution: ";
            *Needer_WxTextLog << resx;
            *Needer_WxTextLog << " * ";
			*Needer_WxTextLog << resy;
            *Needer_WxTextLog << "\n";

			//printf("%d %d", resx, resy);
		}
		//2.compress-source.bat
		if((fp=fopen("compress-source.bat", "w+"))==NULL)
        {
            *Needer_WxTextLog << "Creating compress script failed.\n";
        }
        else
        {
            fprintf(fp, "cd temp\n");
			fprintf(fp, "copy /y output.xml source.xml\n");
			fprintf(fp, "..\\7z a -y source.7z *.*\n");
            fclose(fp);
            ExeCmdline("compress-source.bat");
		}
		//3. needer-upload.bat
        if((fp=fopen("needer-upload.bat", "w+"))==NULL)
        {
            *Needer_WxTextLog << "Creating upload script failed.\n";
        }
        else
        {
            strcpy(line, "curl -o upresult.txt -F MAX_FILE_SIZE='104857600' -F userfile=@\"temp\\source.7z\" \"http://localhost/needer-up.php?resx=%d&resy=%d\"");
            HttpAddress((char *)&web, (char *)&line);
            fprintf(fp, line, resx, resy);

            fclose(fp);
            *Needer_WxTextLog << "Upload script is created successfully.\n";
            *Needer_WxTextLog << "Uploading...\n";

            ExeCmdline("needer-upload.bat");
            if((fp=fopen("upresult.txt", "r"))==NULL)
                *Needer_WxTextLog << "Upload failed. Try to examine Curl.\n";
            else
            {
                fscanf(fp, "%s", upresult);
                fscanf(fp, "%s", id);
                fclose(fp);
                if(!strcmp(upresult, "OK"))
                {
                    *Needer_WxTextLog << "Upload successfully.\n";
                    *Needer_WxTextLog << "ID is: ";
                    *Needer_WxTextLog << id;
                    *Needer_WxTextLog << "\n";

                    Needer_WxTextID->Clear();
                    *Needer_WxTextID << id;
                    strncpy(TextID, id, 16);
                }
                else if(!strcmp(upresult, "ERROR"))
                    *Needer_WxTextLog << "Upload failed. Please try again.\n";
                else
                    *Needer_WxTextLog << "Can't get a result.\n";

            }
        }
    }
}

void MyFrame::Needer_OnButtonResult(wxCommandEvent& WXUNUSED(event))
{
    FILE *fp;
    FILE *fp1;
    FILE *fp2;
	
    char temp[512];
    char filename[512];
    char web[256];
    char line[256];

    strncpy(TextID, Needer_WxTextID->GetValue().Trim(true).Trim(false), 16);
    if((fp=fopen("neederconf.txt", "r"))==NULL)
    {
        *Needer_WxTextLog << "neederconf.txt does not exist.\n";
		return;
    }
    else
    {
        fgets(web, 64, fp);
        *Needer_WxTextLog << "Web site: ";
        *Needer_WxTextLog << web;
        *Needer_WxTextLog << "\n";

        fclose(fp);
    }

    if((fp=fopen("get-result.bat", "w+"))==NULL)
    {
        *Needer_WxTextLog << "Creating get result script failed.\n";
    }
    else
    {
        //curl -F MAX_FILE_SIZE='104857600' -F userfile=@YBtest.xml http://localhost/needer-up.php
        fprintf(fp, "mkdir %s\n", TextID);
        fprintf(fp, "copy /y %s\\upload.txt %s\\oldupload.txt\n", TextID, TextID);
        //fprintf(fp, "curl -O http://localhost/%s/upload.txt\n", TextID);
        strcpy(line, "curl -o %s/upload.txt http://localhost/%s/upload.txt\n");
        HttpAddress((char *)&web, (char *)&line);
        fprintf(fp, line, TextID, TextID);

        fclose(fp);
        *Needer_WxTextLog << "Get result script is created successfully.\n";
        *Needer_WxTextLog << "Downloading...\n";

        ExeCmdline("get-result.bat");
        *Needer_WxTextLog << "Download successfully.\n";

        strcpy(filename, TextID);
        strcat(filename, "\\upload.txt");

        if((fp=fopen(filename, "r"))==NULL)
        {
            *Needer_WxTextLog << "Open upload.txt failed.\n";
        }
        else
        {
            if((fp1=fopen("combine.bat", "w+"))==NULL)
            {
                *Needer_WxTextLog << "Creating combine script failed.\n";
            }
            else
            {
                *Needer_WxTextLog << "Creating combine script...\n";

                if(!feof(fp))
                {
                    fgets(filename, 1024, fp);
                    if(strchr(filename, '\r'))
                        ((char *)strchr(filename, '\r'))[0]='\0';
                    if(strchr(filename, '\n'))
                        ((char *)strchr(filename, '\n'))[0]='\0';
                    //printf("%s", filename);
                    //fprintf(fp1, "curl -O http://localhost/%s/%s\n", TextID, filename);
                    strcpy(line, "curl -o %s/%s http://localhost/%s/%s\n");
                    HttpAddress((char *)&web, (char *)&line);
                    fprintf(fp1, line, TextID, filename, TextID, filename);

                    fprintf(fp1, "7z x -y -o%s \"%s\\%s\"\n", TextID, TextID, filename);
                    if(strchr(filename, '.'))
                        ((char *)strchr(filename, '.'))[0]='\0';
                    strcat(filename, ".tga");
                    fprintf(fp1, "copy /y \"%s\\%s\" %s\\final.tga\n", TextID, filename, TextID);
                    fprintf(fp1, "final-tga \"%s\\final.tga\" \"%s\\%s\"\n", TextID, TextID, filename);
                    fprintf(fp1, "final-tga \"%s\\final.tga\" \"%s\\%s\"\n", TextID, TextID, filename);

#ifdef DEBUG

                    fprintf(fp1, "pause\n");
#endif

                }
                while(!feof(fp))
                {
                    if (fgets(filename, 1024, fp) != NULL)
                    {
                        if(strchr(filename, '\r'))
                            ((char *)strchr(filename, '\r'))[0]='\0';
                        if(strchr(filename, '\n'))
                            ((char *)strchr(filename, '\n'))[0]='\0';

						printf("%s\n", filename);

						strcpy(temp, TextID);
						strcat(temp, "\\");
						strcat(temp, filename);
						fp2=fopen(temp, "r");
						if(fp2==NULL)
						{
							strcpy(line, "curl -o %s\\%s http://localhost/%s/%s\n");
							HttpAddress((char *)&web, (char *)&line);
							fprintf(fp1, line, TextID, filename, TextID, filename);
						}
						else if(fgetc(fp2)!='7')
						{
							strcpy(line, "curl -o %s\\%s http://localhost/%s/%s\n");
							HttpAddress((char *)&web, (char *)&line);
							fprintf(fp1, line, TextID, filename, TextID, filename);
							fclose(fp2);
						}
						else
							fclose(fp2);
						

                        strcpy(temp, TextID);
						strcat(temp, "\\");
						strcat(temp, filename);
						fp2=fopen(temp, "r");
						if(fp2==NULL)
							fprintf(fp1, "7z x -y -o%s \"%s\\%s\"\n", TextID, TextID, filename);
						else fclose(fp2);

						if(strchr(filename, '.'))
                            ((char *)strchr(filename, '.'))[0]='\0';
                        strcat(filename, ".tga");
                        fprintf(fp1, "final-tga \"%s\\final.tga\" \"%s\\%s\"\n", TextID, TextID, filename);

#ifdef DEBUG

                        fprintf(fp1, "pause\n");
#endif

                    }
                }
                fprintf(fp1, "final-tga %s\\final.tga -laststep\n",  TextID);


                fclose(fp1);
            }
            fclose(fp);
        }
    }
    *Needer_WxTextLog << "Getting and merging files...\n";

    ExeCmdline("combine.bat");

	strcpy(NeederImage, TextID);
	strcat(NeederImage, "\\final.png");
 	Needer_image.LoadFile( NeederImage );
 	Needer_image.Rescale(279* Needer_image.GetWidth()/Needer_image.GetHeight(), 279);
	Refresh();

    if((fp=fopen("needer-upload.bat", "w+"))==NULL)
    {
        *Needer_WxTextLog << "Creating upload script failed.\n";
		return;
    }
    else
    {
        //curl -F MAX_FILE_SIZE='104857600' -F userfile=@YBtest.xml http://localhost/needer-up.php
        fprintf(fp, "cd %s\n", TextID);
        fprintf(fp, "..\\7z a -y %s.7z final.tga final.png\n", TextID);
        fprintf(fp, "cd ..\n");
        //fprintf(fp, "curl -o upresult.txt -F MAX_FILE_SIZE='104857600' -F userfile=@final.7z http://localhost/needer-up.php?id=%s\n", TextID);
        strcpy(line, "curl -o upresult.txt -F MAX_FILE_SIZE='104857600' -F userfile=@%s\\%s.7z http://localhost/needer-up.php?id=%s\n");
        HttpAddress((char *)&web, (char *)&line);
        fprintf(fp, line, TextID, TextID, TextID);

        //fprintf(fp, "curl -o upresult.txt -F MAX_FILE_SIZE='104857600' -F userfile=@final.7z http://localhost/needer-up.php?id=%s\n", TextID);
        strcpy(line, "curl -o upresult.txt -F MAX_FILE_SIZE='104857600' -F userfile=@%s\\%s.7z http://localhost/needer-up.php?id=%s\n");
        HttpAddress((char *)&web, (char *)&line);
        fprintf(fp, line, TextID, TextID, TextID);

        fclose(fp);

        *Needer_WxTextLog << "Upload script is created successfully.\n";
        *Needer_WxTextLog << "Uploading...\n";

        ExeCmdline("needer-upload.bat");

        *Needer_WxTextLog << "Uploaded.\n";
    }
}

void MyFrame::Needer_OnButtonQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

//Waiter//





void MyFrame::Waiter_OnButtonRun(wxCommandEvent& WXUNUSED(event))
{
    if(false==Waiter_WxCheckAuto->GetValue())
	{
		Waiter_OnButtonRun_Sub();
	}
	else
	{
		while(Waiter_OnButtonRun_Sub()!=616);
	}
}

int MyFrame::Waiter_OnButtonRun_Sub()
{
    FILE *fp;
    FILE *fp1;
    char filename[256];
    char temp[256];
    char _7zname[256];
    char upresult[8];
    char yafraycmd[256];
    char *locate;
    char web[256];
    char line[256];

    strncpy(TextID, Waiter_WxTextID->GetValue().Trim(true).Trim(false), 16);
	if(!strlen(TextID))
    {
        *Waiter_WxTextLog << "Please input project ID.\n";
        return 0;
    }

    if((fp=fopen("waiterconf.txt", "r"))==NULL)
    {
        *Waiter_WxTextLog << "waiterconf.txt does not exist.\n";
        return 0;
    }
    else
    {
        fgets(web, 64, fp);
        *Waiter_WxTextLog << "Web site: ";
        *Waiter_WxTextLog << web;
        *Waiter_WxTextLog << "\n";
        fclose(fp);
    }

    if((fp=fopen("waiter-download.bat", "w+"))==NULL)
    {
        *Waiter_WxTextLog << "Creating download script failed.\n";
    }
    else
    {
        //curl -O http://localhost/source.7z.xml
        fprintf(fp, "mkdir %s\n", TextID);

        strcpy(filename, TextID);
        strcat(filename, "\\source.7z");
        if((fp1=fopen(filename, "r"))==NULL)
        {
			*Waiter_WxTextLog << "Because source.7z doesn't exists, I will download it now.\n";
			strcpy(line, "curl -o %s/source.7z http://localhost/%s/source.7z\n");
			HttpAddress((char *)&web, (char *)&line);
			fprintf(fp, line, TextID, TextID);
        }
        else if(fgetc(fp1)!='7')
        {
			*Waiter_WxTextLog << "Because source.7z is broken, I will download it again.\n";
			strcpy(line, "curl -o %s/source.7z http://localhost/%s/source.7z\n");
			HttpAddress((char *)&web, (char *)&line);
			fprintf(fp, line, TextID, TextID);
			fclose(fp1);
		}
        else
        {
		   *Waiter_WxTextLog << "Because source.7z exists, I won't download it again. ;)\n";
			fclose(fp1);
		}

#ifdef DEBUG
        fprintf(fp, "pause\n");
#endif

        fclose(fp);

        *Waiter_WxTextLog << "Download script is created successfully.\n";
        *Waiter_WxTextLog << "Downloading...\n";
        ExeCmdline("waiter-download.bat");

        strcpy(filename, TextID);
        strcat(filename, "\\source.7z");

        if((fp=fopen(filename, "r"))==NULL)
        {
            *Waiter_WxTextLog << "The project does not exist.\n";
            return 0;
        }
        else if(fgetc(fp)!='7')
        {
            *Waiter_WxTextLog << "Error occurs while initializing.\n";
            fclose(fp);
            return 0;
        }
        else
            fclose(fp);

        *Waiter_WxTextLog << "Unpackaging...\n";

        if((fp=fopen("unpackage.bat", "w+"))==NULL)
        {
            *Waiter_WxTextLog << "Creating unpackage script failed.\n";
        }
        else
        {
            fprintf(fp, "7z x -y -o%s %s\\source.7z\n", TextID, TextID);
#ifdef DEBUG

            fprintf(fp, "pause\n");
#endif

            fclose(fp);
            ExeCmdline("unpackage.bat");
            *Waiter_WxTextLog << "File Unpackaged.\n";

            *Waiter_WxTextLog << "Getting Yafray command line...\n";

            if((fp=fopen("get-yafray-cmd.bat", "w+"))==NULL)
            {
                *Waiter_WxTextLog << "Creating get script failed.\n";
            }
            else
            {
                //fprintf(fp, "curl -o %s/task.html http://localhost/task.php?id=%s\n", TextID, TextID);
                strcpy(line, "curl -o %s/task.html http://localhost/task.php?id=%s\n");
                HttpAddress((char *)&web, (char *)&line);
                fprintf(fp, line, TextID, TextID);

#ifdef DEBUG

                fprintf(fp, "pause\n");
#endif

                fclose(fp);
                ExeCmdline("get-yafray-cmd.bat");
                strcpy(filename, TextID);
                strcat(filename, "\\task.html");
                if((fp=fopen(filename, "r"))==NULL)
                {
                    *Waiter_WxTextLog << "Cant open task.html.\n";
                }
                else
                {
                    fgets(filename, 64, fp);
                    if(strstr(filename, "ERR: The task has been finished"))
                    {
                        *Waiter_WxTextLog << "The task has been finished already.\n";
                        return 616;
                    }
                    if(filename[0]<'0' || filename[0]>'9')
                    {
                        *Waiter_WxTextLog << "Getting yafray cmdline failed.\n";
                        return 0;
                    }
                    *Waiter_WxTextLog << "Yafray command line getted.\n";
                    *Waiter_WxTextLog << "Computing...\n";
                    if(filename[strlen(filename)-1]=='\r' || filename[strlen(filename)-1]=='\n')
                        filename[strlen(filename)-1]='\0';
                    if(filename[strlen(filename)-2]=='\r' || filename[strlen(filename)-2]=='\n')
                        filename[strlen(filename)-2]='\0';
                    fgets(yafraycmd, 128, fp);
                    fgets(yafraycmd, 128, fp);
                    if(yafraycmd[strlen(yafraycmd)-1]=='\r' || yafraycmd[strlen(yafraycmd)-1]=='\n')
                        yafraycmd[strlen(yafraycmd)-1]='\0';
                    if(filename[strlen(filename)-2]=='\r' || yafraycmd[strlen(yafraycmd)-2]=='\n')
                        yafraycmd[strlen(yafraycmd)-2]='\0';
                    fclose(fp);

                    if((fp=fopen("run-yafray-cmd.bat", "w+"))==NULL)
                    {
                        *Waiter_WxTextLog << "Creating run yafray script failed.\n";
                    }
                    else
                    {
                        fprintf(fp, "cd %s\n", TextID);
                        fprintf(fp, "PATH=%%PATH%%;..\\..\\YafRay\n");
                        fprintf(fp, "%s\n", yafraycmd);
                        strcpy(yafraycmd, "copy /y \"salida.tga\" \"");
                        strcat(yafraycmd, filename);
                        strcat(yafraycmd, ".tga\"\n");
                        fprintf(fp, yafraycmd);
                        fprintf(fp, "cd ..\n");
#ifdef DEBUG

                        fprintf(fp, "pause\n");
#endif

                        fclose(fp);
                        *Waiter_WxTextLog << "Running yafray...\n";
                        ExeCmdline("run-yafray-cmd.bat");

                        if((fp=fopen("compress.bat", "w+"))==NULL)
                        {
                            *Waiter_WxTextLog << "Creating compress script failed.\n";
                        }
                        else
                        {	//7z a YBtest.7z YBtest.tga
                            strcpy(_7zname, filename);
                            strcat(_7zname, ".7z");
                            fprintf(fp, "cd %s\n", TextID);
                            fprintf(fp, "..\\7z a -y \"%s\" \"%s.tga\"\n", _7zname, filename);
                            fprintf(fp, "cd ..\n");
#ifdef DEBUG

                            fprintf(fp, "pause\n");
#endif

                            fclose(fp);
                            ExeCmdline("compress.bat");

                            if((fp=fopen("waiter-upload.bat", "w+"))==NULL)
                            {
                                *Waiter_WxTextLog << "Creating upload script failed.\n";
                            }
                            else
                            {
                                //curl -F MAX_FILE_SIZE='104857600' -F userfile=@YBtest.xml http://localhost/up.php
                                locate=filename;
                                while(strchr(locate, ','))
                                {
                                    locate=strchr(locate, ',');
                                    strcpy(temp, locate);
                                    strcpy(locate+1, temp);
                                    *locate='\\';
                                    locate+=2;
                                }
                                printf("%s", filename);
                                //if i upload once, right reuslt return, but the file is not uploaded. I dont know why. when I upload again, it's ok, I think it is because of my patch on curl, it's dirty :)
                                //fprintf(fp, "curl -o upresult.txt -F MAX_FILE_SIZE='104857600' -F userfile=@%s/%s.7z http://localhost/waiter-up.php?id=%s\n", TextID, filename, TextID);
                                //fprintf(fp, "curl -o upresult.txt -F MAX_FILE_SIZE='104857600' -F userfile=@%s/%s.7z http://localhost/waiter-up.php?id=%s\n", TextID, filename, TextID);
                                //somedays later, dir will be added, tired of it today
                                //fprintf(fp, "curl -o %s/upresult.txt -F MAX_FILE_SIZE='104857600' -F userfile=@%s/%s.7z http://localhost/waiter-up.php?id=%s\n", TextID, TextID, filename, TextID);
                                strcpy(line, "curl -o %s/upresult.txt -F MAX_FILE_SIZE='104857600' -F userfile=@%s/%s.7z http://localhost/waiter-up.php?id=%s\n");
                                HttpAddress((char *)&web, (char *)&line);
                                fprintf(fp, line, TextID, TextID, filename, TextID);


                                //fprintf(fp, "curl -o %s/upresult.txt -F MAX_FILE_SIZE='104857600' -F userfile=@%s/%s.7z http://localhost/waiter-up.php?id=%s\n", TextID, TextID, filename, TextID);

                                strcpy(line, "curl -o %s/upresult.txt -F MAX_FILE_SIZE='104857600' -F userfile=@%s/%s.7z http://localhost/waiter-up.php?id=%s\n");
                                HttpAddress((char *)&web, (char *)&line);
                                fprintf(fp, line, TextID, TextID, filename, TextID);

#ifdef DEBUG

                                fprintf(fp, "pause\n");
#endif

                                fclose(fp);
                                *Waiter_WxTextLog << "Upload script is created successfully.\n";
                                *Waiter_WxTextLog << "Uploading...\n";
                                ExeCmdline("waiter-upload.bat");

                                strcpy(filename, TextID);
                                strcat(filename, "\\upresult.txt");
                                if((fp=fopen(filename, "r"))==NULL)
                                    *Waiter_WxTextLog << "Can't get a result.\n";
                                else
                                {
                                    fscanf(fp, "%s", upresult);
                                    if(!strcmp(upresult, "OK"))
                                        *Waiter_WxTextLog << "Upload successfully.\n";
                                    else if(!strcmp(upresult, "ERROR"))
                                        *Waiter_WxTextLog << "Upload failed.\n";
                                    else
                                        *Waiter_WxTextLog << "Can't get a result .\n";
                                }
                            }
                        }
                    }
                }

            }

        }
    }
	return 1;
}

void MyFrame::Waiter_OnButtonResult(wxCommandEvent& WXUNUSED(event))
{
    FILE *fp;
    char line[256];
    char web[256];

    strncpy(TextID, Waiter_WxTextID->GetValue().Trim(true).Trim(false), 16);
    if((fp=fopen("waiterconf.txt", "r"))==NULL)
    {
        *Waiter_WxTextLog << "waiterconf.txt does not exist.\n";
        return;
    }
    else
    {
        fgets(web, 64, fp);
        *Waiter_WxTextLog << "Web site: ";
        *Waiter_WxTextLog << web;
        *Waiter_WxTextLog << "\n";
        fclose(fp);
    }

        if((fp=fopen("waiter-download.bat", "w+"))==NULL)
    {
        *Waiter_WxTextLog << "Creating download script failed.\n";
    }
        else
    {

        strcpy(line, "curl -o%s/%s.7z http://localhost/%s/%s.7z\n");
        HttpAddress((char *)&web, (char *)&line);
        fprintf(fp, line, TextID, TextID, TextID, TextID);

        fprintf(fp, "7z x -y -o%s %s/%s.7z\n", TextID, TextID, TextID);
        fclose(fp);
        ExeCmdline("waiter-download.bat");
        strcpy(line, TextID);
        strcat(line, "\\");
        strcat(line, TextID);
        strcat(line, ".7z");
        if((fp=fopen(line, "r"))==NULL)
            *Waiter_WxTextLog << "Task has not been finished.\n";
        else
        {
            if(fgetc(fp)!='7')
                *Waiter_WxTextLog << "Task has not been finished.\n";
            else
                *Waiter_WxTextLog << "File downloaded.\n";
            fclose(fp);
        }

		strcpy(WaiterImage, TextID);
		strcat(WaiterImage, "\\final.png");
		Waiter_image.LoadFile( WaiterImage );
		Waiter_image.Rescale(279* Waiter_image.GetWidth()/Waiter_image.GetHeight(), 279);
		Refresh();

    }
}


void MyFrame::Waiter_OnButtonQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}




//Common//
void MyFrame::ExeCmdline(char *cmd)
{
    SHELLEXECUTEINFO ShExecInfo = {0};
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = NULL;
    ShExecInfo.lpFile=cmd;
    ShExecInfo.lpParameters = "";
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_SHOW;
    ShExecInfo.hInstApp = NULL;
    ShellExecuteEx(&ShExecInfo);
    WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
}

void MyFrame::HttpAddress(char *http_addr, char *str)
{
    char *location;
    char *replace_begin;

    if((int)(strlen("localhost")-strlen(http_addr)) >= 0)
    {
        location=(char *)strstr(str,"localhost")+strlen("localhost")-strlen(http_addr)-1;
        strcpy(location, location+strlen("localhost")-strlen(http_addr));
        strncpy(strstr(str,"http://")+7, http_addr, strlen(http_addr));
    }
    else
    {
        //printf("%s\n", str);
        replace_begin=(char *)strstr(str,"localhost")+strlen(http_addr)-strlen("localhost")-1;
        location=str+strlen(str)+strlen(http_addr)-strlen("localhost");
        for(; location>replace_begin; location--)
            *location=*(location-(strlen(http_addr)-strlen("localhost")));
        //printf("%s\n", str);
        strncpy(strstr(str,"http://")+7, http_addr, strlen(http_addr));
        //printf("%s\n", str);
    }
}

void MyFrame::Needer_ReDo(int x, int y)
{
	FILE *fp;
    char web[256];
    char line[256];

	strncpy(TextID, Needer_WxTextID->GetValue().Trim(true).Trim(false), 16);

	if(strlen(TextID) == 0)
	{
	    *Needer_WxTextLog << "Please Input ID\n";
		return;
	}

    if((fp=fopen("neederconf.txt", "r"))==NULL)
    {
        *Needer_WxTextLog << "neederconf.txt does not exist.\n";
		return;
    }
    else
    {
        fgets(web, 64, fp);
        *Needer_WxTextLog << "Web site: ";
        *Needer_WxTextLog << web;
        *Needer_WxTextLog << "\n";

        fclose(fp);
    }

		if((fp=fopen("redo.bat", "w+"))==NULL)
        {
            *Needer_WxTextLog << "Creating redo script failed.\n";
        }
        else
        {
        strcpy(line, "curl -o redoresult.txt \"http://localhost/redo.php?id=%s&x=%d&y=%d\"\n");
        HttpAddress((char *)&web, (char *)&line);
        fprintf(fp, line, TextID, x, y);
        ExeCmdline("extract-source.bat");
        *Needer_WxTextLog << "Job is sent.\n";
		
		fclose(fp);
		}
}
