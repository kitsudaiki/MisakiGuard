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

#ifndef USERS_TABLE_H
#define USERS_TABLE_H

#include <libKitsunemimiCommon/logger.h>
#include <libKitsunemimiSakuraDatabase/sql_table.h>

class UsersTable
        : public Kitsunemimi::Sakura::SqlTable
{
public:
    struct UserData
    {
        std::string uuid = "";
        std::string name = "";
        std::string pwHash = "";
        bool isAdmin = false;
        std::vector<std::string> groups;
    };

    UsersTable(Kitsunemimi::Sakura::SqlDatabase* db);
    ~UsersTable();

    const std::string addUser(const UserData &data,
                              Kitsunemimi::ErrorContainer &error);
    bool getUserByName(UserData &result,
                       Kitsunemimi::TableItem &tableContent,
                       const std::string &userName,
                       Kitsunemimi::ErrorContainer &error);
    bool getUser(UserData &result,
                 Kitsunemimi::TableItem &tableContent,
                 const std::string &uuid,
                 Kitsunemimi::ErrorContainer &error);
    bool getUser(Kitsunemimi::TableItem &result,
                 const std::string &uuid,
                 Kitsunemimi::ErrorContainer &error);
    bool getAllUser(Kitsunemimi::TableItem &result,
                    Kitsunemimi::ErrorContainer &error);
    bool deleteUser(const std::string &userID,
                     Kitsunemimi::ErrorContainer &error);

private:
    void processGetResult(UserData &result,
                          Kitsunemimi::TableItem &tableContent);
};

#endif // USERS_TABLE_H
