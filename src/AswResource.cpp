/* 
 * Routeur AswResource
 * @author cde
 * @date 21/05/2015
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2015 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "AswResource.h"

using namespace std;

AswResource::AswResource() : PublicApiResource::PublicApiResource()
{
}

AswResource::AswResource(Session* session) : PublicApiResource::PublicApiResource(session)
{
}

AswResource::~AswResource()
{
}

EReturnCode AswResource::receptionAsw(map<string, string> parameters, const vector<string> &pathElements, const string &sRequest, string &responseMsg)
{
    EReturnCode res = EReturnCode::OK;
    
    dbo::Transaction transaction(*session);
    
    Wt::Dbo::ptr<SavedSend> ptrMessage = session->find<SavedSend>().where("\"code_ref\" = ?").bind(parameters["message"].substr(0,3));
    
    if(ptrMessage)
    {
        //création de l'état asw
        StateMessage *stateMessage = new StateMessage();
        stateMessage->id_message = ptrMessage;
        stateMessage->date_event = Wt::WDateTime::currentDateTime();
        stateMessage->state = StateMessage::StateList::Answered;

        //enregistrement
        Wt::Dbo::ptr<StateMessage> stateMessageRcvPtr = session->add<StateMessage>(stateMessage);
            
        //preparation de la reponse
        Wt::Http::Client *client = new Wt::Http::Client();
            
        string url = "http";
        url += "://" + Wt::Utils::urlEncode(ptrMessage->adress_sender) +
                ":" + Wt::Utils::urlEncode(boost::lexical_cast<std::string>(ptrMessage->port)) + 
              "/itooki/asw";
            
        string json = "{";
        json += "\"refenvoi\" : \"" + ptrMessage->refenvoi + "\",";
        json += "\"message\" : \"" + parameters["message"].erase(0,4) + "\"";
        json += "}";
            
        Wt::Http::Message httpMessage;
        httpMessage.addBodyText(json);
            
        client->post(url, httpMessage);
    }
    else
    {
        res = EReturnCode::BAD_REQUEST;
        const string err = "[ASW Resource] No message with this code";
        responseMsg = httpCodeToJSON(res, err);
    }

    return (res);
}

EReturnCode AswResource::processGetRequest(const Wt::Http::Request &request, std::string &responseMsg)
{
    EReturnCode res = EReturnCode::INTERNAL_SERVER_ERROR;
    string nextElement = "";
    unsigned short indexPathElement = 1;
    vector<string> pathElements;
    map<string, string> parameters;

    parameters["numero"] = "";
    parameters["message"] = "";

    const string sRequest = processRequestParameters(request, pathElements, parameters);

    nextElement = getNextElementFromPath(indexPathElement, pathElements);

    if (nextElement.empty())
    {
        res = receptionAsw(parameters, pathElements, sRequest, responseMsg);
    }
    else
    {
        res = EReturnCode::BAD_REQUEST;
        responseMsg = "chemin incorrect";
    }

    return res;
}



