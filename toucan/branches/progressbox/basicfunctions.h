/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BASICFUNCTIONS
#define H_BASICFUNCTIONS

#include <wx/combobox.h>

//Turns an array string into a string with the strings seperated by strSeperator, used when writing to ini files
wxString ArrayStringToString(wxArrayString arrStrings, wxString strSeperator);
//Used for turning a string into an arraystring when the string are seperated by strSeperator, used when read from ini files
wxArrayString StringToArrayString(wxString strMain, wxString strSeperator);

//Basic messagebox
void MessageBox(wxString strMessage, wxString strCaption);
//Messagebox with error icon
void ErrorBox(wxString strMessage);

//Basic function to write output to progress form, used to keep code clean, with optional date
void OutputProgress(wxString strValue);
void OutputProgress(wxString time, wxString message);
void OutputBlank();

/*Gets a files size in petabytes, should do for now but it may need changing in a few years ;)
Expects to be passed the result of a wxFilename.GetHumanReadableSize()*/
double GetInPB(wxString strValue);

//The various sets of ComboBoxes on the form
bool SetRulesBox(wxComboBox *box);
bool SetJobsBox(wxComboBox *box, wxString strType);
bool SetVariablesBox(wxComboBox *box);
bool SetScriptsBox(wxComboBox *box);

//Language support functions
wxArrayString GetLanguages();
wxArrayString GetTranslatorNames();

//For use either from the command line or the GUI
wxString InputPassword();

//For the baclup compression level slider
void SetSliderText();

//On each run of Toucan to ensure the jobs are up to date
bool UpdateJobs(int version);
bool UpdateRules(int version);
bool UpdateScripts(int version);
	
#endif
