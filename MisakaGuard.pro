QT -= qt core gui

TARGET = MisakaGuard
CONFIG += console c++17
CONFIG -= app_bundle


LIBS += -L../libKitsunemimiHanamiCommon/src -lKitsunemimiHanamiCommon
LIBS += -L../libKitsunemimiHanamiCommon/src/debug -lKitsunemimiHanamiCommon
LIBS += -L../libKitsunemimiHanamiCommon/src/release -lKitsunemimiHanamiCommon
INCLUDEPATH += ../libKitsunemimiHanamiCommon/include

LIBS += -L../libKitsunemimiSakuraLang/src -lKitsunemimiSakuraLang
LIBS += -L../libKitsunemimiSakuraLang/src/debug -lKitsunemimiSakuraLang
LIBS += -L../libKitsunemimiSakuraLang/src/release -lKitsunemimiSakuraLang
INCLUDEPATH += ../libKitsunemimiSakuraLang/include

LIBS += -L../libKitsunemimiArgs/src -lKitsunemimiArgs
LIBS += -L../libKitsunemimiArgs/src/debug -lKitsunemimiArgs
LIBS += -L../libKitsunemimiArgs/src/release -lKitsunemimiArgs
INCLUDEPATH += ../libKitsunemimiArgs/include

LIBS += -L../libKitsunemimiConfig/src -lKitsunemimiConfig
LIBS += -L../libKitsunemimiConfig/src/debug -lKitsunemimiConfig
LIBS += -L../libKitsunemimiConfig/src/release -lKitsunemimiConfig
INCLUDEPATH += ../libKitsunemimiConfig/include

LIBS += -L../libKitsunemimiSqlite/src -lKitsunemimiSqlite
LIBS += -L../libKitsunemimiSqlite/src/debug -lKitsunemimiSqlite
LIBS += -L../libKitsunemimiSqlite/src/release -lKitsunemimiSqlite
INCLUDEPATH += ../libKitsunemimiSqlite/include

LIBS += -L../libKitsunemimiSakuraNetwork/src -lKitsunemimiSakuraNetwork
LIBS += -L../libKitsunemimiSakuraNetwork/src/debug -lKitsunemimiSakuraNetwork
LIBS += -L../libKitsunemimiSakuraNetwork/src/release -lKitsunemimiSakuraNetwork
INCLUDEPATH += ../libKitsunemimiSakuraNetwork/include

LIBS += -L../libKitsunemimiSakuraDatabase/src -lKitsunemimiSakuraDatabase
LIBS += -L../libKitsunemimiSakuraDatabase/src/debug -lKitsunemimiSakuraDatabase
LIBS += -L../libKitsunemimiSakuraDatabase/src/release -lKitsunemimiSakuraDatabase
INCLUDEPATH += ../libKitsunemimiSakuraDatabase/include

LIBS += -L../libKitsunemimiCommon/src -lKitsunemimiCommon
LIBS += -L../libKitsunemimiCommon/src/debug -lKitsunemimiCommon
LIBS += -L../libKitsunemimiCommon/src/release -lKitsunemimiCommon
INCLUDEPATH += ../libKitsunemimiCommon/include

LIBS += -L../libKitsunemimiNetwork/src -lKitsunemimiNetwork
LIBS += -L../libKitsunemimiNetwork/src/debug -lKitsunemimiNetwork
LIBS += -L../libKitsunemimiNetwork/src/release -lKitsunemimiNetwork
INCLUDEPATH += ../libKitsunemimiNetwork/include

LIBS += -L../libKitsunemimiJinja2/src -lKitsunemimiJinja2
LIBS += -L../libKitsunemimiJinja2/src/debug -lKitsunemimiJinja2
LIBS += -L../libKitsunemimiJinja2/src/release -lKitsunemimiJinja2
INCLUDEPATH += ../libKitsunemimiJinja2/include

LIBS += -L../libKitsunemimiJson/src -lKitsunemimiJson
LIBS += -L../libKitsunemimiJson/src/debug -lKitsunemimiJson
LIBS += -L../libKitsunemimiJson/src/release -lKitsunemimiJson
INCLUDEPATH += ../libKitsunemimiJson/include

LIBS += -L../libKitsunemimiIni/src -lKitsunemimiIni
LIBS += -L../libKitsunemimiIni/src/debug -lKitsunemimiIni
LIBS += -L../libKitsunemimiIni/src/release -lKitsunemimiIni
INCLUDEPATH += ../libKitsunemimiIni/include

LIBS += -L../libKitsunemimiJwt/src -lKitsunemimiJwt
LIBS += -L../libKitsunemimiJwt/src/debug -lKitsunemimiJwt
LIBS += -L../libKitsunemimiJwti/src/release -lKitsunemimiJwt
INCLUDEPATH += ../libKitsunemimiJwt/include

LIBS += -L../libKitsunemimiCrypto/src -lKitsunemimiCrypto
LIBS += -L../libKitsunemimiCrypto/src/debug -lKitsunemimiCrypto
LIBS += -L../libKitsunemimiCrypto/src/release -lKitsunemimiCrypto
INCLUDEPATH += ../libKitsunemimiCrypto/include

LIBS += -L../libKitsunemimiHanamiEndpoints/src -lKitsunemimiHanamiEndpoints
LIBS += -L../libKitsunemimiHanamiEndpoints/src/debug -lKitsunemimiHanamiEndpoints
LIBS += -L../libKitsunemimiHanamiEndpoints/src/release -lKitsunemimiHanamiEndpoints
INCLUDEPATH += ../libKitsunemimiHanamiEndpoints/include

LIBS += -L../libKitsunemimiHanamiPolicies/src -lKitsunemimiHanamiPolicies
LIBS += -L../libKitsunemimiHanamiPolicies/src/debug -lKitsunemimiHanamiPolicies
LIBS += -L../libKitsunemimiHanamiPolicies/src/release -lKitsunemimiHanamiPolicies
INCLUDEPATH += ../libKitsunemimiHanamiPolicies/include

LIBS += -lcryptopp -lssl -lsqlite3 -luuid -lcrypto

INCLUDEPATH += $$PWD \
               src

SOURCES += src/main.cpp \
    src/api/auth/create_token.cpp \
    src/api/user/create_user.cpp \
    src/api/user/get_user.cpp \
    src/misaka_root.cpp \
    src/users/users_database.cpp \
    src/api/auth/validate_token.cpp

HEADERS += \
    src/api/auth/create_token.h \
    src/api/user/create_user.h \
    src/api/user/get_user.h \
    src/args.h \
    src/config.h \
    src/misaka_root.h \
    src/users/users_database.h \
    src/api/auth/validate_token.h
