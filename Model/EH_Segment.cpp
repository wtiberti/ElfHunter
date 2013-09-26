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
 * Filename: EH_Segment.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#include "EH_FileHeader.h"
#include "EH_Segment.h"

EH_Segment::EH_Segment()
	: EH_Model()
{
	hdr.phdr32 = NULL;
	base = NULL;
	segment_data= NULL;
	is64bit = false;
}

EH_Segment::EH_Segment( const char *data, uElf_Proghdr u )
	: EH_Model()
{
	hdr.phdr32 = NULL;
	segment_data= NULL;
	is64bit = false;
	Set( data, u );
}

EH_Segment::~EH_Segment()
{
	
}

bool EH_Segment::Set( const char *base_data, uElf_Proghdr u )
{
	is64bit = EH_FileHeader::IsELF64( (void*)base_data )==1?true:false;

	base = (char*)base_data;

	if( is64bit )
	{
		hdr.phdr64 = u.phdr64;
		segment_data = (void*) (base + u.phdr64->p_offset);
	}
	else
	{
		hdr.phdr32 = u.phdr32;
		segment_data = (void*) (base + u.phdr32->p_offset);
	}
	
// 	qDebug() << "is64bit = " << is64bit;
// 	qDebug() << "segmentdata = " << (long)segment_data;
// 	qDebug() << "str = " << (char*)segment_data;
// 	qDebug() << "u32 = " << hdr.phdr32;
// 	qDebug() << "u64 = " << hdr.phdr64;
	return true;
}
