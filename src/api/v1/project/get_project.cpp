/**
 * @file        get_project.cpp
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

#include "get_project.h"

#include <misaki_root.h>

#include <libKitsunemimiJson/json_item.h>

#include <libKitsunemimiHanamiCommon/enums.h>

using namespace Kitsunemimi::Sakura;

/**
 * @brief constructor
 */
GetProject::GetProject()
    : Kitsunemimi::Sakura::Blossom("Show information of a specific registered user.")
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
    // output
    //----------------------------------------------------------------------------------------------

    registerOutputField("uuid",
                        SAKURA_STRING_TYPE,
                        "UUID of the user.");
    registerOutputField("name",
                        SAKURA_STRING_TYPE,
                        "Name of the user.");
    registerOutputField("is_admin",
                        SAKURA_BOOL_TYPE,
                        "Set this to true to register the new user as admin.");
    registerOutputField("roles",
                        SAKURA_STRING_TYPE,
                        "Comma-separated liste of all roles of the user.");
    registerOutputField("projects",
                        SAKURA_STRING_TYPE,
                        "Comma-separated liste of all projects of the user.");

    //----------------------------------------------------------------------------------------------
    //
    //----------------------------------------------------------------------------------------------
}

/**
 * @brief runTask
 */
bool
GetProject::runTask(BlossomLeaf &blossomLeaf,
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

    // get data from table
    if(MisakiRoot::projectsTable->getProjectByName(blossomLeaf.output, projectName, error) == false)
    {
        status.errorMessage = "Project with name '" + projectName + "' not found.";
        status.statusCode = Kitsunemimi::Hanami::NOT_FOUND_RTYPE;
        return false;
    }

    // remove irrelevant fields
    blossomLeaf.output.remove("owner_uuid");
    blossomLeaf.output.remove("project_uuid");
    blossomLeaf.output.remove("visibility");

    return true;
}
