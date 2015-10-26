/* 
 * File:   SendResource.h
 * Author: cde
 *
 * Created on 21 mai 2015, 17:01
 */

#ifndef SENDRESOURCE_H
#define	SENDRESOURCE_H

#include "PublicApiResource.h"
#include "Session.h"

class SendResource : public PublicApiResource
{
    public :
        SendResource();
        SendResource(Session*);
        virtual ~SendResource();
        
    protected :
        void handleHttpResponse(boost::system::error_code err, const Wt::Http::Message& response, Wt::Dbo::ptr<SavedSend> savedSendPtr);
        EReturnCode receptionSend(std::string &responseMsg, const std::vector<std::string> &pathElements = std::vector<std::string>(), const std::string &sRequest = "", std::map<std::string, std::string> parameters = std::map<std::string, std::string>());       
        virtual EReturnCode processPostRequest(const Wt::Http::Request &request, std::string &responseMsg);
        
        EReturnCode deleteMessage(std::string &responseMsg, const std::vector<std::string> &pathElements = std::vector<std::string>(), const std::string &sRequest = "", std::map<std::string, std::string> parameters = std::map<std::string, std::string>());
        virtual EReturnCode processDeleteRequest(const Wt::Http::Request &request, std::string &responseMsg);
        
        std::string generateTemporaryCode();
};

#endif	/* SENDRESOURCE_H */

