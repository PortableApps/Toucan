set(source ccryptlib.c platform.c rijndael.c tables.c)
set(headers ccryptlib.h platform.h rijndael.h tables.h)

add_library(ccrypt STATIC ${source} ${headers})