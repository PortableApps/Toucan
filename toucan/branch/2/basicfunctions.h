#ifndef H_BASICFUNCTIONS
#define H_BASICFUNCTIONS

#include <wx/tokenzr.h>
#include <wx/variant.h>
#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include "frmprogress.h"

wxString ArrayStringToString(wxArrayString arrStrings, wxString strSeperator){
	wxString strTemp;
	for(unsigned int i = 0; i < arrStrings.GetCount(); i++){
		strTemp = strTemp + strSeperator + arrStrings.Item(i);
	}
	return strTemp;
}

wxArrayString StringToArrayString(wxString strMain, wxString strSeperator){
	wxArrayString arrMain;
	wxStringTokenizer tkzMain(strMain, strSeperator, wxTOKEN_STRTOK);
	wxString strToken;
	while ( tkzMain.HasMoreTokens() ){  
		strToken = tkzMain.GetNextToken();   
		arrMain.Add(strToken);

	}
	return arrMain;
}

void MessageBox(wxString strMessage, wxString strCaption){
	wxMessageBox(strMessage, strCaption, wxICON_INFORMATION);
}
void ErrorBox(wxString strMessage){
	wxMessageBox(strMessage, _("Error"), wxICON_ERROR);
}

void OutputProgress(wxString strValue, frmProgress *window){
	//frmProgress* window = wxDynamicCast(wxWindow::FindWindow(ID_FRMPROGRESS), frmProgress);
	wxMutexGuiEnter();
	window->m_Text->AppendText(strValue + wxT("\n"));
	wxMutexGuiLeave();
}

double GetInPB(wxString strValue){
	wxString strSize = strValue.Right(2);
	wxVariant var = strValue.Left(strValue.Length() - 2);
	double dSize = var.GetDouble();
	if(strSize = wxT("kB")){
		dSize = dSize/1024;
	}
	if(strSize = wxT("MB")){
		dSize = dSize/1024;
	}
	if(strSize = wxT("GB")){
		dSize = dSize/1024;
	}
	//Conveting to PB, should be plenty big for a while
	dSize = dSize/1024;
	return dSize;
}

//Needs error checking code
bool SetRulesBox(wxComboBox *box){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Rules.ini"));
	bool blCont;
	wxString strValue;
	long dummy;
	blCont = config->GetFirstGroup(strValue, dummy);
	while (blCont){
		box->Append(strValue);
		blCont = config->GetNextGroup(strValue, dummy);
	}
	return true;
}

#endif
