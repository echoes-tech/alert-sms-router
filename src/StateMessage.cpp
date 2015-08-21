/* 
 * File:   StateMessage.cpp
 * Author: cde
 * 
 * Created on 29 mai 2015, 14:30
 */

#include "StateMessage.h"

StateMessage::StateMessage() {
    state = StateList::Null;
}

StateMessage::StateMessage(const StateMessage& orig) {
    state = orig.state;
    date_event = orig.date_event;
    id_message = orig.id_message;
}

StateMessage::~StateMessage() {
}

