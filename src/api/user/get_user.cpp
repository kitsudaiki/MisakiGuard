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

#include <libKitsunemimiJson/json_item.h>

#include <libKitsunemimiHanamiCommon/enums.h>

using namespace Kitsunemimi::Sakura;

/**
 * @brief constructor
 */
GetUser::GetUser()
    : Kitsunemimi::Sakura::Blossom("Show information of a specific registered user.")
{
    // input
    registerInputField("user_name",
                       SAKURA_STRING_TYPE,
                       true,
                       "Name of the user.");

    // output
    registerOutputField("uuid",
                        SAKURA_STRING_TYPE,
                        "UUID of the new user.");
    registerOutputField("user_name",
                        SAKURA_STRING_TYPE,
                        "Name of the new user.");
    registerOutputField("is_admin",
                        SAKURA_STRING_TYPE,
                        "Set this to true to register the new user as admin.");
    registerOutputField("roles",
                        SAKURA_STRING_TYPE,
                        "Comma-separated list of roles of the user.");

    //TODO: hanle projects
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
    const std::string userName = blossomLeaf.input.get("user_name").getString();

    // get data from table
    if(MisakaRoot::usersTable->getUserByName(blossomLeaf.output, userName, error) == false)
    {
        status.errorMessage = "User with name '" + userName + "' not found.";
        status.statusCode = Kitsunemimi::Hanami::NOT_FOUND_RTYPE;
        return false;
    }

    return true;
}
