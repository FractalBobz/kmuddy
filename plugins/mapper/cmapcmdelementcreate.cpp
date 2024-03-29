/***************************************************************************
                               cmapcmdelementcreate.cpp
                             -------------------
    begin                : Wed Feb 27 2002
    copyright            : (C) 2002 by Kmud Developer Team
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

#include "cmapcmdelementcreate.h"


#include "cmapelement.h"
#include "cmapmanager.h"
#include "cmaplevel.h"
#include "cmaproom.h"
#include "cmappath.h"

#include "cmapcmdelementproperties.h"

#include <klocale.h>
//Added by qt3to4:
#include <Q3PtrList>
#include <kvbox.h>

CMapCmdElementCreate::CMapCmdElementCreate(CMapManager *mapManager,QString name) : CMapCommand(name),CMapElementUtil(mapManager)
{
	manager = mapManager;
	properties = new KMemConfig();
	groups = 0;
	elements.setAutoDelete(false);
}

CMapCmdElementCreate::~CMapCmdElementCreate()
{
	delete properties;
}

void CMapCmdElementCreate::execute()
{
	QStringList groupList = properties->groupList();
	elements.clear();

	for (QStringList::Iterator it = groupList.begin(); it != groupList.end(); ++it)
	{
		if (*it != "<default>")
		{
			CMapElement *element = createElement(properties->group (*it));
			elements.append(element);
		}
	}
}

void CMapCmdElementCreate::unexecute()
{
	QStringList groupList = properties->groupList();

	for (QStringList::Iterator it = groupList.begin(); it != groupList.end(); ++it)
	{
		if (*it != "<default>")
		{
			deleteElement(properties->group(*it));
		}
	}	

	elements.clear();
}

void CMapCmdElementCreate::secondStage(void)
{
	bool active = manager->getUndoActive();
	manager->setUndoActive(true);

	for (CMapElement *element = elements.first();element!=NULL;element = elements.next())
	{
		if (element->getElementType()==PATH)
		{
			CMapPath *path = (CMapPath *)element;
			if (path->getTwoWayLater())
			{
				manager->makePathTwoWay(path);

				CMapCmdElementProperties *cmd = new CMapCmdElementProperties(manager,i18n("Set Element Properties"),path);
				cmd->setNewProperties(path->getTwoWayLaterProperties());
				manager->addCommand(cmd);
			}
		}
	}

	manager->setUndoActive(active);
}

void CMapCmdElementCreate::addElement(KMemConfig *newElementProperties,QString grp)
{
  KConfigGroup group = properties->group(QString::number(groups++));
	newElementProperties->group(grp).copyTo(&group);
}

Q3PtrList<CMapElement> *CMapCmdElementCreate::getElements(void)
{
	return &elements;
}
