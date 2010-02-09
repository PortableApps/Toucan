set(source dirctrl.cpp dirtraverser.cpp extendeddirctrl.cpp loglistctrl.cpp vdtc.cpp)
set(headers dirctrl.h dirtraverser.h extendeddirctrl.h loglistctrl.h vdtc.h)

add_library(controls STATIC ${source} ${headers})

