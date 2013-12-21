# QMake pro-file for PokerTH game

folder_data.source = data
folder_data.target = data
DEPLOYMENTFOLDERS += folder_data
CONFIG(debug, debug|release) {
    CONFIG += NO_RESOURCES
}
NO_RESOURCES {
    DEFINES += NO_RESOURCES
    no_resources.source = src/gui/qml/resources
    no_resources.target =
    DEPLOYMENTFOLDERS += no_resources
}

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =


# Hack around https://bugreports.qt-project.org/browse/QTBUG-22829
QMAKE_MOC = $$QMAKE_MOC -DBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

isEmpty( PREFIX ){
	PREFIX =/usr
}

# Installation path
# target.path =

TEMPLATE = app
CODECFORSRC = UTF-8
CONFIG += qt \
	thread \
	embed_manifest_exe \
	exceptions \
	rtti \
	stl \
        warn_on
QT += sql widgets

include(deploy.pri)
include(src/third_party/qtsingleapplication/qtsingleapplication.pri)

MOC_DIR = mocs
OBJECTS_DIR = obj
DEFINES += ENABLE_IPV6 TIXML_USE_STL BOOST_FILESYSTEM_DEPRECATED
DEFINES += PREFIX=\"$${PREFIX}\"
TARGET = pokerth
RESOURCES = src/gui/qml/resources/pokerth.qrc
!NO_RESOURCES {
    RESOURCES += src/gui/qml/resources/pokerth_qml.qrc
}

# PRECOMPILED_HEADER = src/pch_game.h

INCLUDEPATH += . \
	src \
	src/engine \
        src/gui \
        src/qt/qttools \
	src/net \
	src/engine/local_engine \
	src/engine/network_engine \
        src/config \
        src/core \
        src/gui/qml

DEPENDPATH += . \
	src \
	src/config \
	src/core \
	src/engine \
	src/gui \
	src/net \
	src/core/common \
	src/engine/local_engine \
        src/engine/network_engine \
        src/net/common \

# Input
HEADERS += src/engine/game.h \
	src/session.h \
	src/playerdata.h \
	src/gamedata.h \
	src/config/configfile.h \
	src/core/thread.h \
	src/core/loghelper.h \
	src/engine/boardinterface.h \
	src/engine/enginefactory.h \
	src/engine/handinterface.h \
	src/engine/playerinterface.h \
	src/engine/berointerface.h \
	src/gui/guiinterface.h \
	src/net/clientcallback.h \
	src/net/clientcontext.h \
	src/net/clientexception.h \
	src/net/clientstate.h \
	src/net/clientthread.h \
	src/net/genericsocket.h \
	src/net/netpacket.h \
	src/net/senderhelper.h \
	src/net/serveraccepthelper.h \
	src/net/serverlobbythread.h \
	src/net/socket_helper.h \
	src/net/socket_msg.h \
	src/net/socket_startup.h \
	src/net/net_helper.h \
	src/engine/local_engine/cardsvalue.h \
	src/engine/local_engine/localboard.h \
	src/engine/local_engine/localenginefactory.h \
	src/engine/local_engine/localhand.h \
	src/engine/local_engine/localplayer.h \
	src/engine/local_engine/localberopreflop.h \
	src/engine/local_engine/localberoflop.h \
	src/engine/local_engine/localberoturn.h \
	src/engine/local_engine/localberoriver.h \
	src/engine/local_engine/localberopostriver.h \
	src/engine/local_engine/tools.h \
	src/engine/local_engine/localbero.h \
	src/engine/network_engine/clientboard.h \
	src/engine/network_engine/clientenginefactory.h \
	src/engine/network_engine/clienthand.h \
	src/engine/network_engine/clientplayer.h \
        src/engine/network_engine/clientbero.h \
        src/gui/qttoolsinterface.h \
        src/gui/qt/qttools/qttoolswrapper.h \
        src/gui/qt/qttools/qthelper/qthelper.h \
        src/gui/generic/serverguiwrapper.h \
        src/engine/local_engine/replay.h \
    src/gui/qml/guiwrapper.h \
    src/gui/qml/manager.h \
    src/singleton.h \
    src/gui/qml/customqmlapplicationengine.h \
    src/gui/qml/configwrapper.h \
    src/gui/qml/gamedataclass.h \
    src/gui/qml/imageprovider.h \
    src/gui/qml/styles/gametablestylereader.h \
    src/gui/qml/playermodel.h \
    src/gui/qml/storeplayers.h \
    src/gui/qml/qmlgame.h \
    src/gui/qml/qmlcard.h \
    src/gui/qml/qmlserver.h \
    src/gui/qml/qmlplayer.h \
    src/gui/qml/servermodel.h

SOURCES += src/pokerth_qml.cpp \
        src/gui/qttoolsinterface.h \
        src/gui/qt/qttools/qttoolswrapper.cpp \
        src/gui/qt/qttools/qthelper/qthelper.cpp \
	src/net/common/net_helper_client.cpp \
        src/core/common/loghelper_client.cpp \
        src/engine/local_engine/replay.cpp \
        src/net/common/servermanagerfactoryclient.cpp \
    src/gui/qml/guiwrapper.cpp \
    src/gui/qml/manager.cpp \
    src/gui/qml/customqmlapplicationengine.cpp \
    src/gui/qml/configwrapper.cpp \
    src/gui/qml/gamedataclass.cpp \
    src/gui/qml/imageprovider.cpp \
    src/gui/qml/styles/gametablestylereader.cpp \
    src/gui/qml/playermodel.cpp \
    src/gui/qml/storeplayers.cpp \
    src/gui/qml/qmlgame.cpp \
    src/gui/qml/qmlcard.cpp \
    src/gui/qml/qmlserver.cpp \
    src/gui/qml/qmlplayer.cpp \
    src/gui/qml/servermodel.cpp

TRANSLATIONS = ts/pokerth_af.ts \
	ts/pokerth_bg.ts \
	ts/pokerth_zhcn.ts \
	ts/pokerth_ca.ts \
	ts/pokerth_cz.ts \
	ts/pokerth_dk.ts \
	ts/pokerth_nl.ts \
	ts/pokerth_fr.ts \
	ts/pokerth_fi.ts \
	ts/pokerth_de.ts \
	ts/pokerth_gd.ts \
	ts/pokerth_gl.ts \
	ts/pokerth_gr.ts \
	ts/pokerth_hu.ts \
	ts/pokerth_it.ts \
	ts/pokerth_jp.ts \
	ts/pokerth_lt.ts \
	ts/pokerth_no.ts \
	ts/pokerth_pl.ts \
	ts/pokerth_ptbr.ts \
	ts/pokerth_ptpt.ts \
	ts/pokerth_ru.ts \
	ts/pokerth_sk.ts \
	ts/pokerth_es.ts \
	ts/pokerth_sv.ts \
	ts/pokerth_ta.ts \
	ts/pokerth_tr.ts \
	ts/pokerth_vi.ts \
	ts/pokerth_START_HERE.ts

LIBS += -lpokerth_lib \
	-lpokerth_db \
	-lpokerth_protocol \
	-lcurl

win32 { 
	DEFINES += CURL_STATICLIB
	DEFINES += _WIN32_WINNT=0x0501
	DEFINES += HAVE_OPENSSL
	DEPENDPATH += src/net/win32/ \
		src/core/win32
	INCLUDEPATH += ../boost/ \
		../SDL/include \
		../SDL/include/SDL \
		../SDL_mixer/include \
		../openssl/include \
		../gsasl/include \
		../curl/include \
		../sqlite \
		../zlib
	LIBPATH += ../boost/stage/lib \
		../openssl/lib \
		../gsasl/lib \
		../curl/lib \
		../SDL/lib \
		../SDL_mixer/lib \
		../mysql/lib \
		../zlib
	LIBS += -lSDL_mixer \
			-lsmpeg \
			-lSDL \
			-lSDLmain \
			-ltinyxml \
			-lz \
			-lssl \
			-lcrypto \
			-lssh2 \
			-lgnutls \
			-lhogweed \
			-lgmp \
			-lgcrypt \
			-lgpg-error \
			-lgsasl \
			-lnettle \
			-lidn \
			-lintl \
			-lprotobuf
	debug:LIBPATH += debug/lib
	release:LIBPATH += release/lib
			LIBS += -lsqlite3
			LIBS += -lntlm -lmodplug -lddraw -ldxguid -lvorbisfile -lvorbis -logg
			LIBS += -lboost_thread_win32-mt
			LIBS += -lboost_filesystem-mt
			LIBS += -lboost_regex-mt
			LIBS += -lboost_iostreams-mt
			LIBS += -lboost_random-mt
			LIBS += -lboost_chrono-mt
			LIBS += -lboost_system-mt

	LIBS += \
		-lgdi32 \
		-lcomdlg32 \
		-loleaut32 \
		-limm32 \
		-lwinmm \
		-lwinspool \
		-lole32 \
		-luuid \
		-luser32 \
		-lmsimg32 \
		-lshell32 \
		-lkernel32 \
		-lmswsock \
		-lws2_32 \
		-ladvapi32 \
		-lwldap32 \
		-lcrypt32
	RC_FILE = pokerth.rc
}
unix:!mac { 
	# #### My release static build options
	# QMAKE_CXXFLAGS += -ffunction-sections -fdata-sections
	# QMAKE_LFLAGS += -Wl,--gc-sections
	INCLUDEPATH += $${PREFIX}/include
	QMAKE_LIBDIR += lib
	!android{
                LIBPATH += $${PREFIX}/lib /opt/gsasl/lib
                LIB_DIRS = $${PREFIX}/lib \
                        $${PREFIX}/lib64 \
                        $$system(qmake -query QT_INSTALL_LIBS) \
                        /usr/lib/x86_64-linux-gnu
	}
	android{
		LIBPATH += $${PREFIX}/lib/armv7
		LIB_DIRS = $${PREFIX}/lib/armv7
	}
	BOOST_FS = boost_filesystem \
		boost_filesystem-mt
	BOOST_THREAD = boost_thread \
		boost_thread-mt
	BOOST_IOSTREAMS = boost_iostreams \
		boost_iostreams-mt
	BOOST_REGEX = boost_regex \
		boost_regex-mt
	BOOST_SYS = boost_system \
		boost_system-mt
	BOOST_RANDOM = boost_random \
		boost_random-mt

	# searching in $PREFIX/lib and $PREFIX/lib64
	# to override the default '/usr' pass PREFIX
	# variable to qmake.
	for(dir, LIB_DIRS):exists($$dir) {
			for(lib, BOOST_THREAD):exists($${dir}/lib$${lib}.so*) {
				message("Found $$lib")
				BOOST_THREAD = -l$$lib
			}
			for(lib, BOOST_THREAD):exists($${dir}/lib$${lib}.a) {
				message("Found $$lib")
				BOOST_THREAD = -l$$lib
			}
			for(lib, BOOST_FS):exists($${dir}/lib$${lib}.so*) {
				message("Found $$lib")
				BOOST_FS = -l$$lib
			}
			for(lib, BOOST_FS):exists($${dir}/lib$${lib}.a) {
				message("Found $$lib")
				BOOST_FS = -l$$lib
			}
			for(lib, BOOST_IOSTREAMS):exists($${dir}/lib$${lib}.so*) {
				message("Found $$lib")
				BOOST_IOSTREAMS = -l$$lib
			}
			for(lib, BOOST_IOSTREAMS):exists($${dir}/lib$${lib}.a) {
				message("Found $$lib")
				BOOST_IOSTREAMS = -l$$lib
			}
			for(lib, BOOST_REGEX):exists($${dir}/lib$${lib}.so*) {
				message("Found $$lib")
				BOOST_REGEX = -l$$lib
			}
			for(lib, BOOST_REGEX):exists($${dir}/lib$${lib}.a) {
				message("Found $$lib")
				BOOST_REGEX = -l$$lib
			}
			!android{
				for(lib, BOOST_RANDOM):exists($${dir}/lib$${lib}.so*) {
					message("Found $$lib")
					BOOST_RANDOM = -l$$lib
				}
				for(lib, BOOST_RANDOM):exists($${dir}/lib$${lib}.a) {
					message("Found $$lib")
					BOOST_RANDOM = -l$$lib
				}
			}
			for(lib, BOOST_SYS):exists($${dir}/lib$${lib}.so*) {
				message("Found $$lib")
				BOOST_SYS = -l$$lib
			}
			for(lib, BOOST_SYS):exists($${dir}/lib$${lib}.a) {
				message("Found $$lib")
				BOOST_SYS = -l$$lib
			}
	}
	!android{
		BOOST_LIBS = $$BOOST_THREAD \
			$$BOOST_FS \
			$$BOOST_IOSTREAMS \
			$$BOOST_REGEX \
			$$BOOST_RANDOM \
			$$BOOST_SYS
		!count(BOOST_LIBS, 6):error("Unable to find boost libraries in PREFIX=$${PREFIX}")
		if($$system(sdl-config --version)):error("sdl-config not found in PATH - libSDL_mixer, libSDL are required!")
		UNAME = $$system(uname -s)
		BSD = $$find(UNAME, "BSD")
		kFreeBSD = $$find(UNAME, "kFreeBSD")
		LIBS += -lsqlite3 \
				-ltinyxml \
				-lprotobuf
		LIBS += $$BOOST_LIBS
		LIBS += -lSDL \
				-lSDL_mixer \
			-lgsasl
		!isEmpty( BSD ):isEmpty( kFreeBSD ):LIBS += -lcrypto
		else:LIBS += -lgcrypt
	}
	android{
		BOOST_LIBS = $$BOOST_THREAD \
			$$BOOST_FS \
			$$BOOST_IOSTREAMS \
			$$BOOST_REGEX \
			$$BOOST_SYS
		!count(BOOST_LIBS, 5):error("Unable to find boost libraries in PREFIX=$${PREFIX}/armv5")
		LIBS += -ltinyxml
		LIBS += $$BOOST_LIBS
		LIBS += -lgsasl -lidn
		LIBS += -lssl -lcrypto -lgcrypt -lgpg-error -lprotobuf-lite

		!android_api8 {
			#android sound lib for api9 and higher
			LIBS += -lOpenSLES
		}
	}
	TARGETDEPS += ./lib/libpokerth_lib.a \
		./lib/libpokerth_db.a \
		./lib/libpokerth_protocol.a

	!android{
		# #### My release static libs
		# LIBS += -lgcrypt_static -lgpg-error_static -lgnutls_static -lSDL_mixer_static -lSDL -lmikmod -lcurl
                # ### INSTALL ####
                binary.path += $${PREFIX}/bin/
                binary.files += pokerth
                data.path += $${PREFIX}/share/pokerth/data/
                data.files += data/*
                pixmap.path += $${PREFIX}/share/pixmaps/
                pixmap.files += pokerth.png
		desktop.path += $${PREFIX}/share/applications/
		desktop.files += pokerth.desktop
		INSTALLS += binary \
			data \
			pixmap \
			desktop
	}
}
mac { 
	# make it x86_64 only
	CONFIG += x86_64
	CONFIG -= x86
	CONFIG -= ppc
	QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6
	QMAKE_CXXFLAGS -= -std=gnu++0x

	# workaround for problems with boost_filesystem exceptions
	QMAKE_LFLAGS += -no_dead_strip_inits_and_terms

	# for universal-compilation on PPC-Mac uncomment the following line
	# on Intel-Mac you have to comment this line out or build will fail.
	# QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk/
	LIBPATH += lib

	# QT dynamic linked framework (see also mac_post_make.sh)
	LIBS += -framework \
		QtCore
	LIBS += -framework \
		QtGui

	# SDL and SDL_mixer come as frameworks
	LIBS += -framework \
		SDL
	LIBS += -framework \
		SDL_mixer

	# make sure you have an x86_64 version of boost
	LIBS += /usr/local/lib/libboost_thread.a
	LIBS += /usr/local/lib/libboost_filesystem.a
	LIBS += /usr/local/lib/libboost_regex.a
	LIBS += /usr/local/lib/libboost_random.a
	LIBS += /usr/local/lib/libboost_system.a
	LIBS += /usr/local/lib/libboost_iostreams.a
	LIBS += /usr/local/lib/libgsasl.a

	# libraries installed on every mac
	LIBS += -lcrypto \
		-lssl \
		-lsqlite3 \
		-ltinyxml \
		-lprotobuf \
		-lz \
		-framework \
		Carbon

	# set the application icon
	RC_FILE = pokerth.icns
	LIBPATH += /Developer/SDKs/MacOSX10.6.sdk/usr/lib
	INCLUDEPATH += /Developer/SDKs/MacOSX10.6.sdk/usr/include/
	INCLUDEPATH += /Library/Frameworks/SDL.framework/Headers
	INCLUDEPATH += /Library/Frameworks/SDL_mixer.framework/Headers
}
OTHER_FILES += docs/infomessage-id-desc.txt
official_server {
	LIBPATH += pkth_stat/daemon_lib/lib
	LIBS += -lpokerth_closed \
		-lmysqlpp
	DEFINES += POKERTH_OFFICIAL_SERVER
}

gui_800x480 {

        DEFINES += GUI_800x480
}

android{
	# Use old boost::filesystem, because the new version requires std::wstring.
	DEFINES += BOOST_FILESYSTEM_VERSION=3
	# sqlite3 is included directly.
	INCLUDEPATH += src/third_party/sqlite3
}

maemo{
	DEFINES += MAEMO
}

qtcAddDeployment()
