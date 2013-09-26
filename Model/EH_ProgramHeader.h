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
 * Filename: EH_ProgramHeader.h
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#ifndef EH_ProgramHeader_h
#define EH_ProgramHeader_h

#include <QtCore>
#include "../Global/EH_types.h"
#include "EH_Model.h"

/** @class EH_ProgramHeader
 * @brief Model for Elfxx_Phdr structures */
class EH_ProgramHeader : public EH_Model
{
	private:
		uElf_Proghdr hdr; ///< Raw pointers to data
		EH_index_t nHeaders; ///< Number of Elfxx_Phdr structure found
		bool is64bit; ///< true in 64-bit files
		EH_off_t base_offset; ///< Base offset to the structure array
	public:
		EH_ProgramHeader();
		EH_ProgramHeader( void *data );
		~EH_ProgramHeader();
		
		/** @ref EH_Model::GetFieldInfo */
		EH_FieldInfo GetFieldInfo( QModelIndex &index ) const;

		int rowCount( const QModelIndex &parent = QModelIndex() ) const;
		int columnCount( const QModelIndex &parent = QModelIndex() ) const;
		QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
		QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		Qt::ItemFlags flags( const QModelIndex &index ) const;
		bool setData( const QModelIndex &index, const QVariant &value, int role );
		
		/** @brief Fills the model
		 * @param[in] data pointer to raw data
		 * @return true on success, false otherwise */
		bool Set( void *data );

		/** @brief Return the number of Elfxx_Phdr structs
		 * @return The number of Elfxx_Phdr structs */
		EH_size_t GetNumOfSegments() const;

		/** @brief Retrieve the raw pointer to a segment's data
		 * @param[in] index Index of the segment
		 * @return the pointer to segment's data on success. NULL on failure */
		void *GetSegment( EH_index_t index );
};

#endif
