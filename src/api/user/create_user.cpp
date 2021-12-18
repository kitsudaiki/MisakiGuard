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
    registerInputField("user_projects",
                       SAKURA_STRING_TYPE,
                       true,
                       "Comma-separated list of projects of the user.");
    registerInputField("user_roles",
                       SAKURA_STRING_TYPE,
                       true,
                       "Comma-separated list of roles of the user.");
    registerInputField("pw",
                       SAKURA_STRING_TYPE,
                       true,
                       "Passphrase of the user.");
    registerInputField("is_admin",
                       SAKURA_BOOL_TYPE,
                       true,
                       "Set this to 1 to register the new user as admin.");

    // output
    registerOutputField("uuid",
                        SAKURA_STRING_TYPE,
                        "UUID of the new user.");
    registerOutputField("user_name",
                        SAKURA_STRING_TYPE,
                        "Name of the new user.");
    registerOutputField("is_admin",
                        SAKURA_BOOL_TYPE,
                        "Show if the user is an admin or not.");
    registerOutputField("roles",
                        SAKURA_STRING_TYPE,
                        "Comma-separated list of roles of the user.");

    //TODO: hanle projects
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

    // genreate hash from password
    std::string pwHash;
    Kitsunemimi::Crypto::generate_SHA_256(pwHash, blossomLeaf.input.get("pw").getString());

    // convert values
    const std::string userName = blossomLeaf.input.get("user_name").getString();
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

    return true;
}
