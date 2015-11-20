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
        void handleHttpResponse(boost::system::error_code err, const Wt::Http::Message& response, Wt::Dbo::ptr<SavedSend> savedSendPtrId);
        EReturnCode receptionSend(std::map<std::string, long long> parameters, const std::vector<std::string> &pathElements, const std::string &sRequest, std::string &responseMsg, std::string ipSender);       
        virtual EReturnCode processPostRequest(const Wt::Http::Request &request, std::string &responseMsg);
        
        EReturnCode deleteMessage(std::map<std::string, long long> parameters, const std::vector<std::string> &pathElements, const std::string &sRequest, std::string &responseMsg);
        virtual EReturnCode processDeleteRequest(const Wt::Http::Request &request, std::string &responseMsg);
        
        std::string generateTemporaryCode();
};

#endif	/* SENDRESOURCE_H */

