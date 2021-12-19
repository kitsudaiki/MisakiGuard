/**
 * @file        create_user.cpp
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

#include "create_user.h"

#include <misaka_root.h>
#include <libKitsunemimiHanamiCommon/uuid.h>
#include <libKitsunemimiHanamiCommon/enums.h>

#include <libKitsunemimiCrypto/hashes.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiJson/json_item.h>

using namespace Kitsunemimi::Sakura;

/**
 * @brief constructor
 */
CreateUser::CreateUser()
    : Kitsunemimi::Sakura::Blossom("Register a new user within Misaka.")
{
    // input
    registerInputField("user_name",
                       SAKURA_STRING_TYPE,
                       true,
                       "Name of the new user.");
    // column in database is limited to 256 characters size
    assert(addFieldBorder("user_name", 4, 256));
    assert(addFieldRegex("user_name", "[a-zA-Z][a-zA-Z_0-9]*"));

    registerInputField("user_projects",
                       SAKURA_STRING_TYPE,
                       true,
                       "Comma-separated list of projects of the user.");
    assert(addFieldRegex("user_projects", "[a-zA-Z][a-zA-Z_,0-9]*"));

    registerInputField("user_roles",
                       SAKURA_STRING_TYPE,
                       true,
                       "Comma-separated list of roles of the user.");
    assert(addFieldRegex("user_projects", "[a-zA-Z][a-zA-Z_,0-9]*"));

    registerInputField("pw",
                       SAKURA_STRING_TYPE,
                       true,
                       "Passphrase of the user.");
    assert(addFieldBorder("pw", 8, 4096));
    assert(addFieldRegex("pw", "[^=]*"));  // no = allowed

    registerInputField("is_admin",
                       SAKURA_BOOL_TYPE,
                       false,
                       "Set this to 1 to register the new user as admin.");
    assert(addFieldDefault("is_admin", new Kitsunemimi::DataValue(false)));

    // output
    registerOutputField("uuid",
                        SAKURA_STRING_TYPE,
                        "UUID of the new user.");
    registerOutputField("user_name",
                        SAKURA_STRING_TYPE,
                        "Name of the new user.");
    registerOutputField("is_admin",
                        SAKURA_BOOL_TYPE,
                        "Set this to true to register the new user as admin.");
    registerOutputField("user_roles",
                        SAKURA_STRING_TYPE,
                        "Comma-separated liste of all roles of the user.");
    registerOutputField("user_projects",
                        SAKURA_STRING_TYPE,
                        "Comma-separated liste of all projects of the user.");
}

/**
 * @brief runTask
 */
bool
CreateUser::runTask(BlossomLeaf &blossomLeaf,
                    const Kitsunemimi::DataMap &,
                    BlossomStatus &status,
                    Kitsunemimi::ErrorContainer &error)
{
    Kitsunemimi::Json::JsonItem userData;
    const std::string userName = blossomLeaf.input.get("user_name").getString();

    // check if user already exist within the table
    Kitsunemimi::Json::JsonItem getResult;
    if(MisakaRoot::usersTable->getUserByName(getResult, userName, error))
    {
        status.errorMessage = "User with name '" + userName + "' already exist.";
        status.statusCode = Kitsunemimi::Hanami::CONFLICT_RTYPE;
        return false;
    }

    // genreate hash from password
    std::string pwHash;
    Kitsunemimi::Crypto::generate_SHA_256(pwHash, blossomLeaf.input.get("pw").getString());

    // convert values
    userData.insert("user_name", userName);
    userData.insert("user_roles", blossomLeaf.input.get("user_roles"));
    userData.insert("user_projects", blossomLeaf.input.get("user_projects"));
    userData.insert("pw_hash", pwHash);
    userData.insert("is_admin", blossomLeaf.input.get("is_admin").getBool());
    userData.insert("project_uuid", "-");
    userData.insert("owner_uuid", "-");
    userData.insert("visibility", 0);

    std::cout<<userData.toString(true)<<std::endl;

    // add new user to table
    if(MisakaRoot::usersTable->addUser(userData, error) == false)
    {
        status.errorMessage = error.toString();
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    // get new created user from database
    if(MisakaRoot::usersTable->getUserByName(blossomLeaf.output, userName, error) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    // remove irrelevant fields
    blossomLeaf.output.remove("owner_uuid");
    blossomLeaf.output.remove("project_uuid");
    blossomLeaf.output.remove("visibility");

    return true;
}
