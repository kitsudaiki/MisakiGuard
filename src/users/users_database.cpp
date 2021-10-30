#include <users/users_database.h>

#include <libKitsunemimiCommon/common_items/table_item.h>

namespace Kitsunemimi
{
namespace Hanami
{

UsersDatabase::UsersDatabase()
    : SqlDatabase()
{
    m_tableName = "users";

    DbHeaderEntry userId;
    userId.name = "user_id";
    userId.maxLength = 256;
    userId.isPrimary = true;
    m_tableHeader.push_back(userId);

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
                    std::string &errorMessag)
{
    std::lock_guard<std::mutex> guard(m_lock);
    if(m_isOpen) {
        return false;
    }

    if(m_db.initDB(path, errorMessag))
    {
        m_isOpen = true;
        m_path = path;

        return initUserTable(errorMessag);
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
               std::string &errorMessage)
{
    std::lock_guard<std::mutex> guard(m_lock);
    if(m_isOpen == false)
    {
        errorMessage = "database not open";
        return false;
    }

    const std::vector<std::string> values = { data.userId,
                                              data.pwHash,
                                              std::to_string(data.isAdmin)};
    Kitsunemimi::TableItem resultItem;
    const bool ret = m_db.execSqlCommand(&resultItem, createInsertQuery(values), errorMessage);
    if(ret == false) {
        return false;
    }

    return true;
}

/**
 * @brief Users::getUser
 * @param data
 * @param userID
 * @param errorMessage
 * @return
 */
bool
UsersDatabase::getUser(UserData &data,
               const std::string &userID,
               std::string &errorMessage)
{
    std::lock_guard<std::mutex> guard(m_lock);
    if(m_isOpen == false)
    {
        errorMessage = "database not open";
        return false;
    }

    Kitsunemimi::TableItem resultItem;
    const bool ret = m_db.execSqlCommand(&resultItem, createSelectQuery(userID), errorMessage);
    if(ret == false) {
        return false;
    }

    if(resultItem.getNumberOfRows() == 0)
    {
        errorMessage = "User with ID '" + userID + "' not found;";
        return false;
    }

    data.userId =  resultItem.getCell(0, 0);
    data.pwHash =  resultItem.getCell(1, 0);
    data.isAdmin = resultItem.getCell(2, 0) == "1";

    return true;
}

/**
 * @brief Users::initUserTable
 * @param errorMessage
 * @return
 */
bool
UsersDatabase::initUserTable(std::string &errorMessage)
{
    return m_db.execSqlCommand(nullptr, createTableCreateQuery(), errorMessage);
}

}
}
