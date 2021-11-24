/**
 * @file        blossom_initializing.h
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

#ifndef MISAKAGUARD_BLOSSOM_INITIALIZING_H
#define MISAKAGUARD_BLOSSOM_INITIALIZING_H

#include <libKitsunemimiSakuraLang/sakura_lang_interface.h>
#include <libKitsunemimiCommon/logger.h>

#include <api/user/create_user.h>
#include <api/user/get_user.h>
#include <api/user/list_users.h>

#include <api/auth/create_token.h>
#include <api/auth/validate_access.h>

using Kitsunemimi::Sakura::SakuraLangInterface;

/**
 * @brief init special blossoms
 */
void
tokenBlossomes()
{
    SakuraLangInterface* interface = SakuraLangInterface::getInstance();
    const std::string group = "-";

    assert(interface->addBlossom(group, "create_token", new CreateToken()));
    assert(interface->addBlossom(group, "validate_access", new ValidateAccess()));
}

/**
 * @brief init special blossoms
 */
void
userBlossomes()
{
    SakuraLangInterface* interface = SakuraLangInterface::getInstance();
    const std::string group = "-";

    assert(interface->addBlossom(group, "create_user", new CreateUser()));
    assert(interface->addBlossom(group, "get_user", new GetUser()));
    assert(interface->addBlossom(group, "list_users", new ListUsers()));
}

void
initBlossoms()
{
    userBlossomes();
    tokenBlossomes();
}

#endif // MISAKAGUARD_BLOSSOM_INITIALIZING_H
