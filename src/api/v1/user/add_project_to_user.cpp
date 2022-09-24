/**
 * @file        add_project_to_user.cpp
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

#include "add_project_to_user.h"

#include <misaki_root.h>
#include <libKitsunemimiHanamiCommon/uuid.h>
#include <libKitsunemimiHanamiCommon/enums.h>
#include <libKitsunemimiHanamiCommon/defines.h>

#include <libKitsunemimiCrypto/hashes.h>
#include <libKitsunemimiCommon/methods/string_methods.h>
#include <libKitsunemimiJson/json_item.h>

using namespace Kitsunemimi::Sakura;

/**
 * @brief constructor
 */
AddProjectToUser::AddProjectToUser()
    : Blossom("Add a project to a specific user.")
{
    //----------------------------------------------------------------------------------------------
    // input
    //----------------------------------------------------------------------------------------------

    registerInputField("user_id",
                       SAKURA_STRING_TYPE,
                       true,
                       "ID of the user.");
    assert(addFieldBorder("id", 4, 256));
    assert(addFieldRegex("id", ID_EXT_REGEX));

    registerInputField("project_id",
                       SAKURA_STRING_TYPE,
                       true,
                       "ID of the project, which has to be added to the user.");
    assert(addFieldBorder("project_id", 4, 256));
    assert(addFieldRegex("project_id", ID_REGEX));

    registerInputField("roles",
                       SAKURA_STRING_TYPE,
                       true,
                       "Role, which has to be assigned to the user within the project");
    assert(addFieldBorder("roles", 4, 256));
    assert(addFieldRegex("roles", ID_REGEX));

    registerInputField("is_project_admin",
                       SAKURA_BOOL_TYPE,
                       false,
                       "Set this to true, if the user should be an admin "
                       "within the assigned project.");
    assert(addFieldDefault("is_project_admin", new Kitsunemimi::DataValue(false)));

    //----------------------------------------------------------------------------------------------
    // output
    //----------------------------------------------------------------------------------------------

    registerOutputField("id",
                        SAKURA_STRING_TYPE,
                        "ID of the user.");
    registerOutputField("name",
                        SAKURA_STRING_TYPE,
                        "Name of the user.");
    registerOutputField("is_admin",
                        SAKURA_BOOL_TYPE,
                        "True, if user is an admin.");
    registerOutputField("creator_id",
                        SAKURA_STRING_TYPE,
                        "Id of the creator of the user.");
    registerOutputField("projects",
                        SAKURA_STRING_TYPE,
                        "Json-formated string with all assigned projects "
                        "together with roles and project-admin-status.");

    //----------------------------------------------------------------------------------------------
    //
    //----------------------------------------------------------------------------------------------
}

/**
 * @brief runTask
 */
bool
AddProjectToUser::runTask(BlossomLeaf &blossomLeaf,
                          const Kitsunemimi::DataMap &context,
                          BlossomStatus &status,
                          Kitsunemimi::ErrorContainer &error)
{
    // check if admin
    if(context.getBoolByKey("is_admin") == false)
    {
        status.statusCode = Kitsunemimi::Hanami::UNAUTHORIZED_RTYPE;
        return false;
    }

    const std::string userId = blossomLeaf.input.get("id").getString();
    const std::string projectId = blossomLeaf.input.get("project_id").getString();
    const std::string roles = blossomLeaf.input.get("roles").getString();
    const bool isProjectAdmin = blossomLeaf.input.get("is_project_admin").getBool();
    const std::string creatorId = context.getStringByKey("id");

    // check if user already exist within the table
    Kitsunemimi::Json::JsonItem getResult;
    if(MisakiRoot::usersTable->getUser(getResult, userId, error, false))
    {
        status.errorMessage = "User with id '" + userId + "' already exist.";
        status.statusCode = Kitsunemimi::Hanami::CONFLICT_RTYPE;
        return false;
    }

    // parse projects from result
    Kitsunemimi::Json::JsonItem parsedProjects;
    if(parsedProjects.parse(getResult.get("projects").getString(), error) == false)
    {
        error.addMeesage("Failed to parse projects of user with id '" + userId + "'");
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    // check if project is already assigned to user
    for(uint64_t i = 0; i < parsedProjects.size(); i++)
    {
        if(parsedProjects.get(i).get("project_id").getString() == projectId)
        {
            status.errorMessage = "Project with ID '"
                                  + projectId
                                  + "' is already assigned to user with id '"
                                  + userId
                                  + "'.";
            error.addMeesage(status.errorMessage);
            status.statusCode = Kitsunemimi::Hanami::CONFLICT_RTYPE;
            return false;
        }
    }

    // create new entry
    Kitsunemimi::Json::JsonItem newEntry;
    newEntry.insert("project_id", projectId);
    newEntry.insert("roles", roles);
    newEntry.insert("is_project_admin", isProjectAdmin);
    parsedProjects.append(newEntry);

    // updated projects of user in database
    if(MisakiRoot::usersTable->updateProjectsOfUser(userId,
                                                    parsedProjects.toString(),
                                                    error) == false)
    {
        error.addMeesage("Failed to update projects of user with id '" + userId + "'.");
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    // get new created user from database
    if(MisakiRoot::usersTable->getUser(blossomLeaf.output,
                                       userId,
                                       error,
                                       false) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    return true;
}
