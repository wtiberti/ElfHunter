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
 * Filename: EH_SECT_rel.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */

#include "EH_SECT_rel.h"

namespace eh_rel
{
	const int fieldnum = 3;

	const char *names[] =
	{
		"r_offset",
		"r_info(sym)",
		"r_info(type)"
	};
};

EH_SECT_rel::EH_SECT_rel()
	:EH_Section()
{
	rel.rel32 = NULL;
	nRelocs = 0;
}

EH_SECT_rel::EH_SECT_rel( const char *data, uElf_Secthdr u )
	:EH_Section( data, u )
{
	if( is64bit )
	{
		rel.rel64 = (Elf64_Rel *) section_data;
		nRelocs = u.shdr64->sh_size / sizeof( Elf64_Rel );
	}
	else
	{
		rel.rel32 = (Elf32_Rel *) section_data;
		nRelocs = u.shdr32->sh_size / sizeof( Elf32_Rel );
	}
}

EH_SECT_rel::~EH_SECT_rel()
{

}

EH_FieldInfo EH_SECT_rel::GetFieldInfo( QModelIndex &index ) const
{
	EH_FieldInfo fi = {0,0};

	if( !index.isValid() )
		return fi;
	
	if( (EH_index_t) index.row() >= nRelocs || (EH_index_t)index.column() >= eh_rel::fieldnum )
		return fi;
	
	if( is64bit )
	{
		fi.start = hdr.shdr64->sh_offset;
		fi.start += sizeof(Elf64_Rel)*index.row();
	}
	else
	{
		fi.start = hdr.shdr32->sh_offset;
		fi.start += sizeof(Elf32_Rel)*index.row();
	}

	switch( index.column() )
	{
		case 0:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Rel, r_offset );
				fi.size = sizeof( Elf64_Rel::r_offset );
			}
			else
			{
				fi.start += offsetof( Elf32_Rel, r_offset );
				fi.size = sizeof( Elf32_Rel::r_offset );
			}
			break;
		case 1:
		case 2:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Rel, r_info );
				fi.size = sizeof( Elf64_Rel::r_info );
			}
			else
			{
				fi.start += offsetof( Elf32_Rel, r_info );
				fi.size = sizeof( Elf32_Rel::r_info );
			}
			break;
	}
	return fi;
}

int EH_SECT_rel::rowCount( const QModelIndex &parent ) const
{
	(void)parent;
	return nRelocs;
}

int EH_SECT_rel::columnCount( const QModelIndex &parent ) const
{
	(void)parent;
	return eh_rel::fieldnum;
}

QVariant EH_SECT_rel::data( const QModelIndex &index, int role ) const
{
	EH_index_t value;
	QString temp;
	uElf_Rel current_header = rel;

	if( !index.isValid() )
		return QVariant();

	if( (EH_index_t) index.row() >= nRelocs || (EH_index_t) index.column() >= eh_rel::fieldnum )
		return QVariant();

	if( is64bit )
		current_header.rel64 += index.row();
	else
		current_header.rel32 += index.row();

	switch( role )
	{
		case Qt::DisplayRole:
			switch( index.column() )
			{
				case 0:
					value = is64bit ? current_header.rel64->r_offset : current_header.rel32->r_offset;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 1:
					value = is64bit ? (current_header.rel64->r_info)>>32 : (current_header.rel32->r_info)>>8;
					temp = "Sym# " + QString::number( value );
					break;
				case 2:
					value = is64bit ? (current_header.rel64->r_info)&0xFFFFFFFF : (current_header.rel32->r_info)&0xFF;
					switch( value )
					{
						case R_386_NONE:
							temp = "R_386_NONE";
							break;
						case R_386_32:
							temp = "R_386_32";
							break;
						case R_386_PC32:
							temp = "R_386_PC32";
							break;
						case R_386_GOT32:
							temp = "R_386_GOT32";
							break;
						case R_386_PLT32:
							temp = "R_386_PLT32";
							break;
						case R_386_COPY:
							temp = "R_386_COPY";
							break;
						case R_386_GLOB_DAT:
							temp = "R_386_GLOB_DAT";
							break;
						case R_386_JMP_SLOT:
							temp = "R_386_JMP_SLOT";
							break;
						case R_386_RELATIVE:
							temp = "R_386_RELATIVE";
							break;
						case R_386_GOTOFF:
							temp = "R_386_GOTOFF";
							break;
						case R_386_GOTPC:
							temp = "R_386_GOTPC";
							break;
						case R_386_32PLT:
							temp = "R_386_32PLT";
							break;
						case R_386_TLS_TPOFF:
							temp = "R_386_TLS_TPOFF";
							break;
						case R_386_TLS_IE:
							temp = "R_386_TLS_IE";
							break;
						case R_386_TLS_GOTIE:
							temp = "R_386_TLS_GOTIE";
							break;
						case R_386_TLS_LE:
							temp = "R_386_TLS_LE";
							break;
						case R_386_TLS_GD:
							temp = "R_386_TLS_GD";
							break;
						case R_386_TLS_LDM:
							temp = "R_386_TLS_LDM";
							break;
						case R_386_16:
							temp = "R_386_16";
							break;
						case R_386_PC16:
							temp = "R_386_PC16";
							break;
						case R_386_8:
							temp = "R_386_8";
							break;
						case R_386_PC8:
							temp = "R_386_PC8";
							break;
						case R_386_TLS_GD_32:
							temp = "R_386_TLS_GD_32";
							break;
						case R_386_TLS_GD_PUSH:
							temp = "R_386_TLS_GD_PUSH";
							break;
						case R_386_TLS_GD_CALL:
							temp = "R_386_TLS_GD_CALL";
							break;
						case R_386_TLS_GD_POP:
							temp = "R_386_TLS_GD_POP";
							break;
						case R_386_TLS_LDM_32:
							temp = "R_386_TLS_LDM_32";
							break;
						case R_386_TLS_LDM_PUSH:
							temp = "R_386_TLS_LDM_PUSH";
							break;
						case R_386_TLS_LDM_CALL:
							temp = "R_386_TLS_LDM_CALL";
							break;
						case R_386_TLS_LDM_POP:
							temp = "R_386_TLS_LDM_POP";
							break;
						case R_386_TLS_LDO_32:
							temp = "R_386_TLS_LDO_32";
							break;
						case R_386_TLS_IE_32:
							temp = "R_386_TLS_IE_32";
							break;
						case R_386_TLS_LE_32:
							temp = "R_386_TLS_LE_32";
							break;
						case R_386_TLS_DTPMOD32:
							temp = "R_386_TLS_DTPMOD32";
							break;
						case R_386_TLS_DTPOFF32:
							temp = "R_386_TLS_DTPOFF32";
							break;
						case R_386_TLS_TPOFF32:
							temp = "R_386_TLS_TPOFF32";
							break;
						case R_386_TLS_GOTDESC:
							temp = "R_386_TLS_GOTDESC";
							break;
						case R_386_TLS_DESC_CALL:
							temp = "R_386_TLS_DESC_CALL";
							break;
						case R_386_TLS_DESC:
							temp = "R_386_TLS_DESC";
							break;
						case R_386_IRELATIVE:
							temp = "R_386_IRELATIVE";
							break;
						default:
							temp = "<Unknown>";
							break;
					}
					break;
				default:
					return QVariant();
					break;
			}
			break;
		case Qt::ToolTipRole:
		case Qt::StatusTipRole:
			switch( index.column() )
			{
				case 0:
					value = is64bit ? current_header.rel64->r_offset : current_header.rel32->r_offset;
					break;
				case 1:
					value = is64bit ? (current_header.rel64->r_info)>>32 : (current_header.rel32->r_info)>>8;
					break;
				case 2:
					value = is64bit ? (current_header.rel64->r_info)&0xFFFFFFFF : (current_header.rel32->r_info)&0xFF;
					break;
				default:
					return QVariant();
					break;
			}
			temp = "0x" + QString::number( value, 16 );
			break;
		default:
			return QVariant();
	}

	return temp;
}

QVariant EH_SECT_rel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if( role == Qt::DisplayRole )
	{
		if( orientation == Qt::Horizontal )
			if( section < eh_rel::fieldnum )
				return QString( eh_rel::names[ section ] );
			else
				return QVariant();
		else
			return QString::number( section );
	}
	return QVariant();
}

Qt::ItemFlags EH_SECT_rel::flags( const QModelIndex &index ) const
{
	if( !index.isValid() )
		return Qt::ItemIsEnabled;
	
	Qt::ItemFlags temp = QAbstractItemModel::flags( index );
	return temp; //return temp | Qt::ItemIsEditable;
}

bool EH_SECT_rel::setData( const QModelIndex &index, const QVariant &value, int role )
{
	(void)index;
	(void)value;
	(void)role;
	return true;
}
