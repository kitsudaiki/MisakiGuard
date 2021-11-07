/**
 * @file        validate_token.cpp
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

#include "validate_token.h"

#include <libKitsunemimiCommon/common_items/data_items.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiJwt/jwt.h>
#include <libKitsunemimiJson/json_item.h>

#include <libKitsunemimiHanamiCommon/enums.h>
#include <libKitsunemimiHanamiPolicies/policy.h>

#include <misaka_root.h>

using namespace Kitsunemimi::Sakura;
using Kitsunemimi::Hanami::HttpRequestType;

ValidateToken::ValidateToken()
    : Blossom()
{
    registerField("token", INPUT_TYPE, true);
    registerField("component", INPUT_TYPE, true);
    registerField("endpoint", INPUT_TYPE, true);
    registerField("http_type", INPUT_TYPE, true);
}

bool
ValidateToken::runTask(BlossomLeaf &blossomLeaf,
                       uint64_t &status,
                       std::string &errorMessage)
{
    const std::string token = blossomLeaf.input.getStringByKey("token");
    const std::string component = blossomLeaf.input.getStringByKey("component");
    const std::string endpoint = blossomLeaf.input.getStringByKey("endpoint");
    const uint32_t httpTypeValue = blossomLeaf.input.get("http_type")->toValue()->getInt();
    const HttpRequestType httpType = static_cast<HttpRequestType>(httpTypeValue);

    std::string payload;
    const bool isValid = MisakaRoot::jwt->validate_HS256_Token(payload, token);
    if(isValid == false)
    {
        errorMessage = "token invalid";
        status = Kitsunemimi::Hanami::UNAUTHORIZED_RESPONE;
        return false;
    }

    Kitsunemimi::Json::JsonItem jsonItem;
    const bool parseResult = jsonItem.parse(payload, errorMessage);
    if(parseResult == false)
    {
        errorMessage = "token-payload broken";
        status = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RESPONE;
        return false;
    }

    Kitsunemimi::DataMap* payloadContent = jsonItem.getItemContent()->toMap();
    std::vector<std::string> groups;
    Kitsunemimi::splitStringByDelimiter(groups,
                                        payloadContent->get("groups")->toValue()->getString(),
                                        ',');

    bool foundPolicy = false;
    for(const std::string &group : groups)
    {
        if(MisakaRoot::policies->checkUserAgainstPolicy(component, endpoint, httpType, group)) {
            foundPolicy = true;
        }
    }

    return foundPolicy;
}
