set(source backupjob.cpp backupprocess.cpp backupthread.cpp)
set(headers backupjob.h backupprocess.h backupthread.h)

add_library(backup STATIC  ${source} ${headers})