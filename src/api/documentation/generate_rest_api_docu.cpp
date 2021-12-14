﻿/**
 * @file        generate_rest_api_docu.cpp
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

#include "generate_rest_api_docu.h"

#include <misaka_root.h>

#include <libKitsunemimiHanamiCommon/enums.h>
#include <libKitsunemimiHanamiCommon/uuid.h>
#include <libKitsunemimiHanamiCommon/component_support.h>
#include <libKitsunemimiHanamiMessaging/hanami_messaging.h>

#include <libKitsunemimiSakuraLang/sakura_lang_interface.h>

#include <libKitsunemimiCrypto/common.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiCommon/common_methods/file_methods.h>
#include <libKitsunemimiCommon/files/text_file.h>
#include <libKitsunemimiCommon/files/binary_file.h>
#include <libKitsunemimiCommon/process_execution.h>
#include <libKitsunemimiJson/json_item.h>

using namespace Kitsunemimi::Sakura;
using Kitsunemimi::Hanami::SupportedComponents;
using Kitsunemimi::Sakura::SakuraLangInterface;

/**
 * @brief constructor
 */
GenerateRestApiDocu::GenerateRestApiDocu()
    : Kitsunemimi::Sakura::Blossom("Generate a user-specific documentation for the REST-API "
                                   "of all available components.")
{
    // output
    registerOutputField("documentation",
                        SAKURA_STRING_TYPE,
                        "REST-API-documentation as base64 converted string.");
}

/**
 * @brief request another component for its documentation
 *
 * @param completeDocumentation reference for the final document to attach new content
 * @param component name of the requested component
 * @param request prebuild request-object
 * @param error reference for error-output
 *
 * @return
 */
bool
requestComponent(std::string &completeDocumentation,
                 const std::string &component,
                 const Kitsunemimi::Hanami::RequestMessage &request,
                 Kitsunemimi::ErrorContainer &error)
{
    Kitsunemimi::Hanami::HanamiMessaging* msg = Kitsunemimi::Hanami::HanamiMessaging::getInstance();
    Kitsunemimi::Hanami::ResponseMessage response;

    // send request to the target
    if(msg->triggerSakuraFile(component, response, request, error) == false)
    {
        LOG_ERROR(error);
        return false;
    }

    // check response
    if(response.success == false)
    {
        error.addMeesage(response.responseContent);
        LOG_ERROR(error);
        return false;
    }

    // parse result
    Kitsunemimi::Json::JsonItem jsonItem;
    if(jsonItem.parse(response.responseContent, error) == false)
    {
        LOG_ERROR(error);
        return false;
    }

    // get payload and convert it from base64 back to rst-file-format
    std::string rstDocu;
    const std::string componentDocu = jsonItem.get("documentation").getString();
    if(Kitsunemimi::Crypto::decodeBase64(rstDocu, componentDocu) == false)
    {
        error.addMeesage("Unable to convert documentation-payload from base64 back to rst");
        LOG_ERROR(error);
        return false;
    }

    // attach new text to the final document
    completeDocumentation.append("\n");
    completeDocumentation.append(rstDocu);

    return true;
}

/**
 * @brief request endpoint-documentation from misaka itself
 *
 * @param completeDocumentation reference for the final document to attach new content
 */
void
makeInternalRequest(std::string &completeDocumentation)
{
    const std::string* localComponent = &SupportedComponents::getInstance()->localComponent;

    std::string documentsion = "";
    documentsion.append(*localComponent);
    Kitsunemimi::toUpperCase(documentsion);
    documentsion.append("\n");
    documentsion.append(localComponent->size(), '=');
    documentsion.append("\n");

    Kitsunemimi::Hanami::HanamiMessaging::getInstance()->generateDocu(documentsion);

    completeDocumentation.append("\n");
    completeDocumentation.append(documentsion);
}

/**
 * @brief runTask
 */
bool
GenerateRestApiDocu::runTask(BlossomLeaf &blossomLeaf,
                             const Kitsunemimi::DataMap &context,
                             BlossomStatus &status,
                             Kitsunemimi::ErrorContainer &error)
{
    const std::string roles = context.getStringByKey("user_roles");

    // create request for remote-calls
    Kitsunemimi::Hanami::RequestMessage request;
    request.id = "documentation/api";
    request.httpType = Kitsunemimi::Hanami::GET_TYPE;
    request.inputValues = "{ \"token\" : \"" + context.getStringByKey("token") + "\"}";

    // create header of the final document
    std::string completeDocumentation = "";
    completeDocumentation.append("*****************\n");
    completeDocumentation.append("API documentation\n");
    completeDocumentation.append("*****************\n\n");

    SupportedComponents* scomp = SupportedComponents::getInstance();

    //----------------------------------------------------------------------------------------------
    makeInternalRequest(completeDocumentation);
    //----------------------------------------------------------------------------------------------
    if(scomp->support[Kitsunemimi::Hanami::KYOUKO]
            && requestComponent(completeDocumentation, "kyouko", request, error) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
    }
    //----------------------------------------------------------------------------------------------
    if(scomp->support[Kitsunemimi::Hanami::AZUKI]
            && requestComponent(completeDocumentation, "azuki", request, error) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
    }
    //----------------------------------------------------------------------------------------------
    if(scomp->support[Kitsunemimi::Hanami::SAGIRI]
            && requestComponent(completeDocumentation, "sagiri", request, error) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
    }
    //----------------------------------------------------------------------------------------------
    if(scomp->support[Kitsunemimi::Hanami::NAGATO]
            && requestComponent(completeDocumentation, "nagato", request, error) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
    }
    //----------------------------------------------------------------------------------------------
    if(scomp->support[Kitsunemimi::Hanami::IZUNA]
            && requestComponent(completeDocumentation, "izuna", request, error) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
    }
    //----------------------------------------------------------------------------------------------

    // create unique temporary directory
    const std::string uuid = Kitsunemimi::Hanami::generateUuid().toString();
    if(Kitsunemimi::createDirectory("/tmp/" + uuid, error) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        status.errorMessage = "directory";
        return false;
    }

    // define file-paths
    const std::string rstPath = "/tmp/" + uuid + "/rest_api_docu.rst";
    const std::string pdfPath = "/tmp/" + uuid + "/output.pdf";

    // write complete rst-content to the source-file
    if(Kitsunemimi::writeFile(rstPath, completeDocumentation, error) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        status.errorMessage = "files";
        return false;
    }

    // run rst2pdf to convert the rst-document into a pdf-document
    std::vector<std::string> args;
    args.reserve(2);
    args.emplace_back(rstPath);
    args.emplace_back(pdfPath);
    Kitsunemimi::runSyncProcess("rst2pdf", args);

    // read pdf-document into a byte-buffer
    Kitsunemimi::DataBuffer pdfContent;
    Kitsunemimi::BinaryFile pdfFile(pdfPath);
    pdfFile.readCompleteFile(pdfContent);
    pdfFile.closeFile();

    // create output for the client
    std::string output;
    Kitsunemimi::Crypto::encodeBase64(output, pdfContent.data, pdfContent.usedBufferSize);
    blossomLeaf.output.insert("documentation", new Kitsunemimi::DataValue(output));

    // delete temporary directory again
    Kitsunemimi::deleteFileOrDir("/tmp/" + uuid, error);

    return true;
}
