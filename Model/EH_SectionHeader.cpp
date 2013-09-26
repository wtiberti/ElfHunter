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
 * Filename: EH_SectionHeader.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#include "EH_FileHeader.h"
#include "EH_SectionHeader.h"

namespace eh_sh
{
	const int fieldnum = 10;
	const char *names[] =
	{
		"sh_name",
		"sh_type",
		"sh_flags",
		"sh_addr",
		"sh_offset",
		"sh_size",
		"sh_link",
		"sh_info",
		"sh_addralign",
		"sh_entsize"
	};
};

EH_SectionHeader::EH_SectionHeader()
: EH_Model()
{
	hdr.shdr32 = NULL;
	base_data = NULL;
	is64bit = false;
}

EH_SectionHeader::EH_SectionHeader( void *data )
: EH_Model()
{
	hdr.shdr32 = NULL;
	base_data = NULL;
	is64bit = false;
	Set( data );
}

EH_SectionHeader::~EH_SectionHeader()
{
	
}

int EH_SectionHeader::rowCount( const QModelIndex &parent ) const
{
	(void)parent;
	return nHeaders;
}

int EH_SectionHeader::columnCount( const QModelIndex &parent ) const
{
	(void)parent;
	return eh_sh::fieldnum;
}

QVariant EH_SectionHeader::data( const QModelIndex &index, int role ) const
{
	if( !index.isValid() )
		return QVariant();
	
	if( (EH_index_t) index.row() >= nHeaders || (EH_index_t)index.column() >= eh_sh::fieldnum )
		return QVariant();
	
	uElf_Secthdr current_header = hdr;
	
	if( is64bit )
	{
		current_header.shdr64 += index.row();
	}
	else
	{
		current_header.shdr32 += index.row();
	}
	
	QString temp;
	EH_index_t value;

	switch( role )
	{
		case Qt::DisplayRole:
			switch( index.column() )
			{
				case 0:
					value = is64bit ? current_header.shdr64->sh_name : current_header.shdr32->sh_name;
					if( value <= 0 || value >= SHN_LOPROC )
					{
						if( value == SHN_UNDEF )
							temp = "SHN_UNDEF";
						if( value >= SHN_LOPROC && value <= SHN_HIPROC )
							temp = "SHN_PROC";
						if( value >= SHN_LOOS && value <= SHN_HIOS )
							temp = "SHN_OS";
						if( value == SHN_ABS )
							temp = "SHN_ABS";
						if( value == SHN_COMMON )
							temp = "SHN_COMMON";
						if( value == SHN_XINDEX )
							temp = "SHN_XINDEX (extra-table)";
					}
					else
					{
						value = is64bit ? current_header.shdr64->sh_name : current_header.shdr32->sh_name;
						char *sectname = GetNameSection( base_data ) + value;
						temp = QString( sectname );
					}
					break;
				case 1:
					value = is64bit ? current_header.shdr64->sh_type : current_header.shdr32->sh_type;
					switch( value )
					{
						case SHT_NULL:
							temp = "SHT_NULL";
							break;
						case SHT_PROGBITS:
							temp = "SHT_PROGBITS";
							break;
						case SHT_SYMTAB:
							temp = "SHT_SYMTAB";
							break;
						case SHT_STRTAB:
							temp = "SHT_STRTAB";
							break;
						case SHT_RELA:
							temp = "SHT_RELA";
							break;
						case SHT_HASH:
							temp = "SHT_HASH";
							break;
						case SHT_DYNAMIC:
							temp = "SHT_DYNAMIC";
							break;
						case SHT_NOTE:
							temp = "SHT_NOTE";
							break;
						case SHT_NOBITS:
							temp = "SHT_NOBITS";
							break;
						case SHT_REL:
							temp = "SHT_REL";
							break;
						case SHT_SHLIB:
							temp = "SHT_SHLIB";
							break;
						case SHT_DYNSYM:
							temp = "SHT_DYNSYM";
							break;
						case SHT_INIT_ARRAY:
							temp = "SHT_INIT_ARRAY";
							break;
						case SHT_FINI_ARRAY:
							temp = "SHT_FINI_ARRAY";
							break;
						case SHT_PREINIT_ARRAY:
							temp = "SHT_PREINIT_ARRAY";
							break;
						case SHT_GROUP:
							temp = "SHT_GROUP";
							break;
						case SHT_SYMTAB_SHNDX:
							temp = "SHT_SYMTAB_SHNDX";
							break;
						case SHT_GNU_ATTRIBUTES:
							temp = "SHT_GNU_ATTR";
							break;
						case SHT_GNU_HASH:
							temp = "SHT_GNU_HASH";
							break;
						case SHT_GNU_LIBLIST:
							temp = "SHT_GNU_LIBLIST";
							break;
						case SHT_CHECKSUM:
							temp = "SHT_CHECKSUM";
							break;
						case SHT_SUNW_move:
							temp = "SHT_SUNW_move";
							break;
						case SHT_SUNW_COMDAT:
							temp = "SHT_SUNW_COMDAT";
							break;
						case SHT_SUNW_syminfo:
							temp = "SHT_SUNW_syminfo";
							break;
						case SHT_GNU_verdef:
							temp = "SHT_GNU_verdef";
							break;
						case SHT_GNU_verneed:
							temp = "SHT_GNU_verdef";
							break;
						case SHT_GNU_versym:
							temp = "SHT_GNU_versym";
							break;
						default:
							if( value > SHT_HIUSER )
								temp = "<Invalid>";
							else
								if( value >= SHT_LOUSER )
									temp = "SHT_USR 0x" + QString::number( value, 16 );
								else
									if( value >= SHT_LOPROC )
										temp = "SHT_PROC 0x" + QString::number( value, 16 );
									else
										if( value >= SHT_LOOS )
											temp = "SHT_OS 0x" + QString::number( value, 16 );
										else
											temp = "<Error>";
							break;
					}
					break;
				case 2:
					value = is64bit ? current_header.shdr64->sh_flags : current_header.shdr32->sh_flags;
					if( value & SHF_WRITE )
						temp.append( "SHF_WRITE\n" );
					if( value & SHF_ALLOC )
						temp.append( "SHF_ALLOC\n" );
					if( value & SHF_EXECINSTR )
						temp.append( "SHF_EXECINSTR\n" );
					if( value & SHF_MERGE )
						temp.append( "SHF_MERGE\n" );
					if( value & SHF_STRINGS )
						temp.append( "SHF_STRINGS\n" );
					if( value & SHF_INFO_LINK )
						temp.append( "SHF_INFO_LINK\n" );
					if( value & SHF_LINK_ORDER )
						temp.append( "SHF_LINK_ORDER\n" );
					if( value & SHF_OS_NONCONFORMING )
						temp.append( "SHF_OS_NONCONFORMING\n" );
					if( value & SHF_GROUP )
						temp.append( "SHF_GROUP\n" );
					if( value & SHF_TLS )
						temp.append( "SHF_TLS\n" );
					if( value & SHF_MASKOS )
						temp.append( "SHF_OS\n" );
					if( value & SHF_MASKPROC )
						temp.append( "SHF_PROC\n" );
					break;
				case 3:
					value = is64bit ? current_header.shdr64->sh_addr : current_header.shdr32->sh_addr;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 4:
					value = is64bit ? current_header.shdr64->sh_offset : current_header.shdr32->sh_offset;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 5:
					value = is64bit ? current_header.shdr64->sh_size : current_header.shdr32->sh_size;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 6:
					value = is64bit ? current_header.shdr64->sh_link : current_header.shdr32->sh_link;
					switch( is64bit ? current_header.shdr64->sh_type : current_header.shdr32->sh_type )
					{
						case SHT_SYMTAB:
						case SHT_DYNAMIC:
							temp = "Str Sect #: " + QString::number( value );
							break;
						case SHT_HASH:
						case SHT_REL:
						case SHT_RELA:
							temp = "Sym Sect #: " + QString::number( value );
							break;
						default:
							temp = "0x" + QString::number( value, 16 );
							break;
					}
					break;
				case 7:
					value = is64bit ? current_header.shdr64->sh_info : current_header.shdr32->sh_info;
					switch( is64bit ? current_header.shdr64->sh_type : current_header.shdr32->sh_type )
					{
						case SHT_REL:
						case SHT_RELA:
							temp = "Sect #: " + QString::number( value );
							break;
						case SHT_SYMTAB:
						case SHT_DYNAMIC:
							temp = "Local Syms: " + QString::number( value );
							break;
						default:
							temp = "0x" + QString::number( value, 16 );
							break;
					}
					break;
				case 8:
					value = is64bit ? current_header.shdr64->sh_addralign : current_header.shdr32->sh_addralign;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 9:
					value = is64bit ? current_header.shdr64->sh_entsize : current_header.shdr32->sh_entsize;
					temp = "0x" + QString::number( value, 16 );
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
					value = is64bit ? current_header.shdr64->sh_name : current_header.shdr32->sh_name;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 1:
					value = is64bit ? current_header.shdr64->sh_type : current_header.shdr32->sh_type;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 2:
					value = is64bit ? current_header.shdr64->sh_flags : current_header.shdr32->sh_flags;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 3:
					value = is64bit ? current_header.shdr64->sh_addr : current_header.shdr32->sh_addr;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 4:
					value = is64bit ? current_header.shdr64->sh_offset : current_header.shdr32->sh_offset;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 5:
					value = is64bit ? current_header.shdr64->sh_size : current_header.shdr32->sh_size;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 6:
					value = is64bit ? current_header.shdr64->sh_link : current_header.shdr32->sh_link;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 7:
					value = is64bit ? current_header.shdr64->sh_info : current_header.shdr32->sh_info;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 8:
					value = is64bit ? current_header.shdr64->sh_addralign : current_header.shdr32->sh_addralign;
					temp = "0x" + QString::number( value, 16 );
					break;
				case 9:
					value = is64bit ? current_header.shdr64->sh_entsize : current_header.shdr32->sh_entsize;
					temp = "0x" + QString::number( value, 16 );
					break;
				default:
					return QVariant();
					break;
			}
			break;

		default:
			return QVariant();
	}

	return temp;
}

QVariant EH_SectionHeader::headerData ( int section, Qt::Orientation orientation, int role ) const
{
	if( role != Qt::DisplayRole )
		return QVariant();
	
	if( orientation != Qt::Vertical )
	{
		if( section < eh_sh::fieldnum )
			return QString( eh_sh::names[section] );
	}
	else
	{
		if( (unsigned)section < nHeaders )
			return QString::number( section );
	}
	return QVariant();
}

Qt::ItemFlags EH_SectionHeader::flags( const QModelIndex &index ) const
{
	if( !index.isValid() )
		return Qt::ItemIsEnabled;
	
	// return Qt::ItemIsEditable | QAbstractItemModel::flags( index );
	return QAbstractItemModel::flags( index );
}

bool EH_SectionHeader::setData( const QModelIndex &index, const QVariant &value, int role )
{
	(void)index;
	(void)value;
	(void)role;
	return true;
}

bool EH_SectionHeader::Set( void *data )
{
	base_data = (char *)data;
	base_offset = EH_FileHeader::GetShdrOffset( data );
	hdr.shdr32 = (Elf32_Shdr *) ( (char *)data + base_offset );
	is64bit =  EH_FileHeader::IsELF64( data )==1?true:false;
	nHeaders = EH_FileHeader::GetShdrNum( data );
	return true;
}

EH_size_t EH_SectionHeader::GetNumOfSections() const
{
	return nHeaders;
}

void *EH_SectionHeader::GetSection( EH_index_t index )
{
	if( index >= nHeaders )
		return NULL;
	
	if( is64bit )
		return (void *)(hdr.shdr64 + index);
	
	return (void *)(hdr.shdr32 + index);
}


char *EH_SectionHeader::GetNameSection( void *data )
{
	int index;
	bool is64bit = ((Elf32_Ehdr *)data)->e_ident[EI_CLASS]==2?true:false;
	
	if( is64bit )
	{
		index = ((Elf64_Ehdr *)data)->e_shstrndx;
		EH_off_t shoff64 = ((Elf64_Ehdr *)data)->e_shoff;
		Elf64_Shdr *shdr64 = (Elf64_Shdr *)((char*)data + shoff64);
		shdr64 += index;
		return (char *)data + shdr64->sh_offset;
	}
	else
	{
		index = ((Elf32_Ehdr *)data)->e_shstrndx;
		EH_off_t shoff32 = ((Elf32_Ehdr *)data)->e_shoff;
		Elf32_Shdr *shdr = (Elf32_Shdr *)((char*)data + shoff32);
		shdr += index;
		return (char *)data + shdr->sh_offset;
	}

	return NULL;
}


EH_FieldInfo EH_SectionHeader::GetFieldInfo( QModelIndex &index ) const
{
	EH_FieldInfo fi = {0,0};

	if( !index.isValid() )
		return fi;
	
	if( (EH_index_t) index.row() >= nHeaders || (EH_index_t)index.column() >= eh_sh::fieldnum )
		return fi;
	
	fi.start = base_offset;

	if( is64bit )
		fi.start += sizeof(Elf64_Shdr)*index.row();
	else
		fi.start += sizeof(Elf32_Shdr)*index.row();

	switch( index.column() )
	{
		case 0:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Shdr, sh_name );
				fi.size = sizeof( Elf64_Shdr::sh_name );
			}
			else
			{
				fi.start += offsetof( Elf32_Shdr, sh_name );
				fi.size = sizeof( Elf32_Shdr::sh_name );
			}
			break;
		case 1:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Shdr, sh_type );
				fi.size = sizeof( Elf64_Shdr::sh_type );
			}
			else
			{
				fi.start += offsetof( Elf32_Shdr, sh_type );
				fi.size = sizeof( Elf32_Shdr::sh_type );
			}
			break;
		case 2:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Shdr, sh_flags );
				fi.size = sizeof( Elf64_Shdr::sh_flags );
			}
			else
			{
				fi.start += offsetof( Elf32_Shdr, sh_flags );
				fi.size = sizeof( Elf32_Shdr::sh_flags );
			}
			break;
		case 3:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Shdr, sh_addr );
				fi.size = sizeof( Elf64_Shdr::sh_addr );
			}
			else
			{
				fi.start += offsetof( Elf32_Shdr, sh_addr );
				fi.size = sizeof( Elf32_Shdr::sh_addr );
			}
			break;
		case 4:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Shdr, sh_offset );
				fi.size = sizeof( Elf64_Shdr::sh_offset );
			}
			else
			{
				fi.start += offsetof( Elf32_Shdr, sh_offset );
				fi.size = sizeof( Elf32_Shdr::sh_offset );
			}
			break;
		case 5:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Shdr, sh_size );
				fi.size = sizeof( Elf64_Shdr::sh_size );
			}
			else
			{
				fi.start += offsetof( Elf32_Shdr, sh_size );
				fi.size = sizeof( Elf32_Shdr::sh_size );
			}
			break;
		case 6:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Shdr, sh_link );
				fi.size = sizeof( Elf64_Shdr::sh_link );
			}
			else
			{
				fi.start += offsetof( Elf32_Shdr, sh_link );
				fi.size = sizeof( Elf32_Shdr::sh_link );
			}
			break;
		case 7:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Shdr, sh_info );
				fi.size = sizeof( Elf64_Shdr::sh_info );
			}
			else
			{
				fi.start += offsetof( Elf32_Shdr, sh_info );
				fi.size = sizeof( Elf32_Shdr::sh_info );
			}
			break;
		case 8:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Shdr, sh_addralign );
				fi.size = sizeof( Elf64_Shdr::sh_addralign );
			}
			else
			{
				fi.start += offsetof( Elf32_Shdr, sh_addralign );
				fi.size = sizeof( Elf32_Shdr::sh_addralign );
			}
			break;
		case 9:
			if( is64bit )
			{
				fi.start += offsetof( Elf64_Shdr, sh_entsize );
				fi.size = sizeof( Elf64_Shdr::sh_entsize );
			}
			else
			{
				fi.start += offsetof( Elf32_Shdr, sh_entsize );
				fi.size = sizeof( Elf32_Shdr::sh_entsize );
			}
			break;
	}
	return fi;
}
