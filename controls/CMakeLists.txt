set(source dirctrl.cpp localdirctrl.cpp previewctrl.cpp rulesgrid.cpp syncctrl.cpp)
set(headers dirctrl.h localdirctrl.h previewctrl.h rulesgrid.h syncctrl.h)

add_library(controls STATIC ${source} ${headers})
