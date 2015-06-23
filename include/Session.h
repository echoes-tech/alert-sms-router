/* 
 * File:   Session.h
 * Author: cde
 *
 * Created on 26 mai 2015, 16:49
 */

#ifndef SESSION_H
#define	SESSION_H

#include <Wt/Auth/Login>
#include "Wt/Auth/AuthService"
#include "Wt/Auth/HashFunction"
#include "Wt/Auth/PasswordService"
#include "Wt/Auth/PasswordStrengthValidator"
#include "Wt/Auth/PasswordVerifier"
#include "Wt/Auth/GoogleService"
#include "Wt/Auth/Dbo/AuthInfo"

#include <Wt/Dbo/Session>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/backend/Postgres>

namespace dbo = Wt::Dbo;

#include "SavedSend.h"
#include "StateMessage.h"

class Session : public Wt::Dbo::Session
{
    public:
        Session();
        Session(std::string connectionParams);
        ~Session();
        
        void initMapClass();
        
    private:
        Wt::Dbo::backend::Postgres postgreSession;
        
};

#endif	/* SESSION_H */

