/* -----------------------------------------------------------------------------
 * This is a set of typemaps for wxString <-> Lua String conversion
 * Based on the SWIG file std_string.i
 * ----------------------------------------------------------------------------- */

%{
	#include <wx/string.h>
	#include <wx/arrstr.h>
%}

//This allows us to get a filed from a table, returns a bool
%{
	bool getfield(lua_State *L, int index, const char *key, bool bldefault){
		bool ret = bldefault;
		lua_getfield(L, index, key);
		if(!lua_isboolean(L, -1)){
			lua_pop(L, 1);
			return ret;
		}
		ret = static_cast<bool>(lua_toboolean(L, -1));
		lua_pop(L, 1);
		return ret;
	}
%}

%naturalvar wxString;
%naturalvar wxArrayString;

%typemap(in,checkfn="lua_isstring") wxString
%{$1 = wxString(lua_tostring(L,$input), wxConvUTF8);%}

%typemap(in,checkfn="lua_isstring")	const wxString& (wxString temp)
%{temp = wxString(lua_tostring(L,$input), wxConvUTF8); $1=&temp;%}

%typemap(out) wxString
%{ lua_pushlstring(L,$1.data(),$1.size()); SWIG_arg++;%}

%typemap(out) const wxString&
%{ lua_pushlstring(L,$1->data(),$1->size()); SWIG_arg++;%}

%typemap(in,checkfn="lua_istable") const wxArrayString& (wxArrayString array, int i = 0)
%{	
	while(1)
	{
		lua_rawgeti(L,$input,i+1);
		if (lua_isnil(L,-1))
		{
			lua_pop(L,1);
			break;
		}
		//If we find something that is not a string then fail
		if (!lua_isstring(L,-1))
		{
			lua_pop(L,1);
			return 0;
		}
		array.Add(wxString(lua_tostring(L,-1), wxConvUTF8));
		lua_pop(L,1);
		++i;
	}
	$1 = &array;
%}

%typemap(in,checkfn="lua_istable") SyncChecks()
%{	
	//By default we check size and short byte
	$1.Size = getfield(L, $input,"size", $1.Size);
	$1.Time = getfield(L, $input, "time", $1.Time);
	$1.Short = getfield(L, $input,"short", $1.Short);
	$1.Full = getfield(L, $input,"full", $1.Full);
%}

%typemap(in,checkfn="lua_istable") SyncOptions()
%{
	$1.TimeStamps = getfield(L, $input,"timestamps", $1.TimeStamps);
	$1.Attributes = getfield(L, $input,"attributes", $1.Attributes);
	$1.IgnoreRO = getfield(L, $input,"ignorero", $1.IgnoreRO);
	$1.IgnoreDLS = getfield(L, $input,"ignoredls", $1.IgnoreDLS);
	$1.Recycle = getfield(L, $input,"recycle", $1.Recycle);
	$1.PreviewChanges = getfield(L, $input,"previewchanges", $1.PreviewChanges);
%}

%typemap(in,checkfn="lua_istable") BackupOptions()
%{
	$1.Password = getfield(L, $input,"password", $1.Password);
	$1.Test = getfield(L, $input,"test", $1.Test);
	$1.Solid = getfield(L, $input, "solid", $1.Solid);
%}

// and the typechecks
%typecheck(SWIG_TYPECHECK_STRING) wxString,const wxString& {
  $1 = lua_isstring(L,$input);
}

%typecheck(SWIG_TYPECHECK_STRING_ARRAY) const wxArrayString&, SyncChecks, SyncOptions, BackupOptions {
  $1 = lua_istable(L,$input);
}
