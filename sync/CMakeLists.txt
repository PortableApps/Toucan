set(source syncbase.cpp syncjob.cpp syncpreview.cpp)
set(headers syncbase.h syncjob.h syncpreview.h)

add_library(sync STATIC ${source} ${headers})