/**
 * @file        main.cpp
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

#include <iostream>
#include <thread>

#include <misaka_root.h>
#include <args.h>
#include <config.h>

#include <libKitsunemimiHanamiCommon/generic_main.h>
#include <libKitsunemimiHanamiMessaging/hanami_messaging.h>


using Kitsunemimi::Hanami::HanamiMessaging;
using Kitsunemimi::Hanami::initMain;

int main(int argc, char *argv[])
{
    if(initMain(argc, argv, "MisakaGuard", &registerArguments, &registerConfigs) == false) {
        return 1;
    }

    // initialize server and connections based on the config-file
    const std::vector<std::string> groupNames = {};
    const bool ret = HanamiMessaging::getInstance()->initialize("Misaka", groupNames, true);
    if(ret == false) {
        return 1;
    }

    MisakaRoot rootObj;

    // sleep forever
    std::this_thread::sleep_until(std::chrono::time_point<std::chrono::system_clock>::max());

    return 0;
}
