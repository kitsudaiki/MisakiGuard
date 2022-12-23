QT -= qt core gui

TARGET = MisakiGuard
CONFIG += console c++17
CONFIG -= app_bundle

LIBS += -L../libAzukiHeart/src -lAzukiHeart
LIBS += -L../libAzukiHeart/src/debug -lAzukiHeart
LIBS += -L../libAzukiHeart/src/release -lAzukiHeart
INCLUDEPATH += ../libAzukiHeart/include

LIBS += -L../libMisakiGuard/src -lMisakiGuard
LIBS += -L../libMisakiGuard/src/debug -lMisakiGuard
LIBS += -L../libMisakiGuard/src/release -lMisakiGuard
INCLUDEPATH += ../libMisakiGuard/include

LIBS += -L../libKitsunemimiHanamiNetwork/src -lKitsunemimiHanamiNetwork
LIBS += -L../libKitsunemimiHanamiNetwork/src/debug -lKitsunemimiHanamiNetwork
LIBS += -L../libKitsunemimiHanamiNetwork/src/release -lKitsunemimiHanamiNetwork
INCLUDEPATH += ../libKitsunemimiHanamiNetwork/include

LIBS += -L../libKitsunemimiHanamiDatabase/src -lKitsunemimiHanamiDatabase
LIBS += -L../libKitsunemimiHanamiDatabase/src/debug -lKitsunemimiHanamiDatabase
LIBS += -L../libKitsunemimiHanamiDatabase/src/release -lKitsunemimiHanamiDatabase
INCLUDEPATH += ../libKitsunemimiHanamiDatabase/include

LIBS += -L../libKitsunemimiHanamiPolicies/src -lKitsunemimiHanamiPolicies
LIBS += -L../libKitsunemimiHanamiPolicies/src/debug -lKitsunemimiHanamiPolicies
LIBS += -L../libKitsunemimiHanamiPolicies/src/release -lKitsunemimiHanamiPolicies
INCLUDEPATH += ../libKitsunemimiHanamiPolicies/include

LIBS += -L../libKitsunemimiHanamiCommon/src -lKitsunemimiHanamiCommon
LIBS += -L../libKitsunemimiHanamiCommon/src/debug -lKitsunemimiHanamiCommon
LIBS += -L../libKitsunemimiHanamiCommon/src/release -lKitsunemimiHanamiCommon
INCLUDEPATH += ../libKitsunemimiHanamiCommon/include

LIBS += -L../libKitsunemimiSakuraDatabase/src -lKitsunemimiSakuraDatabase
LIBS += -L../libKitsunemimiSakuraDatabase/src/debug -lKitsunemimiSakuraDatabase
LIBS += -L../libKitsunemimiSakuraDatabase/src/release -lKitsunemimiSakuraDatabase
INCLUDEPATH += ../libKitsunemimiSakuraDatabase/include

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

LIBS += -L../libKitsunemimiCommon/src -lKitsunemimiCommon
LIBS += -L../libKitsunemimiCommon/src/debug -lKitsunemimiCommon
LIBS += -L../libKitsunemimiCommon/src/release -lKitsunemimiCommon
INCLUDEPATH += ../libKitsunemimiCommon/include

LIBS += -L../libKitsunemimiNetwork/src -lKitsunemimiNetwork
LIBS += -L../libKitsunemimiNetwork/src/debug -lKitsunemimiNetwork
LIBS += -L../libKitsunemimiNetwork/src/release -lKitsunemimiNetwork
INCLUDEPATH += ../libKitsunemimiNetwork/include

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


LIBS += -lcryptopp -lssl -lsqlite3 -luuid -lcrypto -pthread -lprotobuf

INCLUDEPATH += $$PWD \
               src

SOURCES += src/main.cpp \
    src/api/v1/auth/create_internal_token.cpp \
    src/api/v1/auth/create_token.cpp \
    src/api/v1/auth/list_user_projects.cpp \
    src/api/v1/auth/renew_token.cpp \
    src/api/v1/auth/validate_access.cpp \
    src/api/v1/project/create_project.cpp \
    src/api/v1/project/delete_project.cpp \
    src/api/v1/project/get_project.cpp \
    src/api/v1/project/list_projects.cpp \
    src/api/v1/user/add_project_to_user.cpp \
    src/api/v1/user/create_user.cpp \
    src/api/v1/user/delete_user.cpp \
    src/api/v1/user/get_user.cpp \
    src/api/v1/user/list_users.cpp \
    src/api/v1/documentation/generate_rest_api_docu.cpp \
    src/api/v1/user/remove_project_from_user.cpp \
    src/database/projects_table.cpp \
    src/misaki_root.cpp \
    src/database/users_table.cpp

HEADERS += \
    src/api/v1/auth/create_internal_token.h \
    src/api/v1/auth/create_token.h \
    src/api/v1/auth/list_user_projects.h \
    src/api/v1/auth/renew_token.h \
    src/api/v1/auth/validate_access.h \
    src/api/blossom_initializing.h \
    src/api/v1/project/create_project.h \
    src/api/v1/project/delete_project.h \
    src/api/v1/project/get_project.h \
    src/api/v1/project/list_projects.h \
    src/api/v1/user/add_project_to_user.h \
    src/api/v1/user/create_user.h \
    src/api/v1/user/delete_user.h \
    src/api/v1/user/get_user.h \
    src/api/v1/user/list_users.h \
    src/api/v1/user/remove_project_from_user.h \
    src/args.h \
    src/callbacks.h \
    src/config.h \
    src/api/v1/documentation/generate_rest_api_docu.h \
    src/database/projects_table.h \
    src/misaki_root.h \
    src/database/users_table.h

AZUKI_PROTO_BUFFER = ../libKitsunemimiHanamiMessages/protobuffers/azuki_messages.proto3

OTHER_FILES += $$AZUKI_PROTO_BUFFER

protobuf_decl.name = protobuf headers
protobuf_decl.name = protobuf headers
protobuf_decl.input = KYOUKO_PROTO_BUFFER
protobuf_decl.output = ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_BASE}.proto3.pb.h
protobuf_decl.commands = protoc --cpp_out=${QMAKE_FILE_IN_PATH} --proto_path=${QMAKE_FILE_IN_PATH} ${QMAKE_FILE_NAME}
protobuf_decl.variable_out = HEADERS
QMAKE_EXTRA_COMPILERS += protobuf_decl

protobuf_impl.name = protobuf sources
protobuf_impl.input = KYOUKO_PROTO_BUFFER
protobuf_impl.output = ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_BASE}.proto3.pb.cc
protobuf_impl.depends = ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_BASE}.proto3.pb.h
protobuf_impl.commands = $$escape_expand(\n)
protobuf_impl.variable_out = SOURCES
QMAKE_EXTRA_COMPILERS += protobuf_impl
