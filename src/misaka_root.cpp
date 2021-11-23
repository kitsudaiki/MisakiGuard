/**
 * @file        misaka_root.cpp
 *
 * @author      Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 * @copyright   Apache License Version 2.0
 *
 *      Copyright 2021 Tobias Anker
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include "misaka_root.h"

#include <libKitsunemimiConfig/config_handler.h>
#include <libKitsunemimiSakuraDatabase/sql_database.h>
#include <libKitsunemimiCommon/files/text_file.h>

#include <api/blossom_initializing.h>

Kitsunemimi::Jwt::Jwt* MisakaRoot::jwt = nullptr;
UsersTable* MisakaRoot::usersTable = nullptr;
Kitsunemimi::Sakura::SqlDatabase* MisakaRoot::database = nullptr;
Kitsunemimi::Hanami::Policy* MisakaRoot::policies = nullptr;

MisakaRoot::MisakaRoot() {}

/**
 * @brief MisakaRoot::init
 * @return
 */
bool
MisakaRoot::init()
{
    Kitsunemimi::ErrorContainer error;
    bool success = false;

    // read database-path from config
    database = new Kitsunemimi::Sakura::SqlDatabase();
    const std::string databasePath = GET_STRING_CONFIG("DEFAULT", "database", success);
    if(success == false)
    {
        error.addMeesage("No database-path defined in config.");
        LOG_ERROR(error);
        return false;
    }

    // initalize database
    if(database->initDatabase(databasePath, error) == false)
    {
        error.addMeesage("Failed to initialize sql-database.");
        LOG_ERROR(error);
        return false;
    }

    // initialize users-table
    usersTable = new UsersTable(database);
    if(usersTable->initTable(error) == false)
    {
        error.addMeesage("Failed to initialize user-table in database.");
        LOG_ERROR(error);
        return false;
    }

    initBlossoms();

    // read jwt-token-key from config
    const std::string tokenKeyString = GET_STRING_CONFIG("misaka", "token_key", success);
    if(success == false)
    {
        error.addMeesage("No token-key defined in config.");
        LOG_ERROR(error);
        return false;
    }

    // init jwt for token create and sign
    CryptoPP::SecByteBlock tokenKey((unsigned char*)tokenKeyString.c_str(), tokenKeyString.size());
    jwt = new Kitsunemimi::Jwt::Jwt(tokenKey);

    // read policy-file-path from config
    const std::string policyFilePath = GET_STRING_CONFIG("misaka", "policies", success);
    if(success == false)
    {
        error.addMeesage("No policy-file defined in config.");
        LOG_ERROR(error);
        return false;
    }

    // read policy-file
    std::string policyFileContent;
    if(Kitsunemimi::readFile(policyFileContent, policyFilePath, error) == false)
    {
        error.addMeesage("Failed to read policy-file");
        LOG_ERROR(error);
        return false;
    }

    // parse policy-file
    policies = new Kitsunemimi::Hanami::Policy();
    if(policies->parse(policyFileContent, error) == false)
    {
        error.addMeesage("Failed to parser policy-file");
        LOG_ERROR(error);
        return false;
    }

    return true;
}
