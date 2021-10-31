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

#include <users/users_database.h>

#include <libKitsunemimiCommon/common_items/table_item.h>

UsersDatabase::UsersDatabase()
    : SqlDatabase()
{
    m_tableName = "users";

    DbHeaderEntry userId;
    userId.name = "user_id";
    userId.maxLength = 256;
    userId.isPrimary = true;
    m_tableHeader.push_back(userId);

    DbHeaderEntry userName;
    userName.name = "user_id";
    userName.maxLength = 256;
    m_tableHeader.push_back(userName);

    DbHeaderEntry pwHash;
    pwHash.name = "pw_hash";
    pwHash.maxLength = 64;
    m_tableHeader.push_back(pwHash);

    DbHeaderEntry isAdmin;
    isAdmin.name = "is_admin";
    isAdmin.type = BOOL_TYPE;
    m_tableHeader.push_back(isAdmin);
}

UsersDatabase::~UsersDatabase()
{
    closeDatabase();
}

/**
 * @brief Users::initDatabase
 * @param path
 * @param errorMessag
 * @return
 */
bool
UsersDatabase::initDatabase(const std::string &path,
                            Kitsunemimi::ErrorContainer &error)
{
    std::lock_guard<std::mutex> guard(m_lock);
    if(m_isOpen) {
        return false;
    }

    if(m_db.initDB(path, error))
    {
        m_isOpen = true;
        m_path = path;

        return initUserTable(error);
    }

    return false;
}

/**
 * @brief Users::closeDatabase
 * @return
 */
bool
UsersDatabase::closeDatabase()
{
    std::lock_guard<std::mutex> guard(m_lock);
    if(m_isOpen == false) {
        return true;
    }

    if(m_db.closeDB())
    {
        m_isOpen = false;
        return true;
    }

    return false;
}

/**
 * @brief Users::addUser
 * @param data
 * @param errorMessage
 * @return
 */
bool
UsersDatabase::addUser(const UserData &data,
                       Kitsunemimi::ErrorContainer &error)
{
    std::lock_guard<std::mutex> guard(m_lock);
    if(m_isOpen == false)
    {
        error.errorMessage = "database not open";
        LOG_ERROR(error);
        return false;
    }

    const std::vector<std::string> values = { data.userId,
                                              data.pwHash,
                                              std::to_string(data.isAdmin)};
    Kitsunemimi::TableItem resultItem;
    const bool ret = m_db.execSqlCommand(&resultItem, createInsertQuery(values), error);
    if(ret == false)
    {
        LOG_ERROR(error);
        return false;
    }

    return true;
}

/**
 * @brief UsersDatabase::getUser
 * @param userID
 * @param error
 * @return
 */
Kitsunemimi::DataItem*
UsersDatabase::getUser(const std::string &userID,
                       Kitsunemimi::ErrorContainer &error)
{
    std::lock_guard<std::mutex> guard(m_lock);

    // check db-state
    if(m_isOpen == false)
    {
        error.errorMessage = "database not open";
        LOG_ERROR(error);
        return new Kitsunemimi::DataArray();
    }

    // get user from db
    Kitsunemimi::TableItem resultItem;
    const bool ret = m_db.execSqlCommand(&resultItem, createSelectQuery("user_id", userID), error);
    if(ret == false) {
        return new Kitsunemimi::DataArray();
    }

    if(resultItem.getNumberOfRows() == 0)
    {
        error.errorMessage = "User with ID '" + userID + "' not found;";
        LOG_ERROR(error);
        return new Kitsunemimi::DataArray();
    }

    return resultItem.getBody();
}

/**
 * @brief UsersDatabase::getAllUser
 * @param error
 * @return
 */
Kitsunemimi::DataItem*
UsersDatabase::getAllUser(Kitsunemimi::ErrorContainer &error)
{
    return getUser("", error);
}

/**
 * @brief Users::initUserTable
 * @param errorMessage
 * @return
 */
bool
UsersDatabase::initUserTable(Kitsunemimi::ErrorContainer &error)
{
    return m_db.execSqlCommand(nullptr, createTableCreateQuery(), error);
}
