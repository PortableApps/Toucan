set(source secure.cpp securejob.cpp)
set(headers secure.h securejob.h)

add_library(secure STATIC ${source} ${headers})