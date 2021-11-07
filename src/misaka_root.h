/**
 * @file        misaka_root.h
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

#ifndef MISAKAROOT_H
#define MISAKAROOT_H

#include <libKitsunemimiJwt/jwt.h>
#include <libKitsunemimiHanamiPolicies/policy.h>
#include <users/users_table.h>

class MisakaRoot
{
public:
    MisakaRoot();

    static Kitsunemimi::Jwt::Jwt* jwt;
    static UsersTable* usersTable;
    static Kitsunemimi::Sakura::SqlDatabase* database;
    static Kitsunemimi::Hanami::Policy* policies;
};

#endif // MISAKAROOT_H
