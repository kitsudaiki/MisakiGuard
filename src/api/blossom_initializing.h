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

#include <libKitsunemimiHanamiEndpoints/endpoint.h>

#include <api/user/create_user.h>
#include <api/user/get_user.h>
#include <api/user/list_users.h>

#include  <api/documentation/generate_rest_api_docu.h>

#include <api/auth/create_internal_token.h>
#include <api/auth/create_token.h>
#include <api/auth/validate_access.h>

using Kitsunemimi::Sakura::SakuraLangInterface;

/**
 * @brief init special blossoms
 */
void
tokenBlossomes()
{
    Kitsunemimi::Hanami::Endpoint* endpoints = Kitsunemimi::Hanami::Endpoint::getInstance();
    SakuraLangInterface* interface = SakuraLangInterface::getInstance();
    const std::string group = "token";

    assert(interface->addBlossom(group, "create", new CreateToken()));
    endpoints->addEndpoint("token",
                           Kitsunemimi::Hanami::GET_TYPE,
                           Kitsunemimi::Hanami::BLOSSOM_TYPE,
                           group,
                           "create");

    assert(interface->addBlossom(group, "create_internal", new CreateInternalToken()));
    endpoints->addEndpoint("token/internal",
                           Kitsunemimi::Hanami::GET_TYPE,
                           Kitsunemimi::Hanami::BLOSSOM_TYPE,
                           group,
                           "create_internal");

    assert(interface->addBlossom(group, "validate", new ValidateAccess()));
    endpoints->addEndpoint("auth",
                           Kitsunemimi::Hanami::GET_TYPE,
                           Kitsunemimi::Hanami::BLOSSOM_TYPE,
                           group,
                           "validate");
}

/**
 * @brief documentationBlossomes
 */
void
documentationBlossomes()
{
    Kitsunemimi::Hanami::Endpoint* endpoints = Kitsunemimi::Hanami::Endpoint::getInstance();
    SakuraLangInterface* interface = SakuraLangInterface::getInstance();
    const std::string group = "documentation";

    assert(interface->addBlossom(group, "generate_rest_api", new GenerateRestApiDocu()));
    endpoints->addEndpoint("documentation/api/rest",
                           Kitsunemimi::Hanami::GET_TYPE,
                           Kitsunemimi::Hanami::BLOSSOM_TYPE,
                           group,
                           "generate_rest_api");
}

/**
 * @brief init special blossoms
 */
void
userBlossomes()
{
    Kitsunemimi::Hanami::Endpoint* endpoints = Kitsunemimi::Hanami::Endpoint::getInstance();
    SakuraLangInterface* interface = SakuraLangInterface::getInstance();
    const std::string group = "user";

    assert(interface->addBlossom(group, "create", new CreateUser()));
    endpoints->addEndpoint("user",
                           Kitsunemimi::Hanami::POST_TYPE,
                           Kitsunemimi::Hanami::BLOSSOM_TYPE,
                           group,
                           "create");

    assert(interface->addBlossom(group, "get", new GetUser()));
    endpoints->addEndpoint("user",
                           Kitsunemimi::Hanami::GET_TYPE,
                           Kitsunemimi::Hanami::BLOSSOM_TYPE,
                           group,
                           "get");

    assert(interface->addBlossom(group, "list", new ListUsers()));
    endpoints->addEndpoint("user/all",
                           Kitsunemimi::Hanami::GET_TYPE,
                           Kitsunemimi::Hanami::BLOSSOM_TYPE,
                           group,
                           "list");

    assert(interface->addBlossom(group, "delete", new ListUsers()));
    endpoints->addEndpoint("user",
                           Kitsunemimi::Hanami::DELETE_TYPE,
                           Kitsunemimi::Hanami::BLOSSOM_TYPE,
                           group,
                           "delete");
}

void
initBlossoms()
{
    userBlossomes();
    documentationBlossomes();
    tokenBlossomes();
}

#endif // MISAKAGUARD_BLOSSOM_INITIALIZING_H
