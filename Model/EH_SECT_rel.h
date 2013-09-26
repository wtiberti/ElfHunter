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
 * Filename: EH_SECT_rel.h
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#ifndef EH_SECT_rel_h
#define EH_SECT_rel_h

#include <QtCore>
#include "../Global/EH_types.h"
#include "EH_Section.h"

/** @class EH_SECT_rel
 * @brief Handles relocation sections (Elfxx_Rel) */
class EH_SECT_rel : public EH_Section
{
	private:
		uElf_Rel rel; ///< Raw data pointers
		EH_index_t nRelocs; ///< Number of relocations in the section

	public:
		EH_SECT_rel();
		EH_SECT_rel( const char *data, uElf_Secthdr u );
		~EH_SECT_rel();

		/** @ref EH_Model::GetFieldInfo */
		EH_FieldInfo GetFieldInfo( QModelIndex &index ) const;

		int rowCount( const QModelIndex &parent = QModelIndex() ) const;
		int columnCount( const QModelIndex &parent = QModelIndex() ) const;
		QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
		QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		Qt::ItemFlags flags( const QModelIndex &index ) const;
		bool setData( const QModelIndex &index, const QVariant &value, int role );
};

#endif
