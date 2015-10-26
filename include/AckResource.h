/* 
 * File:   AckResource.h
 * Author: cde
 *
 * Created on 21 mai 2015, 17:03
 */

#ifndef ACKRESOURCE_H
#define	ACKRESOURCE_H

#include "PublicApiResource.h"

class AckResource : public PublicApiResource
{
    public :
        AckResource();
        AckResource(Session*);
        virtual ~AckResource();

    protected :
        EReturnCode receptionAck(std::string &responseMsg, const std::vector<std::string> &pathElements = std::vector<std::string>(), const std::string &sRequest = "", std::map<std::string, std::string> parameters = std::map<std::string, std::string>());
        virtual EReturnCode processGetRequest(const Wt::Http::Request &request, std::string &responseMsg);
};

#endif	/* ACKRESOURCE_H */

