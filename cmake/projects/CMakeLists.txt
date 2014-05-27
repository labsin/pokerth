add_subdirectory(db)
add_subdirectory(protocol)
add_subdirectory(lib)

if(QML_APP)
    add_subdirectory(qml)
else(QML_APP)
    add_subdirectory(game)
endif(QML_APP)

if(BUILD_SERVER)
    add_subdirectory(server)
    add_subdirectory(chatcleaner)
endif(BUILD_SERVER)
