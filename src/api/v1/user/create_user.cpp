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
#include <libKitsunemimiHanamiCommon/defines.h>

#include <libKitsunemimiCrypto/hashes.h>
#include <libKitsunemimiCommon/methods/string_methods.h>
#include <libKitsunemimiJson/json_item.h>

using namespace Kitsunemimi::Hanami;

/**
 * @brief constructor
 */
CreateUser::CreateUser()
    : Blossom("Register a new user within Misaki.")
{
    //----------------------------------------------------------------------------------------------
    // input
    //----------------------------------------------------------------------------------------------

    registerInputField("id",
                       SAKURA_STRING_TYPE,
                       true,
                       "ID of the new user.");
    assert(addFieldBorder("id", 4, 256));
    assert(addFieldRegex("id", ID_EXT_REGEX));

    registerInputField("name",
                       SAKURA_STRING_TYPE,
                       true,
                       "Name of the new user.");
    assert(addFieldBorder("name", 4, 256));
    assert(addFieldRegex("name", NAME_REGEX));

    registerInputField("password",
                       SAKURA_STRING_TYPE,
                       true,
                       "Passphrase of the user.");
    assert(addFieldBorder("password", 8, 4096));

    registerInputField("is_admin",
                       SAKURA_BOOL_TYPE,
                       false,
                       "Set this to 1 to register the new user as admin.");
    assert(addFieldDefault("is_admin", new Kitsunemimi::DataValue(false)));

    //----------------------------------------------------------------------------------------------
    // output
    //----------------------------------------------------------------------------------------------

    registerOutputField("id",
                        SAKURA_STRING_TYPE,
                        "ID of the new user.");
    registerOutputField("name",
                        SAKURA_STRING_TYPE,
                        "Name of the new user.");
    registerOutputField("is_admin",
                        SAKURA_BOOL_TYPE,
                        "True, if user is an admin.");
    registerOutputField("creator_id",
                        SAKURA_STRING_TYPE,
                        "Id of the creator of the user.");
    registerOutputField("projects",
                        SAKURA_ARRAY_TYPE,
                        "Json-array with all assigned projects "
                        "together with role and project-admin-status.");

    //----------------------------------------------------------------------------------------------
    //
    //----------------------------------------------------------------------------------------------
}

/**
 * @brief runTask
 */
bool
CreateUser::runTask(BlossomIO &blossomIO,
                    const Kitsunemimi::DataMap &context,
                    BlossomStatus &status,
                    Kitsunemimi::ErrorContainer &error)
{
    // check if admin
    if(context.getBoolByKey("is_admin") == false)
    {
        status.statusCode = Kitsunemimi::Hanami::UNAUTHORIZED_RTYPE;
        return false;
    }

    const std::string newUserId = blossomIO.input.get("id").getString();
    const std::string creatorId = context.getStringByKey("id");

    // check if user already exist within the table
    Kitsunemimi::Json::JsonItem getResult;
    if(MisakiRoot::usersTable->getUser(getResult, newUserId, error, false))
    {
        status.errorMessage = "User with id '" + newUserId + "' already exist.";
        status.statusCode = Kitsunemimi::Hanami::CONFLICT_RTYPE;
        return false;
    }

    // genreate hash from password and random salt
    std::string pwHash;
    const std::string salt = Kitsunemimi::Hanami::generateUuid().toString();
    const std::string saltedPw = blossomIO.input.get("password").getString() + salt;
    Kitsunemimi::Crypto::generate_SHA_256(pwHash, saltedPw);

    // convert values
    Kitsunemimi::Json::JsonItem userData;
    userData.insert("id", newUserId);
    userData.insert("name", blossomIO.input.get("name").getString());
    userData.insert("projects", new Kitsunemimi::DataArray());
    userData.insert("pw_hash", pwHash);
    userData.insert("is_admin", blossomIO.input.get("is_admin").getBool());
    userData.insert("creator_id", creatorId);
    userData.insert("salt", salt);

    // add new user to table
    if(MisakiRoot::usersTable->addUser(userData, error) == false)
    {
        status.errorMessage = error.toString();
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    // get new created user from database
    if(MisakiRoot::usersTable->getUser(blossomIO.output,
                                       newUserId,
                                       error,
                                       false) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    return true;
}
