set(source syncbase.cpp syncjob.cpp)
set(headers syncbase.h syncjob.h)

add_library(sync STATIC ${source} ${headers})

