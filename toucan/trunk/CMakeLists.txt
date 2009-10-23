cmake_minimum_required(VERSION 2.6)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

project(Toucan)

#Find wxWidgets
find_package(wxWidgets COMPONENTS base core net adv html aui REQUIRED)
include_directories(${wxWidgets_INCLUDE_DIRS})
add_definitions(${wxWidgets_DEFINITIONS})

#Define unicode if we need to
if(wxWidgets_CONFIGURATION MATCHES "u")
    add_definitions(-DUNICODE)
endif(wxWidgets_CONFIGURATION MATCHES "u")

#Find Lua
find_package(Lua51 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

if(MSVC)
    #Set the warning level to maximum
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endif(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")

    #We statically link to reduce dependancies
    foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif(${flag_var} MATCHES "/MD")
        if(${flag_var} MATCHES "/MDd")
            string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
        endif(${flag_var} MATCHES "/MDd")
    endforeach(flag_var)
endif(MSVC)

#Traverse through the sub-directories
add_subdirectory(backup)
add_subdirectory(controls)
add_subdirectory(data)
add_subdirectory(forms)
add_subdirectory(secure)
add_subdirectory(sync)

#Link against them
link_directories(backup)
link_directories(controls)
link_directories(data)
link_directories(forms)
link_directories(secure)
link_directories(sync)

#Add the source and header files
set(source ${source} basicfunctions.cpp cmdline.cpp dragndrop.cpp)
set(source ${source} filecounter.cpp job.cpp rules.cpp script.cpp)
set(source ${source} settings.cpp toucan.cpp variables.cpp)

set(headers ${headers} basicfunctions.h cmdline.h dragndrop.h)
set(headers ${headers} filecounter.h job.h rules.h script.h)
set(headers ${headers} settings.h toucan.h variables.h)

if(WIN32)
    #Add the resource file for the icon
    set(source ${source} toucan.rc)
endif(WIN32)

#Options for our unit tests
option(Enable_Tests "Enable building Toucans unit tests?" true)

if(Enable_Tests)
    find_package(CxxTest)
    include_directories(${CXXTEST_INCLUDE_DIR})

    enable_testing()

    add_custom_command(
        OUTPUT  test.cpp
        DEPENDS test.h
        COMMAND ${CXXTEST_PYTHON_TESTGEN_EXECUTABLE} --error-printer -o ${CMAKE_CURRENT_BINARY_DIR}/test.cpp ${CMAKE_CURRENT_SOURCE_DIR}/test.h
    )

    set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/test.cpp PROPERTIES GENERATED true)
    add_executable(test test.cpp test.h rules.cpp variables.cpp)
    add_test(test test)
    target_link_libraries(test ${wxWidgets_LIBRARIES})
endif(Enable_Tests)

#Set up the exe
add_executable(toucan ${source} ${headers})
target_link_libraries(toucan backup controls data forms secure sync ${wxWidgets_LIBRARIES} ${LUA_LIBRARIES})