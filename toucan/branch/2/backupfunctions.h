#ifndef H_BACKUPFUNCTIONS
#define H_BACKUPFUNCTIONS

bool CreateList(wxTextFile *file, Rules rules, wxString strPath){
	//Clean up the path passed
	if (strPath[strPath.length()-1] != wxFILE_SEP_PATH) {
		strPath += wxFILE_SEP_PATH;       
	}
	wxDir dir(strPath);
	wxString strFilename;
	bool blDir = dir.GetFirst(&strFilename);
	//If the path is ok
	if(blDir){
		//Loop through all of the files and folders in the directory
		do {
			//If it is a directory
			if(wxDirExists(strPath + strFilename))
			{
				//It the folder is not excluded then add it to the file
				if(!rules.ShouldExclude(strPath + strFilename, true)){
					file->AddLine(strPath + strFilename);
				}
				//Always call the function again to ensure that ALL files and folders are processed
				CreateList(file, rules, strPath + strFilename);
			}
			//If it is a file
			else{
				if(!rules.ShouldExclude(strPath + strFilename, true)){
					file->AddLine(strPath + strFilename);
				}
			}
		}
		while (dir.GetNext(&strFilename) );
	}  
	return true;
}

#endif
