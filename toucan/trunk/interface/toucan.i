%module toucan
%include "wxstring.i"

%{
#include "../basicfunctions.h"
%}

void OutputProgress(wxString message);