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
 * Filename: EH_SECT_sym.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */

#include "EH_SECT_sym.h"

namespace eh_sym
{
	const int fieldnum = 6;

	const char *names[] =
	{
		"st_name",
		"st_value",
		"st_size",
		"st_info",
		"st_other",
		"st_shndx"
	};
};

EH_SECT_sym::EH_SECT_sym()
	:EH_Section()
{
	sym.sym32 = NULL;
	names_ptr = NULL;
	nSyms = 0;
}

EH_SECT_sym::EH_SECT_sym( const char *data, uElf_Secthdr u )
	:EH_Section( data, u )
{
	uElf_Secthdr str_sect;

	if( is64bit )
	{
		sym.sym64 = (Elf64_Sym *) section_data;
		nSyms = u.shdr64->sh_size / sizeof( Elf64_Sym );
		str_sect.shdr64 = (Elf64_Shdr *) ((char *)data + ((Elf64_Ehdr *)data)->e_shoff);
		str_sect.shdr64 += u.shdr64->sh_link;
		names_ptr = (char *)data + str_sect.shdr64->sh_offset;
	}
	else
	{
		sym.sym32 = (Elf32_Sym *) section_data;
		nSyms = u.shdr32->sh_size / sizeof( Elf32_Sym );
		str_sect.shdr32 = (Elf32_Shdr *) ((char *)data + ((Elf32_Ehdr *)data)->e_shoff);
		str_sect.shdr32 += u.shdr32->sh_link;
		names_ptr = (char *)data + str_sect.shdr32->sh_offset;
	}
}

EH_SECT_sym::~EH_SECT_sym()
{

}

EH_FieldInfo EH_SECT_sym::GetFieldInfo( QModelIndex &index ) const
{
	EH_FieldInfo fi = {0,0};

	if( !index.isValid() )
		return fi;
	
	if( (EH_index_t) index.row() >= nSyms || (EH_index_t)index.column() >= eh_sym::fieldnum )
		return fi;
	
	if( is64bit )
	{
		fi.start = hdr.shdr64->sh_offset;
		fi.start += sizeof(Elf64_Sym)*index.row();
	}
	else
	{
		fi.start = hdr.shdr32->sh_offset;
		fi.start += sizeof(Elf32_Sym)*index.row();
	}

	switch( index.column() )
	{
		case 0:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Sym, st_name );
				fi.size = sizeof( Elf64_Sym::st_name );
			}
			else
			{
				fi.start += offsetof( Elf32_Sym, st_name );
				fi.size = sizeof( Elf32_Sym::st_name );
			}
			break;
		case 1:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Sym, st_value );
				fi.size = sizeof( Elf64_Sym::st_value );
			}
			else
			{
				fi.start += offsetof( Elf32_Sym, st_value );
				fi.size = sizeof( Elf32_Sym::st_value );
			}
			break;
		case 2:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Sym, st_size );
				fi.size = sizeof( Elf64_Sym::st_size );
			}
			else
			{
				fi.start += offsetof( Elf32_Sym, st_size );
				fi.size = sizeof( Elf32_Sym::st_size );
			}
			break;
		case 3:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Sym, st_info );
				fi.size = sizeof( Elf64_Sym::st_info );
			}
			else
			{
				fi.start += offsetof( Elf32_Sym, st_info );
				fi.size = sizeof( Elf32_Sym::st_info );
			}
			break;
		case 4:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Sym, st_other );
				fi.size = sizeof( Elf64_Sym::st_other );
			}
			else
			{
				fi.start += offsetof( Elf32_Sym, st_other );
				fi.size = sizeof( Elf32_Sym::st_other );
			}
			break;
		case 5:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Sym, st_shndx );
				fi.size = sizeof( Elf64_Sym::st_shndx );
			}
			else
			{
				fi.start += offsetof( Elf32_Sym, st_name );
				fi.size = sizeof( Elf32_Sym::st_name );
			}
			break;
	}
	return fi;
}

int EH_SECT_sym::rowCount( const QModelIndex &parent ) const
{
	(void)parent;
	return nSyms;
}

int EH_SECT_sym::columnCount( const QModelIndex &parent ) const
{
	(void)parent;
	return eh_sym::fieldnum;
}

QVariant EH_SECT_sym::data( const QModelIndex &index, int role ) const
{
	EH_index_t value;
	QString temp;
	uElf_Sym current_header = sym;

	if( !index.isValid() )
		return QVariant();

	if( (EH_index_t) index.row() >= nSyms || (EH_index_t) index.column() >= eh_sym::fieldnum )
		return QVariant();

	if( is64bit )
		current_header.sym64 += index.row();
	else
		current_header.sym32 += index.row();

	switch( role )
	{
		case Qt::DisplayRole:
			switch( index.column() )
			{
				case 0:
					value = is64bit ? current_header.sym64->st_name : current_header.sym32->st_name;
					if( value != 0 )
						temp = QString( names_ptr + value );
					else
						temp = "<no name>";
					break;
				case 1:
					value = is64bit ? current_header.sym64->st_value : current_header.sym32->st_value;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 2:
					value = is64bit ? current_header.sym64->st_size : current_header.sym32->st_size;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 3:
					value = is64bit ? current_header.sym64->st_info : current_header.sym32->st_info;
					switch( ELF32_ST_BIND( (char)value ) )
					{
						case STB_LOCAL:
							temp = QString( "STB_LOCAL" );
							break;
						case STB_GLOBAL:
							temp = QString( "STB_GLOBAL" );
							break;
						case STB_WEAK:
							temp = QString( "STB_WEAK" );
							break;
						case STB_NUM:
							temp = QString( "STB_NUM" );
							break;
						case STB_GNU_UNIQUE:
							temp = QString( "STB_GNU_UNIQUE" );
							break;
						case STB_HIOS:
							temp = QString( "STB_HIOS" );
							break;
						default:
							if( value <= STB_HIPROC && value >= STB_LOPROC )
								temp = QString( "STB_PROC" );
							else
								temp = "N/A";
					}
					switch( ELF32_ST_TYPE( (char)value ) )
					{
						case STT_NOTYPE:
							temp += QString( "\nSTT_NOTYPE" );
							break;
						case STT_OBJECT:
							temp += QString( "\nSTT_OBJECT" );
							break;
						case STT_FUNC:
							temp += QString( "\nSTT_FUNC" );
							break;
						case STT_SECTION:
							temp += QString( "\nSTT_SECTION" );
							break;
						case STT_FILE:
							temp += QString( "\nSTT_FILE" );
							break;
						case STT_COMMON:
							temp += QString( "\nSTT_COMMON" );
							break;
						case STT_TLS:
							temp += QString( "\nSTT_TLS" );
							break;
						case STT_NUM:
							temp += QString( "\nSTT_NUM" );
							break;
						case STT_GNU_IFUNC:
							temp += QString( "\nSTT_GNU_IFUNC" );
							break;
						default:
							if( value <= STT_HIPROC && value >= STT_LOPROC )
								temp += QString( "\nSTT_PROC" );
							if( value <= STT_HIOS && value >= STT_LOOS )
								temp += QString( "\nSTT_OS" );
					}
					break;
				case 4:
					value = is64bit ? current_header.sym64->st_other : current_header.sym32->st_other;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 5:
					value = is64bit ? current_header.sym64->st_shndx : current_header.sym32->st_shndx;
					switch( value )
					{
						case SHN_UNDEF:
							temp = "SHN_UNDEF";
							break;
						case SHN_ABS:
							temp = "SHN_ABS";
							break;
						case SHN_COMMON:
							temp = "SHN_COMMON";
							break;
						default:
							temp = "Defined in Sect #" + QString::number( value );
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
					value = is64bit ? current_header.sym64->st_name : current_header.sym32->st_name;
					break;
				case 1:
					value = is64bit ? current_header.sym64->st_value : current_header.sym32->st_value;
					break;
				case 2:
					value = is64bit ? current_header.sym64->st_size : current_header.sym32->st_size;
					break;
				case 3:
					value = is64bit ? current_header.sym64->st_info : current_header.sym32->st_info;
					break;
				case 4:
					value = is64bit ? current_header.sym64->st_other : current_header.sym32->st_other;
					break;
				case 5:
					value = is64bit ? current_header.sym64->st_shndx : current_header.sym32->st_shndx;
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

QVariant EH_SECT_sym::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if( role == Qt::DisplayRole )
	{
		if( orientation == Qt::Horizontal )
			if( section < eh_sym::fieldnum )
				return QString( eh_sym::names[ section ] );
			else
				return QVariant();
		else
			return QString::number( section );
	}
	return QVariant();
}

Qt::ItemFlags EH_SECT_sym::flags( const QModelIndex &index ) const
{
	if( !index.isValid() )
		return Qt::ItemIsEnabled;
	
	Qt::ItemFlags temp = QAbstractItemModel::flags( index );
	return temp; //return temp | Qt::ItemIsEditable;
}

bool EH_SECT_sym::setData( const QModelIndex &index, const QVariant &value, int role )
{
	(void)index;
	(void)value;
	(void)role;
	return true;
}

char *EH_SECT_sym::GetSymbolName( EH_index_t sectn, EH_index_t symn, void *data )
{
	uElf_Ehdr ehdr;
	uElf_Secthdr shdr;
	uElf_Secthdr shdr_str;
	uElf_Sym sym;
	char *name;
	bool is64bit = false;

	if( data == NULL )
		return NULL;

	ehdr.ehdr32 = (Elf32_Ehdr*)data;
	is64bit = ehdr.ehdr32->e_ident[EI_CLASS] == ELFCLASS64 ? true : false;

	if( is64bit )
	{
		if( sectn >= ehdr.ehdr64->e_shnum )
			return NULL;

		shdr.shdr64 = (Elf64_Shdr*)( (char*)data + ehdr.ehdr64->e_shoff );
		shdr_str.shdr64 = shdr.shdr64;
		shdr.shdr64 += sectn;
		shdr_str.shdr64 += shdr.shdr64->sh_link;
		name = (char*)data + shdr_str.shdr64->sh_offset;
		sym.sym64 = (Elf64_Sym*)( (char*)data + shdr.shdr64->sh_offset );
		sym.sym64 += symn;
		name += sym.sym64->st_name;
	}
	else
	{
		if( sectn >= ehdr.ehdr32->e_shnum )
			return NULL;

		shdr.shdr32 = (Elf32_Shdr*)( (char*)data + ehdr.ehdr32->e_shoff );
		shdr_str.shdr32 = shdr.shdr32;
		shdr.shdr32 += sectn;
		shdr_str.shdr32 += shdr.shdr32->sh_link;
		name = (char*)data + shdr_str.shdr32->sh_offset;
		sym.sym32 = (Elf32_Sym*)( (char*)data + shdr.shdr32->sh_offset );
		sym.sym32 += symn;
		name += sym.sym32->st_name;
	}
	return name;
}
