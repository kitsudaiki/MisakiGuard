/**
 * @file        create_project.cpp
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

#include "create_project.h"

#include <misaki_root.h>
#include <libKitsunemimiHanamiCommon/uuid.h>
#include <libKitsunemimiHanamiCommon/enums.h>

#include <libKitsunemimiCrypto/hashes.h>
#include <libKitsunemimiCommon/methods/string_methods.h>
#include <libKitsunemimiJson/json_item.h>

using namespace Kitsunemimi::Sakura;

/**
 * @brief constructor
 */
CreateProject::CreateProject()
    : Kitsunemimi::Sakura::Blossom("Register a new user within Misaki.")
{
    //----------------------------------------------------------------------------------------------
    // input
    //----------------------------------------------------------------------------------------------

    registerInputField("name",
                       SAKURA_STRING_TYPE,
                       true,
                       "Name of the new user.");
    // column in database is limited to 256 characters size
    assert(addFieldBorder("name", 4, 256));
    assert(addFieldRegex("name", "[a-zA-Z][a-zA-Z_0-9]*"));

    //----------------------------------------------------------------------------------------------
    // output
    //----------------------------------------------------------------------------------------------

    registerOutputField("uuid",
                        SAKURA_STRING_TYPE,
                        "UUID of the new user.");
    registerOutputField("name",
                        SAKURA_STRING_TYPE,
                        "Name of the new user.");
    //----------------------------------------------------------------------------------------------
    //
    //----------------------------------------------------------------------------------------------
}

/**
 * @brief runTask
 */
bool
CreateProject::runTask(BlossomLeaf &blossomLeaf,
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

    // check if user already exist within the table
    Kitsunemimi::Json::JsonItem getResult;
    if(MisakiRoot::projectsTable->getProjectByName(getResult, projectName, error))
    {
        status.errorMessage = "Project with name '" + projectName + "' already exist.";
        status.statusCode = Kitsunemimi::Hanami::CONFLICT_RTYPE;
        return false;
    }

    // generate predefined uuid, because this will also be used as salt-value
    const std::string uuid = Kitsunemimi::Hanami::generateUuid().toString();

    // genreate hash from password
    std::string pwHash;
    const std::string saltedPw = blossomLeaf.input.get("password").getString() + uuid;
    Kitsunemimi::Crypto::generate_SHA_256(pwHash, saltedPw);

    // convert values
    Kitsunemimi::Json::JsonItem userData;
    userData.insert("uuid", uuid);
    userData.insert("name", projectName);
    userData.insert("project_uuid", "-");
    userData.insert("owner_uuid", "-");
    userData.insert("visibility", "private");

    // add new user to table
    if(MisakiRoot::projectsTable->addProject(userData, error) == false)
    {
        status.errorMessage = error.toString();
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    // get new created user from database
    if(MisakiRoot::projectsTable->getProjectByName(blossomLeaf.output,
                                                   projectName,
                                                   error) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    // remove irrelevant fields
    blossomLeaf.output.remove("owner_uuid");
    blossomLeaf.output.remove("project_uuid");
    blossomLeaf.output.remove("visibility");

    return true;
}
