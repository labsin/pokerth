project(pokerth_qml)

find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)

find_package(GSasl REQUIRED)
find_package(SDL REQUIRED)
find_package(SDLMixer REQUIRED)
find_package(Sqlite3 REQUIRED)
find_package(TinyXML REQUIRED)
find_package(Protobuf REQUIRED)
find_package(GCrypt REQUIRED)

find_package(Boost REQUIRED COMPONENTS filesystem thread iostreams regex system random chrono)

add_definitions(-DENABLE_IPV6 -DTIXML_USE_STL -DBOOST_FILESYSTEM_DEPRECATED)
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/engine
    ${CMAKE_SOURCE_DIR}/src/gui
    ${CMAKE_SOURCE_DIR}/src/qt/qttools
    ${CMAKE_SOURCE_DIR}/src/net
    ${CMAKE_SOURCE_DIR}/src/engine/local_engine
    ${CMAKE_SOURCE_DIR}/src/engine/network_engine
    ${CMAKE_SOURCE_DIR}/src/config
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/gui/qml)

include_directories(
    ${GSASL_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}
    ${SDL_MIXER_INCLUDE_DIR}
    ${SQLITE3_INCLUDE_DIR}
    ${TINYXML_INCLUDE_DIR}
    ${GCRYPT_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    )

if(WIN32)
    add_definitions(-DCURL_STATICLIB -D_WIN32_WINNT=0x0501 -DHAVE_OPENSSL)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../boost/ ${CMAKE_CURRENT_SOURCE_DIR}/../GnuTLS/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../gsasl/include ${CMAKE_CURRENT_SOURCE_DIR}/../curl/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../zlib ${CMAKE_CURRENT_SOURCE_DIR}/../sqlite
        ${CMAKE_CURRENT_SOURCE_DIR}/../openssl/include)
endif(WIN32)

set(pokerth_qml_HDRS
    ${CMAKE_SOURCE_DIR}/src/config/configfile.h
    ${CMAKE_SOURCE_DIR}/src/core/loghelper.h
    ${CMAKE_SOURCE_DIR}/src/core/thread.h
    ${CMAKE_SOURCE_DIR}/src/engine/berointerface.h
    ${CMAKE_SOURCE_DIR}/src/engine/boardinterface.h
    ${CMAKE_SOURCE_DIR}/src/engine/enginefactory.h
    ${CMAKE_SOURCE_DIR}/src/engine/game.h
    ${CMAKE_SOURCE_DIR}/src/engine/handinterface.h
    ${CMAKE_SOURCE_DIR}/src/engine/local_engine/cardsvalue.h
    ${CMAKE_SOURCE_DIR}/src/engine/local_engine/localbero.h
    ${CMAKE_SOURCE_DIR}/src/engine/local_engine/localberoflop.h
    ${CMAKE_SOURCE_DIR}/src/engine/local_engine/localberopostriver.h
    ${CMAKE_SOURCE_DIR}/src/engine/local_engine/localberopreflop.h
    ${CMAKE_SOURCE_DIR}/src/engine/local_engine/localberoriver.h
    ${CMAKE_SOURCE_DIR}/src/engine/local_engine/localberoturn.h
    ${CMAKE_SOURCE_DIR}/src/engine/local_engine/localboard.h
    ${CMAKE_SOURCE_DIR}/src/engine/local_engine/localenginefactory.h
    ${CMAKE_SOURCE_DIR}/src/engine/local_engine/localhand.h
    ${CMAKE_SOURCE_DIR}/src/engine/local_engine/localplayer.h
    ${CMAKE_SOURCE_DIR}/src/engine/local_engine/replay.h
    ${CMAKE_SOURCE_DIR}/src/engine/local_engine/tools.h
    ${CMAKE_SOURCE_DIR}/src/engine/network_engine/clientbero.h
    ${CMAKE_SOURCE_DIR}/src/engine/network_engine/clientboard.h
    ${CMAKE_SOURCE_DIR}/src/engine/network_engine/clientenginefactory.h
    ${CMAKE_SOURCE_DIR}/src/engine/network_engine/clienthand.h
    ${CMAKE_SOURCE_DIR}/src/engine/network_engine/clientplayer.h
    ${CMAKE_SOURCE_DIR}/src/engine/playerinterface.h
    ${CMAKE_SOURCE_DIR}/src/gamedata.h
    ${CMAKE_SOURCE_DIR}/src/gui/generic/serverguiwrapper.h
    ${CMAKE_SOURCE_DIR}/src/gui/guiinterface.h
    ${CMAKE_SOURCE_DIR}/src/gui/qml/configwrapper.h
    ${CMAKE_SOURCE_DIR}/src/gui/qml/customqmlapplicationengine.h
    ${CMAKE_SOURCE_DIR}/src/gui/qml/gamedataclass.h
    ${CMAKE_SOURCE_DIR}/src/gui/qml/guiwrapper.h
    ${CMAKE_SOURCE_DIR}/src/gui/qml/imageprovider.h
    ${CMAKE_SOURCE_DIR}/src/gui/qml/manager.h
    ${CMAKE_SOURCE_DIR}/src/gui/qml/playermodel.h
    ${CMAKE_SOURCE_DIR}/src/gui/qml/qmlcard.h
    ${CMAKE_SOURCE_DIR}/src/gui/qml/qmlgame.h
    ${CMAKE_SOURCE_DIR}/src/gui/qml/qmlplayer.h
    ${CMAKE_SOURCE_DIR}/src/gui/qml/qmlserver.h
    ${CMAKE_SOURCE_DIR}/src/gui/qml/servermodel.h
    ${CMAKE_SOURCE_DIR}/src/gui/qml/storeplayers.h
    ${CMAKE_SOURCE_DIR}/src/gui/qml/styles/gametablestylereader.h
    ${CMAKE_SOURCE_DIR}/src/gui/qt/qttools/qthelper/qthelper.h
    ${CMAKE_SOURCE_DIR}/src/gui/qt/qttools/qttoolswrapper.h
    ${CMAKE_SOURCE_DIR}/src/gui/qttoolsinterface.h
    ${CMAKE_SOURCE_DIR}/src/net/clientcallback.h
    ${CMAKE_SOURCE_DIR}/src/net/clientcontext.h
    ${CMAKE_SOURCE_DIR}/src/net/clientexception.h
    ${CMAKE_SOURCE_DIR}/src/net/clientstate.h
    ${CMAKE_SOURCE_DIR}/src/net/clientthread.h
    ${CMAKE_SOURCE_DIR}/src/net/genericsocket.h
    ${CMAKE_SOURCE_DIR}/src/net/net_helper.h
    ${CMAKE_SOURCE_DIR}/src/net/netpacket.h
    ${CMAKE_SOURCE_DIR}/src/net/senderhelper.h
    ${CMAKE_SOURCE_DIR}/src/net/serveraccepthelper.h
    ${CMAKE_SOURCE_DIR}/src/net/serverlobbythread.h
    ${CMAKE_SOURCE_DIR}/src/net/socket_helper.h
    ${CMAKE_SOURCE_DIR}/src/net/socket_msg.h
    ${CMAKE_SOURCE_DIR}/src/net/socket_startup.h
    ${CMAKE_SOURCE_DIR}/src/playerdata.h
    ${CMAKE_SOURCE_DIR}/src/session.h
    ${CMAKE_SOURCE_DIR}/src/singleton.h
    )

set(pokerth_qml_SRCS
    ${CMAKE_SOURCE_DIR}/src/core/common/loghelper_client.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/local_engine/replay.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/qml/configwrapper.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/qml/customqmlapplicationengine.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/qml/gamedataclass.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/qml/guiwrapper.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/qml/imageprovider.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/qml/manager.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/qml/playermodel.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/qml/qmlcard.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/qml/qmlgame.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/qml/qmlplayer.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/qml/qmlserver.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/qml/servermodel.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/qml/storeplayers.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/qml/styles/gametablestylereader.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/qt/qttools/qthelper/qthelper.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/qt/qttools/qttoolswrapper.cpp
    ${CMAKE_SOURCE_DIR}/src/net/common/net_helper_client.cpp
    ${CMAKE_SOURCE_DIR}/src/net/common/servermanagerfactoryclient.cpp
    ${CMAKE_SOURCE_DIR}/src/pokerth_qml.cpp
    )

set(RESOURCES
    ${CMAKE_SOURCE_DIR}/src/gui/qml/resources/pokerth.qrc
    )

if(UBUNTU_COMPONENTS)
    set(RESOURCES
        ${CMAKE_SOURCE_DIR}/src/gui/qml/resources/pokerth_ubuntu_qml.qrc
        )
else(UBUNTU_COMPONENTS)
    set(RESOURCES
        ${CMAKE_SOURCE_DIR}/src/gui/qml/resources/pokerth_qml.qrc
        )
endif(UBUNTU_COMPONENTS)

set(CLICK_FILES
    ${CMAKE_SOURCE_DIR}/pokerth.ico
    ${CMAKE_SOURCE_DIR}/click/pokerth.desktop
    ${CMAKE_SOURCE_DIR}/click/pokerth.json
    ${CMAKE_SOURCE_DIR}/click/manifest.json
    )

qtcreator_add_project_resources(${RESOURCES})
qtcreator_add_project_resources(${CLICK_FILES})

qt5_add_resources(RESOURCE_ADDED ${RESOURCES})

SET_SOURCE_FILES_PROPERTIES(
    ${CMAKE_SOURCE_DIR}/pokerth.icns
    PROPERTIES
    MACOSX_PACKAGE_LOCATION Resources
    ) 

set(LIBS_TO_COPY ${GSASL_LIBRARIES} ${SDL_LIBRARY} ${SDL_MIXER_LIBRARY}
    ${SQLITE3_LIBRARIES} ${TINYXML_LIBRARIES} ${PROTOBUF_LIBRARIES}
    ${GCRYPT_LIBRARIES} ${Boost_LIBRARIES}
    )

if(CLICK_MODE)
    deploy_libs(${LIBS_TO_COPY})
endif(CLICK_MODE)

set(LIBS pokerth_lib pokerth_db pokerth_protocol curl ${LIBS_TO_COPY})

add_executable(pokerth
    ${pokerth_qml_MOC_SRCS}
    ${pokerth_qml_SRCS}
    ${pokerth_qml_HDRS}
    ${RESOURCE_ADDED}
    ${CMAKE_SOURCE_DIR}/pokerth.icns
    )

qt5_use_modules(pokerth Gui Quick Qml Core Widgets Sql)

target_link_libraries(pokerth
    ${LIBS})

install(TARGETS pokerth
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/)

if(CLICK_MODE)
    install(FILES ${CLICK_FILES}
        DESTINATION ${CMAKE_INSTALL_DATADIR}/
        )
endif()
install(FILES pokerth.sh
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
    DESTINATION ${CMAKE_INSTALL_DATADIR}/
    )
