/***************************************************************************
                          dlgmaptextproperties.cpp  -  description
                             -------------------
    begin                : Thu Mar 8 2001
    copyright            : (C) 2001 by KMud Development Team
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

#include "dlgmaptextproperties.h"

#include <qfile.h>
#include <q3listbox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <q3multilineedit.h>
#include <q3textstream.h>
#include <qpalette.h>
#include <qfontdatabase.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qfontmetrics.h>
//Added by qt3to4:
#include <QPixmap>
#include <Q3PtrList>
#include <Q3VBoxLayout>

#include <kcolorbutton.h>
#include <knuminput.h>
#include <klocale.h>

#include "../cmaptext.h"
#include "../cmapmanager.h"
#include "../cmapcmdelementcreate.h"
#include "../cmapcmdelementdelete.h"
#include "../cmapcmdelementproperties.h"
#include "../cmapcmdgroup.h"

#include "../cmappluginbase.h"
#include "../cmappropertiespanebase.h"
#include "../cmapviewbase.h"

#include <kdebug.h>

CMapTextPreview::CMapTextPreview(CMapManager *manager,QWidget *parent, const char *name)
	: Q3ScrollView(parent,name, Qt::WNorthWestGravity | Qt::WResizeNoErase | Qt::WRepaintNoErase)
{
	buffer = NULL;
	mapManager = manager;

	setHScrollBarMode(Auto);
	setVScrollBarMode(Auto);
}

CMapTextPreview::~CMapTextPreview()
{
	if (buffer)
		delete buffer;
}

void CMapTextPreview::drawContents(QPainter *paint,int , int , int, int )
{
	int width,height;

	if (contentsWidth()>viewport()->width())
		width = contentsWidth();
	else
		width = viewport()->width();

	if (contentsHeight()>viewport()->height())
		height = contentsHeight();
	else
		height = viewport()->height();

	QRect drawArea(0,0,width,height);
	
	// delete the buffer only when we need one with a different size
	if (buffer && (buffer->size() != drawArea.size()))
	{
		delete buffer;
		buffer = NULL;
	}

	if (!buffer)
	{
		buffer = new QPixmap(drawArea.size());
	}

	QPainter p;

	p.begin(buffer);

	if (mapManager->getActiveView()->getCurrentlyViewedZone()->getUseDefaultBackground())
	{
		p.fillRect(drawArea,mapManager->getMapData()->backgroundColor);
	}
	else
	{
		p.fillRect(drawArea,mapManager->getActiveView()->getCurrentlyViewedZone()->getBackgroundColor());
	}

	QStringList textList;
	CMapText::stringToList(text,&textList);
	CMapText::paintText(&p,color,QPoint(0,0),font,&textList,size);

	paint->drawPixmap(0,0,*buffer);
}


DlgMapTextProperties::DlgMapTextProperties(CMapManager *manager,CMapText *textElement,QWidget *parent, const char *name ) : DlgMapTextPropertiesBase(parent,name,true)
{
	text = textElement;
	mapManager = manager;
	QString width;
	QString height;
	Q3VBoxLayout *vbox = new Q3VBoxLayout((QWidget *)fraPreview);
	textScrollView = new CMapTextPreview(mapManager,fraPreview,"textPreview");
	vbox->addWidget( textScrollView);
	textScrollView->show();
	fillFamilyList();
	setFont(text->getFont());
	width.sprintf("%d",text->getWidth());
	height.sprintf("%d",text->getHeight());
	txtText->setText(text->getText());
	txtWidth->setText(width);
	txtHeight->setText(height);
	cmdColor->setColor(text->getColor());

	//FIXME_jp: set txtText background to background color of the map

	// Get the extension panels from the plugins
	for (CMapPluginBase *plugin=mapManager->getPluginList()->first();
	     plugin!=0;
	     plugin=mapManager->getPluginList()->next())
	{
		Q3PtrList<CMapPropertiesPaneBase> paneList = plugin->getPropertyPanes(TEXT,(CMapElement*)textElement,(QWidget *)TextTabs);
		for (CMapPropertiesPaneBase *pane = paneList.first();pane!=0;pane = paneList.next())
		{
			TextTabs->addTab(pane,pane->getTitle());
			connect(cmdOk,SIGNAL(clicked()),pane,SLOT(slotOk()));
			connect(cmdCancel,SIGNAL(clicked()),pane,SLOT(slotCancel()));
		}
	}

	slotUpdatePreview();
}

DlgMapTextProperties::~DlgMapTextProperties()
{
}

void DlgMapTextProperties::fillFamilyList(void)
{
  lstFamily->insertStringList (QFontDatabase().families());
}

void DlgMapTextProperties::setFont(QFont font)
{
	unsigned int i;

	textFont = font;
	
	QString family = font.family();
	family = family.trimmed();
	QString size;
	size.sprintf("%d",font.pointSize());	
	
	for (i = 0 ; i<lstFamily->count();i++)
	{
		QString s = lstFamily->text(i);
		if (s == family)
		{
			lstFamily->setCurrentItem(i);
			break;
		}
	}	

	chkBold->setChecked(font.bold());
	chkItalic->setChecked(font.italic());
	lstFamily->centerCurrentItem();	
	slotUpdatePreview();
}

void DlgMapTextProperties::slotSetSize(void)
{
	kDebug() << "CMapTextPreview::slotSetSize1 ";
	int fontSize = txtFontSize->text().toInt();
	textFont.setPointSize(fontSize);
	QFontMetrics fm(textFont);	
	QString width;
	QString height;
	QStringList textList;
	CMapText::stringToList(txtText->text(),&textList);
	int tmpWidth = 0;
	for (QStringList::iterator it = textList.begin(); it != textList.end(); ++it)
	{
		if (fm.width(*it) > tmpWidth)
			tmpWidth = fm.width(*it);
    }

	width.sprintf("%d",tmpWidth);
	height.sprintf("%d",fm.height() * textList.count());
	txtWidth->setText(width);
	txtHeight->setText(height);
	txtFontSize->setText("");
	slotUpdatePreview();
}

void DlgMapTextProperties::slotBoldClicked(void)
{
	textFont.setBold(chkBold->isChecked());
	slotUpdatePreview();
}

void DlgMapTextProperties::slotItalicClicked(void)
{
	textFont.setItalic(chkItalic->isChecked());
	slotUpdatePreview();
}

void DlgMapTextProperties::slotFamilySelected(int index)						
{
	textFont.setFamily(lstFamily->text(index));
	slotUpdatePreview();
}

void DlgMapTextProperties::slotColorChanged(const QColor &color)
{
	textColor = color;
	slotUpdatePreview();
}

void DlgMapTextProperties::slotAccept()
{
	CMapCmdElementProperties *command = new CMapCmdElementProperties(mapManager,i18n("Changed Room Properties"),text);

	QStringList textList;
	int width = txtWidth->text().toInt();
	int height = txtHeight->text().toInt();

	command->compare("Text",text->getText(),txtText->text());
	command->compare("Color",text->getColor(),textColor);
	command->compare("Font",text->getFont(),textFont);
	command->compare("Size",text->getSize(),QSize(width,height));

	mapManager->addCommand(command);

	accept();
}

void DlgMapTextProperties::slotUpdatePreview()
{
	int gridWidth = mapManager->getMapData()->gridSize.width();
	int gridHeight = mapManager->getMapData()->gridSize.height();
	int width =txtWidth->text().toInt();
	if (width<gridWidth)
		width = gridWidth;
	int height = txtHeight->text().toInt();
	if (height < gridHeight)
		height = 20;

	textScrollView->setFont(textFont);
	textScrollView->setColor(textColor);
	textScrollView->setSize(QSize(width,height));
	textScrollView->setText(txtText->text());
	textScrollView->resizeContents(txtWidth->text().toInt(),txtHeight->text().toInt());
	textScrollView->viewport()->repaint(false);
}
