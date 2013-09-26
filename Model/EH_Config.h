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
 * Filename: EH_Config.h
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#ifndef EH_Config_H
#define EH_Config_H

#include <QtCore>
/** @class EH_Config
  * @brief Handles options and settings */
class EH_Config
{
	private:
		char *conf_file; ///< name of the config file

		unsigned eh_window_height; ///< window height
		unsigned eh_window_width; ///< window width

		QStack< QString > recent_files; ///< list of last opened files
	public:
		EH_Config();
		EH_Config( const char *conf_filename );
		
		/** @brief Load a configuration file
		 * @param[in] conf_filename path of the conf file
		 * @return true on success, false otherwise */
		bool Load( const char *conf_filename );

		/** @brief Write the config file
		 * @return true on success, false otherwise */
		bool Write();

		/** @brief Set window height
		 * @param[in] h new height */
		void SetWindowHeight( unsigned h );

		/** @brief Get window height
		 * @return height from config file */
		unsigned int GetWindowHeight();

		/** @brief Set window width
		 * @param[in] w new width */
		void SetWindowWidth( unsigned w );

		/** @brief Get window width
		 * @return width from config file */
		unsigned int GetWindowWidth();

		/** @brief Get recent-files path
		 * @param[in] index Index to the recent file list
		 * @return path of the recent-file */
		QString GetRecentFile( unsigned index );

		/** @brief Insert a new entry in the recent file list
		 * @param[in] filename path to the filename to insert
		 * @return true on success, false otherwise */
		bool PushRecentFile( QString filename );
};

#endif
