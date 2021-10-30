#ifndef USERS_DATABASE_H
#define USERS_DATABASE_H

#include <mutex>
#include <libKitsunemimiSqlite/sqlite.h>
#include <libKitsunemimiSakuraDatabase/sql_database.h>

namespace Kitsunemimi
{
namespace Sqlite {
class Sqlite;
}
namespace Hanami
{

class UsersDatabase :
        Kitsunemimi::Sakura::SqlDatabase
{
public:
    struct UserData
    {
        std::string userId = "";
        std::string pwHash = "";
        bool isAdmin = false;
    };

    UsersDatabase();
    ~UsersDatabase();

    bool initDatabase(const std::string &path,
                      std::string &errorMessag);
    bool closeDatabase();

    bool addUser(const UserData &data, std::string &errorMessage);
    bool getUser(UserData &data, const std::string &userID, std::string &errorMessage);


private:
    std::mutex m_lock;
    bool m_isOpen = false;
    std::string m_path = "";

    Kitsunemimi::Sqlite::Sqlite m_db;

    bool initUserTable(std::string &errorMessage);
};

}
}

#endif // USERS_DATABASE_H
