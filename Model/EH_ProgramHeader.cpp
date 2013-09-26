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
 * Filename: EH_ProgramHeader.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#include "EH_FileHeader.h"
#include "EH_ProgramHeader.h"

/** Namespace for EH_ProgramHeader class */
namespace eh_ph
{
	const int fieldnum = 8;
	
	//NOTE: in ELf64_Phdr fields p_flags change position
	const char *names[] =
	{
		"p_type",
		"p_offset",
		"p_vaddr",
		"p_paddr",
		"p_filesz",
		"p_memsiz",
		"p_flags",
		"p_align"
	};
};

EH_ProgramHeader::EH_ProgramHeader()
: EH_Model()
{
	hdr.phdr32 = NULL;
	is64bit = false;
}

EH_ProgramHeader::EH_ProgramHeader( void *data )
: EH_Model()
{
	hdr.phdr32 = NULL;
	is64bit = false;
	Set( data );
}

EH_ProgramHeader::~EH_ProgramHeader()
{
	
}

int EH_ProgramHeader::rowCount( const QModelIndex &parent ) const
{
	(void)parent;
	return nHeaders;
}

int EH_ProgramHeader::columnCount( const QModelIndex &parent ) const
{
	(void)parent;
	return eh_ph::fieldnum;
}

QVariant EH_ProgramHeader::data( const QModelIndex &index, int role ) const
{
	if( !index.isValid() )
		return QVariant();
	
	if( (EH_index_t) index.row() >= nHeaders || (EH_index_t)index.column() >= eh_ph::fieldnum )
		return QVariant();
	
	uElf_Proghdr current_header = hdr;
	
	if( is64bit )
	{
		current_header.phdr64 += index.row();
	}
	else
	{
		current_header.phdr32 += index.row();
	}
	
	QString temp;
	EH_index_t value;
	
	switch( role )
	{
		case Qt::DisplayRole:
			switch( index.column() )
			{
			case 0:
				value = (is64bit) ? current_header.phdr64->p_type : current_header.phdr32->p_type;
				switch( value )
				{
					case PT_NULL:
						temp = "PT_NULL";
						break;
					case PT_LOAD:
						temp = "PT_LOAD";
						break;
					case PT_DYNAMIC:
						temp = "PT_DYNAMIC";
						break;
					case PT_INTERP:
						temp = "PT_INTERP";
						break;
					case PT_NOTE:
						temp = "PT_NOTE";
						break;
					case PT_SHLIB:
						temp = "PT_SHLIB";
						break;
					case PT_PHDR:
						temp = "PT_PHDR";
						break;
					case PT_GNU_EH_FRAME:
						temp = "PT_GNU_EH_FRAME";
						break;
					case PT_GNU_STACK:
						temp = "PT_GNU_STACK";
						break;
					default:
						if( value >= PT_LOOS && value <= PT_HIOS )
							temp = "OS-specific  0x" + QString::number( value, 16 );
						else
							if( value >= PT_LOPROC && value <= PT_HIPROC )
								temp = "Cpu-spec  0x" + QString::number( value, 16 );
							else
								temp = "N/A";
						break;
				}
				break;
			case 1:
				value = (is64bit) ? current_header.phdr64->p_offset : current_header.phdr32->p_offset;
				temp = "0x" + QString::number( value, 16 );
				break;
			case 2:
				value = (is64bit) ? current_header.phdr64->p_vaddr : current_header.phdr32->p_vaddr;
				temp = "0x" + QString::number( value, 16 );
				break;
			case 3:
				value = (is64bit) ? current_header.phdr64->p_paddr : current_header.phdr32->p_vaddr;
				temp = "0x" + QString::number( value, 16 );
				break;
			case 4:
				value = (is64bit) ? current_header.phdr64->p_filesz : current_header.phdr32->p_filesz;
				temp = "0x" + QString::number( value, 16 );
				break;
			case 5:
				value = (is64bit) ? current_header.phdr64->p_memsz : current_header.phdr32->p_memsz;
				temp = "0x" + QString::number( value, 16 );
				break;
			case 6:
				value = (is64bit) ? current_header.phdr64->p_flags : current_header.phdr32->p_flags;
				temp = "";
				if( value & PF_MASKOS )
					temp.append( "OS-Specific\n" );
				if( value & PF_MASKPROC )
					temp.append( "CPU-Specific\n" );
				if( value & PF_R )
					temp.append( "R" );
				if( value & PF_W )
					temp.append( "W" );
				if( value & PF_X )
					temp.append( "X" );
				break;
			case 7:
				value = (is64bit) ? current_header.phdr64->p_align : current_header.phdr32->p_align;
				temp = "0x" + QString::number( value, 16 );
				break;
			}
			return temp;
			break;

		case Qt::ToolTipRole:
		case Qt::StatusTipRole:
			switch( index.column() )
			{
				case 0:
					value = (is64bit) ? current_header.phdr64->p_type : current_header.phdr32->p_type;
					break;
				case 1:
					value = (is64bit) ? current_header.phdr64->p_offset : current_header.phdr32->p_offset;
					break;
				case 2:
					value = (is64bit) ? current_header.phdr64->p_vaddr : current_header.phdr32->p_vaddr;
					break;
				case 3:
					value = (is64bit) ? current_header.phdr64->p_paddr : current_header.phdr32->p_paddr;
					break;
				case 4:
					value = (is64bit) ? current_header.phdr64->p_filesz : current_header.phdr32->p_filesz;
					break;
				case 5:
					value = (is64bit) ? current_header.phdr64->p_memsz : current_header.phdr32->p_memsz;
					break;
				case 6:
					value = (is64bit) ? current_header.phdr64->p_flags : current_header.phdr32->p_flags;
					break;
				case 7:
					value = (is64bit) ? current_header.phdr64->p_align : current_header.phdr32->p_align;
					break;
				default:
					value = 0;
			}
			return QString::number( value );
			break;
	}
	return QVariant();
}

QVariant EH_ProgramHeader::headerData ( int section, Qt::Orientation orientation, int role ) const
{
	if( role != Qt::DisplayRole )
		return QVariant();
	
	if( orientation != Qt::Vertical )
	{
		if( section < eh_ph::fieldnum )
			return QString( eh_ph::names[section] );
	}
	else
	{
		if( (unsigned)section < nHeaders )
			return QString::number( section );
	}
	return QVariant();
}

Qt::ItemFlags EH_ProgramHeader::flags( const QModelIndex &index ) const
{
	if( !index.isValid() )
		return Qt::ItemIsEnabled;
	
	return QAbstractItemModel::flags( index );//Qt::ItemIsEditable | QAbstractItemModel::flags( index );
}

bool EH_ProgramHeader::setData( const QModelIndex &index, const QVariant &value, int role )
{
	(void)index;
	(void)value;
	(void)role;
	return true;
}

bool EH_ProgramHeader::Set( void *data )
{
	base_offset = EH_FileHeader::GetPhdrOffset( data );
	hdr.phdr32 = (Elf32_Phdr *) ( (char *)data + base_offset );
	is64bit =  EH_FileHeader::IsELF64( data )==1?true:false;
	nHeaders = EH_FileHeader::GetPhdrNum( data );
	return true;
}

EH_size_t EH_ProgramHeader::GetNumOfSegments() const
{
	return nHeaders;
}

void *EH_ProgramHeader::GetSegment( EH_index_t index )
{
	if( index >= nHeaders )
		return NULL;
	
	if( is64bit )
		return (void*)(hdr.phdr64 + index);
	
	return (void*)(hdr.phdr32 + index);
}

EH_FieldInfo EH_ProgramHeader::GetFieldInfo( QModelIndex &index ) const
{
	EH_FieldInfo fi = {0,0};

	if( !index.isValid() )
		return fi;
	
	if( (EH_index_t) index.row() >= nHeaders || (EH_index_t)index.column() >= eh_ph::fieldnum )
		return fi;
	
	fi.start = base_offset;
	
	if( is64bit )
		fi.start += sizeof(Elf64_Phdr)*index.row();
	else
		fi.start += sizeof(Elf32_Phdr)*index.row();

	switch( index.column() )
	{ 
		case 0:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Phdr, p_type );
				fi.size = sizeof( Elf64_Phdr::p_type );
			}
			else
			{
				fi.start += offsetof( Elf32_Phdr, p_type );
				fi.size = sizeof( Elf32_Phdr::p_type );
			}
			break;
		case 1:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Phdr, p_offset );
				fi.size = sizeof( Elf64_Phdr::p_offset );
			}
			else
			{
				fi.start += offsetof( Elf32_Phdr, p_offset );
				fi.size = sizeof( Elf32_Phdr::p_offset );
			}
			break;
		case 2:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Phdr, p_vaddr );
				fi.size = sizeof( Elf64_Phdr::p_vaddr );
			}
			else
			{
				fi.start += offsetof( Elf32_Phdr, p_vaddr );
				fi.size = sizeof( Elf32_Phdr::p_vaddr );
			}
			break;
		case 3:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Phdr, p_paddr );
				fi.size = sizeof( Elf64_Phdr::p_paddr );
			}
			else
			{
				fi.start += offsetof( Elf32_Phdr, p_paddr );
				fi.size = sizeof( Elf32_Phdr::p_paddr );
			}
			break;
		case 4:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Phdr, p_filesz );
				fi.size = sizeof( Elf64_Phdr::p_filesz );
			}
			else
			{
				fi.start += offsetof( Elf32_Phdr, p_filesz );
				fi.size = sizeof( Elf32_Phdr::p_filesz );
			}
			break;
		case 5:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Phdr, p_memsz );
				fi.size = sizeof( Elf64_Phdr::p_memsz );
			}
			else
			{
				fi.start += offsetof( Elf32_Phdr, p_memsz );
				fi.size = sizeof( Elf32_Phdr::p_memsz );
			}
			break;
		case 6:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Phdr, p_flags );
				fi.size = sizeof( Elf64_Phdr::p_flags );
			}
			else
			{
				fi.start += offsetof( Elf32_Phdr, p_flags );
				fi.size = sizeof( Elf32_Phdr::p_flags );
			}
			break;
		case 7:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Phdr, p_align );
				fi.size = sizeof( Elf64_Phdr::p_align );
			}
			else
			{
				fi.start += offsetof( Elf32_Phdr, p_align );
				fi.size = sizeof( Elf32_Phdr::p_align );
			}
			break;
	}
	return fi;
}
