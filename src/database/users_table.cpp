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
#include <libKitsunemimiCrypto/hashes.h>

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

/**
 * @brief destructor
 */
UsersTable::~UsersTable() {}

/**
 * @brief get content of an environment-variable
 *
 * @param content reference for output
 * @param key name of the environment-variable
 *
 * @return false, if varibale is not set, else true
 */
bool
UsersTable::getEnvVar(std::string &content,
                      const std::string &key) const
{
    const char* val = getenv(key.c_str());
    if(val == NULL) {
        return false;
    }

    content = std::string(val);
    return true;
}

/**
 * @brief get list of all users, who have admin-status
 *
 * @param error reference for error-output
 *
 * @return true, if seccuessful, else false
 */
bool
UsersTable::getAllAdminUser(Kitsunemimi::ErrorContainer &error)
{
    std::vector<RequestCondition> conditions;
    conditions.emplace_back("is_admin", "true");

    // get admin-user from db
    Kitsunemimi::Json::JsonItem users;
    if(get(users, "-", "-", true, conditions, error, false) == false)
    {
        error.addMeesage("Failed to get admin-users from database");
        LOG_ERROR(error);
        return false;
    }

    return true;
}

/**
 * @brief try to initialize a new admin-user in database
 *
 * @param error reference for error-output
 *
 * @return true, if seccuessful, else false
 */
bool
UsersTable::initNewAdminUser(Kitsunemimi::ErrorContainer &error)
{
    std::string userName = "";
    std::string pw = "";
    std::string role = "";
    const std::string uuid = "e307bee0-9286-49bd-9273-6f644c12da1d";

    // check if there is already an admin-user in the databasae
    if(getAllAdminUser(error)) {
        return true;
    }
    LOG_DEBUG("Found no admin-users in database, so try to create a new one");

    // get env with new admin-user name
    if(getEnvVar(userName, "HANAMI_ADMIN_USER") == false)
    {
        error.addMeesage("environment variable 'HANAMI_ADMIN_USER' was not set, "
                         "but is required to initialize a new admin-user");
        LOG_ERROR(error);
        return false;
    }

    // get env with new admin-user password
    if(getEnvVar(pw, "HANAMI_ADMIN_PASSWORD") == false)
    {
        error.addMeesage("environment variable 'HANAMI_ADMIN_PASSWORD' was not set, "
                         "but is required to initialize a new admin-user");
        LOG_ERROR(error);
        return false;
    }

    // get env with new admin-user role
    if(getEnvVar(role, "HANAMI_ADMIN_ROLE") == false)
    {
        error.addMeesage("environment variable 'HANAMI_ADMIN_ROLE' was not set, "
                         "but is required to initialize a new admin-user");
        LOG_ERROR(error);
        return false;
    }

    // generate hash from password
    std::string pwHash;
    const std::string saltedPw = pw + uuid;
    Kitsunemimi::Crypto::generate_SHA_256(pwHash, saltedPw);

    Kitsunemimi::Json::JsonItem userData;
    userData.insert("uuid", uuid);
    userData.insert("name", userName);
    userData.insert("roles", role);
    userData.insert("projects", "-");
    userData.insert("pw_hash", pwHash);
    userData.insert("is_admin", true);
    userData.insert("project_uuid", "-");
    userData.insert("owner_uuid", "-");
    userData.insert("visibility", "private");

    // add new admin-user to db
    if(addUser(userData, "-", "-", error) == false)
    {
        error.addMeesage("Failed to add new initial admin-user to database");
        LOG_ERROR(error);
        return false;
    }

    return true;
}

/**
 * @brief add a new user to the database
 *
 * @param userData json-item with all information of the user to add to database
 * @param userUuid user-uuid to filter
 * @param projectUuid project-uuid to filter
 * @param error reference for error-output
 *
 * @return true, if successful, else false
 */
bool
UsersTable::addUser(Kitsunemimi::Json::JsonItem &userData,
                    const std::string &userUuid,
                    const std::string &projectUuid,
                    Kitsunemimi::ErrorContainer &error)
{
    if(add(userData, userUuid, projectUuid, error) == false)
    {
        error.addMeesage("Failed to add user to database");
        return false;
    }

    return true;
}

/**
 * @brief get a user from the database by his name
 *
 * @param result reference for the result-output in case that a user with this name was found
 * @param userName name of the requested user
 * @param userUuid user-uuid to filter
 * @param projectUuid project-uuid to filter
 * @param isAdmin true, if use who makes request is admin
 * @param error reference for error-output
 * @param showHiddenValues set to true to also show as hidden marked fields
 *
 * @return true, if successful, else false
 */
bool
UsersTable::getUserByName(Kitsunemimi::Json::JsonItem &result,
                          const std::string &userName,
                          const std::string &userUuid,
                          const std::string &projectUuid,
                          const bool isAdmin,
                          Kitsunemimi::ErrorContainer &error,
                          const bool showHiddenValues)
{
    std::vector<RequestCondition> conditions;
    conditions.emplace_back("name", userName);

    // get user from db
    if(get(result, userUuid, projectUuid, isAdmin, conditions, error, showHiddenValues) == false)
    {
        error.addMeesage("Failed to get user with name '"
                         + userName
                         + "' from database");
        LOG_ERROR(error);
        return false;
    }

    return true;
}

/**
 * @brief get all users from the database table
 *
 * @param result reference for the result-output
 * @param userUuid user-uuid to filter
 * @param projectUuid project-uuid to filter
 * @param isAdmin true, if use who makes request is admin
 * @param error reference for error-output
 *
 * @return true, if successful, else false
 */
bool
UsersTable::getAllUser(Kitsunemimi::TableItem &result,
                       const std::string &userUuid,
                       const std::string &projectUuid,
                       const bool isAdmin,
                       Kitsunemimi::ErrorContainer &error)
{
    if(getAll(result, userUuid, projectUuid, isAdmin, error) == false)
    {
        error.addMeesage("Failed to get all users from database");
        return false;
    }

    return true;
}

/**
 * @brief delete a user from the table
 *
 * @param userName name of the user to delete
 * @param userUuid user-uuid to filter
 * @param projectUuid project-uuid to filter
 * @param isAdmin true, if use who makes request is admin
 * @param error reference for error-output
 *
 * @return true, if successful, else false
 */
bool
UsersTable::deleteUser(const std::string &userName,
                       const std::string &userUuid,
                       const std::string &projectUuid,
                       const bool isAdmin,
                       Kitsunemimi::ErrorContainer &error)
{
    std::vector<RequestCondition> conditions;
    conditions.emplace_back("name", userName);

    if(del(conditions, userUuid, projectUuid, isAdmin, error) == false)
    {
        error.addMeesage("Failed to delete user with name '"
                         + userName
                         + "' from database");
        return false;
    }

    return true;
}
