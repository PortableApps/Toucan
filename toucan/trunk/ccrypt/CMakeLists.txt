set(source ccryptlib.c platform.c rijndael.c tables.c)
set(headers ccryptlib.h platform.c rijndael.h tables.h)

add_library(ccrypt STATIC ${source} ${headers})