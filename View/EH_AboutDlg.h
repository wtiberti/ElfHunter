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
 * Filename: EH_AboutDlg.h
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#ifndef EH_VIEW_AboutDlg_H 
#define EH_VIEW_AboutDlg_H

#include "../ui_EH_AboutDlg.h"
#include <QtGui>

/** @class EH_AboutDlg
 * @brief Class for the "About" dialog box */
class EH_AboutDlg : public QDialog
{
	Q_OBJECT
	
	private:
		Ui::EH_AboutDlg ui_aboutdlg; ///< QT about dialog box
		QWidget *inner_widget; ///< widget inside the about dialog box
		QVBoxLayout *layout; ///< layout of the about dialog box
		
	public:
		EH_AboutDlg( QWidget *parent = NULL );
		~EH_AboutDlg();
};

#endif