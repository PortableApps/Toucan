set(source frmmain.cpp frmpassword.cpp frmprogress.cpp frmrule.cpp frmvariable.cpp)
set(headers frmmain.h frmpassword.h frmprogress.h frmrule.h frmvariable.h)

add_library(forms STATIC ${source} ${headers})