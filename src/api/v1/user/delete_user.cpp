/**
 * @file        delete_user.cpp
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

#include "delete_user.h"

#include <misaka_root.h>

#include <libKitsunemimiJson/json_item.h>

#include <libKitsunemimiHanamiCommon/enums.h>

using namespace Kitsunemimi::Sakura;

/**
 * @brief constructor
 */
DeleteUser::DeleteUser()
    : Kitsunemimi::Sakura::Blossom("Delete a specific user from the database.")
{
    // input
    registerInputField("user_name",
                       SAKURA_STRING_TYPE,
                       true,
                       "Name of the user.");
    // column in database is limited to 256 characters size
    assert(addFieldBorder("user_name", 4, 256));
    assert(addFieldRegex("user_name", "[a-zA-Z][a-zA-Z_0-9]*"));
}

/**
 * @brief runTask
 */
bool
DeleteUser::runTask(BlossomLeaf &blossomLeaf,
                    const Kitsunemimi::DataMap &,
                    BlossomStatus &status,
                    Kitsunemimi::ErrorContainer &error)
{
    // get information from request
    const std::string userName = blossomLeaf.input.get("user_name").getString();

    // check if user exist within the table
    Kitsunemimi::Json::JsonItem getResult;
    if(MisakaRoot::usersTable->getUserByName(getResult, userName, error) == false)
    {
        status.errorMessage = "User with name '" + userName + "' not found.";
        status.statusCode = Kitsunemimi::Hanami::NOT_FOUND_RTYPE;
        return false;
    }

    // get data from table
    if(MisakaRoot::usersTable->deleteUser(userName, error) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    return true;
}
