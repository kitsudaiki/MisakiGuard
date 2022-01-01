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

/**
 * @brief constructor
 */
UsersTable::UsersTable(Kitsunemimi::Sakura::SqlDatabase* db)
    : HanamiSqlTable(db)
{
    m_tableName = "users";

    DbHeaderEntry userName;
    userName.name = "name";
    userName.maxLength = 256;
    m_tableHeader.push_back(userName);

    DbHeaderEntry roles;
    roles.name = "roles";
    m_tableHeader.push_back(roles);

    DbHeaderEntry projects;
    projects.name = "projects";
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
 * @brief add a new user to the database
 *
 * @param userData json-item with all information of the user to add to database
 * @param error reference for error-output
 *
 * @return true, if successfull, else false
 */
bool
UsersTable::addUser(Kitsunemimi::Json::JsonItem &userData,
                    Kitsunemimi::ErrorContainer &error)
{
    return add(userData, error);
}

/**
 * @brief get a user from the database by his name
 *
 * @param result reference for the result-output in case that a user with this name was found
 * @param userName name of the requested user
 * @param error reference for error-output
 * @param showHiddenValues set to true to also show as hidden marked fields
 *
 * @return true, if successfull, else false
 */
bool
UsersTable::getUserByName(Kitsunemimi::Json::JsonItem &result,
                          const std::string &userName,
                          Kitsunemimi::ErrorContainer &error,
                          const bool showHiddenValues)
{
    std::vector<RequestCondition> conditions;
    conditions.emplace_back("name", userName);

    // get user from db
    if(get(result, conditions, error, showHiddenValues) == false)
    {
        LOG_ERROR(error);
        return false;
    }

    return true;
}

/**
 * @brief get all users from the database table
 *
 * @param result reference for the result-output
 * @param error reference for error-output
 *
 * @return true, if successfull, else false
 */
bool
UsersTable::getAllUser(Kitsunemimi::TableItem &result,
                       Kitsunemimi::ErrorContainer &error)
{
    return getAll(result, error);
}

/**
 * @brief delete a user from the table
 *
 * @param userName name of the user to delete
 * @param error reference for error-output
 *
 * @return true, if successfull, else false
 */
bool
UsersTable::deleteUser(const std::string &userName,
                       Kitsunemimi::ErrorContainer &error)
{
    std::vector<RequestCondition> conditions;
    conditions.emplace_back("name", userName);

    return deleteFromDb(conditions, error);
}
