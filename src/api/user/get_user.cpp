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

using namespace Kitsunemimi::Sakura;

GetUser::GetUser()
    : Blossom()
{
    registerField("user_name", INPUT_TYPE, false);
    registerField("result", OUTPUT_TYPE, true);
}

bool
GetUser::runTask(BlossomLeaf &blossomLeaf,
                 std::string &errorMessage)
{
    Kitsunemimi::ErrorContainer error;
    const std::string userName = blossomLeaf.input.getStringByKey("user_name");
    blossomLeaf.output.insert("result", MisakaRoot::usersDb->getUser(userName, error));
    errorMessage = error.errorMessage;

    return true;
}
