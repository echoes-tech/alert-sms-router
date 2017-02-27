/**
 * @file SendResource.h
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
