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

#include "get_user.h"

#include <misaka_root.h>
#include <libKitsunemimiHanamiCommon/enums.h>

using namespace Kitsunemimi::Sakura;

GetUser::GetUser()
    : Kitsunemimi::Hanami::HanamiBlossom()
{
    registerInputField("user_name", true);
    registerInputField("get_table", false);

    registerOutputField("uuid",      true);
    registerOutputField("user_name", true);
    registerOutputField("pw_hash",   true);
    registerOutputField("is_admin",  true);
    registerOutputField("table",     false);
}

bool
GetUser::runTask(BlossomLeaf &blossomLeaf,
                 uint64_t &status,
                 std::string &errorMessage)
{
    Kitsunemimi::ErrorContainer error;

    // get information from request
    const std::string userName = blossomLeaf.input.getStringByKey("user_name");
    bool getTable = false;
    if(blossomLeaf.input.contains("get_table")) {
        getTable = blossomLeaf.input.get("get_table")->toValue()->getBool();
    }

    // get data from table
    UsersTable::UserData userData;
    Kitsunemimi::TableItem table;
    if(MisakaRoot::usersTable->getUserByName(userData, table, userName, error) == false)
    {
        errorMessage = error.errorMessage;
        status = Kitsunemimi::Hanami::NOT_FOUND_RTYPE;
        return false;
    }

    // create response
    blossomLeaf.output = *table.getRow(0, false);
    if(getTable) {
        blossomLeaf.output.insert("table", table.stealContent());
    }

    return true;
}
