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
 * Filename: EH_File.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#include "EH_File.h"
#include <QDebug>

#include "libgen.h" // Used for 'basename' function
#include <fcntl.h>
#include <sys/mman.h>

EH_File::EH_File( bool autodelete )
{
	filename.clear();
	shortname.clear();
	patches.clear();
	sections.clear();
	segments.clear();
	
	data = NULL;
	is_open = false;
	read_only = false;
	fd = -1;
	delete_on_free = autodelete;
}

EH_File::EH_File( const std::string &path, bool rd_only, bool autodelete )
{
	filename.clear();
	shortname.clear();
	patches.clear();
	sections.clear();
	segments.clear();
	
	data = NULL;
	is_open = false;
	read_only = false;
	fd = -1;
	delete_on_free = autodelete;

	// Tries to open the file
	Open( path, rd_only );
}

EH_File::~EH_File()
{
	Close();
	filename.clear();
	shortname.clear();
	patches.clear();
	sections.clear();
	segments.clear();
}

bool EH_File::Exists() const
{	
	if( access( filename.c_str(), F_OK ) != -1 )
		return true;
	return false;
}

bool EH_File::Open( const std::string &path, bool rd_only )
{
	int open_flags = O_RDWR; // default to read+write
	
	// Sets the filename
	if( !SetFileName( path ) )
		return false; // return false on error
	
	read_only = rd_only; // Sets read-only flag
	
	// If the file exists and stat struct was filled...
	if( Exists() && stat( path.c_str(), &file_stat )==0 )
	{
		// Close the file if there wan any file opened
		if( is_open )
			Close();
		
		// Sets open syscal flags
		if( read_only )
			open_flags = O_RDONLY;

		// Open
		fd = open( path.c_str(), open_flags );

		if( fd == -1 )
		{
			// Try to open the file in read-only mode
			fd = open( path.c_str(), O_RDONLY );
			
			if( fd==-1)
			{
				is_open = false;
			}
			else
			{
				read_only = true;
				is_open = true;
			}
		}
		else
			is_open = true;
	}
	else
	{
		is_open = false;
	}
	
	return is_open;
}

char *EH_File::Map()
{
	if( !is_open )
		return NULL;
	
	if( data!=NULL )
		return data;
	
	data = (char *) mmap( NULL, file_stat.st_size, read_only?PROT_READ:PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0 );
	
	// If the map was successfull, we fill the file, program and section headers
	if( data != (char*) -1 )
	{
		fileheader.Set( data, file_stat.st_size );
		progheader.Set( data );
		sectheader.Set( data );
		return data;
	}
	else
		return NULL;
}

int EH_File::UnMap()
{
	if( is_open && data!=NULL )
	{
		return munmap( data, file_stat.st_size );
	}
	return -1;
}

bool EH_File::Close()
{
	if( is_open )
	{
		if( data != NULL )
		{
			if( munmap( data, file_stat.st_size ) == -1 )
				return false;
			
			data = NULL;
		}
		if( fd != -1 )
		{
			close( fd );
			is_open = false;
		}
	}
	else
		return false;
	
	return true;
}

bool EH_File::SetFileName( const std::string &path )
{
	char *temp_path = realpath( path.c_str(), NULL );
	
	if( temp_path==NULL )
		return false;
	
	filename = temp_path; // Implicit cast
	free( temp_path ); // See realpath man
	shortname = basename( (char *)filename.c_str() );
	return true;
}

std::string EH_File::GetFileName() const
{
	return filename;
}

std::string EH_File::GetShortName() const
{
	return shortname;
}

char *EH_File::GetData() const
{
	return data;
}

bool EH_File::IsReady() const
{
	return is_open;
}

bool EH_File::IsReadOnly() const
{
	return read_only;
}

EH_size_t EH_File::GetFileSize() const
{
	EH_size_t result = 0;
	
	if( is_open )
	{
		result = file_stat.st_size;
	}
	
	return result;
}

EH_FileHeader *EH_File::GetFileHeader()
{
	if( IsReady() )
	{
		return &fileheader;
	}
	else
		return NULL;
}

EH_ProgramHeader *EH_File::GetProgHeader()
{
	if( IsReady() )
	{
		return &progheader;
	}
	else
		return NULL;
}

EH_SectionHeader *EH_File::GetSectHeader()
{
	if( IsReady() )
	{
		return &sectheader;
	}
	else
		return NULL;
}

int EH_File::GetElfType( const char *data )
{
	try
	{
		// INFO: the e_type field preserves size and offset in 32 and 64 bits file
		return ((Elf32_Ehdr*)data)->e_type;
	}
	catch(...)
	{
		return ET_NONE;
	}
}

bool EH_File::CanDelete() const
{
	return delete_on_free;
}
