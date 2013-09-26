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
 * Filename: ElfHunter.h
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */

#ifndef ELFHUNTER_H
#define ELFHUNTER_H

#include "../Model/EH_ElfHunter.h"
#include <unistd.h>
#include <getopt.h>

#define EH_VERSION "0.2a"

// Main object
EH_ElfHunter eh; ///< Main application object

// Command line facilities
const struct option eh_cmdline_opt[] =
{
	{ "help",0, NULL, 'h' },
	{ NULL, 0, NULL, 0 }
};

// Prototypes
/** @brief Shows command-line help
 *
 * Display information for the invocation of ElfHunter from command line */
void EH_Main_PrintHelp();

#endif
