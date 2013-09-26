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
 * Filename: EH_MainWindow.h
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */

#ifndef EH_VIEW_MainWindow_H 
#define EH_VIEW_MainWindow_H

#include "../ui_ElfHunter.h"

/** @class EH_MainWindow
 * @brief Class for ElfHunter main window */
class EH_MainWindow : public QMainWindow
{
	Q_OBJECT
	
	private:
		Ui::EH_MainWindow ui_mainwindow; ///< QT window widget
		
<<<<<<< HEAD
		/** @brief Create a new @ref EH_FileView widget given a file path
		 * @param[in] filePath Path to the file
		 * @return true on success, false otherwise */
		bool CreateViewFromFilePath( QString filePath );
		
=======
>>>>>>> 50ab7cdea86c11b479220bd47c786af71240271f
	public:
		EH_MainWindow( QWidget *parent = NULL );

		/** @brief Add a tab to the tab widget
		 * @param[in] w Widget to insert into the tab
		 * @param[in] s Name to be displayed on tab
		 * @return The size of the tab list after insertion */
		int AddTab( QWidget *w, QString s );
		
	public slots:
		/** React on About action*/
		void on_actionAbout_triggered();

		/** React on Open action */
		void on_actionOpen_triggered();

		/** React on Close action */
		void on_actionClose_triggered();

		/** React on Exit action */
		void on_actionExit_triggered();

		/** React when a tab is being closed
		 * @param[in] index Index of the tab */
		void on_tabWidget_tabCloseRequested( int index );

		/** React when switching tabs
		 * @param[in] index Index of the new tab */
		void on_tabWidget_currentChanged( int index );
<<<<<<< HEAD

	protected:
		/** QT drag in event handler */
		void dragEnterEvent( QDragEnterEvent *dragEvent );

		/** QY drop event handle */
		void dropEvent( QDropEvent *dropEvent );
=======
>>>>>>> 50ab7cdea86c11b479220bd47c786af71240271f
};

#endif
