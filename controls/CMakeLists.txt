set(source dirctrl.cpp localdirctrl.cpp loglistctrl.cpp previewctrl.cpp rulesgrid.cpp syncctrl.cpp)
set(headers dirctrl.h localdirctrl.h loglistctrl.h previewctrl.h rulesgrid.h syncctrl.h)

add_library(controls STATIC ${source} ${headers})
