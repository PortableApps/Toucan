/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BASICFUNCTIONS
#define H_BASICFUNCTIONS

class wxComboBox;
class wxString;
class wxArrayString;

void SetupLanguageMap();

//Turns an array string into a string with the strings seperated by seperator, used when writing to ini files
wxString ArrayStringToString(const wxArrayString &strings, const wxString &seperator);
//Used for turning a string into an arraystring when the string are seperated by seperator, used when read from ini files
wxArrayString StringToArrayString(const wxString &string, const wxString &seperator);

//Priority codes for progress output, higher codes have higher priority and so 
//go to the bottom of the list
enum OutputType{
    FinishingLine, //(includes time)
    FinishingInfo,
    Message, //(increments the progress bar)
    StartingInfo,
    StartingLine, //(includes time)
    Error //(includes time and red colour and increments the progress bar)
};

namespace Jobs{
    enum Type{
        Sync,
        Backup,
        Secure,
        All
    };
};

void OutputProgress(const wxString &message, OutputType type);

/*Gets a files size in petabytes, should do for now but it may need changing in a few years ;)
Expects to be passed the result of a wxFilename.GetHumanReadableSize()*/
double GetInPB(const wxString &value);

//Get a list of jobs of a specific type, or if an emptystring is passed then get
//all job names that are in use
wxArrayString GetJobs(Jobs::Type type);
wxArrayString GetVariables(bool builtin);
wxArrayString GetRules();
wxArrayString GetScripts();

//On each run of Toucan to ensure the jobs are up to date
bool UpdateJobs();
bool UpdateRules();
bool UpdateScripts();
bool UpdateSettings();

const wxString& ToLang(const wxString &en);
const wxString& ToEn(const wxString &lang);

//Used by for_each in test.h
void makedir(const wxString &path);
void createfiles(const wxString &path);
void deletedir(const wxString &path);
void deletefiles(const wxString &path);
	
#endif
