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
        src
        src/engine
        src/gui
        src/qt/qttools
        src/net
        src/engine/local_engine
        src/engine/network_engine
        src/config
        src/core
        src/gui/qml)

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
   include_directories(../boost/ ../GnuTLS/include ../gsasl/include
       ../curl/include ../zlib ../sqlite ../openssl/include)
endif(WIN32)

set(pokerth_qml_HDRS
    src/engine/game.h
    src/session.h
    src/playerdata.h
    src/gamedata.h
    src/config/configfile.h
    src/core/thread.h
    src/core/loghelper.h
    src/engine/boardinterface.h
    src/engine/enginefactory.h
    src/engine/handinterface.h
    src/engine/playerinterface.h
    src/engine/berointerface.h
    src/gui/guiinterface.h
    src/net/clientcallback.h
    src/net/clientcontext.h
    src/net/clientexception.h
    src/net/clientstate.h
    src/net/clientthread.h
    src/net/genericsocket.h
    src/net/netpacket.h
    src/net/senderhelper.h
    src/net/serveraccepthelper.h
    src/net/serverlobbythread.h
    src/net/socket_helper.h
    src/net/socket_msg.h
    src/net/socket_startup.h
    src/net/net_helper.h
    src/engine/local_engine/cardsvalue.h
    src/engine/local_engine/localboard.h
    src/engine/local_engine/localenginefactory.h
    src/engine/local_engine/localhand.h
    src/engine/local_engine/localplayer.h
    src/engine/local_engine/localberopreflop.h
    src/engine/local_engine/localberoflop.h
    src/engine/local_engine/localberoturn.h
    src/engine/local_engine/localberoriver.h
    src/engine/local_engine/localberopostriver.h
    src/engine/local_engine/tools.h
    src/engine/local_engine/localbero.h
    src/engine/network_engine/clientboard.h
    src/engine/network_engine/clientenginefactory.h
    src/engine/network_engine/clienthand.h
    src/engine/network_engine/clientplayer.h
    src/engine/network_engine/clientbero.h
    src/gui/qttoolsinterface.h
    src/gui/qt/qttools/qttoolswrapper.h
    src/gui/qt/qttools/qthelper/qthelper.h
    src/gui/generic/serverguiwrapper.h
    src/engine/local_engine/replay.h
    src/gui/qml/guiwrapper.h
    src/gui/qml/manager.h
    src/singleton.h
    src/gui/qml/customqmlapplicationengine.h
    src/gui/qml/configwrapper.h
    src/gui/qml/gamedataclass.h
    src/gui/qml/imageprovider.h
    src/gui/qml/styles/gametablestylereader.h
    src/gui/qml/playermodel.h
    src/gui/qml/storeplayers.h
    src/gui/qml/qmlgame.h
    src/gui/qml/qmlcard.h
    src/gui/qml/qmlserver.h
    src/gui/qml/qmlplayer.h
    src/gui/qml/servermodel.h)

set(pokerth_qml_SRCS
    src/pokerth_qml.cpp
    src/gui/qttoolsinterface.h
    src/gui/qt/qttools/qttoolswrapper.cpp
    src/gui/qt/qttools/qthelper/qthelper.cpp
    src/net/common/net_helper_client.cpp
    src/core/common/loghelper_client.cpp
    src/engine/local_engine/replay.cpp
    src/net/common/servermanagerfactoryclient.cpp
    src/gui/qml/guiwrapper.cpp
    src/gui/qml/manager.cpp
    src/gui/qml/customqmlapplicationengine.cpp
    src/gui/qml/configwrapper.cpp
    src/gui/qml/gamedataclass.cpp
    src/gui/qml/imageprovider.cpp
    src/gui/qml/styles/gametablestylereader.cpp
    src/gui/qml/playermodel.cpp
    src/gui/qml/storeplayers.cpp
    src/gui/qml/qmlgame.cpp
    src/gui/qml/qmlcard.cpp
    src/gui/qml/qmlserver.cpp
    src/gui/qml/qmlplayer.cpp
    src/gui/qml/servermodel.cpp)

set(RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/qml/resources/pokerth.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/qml/resources/pokerth_qml.qrc
    #${CMAKE_CURRENT_SOURCE_DIR}/src/gui/qml/resources/pokerth_ubuntu_qml.qrc
)
set(CLICK_FILES
    pokerth.ico
    click/pokerth.desktop
    click/pokerth.json
    click/manifest.json
)

qtcreator_add_project_resources(${RESOURCES})
qtcreator_add_project_resources(${CLICK_FILES})

qt5_add_resources(RESOURCE_ADDED ${RESOURCES})

set(LIBS_TO_COPY ${GSASL_LIBRARIES} ${SDL_LIBRARY} ${SDL_MIXER_LIBRARY}
    ${SQLITE3_LIBRARIES} ${TINYXML_LIBRARIES} ${PROTOBUF_LIBRARIES}
    ${GCRYPT_LIBRARIES} ${Boost_LIBRARIES})

deploy_libs(${LIBS_TO_COPY})
set(LIBS pokerth_lib pokerth_db pokerth_protocol curl ${LIBS_TO_COPY})

#foreach(lib ${LIBS_TO_COPY})
#    resolve_lib(${lib} lib_resolved success)
#    if(success)
#        set(RESOLVED_LIBS_TO_COPY ${RESOLVED_LIBS_TO_COPY} ${lib_resolved})
#        set(LIBS ${LIBS} ${lib_resolved})
#    else(success)
#        set(LIBS ${LIBS} ${lib_resolved})
#    endif(success)
#endforeach(lib)

add_executable(pokerth_qml
    ${pokerth_qml_MOC_SRCS}
    ${pokerth_qml_SRCS}
    ${RESOURCE_ADDED})

qt5_use_modules(pokerth_qml Gui Quick Qml Core Widgets Sql)

target_link_libraries(pokerth_qml
    ${LIBS})

install(TARGETS pokerth_qml
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/)

if(CLICK_MODE)
    install(FILES ${CLICK_FILES}
        DESTINATION ${CMAKE_INSTALL_DATADIR}/)
endif()
install(FILES pokerth.sh
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
    DESTINATION ${CMAKE_INSTALL_DATADIR}/
    )
