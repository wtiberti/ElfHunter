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
 * Filename: EH_AboutDlg.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#include "EH_AboutDlg.h"

EH_AboutDlg::EH_AboutDlg( QWidget *parent )
 :QDialog( parent )
{
	inner_widget = new QWidget();
	layout = new QVBoxLayout();
	
	ui_aboutdlg.setupUi( inner_widget );
	layout->addWidget( inner_widget );
	
	setLayout( layout );
}

EH_AboutDlg::~EH_AboutDlg()
{
	delete layout;
	delete inner_widget;
}