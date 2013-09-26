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
 * Filename: EH_FileHeader.h
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#ifndef EH_FileHeader_H
#define EH_FileHeader_H

#include <QtCore>
#include "../Global/EH_types.h"
#include "EH_Model.h"
#include <vector>

/** @class EH_FileHeader
 * @brief Model class for Elfxx_Ehdr headers */
class EH_FileHeader : public EH_Model
{
	private:
		uElf_Ehdr hdr; ///< data pointers
		EH_size_t total_file_size; ///< total length of file in bytes
	public:
		EH_FileHeader();
		EH_FileHeader( void *data, EH_size_t size );
		~EH_FileHeader();

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
		 * @param[in] size filesize
		 * @return true on success, false otherwise */
		bool Set( void *data, EH_size_t size );

		/** Checks the file for validity
		 * @return true on success, false otherwise */
		bool Valid() const;

		/** Return the architecture type
		 * @return a value of 32 or 64. On error, 0 is returned */
		int Bits() const;
		
		/** @brief Static Method. Returns the architecture type
		 * @param[in] data Pointer to raw data
		 * @return -1 on error. if 32-bit, it returns 0. if 64-bit, it returns 1 */
		static int IsELF64( void *data );

		/** @brief Gets the Program Header offset
		 * @param[in] data Pointer to raw data
		 * @return The Program Header offset */
		static EH_off_t GetPhdrOffset( void *data );

		/** @brief Gets the Section Header offset
		 * @param[in] data Pointer to raw data
		 * @return The Section Header offset */
		static EH_off_t GetShdrOffset( void *data );

		/** @brief Gets the number of Program Headers
		 * @param[in] data Pointer to raw data
		 * @return The number of Program Headers */
		static EH_size_t GetPhdrNum( void *data );

		/** @brief Gets the number of Section Headers
		 * @param[in] data Pointer to raw data
		 * @return The number of Section Headers */
		static EH_size_t GetShdrNum( void *data );
};

#endif
