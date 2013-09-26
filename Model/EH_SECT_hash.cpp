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
 * Filename: EH_SECT_hash.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */

#include "EH_SECT_hash.h"
#include "EH_SECT_sym.h"

typedef Elf32_Word EH_Hash;

EH_SECT_hash::EH_SECT_hash()
	:EH_Section()
{
	nbuckets = 0;
	nchains = 0;
	nMax = 0;
}

EH_SECT_hash::EH_SECT_hash( const char *data, uElf_Secthdr u )
	:EH_Section( data, u )
{
	nbuckets = ((EH_Hash*)section_data)[0];
	nchains = ((EH_Hash*)section_data)[1];
	nMax = nbuckets>nchains ? nbuckets : nchains;
}

EH_SECT_hash::~EH_SECT_hash()
{

}

EH_FieldInfo EH_SECT_hash::GetFieldInfo( QModelIndex &index ) const
{
	EH_FieldInfo fi = {0,0};

	if( !index.isValid() )
		return fi;
	
	if( (EH_index_t) index.row() >= nMax+2 || (EH_index_t)index.column() >= 2 )
		return fi;

	if( index.row() == 1 )
		return fi;
	
	if( index.row() == 0 )
	{
		fi.start = (EH_off_t)section_data - (EH_off_t)base + index.column() * sizeof( EH_Hash );
		fi.start += sizeof( EH_Hash ) * (index.row() );
		fi.size = sizeof( EH_Hash );
	}
	else
	{
		fi.start = (EH_off_t)section_data - (EH_off_t)base + index.column()*nbuckets*sizeof( EH_Hash );
		fi.start += sizeof( EH_Hash ) * (index.row() );
		fi.size = sizeof( EH_Hash );
	}
	return fi;
}

int EH_SECT_hash::rowCount( const QModelIndex &parent ) const
{
	(void)parent;
	return nMax + 2;
}

int EH_SECT_hash::columnCount( const QModelIndex &parent ) const
{
	(void)parent;
	return 2;
}

QVariant EH_SECT_hash::data( const QModelIndex &index, int role ) const
{
	QString temp;
	EH_Hash* arraystart = (EH_Hash*)section_data;

	if( !index.isValid() )
		return QVariant();

	if( (EH_index_t) index.row() >= nMax+2  || (EH_index_t) index.column() >= 2 )
		return QVariant();

	switch( role )
	{
		case Qt::DisplayRole:
			if( index.row() == 0 )
			{
				if( index.column() == 0 )
					return QString::number( nbuckets );
				else
					return QString::number( nchains );
			}
			else
			{
				if( index.row() > 1 )
				{
					if( index.column() == 0 && (unsigned)index.row()-2>=nbuckets )
						return QVariant();
					if( index.column() == 1 && (unsigned)index.row()-2>=nchains )
						return QVariant();

					arraystart += index.column()*nbuckets + index.row();

					//NOTICE
					//Choose whenever display name or symbol n° for chains
					if( index.column() == 0 )			
						temp = QString( EH_SECT_sym::GetSymbolName( is64bit ? hdr.shdr64->sh_link : hdr.shdr32->sh_link, *arraystart, base ) );
					else
						return QString::number( *arraystart ); // Decimal

					if( temp.length() < 1 )
						return QString( "<no name>" );
					return temp;
				}
			}
			break;
		case Qt::ToolTipRole:
		case Qt::StatusTipRole:
			if( index.row() > 1 )
			{
				if( index.column() == 0 && (unsigned)index.row()-2>=nbuckets )
					return QVariant();
				if( index.column() == 1 && (unsigned)index.row()-2>=nchains )
					return QVariant();

				arraystart += index.column()*nbuckets + index.row();
				return QString::number( *arraystart );
			}
			else
				return QVariant();
		default:
			return QVariant();
	}
	return QVariant();
}

QVariant EH_SECT_hash::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if( role == Qt::DisplayRole )
	{
		if( orientation == Qt::Horizontal )
			switch( section )
			{
				case 0:
					return QString( "Buckets" );
				case 1:
					return QString( "Chains" );
			}
		else
			if( section < 2 )
				return QVariant();
			else
				return QString::number( section-2 );
	}
	return QVariant();
}

Qt::ItemFlags EH_SECT_hash::flags( const QModelIndex &index ) const
{
	if( !index.isValid() )
		return Qt::ItemIsEnabled;
	
	Qt::ItemFlags temp = QAbstractItemModel::flags( index );
	return temp; //return temp | Qt::ItemIsEditable;
}

bool EH_SECT_hash::setData( const QModelIndex &index, const QVariant &value, int role )
{
	(void)index;
	(void)value;
	(void)role;
	return true;
}
