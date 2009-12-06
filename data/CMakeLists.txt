set(source backupdata.cpp jobdata.cpp securedata.cpp syncdata.cpp)
set(headers backupdata.h jobdata.h securedata.h syncdata.h)

add_library(data STATIC ${source} ${headers})

