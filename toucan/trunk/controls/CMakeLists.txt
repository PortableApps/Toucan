set(source dirctrl.cpp extendeddirctrl.cpp loglistctrl.cpp vdtc.cpp)
set(headers dirctrl.h extendeddirctrl.h loglistctrl.h vdtc.h)

add_library(controls STATIC ${source} ${headers})