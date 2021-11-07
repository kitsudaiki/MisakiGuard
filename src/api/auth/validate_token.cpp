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
#include <libKitsunemimiJwt/jwt.h>
#include <libKitsunemimiHanamiCommon/enums.h>
#include <misaka_root.h>
#include <libKitsunemimiJson/json_item.h>

using namespace Kitsunemimi::Sakura;

ValidateToken::ValidateToken()
    : Blossom()
{
    registerField("token", INPUT_TYPE, true);

    registerField("uuid", OUTPUT_TYPE, false);
    registerField("user_name", OUTPUT_TYPE, false);
    registerField("pw_hast", OUTPUT_TYPE, false);
    registerField("is_admin", OUTPUT_TYPE, false);
}

bool
ValidateToken::runTask(BlossomLeaf &blossomLeaf,
                       uint64_t &status,
                       std::string &errorMessage)
{
    const std::string token = blossomLeaf.input.getStringByKey("token");
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

    blossomLeaf.output = *(jsonItem.getItemContent()->toMap());

    return true;
}
