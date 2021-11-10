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

#include <misaka_root.h>

#include <libKitsunemimiCrypto/hashes.h>
#include <libKitsunemimiJwt/jwt.h>
#include <libKitsunemimiHanamiCommon/enums.h>
#include <misaka_root.h>

using namespace Kitsunemimi::Sakura;

CreateToken::CreateToken()
    : Kitsunemimi::Hanami::HanamiBlossom()
{
    registerInputField("user_name", true);
    registerInputField("pw", true);

    registerOutputField("token", false);
}

bool
CreateToken::runTask(BlossomLeaf &blossomLeaf,
                     BlossomStatus &status,
                     std::string &errorMessage)
{
    Kitsunemimi::ErrorContainer error;

    // get information from request
    const std::string userName = blossomLeaf.input.getStringByKey("user_name");
    std::string pwHash = "";
    Kitsunemimi::Crypto::generate_SHA_256(pwHash, blossomLeaf.input.getStringByKey("pw"));

    // get data from table
    UsersTable::UserData userData;
    Kitsunemimi::TableItem table;
    if(MisakaRoot::usersTable->getUserByName(userData, table, userName, error) == false)
    {
        errorMessage = "user or password is incorrect";
        status.errorMessage = errorMessage;
        status.statusCode = Kitsunemimi::Hanami::UNAUTHORIZED_RTYPE;
        return false;
    }

    // check password
    if(userData.pwHash != pwHash)
    {
        errorMessage = "user or password is incorrect";
        status.errorMessage = errorMessage;
        status.statusCode = Kitsunemimi::Hanami::UNAUTHORIZED_RTYPE;
        return false;
    }

    // create new token
    std::string jwtToken;
    const std::string payload = table.getRow(0, false)->toString();
    MisakaRoot::jwt->create_HS256_Token(jwtToken, payload);
    blossomLeaf.output.insert("token", new Kitsunemimi::DataValue(jwtToken));

    return true;
}
