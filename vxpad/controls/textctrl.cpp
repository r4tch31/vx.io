#include "textctrl.h"

// -- lexars --
#include "lexars/base.h"
#include "lexars/cpp.h"
#include "lexars/python.h"
#include "lexars/html.h"
#include "lexars/hlsl.h"
#include "lexars/nastran.h"

BEGIN_EVENT_TABLE(TextCtrl, wxStyledTextCtrl)
    //EVT_STC_MARGINCLICK (wxID_ANY,     TextCtrl::OnMarginClick)
    EVT_STC_CHARADDED (wxID_ANY,       TextCtrl::OnCharAdded)
    EVT_STC_KEY( wxID_ANY , TextCtrl::OnKey )
END_EVENT_TABLE()

TextCtrl::TextCtrl(wxWindow* parent, wxString filePath) : wxStyledTextCtrl(parent, wxID_ANY)
{

    //Set the File Path
    FilePath = filePath;

    //Read In File
    if(FilePath != _(""))
    {
        //LoadFile(FilePath);
        wxString str;

        // open the file
        wxTextFile tfile;
        if(wxFileName::FileExists(FilePath))
        {
            LoadFile(FilePath);
             IsNewFile = false;
        }
    }
        else
            wxMessageBox(_("Could Not Open File '")+FilePath+_("'"));

   ParseLexar();


    FileName = filePath;
    //Set File Name
    int StartIndex = FilePath.find_last_of(_("\\"),FilePath.length())+1;

        if(StartIndex != -1)
        {
            FileName = FilePath.substr(StartIndex,FilePath.length()-StartIndex);
        }
        else
        {
            StartIndex = FilePath.find_last_of('/',FilePath.length())+1;
           if(StartIndex != -1)
        {
            FileName = FilePath.substr(StartIndex,FilePath.length()-StartIndex);
        }
        }
}


TextCtrl::~TextCtrl()
{

}

/***********************************************************/

void TextCtrl::Save()
{

    //Set File Name
    int StartIndex = FilePath.find_last_of(_("\\"),FilePath.length())+1;

        if(StartIndex != -1)
        {
            FileName = FilePath.substr(StartIndex,FilePath.length()-StartIndex);
        }
        else
        {
            StartIndex = FilePath.find_last_of('/',FilePath.length())+1;
           if(StartIndex != -1)
        {
            FileName = FilePath.substr(StartIndex,FilePath.length()-StartIndex);
        }
        }
        IsNewFile = false;

    SaveFile(FilePath);

   ParseLexar();
}

void TextCtrl::OnMarginClick (wxStyledTextEvent &event)
{
    if (event.GetMargin() == 2)
        {
        int lineClick = LineFromPosition (event.GetPosition());
        int levelClick = GetFoldLevel (lineClick);
        if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0)
        {
            ToggleFold (lineClick);
        }
    }
}

void TextCtrl::OnCharAdded (wxStyledTextEvent &event)
{
    event.Skip();
/*
    char chr = (char)event.GetKey();
    int currentLine = GetCurrentLine();

    // Change this if support for mac files with \r is needed
    if (chr == '\n')
        {
        int lineInd = 0;
        if (currentLine > 0)
        {
            lineInd = GetLineIndentation(currentLine - 1);
        }
        if (lineInd == 0) return;
        SetLineIndentation (currentLine, lineInd);
        GotoPos(PositionFromLine (currentLine) + lineInd);
    }*/
}

void TextCtrl::OnKey (wxStyledTextEvent &WXUNUSED(event))
{
    wxMessageBox(wxT("OnKey"));
}

void TextCtrl::ParseLexar()
{
     SetLexarBase(this);
    //Set Lexar Based off of Input
    int StartIndex = FilePath.find_last_of('.',FilePath.length())+1;

        if(StartIndex != -1)
        {
            wxString ext = FilePath.substr(StartIndex,FilePath.length()-StartIndex);

            //C++
            if(ext == _("cpp") || ext == _("h"))
                SetLexarAsCPP(this);

            //HLSL for XAN Shader Language
            if(ext == _("fx"))
                SetLexarAsHLSL(this);

            //Python
            if(ext == _("py"))
                SetLexarAsPython(this);

            //HTML
            if(ext == _("html") || ext == _("htm"))
                SetLexarAsHTML(this);
        }
}
