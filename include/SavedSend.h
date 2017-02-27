/**
 * @file SavedSend.h
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

#ifndef SAVEDSEND_H
#define	SAVEDSEND_H

#include <Wt/WDateTime>
#include <Wt/Dbo/Dbo>
#include <string>

namespace dbo = Wt::Dbo;

class SavedSend {
public:
    SavedSend();
    SavedSend(const SavedSend& orig);
    virtual ~SavedSend();
    
    std::string adress_sender;
    int         port;
    std::string number;
    std::string refenvoi;
    std::string code_ref;
     Wt::WDateTime deleteTag;

    template<class Action>
    void persist(Action& a)
    {
      dbo::field(a, adress_sender, "adress_sender");
      dbo::field(a, port,          "port");
      dbo::field(a, number,        "number");
      dbo::field(a, refenvoi,        "refenvoi");
      dbo::field(a, code_ref,        "code_ref");
      dbo::field(a, deleteTag,          "delete");
    }
    
private:

};

#endif	/* SAVEDSEND_H */
