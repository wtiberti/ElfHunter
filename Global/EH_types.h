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
 * Filename: EH_types.h
 * Author: Walter Tiberti <wtuniv@gmail.com>
 *
 */
#ifndef EH_TYPES_H
#define EH_TYPES_H

#include <unistd.h>
#include <elf.h>

// Generic data types
typedef __uint64_t EH_off_t;
typedef __uint64_t EH_size_t;
typedef __uint64_t EH_index_t;

/** Union for Elf file headers */
union uElf_Ehdr
{
	Elf32_Ehdr *ehdr32;
	Elf64_Ehdr *ehdr64;
};

/** Union for Elf program headers */
union uElf_Proghdr
{
	Elf32_Phdr *phdr32;
	Elf64_Phdr *phdr64;
};

/** Union for Elf section headers */
union uElf_Secthdr
{
	Elf32_Shdr *shdr32;
	Elf64_Shdr *shdr64;
};

/** Union for Elf note headers */
union uElf_Notehdr
{
	Elf32_Nhdr *nhdr32;
	Elf64_Nhdr *nhdr64;
};

/** Union for Elf dynamic headers */
union uElf_Dynhdr
{
	Elf32_Dyn *dyn32;
	Elf64_Dyn *dyn64;
};

/** hexdump-marking information structure */
typedef struct
{
	EH_off_t start;
	EH_size_t size;
} EH_FieldInfo;

/** Union for Elf symbol/dynsym headers */
union uElf_Sym
{
	Elf32_Sym *sym32;
	Elf64_Sym *sym64;
};

/** Union for Elf relocation(rel) headers */
union uElf_Rel
{
	Elf32_Rel *rel32;
	Elf64_Rel *rel64;
};

/** Union for Elf relocation(rela) headers */
union uElf_Rela
{
	Elf32_Rela *rela32;
	Elf64_Rela *rela64;
};

#endif
