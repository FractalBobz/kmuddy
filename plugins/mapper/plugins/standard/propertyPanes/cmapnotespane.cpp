/***************************************************************************
                               cmapnotespane.cpp
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

#include "cmapnotespane.h"

#include "../cmappluginstandard.h"
#include "../cmapcmdnotes.h"
#include "../../../cmapmanager.h"

#include <klocale.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <QLabel>
#include <kvbox.h>

CMapNotesPane::CMapNotesPane(CMapPluginStandard *plugin,QString title,QIcon *icon,elementTyp panelType,CMapElement *element,QWidget *parent, const char *name)
	: CMapPropertiesPaneBase(title,icon,panelType,element,parent,name)
{
	m_plugin = plugin;
	m_element = element;
	notesLayout = new Q3VBoxLayout( this );
    notesLayout->setSpacing( 6 );
    notesLayout->setMargin( 11 );

	lblNotes = new QLabel( this, "lblNotes" );
    lblNotes->setText( i18n( "Notes:" ) );
    notesLayout->addWidget( lblNotes );

	txtNotes = new Q3MultiLineEdit( this, "txtNotes" );
	notesLayout->addWidget( txtNotes );

	txtNotes->setText(m_plugin->getNote(element));
}

CMapNotesPane::~CMapNotesPane()
{
}

/** This is called when the ok button of the property dialog is pressed */
void CMapNotesPane::slotOk()
{
	CMapCMDNotes *cmd = new CMapCMDNotes(m_plugin,m_element,txtNotes->text());
	m_plugin->getManager()->addCommand(cmd);
}

/** This is called when the cancel button of the property dialog is pressed */
void CMapNotesPane::slotCancel()
{
	
}
