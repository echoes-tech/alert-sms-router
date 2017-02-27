/**
 * @file StateMessage.h
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

#ifndef STATE_H
#define	STATE_H


#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/WtSqlTraits>
#include <string>

#include "SavedSend.h"

namespace dbo = Wt::Dbo;

class StateMessage {
public:
    StateMessage();
    StateMessage(const StateMessage& orig);
    virtual ~StateMessage();
    
    enum StateList 
    {
        Null = 0,
        SendRefused = 1,
        Created = 2,
        SendFailed = 3,
        Sended = 4,
        Received = 5,
        AckFailed = 6,
        Answered = 7,
        Deleted = 8
    };
    
    StateList state;
    Wt::WDateTime date_event;
    Wt::Dbo::ptr<SavedSend> id_message;
   
    
    template<class Action>
    void persist(Action& a)
    {
      dbo::field(a, state,          "state");
      dbo::field(a, date_event,    "date_event");
      dbo::belongsTo(a, id_message,     "id_message");

    }
    
private:

};

#endif	/* STATE_H */
