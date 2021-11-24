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

using namespace Kitsunemimi::Sakura;

CreateUser::CreateUser()
    : Kitsunemimi::Sakura::Blossom()
{
    registerInputField("user_name", true);
    registerInputField("pw",        true);
    registerInputField("is_admin",  true);
    registerInputField("groups",    true);

    registerOutputField("uuid");
    registerOutputField("user_name");
    registerOutputField("pw_hash");
    registerOutputField("is_admin");
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
    UsersTable::UserData userData;

    // get input-data
    userData.name = blossomLeaf.input.getStringByKey("user_name");
    Kitsunemimi::Crypto::generate_SHA_256(userData.pwHash, blossomLeaf.input.getStringByKey("pw"));
    const std::string groups = blossomLeaf.input.getStringByKey("groups");
    userData.isAdmin = blossomLeaf.input.get("is_admin")->toValue()->getBool();
    Kitsunemimi::splitStringByDelimiter(userData.groups, groups, ',');

    // add new user to table
    const std::string uuid = MisakaRoot::usersTable->addUser(userData, error);
    if(uuid.size() == 0)
    {
        status.errorMessage = error.toString();
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    // create response
    blossomLeaf.output.insert("uuid", new Kitsunemimi::DataValue(userData.uuid));
    blossomLeaf.output.insert("user_name", new Kitsunemimi::DataValue(userData.name));
    blossomLeaf.output.insert("is_admin", new Kitsunemimi::DataValue(userData.isAdmin));
    blossomLeaf.output.insert("groups", new Kitsunemimi::DataValue(groups));

    return true;
}
