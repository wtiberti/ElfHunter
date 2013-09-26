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
 * Filename: EH_Segment.h
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#ifndef EH_Segment_H
#define EH_Segment_H

#include <QtCore>
#include "../Global/EH_types.h"
#include "EH_Model.h"

/** @class EH_Segment
 * @brief Abstract model for ELF segments */
class EH_Segment : public EH_Model
{
	protected:
		uElf_Proghdr hdr; ///< Raw data pointer to the program header
		bool is64bit; ///< true on 64-bit files
		char *base; ///< Base data pointer (file start)
		void *segment_data; ///< Pointer to segment data
	public:
		EH_Segment();
		EH_Segment( const char *data, uElf_Proghdr u );
		~EH_Segment();
		
		/** @ref EH_Model::GetFieldInfo */
		virtual EH_FieldInfo GetFieldInfo( QModelIndex &index ) const = 0;

		virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const = 0;
		virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const = 0;
		virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const = 0;
		virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const = 0;
		virtual Qt::ItemFlags flags( const QModelIndex &index ) const = 0;
		virtual bool setData( const QModelIndex &index, const QVariant &value, int role ) = 0;
		
		/** @brief Fills the model
		 * @param[in] base_data pointer to raw data
		 * @param[in] u @ref uElf_Secthdr pointers to program header
		 * @return true on success, false otherwise */
		bool Set( const char *base_data, uElf_Proghdr u );
};

#endif
