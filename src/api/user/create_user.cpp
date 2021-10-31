/**
 * @file        create_token.h
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

#include "create_user.h"

#include <misaka_root.h>
#include <libKitsunemimiHanamiCommon/uuid.h>
#include <libKitsunemimiCrypto/hashes.h>

using namespace Kitsunemimi::Sakura;

CreateUser::CreateUser()
    : Blossom()
{
    registerField("user_name", INPUT_TYPE, true);
    registerField("pw", INPUT_TYPE, true);
    registerField("successfull", OUTPUT_TYPE, true);
}

bool
CreateUser::runTask(BlossomLeaf &blossomLeaf,
                    std::string &errorMessage)
{
    Kitsunemimi::ErrorContainer error;
    UsersDatabase::UserData userData;

    userData.userId = Kitsunemimi::Hanami::generateUuid().toString();
    userData.userName = blossomLeaf.input.getStringByKey("user_name");
    Kitsunemimi::Crypto::generate_SHA_256(userData.pwHash, blossomLeaf.input.getStringByKey("pw"));

    const bool ret = MisakaRoot::usersDb->addUser(userData, error);
    errorMessage = error.errorMessage;

    blossomLeaf.output.insert("successfull", new Kitsunemimi::DataValue(ret));

    return ret;
}
