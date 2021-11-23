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
#include <libKitsunemimiSakuraDatabase/sql_database.h>

UsersTable::UsersTable(Kitsunemimi::Sakura::SqlDatabase* db)
    : SqlTable(db)
{
    m_tableName = "users";

    DbHeaderEntry userName;
    userName.name = "name";
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

    DbHeaderEntry groups;
    groups.name = "groups";
    m_tableHeader.push_back(groups);
}

UsersTable::~UsersTable() {}

/**
 * @brief Users::addUser
 * @param data
 * @param errorMessage
 * @return
 */
const std::string
UsersTable::addUser(const UserData &data,
                    Kitsunemimi::ErrorContainer &error)
{
    std::string groupList = "";
    for(uint64_t i = 0; i < data.groups.size(); i++)
    {
        if(i != 0) {
            groupList.append(",");
        }
        groupList.append(data.groups.at(i));
    }

    const std::vector<std::string> values = { data.name,
                                              data.pwHash,
                                              std::to_string(data.isAdmin),
                                              groupList};
    return insertToDb(values, error);
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
UsersTable::getUserByName(UserData &result,
                          Kitsunemimi::TableItem &tableContent,
                          const std::string &userName,
                          Kitsunemimi::ErrorContainer &error)
{
    std::vector<RequestCondition> conditions;
    conditions.emplace_back("name", userName);

    // get user from db
    if(getFromDb(&tableContent, conditions, error) == false) {
        return false;
    }

    // check response
    if(tableContent.getNumberOfRows() == 0)
    {
        error.addMeesage("User with name '" + userName + "' not found;");
        LOG_ERROR(error);
        return false;
    }

    // prepare result
    processGetResult(result, tableContent);

    return true;
}

/**
 * @brief UsersTable::getUser
 * @param result
 * @param uuid
 * @param error
 * @return
 */
bool
UsersTable::getUser(UserData &result,
                    Kitsunemimi::TableItem &tableContent,
                    const std::string &uuid,
                    Kitsunemimi::ErrorContainer &error)
{
    if(getUser(tableContent, uuid, error) == false) {
        return false;
    }

    processGetResult(result, tableContent);

    return true;
}

/**
 * @brief UsersDatabase::getUser
 * @param userID
 * @param error
 * @return
 */
bool
UsersTable::getUser(Kitsunemimi::TableItem &result,
                    const std::string &uuid,
                    Kitsunemimi::ErrorContainer &error)
{
    std::vector<RequestCondition> conditions;
    conditions.emplace_back("uuid", uuid);

    // get user from db
    if(getFromDb(&result, conditions, error) == false)
    {
        LOG_ERROR(error);
        return false;
    }

    if(result.getNumberOfRows() == 0)
    {
        error.addMeesage("User with ID '" + uuid + "' not found;");
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
    return getAllFromDb(&result, error);
}

/**
 * @brief UsersDatabase::deleteUser
 * @param userID
 * @param error
 * @return
 */
bool
UsersTable::deleteUser(const std::string &userID,
                       Kitsunemimi::ErrorContainer &error)
{
    std::vector<RequestCondition> conditions;
    conditions.emplace_back("uuid", userID);

    return deleteFromDb(conditions, error);
}

/**
 * @brief UsersTable::processGetResult
 * @param result
 * @param tableContent
 */
void
UsersTable::processGetResult(UserData &result,
                             Kitsunemimi::TableItem &tableContent)
{
    // prepare result
    const Kitsunemimi::DataItem* firstRow = tableContent.getBody()->get(0);
    result.uuid = firstRow->get("uuid")->toValue()->getString();
    result.name = firstRow->get("name")->toValue()->getString();
    result.pwHash = firstRow->get("pw_hash")->toValue()->getString();
    result.isAdmin = firstRow->get("is_admin")->toValue()->getBool();

    if(result.groups.size() > 0) {
        result.groups.clear();
    }
    Kitsunemimi::splitStringByDelimiter(result.groups,
                                        firstRow->get("groups")->toValue()->getString(),
                                        ',');

}
