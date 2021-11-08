/**
 * @file        validate_access.h
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

#ifndef VALIDATE_ACCESS_H
#define VALIDATE_ACCESS_H

#include <libKitsunemimiSakuraLang/blossom.h>

class ValidateAccess
        : public Kitsunemimi::Sakura::Blossom
{
public:
    ValidateAccess();

protected:
    bool runTask(Kitsunemimi::Sakura::BlossomLeaf &blossomLeaf,
                 uint64_t &status,
                 std::string &errorMessage);
};

#endif // VALIDATE_ACCESS_H
