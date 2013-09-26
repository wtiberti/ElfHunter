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
 * Filename: EH_SECT_str.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#include "EH_SECT_str.h"
#include <cstring>

EH_SECT_str::EH_SECT_str()
	: EH_Section()
{
	strbase = NULL;
	offsets.clear();
}

EH_SECT_str::EH_SECT_str( const char *data, uElf_Secthdr u )
	: EH_Section( data, u )
{
	EH_off_t cursor = 0;
	EH_off_t limit;

	strbase = NULL;
	offsets.clear();

	if( is64bit )
	{
		strbase = (char *) (data + u.shdr64->sh_offset );
		limit = u.shdr64->sh_size;
	}
	else
	{
		strbase = (char *) (data + u.shdr32->sh_offset );
		limit = u.shdr32->sh_size;
	}
	
	while( cursor < limit )
	{
		offsets.push_back( cursor );
		cursor += strlen( strbase + cursor ) + 1;
	}
}

EH_SECT_str::~EH_SECT_str()
{

}

EH_FieldInfo EH_SECT_str::GetFieldInfo( QModelIndex &index ) const
{
	EH_FieldInfo fi = {0,0};

	if( index.isValid() )
	{
		if( is64bit )
			fi.start = hdr.shdr64->sh_offset;
		else
			fi.start = hdr.shdr32->sh_offset;

		fi.start += offsets[ index.row() ];
		fi.size = strlen( (char *) strbase + offsets[ index.row() ] ) + 1;
	}
	return fi;
}

int EH_SECT_str::rowCount( const QModelIndex &parent ) const
{
	return offsets.size();
}

int EH_SECT_str::columnCount( const QModelIndex &parent ) const
{
	return 1;
}

QVariant EH_SECT_str::data( const QModelIndex &index, int role ) const
{
	if( index.isValid() )
	{
		if( role == Qt::DisplayRole )
		{
			if( (unsigned)index.row() < offsets.size() )
				return QString( strbase + offsets[ index.row() ] );
			else
				return QVariant();
		}
	}
	return QVariant();
}

QVariant EH_SECT_str::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if( role == Qt::DisplayRole )
	{
		if( orientation == Qt::Vertical )
		{
			if( (unsigned)section < offsets.size() )
				return "+ 0x" + QString::number( offsets[ section ], 16 );
			else
				return QVariant();
		}
		else
		{
			return "String";
		}
	}
	return QVariant();
}

Qt::ItemFlags EH_SECT_str::flags( const QModelIndex &index ) const
{
	if( !index.isValid() )
		return Qt::ItemIsEnabled;
	
	Qt::ItemFlags temp = QAbstractItemModel::flags( index );
	return temp; //return temp | Qt::ItemIsEditable;
}

bool EH_SECT_str::setData( const QModelIndex &index, const QVariant &value, int role )
{
	(void)index;
	(void)value;
	(void)role;
	return true;
}

