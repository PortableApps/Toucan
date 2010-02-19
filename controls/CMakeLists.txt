set(source dirctrl.cpp dirctrlspec.cpp extendeddirctrl.cpp loglistctrl.cpp vdtc.cpp)
set(headers dirctrl.h dirctrlspec.h extendeddirctrl.h loglistctrl.h vdtc.h)

add_library(controls STATIC ${source} ${headers})

