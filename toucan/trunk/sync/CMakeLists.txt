if(MSVC)
    #Set up folders for visual studio
    source_group(source\\sync FILES syncbase.cpp syncfiles.cpp syncjob.cpp syncpreview.cpp syncthread.cpp)
    source_group(headers\\sync FILES syncbase.h syncfiles.h syncjob.h syncpreview.h syncthread.h)
endif(MSVC)

add_library(sync STATIC syncbase.cpp syncfiles.cpp syncjob.cpp syncpreview.cpp syncthread.cpp)