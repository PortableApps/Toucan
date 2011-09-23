set(source dirctrl.cpp localdirctrl.cpp previewctrl.cpp progresslistctrl.cpp rulesgrid.cpp syncctrl.cpp)
set(headers dirctrl.h localdirctrl.h previewctrl.h progresslistctrl.h rulesgrid.h syncctrl.h)

add_library(controls STATIC ${source} ${headers})
