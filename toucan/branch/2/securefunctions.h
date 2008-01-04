#ifndef H_SECUREFUNCTIONS
#define H_SECUREFUNCTIONS

bool PreviewSecureLoop(wxString strFrom, Rules rules, wxTreeCtrl* to, wxTreeItemId idParent){
	if (strFrom[strFrom.length()-1] != wxFILE_SEP_PATH) {
		strFrom += wxFILE_SEP_PATH;       
	}
	wxDir dir(strFrom);
	wxString strFilename;
	bool blDir = dir.GetFirst(&strFilename);
	if(blDir){
		do {
			if(wxDirExists(strFrom + strFilename)){
				wxTreeItemId tempidParent = to->AppendItem(idParent, strFilename.AfterLast(wxFILE_SEP_PATH));
				if(rules.ShouldExclude(strFrom+strFilename, true)){
					to->SetItemTextColour(tempidParent, wxColour(wxT("Red")));
				}
				PreviewSecureLoop(strFrom + strFilename, rules, to, tempidParent);
			}
			else{
				wxTreeItemId tempidParent = to->AppendItem(idParent, strFilename);
				if(rules.ShouldExclude(strFrom+strFilename, false)){
					to->SetItemTextColour(tempidParent, wxColour(wxT("Red")));
				}
			}	
		}
		while (dir.GetNext(&strFilename));
	}  
	return true;	
}

bool PreviewSecure(Rules rules, wxTreeCtrl* to){
	to->DeleteAllItems();
	for(unsigned int i = 0; i < wxGetApp().GetSecureLocations().GetCount(); i++){
		wxTreeItemId parent = to->AppendItem(to->GetRootItem(), wxGetApp().GetSecureLocations().Item(i).AfterLast(wxFILE_SEP_PATH));
		PreviewSecureLoop(wxGetApp().GetSecureLocations().Item(i), rules, to, parent);
	}
	return true;
}

#endif
