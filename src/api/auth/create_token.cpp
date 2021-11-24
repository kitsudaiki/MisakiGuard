﻿/**
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

#include <misaka_root.h>

#include <libKitsunemimiCrypto/hashes.h>
#include <libKitsunemimiJwt/jwt.h>
#include <libKitsunemimiJson/json_item.h>
#include <libKitsunemimiHanamiCommon/enums.h>

using namespace Kitsunemimi::Sakura;

CreateToken::CreateToken()
    : Kitsunemimi::Sakura::Blossom()
{
    registerInputField("user_name", true);
    registerInputField("pw", true);

    registerOutputField("token");
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
    // get information from request
    const std::string userName = blossomLeaf.input.getStringByKey("user_name");
    std::string pwHash = "";
    Kitsunemimi::Crypto::generate_SHA_256(pwHash, blossomLeaf.input.getStringByKey("pw"));

    // get data from table
    Kitsunemimi::Json::JsonItem userData;
    if(MisakaRoot::usersTable->getUserByName(userData, userName, error) == false)
    {
        status.errorMessage = "ACCESS DENIED!\nUser or password is incorrect.";
        error.addMeesage(status.errorMessage);
        status.statusCode = Kitsunemimi::Hanami::UNAUTHORIZED_RTYPE;
        return false;
    }

    // check password
    if(userData.get("pw_hash").getString() != pwHash)
    {
        status.errorMessage = "ACCESS DENIED!\nUser or password is incorrect.";
        error.addMeesage(status.errorMessage);
        status.statusCode = Kitsunemimi::Hanami::UNAUTHORIZED_RTYPE;
        return false;
    }

    // TODO: make validation-time configurable
    std::string jwtToken;
    userData.remove("pw_hash");
    MisakaRoot::jwt->create_HS256_Token(jwtToken, userData, 3600);
    blossomLeaf.output.insert("token", new Kitsunemimi::DataValue(jwtToken));
    return true;
}
