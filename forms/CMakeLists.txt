set(source frmmain.cpp frmpassword.cpp frmprogress.cpp frmvariable.cpp)
set(headers frmmain.h frmpassword.h frmprogress.h frmvariable.h)

add_library(forms STATIC ${source} ${headers})

