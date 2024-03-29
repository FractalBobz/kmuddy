/***************************************************************************
                               cmappluginstandard.cpp
                             -------------------
    begin                : Mon Aug 6 2001
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

#include "cmappluginstandard.h"

#include "tools/cmaptoolselect.h"
#include "tools/cmaptoolroom.h"
#include "tools/cmaptoolpath.h"
#include "tools/cmaptooltext.h"
#include "tools/cmaptoolzone.h"
#include "tools/cmaptooleraser.h"
#include "propertyPanes/cmapnotespane.h"

#include "../../cmapmanager.h"
#include "../../cmaplevel.h"
#include "../../cmapzone.h"
#include "../../cmaproom.h"
#include "../../cmaptext.h"
#include "../../cmappath.h"

#include <qicon.h>
//Added by qt3to4:
#include <Q3PtrList>

#include <kdebug.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kactioncollection.h>
#include <kpluginfactory.h>
#include <kpluginloader.h>

K_PLUGIN_FACTORY (KMuddyMapperStandardFactory, registerPlugin<CMapPluginStandard>();)
K_EXPORT_PLUGIN (KMuddyMapperStandardFactory("kmuddymapper"))


CMapPluginStandard::CMapPluginStandard(QObject *parent, const QVariantList &) : CMapPluginBase(parent)
{
        CMapManager *manager = dynamic_cast<CMapManager *>(parent);
	kDebug() << "CMapPluginStandard::CMapPluginStandard";
	kDebug() << "CMapPluginStandard::CMapPluginStandard Create Tools";

	// Create and Add the tools to the tools list
	toolList.append(new CMapToolSelect(actionCollection(),manager,parent));
	toolList.append(new CMapToolRoom(actionCollection(),manager,parent));
	toolList.append(new CMapToolPath(actionCollection(),manager,parent));
	toolList.append(new CMapToolText(actionCollection(),manager,parent));
	toolList.append(new CMapToolZone(actionCollection(),manager,parent));
	toolList.append(new CMapToolEraser(actionCollection(),manager,parent));
	kDebug() << "CMapPluginStandard::CMapPluginStandard Tools Created";

	actionCollection()->action("toolsEraser")->setEnabled(true);
	actionCollection()->action("toolsPath")->setEnabled(true);
	actionCollection()->action("toolsRoom")->setEnabled(true);
	actionCollection()->action("toolsSelect")->setEnabled(true);
	actionCollection()->action("toolsText")->setEnabled(true);
	actionCollection()->action("toolsZone")->setEnabled(true);


	m_elementList.setAutoDelete(false);	
	setXMLFile (KStandardDirs::locate("appdata", "kmuddymapper_standard.rc"));
}

CMapPluginStandard::~CMapPluginStandard()
{
}

/** Used to get a list of the property pages for a map element */
Q3PtrList<CMapPropertiesPaneBase> CMapPluginStandard::getPropertyPanes(elementTyp type,CMapElement *element,QWidget *parent)
{
	Q3PtrList<CMapPropertiesPaneBase> list;

	if (type == ROOM || type == ZONE)
	{
		list.append(new CMapNotesPane(this,i18n("Notes"),NULL,type,element,parent,"notesPane"));
	}

	return list;
}

/** This is called when the character or mud profiles change */
void CMapPluginStandard::profileChanged(void)
{
	actionCollection()->action("toolsEraser")->setEnabled(true);
	actionCollection()->action("toolsPath")->setEnabled(true);
	actionCollection()->action("toolsRoom")->setEnabled(true);
	actionCollection()->action("toolsSelect")->setEnabled(true);
	actionCollection()->action("toolsText")->setEnabled(true);
	actionCollection()->action("toolsZone")->setEnabled(true);
}

/**
 * This method is used to add a note or change a exsiting note
 * @param elemenet The element the note is for
 * @param note The next of the note
 */
void CMapPluginStandard::addNote(CMapElement *element,QString note)
{
	removeNote(element);

	if (note != "" )
	{
		m_elementList.append(element);
		m_noteList.append(note);
	}
}

/** This method is used to remove a note
  * @param element The note to remove
  */
void CMapPluginStandard::removeNote(CMapElement *element)
{
	int index = m_elementList.find(element);
	if (index!=-1)
	{
		m_elementList.remove(index);
		m_noteList.remove(m_noteList.at(index));
	}
}

/**
 * This method is used to get a note for the given element
 * @param element The element to get the note of
 * @return The note or empty string if there is no note
 */
QString CMapPluginStandard::getNote(CMapElement *element)
{
	int index = m_elementList.find(element);
	if (index!=-1)
	{	
		return m_noteList.at(index);
	}
	else
	{
		return "";
	}
}

/** This method is used to get a list of new properties for a element
  * It will usally be called when saving map data to file
  * @param element The element being saved
  * @param properties When method exits this should contain the new properties
  */
void CMapPluginStandard::saveElementProperties(CMapElement *element,KMemConfig *properties)
{
	QString note = getNote(element);

	if (note!="")
	{
		properties->group("Properties").writeEntry("Note",note);
	}
}

/** This method is used to update an element with the properties load from a file
  * It will usally be called when loading map data to file
  * @param element The element being loaded
  * @param properties The properties being loaded from the file
  */
void CMapPluginStandard::loadElementProperties(CMapElement *element,KMemConfig *properties)
{
	if (properties->group("Properties").hasKey("Note"))
	{
		QString note = properties->group("Properties").readEntry("Note","");
		addNote(element,note);
	}
}

/** This is called before a element is deleted
  * @param element The element about to be deleted */
void CMapPluginStandard::beforeElementDeleted(CMapElement *element)
{
	QString note = getNote(element);
	if (note!="")
	{
		DeletedElement e;
		e.type = (int)element->getElementType();
    
		if (element->getElementType() == ROOM)
		{
			e.id = ((CMapRoom *)element)->getRoomID();
			e.level = element->getLevel()->getLevelID();
			e.note = note;
			m_deletedElements.append(e);
		}
			
		if (element->getElementType() == ZONE)
		{
			e.id = ((CMapZone *)element)->getZoneID();
			e.note = note;
			m_deletedElements.append(e);
		}
	}
	
	removeNote(element);
}

/** This method is called after undoing a delete action
  * @param element The elemening being restored */
void CMapPluginStandard::afterElementUndeleted(CMapElement *element)
{
	DeletedElementList::iterator e;
    bool found = false;

	if (element->getElementType() == ROOM)
	{
		e = findRoom(element->getLevel()->getLevelID(),((CMapRoom*)element)->getRoomID(),&found);
	}

	if (element->getElementType()== ZONE)
	{
		e = findZone(((CMapZone*)element)->getZoneID(),&found);
	}
	
	if (found)
	{		
		addNote(element,(*e).note);

		m_deletedElements.remove(e);
	}
}

/**
 * This is called when the map is about to be loaded from file
 */
void CMapPluginStandard::loadAboutToStart()
{
	m_deletedElements.clear();
	m_noteList.clear();
	m_elementList.clear();
}

/**
 * This is called when the map is about to be saved to file
 */
void CMapPluginStandard::saveAboutToStart(void)
{
	m_deletedElements.clear();
}

/**
 * This is called when a new map is created
 */
void CMapPluginStandard::newMapCreated(void)
{
	m_deletedElements.clear();
	m_noteList.clear();
	m_elementList.clear();
}


CMapPluginStandard::DeletedElementList::iterator CMapPluginStandard::findRoom(int level,int id,bool *found)
{
	DeletedElementList::iterator it;
    for ( it = m_deletedElements.begin(); it != m_deletedElements.end(); ++it )
    {
		if ((*it).level == level && (*it).id == id)
		{
			*found = true;
			return it;
		}
    }
    
    return m_deletedElements.end();
}

CMapPluginStandard::DeletedElementList::iterator CMapPluginStandard::findZone(int id,bool *found)
{
	DeletedElementList::iterator it;
    for ( it = m_deletedElements.begin(); it != m_deletedElements.end(); ++it )
    {
		if ((*it).id == id)
		{
			*found = true;
			return it;
		}
    }
	
	return m_deletedElements.end();
}

