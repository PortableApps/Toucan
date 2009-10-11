if(MSVC)
    #Set up folders for visual studio
    source_group(source\\controls FILES dirctrl.cpp extendeddirctrl.cpp loglistctrl.cpp vdtc.cpp)
    source_group(headers\\controls FILES dirctrl.h extendeddirctrl.h loglistctrl.h vdtc.h)
endif(MSVC)

add_library(controls STATIC dirctrl.cpp extendeddirctrl.cpp loglistctrl.cpp vdtc.cpp)