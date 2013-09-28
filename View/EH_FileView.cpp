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
 * Filename: EH_FileView.cpp
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#include "EH_FileView.h"
#include "Model/EH_SEG_interp.h"
#include "Model/EH_SEG_note.h"
#include "Model/EH_SEG_dyn.h"
#include "Model/EH_SECT_sym.h"
#include "Model/EH_SECT_str.h"
#include "Model/EH_SECT_rel.h"
#include "Model/EH_SECT_rela.h"
#include "Model/EH_SECT_hash.h"

EH_FileView::EH_FileView( QWidget *parent )
 :QWidget( parent )
{
	ui_fileview.setupUi( this );
	actual_file = NULL;
	models.clear();
	garbage.clear();
	ui_fileview.hexdump->setByteArrayModel( &hexmodel );
	connect( ui_fileview.tableView,
			SIGNAL( clicked( const QModelIndex & ) ),
			this, SLOT( MarkBytes( const QModelIndex & ) ) );
}

EH_FileView::~EH_FileView()
{
	models.clear();
	
	
	for( unsigned i=0; i<garbage.size(); i++ )
	{
		delete garbage[ i ];
	}
	garbage.clear();
}

int EH_FileView::SetFile( EH_File *f )
{
	ui_fileview.comboLabel->setText( QString( "File: " ) + f->GetFileName().c_str() );
	
	ui_fileview.size_label->setText( QString::number( f->GetFileSize() ) + " bytes" );

	hexmodel.setData( QByteArray::fromRawData( f->GetData(), f->GetFileSize() ) );
	ui_fileview.hexdump->setCursorPosition( 0 );
	
	models.clear();
	if( f->GetFileHeader()->Valid() )
	{
		EH_FileHeader *viewfh = f->GetFileHeader();
		models.push_back( viewfh );
		ui_fileview.modelCombo->addItem( "ELF Header", QVariant( (int)models.size()-1 ) );

		bool is64bit = viewfh->Bits() == 64;
		
		EH_ProgramHeader *viewph = f->GetProgHeader();
		models.push_back( viewph );
		ui_fileview.modelCombo->addItem( "---- Program Headers ----", QVariant( (int)models.size()-1 ) );

		for( unsigned i = 0; i<viewph->GetNumOfSegments(); i++ )
		{
			void *entry = viewph->GetSegment( i );
			uElf_Proghdr u_entry;
			int segtype;
			
			if( entry == NULL )
				continue;
			
			if( is64bit )
			{
				u_entry.phdr64 = (Elf64_Phdr*)entry;
				segtype = u_entry.phdr64->p_type;
				
			}
			else
			{
				u_entry.phdr32 = (Elf32_Phdr*)entry;
				segtype = u_entry.phdr32->p_type;
			}
			
			EH_Segment* viewseg;
			switch( segtype )
			{
				case PT_INTERP:
					viewseg = new EH_SEG_interp( f->GetData(), u_entry );
					garbage.push_back( viewseg );
					models.push_back( viewseg );
					ui_fileview.modelCombo->addItem( "p#" + QString::number( i ) + " - Interpreter", QVariant( (int)models.size()-1 ) );
					break;
				case PT_NOTE:
					viewseg = new EH_SEG_note( f->GetData(), u_entry );
					garbage.push_back( viewseg );
					models.push_back( viewseg );
					ui_fileview.modelCombo->addItem( "p#" + QString::number( i ) + " - Note", QVariant( (int)models.size()-1 ) );
					break;
				case PT_DYNAMIC:
					viewseg = new EH_SEG_dyn( f->GetData(), u_entry );
					garbage.push_back( viewseg );
					models.push_back( viewseg );
					ui_fileview.modelCombo->addItem( "p#" + QString::number( i ) + " - Dynamic", QVariant( (int)models.size()-1 ) );
					break;
			}
		}

		EH_SectionHeader *viewsh = f->GetSectHeader();
		if( viewsh->GetNumOfSections() != 0 )
		{
			models.push_back( viewsh );
			ui_fileview.modelCombo->addItem( "---- Section Headers ----", QVariant( (int)models.size()-1 ) );

			for( unsigned i = 0; i<viewsh->GetNumOfSections(); i++ )
			{
				void *entry = viewsh->GetSection( i );
				uElf_Secthdr u_entry;
				int secttype;

				if( entry == NULL )
					continue;

				if( is64bit )
				{
					u_entry.shdr64 = (Elf64_Shdr*)entry;
					secttype = u_entry.shdr64->sh_type;

				}
				else
				{
					u_entry.shdr32 = (Elf32_Shdr*)entry;
					secttype = u_entry.shdr32->sh_type;
				}

				EH_Section* viewsect;
				char *fdata = f->GetData();
				switch( secttype )
				{
					case SHT_SYMTAB:
					case SHT_DYNSYM:
						viewsect = new EH_SECT_sym( fdata, u_entry );
						garbage.push_back( viewsect );
						models.push_back( viewsect );
						ui_fileview.modelCombo->addItem( "s#" + QString::number( i ) + " - " +
								QString(
									EH_SectionHeader::GetNameSection( fdata ) +
									( is64bit ? u_entry.shdr64->sh_name : u_entry.shdr32->sh_name ) ),
								QVariant( (int)models.size()-1 ) );
						break;
					case SHT_STRTAB:
						viewsect = new EH_SECT_str( fdata, u_entry );
						garbage.push_back( viewsect );
						models.push_back( viewsect );
						ui_fileview.modelCombo->addItem( "s#" + QString::number( i ) + " - " +
								QString(
									EH_SectionHeader::GetNameSection( fdata ) +
									( is64bit ? u_entry.shdr64->sh_name : u_entry.shdr32->sh_name ) ),
								QVariant( (int)models.size()-1 ) );
						break;
					case SHT_REL:
						viewsect = new EH_SECT_rel( fdata, u_entry );
						garbage.push_back( viewsect );
						models.push_back( viewsect );
						ui_fileview.modelCombo->addItem( "s#" + QString::number( i ) + " - " +
								QString(
									EH_SectionHeader::GetNameSection( fdata ) +
									( is64bit ? u_entry.shdr64->sh_name : u_entry.shdr32->sh_name ) ),
								QVariant( (int)models.size()-1 ) );
						break;
					case SHT_RELA:
						viewsect = new EH_SECT_rela( fdata, u_entry );
						garbage.push_back( viewsect );
						models.push_back( viewsect );
						ui_fileview.modelCombo->addItem( "s#" + QString::number( i ) + " - " +
								QString(
									EH_SectionHeader::GetNameSection( fdata ) +
									( is64bit ? u_entry.shdr64->sh_name : u_entry.shdr32->sh_name ) ),
								QVariant( (int)models.size()-1 ) );
						break;
					case SHT_HASH:
						viewsect = new EH_SECT_hash( fdata, u_entry );
						garbage.push_back( viewsect );
						models.push_back( viewsect );
						ui_fileview.modelCombo->addItem( "s#" + QString::number( i ) + " - " +
								QString(
									EH_SectionHeader::GetNameSection( fdata ) +
									( is64bit ? u_entry.shdr64->sh_name : u_entry.shdr32->sh_name ) ),
								QVariant( (int)models.size()-1 ) );
						break;
				}
			}
		}
	}
	
	actual_file = f;
	return 0;
}

int EH_FileView::SetTableModel( EH_Model *m )
{
	ui_fileview.tableView->setModel( m );
	return 0;
}

void EH_FileView::on_modelCombo_currentIndexChanged( int index )
{
	if( index < (int)models.size() )
	{
		SetTableModel( models[index] );
	}
	
	ui_fileview.tableView->resizeColumnsToContents();
	ui_fileview.tableView->resizeRowsToContents();
}

bool EH_FileView::Hex_SelectRange( EH_off_t offset, EH_size_t size )
{
	EH_size_t filesize = actual_file->GetFileSize();
	EH_off_t last_offset = offset + size;

	if( offset >= filesize || last_offset >= filesize )
		return false;
	
	Okteta::AddressRange ar( offset, offset+size-1 );
	ui_fileview.hexdump->setMarking( ar );
	ui_fileview.hexdump->ensureVisible( ar, true );
	return true;
}

bool EH_FileView::Hex_GoTo( EH_off_t offset )
{
	if( offset >= actual_file->GetFileSize() )
		return false;

	Okteta::AddressRange ar( offset, 1 );
	
	ui_fileview.hexdump->setFocus( Qt::OtherFocusReason );
	ui_fileview.hexdump->ensureVisible( ar, true );
	ui_fileview.hexdump->setCursorPosition( offset );
	return true;
}

void EH_FileView::MarkBytes( const QModelIndex &index )
{
	EH_Model *m = (EH_Model*) ui_fileview.tableView->model();

	EH_FieldInfo fi = m->GetFieldInfo( (QModelIndex&)index );
	Hex_SelectRange( fi.start, fi.size );
}

void EH_FileView::on_goto_btn_clicked()
{
	QString entered = ui_fileview.offset_edit->text();
	bool success;
	
	EH_off_t off = entered.toULongLong( &success, 0 );

	if( success )
		Hex_GoTo( off );
	else
		qDebug() << "[EH] Goto failed. Invalid Syntax : " << entered;
}

void EH_FileView::on_hexdump_cursorPositionChanged( Okteta::Address addr )
{
	ui_fileview.offset_edit->setText( "0x" + QString::number( (EH_off_t)addr, 16 ) );
}

void EH_FileView::on_demark_clicked()
{
	Hex_SelectRange( 0, 0 );
}

