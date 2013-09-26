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
 * Filename: EH_SectionHeader.h
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#ifndef EH_SectionHeader_h
#define EH_SectionHeader_h

#include <QtCore>
#include "../Global/EH_types.h"
#include "EH_Model.h"

class EH_SectionHeader : public EH_Model
{
	private:
		char *base_data;
		uElf_Secthdr hdr;
		EH_index_t nHeaders;
		bool is64bit;
		EH_off_t base_offset;
	public:
		EH_SectionHeader();
		EH_SectionHeader( void *data );
		~EH_SectionHeader();
		
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

		/** @brief Return the number of Elfxx_Shdr structs
		 * @return The number of Elfxx_Shdr structs */
		EH_size_t GetNumOfSections() const;

		/** @brief Retrieve the raw pointer to a section's data
		 * @param[in] index Index of the section
		 * @return the pointer to section's data on success. NULL on failure */
		void *GetSection( EH_index_t index );

		/** @brief Retrieve sections names
		 * @param[in] data base data pointer (file start)
		 * @return The pointer to the name of the section on success. NULL otherwise */
		static char *GetNameSection( void *data );
};

#endif
