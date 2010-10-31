set(source dirctrl.cpp localdirctrl.cpp loggridctrl.cpp previewctrl.cpp syncctrl.cpp)
set(headers dirctrl.h localdirctrl.h loggridctrl.h previewctrl.h syncctrl.h)

add_library(controls STATIC ${source} ${headers})
