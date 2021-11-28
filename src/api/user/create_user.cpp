/**
 * @file        create_token.h
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

CreateUser::CreateUser()
    : Kitsunemimi::Sakura::Blossom()
{
    registerInputField("user_name",     true);
    registerInputField("user_projects", true);
    registerInputField("user_roles",    true);
    registerInputField("pw",            true);
    registerInputField("is_admin",      true);

    registerOutputField("uuid");
    registerOutputField("user_name");
    registerOutputField("is_admin");
    registerOutputField("roles");
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
    Kitsunemimi::Crypto::generate_SHA_256(pwHash, blossomLeaf.input.getStringByKey("pw"));

    // convert values
    const std::string userName = blossomLeaf.input.getStringByKey("user_name");
    userData.insert("user_name", userName);
    userData.insert("user_roles", blossomLeaf.input.getStringByKey("user_roles"));
    userData.insert("user_projects", blossomLeaf.input.getStringByKey("user_projects"));
    userData.insert("pw_hash", pwHash);
    userData.insert("is_admin", blossomLeaf.input.get("is_admin")->toValue()->getBool());
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
    Kitsunemimi::Json::JsonItem outputData;
    if(MisakaRoot::usersTable->getUserByName(outputData, userName, error) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    // create response
    blossomLeaf.output = *outputData.getItemContent()->toMap();

    return true;
}
