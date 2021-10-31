/**
 * @file        create_token.cpp
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

#include "create_token.h"

#include <misaka_root.h>

using namespace Kitsunemimi::Sakura;

CreateToken::CreateToken()
    : Blossom()
{
    registerField("user_name", INPUT_TYPE, true);
    registerField("pw", INPUT_TYPE, true);
    registerField("token", OUTPUT_TYPE, true);
}

bool
CreateToken::runTask(BlossomLeaf &blossomLeaf,
                     std::string &errorMessage)
{
    std::string result = "";
}
