//
// C++ Interface: cgenericitem
//
// Description: A generic item that can only store data.
//
/*
Copyright 2008-2011 Tomas Mecir <kmuddy@kmuddy.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of 
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CGENERICITEM_H
#define CGENERICITEM_H

#include <clistobject.h>

class cGenericItem : public cListObject {
public:
  virtual ~cGenericItem ();

protected:
  friend class cGenericList;
  cGenericItem (cList *list);
};

#endif  // CGENERICITEM_H
