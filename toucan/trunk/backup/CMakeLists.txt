if(MSVC)
    #Set up folders for visual studio
    source_group(source\\backup FILES backupjob.cpp backupprocess.cpp backupthread.cpp)
    source_group(headers\\backup FILES backupjob.h backupprocess.h backupthread.h)
endif(MSVC)

add_library(backup STATIC backupjob.cpp backupprocess.cpp backupthread.cpp)