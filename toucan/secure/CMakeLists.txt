set(source securejob.cpp secureprocess.cpp)
set(headers securejob.h secureprocess.h)

add_library(secure STATIC ${source} ${headers})