set(source backupjob.cpp backupprocess.cpp)
set(headers backupjob.h backupprocess.h)

add_library(backup STATIC  ${source} ${headers})

