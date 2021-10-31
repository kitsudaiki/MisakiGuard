/**
 * @file        misaka_root.cpp
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

#include "misaka_root.h"

#include <libKitsunemimiConfig/config_handler.h>

Kitsunemimi::Jwt::Jwt* MisakaRoot::jwt = nullptr;
UsersDatabase* MisakaRoot::usersDb = nullptr;

MisakaRoot::MisakaRoot()
{
    usersDb = new UsersDatabase();

    bool success = false;
    const std::string tokenKeyString = GET_STRING_CONFIG("Misaka", "token_key", success);
    CryptoPP::SecByteBlock tokenKey((unsigned char*)tokenKeyString.c_str(), tokenKeyString.size());
    jwt = new Kitsunemimi::Jwt::Jwt(tokenKey);
}
