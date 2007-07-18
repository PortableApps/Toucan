#ifndef H_REMDIR
#define H_REMDIR

#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/filename.h>

bool RemDir(wxString rmDir) {

        // *************************************************************
        // COMMENT THIS BLOCK OUT IF YOU WANT TO BE ABLE TO DELETE ROOT
        // DIRECTORIES : IT IS RECOMMENDED YOU LEAVE IT AS IS
        // *************************************************************
        // BEGIN SAFETY CHECK
        // *************************************************************
        ///*
        // if it's a possible root directory
        if (rmDir.length() <= 3) {
                wxLogError(wxT("You asked RemDirRF() to delete a possible root directory.\nThis is disabled by default.  If you really need to delete") + rmDir + wxT(" please contact me at www.portableapps.com"));

                return false;
        }
        //*/
        // *************************************************************
        // END SAFETY CHECK
        // *************************************************************

    // first make sure that the dir exists
    if(!wxDir::Exists(rmDir)) {
            wxLogError(rmDir + wxT(" does not exist.  Could not remove directory."));
            return false;
    }
       
        // append a slash if we don't have one
        if (rmDir[rmDir.length()-1] != wxFILE_SEP_PATH) {
                rmDir += wxFILE_SEP_PATH;       
    }

        // define our directory object.  When we begin traversing it, the
        // os will not let go until the object goes out of scope.
    wxDir* dir = new wxDir(rmDir);

        // check for allocation failure
        if (dir == NULL) {
                wxLogError(wxT("Could not allocate new memory on the heap!"));
                return false;
        }

        // our file name temp var
    wxString filename;
        // get the first filename
    bool cont = dir->GetFirst(&filename);

        // if there are files to process
    if (cont){
        do {
                        // if the next filename is actually a directory
            if (wxDirExists(rmDir + filename)) {
                                // delete this directory
                RemDir(rmDir + filename);
            }
            else {
                                // otherwise attempt to delete this file
                if(!wxRemoveFile(rmDir + filename)) {
                                        // error if we couldn't
                                        wxLogError(wxT("Could not remove file") + rmDir + filename);
                                }
            }
        }
                // get the next file name
        while (dir->GetNext(&filename));
    }   

        // Remove our directory object, so the OS will let go of it and
        // allow us to delete it
        delete dir;

        // now actually try to delete it
        if (!wxFileName::Rmdir(rmDir)) {
                // error if we couldn't
                wxLogNull log;
                wxLogError(wxT("Could not remove directory ") + rmDir);
                // return accordingly
                return false;
        }
        // otherwise
        else {
                // return that we were successfull.
                return true;
        }

} 

#endif