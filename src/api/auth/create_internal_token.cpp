﻿/**
 * @file        create_internal_token.cpp
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

#include "create_internal_token.h"

#include <misaka_root.h>

#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiCrypto/hashes.h>
#include <libKitsunemimiJwt/jwt.h>
#include <libKitsunemimiJson/json_item.h>

#include <libKitsunemimiHanamiCommon/enums.h>

using namespace Kitsunemimi::Sakura;

CreateInternalToken::CreateInternalToken()
    : Kitsunemimi::Sakura::Blossom("Create a JWT-access-token for a internal services, "
                                   "which can not be used from the outside.")
{
    registerInputField("service_name",
                       SAKURA_STRING_TYPE,
                       true,
                       "Name of the service.");

    registerOutputField("token",
                        SAKURA_STRING_TYPE,
                        "New JWT-access-token for the service.");
}

/**
 * @brief runTask
 */
bool
CreateInternalToken::runTask(BlossomLeaf &blossomLeaf,
                             const Kitsunemimi::DataMap &,
                             BlossomStatus &status,
                             Kitsunemimi::ErrorContainer &)
{
    // get information from request
    const std::string serviceName = blossomLeaf.input.getStringByKey("service_name");

    // create struct with the payload for the token
    Kitsunemimi::Json::JsonItem serviceData;
    serviceData.insert("name", new Kitsunemimi::DataValue(serviceName));

    // TODO: make validation-time configurable
    std::string jwtToken;
    if(MisakaRoot::jwt->create_HS256_Token(jwtToken, serviceData, 3600) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    // cut of signature to make the token invalid for the usage for outside of the node
    std::vector<std::string> splitedContent;
    Kitsunemimi::splitStringByDelimiter(splitedContent, jwtToken, '.');
    jwtToken = splitedContent.at(0) + "." + splitedContent.at(1) + "._";

    // create output
    blossomLeaf.output.insert("token", new Kitsunemimi::DataValue(jwtToken));

    return true;
}
