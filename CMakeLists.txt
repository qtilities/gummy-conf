cmake_minimum_required(VERSION 3.15)
project(GummyConf
    VERSION 0.1.0
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#===============================================================================
# Qt
#===============================================================================
option(PROJECT_TRANSLATIONS_UPDATE "Update source translations [default: OFF]" OFF)
set(PROJECT_TRANSLATION_TEST_ENABLED 0 CACHE STRING "Whether to enable translation testing [default: 0]")
set(PROJECT_TRANSLATION_TEST "it" CACHE STRING "Country code of language to test in IDE [default: it]")
set(PROJECT_QT_VERSION 6 CACHE STRING "Qt version to use [default: 6]")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(QT NAMES Qt${PROJECT_QT_VERSION})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED LinguistTools Widgets)
find_package(Qtilitools REQUIRED)
#===============================================================================
# Project files
#===============================================================================
set(PROJECT_SOURCES
    src/application.hpp
    src/application.cpp
    src/dialogabout.hpp
    src/dialogabout.cpp
    src/gummyd.hpp
    src/gummyd.cpp
    src/maindialog.hpp
    src/maindialog.cpp
    src/qtilities.hpp
    src/settings.hpp
    src/settings.cpp
    src/systemtrayicon.hpp
    src/systemtrayicon.cpp
)
set(PROJECT_UI_FILES
    src/dialogabout.ui
    src/maindialog.ui
)
set(PROJECT_OTHER_FILES
    .github/workflows/build.yml
    .clang-format
    .editorconfig
    .gitattributes
    .gitignore
    README.md
)
source_group("" FILES ${PROJECT_SOURCES})
source_group("" FILES ${PROJECT_OTHER_FILES})
#===============================================================================
# Resources
#===============================================================================
include(Config.cmake)
include(QtAppResources)
#===============================================================================
# Application executable
#===============================================================================
set(PROJECT_ALL_FILES
    ${PROJECT_DESKTOP_FILES}
    ${PROJECT_RESOURCES}
    ${PROJECT_SOURCES}
    ${PROJECT_OTHER_FILES}
    ${PROJECT_QM_FILES}
    ${PROJECT_TRANSLATION_SOURCES}
    ${PROJECT_UI_FILES}
)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_ALL_FILES})
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED ${PROJECT_ALL_FILES})
    else()
        add_executable(${PROJECT_NAME} ${PROJECT_ALL_FILES})
    endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Widgets
)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_ID}")

target_compile_definitions(${PROJECT_NAME} PRIVATE
    APPLICATION_NAME="${PROJECT_NAME}"
    ORGANIZATION_NAME="${PROJECT_ORGANIZATION_NAME}"
    ORGANIZATION_DOMAIN="${PROJECT_ORGANIZATION_URL}"
    PROJECT_ID="${PROJECT_ID}"
    PROJECT_DATA_DIR="${CMAKE_INSTALL_FULL_DATADIR}/${PROJECT_ID}"
    PROJECT_APPSTREAM_ID="${PROJECT_APPSTREAM_ID}"
    PROJECT_ICON_FORMAT="${PROJECT_ICON_FORMAT}"
    PROJECT_ICON_SYSTEM_PATH="${PROJECT_ICON_FILE_PATH}/${PROJECT_APPSTREAM_ID}.${PROJECT_ICON_FORMAT}"
    PROJECT_TRANSLATION_TEST="${PROJECT_TRANSLATION_TEST}"
    PROJECT_TRANSLATION_TEST_ENABLED=${PROJECT_TRANSLATION_TEST_ENABLED}
)
#===============================================================================
# Install application
#===============================================================================
if (UNIX AND NOT APPLE)
    install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER ${PROJECT_ORGANIZATION_URL}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
    )
    install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()
#===============================================================================
# Project information
#===============================================================================
message(STATUS "
Project name:                     ${PROJECT_NAME}
AppStream ID:                     ${PROJECT_APPSTREAM_ID}
Version:                          ${PROJECT_VERSION}
Qt version:                       ${QT_VERSION}
Build type:                       ${CMAKE_BUILD_TYPE}
Install prefix:                   ${CMAKE_INSTALL_PREFIX}
Update translations before build: ${PROJECT_TRANSLATIONS_UPDATE}

CXX Debug flags:                  ${CMAKE_CXX_FLAGS_DEBUG}
CXX Release flags:                ${CMAKE_CXX_FLAGS_RELEASE}
CXX MinSize flags:                ${CMAKE_CXX_FLAGS_MINSIZEREL}
CXX RelWithDebInfo flags:         ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}
")

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
