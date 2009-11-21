/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BASICFUNCTIONS
#define H_BASICFUNCTIONS

class wxComboBox;
class wxString;
class wxArrayString;

//Turns an array string into a string with the strings seperated by strSeperator, used when writing to ini files
wxString ArrayStringToString(wxArrayString arrStrings, wxString strSeperator);
//Used for turning a string into an arraystring when the string are seperated by strSeperator, used when read from ini files
wxArrayString StringToArrayString(wxString strMain, wxString strSeperator);

//Basic function to write output to progress form, used to keep code clean, with optional date and error
void OutputProgress(const wxString &message, bool time = false, bool error = false);

//Functions to set up the progress bar
void SetGaugeRange(int range);
void SetGaugeValue(int value);
void IncrementGauge();
void EnableGauge(bool enable);

/*Gets a files size in petabytes, should do for now but it may need changing in a few years ;)
Expects to be passed the result of a wxFilename.GetHumanReadableSize()*/
double GetInPB(wxString strValue);

//Get a list of jobs of a specific type, or if an emptystring is passed then get
//all job names that are in use
wxArrayString GetJobs(const wxString &type);
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
	
#endif
