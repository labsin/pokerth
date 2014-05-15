project(pokerth)
cmake_minimum_required(VERSION 2.8.10)
set(APPLICATION_NAME pokerth)
set(APPLICATION_NAME_LOWER pokerth)
set(APPLICAION_URL "https://github.com/labsin/pokerth")
set(APPLICAION_BUGREPORT "https://github.com/labsin/pokerth/issues")
set(APPLICAION_TARNAME pokerth)
set(APPLICATION_VERSION_MAJOR 0)
set(APPLICATION_VERSION_MINOR 1)
set(APPLICATION_VERSION_PATCH 0)
set(APPLICATION_VERSION ${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH})
set(APPLICATION_STRING ${APPLICATION_TARNAME}-${APPLICATION_VERSION})
set(APPLICAION_EMAIL "Sam Segers <sam.sgrs@gmail.com>")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

execute_process(COMMAND dpkg-architecture -qDEB_HOST_MULTIARCH
    OUTPUT_VARIABLE BUILD_ARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# add definitions
include(DefineCMakeDefaults)
include(DefinePlatformDefaults)
include(DefineCompilerFlags)
include(DefineInstallationPaths)
include(DefineOptions.cmake)

include(CLICKInstallDirs)
include(CLICKDeployLibs)
include(QtCreatorResources)
include(ResolveLibs)

# disallow in-source build
include(MacroEnsureOutOfSourceBuild)
macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build. Please create a separate build directory and run 'cmake /path/to/${PROJECT_NAME} [options]' there.")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# for dh_translations to extract the domain
# (regarding syntax consistency, see http://pad.lv/1181187)
# set (GETTEXT_PACKAGE "tilem")

set(RUN_DIR ${CMAKE_BINARY_DIR}/run)
set(RUN_BINARY_DIR ${RIN_DIR}/lib/${BUILD_ARCH})

set(OTHER_FILES 
    android/src/org/kde/necessitas/ministro/IMinistro.aidl
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java
    android/res/values-pl/strings.xml
    android/res/values-es/strings.xml
    android/res/values-it/strings.xml
    android/res/values-ru/strings.xml
    android/res/values-et/strings.xml
    android/res/values-rs/strings.xml
    android/res/values-ja/strings.xml
    android/res/values-fa/strings.xml
    android/res/values-ro/strings.xml
    android/res/values-zh-rTW/strings.xml
    android/res/values-nl/strings.xml
    android/res/values-zh-rCN/strings.xml
    android/res/values-pt-rBR/strings.xml
    android/res/values-el/strings.xml
    android/res/layout/splash.xml
    android/res/values-fr/strings.xml
    android/res/values-id/strings.xml
    android/res/values-ms/strings.xml
    android/res/values-de/strings.xml
    android/res/values-nb/strings.xml
    android/res/values/strings.xml
    android/res/values/libs.xml
    android/version.xml
    android/AndroidManifest.xml)

#qtcreator_add_project_resources(${OTHER_FILES})

add_subdirectory(data)
include(pokerth_db.cmake)
include(pokerth_protocol.cmake)
include(pokerth_lib.cmake)
include(pokerth_qml.cmake)

add_custom_target(${RUNDIR_TARGET} ALL
    DEPENDS pokerth)
foreach(_target )
add_custom_command(
    TARGET ${RUNDIR_TARGET}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${${_target}_BINARY_DIR}/out"
        "${RUN_DIR}/"
    COMMAND ${CMAKE_COMMAND} -E echo "COPY RUN: ${_target}"
    COMMAND ${CMAKE_COMMAND} -E echo "FROM: ${${_target}_BINARY_DIR}/out"
        "TO: ${RUN_DIR}/" )
endforeach()

add_custom_target(run
    COMMAND qmlscene -I lib/${BUILD_ARCH} TilEm.qml
    WORKING_DIRECTORY run/)
