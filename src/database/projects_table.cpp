/**
 * @file        projects_table.cpp
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

#include <database/projects_table.h>

#include <libKitsunemimiCommon/items/table_item.h>
#include <libKitsunemimiCommon/methods/string_methods.h>
#include <libKitsunemimiJson/json_item.h>
#include <libKitsunemimiCrypto/hashes.h>

#include <libKitsunemimiSakuraDatabase/sql_database.h>

/**
 * @brief constructor
 */
ProjectsTable::ProjectsTable(Kitsunemimi::Sakura::SqlDatabase* db)
    : HanamiSqlTable(db)
{
    m_tableName = "projects";

    DbHeaderEntry userName;
    userName.name = "name";
    userName.maxLength = 256;
    m_tableHeader.push_back(userName);
}

/**
 * @brief destructor
 */
ProjectsTable::~ProjectsTable() {}

/**
 * @brief add a new project to the database
 *
 * @param userData json-item with all information of the project to add to database
 * @param error reference for error-output
 *
 * @return true, if successful, else false
 */
bool
ProjectsTable::addProject(Kitsunemimi::Json::JsonItem &userData,
                          Kitsunemimi::ErrorContainer &error)
{
    if(add(userData, "-", "-", error) == false)
    {
        error.addMeesage("Failed to add user to database");
        return false;
    }

    return true;
}

/**
 * @brief get a project from the database by his name
 *
 * @param result reference for the result-output in case that a project with this name was found
 * @param projectName name of the requested project
 * @param error reference for error-output
 * @param showHiddenValues set to true to also show as hidden marked fields
 *
 * @return true, if successful, else false
 */
bool
ProjectsTable::getProjectByName(Kitsunemimi::Json::JsonItem &result,
                                const std::string &projectName,
                                Kitsunemimi::ErrorContainer &error,
                                const bool showHiddenValues)
{
    std::vector<RequestCondition> conditions;
    conditions.emplace_back("name", projectName);

    // get user from db
    if(get(result, "-", "-", true, conditions, error, showHiddenValues) == false)
    {
        error.addMeesage("Failed to get user with name '"
                         + projectName
                         + "' from database");
        LOG_ERROR(error);
        return false;
    }

    return true;
}

/**
 * @brief get all projects from the database table
 *
 * @param result reference for the result-output
 * @param error reference for error-output
 *
 * @return true, if successful, else false
 */
bool
ProjectsTable::getAllProjects(Kitsunemimi::TableItem &result,
                              Kitsunemimi::ErrorContainer &error)
{
    std::vector<RequestCondition> conditions;
    if(getAll(result, "-", "-", true, conditions, error) == false)
    {
        error.addMeesage("Failed to get all users from database");
        return false;
    }

    return true;
}

/**
 * @brief delete a project from the table
 *
 * @param projectName name of the project to delete
 * @param error reference for error-output
 *
 * @return true, if successful, else false
 */
bool
ProjectsTable::deleteProject(const std::string &projectName,
                             Kitsunemimi::ErrorContainer &error)
{
    std::vector<RequestCondition> conditions;
    conditions.emplace_back("name", projectName);

    if(del(conditions, "-", "-", true, error) == false)
    {
        error.addMeesage("Failed to delete user with name '"
                         + projectName
                         + "' from database");
        return false;
    }

    return true;
}
