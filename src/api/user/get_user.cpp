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
    : Kitsunemimi::Sakura::Blossom()
{
    registerInputField("user_name", true);

    registerOutputField("uuid");
    registerOutputField("user_name");
    registerOutputField("pw_hash");
    registerOutputField("is_admin");
}

/**
 * @brief runTask
 */
bool
GetUser::runTask(BlossomLeaf &blossomLeaf,
                 const Kitsunemimi::DataMap &,
                 BlossomStatus &status,
                 Kitsunemimi::ErrorContainer &error)
{
    // get information from request
    const std::string userName = blossomLeaf.input.getStringByKey("user_name");

    // get data from table
    UsersTable::UserData userData;
    Kitsunemimi::TableItem table;
    if(MisakaRoot::usersTable->getUserByName(userData, table, userName, error) == false)
    {
        status.errorMessage = error.toString();
        status.statusCode = Kitsunemimi::Hanami::NOT_FOUND_RTYPE;
        return false;
    }

    return true;
}
