/**
 * @file main.cpp
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

#include <cstdlib>
#include <signal.h>

#include "Conf.h"

#include "PublicApiResource.h"

#include "AckResource.h"
#include "AswResource.h"
#include "SendResource.h"
#include "Session.h"

using namespace std;

int main(int argc, char** argv)
{

    int res = EXIT_FAILURE;
    // use argv[0] as the application name to match a suitable entry
        // in the Wt configuration file
        Wt::WServer server(argv[0]);

        // On définit la configuration du serveur en lui passant les paramètres d'entrée et son fichier de configuration
        server.setServerConfiguration(argc, argv);
        
        if (conf.readProperties(server))
        {
            Session session(conf.getSessConnectParams());
            
            AckResource     ackResource(&session);
            AswResource     aswResource(&session);
            SendResource    sendResource(&session);
         
            
            server.addResource(&ackResource,    "/ack");
            server.addResource(&aswResource,    "/asw");
            server.addResource(&sendResource,   "/send");
            
            Wt::WString::setDefaultEncoding(Wt::CharEncoding::UTF8);

            // démarrage du serveur en arrière plan
            if (server.start())
            {
                // méthode qui bloque le thread courant en attendant le signal d'exctinction
                int sig = Wt::WServer::waitForShutdown(argv[0]);

                Wt::log("info") << "[Main] Shutdown (signal = " << sig << ")";

                server.stop();

                if (sig == SIGHUP)
                {
                    Wt::WServer::restart(argc, argv, environ);
                }

                res = EXIT_SUCCESS;
            }
        }
        else
        {
            Wt::log("fatal") << "[Main] Every properties are not correctly set in " << WT_CONFIG_XML;
        }
    return res;
}
