/**
 * @file AckResource.cpp
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

#include <Wt/Dbo/Session>

#include "AckResource.h"

using namespace std;

AckResource::AckResource() : PublicApiResource::PublicApiResource()
{
}

AckResource::AckResource(Session* session) : PublicApiResource::PublicApiResource(session)
{
}


AckResource::~AckResource()
{
}

EReturnCode AckResource::receptionAck(map<string, string> parameters, const vector<string> &pathElements, const string &sRequest, string &responseMsg)
{
    EReturnCode res = EReturnCode::OK;
    
    dbo::Transaction transaction(*session, true);
    
    //on recupere le code d'erreur d'envoi. Si celui çi est egal à 0
    //le message a bien ete envoye
    //(104 => expiré, 107 => ne peut pas recevoir, 115 => num invalide)
    string sended;
    if(parameters["erreur"] == "0")
    {
        sended = "true";
    }
    else
    {
        sended = "false";
    }
    
    Wt::Dbo::ptr<SavedSend> dest = session->find<SavedSend>().where("\"refenvoi\" = ?").bind(parameters["refenvoi"]);
    if(dest)
    {
        //création de l'état ack
        StateMessage *stateMessage = new StateMessage();
        stateMessage->id_message = dest;
        stateMessage->date_event = Wt::WDateTime::currentDateTime();
        if(sended == "true")
        {
            stateMessage->state = StateMessage::StateList::Received;
        }
        else
        {
            stateMessage->state = StateMessage::StateList::AckFailed;
        }
        
        //enregistrement
        Wt::Dbo::ptr<StateMessage> stateMessageRcvPtr = session->add<StateMessage>(stateMessage);
            
        //preparation de la reponse
        Wt::Http::Client *client = new Wt::Http::Client();
            
        string url = "http";
        url += "://" + Wt::Utils::urlEncode(dest->adress_sender) +
                ":" + Wt::Utils::urlEncode(boost::lexical_cast<std::string>(dest->port)) + 
                "/itooki/ack";
            
        string json = "{";
        json += "\"sended\" : \"" + sended + "\",";
        json += "\"error\" : \"" + itookiErrorToString(parameters["erreur"]) + "\",";
        json += "\"refenvoi\" : \"" + dest->refenvoi + "\"";
        json += "}";
            
        Wt::Http::Message httpMessage;
        httpMessage.addBodyText(json);
            
        client->post(url, httpMessage);
    }
    else
    {
        res = EReturnCode::BAD_REQUEST;
        const string err = "[Ack Resource] No sender with this id";
        responseMsg = httpCodeToJSON(res, err);
    }
    transaction.commit();
    return (res);
}

EReturnCode AckResource::processGetRequest(const Wt::Http::Request &request, std::string &responseMsg)
{
    EReturnCode res = EReturnCode::INTERNAL_SERVER_ERROR;
    string nextElement = "";
    unsigned short indexPathElement = 1;
    vector<string> pathElements;
    map<string, string> parameters;

    parameters["erreur"] = "";
    parameters["refenvoi"] = "";
    parameters["numero"] = "";
    parameters["nb_credits"] = "";
    
    const string sRequest = processRequestParameters(request, pathElements, parameters);

    nextElement = getNextElementFromPath(indexPathElement, pathElements);

    if (nextElement.empty())
    {
        res = receptionAck(parameters, pathElements, sRequest, responseMsg);
    }
    else
    {
        res = EReturnCode::BAD_REQUEST;
        responseMsg = "chemin incorrect";
    }

    return res;
}
