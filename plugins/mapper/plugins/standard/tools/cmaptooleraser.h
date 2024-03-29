/***************************************************************************
                               cmaperasertool.h
                             -------------------
    begin                : Thu Aug 9 2001
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

#ifndef CMAPTOOLERASER_H
#define CMAPTOOLERASER_H

#include <qobject.h>
#include <qcursor.h>
#include <kvbox.h>

#include "../../../cmaptoolbase.h"

class CMapViewBase;

/**This tool is used to delete map elements
  *@author Kmud Developer Team
  */

class CMapToolEraser : public CMapToolBase
{
   Q_OBJECT
public: 
	CMapToolEraser(KActionCollection *actionCollection,CMapManager *manager,QObject *parent=0);
	~CMapToolEraser();

	/** Called when the tool recives a mouse release event */
	virtual void mouseReleaseEvent(QPoint mousePos,CMapLevel *currentLevel);

	/** This function called when a tool is selected */
	virtual void toolSelected(void);
	/** This function is called when a tool is unselected */
	virtual void toolUnselected(void);

	/** This method is called when the active view changes */
	virtual void viewChangedEvent(CMapViewBase *view);

private:
	/** The mouse cursor */
	QCursor *deleteCursor;

};

#endif
