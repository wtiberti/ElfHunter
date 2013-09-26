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
 * Filename: EH_ElfHunter.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */

#include "EH_ElfHunter.h"

EH_ElfHunter::EH_ElfHunter()
{
	files.clear();
}

EH_ElfHunter::~EH_ElfHunter()
{
	files.clear();
}

int EH_ElfHunter::AddFile( EH_File *f )
{
	files.push_back( f );
	return files.size()-1;
}

EH_File *EH_ElfHunter::GetFile( const unsigned int index )
{
	if( index < files.size() )
		return files[index];

	// Invalid index
	return NULL;
}

bool EH_ElfHunter::RemFile( const unsigned int index )
{
	if( index < files.size() )
	{
		// Checks if the EH_File is allocated in stack or heap
		if( files[ index ]->CanDelete() )
			delete files[ index ]; // It's allocated in heap. Must be free'd down
		
		files.erase( files.begin()+index );
		return true;
	}

	// Invalid index
	return false;
}

unsigned int EH_ElfHunter::GetNumOfFiles()
{
	return files.size();
}
