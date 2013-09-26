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
 * Filename: EH_Section.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#include "EH_FileHeader.h"
#include "EH_Section.h"

EH_Section::EH_Section()
	: EH_Model()
{
	hdr.shdr32 = NULL;
	base = NULL;
	section_data= NULL;
	is64bit = false;
}

EH_Section::EH_Section( const char *data, uElf_Secthdr u )
	: EH_Model()
{
	hdr.shdr32 = NULL;
	section_data= NULL;
	is64bit = false;
	Set( data, u );
}

EH_Section::~EH_Section()
{
	
}

bool EH_Section::Set( const char *base_data, uElf_Secthdr u )
{
	is64bit = EH_FileHeader::IsELF64( (void*)base_data )==1?true:false;

	base = (char*)base_data;

	if( is64bit )
	{
		hdr.shdr64 = u.shdr64;
		section_data = (void*) ( base + u.shdr64->sh_offset );
	}
	else
	{
		hdr.shdr32 = u.shdr32;
		section_data = (void*) ( base + u.shdr32->sh_offset );
	}
	return true;
}
