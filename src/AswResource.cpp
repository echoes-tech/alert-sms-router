/**
 * @file AswResource.cpp
 * @author Thomas Saquet, Florent Poinsaut
 * @date 
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Alert - SMS Router is a part of the Alert software
 * Copyright (C) 2013-2017
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
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
    
    dbo::Transaction transaction(*session, true);
    
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
    transaction.commit();
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
