/**
 * Main dialog
 */

#ifndef WX_PRECOMP
//(*InternalHeadersPCH(MainDlg)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#endif
//(*InternalHeaders(MainDlg)
#include <wx/bitmap.h>
#include <wx/image.h>
//*)

#include <wx/config.h>
#include <wx/msgdlg.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/config.h>

#include "wx_pch.h"

#include "Rule.h"

#include "MainDlg.h"
#include "AboutDlg.h"
#include "CustomRuleDlg.h"

///////////////////////////////////////////////////////////////////////

bool wxXmlDocumentEx::Save(const wxString& filename, int indentstep) const
{
    wxFileOutputStream stream(filename);
    if (!stream.Ok())
        return false;
    return Save(stream, indentstep);
}

bool wxXmlDocumentEx::Save(wxOutputStream& stream, int indentstep) const
{
    if ( !IsOk() )
        return false;

    wxString s;

    wxMBConv *convMem = NULL,
                        *convFile;

#if wxUSE_UNICODE
    convFile = new wxCSConv(GetFileEncoding());
    convMem = NULL;
#else
    if ( GetFileEncoding().CmpNoCase(GetEncoding()) != 0 )
    {
        convFile = new wxCSConv(GetFileEncoding());
        convMem = new wxCSConv(GetEncoding());
    }
    else // file and in-memory encodings are the same, no conversion needed
    {
        convFile =
            convMem = NULL;
    }
#endif

    s.Printf(wxT("<?xml version=\"%s\" encoding=\"%s\"?>\n"),
             GetVersion().c_str(), GetFileEncoding().c_str());
    OutputString(stream, s);

    OutputNode(stream, GetRoot(), 0, convMem, convFile, indentstep);
    OutputString(stream, wxT("\n"));

    delete convFile;
    delete convMem;

    return true;
}

///////////////////////////////////////////////////////////////////////

//(*IdInit(MainDlg)
const long MainDlg::ID_STATICBITMAP1 = wxNewId();
const long MainDlg::ID_STATICTEXT1 = wxNewId();
const long MainDlg::ID_BUTTON1 = wxNewId();
const long MainDlg::ID_BUTTON2 = wxNewId();
const long MainDlg::ID_BUTTON6 = wxNewId();
const long MainDlg::ID_BUTTON8 = wxNewId();
const long MainDlg::ID_BUTTON9 = wxNewId();
const long MainDlg::ID_CHECKLISTBOX1 = wxNewId();
const long MainDlg::ID_RADIOBOX1 = wxNewId();
const long MainDlg::ID_BUTTON7 = wxNewId();
const long MainDlg::ID_BUTTON4 = wxNewId();
const long MainDlg::ID_BUTTON5 = wxNewId();
const long MainDlg::ID_LISTCTRL1 = wxNewId();
const long MainDlg::ID_LISTCTRL2 = wxNewId();
const long MainDlg::ID_NOTEBOOK1 = wxNewId();
const long MainDlg::ID_BUTTON3 = wxNewId();
//*)

///////////////////////////////////////////////////////////////////////

enum NBaseRuleType
{
    NBaseRuleTypeTime = 0,			///< Categorize by file modification time
    NBaseRuleTypeNone,				///< None rule
};

const int N_COL_NUM = 3;	///< Result listctrl column number
/** Result listctrl column names */
const wxString CSZ_COL_NAMES[] =
{
    _("File"),
    _("Destination directory"),
    _("Progress"),
};
const int N_COL_WIDTH[] = { 340, 140, 80};

/** Folder size listctrl */
const int N_FOLDER_SIZE_LC_COL_NUM = 2;
const wxString CSZ_FOLDER_SIZE_LC_COL_NAMES[] =
{
    _("Folder"),
    _("Size(KB)"),
};
const int N_FOLDER_SIZE_LC_COL_WIDTH[] = { 480, 80};

const wxChar* CSZ_DESKTOP_KEY_NAME = _T("Desktop");   ///<
/** Desktop register path */
const wxString CSZ_DESKTOP_KEY_PATH =
    _T("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders");



const wxString CSZ_TODAY = _("___Today");
const wxString CSZ_YESTERDAY = _("___Yesterday");


const wxString CSZ_RULE_TAG_NAMES[] =
{
    _T("index"),
    _T("type"),
    _T("condition"),
    _T("dest"),
};

///////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MainDlg,wxDialog)
    //(*EventTable(MainDlg)
    //*)
END_EVENT_TABLE()

MainDlg::MainDlg(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size):
        m_pRoot(0)
{


    //(*Initialize(MainDlg)
    wxBoxSizer* BoxSizer4;
    wxBoxSizer* BoxSizer6;
    wxBoxSizer* BoxSizer5;
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer1;
    wxStaticBoxSizer* StaticBoxSizer1;
    wxBoxSizer* BoxSizer3;

    Create(parent, id, _("Desktop Assistant"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX, _T("id"));
    SetClientSize(wxDefaultSize);
    Move(wxDefaultPosition);
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    StaticBitmap1 = new wxStaticBitmap(this, ID_STATICBITMAP1, wxBitmap(wxImage(_T("img\\DesktopAssistant.ico"))), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICBITMAP1"));
    BoxSizer5->Add(StaticBitmap1, 0, wxTOP|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Desktop Assistant is a tool for Desktop file categorization.\nThis tool can categorize files to folders by using the rule defined beforehand. \nThe folders that stores the files is automatically created."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    BoxSizer5->Add(StaticText1, 1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer5, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Select customize categorization rules"));
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    m_btnNew = new wxButton(this, ID_BUTTON1, _("&New..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer3->Add(m_btnNew, 0, wxTOP|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_btnEdit = new wxButton(this, ID_BUTTON2, _("&Edit..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    m_btnEdit->Disable();
    BoxSizer3->Add(m_btnEdit, 0, wxTOP|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_btnDelete = new wxButton(this, ID_BUTTON6, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON6"));
    m_btnDelete->Disable();
    BoxSizer3->Add(m_btnDelete, 0, wxTOP|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_btnUp = new wxButton(this, ID_BUTTON8, _("&Up"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON8"));
    m_btnUp->Disable();
    m_btnUp->Hide();
    BoxSizer3->Add(m_btnUp, 1, wxTOP|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_btnDown = new wxButton(this, ID_BUTTON9, _("Dow&n"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON9"));
    m_btnDown->Disable();
    m_btnDown->Hide();
    BoxSizer3->Add(m_btnDown, 1, wxTOP|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer3->Add(-1,-1,1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer3->Add(-1,-1,1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer1->Add(BoxSizer3, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
    m_pLbxCustRules = new wxCheckListBox(this, ID_CHECKLISTBOX1, wxDefaultPosition, wxDefaultSize, 0, 0, wxLB_SINGLE|wxLB_NEEDED_SB, wxDefaultValidator, _T("ID_CHECKLISTBOX1"));
    StaticBoxSizer1->Add(m_pLbxCustRules, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(StaticBoxSizer1, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    wxString __wxRadioBoxChoices_1[2] =
    {
    _("By file modified time"),
    _("None(Do nothing)")
    };
    m_pRbxBaseRules = new wxRadioBox(this, ID_RADIOBOX1, _("Select base categorization rules"), wxDefaultPosition, wxDefaultSize, 2, __wxRadioBoxChoices_1, 1, wxRA_VERTICAL, wxDefaultValidator, _T("ID_RADIOBOX1"));
    BoxSizer1->Add(m_pRbxBaseRules, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    m_btnPreview = new wxButton(this, ID_BUTTON7, _("&Preview"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON7"));
    BoxSizer2->Add(m_btnPreview, 0, wxTOP|wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    m_btnRun = new wxButton(this, ID_BUTTON4, _("&Run"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    m_btnRun->Disable();
    BoxSizer2->Add(m_btnRun, 0, wxTOP|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_btnTest = new wxButton(this, ID_BUTTON5, _("test"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    m_btnTest->Hide();
    BoxSizer2->Add(m_btnTest, 1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer2, 0, wxTOP|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer6 = new wxBoxSizer(wxVERTICAL);
    Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxSize(600,256), wxNB_BOTTOM, _T("ID_NOTEBOOK1"));
    Notebook1->SetMaxSize(wxSize(320,200));
    m_pLcResult = new wxListCtrl(Notebook1, ID_LISTCTRL1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL1"));
    m_pLcFolderSize = new wxListCtrl(Notebook1, ID_LISTCTRL2, wxDefaultPosition, wxDefaultSize, wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL2"));
    Notebook1->AddPage(m_pLcResult, _("Preview and run result"), false);
    Notebook1->AddPage(m_pLcFolderSize, _("Folder size analysis"), false);
    BoxSizer6->Add(Notebook1, 1, wxTOP|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    BoxSizer1->Add(BoxSizer6, 1, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    m_btnCheckUpdate = new wxButton(this, ID_BUTTON3, _("C&heck for update..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    BoxSizer4->Add(m_btnCheckUpdate, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_btnAbout = new wxButton(this, wxID_ABOUT, _("&About..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_ABOUT"));
    BoxSizer4->Add(m_btnAbout, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_btnQuit = new wxButton(this, wxID_EXIT, _("&Quit"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_EXIT"));
    BoxSizer4->Add(m_btnQuit, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);
    Center();

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainDlg::OnBtnNewClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainDlg::OnBtnEditClick);
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainDlg::OnBtnDeleteClick);
    Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainDlg::OnBtnPreviewClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainDlg::OnBtnRunClick);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainDlg::OnBtnTestClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainDlg::OnBtnCheckUpdateClick);
    Connect(wxID_ABOUT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainDlg::OnBtnAboutClick);
    Connect(wxID_EXIT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainDlg::OnBtnQuitClick);
    Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&MainDlg::OnInit);
    //*)

    // BoxSizer1->Hide(BoxSizer5);
    Connect(ID_CHECKLISTBOX1, wxEVT_COMMAND_LISTBOX_SELECTED, (wxObjectEventFunction)&MainDlg::OnLbxRuleItemSelect);

}

MainDlg::~MainDlg()
{
    //(*Destroy(MainDlg)
    //*)
}

void MainDlg::OnBtnQuitClick(wxCommandEvent& event)
{
    Close();
}

void MainDlg::OnBtnAboutClick(wxCommandEvent& event)
{
    AboutDlg dlg(this);
    dlg.ShowModal();
}

void MainDlg::OnBtnCheckUpdateClick(wxCommandEvent& event)
{
    wxLaunchDefaultBrowser(_T("http://down.boomworks.net/"));
}

/**
 * Get desktop full path
 *
 */
void MainDlg::getDesktopPath(wxString& strPath)
{
    wxRegKey *pRegKey = new wxRegKey(CSZ_DESKTOP_KEY_PATH);

    //will create the Key if it does not exist
    if ( !pRegKey->Exists() )
    {
        return;
    }

    pRegKey->QueryValue(CSZ_DESKTOP_KEY_NAME, strPath);

    delete pRegKey;
}

void MainDlg::OnInit(wxInitDialogEvent& event)
{
    ///////////////////////////////////////////////////////////////////

    if (!m_docWords.Load(_T("rules.xml")))
        return;

    // start processing the XML file
    m_pRoot = m_docWords.GetRoot();
    if (m_pRoot->GetName() != _T("rules"))
        return;

    wxXmlNode* child = m_pRoot->GetChildren();
    wxXmlNode* pChildChild = 0;
    while (child)
    {
        if (child->GetName() != _T("rule"))
            break;

        pChildChild = child->GetChildren();
        wxString strNo, strType, strCondition, strDest;

        while (pChildChild)
        {
            if (pChildChild->GetName() == CSZ_RULE_TAG_NAMES[0])
            {
                strNo = pChildChild->GetNodeContent();
            }

            if (pChildChild->GetName() == CSZ_RULE_TAG_NAMES[1])
            {// process tag2
                strType = pChildChild->GetNodeContent();
            }

            if (pChildChild->GetName() == CSZ_RULE_TAG_NAMES[2])
            {// process tag2
                strCondition = pChildChild->GetNodeContent();
            }

            if (pChildChild->GetName() == CSZ_RULE_TAG_NAMES[3])
            {// process tag2
                strDest = pChildChild->GetNodeContent();
            }
            //
            pChildChild = pChildChild->GetNext();
        }

        if (strType == ExtNameRule::ms_strType)
        {
            ExtNameRule* pRule1 = new ExtNameRule();

            strNo.ToULong(&pRule1->m_nNo);
            pRule1->m_arrStrExtName.Add( strCondition );
            pRule1->m_strBaseDestPath = strDest;  //___ZIP

            m_categorizeMgr.AddRule(pRule1);
        }

        if (strType == NameIncludeRule::ms_strType)
        {
            NameIncludeRule* pRule = new NameIncludeRule();

            strNo.ToULong(&pRule->m_nNo);
            pRule->m_strInclude = strCondition;
            pRule->m_strBaseDestPath = strDest;  //___ZIP

            m_categorizeMgr.AddRule(pRule);
        }

        //
        child = child->GetNext();
    }

    ///////////////////////////////////////////////////////////////////
    // Init check list box
//    ArrayRule* pArrRule = m_categorizeMgr.GetRuleArray();
//
//    int nCnt = pArrRule->GetCount();
//    Rule* pRule = 0;
//    wxString strTemp;
//    int nIndex = -1;
//    for (int i=0; i<nCnt; i++)
//    {
//        pRule = pArrRule->Item(i);
//
//        pRule->GetDispStr(strTemp);
//        nIndex = m_pLbxCustRules->Append(strTemp);
//        // m_pLbxCustRules->SetClientData(nIndex, pRule);
//        m_pLbxCustRules->Check(nIndex);
//    }
	updateRuleLbx(false);

    //
    for (int i=0; i<N_COL_NUM; i++)
    {
        m_pLcResult->InsertColumn(i, CSZ_COL_NAMES[i], wxLIST_FORMAT_LEFT, N_COL_WIDTH[i]);
    }
    //
    for (int i=0; i<N_FOLDER_SIZE_LC_COL_NUM; i++)
    {
        m_pLcFolderSize->InsertColumn(i, CSZ_FOLDER_SIZE_LC_COL_NAMES[i], wxLIST_FORMAT_LEFT, N_FOLDER_SIZE_LC_COL_WIDTH[i]);
    }

    // Attach Observer object
    m_categorizeMgr.AttachObserver(this);

    wxString strDesktopPath;
    getDesktopPath(strDesktopPath);
    m_categorizeMgr.SetBaseDestPath(strDesktopPath);

    ///////////////////////////////////////////////////////////////////

}

void MainDlg::OnBtnRunClick(wxCommandEvent& event)
{
    //
    m_pLcResult->DeleteAllItems();
    // m_pLcFolderSize->DeleteAllItems();

    //
    m_categorizeMgr.Categorize();

}

/**
 * Preview
 */
void MainDlg::OnBtnPreviewClick(wxCommandEvent& event)
{
    // Clear list
    m_pLcResult->DeleteAllItems();
    //m_pLcFolderSize->DeleteAllItems();

    //
    wxString strDesktopPath;
    getDesktopPath(strDesktopPath);
    m_categorizeMgr.SetBaseDestPath(strDesktopPath);

    // Colect rules setting
    int nType = m_pRbxBaseRules->GetSelection();
    //
    switch (nType)
    {
    case NBaseRuleTypeTime:
    {
        BasicRule* pBasicRule = new BasicRule();

        pBasicRule->m_nNo = 9999;  ///< Max No.
        pBasicRule->m_strBaseDestPath = strDesktopPath;
        m_categorizeMgr.AddRule(pBasicRule);	// Last rule is basic rule

        break;
    }

    default:
    {// Do nothing

    }
    }

    // Preview and update UI
    m_categorizeMgr.Preview();
    updateUICtrls();
}

void MainDlg::updateUICtrls()
{
    //
    int nCnt = m_pLcResult->GetItemCount();

    if (nCnt > 0)
    {
        m_btnRun->Enable();
    }
    else
    {
        m_btnRun->Enable(false);
    }
}

void MainDlg::OnBtnTestClick(wxCommandEvent& event)
{
    // BoxSizer1->Show(BoxSizer5);

}

void MainDlg::UpdateCategorizationCtrls()
{
    //
    ArrayCategorizationFileInfo* pArrFileInfo = m_categorizeMgr.GetCategorizationFileInfos();

    int nCnt = pArrFileInfo->GetCount();
    int nIndex = 0;
    CategorizationFileInfo* pFileInfo = 0;
    wxString strTemp;
    for (int i=0; i<nCnt; i++)
    {
        pFileInfo = pArrFileInfo->Item(i);

        nIndex = m_pLcResult->InsertItem(m_pLcResult->GetItemCount(), pFileInfo->m_pFileName->GetFullPath());
        // m_pLcResult->SetItem(nIndex, 1, pFileInfo->m_strDestFolderName);
        m_pLcResult->SetItem(nIndex, 1, pFileInfo->GetDestFullPath());

        if (pFileInfo->m_bPreProcessed == true && pFileInfo->m_bProcessed == false )
        {
            strTemp = _T("Previewed");
        }
        else
        {
            strTemp = _T("Processed");
        }
        m_pLcResult->SetItem(nIndex, 2, strTemp);
    }
}
void MainDlg::OnLbxRuleItemSelect(wxCommandEvent& event)
{
    updateButtons();
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void MainDlg::OnBtnNewClick(wxCommandEvent& event)
{
    CustomRuleDlg dlg(this, CustomRuleDlg::RuleModeNew);

    if (dlg.ShowModal() == wxID_OK)
    {
        wxMessageBox(_T("The feature of Customization is still being developed.\nPlease wait for a while. "));

    }
}

void MainDlg::OnBtnEditClick(wxCommandEvent& event)
{
    // Get selection
    int nIndex = -1;
    nIndex = m_pLbxCustRules->GetSelection();

    if (-1 == nIndex)
        return;

    // Find pRule from CateMgr
	Rule* pRule = m_categorizeMgr.GetRule(nIndex);

    // Set pRule to dlg
	CustomRuleDlg dlg(this, CustomRuleDlg::RuleModeEdit);
	dlg.SetRule(pRule);

	if (dlg.ShowModal() == wxID_OK)
    {
        //wxMessageBox(_T("The feature of Customization is still being developed.\nPlease wait for a while. "));
		updateRuleLbx(true);
    }

}

void MainDlg::updateRuleLbx(bool bClear)
{
	if(bClear)
		m_pLbxCustRules->Clear();

	//
    // Init check list box
    ArrayRule* pArrRule = m_categorizeMgr.GetRuleArray();

    int nCnt = pArrRule->GetCount();
    Rule* pRule = 0;
    wxString strTemp;
    int nIndex = -1;
    for (int i=0; i<nCnt; i++)
    {
        pRule = pArrRule->Item(i);

        pRule->GetDispStr(strTemp);
        nIndex = m_pLbxCustRules->Append(strTemp);
        // m_pLbxCustRules->SetClientData(nIndex, pRule);
        m_pLbxCustRules->Check(nIndex);
    }


}

void MainDlg::OnBtnDeleteClick(wxCommandEvent& event)
{
    // Get selection
    int nIndex = -1;
    nIndex = m_pLbxCustRules->GetSelection();

    if (-1 == nIndex)
        return;

    // Delete item
    // m_categorizeMgr->DeleteRule(m_pLbxCustRules->GetString(nIndex));
    m_categorizeMgr.DeleteRule(nIndex);
    m_pLbxCustRules->Delete(nIndex);

    updateButtons();
}

void MainDlg::updateButtons()
{
    if ( wxNOT_FOUND != m_pLbxCustRules->GetSelection())
    {
        m_btnEdit->Enable();
        m_btnDelete->Enable();
    }
    else
    {
		m_btnEdit->Enable(false);
        m_btnDelete->Enable(false);
    }
}
