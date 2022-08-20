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

#include <misaki_root.h>
#include <libKitsunemimiHanamiCommon/uuid.h>
#include <libKitsunemimiHanamiCommon/enums.h>

#include <libKitsunemimiCrypto/hashes.h>
#include <libKitsunemimiCommon/methods/string_methods.h>
#include <libKitsunemimiJson/json_item.h>

using namespace Kitsunemimi::Sakura;

/**
 * @brief constructor
 */
CreateUser::CreateUser()
    : Kitsunemimi::Sakura::Blossom("Register a new user within Misaki.")
{
    //----------------------------------------------------------------------------------------------
    // input
    //----------------------------------------------------------------------------------------------

    registerInputField("name",
                       SAKURA_STRING_TYPE,
                       true,
                       "Name of the new user.");
    // column in database is limited to 256 characters size
    assert(addFieldBorder("name", 4, 256));
    assert(addFieldRegex("name", "[a-zA-Z][a-zA-Z_0-9]*"));

    registerInputField("projects",
                       SAKURA_STRING_TYPE,
                       true,
                       "Comma-separated list of projects of the user.");
    assert(addFieldRegex("projects", "[a-zA-Z][a-zA-Z_,0-9]*"));

    registerInputField("roles",
                       SAKURA_STRING_TYPE,
                       true,
                       "Comma-separated list of roles of the user.");
    assert(addFieldRegex("roles", "[a-zA-Z][a-zA-Z_,0-9]*"));

    registerInputField("password",
                       SAKURA_STRING_TYPE,
                       true,
                       "Passphrase of the user.");
    assert(addFieldBorder("password", 6, 4096));
    assert(addFieldRegex("password", "[^=]*"));  // no '=' allowed

    registerInputField("is_admin",
                       SAKURA_BOOL_TYPE,
                       false,
                       "Set this to 1 to register the new user as admin.");
    assert(addFieldDefault("is_admin", new Kitsunemimi::DataValue(false)));

    //----------------------------------------------------------------------------------------------
    // output
    //----------------------------------------------------------------------------------------------

    registerOutputField("uuid",
                        SAKURA_STRING_TYPE,
                        "UUID of the new user.");
    registerOutputField("name",
                        SAKURA_STRING_TYPE,
                        "Name of the new user.");
    registerOutputField("is_admin",
                        SAKURA_BOOL_TYPE,
                        "Set this to true to register the new user as admin.");
    registerOutputField("roles",
                        SAKURA_STRING_TYPE,
                        "Comma-separated liste of all roles of the user.");
    registerOutputField("projects",
                        SAKURA_STRING_TYPE,
                        "Comma-separated liste of all projects of the user.");

    //----------------------------------------------------------------------------------------------
    //
    //----------------------------------------------------------------------------------------------
}

/**
 * @brief runTask
 */
bool
CreateUser::runTask(BlossomLeaf &blossomLeaf,
                    const Kitsunemimi::DataMap &context,
                    BlossomStatus &status,
                    Kitsunemimi::ErrorContainer &error)
{
    const std::string userName = blossomLeaf.input.get("name").getString();
    const std::string userUuid = context.getStringByKey("uuid");
    const std::string projectUuid = context.getStringByKey("projects");
    const bool isAdmin = context.getBoolByKey("is_admin");

    // check if user already exist within the table
    Kitsunemimi::Json::JsonItem getResult;
    if(MisakiRoot::usersTable->getUserByName(getResult,
                                             userName,
                                             userUuid,
                                             projectUuid,
                                             isAdmin,
                                             error))
    {
        status.errorMessage = "User with name '" + userName + "' already exist.";
        status.statusCode = Kitsunemimi::Hanami::CONFLICT_RTYPE;
        return false;
    }

    // generate predefined uuid, because this will also be used as salt-value
    const std::string uuid = Kitsunemimi::Hanami::generateUuid().toString();

    // genreate hash from password
    std::string pwHash;
    const std::string saltedPw = blossomLeaf.input.get("password").getString() + uuid;
    Kitsunemimi::Crypto::generate_SHA_256(pwHash, saltedPw);

    // convert values
    Kitsunemimi::Json::JsonItem userData;
    userData.insert("uuid", uuid);
    userData.insert("name", userName);
    userData.insert("roles", blossomLeaf.input.get("roles"));
    userData.insert("projects", blossomLeaf.input.get("projects"));
    userData.insert("pw_hash", pwHash);
    userData.insert("is_admin", blossomLeaf.input.get("is_admin").getBool());
    userData.insert("project_uuid", "-");
    userData.insert("owner_uuid", "-");
    userData.insert("visibility", "private");

    // add new user to table
    if(MisakiRoot::usersTable->addUser(userData,
                                       userUuid,
                                       projectUuid,
                                       error) == false)
    {
        status.errorMessage = error.toString();
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    // get new created user from database
    if(MisakiRoot::usersTable->getUserByName(blossomLeaf.output,
                                             userName,
                                             userUuid,
                                             projectUuid,
                                             isAdmin,
                                             error) == false)
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
