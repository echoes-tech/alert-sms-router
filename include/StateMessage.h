/* 
 * File:   state.h
 * Author: cde
 *
 * Created on 29 mai 2015, 12:36
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
        Answered = 7
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

