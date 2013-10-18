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
 * Filename: EH_File.h
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#ifndef EH_File_H
#define EH_File_H

#include "../Global/EH_types.h"
#include <vector>
#include <string>

#include <cstdio>
#include <cstring>

#include "sys/stat.h" // Used for 'stat' function

#include "EH_Patch.h"
#include "EH_Section.h"
#include "EH_Segment.h"
#include "EH_FileHeader.h"
#include "EH_SectionHeader.h"
#include "EH_ProgramHeader.h"

/** @class EH_File
 * @brief High-level file representation */
class EH_File
{
	private:
		int fd; ///< File descriptor
		bool delete_on_free; ///< flag. If true, the object is allocated and its memory must be free'd after program execution
		
		std::string filename; ///< full filename (path+filename)
		std::string shortname; ///< filename (no path)
		char *data; ///< pointer to file data
		bool is_open; ///< flag. If true, the file is opened and ready to be read
		bool read_only; ///< flag. If true, the file is read only and can't be edited
		
		struct stat file_stat; ///< stat struct of the file
		
		std::vector< EH_Patch* > patches; ///< List of @ref EH_Patch object assiociated to the file
		
		// File high-level basic structure
		EH_FileHeader fileheader; ///< File header (Elfxx_Ehdr)
		EH_ProgramHeader progheader; ///< Program Header (Elfxx_Phdr)
		EH_SectionHeader sectheader; ///< Section Header (Elfxx_Shdr)
		std::vector< EH_Section* > sections; ///< List of file's sections
		std::vector< EH_Segment* > segments; ///< List of file's segments

	public:
		EH_File( bool autodelete=false );
		EH_File( const std::string &path, bool rd_only = false, bool autodelete=false );
		~EH_File();
		
		/** @brief Checks if a file exists or not*/
		bool Exists() const;
		
		/** @brief Open a file
		 * @param[in] path Path to the file
		 * @param[in] rd_only Open as read-only?
		 * @return true on success, false otherwise */
		bool Open( const std::string &path, bool rd_only = false );

		/** @brief Maps file in memory
		 * @return Pointer to file data on success. false otherwise */
		char *Map();

		/** @brief Un-Maps the file
		 * @return On error, return -1 */
		int UnMap();

		/** @brief Close the file
		 * @return true on success, false otherwise */
		bool Close();

		/** @brief Set current filename
		 * @param[in] path Path to the file
		 * @return true on success, false otherwise */
		bool SetFileName( const std::string &path );

		/** @brief Gets full filename (path+name)
		 * @return The full filename */
		std::string GetFileName() const;

		/** @brief Gets short filename (without path)
		 * @return The short filename */
		std::string GetShortName() const;

		/** @brief Gets the file data pointer
		 * @return The pointer to file data on success, NULL otherwise */
		char *GetData() const;

		/** @brief Checks if the file can be read
		 * @return true if the file can be read. False otherwise */
		bool IsReady() const;

		/** @brief Checks if the file was opened as read-only
		 * @return true if read-only, false otherwise */
		bool IsReadOnly() const;

		/** @brief Gets the size in bytes of the file
		 * @return The size in bytes of the file */
		EH_size_t GetFileSize() const;
		
		/** @brief Gets the pointer to the @ref EH_FileHeader object
		 * @return The pointer to the @ref EH_FileHeader of the file */
		EH_FileHeader *GetFileHeader();

		/** @brief Gets the pointer to the @ref EH_ProgramHeader object
		 * @return The pointer to the @ref EH_ProgramHeader of the file */
		EH_ProgramHeader *GetProgHeader();

		/** @brief Gets the pointer to the @ref EH_SectionHeader object
		 * @return The pointer to the @ref EH_SectionHeader of the file */
		EH_SectionHeader *GetSectHeader();

		/** @brief Retrieve the value of the e_type field
		 * @param[in] data Pointer to raw data of the Elf file
		 * @return The value of the e_type field in the ElfXX_Ehdr struct */
		static int GetElfType( const char *data );

		/** @brief Checks if the object can be free'd on deletion
		 * @return true if can be free'd, false otherwise */
		bool CanDelete() const;
};

#endif
