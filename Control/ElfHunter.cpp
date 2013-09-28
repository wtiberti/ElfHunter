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
 * Filename: ElfHunter.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */

#include <QtGui>
#include "../View/EH_MainWindow.h"
#include "../View/EH_FileView.h"
#include "../Model/EH_File.h"

#include "../Model/EH_ElfHunter.h"
#include "ElfHunter.h"
#include <cstdio>

/** @brief Main function */
int main( int argc, char *argv[] )
{
	int final_result; ///< returning value
	int next_opt = 0; ///< temp storage for command line option
	
	QApplication eh_app( argc, argv ); ///< QT QApplication Object
	
	do
	{
		// Getting next option
		next_opt = getopt_long( argc, argv, "h", (option*)&eh_cmdline_opt, NULL );
		
		switch( next_opt )
		{
			case 'h':
			case '?':
				EH_Main_PrintHelp();
				return 0;
		}
	} while( next_opt != -1 );
	

	// If any filename specified, ElfHunter tries to open and show it
	// Creating EH_File objects....
	for( int i=optind; i<argc; i++ )
	{
		EH_File *temp = new EH_File( argv[ i ], false, true );
		if( temp->IsReady() )
		{
			eh.AddFile( temp );
		}
		else
		{
			EH_ElfHunter::DebugOutput( "File not found!", EHDBG_ERROR );
			delete temp;
		}
	}

	EH_MainWindow eh_window; ///< Main QT window
	
	// Creating a EH_FileView onject for each file...
	for( unsigned int i=0; i<eh.GetNumOfFiles(); i++ )
	{
		EH_FileView *fv = new EH_FileView( &eh_window );
		eh.GetFile(i)->Map();
		fv->SetFile( eh.GetFile(i) );
		eh_window.AddTab( (QWidget*)fv , QString(eh.GetFile(i)->GetShortName().c_str()) );
	}
	
	eh_window.show();
	
	final_result = eh_app.exec();
	return final_result;
}

void EH_Main_PrintHelp()
{
	puts( "\x1b[0;92m[ELFHUNTER]\x1b[0;0m\n"
			"Command line syntax:\n"
			"\t\x1b[0;93mElfHunter [options] [files...]\x1b[0;0m\n"
			"Options:\n"
			" \x1b[0;92m"
				"-h"
			"\x1b[0;0m\tShow this help\n" );
}
