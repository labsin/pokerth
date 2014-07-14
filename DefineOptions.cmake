include(CMakeDependentOption)
option(QML_APP "Build with QML app" ON)
option(ANDROID "Build for Android (todo)" OFF)
option(UBUNTU_COMPONENTS "Use Ubuntu qml components" OFF)
option(GUI_800x480 "Build 800x480 gui version" OFF)
option(ONLY_CLIENT "Only build the client and not the server" ON)
if(NOT APPLE)
    CMAKE_DEPENDENT_OPTION(BUILD_SERVER "Build the server" ON
        "NOT ONLY_CLIENT;NOT GUI_800x480;NOT ANDROID" OFF)
elseif(NOT APPLE)
    CMAKE_DEPENDENT_OPTION(BUILD_SERVER "Build the server" OFF
        "NOT ONLY_CLIENT;NOT GUI_800x480" OFF)
endif(NOT APPLE)
