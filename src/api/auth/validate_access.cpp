/**
 * @file        validate_access.cpp
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

#include "validate_access.h"

#include <libKitsunemimiCommon/common_items/data_items.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiJwt/jwt.h>
#include <libKitsunemimiJson/json_item.h>

#include <libKitsunemimiHanamiCommon/enums.h>
#include <libKitsunemimiHanamiPolicies/policy.h>

#include <misaka_root.h>

using namespace Kitsunemimi::Sakura;
using Kitsunemimi::Hanami::HttpRequestType;

ValidateAccess::ValidateAccess()
    : Kitsunemimi::Sakura::Blossom()
{
    registerInputField("token", true);
    registerInputField("component", false);
    registerInputField("endpoint", false);
    registerInputField("http_type", false);

    registerOutputField("uuid");
    registerOutputField("name");
    registerOutputField("is_admin");
    registerOutputField("groups");
}

/**
 * @brief runTask
 */
bool
ValidateAccess::runTask(BlossomLeaf &blossomLeaf,
                        const Kitsunemimi::DataMap &,
                        BlossomStatus &status,
                        Kitsunemimi::ErrorContainer &error)
{
    // collect information from the input
    const std::string token = blossomLeaf.input.getStringByKey("token");
    const std::string component = blossomLeaf.input.getStringByKey("component");
    const std::string endpoint = blossomLeaf.input.getStringByKey("endpoint");

    // validate token
    Kitsunemimi::Json::JsonItem payload;
    if(MisakaRoot::jwt->validateToken(payload, token, error) == false)
    {
        error.addMeesage("Misaka failed to validate JWT-Token");
        status.errorMessage = "JWT-Token invalid or broken!";
        status.statusCode = Kitsunemimi::Hanami::UNAUTHORIZED_RTYPE;
        return false;
    }

    // allow skipping policy-check
    // TODO: find better solution to make a difference, if policy should be checked or not
    if(component != "")
    {
        if(blossomLeaf.input.contains("http_type") == false)
        {
            error.addMeesage("http_type is missing in token-request");
            status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
            return false;
        }

        const uint32_t httpTypeValue = blossomLeaf.input.get("http_type")->toValue()->getInt();
        const HttpRequestType httpType = static_cast<HttpRequestType>(httpTypeValue);

        // process payload to get groups of user
        std::vector<std::string> groups;
        const std::string groupString = payload.get("groups").getString();
        Kitsunemimi::splitStringByDelimiter(groups, groupString, ',');

        // check policy
        bool foundPolicy = false;
        for(const std::string &group : groups)
        {
            if(MisakaRoot::policies->checkUserAgainstPolicy(component, endpoint, httpType, group)) {
                foundPolicy = true;
            }
        }

        // if no matching policy was found, then deny access
        if(foundPolicy == false)
        {
            status.errorMessage = "Access denied by policy";
            status.statusCode = Kitsunemimi::Hanami::UNAUTHORIZED_RTYPE;
            error.addMeesage(status.errorMessage);
            return false;
        }
    }

    // create output
    blossomLeaf.output.insert("groups", payload.get("groups").getItemContent()->copy());
    blossomLeaf.output.insert("is_admin", payload.get("is_admin").getItemContent()->copy());
    blossomLeaf.output.insert("name", payload.get("name").getItemContent()->copy());
    blossomLeaf.output.insert("uuid", payload.get("uuid").getItemContent()->copy());

    return true;
}
