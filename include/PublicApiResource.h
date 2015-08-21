/* 
 * Header of API Resource
 * @author ECHOES Technologies (TSA)
 * @date 08/08/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef PUBLICAPIRESOURCE_H
#define	PUBLICAPIRESOURCE_H

#include <unistd.h>

#include <boost/algorithm/string.hpp>
//#include <boost/exception/detail/exception_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
//#include <boost/thread/recursive_mutex.hpp>

#include <Wt/Http/Client>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include <Wt/Json/Array>
#include <Wt/Json/Object>
#include <Wt/Dbo/FixedSqlConnectionPool>
#include <Wt/Json/Parser>
#include <Wt/Utils>
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WLogger>
#include <Wt/WRandom>
#include <Wt/WResource>
#include <Wt/WServer>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

namespace boost
{
  namespace property_tree
  {

    // id_translator template specialization to add quotes " " to strings in json
    template<>
    struct id_translator<std::string> {
        typedef std::string internal_type;
        typedef std::string external_type;

        boost::optional<std::string> get_value(const std::string &v) {
            return v;
        }

        boost::optional<std::string> put_value(const std::string &v) {
            return '"' + json_parser::create_escapes(v) + '"';
        }
    };

   
  }
}
/*
#include <tools/Session.h>
#include <tools/MainIncludeFile.h>
#include <tools/Enums.h>

#include "JsonSerializer.h"*/
#include "Enums.h"
#include "Conf.h"
#include "Session.h"
//#include "Utils.h"

class PublicApiResource : public Wt::WResource {
public:
    PublicApiResource();
    PublicApiResource(Session*);
    virtual ~PublicApiResource();
    
protected:
    Session* session;
    
    unsigned short retrieveCurrentHttpMethod(const std::string &method) const;
    std::string getNextElementFromPath(unsigned short &indexPathElement, std::vector<std::string> &pathElements);
    std::string request2string(const Wt::Http::Request &request);

    std::string itookiErrorToString(std::string error);
    std::string processRequestParameters(const Wt::Http::Request &request, std::vector<std::string> &pathElements, std::map<std::string, long long> &parameters);
    std::string processRequestParameters(const Wt::Http::Request &request, std::vector<std::string> &pathElements, std::map<std::string, std::string> &parameters);
    virtual EReturnCode processGetRequest(const Wt::Http::Request &request, std::string &responseMsg);
    virtual EReturnCode processPostRequest(const Wt::Http::Request &request, std::string &responseMsg);
    virtual EReturnCode processPutRequest(const Wt::Http::Request &request, std::string &responseMsg);
    virtual EReturnCode processDeleteRequest(const Wt::Http::Request &request, std::string &responseMsg);

    virtual void handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response);

    std::string generateCode();
    
    template<class C>
    std::string httpCodeToJSON(EReturnCode code, C &obj) {
        std::string message;
        //std::string tableName = getTableName(obj);
        std::stringstream ss;
        boost::property_tree::ptree elem;

        switch (code) {
            case OK:
                message = "OK";
                break;
            case CREATED:
                message = "table created";
                break;
            case ACCEPTED:
                message = "Accepted";
                break;
            case NO_CONTENT:
                message = "No Content";
                break;
            case RESET_CONTENT:
                message = "Reset Content";
                break;
            case PARTIAL_CONTENT:
                message = "Partial Content";
                break;
            case MULTIPLE_CHOICES:
                message = "Multiple Choices";
                break;
            case MOVED_PERMANENTLY:
                message = "Moved Permanently";
                break;
            case FOUND:
                message = "Found";
                break;
            case SEE_OTHER:
                message = "See Other";
                break;
            case NOT_MODIFIED:
                message = "Not Modified";
                break;
            case USE_PROXY:
                message = "Use Proxy";
                break;
            case TEMPORARY_REDIRECT:
                message = "Temporary Redirect";
                break;
            case TOO_MANY_REDIRECT:
                message = "Permanent Redirect";
                break;
            case BAD_REQUEST:
                message = "Bad Request";
                break;
            case UNAUTHORIZED:
                message = "Unauthorized";
                break;
            case PAYMENT_REQUIRED:
                message = "Payment Required";
                break;
            case FORBIDDEN:
                message = "Forbidden";
                break;
            case NOT_FOUND:
                message = "table not found";
                break;
            case METHOD_NOT_ALLOWED:
                message = "Method Not Allowed";
                break;
            case NOT_ACCEPTABLE:
                message = "Not Acceptable";
                break;
            case PROXY_AUTHENTICATION_REQUIRED:
                message = "Proxy Authentication Required";
                break;
            case REQUEST_TIME_OUT:
                message = "Request Timeout";
                break;
            case CONFLICT:
                message = "Conflict";
                break;
            case GONE:
                message = "Gone";
                break;
            case LENGTH_REQUIRED:
                message = "Length Required";
                break;
            case PRECONDITION_FAILED:
                message = "Precondition Failed";
                break;
            case REQUEST_ENTITY_TOO_LARGE:
                message = "Request Entity Too Large";
                break;
            case REQUEST_URI_TOO_LONG:
                message = "Request-URI Too Long";
                break;
            case UNSUPPORTED_MEDIA_TYPE:
                message = "Unsupported Media Type";
                break;
            case REQUESTED_RANGE_UNSATISFIABLE:
                message = "Requested Range Not Satisfiable";
                break;
            case EXPECTATION_FAILED:
                message = "Expectation Failed";
                break;
            case I_AM_A_TEAPOT:
                message = "I'm a teapot (RFC 2324)";
                break;
            case UPGRADE_REQUIRED:
                message = "Upgrade Required";
                break;
            case INTERNAL_SERVER_ERROR:
                message = "Internal Server Error";
                break;
            case NOT_IMPLEMENTED:
                message = "Not Implemented";
                break;
            case BAD_GATEWAY_OR_PROXY_ERROR:
                message = "Bad Gateway";
                break;
            case SERVICE_UNAVAILABLE:
                message = "Service Unavailable";
                break;
            case GATEWAY_TIME_OUT:
                message = "Gateway Timeout";
                break;
            case HTTP_VERSION_NOT_SUPPORTED:
                message = "HTTP Version Not Supported";
                break;
            case BANDWIDTH_LIMIT_EXCEDED:
                message = "Bandwidth Limit Exceeded";
                break;
            default:
                break;
        }
        elem.put("message", message);
        boost::property_tree::json_parser::write_json(ss, elem);
        
        return ss.str();
    }
    
};

#endif	/* PUBLICAPIRESOURCE_H */

