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

using namespace Kitsunemimi::Sakura;

ValidateToken::ValidateToken()
    : Blossom()
{
    registerField("token", INPUT_TYPE, true);
    registerField("is_valid", OUTPUT_TYPE, true);
}

bool
ValidateToken::runTask(BlossomLeaf &blossomLeaf,
                       std::string &errorMessage)
{

}