add_library(onsetJavaJNI SHARED
        PluginInterface.cpp
        Singleton.h
        Plugin.h
        Plugin.cpp
        )

find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)

target_include_directories(onsetJavaJNI PRIVATE
        ${HORIZONSDK_INCLUDE_DIR}
        ${JNI_INCLUDE_DIRS}
        )

set_property(TARGET onsetJavaJNI PROPERTY CXX_STANDARD 17)
set_property(TARGET onsetJavaJNI PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET onsetJavaJNI PROPERTY PREFIX "")

if(MSVC)
    target_compile_definitions(onsetJavaJNI PRIVATE
            _CRT_SECURE_NO_WARNINGS
            NOMINMAX
            )
endif()

# increase warning levels
if (MSVC)
    target_compile_options(onsetJavaJNI PRIVATE
            /W4
            /WX
            /permissive-
            /wd4505
            )
elseif(UNIX)
    target_compile_options(onsetJavaJNI PRIVATE
            -Wall
            -Wextra
            -pedantic
            -Werror
            -Wno-unused-function
            )
endif()

target_link_libraries(onsetJavaJNI ${HORIZONSDK_LIBRARY} ${JNI_LIBRARIES})


if(UNIX)
    if(NOT APPLE)
        target_link_libraries(onsetJavaJNI rt)
    endif()

    set_target_properties(onsetJavaJNI PROPERTIES
            INSTALL_RPATH "$ORIGIN/..:$ORIGIN/")
endif()

install(TARGETS onsetJavaJNI DESTINATION "plugins/")

set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
if(WIN32)
    set(CPACK_GENERATOR ZIP)
else()
    set(CPACK_GENERATOR TGZ)
endif()

include(CPack)