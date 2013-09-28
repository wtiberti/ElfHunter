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
 * Filename: EH_SEG_dyn.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */

#include "EH_SEG_dyn.h"

namespace eh_dyn
{
	const int fieldnum = 2;

	const char *names[] =
	{
		"d_tag",
		"d_val / d_ptr"
	};
};

EH_SEG_dyn::EH_SEG_dyn()
	: EH_Segment()
{
	dyn.dyn32 = NULL;
}

EH_SEG_dyn::EH_SEG_dyn( const char *data, uElf_Proghdr u )
	: EH_Segment( data, u )
{
	if( is64bit )
		dyn.dyn64 = (Elf64_Dyn *) segment_data;
	else
		dyn.dyn32 = (Elf32_Dyn *) segment_data;

	nHeaders = (unsigned int) ( is64bit ? hdr.phdr64->p_filesz/sizeof(Elf64_Dyn) : hdr.phdr32->p_filesz/sizeof(Elf32_Dyn) );

	RetrieveStringTableFromEntries();
	RetrieveSymTableFromEntries();
}

EH_SEG_dyn::~EH_SEG_dyn()
{
	
}

int EH_SEG_dyn::rowCount( const QModelIndex &parent ) const
{
	(void)parent;
	return nHeaders;
}

int EH_SEG_dyn::columnCount( const QModelIndex &parent ) const
{
	(void)parent;
	return eh_dyn::fieldnum;
}

QVariant EH_SEG_dyn::data( const QModelIndex &index, int role ) const
{
	uElf_Dynhdr current;
	EH_index_t value;

	if( role == Qt::DisplayRole )
	{
		if( (unsigned)index.row() < nHeaders )
		{
			current = GetEntryAt( (unsigned)index.row() );

			if( index.column() == 0 )
			{
				value = is64bit ? current.dyn64->d_tag : current.dyn32->d_tag;
				switch( value )
				{
					case DT_NULL:
						return QString( "DT_NULL" );
					case DT_NEEDED:
						return QString( "DT_NEEDED" ); 
					case DT_PLTRELSZ:
						return QString( "DT_PLTRELSZ" );
					case DT_PLTGOT:
						return QString( "DT_PLTGOT" );
					case DT_HASH:
						return QString( "DT_HASH" );
					case DT_STRTAB:
						return QString( "DT_STRTAB" );
					case DT_SYMTAB:
						return QString( "DT_SYMTAB" );
					case DT_RELA:
						return QString( "DT_RELA" );
					case DT_RELASZ:
						return QString( "DT_RELASZ" );
					case DT_RELAENT:
						return QString( "DT_RELAENT" );
					case DT_STRSZ:
						return QString( "DT_STRSZ" );
					case DT_SYMENT:
						return QString( "DT_SYMENT" );
					case DT_INIT:
						return QString( "DT_INIT" );
					case DT_FINI:
						return QString( "DT_FINI" );
					case DT_SONAME:
						return QString( "DT_SONAME" );
					case DT_RPATH:
						return QString( "DT_RPATH" );
					case DT_SYMBOLIC:
						return QString( "DT_SYMBOLIC" );
					case DT_REL:
						return QString( "DT_REL" );
					case DT_RELSZ:
						return QString( "DT_RELSZ" );
					case DT_RELENT:
						return QString( "DT_RELENT" );
					case DT_PLTREL:
						return QString( "DT_PLTREL" );
					case DT_DEBUG:
						return QString( "DT_DEBUG" );
					case DT_TEXTREL:
						return QString( "DT_TEXTREL" );
					case DT_JMPREL:
						return QString( "DT_JMPREL" );
					case DT_BIND_NOW:
						return QString( "DT_BIND_NOW" );
					case DT_INIT_ARRAY:
						return QString( "DT_INIT_ARRAY" );
					case DT_FINI_ARRAY:
						return QString( "DT_FINI_ARRAY" );
					case DT_INIT_ARRAYSZ:
						return QString( "DT_INIT_ARRAYSZ" );
					case DT_FINI_ARRAYSZ:
						return QString( "DT_FINI_ARRAYSZ" );
					case DT_RUNPATH:
						return QString( "DT_RUNPATH" );
					case DT_FLAGS:
						return QString( "DT_FLAGS" );
					case DT_PREINIT_ARRAY:
						return QString( "DT_PREINIT_ARRAY" );
					case DT_PREINIT_ARRAYSZ:
						return QString( "DT_PREINIT_ARRAYSZ" );
					case DT_GNU_HASH:
						return QString( "DDT_GNU_HASH" );
					case DT_TLSDESC_PLT:
						return QString( "DDT_TLSDESC_PLT" );
					case DT_TLSDESC_GOT:
						return QString( "DDT_TLSDESC_GOT" );
					case DT_GNU_CONFLICT:
						return QString( "DDT_GNU_CONFLICT" );
					case DT_GNU_LIBLIST:
						return QString( "DDT_GNU_LIBLIST" );
					case DT_CONFIG:
						return QString( "DDT_CONFIG" );
					case DT_DEPAUDIT:
						return QString( "DDT_DEPAUDIT" );
					case DT_AUDIT:
						return QString( "DDT_AUDIT" );
					case DT_PLTPAD:
						return QString( "DDT_PLTPAD" );
					case DT_MOVETAB:
						return QString( "DDT_MOVETAB" );
					case DT_SYMINFO:
						return QString( "DDT_SYMINFO" );
					case DT_VERSYM:
						return QString( "DT_VERSYM" );
					case DT_RELACOUNT:
						return QString( "DT_RELACOUNT" );
					case DT_RELCOUNT:
						return QString( "DT_RELCOUNT" );
					case DT_FLAGS_1:
						return QString( "DT_FLAGS_1" );
					case DT_VERDEF:
						return QString( "DT_VERDEF" );
					case DT_VERDEFNUM:
						return QString( "DT_VERDEFNUM" );
					case DT_VERNEED:
						return QString( "DT_VERNEED" );
					case DT_VERNEEDNUM:
						return QString( "DT_VERNEEDNUM" );
					default:
						if( value <= DT_HIPROC )
						{
							if( value >= DT_LOPROC )
								return QString( "DT_PROC" );
							else
								if( value <= DT_HIOS && value >= DT_LOOS )
									return QString( "DT_OS" );
								else 
									return "0x" + QString::number( value, 16 );
						}
						return "0x" + QString::number( value, 16 );
				}
			}
			else
			{
				EH_off_t tag = is64bit ? current.dyn64->d_tag : current.dyn32->d_tag;
				value = is64bit ? current.dyn64->d_un.d_val : current.dyn32->d_un.d_val;

				if( tag == DT_NEEDED )
				{
					return QString( (char*)( strtab + value ) );
				}
				else
				{
					return "0x" + QString::number( value, 16 );
				}
			}
		}
	}
	else
		if( role == Qt::ToolTipRole || role == Qt::StatusTipRole )
		{
			if( (unsigned)(index.row()) < nHeaders )
			{
				current = GetEntryAt( (unsigned)index.row() );

				if( index.column() == 0 )
					value = is64bit ? current.dyn64->d_tag : current.dyn32->d_tag;
				else
					value = is64bit ? current.dyn64->d_un.d_val : current.dyn32->d_un.d_val;

				return "0x" + QString::number( value, 16 );
			}
		}
	return QVariant();
}

QVariant EH_SEG_dyn::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if( role == Qt::DisplayRole )
	{
		if( orientation == Qt::Horizontal && section < eh_dyn::fieldnum )
			return QString( eh_dyn::names[ section ] );
		else
			if( orientation == Qt::Vertical && (unsigned)section < nHeaders )
				return QString::number( section );
	}

	return QVariant();
}

Qt::ItemFlags EH_SEG_dyn::flags( const QModelIndex &index ) const
{
	if( !index.isValid() )
		return Qt::ItemIsEnabled;
	
	Qt::ItemFlags temp = QAbstractItemModel::flags( index );
	return temp; //return temp | Qt::ItemIsEditable;
}

bool EH_SEG_dyn::setData( const QModelIndex &index, const QVariant &value, int role )
{
	(void)index;
	(void)value;
	(void)role;
	return true;
}

EH_FieldInfo EH_SEG_dyn::GetFieldInfo( QModelIndex &index ) const
{
	EH_FieldInfo fi = {0,0};
	
	fi.start = is64bit ? hdr.phdr64->p_offset : hdr.phdr32->p_offset;
	
	if( (unsigned)index.row() < nHeaders )
	{
		if( is64bit )
			fi.start += sizeof( Elf64_Dyn )*index.row();
		else
			fi.start += sizeof( Elf32_Dyn )*index.row();

		switch( index.column() )
		{
			case 0:
				if( is64bit )
				{
					fi.start += offsetof( Elf64_Dyn, d_tag );
					fi.size += sizeof( Elf64_Dyn::d_tag );
				}
				else
				{
					fi.start += offsetof( Elf32_Dyn, d_tag );
					fi.size += sizeof( Elf32_Dyn::d_tag );
				}
				break;
			case 1:
				if( is64bit )
				{
					fi.start += offsetof( Elf64_Dyn, d_un );
					fi.size += sizeof( Elf64_Dyn::d_un );
				}
				else
				{
					fi.start += offsetof( Elf32_Dyn, d_un );
					fi.size += sizeof( Elf32_Dyn:: d_un );
				}
				break;
		}
	}
	return fi;
}

uElf_Dynhdr EH_SEG_dyn::GetEntryAt( unsigned int index ) const
{
	uElf_Dynhdr result;

	if( is64bit )
		result.dyn64 = dyn.dyn64 + index;
	else
		result.dyn32 = dyn.dyn32 + index;

	return result;
}

char *EH_SEG_dyn::RetrieveStringTableFromEntries()
{
	uElf_Dynhdr current;
	EH_off_t tag;
	EH_off_t value;

	strtab = NULL;

	for( unsigned int i=0; i<nHeaders; i++ )
	{
		current = GetEntryAt( i );

		tag = is64bit ? current.dyn64->d_tag : current.dyn32->d_tag;

		if( tag == DT_STRTAB )
		{
			if( is64bit )
				value = current.dyn64->d_un.d_val - 0x00400000; //Subtract image offset for x64
			else
				value = current.dyn64->d_un.d_val - 0x08040000; //Subtract image offset for x86

			strtab = (char*)( base + value ); // Adding current imagebase
			break;
		}
	}
	return strtab;
}

uElf_Sym EH_SEG_dyn::RetrieveSymTableFromEntries()
{
	uElf_Dynhdr current;
	EH_off_t tag;
	EH_off_t value;

	symtab.sym32 = NULL;

	for( unsigned int i=0; i<nHeaders; i++ )
	{
		current = GetEntryAt( i );

		tag = is64bit ? current.dyn64->d_tag : current.dyn32->d_tag;

		if( tag == DT_SYMTAB )
		{
			if( is64bit )
			{
				value = current.dyn64->d_un.d_val - 0x00400000; //Subtract image offset for x64
				symtab.sym64 = (Elf64_Sym *)( base + value );
			}
			else
			{
				value = current.dyn32->d_un.d_val - 0x08040000; //Subtract image offset for x86
				symtab.sym32 = (Elf32_Sym *)( base + value );
			}
			break;
		}
	}
	return symtab;
}

