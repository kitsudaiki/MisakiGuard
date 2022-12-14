/**
 * @file        users_database.h
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

#ifndef MISAKIGUARD_USERS_TABLE_H
#define MISAKIGUARD_USERS_TABLE_H

#include <libKitsunemimiCommon/logger.h>
#include <libKitsunemimiHanamiDatabase/hanami_sql_admin_table.h>

namespace Kitsunemimi {
class JsonItem;
}
class UsersTable
        : public Kitsunemimi::Hanami::HanamiSqlAdminTable
{
public:
    UsersTable(Kitsunemimi::Sakura::SqlDatabase* db);
    ~UsersTable();

    bool initNewAdminUser(Kitsunemimi::ErrorContainer &error);

    bool addUser(Kitsunemimi::JsonItem &userData,
                 Kitsunemimi::ErrorContainer &error);
    bool getUser(Kitsunemimi::JsonItem &result,
                 const std::string &userId,
                 Kitsunemimi::ErrorContainer &error,
                 const bool showHiddenValues);
    bool getAllUser(Kitsunemimi::TableItem &result,
                    Kitsunemimi::ErrorContainer &error);
    bool deleteUser(const std::string &userId,
                    Kitsunemimi::ErrorContainer &error);
    bool updateProjectsOfUser(const std::string &userId,
                              const std::string &newProjects,
                              Kitsunemimi::ErrorContainer &error);

private:
    bool getEnvVar(std::string &content, const std::string &key) const;

    bool getAllAdminUser(Kitsunemimi::ErrorContainer &error);
};

#endif // MISAKIGUARD_USERS_TABLE_H
