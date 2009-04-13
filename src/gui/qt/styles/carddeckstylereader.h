/***************************************************************************
 *   Copyright (C) 2009 by Felix Hammer   *
 *   f.hammer@web.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CARDDECKSTYLEREADER_H
#define CARDDECKSTYLEREADER_H

#include "tinyxml.h"
#include "gametableimpl.h"
#include "configfile.h"
#include <string>
#include <QtCore>
#include <QtGui>

class CardDeckStyleReader : public QObject {
Q_OBJECT
public:
	CardDeckStyleReader(ConfigFile *c, gameTableImpl *w =0 );
	~CardDeckStyleReader();
	
	void readStyleFile(QString);

	QString getStyleDescription() const { return StyleDescription; }
	QString getStyleMaintainerName() const { return StyleMaintainerName; }
	QString getStyleMaintainerEMail() const	{ return StyleMaintainerEMail; }
	QString getStyleCreateDate() const { return StyleCreateDate; }

	QString getCurrentFileName() const { return currentFileName; }
	QString getCurrentDir() const { return currentDir; }
	
	QString getPreview() const { return Preview; }

	bool getFallBack() const { return fallBack; }	
	void showLeftItemsErrorMessage(QString, QStringList, QString);
	void showCardsLeftErrorMessage(QString, QStringList, QString);

private:

	QString StyleDescription;	
	QString StyleMaintainerName;
	QString StyleMaintainerEMail;
	QString StyleCreateDate;
	QString PokerTHStyleFileVersion;
	QString Preview;

	QString currentFileName;
	QString currentDir;

	QStringList cardsLeft;
	QStringList leftItems;

	ConfigFile *myConfig;
	gameTableImpl *myW;

	bool fallBack;
};

#endif