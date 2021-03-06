/**
 * @file SendResource.cpp
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

#include "SendResource.h"

using namespace std;

SendResource::SendResource() : PublicApiResource::PublicApiResource()
{
}

SendResource::SendResource(Session* session) : PublicApiResource::PublicApiResource(session)
{
}

SendResource::~SendResource()
{
}

std::string SendResource::generateTemporaryCode()
{
    Wt::Dbo::ptr<SavedSend> dest;
    bool notUnique = true;
    string codeGenerated;
    
    while(notUnique)
    {
        codeGenerated = generateCode();
        dest = session->find<SavedSend>().where("\"code_ref\" = ?").bind(codeGenerated).where("\"delete\" IS NULL ");
        if(dest)
        {
            notUnique = true;
        }
        else
        {
            notUnique = false;
        }
    }
    return codeGenerated;
}

EReturnCode SendResource::receptionSend(map<string, long long> parameters, const vector<string> &pathElements, const string &sRequest, string &responseMsg, string ipSender)
{   
    EReturnCode res = EReturnCode::OK;
    
    responseMsg = "send recu";
    if (!sRequest.empty())
        {
            int port_back = 0;
            Wt::WString number = "missing";
            Wt::WString message = "missing";
            Wt::WString FQDN_back = "missing";
            bool isValid = false;
            
            try
            {
                Wt::Json::Object result;
                Wt::Json::parse(sRequest, result);

                if (result.contains("number")
                        && result.contains("message")
                        && result.contains("port_back")
                        && result.contains("FQDN_back"))
                {
                    number = result.get("number");
                    message = result.get("message");
                    port_back = result.get("port_back");
                    FQDN_back = result.get("FQDN_back");
                    isValid = true;
                }
            }
            catch (Wt::Json::ParseError const& e)
            {
                res = EReturnCode::BAD_REQUEST;
                responseMsg = httpCodeToJSON(res, e);
            }
            catch (Wt::Json::TypeException const& e)
            {
                res = EReturnCode::BAD_REQUEST;
                responseMsg = httpCodeToJSON(res, e);
            }
            try
            {
                dbo::Transaction transaction(*session, true);

                //création du message
                SavedSend *savedSend = new SavedSend();
                savedSend->number = boost::lexical_cast<std::string>(number);                
                savedSend->adress_sender = boost::lexical_cast<std::string>(FQDN_back);
                savedSend->port = port_back;
                savedSend->refenvoi = "";
                savedSend->code_ref = generateTemporaryCode();
                //enregistrement du message
                Wt::Dbo::ptr<SavedSend> savedSendPtr = session->add<SavedSend>(savedSend);

                
                //si la demande est incorrecte
                if (!isValid)
                {
                    //création de l'état refusé
                    StateMessage *stateMessage = new StateMessage();
                    stateMessage->id_message = savedSendPtr;
                    stateMessage->date_event = Wt::WDateTime::currentDateTime();
                    stateMessage->state = StateMessage::StateList::SendRefused;
                    //enregistrement
                    Wt::Dbo::ptr<StateMessage> stateMessageValidPtr = session->add<StateMessage>(stateMessage);
                    
                    res = EReturnCode::BAD_REQUEST;
                    const string err = "[Send Resource] Missing parameter";
                    responseMsg = httpCodeToJSON(res, err);
                }
                else
                {
                    //création de l'état created
                    StateMessage *stateMessage = new StateMessage();
                    stateMessage->id_message = savedSendPtr;
                    stateMessage->date_event = Wt::WDateTime::currentDateTime();
                    stateMessage->state = StateMessage::StateList::Created;
                    
                    //enregistrement
                    Wt::Dbo::ptr<StateMessage> stateMessageValidPtr = session->add<StateMessage>(stateMessage);
                    
                    try
                    {
                        Wt::Http::Client *client = new Wt::Http::Client();
                        Wt::WString msgTmp = message;
                        message = ("code: " + savedSendPtr->code_ref + "***");
                        message += msgTmp;
                        
                        client->done().connect(boost::bind(&SendResource::handleHttpResponse, this, _1, _2, savedSendPtr));
                        string url = "http";
                        if (conf.isSmsHttps())
                        {
                            url += "s";
                        }
                        url += "://www.itooki.fr/http.php"
                                "?email=" + Wt::Utils::urlEncode(conf.getSmsLogin()) +
                                "&pass=" + Wt::Utils::urlEncode(conf.getSmsPassword()) +
                                "&numero=" + Wt::Utils::urlEncode(boost::lexical_cast<std::string>(number)) +
                                "&message=" + Wt::Utils::urlEncode(boost::lexical_cast<std::string>(message)) + 
                                "&refenvoi=o";

                        Wt::log("info") << "[Itooki SMS Sender] Trying to send request to Itooki API";
                        Wt::log("debug") << "[Itooki SMS Sender] Address : " << url;
                        if (client->get(url))
                        {
                            Wt::log("info") << "[Itooki SMS Sender] Message sent to Itooki API";
                            //création de l'état envoi reussi
                            StateMessage *stateMessage2 = new StateMessage();
                            stateMessage2->id_message = savedSendPtr;
                            stateMessage2->date_event = Wt::WDateTime::currentDateTime();;
                            stateMessage2->state = StateMessage::StateList::Sended;
                            
                            Wt::Dbo::ptr<StateMessage> stateMessageSendedPtr = session->add<StateMessage>(stateMessage2);
                            
                            res = EReturnCode::OK;
                            const string err = "[Send Resource] Message sended";
                            responseMsg = ("{ \"ok\" : true, \"ref\" : \"" + savedSendPtr->code_ref +"\" }");
                        } 
                        else 
                        {
                            //création de l'état envoi echoué
                            StateMessage *stateMessage2 = new StateMessage();
                            stateMessage2->id_message = savedSendPtr;
                            stateMessage2->date_event = Wt::WDateTime::currentDateTime();
                            stateMessage2->state = StateMessage::StateList::SendFailed;
                            
                            Wt::Dbo::ptr<StateMessage> stateMessageSendedPtr = session->add<StateMessage>(stateMessage2);
                    
                            res = EReturnCode::INTERNAL_SERVER_ERROR;
                            const string err = "[Send Resource] Url looks invalid. please check it out";
                            responseMsg = httpCodeToJSON(res, err);
                            
                            Wt::log("error") << "[Itooki SMS Sender] Failed to send message to Itooki API";
                        }
                        }
                        catch (Wt::Dbo::Exception const& e)
                        {
                            Wt::log("error") << "[Itooki SMS Sender] " << e.what();
                        }
                }
                transaction.commit();
            }
            catch (Wt::Dbo::Exception const& e)
            {
                res = EReturnCode::SERVICE_UNAVAILABLE;
                responseMsg = httpCodeToJSON(res, e);
            }
        }
        else
        {
            res = EReturnCode::BAD_REQUEST;
            const string err = "[Send Resource] sRequest is not empty";
            responseMsg = httpCodeToJSON(res, err);
        }
    return (res);
}

void SendResource::handleHttpResponse(boost::system::error_code err, const Wt::Http::Message& response, Wt::Dbo::ptr<SavedSend> savedSendPtr)
{
    dbo::Transaction transaction(*session, true);
    //preparation de la reponse
    Wt::Http::Client *client = new Wt::Http::Client();
    
    if(savedSendPtr)
    {
        string url = "http";
                url += "://" + Wt::Utils::urlEncode(savedSendPtr->adress_sender) +
                        ":" + Wt::Utils::urlEncode(boost::lexical_cast<std::string>(savedSendPtr->port)) + 
                      "/itooki/sended";
        string json = "{";
        if (!err && response.status() == 200)
        {
            //si on a pas de code d'erreur
            if(response.body().length() > 3)
            {
                savedSendPtr.modify()->refenvoi = response.body().erase(0,3); 
                json += "\"sended\" : true,";
                json += "\"refenvoiToChange\" : \"" + savedSendPtr->code_ref + "\",";            
                json += "\"refenvoi\" : \"" + savedSendPtr->refenvoi + "\",";

            }
            else
            {
                json += "\"sended\" : false,";
                json += "\"refenvoiToChange\" : \"none\",";
                json += "\"refenvoi\" : \"none\",";
            }
            json += "\"error\" : \"" + itookiErrorToString(response.body()) + "\"";
        }
        else
        {
            json += "\"sended\" : false,";
            json += "\"refenvoi\" : \"none\",";
            json += "\"refenvoiToChange\" : \"none\",";
            json += "\"error\" : \"send failed\"";
        }
        json += "}";

        Wt::log("info") << "[Itooki SMS Sender] Trying to send request to  API : " << url << " with JSON : " << json;

        Wt::Http::Message httpMessage;
        httpMessage.addBodyText(json);

        client->post(url, httpMessage);
    }
    else
    {
        string url = "http://127.0.0.1/erreur/" + Wt::Utils::urlEncode(boost::lexical_cast<std::string>(savedSendPtr.id()));
        client->get(url);
         Wt::log("error") << "[Itooki SMS Sender] savedSend not found";
    }
    transaction.commit();
}

EReturnCode SendResource::processPostRequest(const Wt::Http::Request &request, std::string &responseMsg)
{
    EReturnCode res = EReturnCode::INTERNAL_SERVER_ERROR;
    string nextElement = "";
    unsigned short indexPathElement = 1;
    vector<string> pathElements;
    map<string, long long> parameters;
    string ipSender = request.clientAddress();
    
    const string sRequest = processRequestParameters(request, pathElements, parameters);

    nextElement = getNextElementFromPath(indexPathElement, pathElements);

    if (nextElement.empty())
    {
        res = receptionSend(parameters, pathElements, sRequest, responseMsg, ipSender);
    }
    else
    {
        res = EReturnCode::BAD_REQUEST;
        responseMsg = "chemin incorrect";
    }

    return res;
}

EReturnCode SendResource::deleteMessage(std::map<std::string, long long> parameters, const std::vector<std::string> &pathElements, const std::string &sRequest, std::string &responseMsg)
{
    EReturnCode res = EReturnCode::INTERNAL_SERVER_ERROR;
    
    dbo::Transaction transaction(*session, true);
    
    
    Wt::Dbo::ptr<SavedSend> ptrMessage = session->find<SavedSend>().where("\"refenvoi\" = ?").bind(pathElements[1]);

    if(ptrMessage)
    {
        StateMessage *stateMessage = new StateMessage();
        stateMessage->id_message = ptrMessage;
        stateMessage->date_event = Wt::WDateTime::currentDateTime();
        stateMessage->state = StateMessage::StateList::Deleted;
        
        Wt::Dbo::ptr<StateMessage> stateMessageSendedPtr = session->add<StateMessage>(stateMessage);
        
        ptrMessage.modify()->deleteTag = Wt::WDateTime::currentDateTime();
        
        res = EReturnCode::NO_CONTENT;
    }
    else
    {
        res = EReturnCode::NOT_FOUND;
        responseMsg = httpCodeToJSON(res, ptrMessage);
    }
    transaction.commit();
    return res;
}
 

EReturnCode SendResource::processDeleteRequest(const Wt::Http::Request &request, std::string &responseMsg)
{
    EReturnCode res = EReturnCode::INTERNAL_SERVER_ERROR;
    string nextElement = "";
    unsigned short indexPathElement = 1;
    vector<string> pathElements;
    map<string, long long> parameters;
    
    const string sRequest = processRequestParameters(request, pathElements, parameters);

    nextElement = getNextElementFromPath(indexPathElement, pathElements);

    if (nextElement.empty())
    {
        res = EReturnCode::BAD_REQUEST;
        responseMsg = "chemin incorrect";
    }
    else
    {
        nextElement = getNextElementFromPath(indexPathElement, pathElements);
        if (nextElement.empty())
        {
            res = deleteMessage(parameters, pathElements, sRequest, responseMsg);
        }
        else
        {
            res = EReturnCode::BAD_REQUEST;
            responseMsg = "chemin incorrect";
        }
    }

    return res;
}
