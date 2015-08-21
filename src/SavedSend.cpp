/* 
 * File:   SavedSend.cpp
 * Author: cde
 * 
 * Created on 26 mai 2015, 14:30
 */

#include "SavedSend.h"

SavedSend::SavedSend() {
    port = 0;
    adress_sender = "";
}

SavedSend::SavedSend(const SavedSend& orig) {
    port = orig.port;
    adress_sender = orig.adress_sender;
    number = orig.number;
    refenvoi = orig.refenvoi;
    code_ref = orig.code_ref;
    deleteTag = orig.deleteTag;
}

SavedSend::~SavedSend() {
}

