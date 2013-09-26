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
 * Filename: EH_FileHeader.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#include "EH_FileHeader.h"
#include "EH_File.h"

/** Namespace for EH_FileHeader */
namespace eh_fh
{
	const int fieldnum = 19;
	const char *names[] =
	{
		"e_ident[EI_MAG]",
		"e_ident[EI_CLASS]",
		"e_ident[EI_DATA]",
		"e_ident[EI_VERSION]",
		"e_ident[EI_OSABI]",
		"e_ident[EI_ABIVERSION]",
		"e_type",
		"e_machine",
		"e_version",
		"e_entry",
		"e_phoff",
		"e_shoff",
		"e_flags",
		"e_ehsize",
		"e_phentsize",
		"e_phnum",
		"e_shentsize",
		"e_shnum",
		"e_shstrndx"
	};
};

EH_FileHeader::EH_FileHeader()
: EH_Model()
{
	hdr.ehdr32 = NULL;
	total_file_size = 0;
}

EH_FileHeader::EH_FileHeader( void *data, EH_size_t size )
: EH_Model()
{
	hdr.ehdr32 = NULL;
	Set( data, size );
}

EH_FileHeader::~EH_FileHeader()
{
	//TODO
}

bool EH_FileHeader::Set( void *data, EH_size_t size )
{
	total_file_size = size;
	
	if( data == NULL )
	{
		hdr.ehdr32 = NULL;
		return false;
	}
	else
	{
		hdr.ehdr32 = (Elf32_Ehdr *) data;
		
		if( !Valid() )
		{
			hdr.ehdr32 = NULL;
			return false;
		}
	}
	return true;
}

bool EH_FileHeader::Valid() const
{
	if( hdr.ehdr32 == NULL )
		return false;
	
	// NOTICE: using 32-bit type pointer
	if( hdr.ehdr32->e_ident[EI_MAG0] != ELFMAG0 ||
		hdr.ehdr32->e_ident[EI_MAG1] != ELFMAG1 ||
		hdr.ehdr32->e_ident[EI_MAG2] != ELFMAG2 ||
		hdr.ehdr32->e_ident[EI_MAG3] != ELFMAG3 )
		return false;
	
	if( total_file_size < EI_NIDENT ) // file too small
	{
		//hdr.ehdr32 = NULL;
		return false;
	}
	
	return true;
}

int EH_FileHeader::Bits() const
{
	unsigned int tempbits;
	
	if( hdr.ehdr32 == NULL )
		return 0;
	
	// using 32-bit type pointer
	tempbits = (unsigned int) (hdr.ehdr32->e_ident[EI_CLASS]);
	return (tempbits<ELFCLASSNUM)?tempbits*32:0;
}

int EH_FileHeader::rowCount( const QModelIndex &parent ) const
{
	(void) parent; // unused
	return eh_fh::fieldnum;
}

int EH_FileHeader::columnCount( const QModelIndex &parent ) const
{
	(void) parent; // unused
	return 1;
}

QVariant EH_FileHeader::data( const QModelIndex &index, int role ) const
{
	EH_index_t temp;
	bool is64bit = (Bits()==64);
	
	if( !index.isValid() )
		return QVariant();
	
	switch( role )
	{
		case Qt::DisplayRole:
			switch( index.row() )
			{
			case 0:
				return QString( "0x%1 0x%2 0x%3 0x%4 (%5)" )
					.arg( hdr.ehdr32->e_ident[EI_MAG0], 2, 16 )
					.arg( hdr.ehdr32->e_ident[EI_MAG1], 2, 16 )
					.arg( hdr.ehdr32->e_ident[EI_MAG2], 2, 16 )
					.arg( hdr.ehdr32->e_ident[EI_MAG3], 2, 16 )
					.arg( Valid()?"Valid":"Invalid" );
			case 1:
				switch( Bits() )
				{
					case 0:
						return QString( "ELFCLASSNONE" );
					case 32:
						return QString( "ELFCLASS32 (32-bit)" );
					case 64:
						return QString( "ELFCLASS64 (64-bit)" );
					default:
						return QString( "0x%1 [Value not valid]" )
							.arg( (int)(hdr.ehdr32->e_ident[EI_CLASS]), 2, 16 );
				}
			case 2:
				switch( hdr.ehdr32->e_ident[EI_DATA] )
				{
					case ELFDATANONE:
						return QString( "ELFDATANONE" );
					case ELFDATA2LSB:
						return QString( "ELFDATA2LSB (Little Endian)" );
					case ELFDATA2MSB:
						return QString( "ELFDATA2MSB (Big Endian)" );
					default:
						return QString( "0x%1 [Value not valid]" )
							.arg( (int)(hdr.ehdr32->e_ident[EI_DATA]), 2, 16 );
				}
			case 3:
				switch( hdr.ehdr32->e_ident[EI_VERSION] )
				{
					case EV_CURRENT:
						return QString( "EV_CURRENT (Current Version)" );
					default:
						return QString( "0x%1 [Value not valid]" )
							.arg( (int)(hdr.ehdr32->e_ident[EI_VERSION]), 2, 16 );
				}
			case 4:
				switch( hdr.ehdr32->e_ident[EI_OSABI] )
				{
					case ELFOSABI_NONE:
						return QString( "ELFOSABI_NONE / ELFOSABI_SYSV" );
					case ELFOSABI_HPUX:
						return QString( "ELFOSABI_HPUX" );
					case ELFOSABI_NETBSD:
						return QString( "ELFOSABI_NETBSD" );
					case ELFOSABI_GNU:
						return QString( "ELFOSABI_GNU / ELFOSABI_LINUX" );
					case ELFOSABI_SOLARIS:
						return QString( "ELFOSABI_SOLARIS" );
					case ELFOSABI_AIX:
						return QString( "ELFOSABI_AIX" );
					case ELFOSABI_IRIX:
						return QString( "ELFOSABI_IRIX" );
					case ELFOSABI_FREEBSD:
						return QString( "ELFOSABI_FREEBSD" );
					case ELFOSABI_TRU64:
						return QString( "ELFOSABI_TRU64" );
					case ELFOSABI_MODESTO:
						return QString( "ELFOSABI_MODESTO" );
					case ELFOSABI_OPENBSD:
						return QString( "ELFOSABI_OPENBSD" );
					case ELFOSABI_ARM_AEABI:
						return QString( "ELFOSABI_ARM_AEABI" );
					case ELFOSABI_ARM:
						return QString( "ELFOSABI_ARM" );
					case ELFOSABI_STANDALONE:
						return QString( "ELFOSABI_STANDALONE" );
					default:
						return QString( "0x%1 [Value not recognized]" )
							.arg( (int)(hdr.ehdr32->e_ident[EI_OSABI]), 2, 16 );
				}
			case 5:
				return QString( "0x%1" ).arg( (int)(hdr.ehdr32->e_ident[EI_ABIVERSION]), 0, 16 );
			case 6:
				switch( hdr.ehdr32->e_type )
				{
					case ET_NONE: return QString( "ET_NONE" );
					case ET_REL: return QString( "ET_REL" );
					case ET_EXEC: return QString( "ET_EXEC" );
					case ET_DYN: return QString( "ET_DYN" );
					case ET_CORE: return QString( "ET_CORE" );
				}
				if( hdr.ehdr32->e_type > ET_HIPROC || hdr.ehdr32->e_type < ET_LOOS )
					return QString( "0x%1 [Value not recognized]" ).arg( (hdr.ehdr32->e_type), 0, 16 );
				else
				{
					if( hdr.ehdr32->e_type < ET_HIOS )
						return QString( "ET_OS [ %1 ]" ).arg( hdr.ehdr32->e_type );
					else
						return QString( "ET_PROC [ %1 ]" ).arg( hdr.ehdr32->e_type );
				}
			case 7:
				switch( hdr.ehdr32->e_machine )
				{
					case EM_NONE: return QString( "EM_NONE" );
					// TODO: insert other values
					case EM_386: return QString( "EM_386" );
					case EM_PPC: return QString( "EM_PPC" );
					case EM_PPC64: return QString( "EM_PPC64" );
					case EM_ARM: return QString( "EM_ARM" );
					case EM_X86_64: return QString( "EM_X86_64" );
					default:
						return QString( "0x%1" ).arg( hdr.ehdr32->e_machine, 0, 16 );
				}
			case 8:
				switch( hdr.ehdr32->e_version )
				{
					case EV_CURRENT:
						return QString( "EV_CURRENT (Current Version)" );
					default:
						return QString( "0x%1 [Value not valid]" )
						.arg( (int)(hdr.ehdr32->e_ident[EI_VERSION]), 2, 16 );
				}
			case 9:
				temp = is64bit?hdr.ehdr64->e_entry:hdr.ehdr32->e_entry;
				return QString( "0x%1" ).arg( temp, 0, 16 );
			case 10:
				temp = is64bit?hdr.ehdr64->e_phoff:hdr.ehdr32->e_phoff;
				return QString( "0x%1" ).arg( temp, 0, 16 );
			case 11:
				temp = is64bit?hdr.ehdr64->e_shoff:hdr.ehdr32->e_shoff;
				return QString( "0x%1" ).arg( temp, 0, 16 );
			case 12:
				temp = is64bit?hdr.ehdr64->e_flags:hdr.ehdr32->e_flags;
				return QString( "0x%1" ).arg( temp, 0, 16 );
			case 13:
				temp = is64bit?hdr.ehdr64->e_ehsize:hdr.ehdr32->e_ehsize;
				return QString( "0x%1" ).arg( temp, 0, 16 );
			case 14:
				temp = is64bit?hdr.ehdr64->e_phentsize:hdr.ehdr32->e_phentsize;
				return QString( "0x%1" ).arg( temp, 0, 16 );
			case 15:
				temp = is64bit?hdr.ehdr64->e_phnum:hdr.ehdr32->e_phnum;
				return QString( "0x%1" ).arg( temp, 0, 16 );
			case 16:
				temp = is64bit?hdr.ehdr64->e_shentsize:hdr.ehdr32->e_shentsize;
				return QString( "0x%1" ).arg( temp, 0, 16 );
			case 17:
				temp = is64bit?hdr.ehdr64->e_shnum:hdr.ehdr32->e_shnum;
				return QString( "0x%1" ).arg( temp, 0, 16 );
			case 18:
				temp = is64bit?hdr.ehdr64->e_shstrndx:hdr.ehdr32->e_shstrndx;
				return QString( "0x%1" ).arg( temp, 0, 16 );
			}
			break;
		case Qt::ToolTipRole:
		case Qt::StatusTipRole:
			switch( index.row() )
			{
				case 0:
					return QVariant();
				case 1:
					return QString::number( hdr.ehdr32->e_ident[EI_CLASS] );
				case 2:
					return QString::number( hdr.ehdr32->e_ident[EI_DATA] );
				case 3:
					return QString::number( hdr.ehdr32->e_ident[EI_VERSION] );
				case 4:
					return QString::number( hdr.ehdr32->e_ident[EI_OSABI] );
				case 5:
					return QString::number( hdr.ehdr32->e_ident[EI_ABIVERSION] );
				case 6:
					return QString::number( hdr.ehdr32->e_type );
				case 7:
					return QString::number( hdr.ehdr32->e_machine );
				case 8:
					return QString::number( hdr.ehdr32->e_version );
				default:
					return QVariant();
			}
			break;
		default:
			return QVariant();
	}
	return QString( "N/A" );
}

QVariant EH_FileHeader::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if( role != Qt::DisplayRole )
		return QVariant();
	
	if( orientation != Qt::Vertical )
		return QString( "Value" );
	else
	{
		if( section < eh_fh::fieldnum )
			return QString( eh_fh::names[section] );
	}
	return QVariant();
}

Qt::ItemFlags EH_FileHeader::flags( const QModelIndex &index ) const
{
	if( !index.isValid() )
		return Qt::ItemIsEnabled;
	
	Qt::ItemFlags temp = QAbstractItemModel::flags( index );
	
	if( index.row() == 0 )
		return temp; // Magic NOT editable
	
	return QAbstractItemModel::flags( index ); //return temp | Qt::ItemIsEditable;
}

bool EH_FileHeader::setData( const QModelIndex &index, const QVariant &value, int role )
{
	if( index.isValid() && role == Qt::EditRole )
	{
		switch( index.row() )
		{
			case 0:
				qDebug() << value;
				break;
			//TODO
			default:
				return false;
		}
		emit dataChanged( index, index );
		return true;
	}
	return false;
}

int EH_FileHeader::IsELF64( void *data )
{
	if( data == NULL )
		return -1;
	
	Elf32_Ehdr *ehdr = (Elf32_Ehdr *) data;
	
	if( ehdr->e_ident[EI_CLASS] == 2 )
		return 1;
	else
		return 0;
	
	return -1;
}

EH_off_t EH_FileHeader::GetPhdrOffset( void *data )
{
	int is64bit = IsELF64( data );
	
	if( data == NULL || is64bit == -1 )
		return 0;
	
	if( is64bit )
	{
		Elf64_Ehdr *ehdr64 = (Elf64_Ehdr *) data;
		return ehdr64->e_phoff;
	}
	else
	{
		Elf32_Ehdr *ehdr = (Elf32_Ehdr *) data;
		return ehdr->e_phoff;
	}
	return 0;
}

EH_off_t EH_FileHeader::GetShdrOffset( void *data )
{
	int is64bit = IsELF64( data );
	
	if( data == NULL || is64bit == -1 )
		return 0;
	
	if( is64bit )
	{
		Elf64_Ehdr *ehdr64 = (Elf64_Ehdr *) data;
		return ehdr64->e_shoff;
	}
	else
	{
		Elf32_Ehdr *ehdr = (Elf32_Ehdr *) data;
		return ehdr->e_shoff;
	}
	return 0;
}

EH_size_t EH_FileHeader::GetPhdrNum( void *data )
{
	int is64bit = IsELF64( data );
	
	if( data == NULL || is64bit == -1 )
		return 0;
	
	if( is64bit )
	{
		Elf64_Ehdr *ehdr64 = (Elf64_Ehdr *) data;
		return ehdr64->e_phnum;
	}
	else
	{
		Elf32_Ehdr *ehdr = (Elf32_Ehdr *) data;
		return ehdr->e_phnum;
	}
	return 0;
}

EH_size_t EH_FileHeader::GetShdrNum( void *data )
{
	int is64bit = IsELF64( data );
	
	if( data == NULL || is64bit == -1 )
		return 0;
	
	if( is64bit )
	{
		Elf64_Ehdr *ehdr64 = (Elf64_Ehdr *) data;
		return ehdr64->e_shnum;
	}
	else
	{
		Elf32_Ehdr *ehdr = (Elf32_Ehdr *) data;
		return ehdr->e_shnum;
	}
	return 0;
}


EH_FieldInfo EH_FileHeader::GetFieldInfo( QModelIndex &index ) const
{
	EH_FieldInfo fi = {0,0};
	bool is64bit = (Bits()==64);

	switch( index.row() )
	{
		case 0:
			fi.start = offsetof( Elf32_Ehdr, e_ident[EI_MAG0] );
			fi.size = SELFMAG;
			break;
		case 1:
			fi.start = offsetof( Elf32_Ehdr, e_ident[EI_CLASS] );
			fi.size = sizeof( Elf32_Ehdr::e_ident[EI_CLASS] );
			break;
		case 2:
			fi.start = offsetof( Elf32_Ehdr, e_ident[EI_DATA] );
			fi.size = sizeof( Elf32_Ehdr::e_ident[EI_DATA] );
			break;
		case 3:
			fi.start = offsetof( Elf32_Ehdr, e_ident[EI_VERSION] );
			fi.size = sizeof( Elf32_Ehdr::e_ident[EI_VERSION] );
			break;
		case 4:
			fi.start = offsetof( Elf32_Ehdr, e_ident[EI_OSABI] );
			fi.size = sizeof( Elf32_Ehdr::e_ident[EI_OSABI] );
			break;
		case 5:
			fi.start = offsetof( Elf32_Ehdr, e_ident[EI_ABIVERSION] );
			fi.size = sizeof( Elf32_Ehdr::e_ident[EI_ABIVERSION] );
			break;
		case 6:
			fi.start = offsetof( Elf32_Ehdr, e_type );
			fi.size = sizeof( Elf32_Ehdr::e_type );
			break;
		case 7:
			fi.start = offsetof( Elf32_Ehdr, e_machine );
			fi.size = sizeof( Elf32_Ehdr::e_machine );
			break;
		case 8:
			fi.start = offsetof( Elf32_Ehdr, e_version );
			fi.size = sizeof( Elf32_Ehdr::e_version );
			break;
		case 9:
			if( is64bit )
			{
				fi.start = offsetof( Elf64_Ehdr, e_entry );
				fi.size = sizeof( Elf64_Ehdr::e_entry );
			}
			else
			{
				fi.start = offsetof( Elf32_Ehdr, e_entry );
				fi.size = sizeof( Elf32_Ehdr::e_entry );
			}
			break;
		case 10:
			if( is64bit )
			{
				fi.start = offsetof( Elf64_Ehdr, e_phoff );
				fi.size = sizeof( Elf64_Ehdr::e_phoff );
			}
			else
			{
				fi.start = offsetof( Elf32_Ehdr, e_phoff );
				fi.size = sizeof( Elf32_Ehdr::e_phoff );
			}
			break;
		case 11:
			if( is64bit )
			{
				fi.start = offsetof( Elf64_Ehdr, e_shoff );
				fi.size = sizeof( Elf64_Ehdr::e_shoff );
			}
			else
			{
				fi.start = offsetof( Elf32_Ehdr, e_shoff );
				fi.size = sizeof( Elf32_Ehdr::e_shoff );
			}
			break;
		case 12:
			if( is64bit )
			{
				fi.start = offsetof( Elf64_Ehdr, e_flags );
				fi.size = sizeof( Elf64_Ehdr::e_flags );
			}
			else
			{
				fi.start = offsetof( Elf32_Ehdr, e_flags );
				fi.size = sizeof( Elf32_Ehdr::e_flags );
			}
			break;
		case 13:
			if( is64bit )
			{
				fi.start = offsetof( Elf64_Ehdr, e_ehsize );
				fi.size = sizeof( Elf64_Ehdr::e_ehsize );
			}
			else
			{
				fi.start = offsetof( Elf32_Ehdr, e_ehsize );
				fi.size = sizeof( Elf32_Ehdr::e_ehsize );
			}
			break;
		case 14:
			if( is64bit )
			{
				fi.start = offsetof( Elf64_Ehdr, e_phentsize );
				fi.size = sizeof( Elf64_Ehdr::e_phentsize );
			}
			else
			{
				fi.start = offsetof( Elf32_Ehdr, e_phentsize );
				fi.size = sizeof( Elf32_Ehdr::e_phentsize );
			}
			break;
		case 15:
			if( is64bit )
			{
				fi.start = offsetof( Elf64_Ehdr, e_phnum );
				fi.size = sizeof( Elf64_Ehdr::e_phnum );
			}
			else
			{
				fi.start = offsetof( Elf32_Ehdr, e_phnum );
				fi.size = sizeof( Elf32_Ehdr::e_phnum );
			}
			break;
		case 16:
			if( is64bit )
			{
				fi.start = offsetof( Elf64_Ehdr, e_shentsize );
				fi.size = sizeof( Elf64_Ehdr::e_shentsize );
			}
			else
			{
				fi.start = offsetof( Elf32_Ehdr, e_shentsize );
				fi.size = sizeof( Elf32_Ehdr::e_shentsize );
			}
			break;
		case 17:
			if( is64bit )
			{
				fi.start = offsetof( Elf64_Ehdr, e_shnum );
				fi.size = sizeof( Elf64_Ehdr::e_shnum );
			}
			else
			{
				fi.start = offsetof( Elf32_Ehdr, e_shnum );
				fi.size = sizeof( Elf32_Ehdr::e_shnum );
			}
			break;
		case 18:
			if( is64bit )
			{
				fi.start = offsetof( Elf64_Ehdr, e_shstrndx );
				fi.size = sizeof( Elf64_Ehdr::e_shstrndx );
			}
			else
			{
				fi.start = offsetof( Elf32_Ehdr, e_shstrndx );
				fi.size = sizeof( Elf32_Ehdr::e_shstrndx );
			}
			break;
	}
	return fi;
}
