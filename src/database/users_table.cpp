/**
 * @file        users_database.cpp
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

#include <database/users_table.h>

#include <libKitsunemimiCommon/common_items/table_item.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiJson/json_item.h>

#include <libKitsunemimiSakuraDatabase/sql_database.h>

UsersTable::UsersTable(Kitsunemimi::Sakura::SqlDatabase* db)
    : HanamiSqlTable(db)
{
    m_tableName = "users";

    DbHeaderEntry userName;
    userName.name = "user_name";
    userName.maxLength = 256;
    m_tableHeader.push_back(userName);

    DbHeaderEntry roles;
    roles.name = "user_roles";
    m_tableHeader.push_back(roles);

    DbHeaderEntry projects;
    projects.name = "user_projects";
    m_tableHeader.push_back(projects);

    DbHeaderEntry pwHash;
    pwHash.name = "pw_hash";
    pwHash.maxLength = 64;
    pwHash.hide = true;
    m_tableHeader.push_back(pwHash);

    DbHeaderEntry isAdmin;
    isAdmin.name = "is_admin";
    isAdmin.type = BOOL_TYPE;
    m_tableHeader.push_back(isAdmin);
}

UsersTable::~UsersTable() {}

/**
 * @brief Users::addUser
 * @param data
 * @param errorMessage
 * @return
 */
bool
UsersTable::addUser(Kitsunemimi::Json::JsonItem &userData,
                    Kitsunemimi::ErrorContainer &error)
{
    return add(userData, error);
}

/**
 * @brief UsersTable::getUserByName
 * @param result
 * @param tableContent
 * @param userName
 * @param error
 * @return
 */
bool
UsersTable::getUserByName(Kitsunemimi::Json::JsonItem &result,
                          const std::string &userName,
                          Kitsunemimi::ErrorContainer &error,
                          const bool showHiddenValues)
{
    std::vector<RequestCondition> conditions;
    conditions.emplace_back("user_name", userName);

    // get user from db
    if(get(result, conditions, error, showHiddenValues) == false)
    {
        LOG_ERROR(error);
        return false;
    }

    return true;
}

/**
 * @brief UsersDatabase::getAllUser
 * @param error
 * @return
 */
bool
UsersTable::getAllUser(Kitsunemimi::TableItem &result,
                       Kitsunemimi::ErrorContainer &error)
{
    return getAll(result, error);
}

/**
 * @brief UsersDatabase::deleteUser
 * @param userID
 * @param error
 * @return
 */
bool
UsersTable::deleteUser(const std::string &userName,
                       Kitsunemimi::ErrorContainer &error)
{
    std::vector<RequestCondition> conditions;
    conditions.emplace_back("user_name", userName);

    return deleteFromDb(conditions, error);
}
