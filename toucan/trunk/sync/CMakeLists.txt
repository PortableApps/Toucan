set(source syncbase.cpp syncfiles.cpp syncjob.cpp syncpreview.cpp syncthread.cpp)
set(headers syncbase.h syncfiles.h syncjob.h syncpreview.h syncthread.h)

add_library(sync STATIC ${source} ${headers})