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
 * Filename: EH_Model.h
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#ifndef EH_MODEL_H
#define EH_MODEL_H

#include <QtCore>
#include "../Global/EH_types.h"

/** @class EH_Model
 * @brief Abstract class for further models */
class EH_Model : public QAbstractTableModel
{
	Q_OBJECT

	public:
		EH_Model();

		/** @brief Creates a @ref EH_FieldInfo strcture for the specified index
		 * @param[in] index index to the selected table cell
		 * @return a @ref EH_FieldInfo filled with values */
		virtual EH_FieldInfo GetFieldInfo( QModelIndex &index ) const = 0;

		/** Returns the number of rows */
		virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const = 0;

		/** Returns the number of columns */
		virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const = 0;

		/** Returns the right content for the selected cell */
		virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const = 0;

		/** Return the right header content for selected index */
		virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const = 0;
		
		/** Returns new flags for the selected cell */
		virtual Qt::ItemFlags flags( const QModelIndex &index ) const = 0;

		/** Write new data in the model */
		virtual bool setData( const QModelIndex &index, const QVariant &value, int role ) = 0;
};
		

#endif
