#ifndef H_BASICFUNCTIONS
#define H_BASICFUNCTIONS

wxString ArrayStringToString(wxArrayString arrStrings, wxString strSeperator){
	wxString strTemp;
	for(int i = 0; i < arrStrings.GetCount(); i++){
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
		arrMain->Add(strToken);

	}
	return arrMain;
}

void MessageBox(wxString strMessage, wxString strCaption){
	wxMessageBox(strMessage, strCaption, wxICON_INFORMATION)
}
void ErrorBox(wxString strMessage, wxString strCaption){
	wxMessageBoc(strMessage, _("Error"), wxICON_ERROR)
}

bool AddDirToTree
wxDir dir(strFrom);
wxString strFilename;
bool blDir = dir.GetFirst(&strFilename);
if(blDir){
	do {
		while (dir.GetNext(&strFilename));
	}  
	return true;	
}

#endif
