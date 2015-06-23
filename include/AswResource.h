/* 
 * File:   AswResource.h
 * Author: cde
 *
 * Created on 21 mai 2015, 17:04
 */

#ifndef ASWRESOURCE_H
#define	ASWRESOURCE_H

#include "PublicApiResource.h"

class AswResource : public PublicApiResource
{
    public :
        AswResource();
        AswResource(Session*);
        virtual ~AswResource();

    protected :
        EReturnCode receptionAsw(std::map<std::string, std::string> parameters, const std::vector<std::string> &pathElements, const std::string &sRequest, std::string &responseMsg);
        virtual EReturnCode processGetRequest(const Wt::Http::Request &request, std::string &responseMsg);
};

#endif	/* ASWRESOURCE_H */

