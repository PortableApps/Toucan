if(MSVC)
    #Set up folders for visual studio
    source_group(source\\data FILES backupdata.cpp jobdata.cpp securedata.cpp syncdata.cpp)
    source_group(headers\\data FILES backupdata.h jobdata.h securedata.h syncdata.h)
endif(MSVC)

add_library(data STATIC backupdata.cpp jobdata.cpp securedata.cpp syncdata.cpp)