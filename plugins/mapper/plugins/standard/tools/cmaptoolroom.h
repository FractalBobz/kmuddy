/***************************************************************************
                               cmaptoolroom.h
                             -------------------
    begin                : Wed Aug 8 2001
    copyright            : (C) 2001 by Kmud Developer Team
    email                : kmud-devel@kmud.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CMAPTOOLROOM_H
#define CMAPTOOLROOM_H

#include <qobject.h>
#include <qpoint.h>
#include <kvbox.h>

#include "../../../cmaptoolbase.h"

/**This tool is used to create rooms
  *@author Kmud Developer Team
  */

class CMapToolRoom : public CMapToolBase
{
   Q_OBJECT
public: 
	CMapToolRoom(KActionCollection *actionCollection,CMapManager *manager,QObject *parent=0);
	~CMapToolRoom();

	/** Called when the tool recives a mouse release event */
	virtual void mouseReleaseEvent(QPoint mousePos,CMapLevel *currentLevel);
	/** Called when the tool recives a mouse move event */
	virtual void mouseMoveEvent(QPoint mousePos,Qt::ButtonState,CMapLevel *currentLevel);
	virtual void mouseLeaveEvent();
	virtual void mouseEnterEvent();

	/** This method is called when the active view changes */
	virtual void viewChangedEvent(CMapViewBase *view);

	/** This function called when a tool is selected */
	virtual void toolSelected(void);
	/** This function is called when a tool is unselected */
	virtual void toolUnselected(void);

private:
	QPoint lastPos;
};

#endif
