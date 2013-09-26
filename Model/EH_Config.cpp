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
 * Filename: EH_Config.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#include "EH_Config.h"

EH_Config::EH_Config()
{
	conf_file = NULL;
	eh_window_height = 600;
	eh_window_width = 800;
	recent_files.clear();
}

EH_Config::EH_Config( const char *conf_filename )
{
	conf_file = NULL;
	eh_window_height = 600;
	eh_window_width = 800;
	recent_files.clear();

	Load( conf_filename );
}

bool EH_Config::Load( const char *conf_filename )
{
	//TODO
	(void)conf_filename;
	return false;
}

bool EH_Config::Write()
{
	//TODO
	return false;
}

void EH_Config::SetWindowHeight( unsigned h )
{
	eh_window_height = h;
}

unsigned int EH_Config::GetWindowHeight()
{
	return eh_window_height;
}

void EH_Config::SetWindowWidth( unsigned w )
{
	eh_window_width = w;
}

unsigned int EH_Config::GetWindowWidth()
{
	return eh_window_width;
}

QString EH_Config::GetRecentFile( unsigned index )
{
	if( index < 5 ) 
		return recent_files.at( index );

	return QString();
}

bool EH_Config::PushRecentFile( QString filename )
{
	if( filename != "" )
	{
		recent_files.push( filename );
		return true;
	}
	return false;
}

