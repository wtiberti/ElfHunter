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
 * Filename: EH_SEG_interp.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */

#include "EH_SEG_interp.h"


EH_SEG_interp::EH_SEG_interp()
	: EH_Segment()
{
	interp_str = NULL;
}

EH_SEG_interp::EH_SEG_interp( const char *data, uElf_Proghdr u )
	: EH_Segment( data, u )
{
	interp_str = (char*) segment_data;
}

EH_SEG_interp::~EH_SEG_interp()
{
	
}

int EH_SEG_interp::rowCount( const QModelIndex &parent ) const
{
	(void)parent;
	return 1;
}

int EH_SEG_interp::columnCount( const QModelIndex &parent ) const
{
	(void)parent;
	return 1;
}

QVariant EH_SEG_interp::data( const QModelIndex &index, int role ) const
{
	(void)index;
	if( role == Qt::DisplayRole )
		return QString( (char*) segment_data );

	return QVariant();
}

QVariant EH_SEG_interp::headerData( int section, Qt::Orientation orientation, int role ) const
{
	(void)section;
	if( orientation == Qt::Vertical && role == Qt::DisplayRole )
		return QString( "Interpreter" );

	return QVariant();
}

Qt::ItemFlags EH_SEG_interp::flags( const QModelIndex &index ) const
{
	if( !index.isValid() )
		return Qt::ItemIsEnabled;
	
	Qt::ItemFlags temp = QAbstractItemModel::flags( index );
	return temp; //return temp | Qt::ItemIsEditable;
}

bool EH_SEG_interp::setData( const QModelIndex &index, const QVariant &value, int role )
{
	(void)index;
	(void)value;
	(void)role;
	return true;
} 

EH_FieldInfo EH_SEG_interp::GetFieldInfo( QModelIndex &index ) const
{
	EH_FieldInfo fi = {0,0};

	fi.start = is64bit ? hdr.phdr64->p_offset : hdr.phdr32->p_offset;
	fi.size = is64bit ? hdr.phdr64->p_filesz : hdr.phdr32->p_filesz;
	return fi;
}
