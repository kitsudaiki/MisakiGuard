/**
 * @file        delete_project.cpp
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

#include "delete_project.h"

#include <misaki_root.h>

#include <libKitsunemimiJson/json_item.h>

#include <libKitsunemimiHanamiCommon/enums.h>

using namespace Kitsunemimi::Sakura;

/**
 * @brief constructor
 */
DeleteProject::DeleteProject()
    : Kitsunemimi::Sakura::Blossom("Delete a specific user from the database.")
{
    //----------------------------------------------------------------------------------------------
    // input
    //----------------------------------------------------------------------------------------------

    registerInputField("name",
                       SAKURA_STRING_TYPE,
                       true,
                       "Name of the user.");
    // column in database is limited to 256 characters size
    assert(addFieldBorder("name", 4, 256));
    assert(addFieldRegex("name", "[a-zA-Z][a-zA-Z_0-9]*"));

    //----------------------------------------------------------------------------------------------
    //
    //----------------------------------------------------------------------------------------------
}

/**
 * @brief runTask
 */
bool
DeleteProject::runTask(BlossomLeaf &blossomLeaf,
                       const Kitsunemimi::DataMap &context,
                       BlossomStatus &status,
                       Kitsunemimi::ErrorContainer &error)
{
    const bool isAdmin = context.getBoolByKey("is_admin");
    if(isAdmin == false)
    {
        status.statusCode = Kitsunemimi::Hanami::UNAUTHORIZED_RTYPE;
        return false;
    }

    // get information from request
    const std::string projectName = blossomLeaf.input.get("name").getString();

    // check if user exist within the table
    Kitsunemimi::Json::JsonItem result;
    if(MisakiRoot::projectsTable->getProjectByName(result, projectName, error) == false)
    {
        status.errorMessage = "Project with name '" + projectName + "' not found.";
        status.statusCode = Kitsunemimi::Hanami::NOT_FOUND_RTYPE;
        error.addMeesage(status.errorMessage);
        return false;
    }

    // get data from table
    if(MisakiRoot::projectsTable->deleteProject(projectName, error) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    return true;
}
