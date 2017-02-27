/**
 * @file Conf.h
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

#ifndef CONF_H
#define	CONF_H

#include <Wt/WServer>
#include <Wt/WLogger>

class Conf {
    public:
        Conf();
        virtual ~Conf();

        /**
         * Reads ECHOES configuration properties.
         * @param WServer
         * @return true if every value are defined or false
         */
        bool readProperties(Wt::WServer& server);

        std::string getSessConnectParams() const;
        std::string getSMTPHost() const;
        unsigned getSMTPPort() const;
        std::string getAlertMailSenderAddress() const;
        std::string getAlertMailSenderName() const;
        bool isSmsHttps() const;
        std::string getSmsPassword() const;
        std::string getSmsLogin() const;

    protected:
        std::string m_sessConnectParams;
        std::string m_smtpHost;
        unsigned m_smtpPort;
        std::string m_alertMailSenderName;
        std::string m_alertMailSenderAddress;
        std::string m_smsLogin;
        std::string m_smsPassword;
        bool m_smsHttps;

        void setSessConnectParams
        (
                std::string dbHost,
                unsigned dbPort,
                std::string dbName,
                std::string dbUser,
                std::string dbPassword
        );
        void setSMTPPort(unsigned smtpPort);
        void setAlertMailSenderAddress(std::string alertMailSenderAddress);
        void setAlertMailSenderName(std::string alertMailSenderName);
        void setSMTPHost(std::string smtpHost);
        void setSmsHttps(bool smsHttps);
        void setSmsPassword(std::string smsPassword);
        void setSmsLogin(std::string smsLogin);
};

extern Conf conf;

#endif	/* CONF_H */
