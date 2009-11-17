/* -----------------------------------------------------------------------------
 * This is a set of typemaps for wxString <-> Lua String conversion
 * Based on the SWIG file std_string.i
 * ----------------------------------------------------------------------------- */

%{
	#include <wx/string.h>
%}

%naturalvar wxString;

%typemap(in,checkfn="lua_isstring") wxString
%{$1 = wxString(lua_tostring(L,$input), wxConvUTF8);%}
%typemap(out) wxString
%{ lua_pushlstring(L,$1.data(),$1.size()); SWIG_arg++;%}

%typemap(in,checkfn="lua_isstring")	const wxString& (wxString temp)
%{temp = wxString(lua_tostring(L,$input), wxConvUTF8); $1=&temp;%}
%typemap(out) const wxString&
%{ lua_pushlstring(L,$1->data(),$1->size()); SWIG_arg++;%}

%typemap(throws) wxString,wxString&,const wxString&
%{ lua_pushlstring(L,$1.data(),$1.size()); SWIG_fail;%}

// and the typechecks
%typecheck(SWIG_TYPECHECK_STRING) wxString,const wxString& {
  $1 = lua_isstring(L,$input);
}
