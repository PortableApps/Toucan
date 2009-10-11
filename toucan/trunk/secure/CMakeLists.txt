if(MSVC)
    #Set up folders for visual studio
    source_group(source\\secure FILES secure.cpp securejob.cpp)
    source_group(headers\\secure FILES secure.h securejob.h)
endif(MSVC)

add_library(secure STATIC secure.cpp securejob.cpp)