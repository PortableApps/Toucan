if(MSVC)
    #Set up folders for visual studio
    source_group(source\\forms FILES frmmain.cpp frmpassword.cpp frmprogress.cpp frmrule.cpp frmvariable.cpp)
    source_group(headers\\forms FILES frmmain.h frmpassword.h frmprogress.h frmrule.h frmvariable.h)
endif(MSVC)

add_library(forms STATIC frmmain.cpp frmpassword.cpp frmprogress.cpp frmrule.cpp frmvariable.cpp)