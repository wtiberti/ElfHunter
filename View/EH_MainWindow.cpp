/*
 * This file is part of ElfHunter software.
 *
 * ElfHunter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ElfHunter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ElfHunter.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2012 Walter Tiberti
 */

/*
 * Filename: EH_MainWindow.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */

#include <QtCore>

#include "EH_MainWindow.h"
#include "EH_FileView.h"
#include "EH_AboutDlg.h"

#include "../Model/EH_ElfHunter.h"

extern EH_ElfHunter eh;

EH_MainWindow::EH_MainWindow( QWidget *parent )
	:QMainWindow( parent )
{
	ui_mainwindow.setupUi( this );
}

void EH_MainWindow::dragEnterEvent( QDragEnterEvent *dragEvent )
{
	if( dragEvent->mimeData()->hasFormat( "text/uri-list" ) )
		dragEvent->acceptProposedAction();
}

void EH_MainWindow::dropEvent( QDropEvent *dropEvent )
{
	const QMimeData* mimeData = dropEvent->mimeData();

	if( mimeData->hasUrls() )
	{
		QList<QUrl> urlList = mimeData->urls();

		for( int i=0; i<urlList.size(); i++ )
		{
			CreateViewFromFilePath( urlList.at(i).toLocalFile() );
		}
	}
}

int EH_MainWindow::AddTab( QWidget *w, QString s )
{
	ui_mainwindow.tabWidget->addTab( w, s );
	return ui_mainwindow.tabWidget->count();
}

void EH_MainWindow::on_actionAbout_triggered()
{
	EH_AboutDlg eh_aboutdlg( this );
	eh_aboutdlg.exec();
}

void EH_MainWindow::on_actionOpen_triggered()
{
	QString chosen_file = QFileDialog::getOpenFileName( this, "Open", "", "" );
	CreateViewFromFilePath( chosen_file );
}

bool EH_MainWindow::CreateViewFromFilePath( QString filePath )
{
	if( filePath != "" )
	{
		//TODO sola lettura (2° parametro)
		EH_File *temp = new EH_File( filePath.toStdString().c_str(), false, true );
		if( temp->IsReady() )
		{
			temp->Map();
			eh.AddFile( temp );
			EH_FileView *fv = new EH_FileView( this );
			fv->SetFile( temp );
			AddTab( (QWidget*)fv , QString(temp->GetShortName().c_str()) );
			return true;
		}
		else
		{
			delete temp;
			return false;
		}
	}
	return false;
}

void EH_MainWindow::on_tabWidget_tabCloseRequested( int index )
{
	if( index >= 0 && index < ui_mainwindow.tabWidget->count() )
	{
		delete ui_mainwindow.tabWidget->widget( index );
		
		if( ui_mainwindow.tabWidget->count() <= 0 )
		{
			ui_mainwindow.actionSave->setDisabled( true );
			ui_mainwindow.actionReload->setDisabled( true );
			ui_mainwindow.actionSave_as->setDisabled( true );
			ui_mainwindow.actionClose->setDisabled( true );
			ui_mainwindow.actionClose_All->setDisabled( true );
			ui_mainwindow.actionPatchlist->setDisabled( true );
			ui_mainwindow.actionFunction_List->setDisabled( true );
			//TODO
		}
	}
}

void EH_MainWindow::on_tabWidget_currentChanged( int index )
{
	EH_FileView *fv = (EH_FileView *) (ui_mainwindow.tabWidget->widget( index ) );
	
	if( fv != NULL )
	{
		ui_mainwindow.actionSave->setDisabled( false );
		ui_mainwindow.actionReload->setDisabled( false );
		ui_mainwindow.actionSave_as->setDisabled( false );
		ui_mainwindow.actionClose->setDisabled( false );
		ui_mainwindow.actionClose_All->setDisabled( false );
		ui_mainwindow.actionPatchlist->setDisabled( false );
		ui_mainwindow.actionFunction_List->setDisabled( false );
		//TODO
	}
}

void EH_MainWindow::on_actionClose_triggered()
{
	on_tabWidget_tabCloseRequested( ui_mainwindow.tabWidget->currentIndex() );
}

void EH_MainWindow::on_actionExit_triggered()
{
	// Clean?
	qApp->quit();
}
