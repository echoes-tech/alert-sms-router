/* 
 * API Resource
 * @author ECHOES Technologies (TSA)
 * @date 10/02/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <Wt/Dbo/SqlConnection>
#include <Wt/Dbo/Session>

namespace boost
{
  namespace property_tree
  {

    // the write_json_helper template specialization to remove the quotes " " 
    // added to every json values, strings or not
    namespace json_parser
    {
        // http://stackoverflow.com/questions/10260688/boostproperty-treejson-parser-and-two-byte-wide-characters
        // Create necessary escape sequences from illegal characters
        template<>
        std::basic_string<char> create_escapes(const std::basic_string<char> &s)
        {
            std::basic_string<char> result;
            std::basic_string<char>::const_iterator b = s.begin();
            std::basic_string<char>::const_iterator e = s.end();
            while (b != e)
            {
                // This assumes an ASCII superset. But so does everything in PTree.
                // We escape everything outside ASCII, because this code can't
                // handle high unicode characters.
                if (*b == 0x20 || *b == 0x21 || (*b >= 0x23 && *b <= 0x2E) ||
                    (*b >= 0x30 && *b <= 0x5B) || (*b >= 0x5D && *b <= 0xFF)  //it fails here because char are signed
                    || (*b >= -0x80 && *b < 0 ) ) // this will pass UTF-8 signed chars
                    result += *b;
                else if (*b == char('\b')) result += char('\\'), result += char('b');
                else if (*b == char('\f')) result += char('\\'), result += char('f');
                else if (*b == char('\n')) result += char('\\'), result += char('n');
                else if (*b == char('\r')) result += char('\\'), result += char('r');
                else if (*b == char('/')) result += char('\\'), result += char('/');
                else if (*b == char('"'))  result += char('\\'), result += char('"');
                else if (*b == char('\\')) result += char('\\'), result += char('\\');
                else
                {
                    const char *hexdigits = "0123456789ABCDEF";
                    typedef make_unsigned<char>::type UCh;
                    unsigned long u = (std::min)(static_cast<unsigned long>(
                                                     static_cast<UCh>(*b)),
                                                 0xFFFFul);
                    int d1 = u / 4096; u -= d1 * 4096;
                    int d2 = u / 256; u -= d2 * 256;
                    int d3 = u / 16; u -= d3 * 16;
                    int d4 = u;
                    result += char('\\'); result += char('u');
                    result += char(hexdigits[d1]); result += char(hexdigits[d2]);
                    result += char(hexdigits[d3]); result += char(hexdigits[d4]);
                }
                ++b;
            }
            return result;
        }

      template<>
      void write_json_helper<ptree>(std::basic_ostream<typename ptree::key_type::value_type> &stream,
              const ptree &pt,
              int indent, bool pretty)
      {
          typedef typename ptree::key_type::value_type Ch;
          typedef typename std::basic_string<Ch> Str;

          // Value or object or array
          if (indent > 0 && pt.empty())
          {
              // Write value
              stream << pt.get_value<Str>();

          }
          else if (indent > 0 && pt.count(Str()) == pt.size())
          {
              // Write array
              stream << Ch('[');
              if (pretty) stream << Ch('\n');
              typename ptree::const_iterator it = pt.begin();
              for (; it != pt.end(); ++it)
              {
                  if (pretty) stream << Str(4 * (indent + 1), Ch(' '));
                  write_json_helper(stream, it->second, indent + 1, pretty);
                  if (boost::next(it) != pt.end())
                      stream << Ch(',');
                  if (pretty) stream << Ch('\n');
              }
              stream << Str(4 * indent, Ch(' ')) << Ch(']');

          }
          else
          {
              // Write object
              stream << Ch('{');
              if (pretty) stream << Ch('\n');
              typename ptree::const_iterator it = pt.begin();
              for (; it != pt.end(); ++it)
              {
                  if (pretty) stream << Str(4 * (indent + 1), Ch(' '));
                  stream << Ch('"') << create_escapes(it->first) << Ch('"') << Ch(':');
                  if (pretty)
                  {
                      if (it->second.empty())
                          stream << Ch(' ');
                      else
                          stream << Ch('\n') << Str(4 * (indent + 1), Ch(' '));
                  }
                  write_json_helper(stream, it->second, indent + 1, pretty);
                  if (boost::next(it) != pt.end())
                      stream << Ch(',');
                  if (pretty) stream << Ch('\n');
              }
              if (pretty) stream << Str(4 * indent, Ch(' '));
              stream << Ch('}');
          }
        }
      
      
    }
  }
}
#include "PublicApiResource.h"

using namespace std;

PublicApiResource::PublicApiResource() : Wt::WResource()
{
}

PublicApiResource::PublicApiResource(Session* sess) : Wt::WResource(), session(sess)
{
}

PublicApiResource::~PublicApiResource()
{
    beingDeleted();
}

unsigned short PublicApiResource::retrieveCurrentHttpMethod(const string &method) const
{
    unsigned short res = 0;

    if (!method.compare("GET"))
    {
        res = Wt::Http::Get;
    }
    else if (!method.compare("POST"))
    {
        res = Wt::Http::Post;
    }
    else if (!method.compare("PUT"))
    {
        res = Wt::Http::Put;
    }
    else if (!method.compare("DELETE"))
    {
        res = Wt::Http::Delete;
    }

    return res;
}

string PublicApiResource::getNextElementFromPath(unsigned short &indexPathElement, vector<string> &pathElements)
{
    string res = "";

    if (pathElements.size() > indexPathElement)
    {
        res = pathElements[indexPathElement++];
    }

    return res;
}

string PublicApiResource::request2string(const Wt::Http::Request &request)
{
    char c;
    string s;

    // Getting the input stream for the request char by char
    c = request.in().get();
    while (request.in())
    {
        s.append(1, c);
        c = request.in().get();
    }

    // WHY: Dans l'appli mobile lorsqu'on fait un post les premiers caractères de la requête sont remplacés par des caractères spéciaux. 
    // N'ayant pas su résoudre ce probléme, l'appli mobile envoie ses requêtes avec des caractères en trop au début du JSON. Il faut donc les supprimer.
    // On supprime donc tous les caractére avant "[" ou "{" suivant les cas pour éviter les problèmes de parse.
    const size_t pos1 = s.find("{", 0);
    const size_t pos2 = s.find("[", 0);
    if (pos1 != 0 || pos2 != 0)
    {
        if (pos2 != string::npos && pos2 < pos1)
        {
            s = s.erase(0, pos2);
        }
        else
        {
            s = s.erase(0, pos1);
        }
    }

    return s;
}

string PublicApiResource::processRequestParameters(const Wt::Http::Request &request, vector<string> &pathElements, map<string, long long> &parameters)
{
    const string path = request.pathInfo();
    boost::split(pathElements, path, boost::is_any_of("/"), boost::token_compress_on);

    for (map<std::string, long long>::iterator it = parameters.begin(); it != parameters.end(); ++it)
    {
        if (request.getParameter(it->first) != 0)
        {
            try
            {
                it->second = boost::lexical_cast<unsigned long long>(*request.getParameter(it->first));
            }
            catch (boost::bad_lexical_cast const& e)
            {
                Wt::log("warning") << "[PUBLIC API] Bad URL parameter: " << e.what();
            }
        }
    }
    
    return request2string(request);
}

string PublicApiResource::itookiErrorToString(string error)
{
    string returnValue = "error";
    
    if(!error.empty())
    {
        if(error == "80")
        {
            returnValue = "message sended";
        }
        else if(error == "81")
        {
            returnValue = "message saved for diff";
        }
        else if(error == "82")
        {
            returnValue = "incorect identfier";
        }
        else if(error == "83")
        {
            returnValue = "no more credit";
        }
        else if(error == "84")
        {
            returnValue = "gsm number invalid";
        }
        else if(error == "85")
        {
            returnValue = "invalid format for diff";
        }
        else if(error == "86")
        {
            returnValue = "group contact is empty";
        }
        else if(error == "87")
        {
            returnValue = "email is empty";
        }
        else if(error == "88")
        {
            returnValue = "pass is empty";
        }
        else if(error == "89")
        {
            returnValue = "number is empty";
        }
        else if(error == "90")
        {
            returnValue = "message is empty";
        }
        else if(error == "91")
        {
            returnValue = "message yet sended today";
        }
        else if(error == "92")
        {
            returnValue = "positive test";
        }
        else if(error == "93")
        {
            returnValue = "DOM TOM option unvalaible";
        }
        else if(error == "94")
        {
            returnValue = "diff saved has been deleted";
        }
        else if(error == "95")
        {
            returnValue = "diff saved has not been deleted";
        }
        else if(error == "96")
        {
            returnValue = "invalid IP";
        }
        else if(error == "97")
        {
            returnValue = "sender id invalid";
        }
        else if(error == "98")
        {
            returnValue = "invalid starting date";
        }
        else if(error == "99")
        {
            returnValue = "invalid ending date";
        }
        else if(error == "100")
        {
            returnValue = "starting date < ending date";
        }
        else if(error == "101")
        {
            returnValue = "blocked or blackisted number";
        }
        else if(error == "102")
        {
            returnValue = "add stop SMS at the end of your message";
        }
        else
        {
            returnValue = "unkow error"; 
        }
    }
    else
    {
        returnValue = "no error number";
    }
    return (returnValue);
}

string PublicApiResource::processRequestParameters(const Wt::Http::Request &request, vector<string> &pathElements, map<string, string> &parameters)
{
    const string path = request.pathInfo();
    boost::split(pathElements, path, boost::is_any_of("/"), boost::token_compress_on);
    
    for (map<std::string, string>::iterator it = parameters.begin(); it != parameters.end(); ++it)
    {
        if (request.getParameter(it->first) != 0)
        {
            it->second = boost::lexical_cast<string>(*request.getParameter(it->first));
        }
    }
    
    return request2string(request);
}

EReturnCode PublicApiResource::processGetRequest(const Wt::Http::Request &request, std::string &responseMsg)
{
    EReturnCode res = EReturnCode::BAD_REQUEST;
    const string err = "[Public Api Resource] GET Method not implemented";
    responseMsg = httpCodeToJSON(res, err);
    return res;
}

EReturnCode PublicApiResource::processPostRequest(const Wt::Http::Request &request, std::string &responseMsg)
{
    EReturnCode res = EReturnCode::BAD_REQUEST;
    const string err = "[Public Api Resource] POST Method not implemented";
    responseMsg = httpCodeToJSON(res, err);
    return res;
}

EReturnCode PublicApiResource::processPutRequest(const Wt::Http::Request &request, std::string &responseMsg)
{
    EReturnCode res = EReturnCode::BAD_REQUEST;
    const string err = "[Public Api Resource] PUT Method not implemented";
    responseMsg = httpCodeToJSON(res, err);
    return res;
}

EReturnCode PublicApiResource::processDeleteRequest(const Wt::Http::Request &request, std::string &responseMsg)
{
    EReturnCode res = EReturnCode::BAD_REQUEST;
    const string err = "[Public Api Resource] DELETE Method not implemented";
    responseMsg = httpCodeToJSON(res, err);
    return res;
}

void PublicApiResource::handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response)
{
        EReturnCode res = EReturnCode::INTERNAL_SERVER_ERROR;
        string responseMsg = "";
        switch (retrieveCurrentHttpMethod(request.method()))
        {
            case Wt::Http::Get:
                res = processGetRequest(request, responseMsg);
                break;
            case Wt::Http::Post:
                res = processPostRequest(request, responseMsg);
                break;
            case Wt::Http::Put:
                res = processPutRequest(request, responseMsg);
                break;
            case Wt::Http::Delete:
                res = processDeleteRequest(request, responseMsg);
                break;
            default:
                res = EReturnCode::METHOD_NOT_ALLOWED;
                responseMsg = "{\n\t\"message\": \"Only GET, POST, PUT and DELETE methods are allowed.\n\"}";
                break;
        }

        response.setStatus(res);
        response.out() << responseMsg;
}

std::string PublicApiResource::generateCode()
{
    srand(time(NULL));
    int random = ((rand() % 900) + 100);
    return (boost::lexical_cast<std::string>(random));
}