/**
 * @file        validate_access.h
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

#ifndef MISAKAGUARD_VALIDATE_ACCESS_H
#define MISAKAGUARD_VALIDATE_ACCESS_H

#include <libKitsunemimiSakuraLang/blossom.h>
#include <libKitsunemimiHanamiCommon/enums.h>

class ValidateAccess
        : public Kitsunemimi::Sakura::Blossom
{
public:
    ValidateAccess();

protected:
    bool runTask(Kitsunemimi::Sakura::BlossomLeaf &blossomLeaf,
                 const Kitsunemimi::DataMap &,
                 Kitsunemimi::Sakura::BlossomStatus &status,
                 Kitsunemimi::ErrorContainer &error);

private:
    void sendAuditMessage(const std::string &targetComponent,
                          const std::string &targetEndpoint,
                          const std::string &userUuid,
                          const Kitsunemimi::Hanami::HttpRequestType requestType);
};

#endif // MISAKAGUARD_VALIDATE_ACCESS_H
