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
    : Kitsunemimi::Hanami::HanamiBlossom()
{
    registerInputField("user_name", true);
    registerInputField("pw",        true);
    registerInputField("is_admin",  true);
    registerInputField("groups",    true);
    registerInputField("get_table", false);

    registerOutputField("uuid",      true);
    registerOutputField("user_name", true);
    registerOutputField("pw_hash",   true);
    registerOutputField("is_admin",  true);
    registerOutputField("table",     false);
}

bool
CreateUser::runTask(BlossomLeaf &blossomLeaf,
                    BlossomStatus &status,
                    std::string &errorMessage)
{
    Kitsunemimi::ErrorContainer error;
    UsersTable::UserData userData;

    // get input-data
    userData.userName = blossomLeaf.input.getStringByKey("user_name");
    Kitsunemimi::Crypto::generate_SHA_256(userData.pwHash, blossomLeaf.input.getStringByKey("pw"));
    const std::string groups = blossomLeaf.input.getStringByKey("groups");
    userData.isAdmin = blossomLeaf.input.get("is_admin")->toValue()->getBool();
    Kitsunemimi::splitStringByDelimiter(userData.groups, groups, ',');

    bool getTable = false;
    if(blossomLeaf.input.contains("get_table")) {
        getTable = blossomLeaf.input.get("get_table")->toValue()->getBool();
    }

    // add new user to table
    const std::string uuid = MisakaRoot::usersTable->addUser(userData, error);
    if(uuid.size() == 0)
    {
        errorMessage = error.errorMessage;
        status.errorMessage = errorMessage;
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    // get data from table
    Kitsunemimi::TableItem table;
    if(MisakaRoot::usersTable->getUser(table, uuid, error) == false)
    {
        errorMessage = error.errorMessage;
        status.errorMessage = errorMessage;
        status.statusCode = Kitsunemimi::Hanami::NOT_FOUND_RTYPE;
        return false;
    }

    // create response
    blossomLeaf.output = *table.getRow(0, false);
    if(getTable) {
        blossomLeaf.output.insert("table", table.stealContent());
    }

    return true;
}
