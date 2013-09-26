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
 * Filename: EH_FileView.h
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#ifndef EH_VIEW_FileView_H 
#define EH_VIEW_FileView_H

#include "../ui_EH_FileView.h"
#include "../Model/EH_File.h"
#include "../Model/EH_Model.h"
#include <okteta1/piecetablebytearraymodel.h>
#include <QtGui>
#include <vector>

/** @class EH_FileView
 * @brief Display all the information about a file
 * 
 * EH_FileView class uses a QTableView widget and a set of
 * models to display information about a chosen file.
 */
class EH_FileView : public QWidget
{
	Q_OBJECT
	
	private:
		EH_File *actual_file; ///< Pointer to the EH_File object ( the actual file )
		Ui::EH_FileView ui_fileview; ///< Qt widget
		std::vector< EH_Model * > models; ///< vector of all the possible models for the QTableViewWidget
		std::vector<  EH_Model * > garbage; ///< vector of models to be deleted on exit

		Okteta::PieceTableByteArrayModel hexmodel; ///< model for the hexdump window
		
	public:
		EH_FileView( QWidget *parent = NULL );
		~EH_FileView();
		
		/** @brief Load a file in the widget
		 * @param[in] f The EH_File object to be inserted
		 * @return 0 (todo) */
		int SetFile( EH_File *f );
		
		/** @brief Select a model from @ref models and use it in the QTableView widget
		 * @param[in] m Pointer to the EH_model object
		 * @return 0 (todo) */
		int SetTableModel( EH_Model *m );

		/** @brief Marks a range of bytes in the hexdump widget
		 * @param[in] offset Starting position
		 * @param[in] size Size (in bytes) to mark
		 * @return true on success; false otherwise */
		bool Hex_SelectRange( EH_off_t offset, EH_size_t size );
		
		/** @brief Move hexdump cursor to a location
		 * @param[in] offset Offset of the chosen location
		 * @return true on success; false otherwise */
		bool Hex_GoTo( EH_off_t offset );
		
	public slots:
		/** @brief Qt slot. Change the active model of the QTableView widget
		 * @param[in] index Index (in the @ref models vector) of the model to select */
		void on_modelCombo_currentIndexChanged( int index );
		
		/** @brief Qt slot. Mark a range of bytes
		 * @param[in] index QModelIndex of the table field */
		void MarkBytes( const QModelIndex &index );
		
		/** @brief Qt slot. Move the hexdump cursor to the location
		 * specified in the edit widget */
		void on_goto_btn_clicked();
		
		/** @brief Qt slot. Removes all the marks from the hexdump widget */
		void on_demark_clicked();
		
		/** @brief Qt slot. Update the actual cursor position in the hexudump
		 * @param addr New position */
		void on_hexdump_cursorPositionChanged( Okteta::Address addr );
};

#endif
