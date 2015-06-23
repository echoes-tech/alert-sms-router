/* 
 * File:   Session.cpp
 * Author: cde
 * 
 * Created on 26 mai 2015, 16:49
 */

#include "Session.h"


Session::Session()
{   
    initMapClass();
}

Session::Session(std::string connectionParams) : postgreSession(connectionParams)
{
    
    setConnection(postgreSession);
    
    initMapClass();
    
    
}

Session::~Session() {
}

void Session::initMapClass()
{
    mapClass<SavedSend>("SavedSend");
    mapClass<StateMessage>("StateMessage");
    try
    {
        createTables();
    }
    catch (Wt::Dbo::Exception const& e)
    {
    }
            
}
