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
 * Filename: EH_SECT_sym.h
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#ifndef EH_SECT_sym_h
#define EH_SECT_sym_h

#include <QtCore>
#include "../Global/EH_types.h"
#include "EH_Section.h"
 
/** @class EH_SECT_sym
 * @brief Handles symbol and dynamic symbol sections */
class EH_SECT_sym : public EH_Section
{
	private:
		uElf_Sym sym; ///< Raw data pointers
		EH_index_t nSyms; ///< Number of symbols
		char *names_ptr; ///< Pointer to symbols names

	public:
		EH_SECT_sym();
		EH_SECT_sym( const char *data, uElf_Secthdr u );
		~EH_SECT_sym();

		/** @ref EH_Model::GetFieldInfo */
		EH_FieldInfo GetFieldInfo( QModelIndex &index ) const;

		int rowCount( const QModelIndex &parent = QModelIndex() ) const;
		int columnCount( const QModelIndex &parent = QModelIndex() ) const;
		QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
		QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		Qt::ItemFlags flags( const QModelIndex &index ) const;
		bool setData( const QModelIndex &index, const QVariant &value, int role );

		/** @brief Retrieve the name of a symbol
		 * @param[in] sectn Number of the associated string section
		 * @param[in] symn Symbol number
		 * @param[in] data Base data pointer (file start)
		 * @return The pointer to the symbol name on success. NULL otherwise */
		static char *GetSymbolName( EH_index_t sectn, EH_index_t symn, void *data );
};

#endif
