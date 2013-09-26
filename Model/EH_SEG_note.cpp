/*
 * This file is part of ElfHunter software.
 *
 * ElfHunter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ElfHunter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ElfHunter.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2012 Walter Tiberti
 */

/*
 * Filename: EH_SEG_note.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */

#include "EH_SEG_note.h"

namespace eh_nh
{
	const int fieldnum = 5;

	const char *names[] =
	{
		"n_namesz",
		"n_descsz",
		"n_type",
		"Name",
		"Description"
	};
};

EH_SEG_note::EH_SEG_note()
	: EH_Segment()
{
	nhdr.nhdr32 = NULL;
}

EH_SEG_note::EH_SEG_note( const char *data, uElf_Proghdr u )
	: EH_Segment( data, u )
{
	if( is64bit )
	{
		nhdr.nhdr64 = (Elf64_Nhdr *) segment_data;
	}
	else
	{
		nhdr.nhdr32 = (Elf32_Nhdr *) segment_data;
	}
}

EH_SEG_note::~EH_SEG_note()
{
	
}

int EH_SEG_note::rowCount( const QModelIndex &parent ) const
{
	(void)parent;
	return 1;
}

int EH_SEG_note::columnCount( const QModelIndex &parent ) const
{
	(void)parent;
	return eh_nh::fieldnum;
}

QVariant EH_SEG_note::data( const QModelIndex &index, int role ) const
{
	if( role == Qt::DisplayRole )
		switch( index.column() )
		{
			case 0:
				return QString::number( is64bit ? nhdr.nhdr64->n_namesz : nhdr.nhdr32->n_namesz );
			case 1: 
				return QString::number( is64bit ? nhdr.nhdr64->n_descsz : nhdr.nhdr32->n_descsz );
			case 2: 
				return QString::number( is64bit ? nhdr.nhdr64->n_type : nhdr.nhdr32->n_type );
			case 3:
				if( is64bit )
					return QString::fromUtf8( (char *) ((char *) &(nhdr.nhdr64->n_type)) + sizeof(nhdr.nhdr64->n_type), nhdr.nhdr64->n_namesz );
				else
					return QString::fromUtf8( (char *) ((char *) &(nhdr.nhdr32->n_type)) + sizeof(nhdr.nhdr32->n_type), nhdr.nhdr32->n_namesz );
			case 4:
				if( is64bit )
					return QString::fromUtf8( (char *) ((char *) &(nhdr.nhdr64->n_type)) + sizeof(nhdr.nhdr64->n_type) + nhdr.nhdr64->n_namesz, nhdr.nhdr64->n_descsz );
				else
					return QString::fromUtf8( (char *) ((char *) &(nhdr.nhdr32->n_type)) + sizeof(nhdr.nhdr32->n_type) + nhdr.nhdr32->n_namesz, nhdr.nhdr32->n_descsz );
			default:
				return QVariant();
		}
	else
		if( role == Qt::ToolTipRole || role == Qt::StatusTipRole )
		{
			switch( index.column() )
			{
			case 0:
				return "0x" + QString::number( is64bit ? nhdr.nhdr64->n_namesz : nhdr.nhdr32->n_namesz, 16 );
			case 1: 
				return "0x" + QString::number( is64bit ? nhdr.nhdr64->n_descsz : nhdr.nhdr32->n_descsz, 16 );
			case 2: 
				return "0x" + QString::number( is64bit ? nhdr.nhdr64->n_type : nhdr.nhdr32->n_type, 16 );
			default:
				return QVariant();
			};
		}

	return QVariant();
}

QVariant EH_SEG_note::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if( orientation == Qt::Horizontal && role == Qt::DisplayRole && (unsigned)section < eh_nh::fieldnum )
	{
		return QString( eh_nh::names[section] );
	}

	return QVariant();
}

Qt::ItemFlags EH_SEG_note::flags( const QModelIndex &index ) const
{
	if( !index.isValid() )
		return Qt::ItemIsEnabled;
	
	Qt::ItemFlags temp = QAbstractItemModel::flags( index );
	return temp; //return temp | Qt::ItemIsEditable;
}

bool EH_SEG_note::setData( const QModelIndex &index, const QVariant &value, int role )
{
	(void)index;
	(void)value;
	(void)role;
	return true;
}

EH_FieldInfo EH_SEG_note::GetFieldInfo( QModelIndex &index ) const
{
	EH_FieldInfo fi = {0,0};

	fi.start = is64bit ? hdr.phdr64->p_offset : hdr.phdr32->p_offset;
	switch( index.column() )
	{
		case 0:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Nhdr, n_namesz );
				fi.size += sizeof( Elf64_Nhdr::n_namesz );
			}
			else
			{
				fi.start += offsetof( Elf64_Nhdr, n_namesz );
				fi.size += sizeof( Elf32_Nhdr::n_namesz );
			}
			break;
		case 1:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Nhdr, n_descsz );
				fi.size += sizeof( Elf64_Nhdr::n_descsz );
			}
			else
			{
				fi.start += offsetof( Elf64_Nhdr, n_descsz );
				fi.size += sizeof( Elf32_Nhdr::n_descsz );
			}
			break;
		case 2:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Nhdr, n_type );
				fi.size += sizeof( Elf64_Nhdr::n_type );
			}
			else
			{
				fi.start += offsetof( Elf64_Nhdr, n_type );
				fi.size += sizeof( Elf32_Nhdr::n_type );
			}
			break;
		case 3:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Nhdr, n_type );
				fi.start += sizeof( Elf64_Nhdr::n_type );
				fi.size = nhdr.nhdr64->n_namesz;
			}
			else
			{
				fi.start += offsetof( Elf32_Nhdr, n_type );
				fi.start += sizeof( Elf32_Nhdr::n_type );
				fi.size = nhdr.nhdr32->n_namesz;
			}
			break;
		case 4:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Nhdr, n_type );
				fi.start += sizeof( Elf64_Nhdr::n_type ) + nhdr.nhdr64->n_namesz;
				fi.size = ((Elf64_Nhdr*)segment_data)->n_descsz;
			}
			else
			{
				fi.start += offsetof( Elf32_Nhdr, n_type );
				fi.start += sizeof( Elf32_Nhdr::n_type ) + nhdr.nhdr32->n_namesz;
				fi.size = nhdr.nhdr32->n_descsz;
			}
			break;
	}
	return fi;
}
