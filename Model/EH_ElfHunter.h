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
 * Filename: EH_ElfHunter.h
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */ 
#ifndef EH_ElfHunter_H
#define EH_ElfHunter_H

#include <vector>
#include "EH_File.h"
#include "EH_Config.h"

enum EH_DebugOut_style
{
	EHDBG_NOSTYLE,
	EHDBG_WARNING,
	EHDBG_ERROR
};

/** @class EH_ElfHunter
 * @brief Main application object class */
class EH_ElfHunter
{
	private:
		std::vector< EH_File* > files; ///< list of opened files
		
	public:
		EH_ElfHunter();
		~EH_ElfHunter();

		static void DebugOutput( const char *message, EH_DebugOut_style style = EHDBG_NOSTYLE );
		
		/** @brief Adds a file to the file list
		 * @param[in] f @ref EH_File object representing the file to add
		 * @return the index of the object file in the list */ 
		int AddFile( EH_File *f );

		/** @brief Retrieve a @ref EH_File object from the list
		 * @param[in] index index of the object to retrieve
		 * @return A pointer to the @ref EH_File object on success. NULL otherwise */
		EH_File *GetFile( const unsigned int index );

		/** @brief Remove a @ref EH_File object from the list
		 * @param[in] index Index to the @ref EH_File object to remove
		 * @return true on success, false otherwise */
		bool RemFile( const unsigned int index );

		/** @brief Gets the number of @ref EH_File object in the list
		 * @return The number of @ref EH_File currently in the list */
		unsigned int GetNumOfFiles();
		

		EH_Config cfg; ///< ElfHunter configuration object
};

#endif
