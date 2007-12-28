#ifndef H_SECUREFUNCTIONS
#define H_SECUREFUNCTIONS

bool PreviewSecureLoop(wxString strFrom, Rules rules, wxTreeCtrl* to, wxTreeItemId idParent){
	wxDir dir(strFrom);
	wxString strFilename;
	bool blDir = dir.GetFirst(&strFilename);
	if(blDir){
		do {
			if(wxDirExists(strFrom + strFilename)){
				if(!rules.ShouldExclude(strFrom+strFilename, true)){
       					idParent = to->AppendItem(idParent, strFilename.AfterLast(wxFILE_SEP_PATH));
					PreviewSecureLoop(strFrom + strFilename, rules, to, idParent);
				}
			}
			else{
				if(!rules.ShouldExclude(strFrom+strFilename, false)){
					to->AppendItem(idParent, strFilename);
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
