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

/**
 * @brief constructor
 */
ValidateAccess::ValidateAccess()
    : Kitsunemimi::Sakura::Blossom("Checks if a JWT-access-token of a user is valid or not "
                                   "and optional check if the user is allowed by its roles "
                                   "and the policy to access a specific endpoint.")
{
    // input
    registerInputField("token",
                       SAKURA_STRING_TYPE,
                       true,
                       "User specific JWT-access-token.");
    assert(addFieldRegex("token", "[a-zA-Z_.0-9]*"));

    registerInputField("component",
                       SAKURA_STRING_TYPE,
                       false,
                       "Requested component-name of the request. If this is not set, then only "
                       "the token in itself will be validated.");
    assert(addFieldBorder("component", 4, 256));
    assert(addFieldRegex("component", "[a-zA-Z][a-zA-Z_0-9]*"));

    registerInputField("endpoint",
                       SAKURA_STRING_TYPE,
                       false,
                       "Requesed endpoint within the component.");
    assert(addFieldBorder("endpoint", 4, 256));
    assert(addFieldRegex("endpoint", "[a-zA-Z][a-zA-Z_/0-9]*"));

    registerInputField("http_type",
                       SAKURA_INT_TYPE,
                       false,
                       "Type of the HTTP-request as enum "
                       "(DELETE = 1, GET = 2, POST = 4, PUT = 5).");
    assert(addFieldBorder("http_type", 1, 5));

    // output
    registerOutputField("uuid",
                        SAKURA_STRING_TYPE,
                        "UUID of the user.");
    registerOutputField("user_name",
                        SAKURA_STRING_TYPE,
                        "Name of the user.");
    registerOutputField("is_admin",
                        SAKURA_BOOL_TYPE,
                        "Show if the user is an admin or not.");
    registerOutputField("user_roles",
                        SAKURA_STRING_TYPE,
                        "Comma-separated liste of all roles of the user.");
    registerOutputField("user_projects",
                        SAKURA_STRING_TYPE,
                        "Comma-separated liste of all projects of the user.");
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
    const std::string token = blossomLeaf.input.get("token").getString();
    const std::string component = blossomLeaf.input.get("component").getString();
    const std::string endpoint = blossomLeaf.input.get("endpoint").getString();

    // validate token
    std::string publicError;
    if(MisakaRoot::jwt->validateToken(blossomLeaf.output, token, publicError, error) == false)
    {
        error.addMeesage("Misaka failed to validate JWT-Token");
        status.errorMessage = publicError;
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

        const uint32_t httpTypeValue = blossomLeaf.input.get("http_type").getInt();
        const HttpRequestType httpType = static_cast<HttpRequestType>(httpTypeValue);

        // process payload to get roles of user
        std::vector<std::string> roles;
        const std::string rolestring = blossomLeaf.output.get("user_roles").getString();
        Kitsunemimi::splitStringByDelimiter(roles, rolestring, ',');

        // check policy
        bool foundPolicy = false;
        for(const std::string &role : roles)
        {
            if(MisakaRoot::policies->checkUserAgainstPolicy(component, endpoint, httpType, role)) {
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

    // remove irrelevant fields
    blossomLeaf.output.remove("pw_hash");
    blossomLeaf.output.remove("exp");
    blossomLeaf.output.remove("iat");
    blossomLeaf.output.remove("nbf");
    blossomLeaf.output.remove("owner_uuid");
    blossomLeaf.output.remove("project_uuid");
    blossomLeaf.output.remove("visibility");

    return true;
}
