/**
 * @file        create_token.cpp
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

#include "create_token.h"

#include <misaki_root.h>

#include <libKitsunemimiCrypto/hashes.h>
#include <libKitsunemimiJwt/jwt.h>
#include <libKitsunemimiJson/json_item.h>
#include <libKitsunemimiHanamiCommon/enums.h>

using namespace Kitsunemimi::Sakura;

/**
 * @brief constructor
 */
CreateToken::CreateToken()
    : Kitsunemimi::Sakura::Blossom("Create a JWT-access-token for a specific user.")
{
    //----------------------------------------------------------------------------------------------
    // input
    //----------------------------------------------------------------------------------------------

    registerInputField("id",
                       SAKURA_STRING_TYPE,
                       true,
                       "ID of the user.");
    assert(addFieldBorder("id", 4, 256));
    assert(addFieldRegex("id", "[a-zA-Z][a-zA-Z_0-9]*"));

    registerInputField("password",
                       SAKURA_STRING_TYPE,
                       true,
                       "Passphrase of the user, to verify the access.");
    assert(addFieldBorder("password", 6, 4096));
    assert(addFieldRegex("password", "[^=]*"));  // no '=' allowed

    //----------------------------------------------------------------------------------------------
    // output
    //----------------------------------------------------------------------------------------------

    registerOutputField("token",
                        SAKURA_STRING_TYPE,
                        "New JWT-access-token for the user.");

    //----------------------------------------------------------------------------------------------
    //
    //----------------------------------------------------------------------------------------------
}

/**
 * @brief runTask
 */
bool
CreateToken::runTask(BlossomLeaf &blossomLeaf,
                     const Kitsunemimi::DataMap &,
                     BlossomStatus &status,
                     Kitsunemimi::ErrorContainer &error)
{
    const std::string userId = blossomLeaf.input.get("id").getString();

    // get data from table
    Kitsunemimi::Json::JsonItem userData;
    if(MisakiRoot::usersTable->getUser(userData, userId, error, true) == false)
    {
        status.errorMessage = "ACCESS DENIED!\n"
                              "User or password is incorrect.";
        error.addMeesage(status.errorMessage);
        status.statusCode = Kitsunemimi::Hanami::UNAUTHORIZED_RTYPE;
        return false;
    }

    // regenerate password-hash for comparism
    std::string compareHash = "";
    const std::string saltedPw = blossomLeaf.input.get("password").getString()
                                 + userData.get("salt").getString();
    Kitsunemimi::Crypto::generate_SHA_256(compareHash, saltedPw);

    // check password
    const std::string pwHash = userData.get("pw_hash").getString();
    if(pwHash.size() != compareHash.size()
            || memcmp(pwHash.c_str(), compareHash.c_str(), pwHash.size()) != 0)
    {
        status.errorMessage = "ACCESS DENIED!\n"
                              "User or password is incorrect.";
        error.addMeesage(status.errorMessage);
        status.statusCode = Kitsunemimi::Hanami::UNAUTHORIZED_RTYPE;
        return false;
    }

    // remove entries, which are NOT allowed to be part of the token
    std::string jwtToken;
    userData.remove("pw_hash");
    userData.remove("salt");

    // parse projects from result
    Kitsunemimi::Json::JsonItem parsedProjects;
    if(parsedProjects.parse(userData.get("projects").getString(), error) == false)
    {
        error.addMeesage("Failed to parse projects of user with id '" + userId + "'");
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    // get project is user is not an admin.
    const bool isAdmin = userData.get("is_admin").getBool();
    if(isAdmin)
    {
        userData.remove("projects");
        userData.insert("project_id", "-");
        userData.insert("roles", "admin");
        userData.insert("is_project_admin", false);
    }
    else
    {
        if(parsedProjects.size() == 0)
        {
            status.errorMessage = "User with id '" + userId + "' has no project assigned.";
            error.addMeesage(status.errorMessage);
            status.statusCode = Kitsunemimi::Hanami::UNAUTHORIZED_RTYPE;
            return false;
        }
        else
        {
            userData.remove("projects");
            userData.insert("project_id", parsedProjects.get(0).get("project_id"));
            userData.insert("roles", parsedProjects.get(0).get("project_id"));
            userData.insert("is_project_admin", parsedProjects.get(0).get("is_project_admin"));
        }
    }

    // create token
    // TODO: make validation-time configurable
    if(MisakiRoot::jwt->create_HS256_Token(jwtToken, userData, 3600, error) == false)
    {
        error.addMeesage("Failed to create JWT-Token");
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    blossomLeaf.output.insert("token", jwtToken);

    return true;
}
