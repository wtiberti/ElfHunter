###################################################
#          ElfHunter Project file                 #
# Author: Walter Tiberti <wtuniv@gmail.com>       #
#                                                 #
# This file is needed for makefile re-generation  #
###################################################
TEMPLATE = app
TARGET = ElfHunter

CONFIG += qt warn_on release
MOC_DIR = ./tmp
OBJECTS_DIR = ./tmp

#DEFINES += DEBUG=1
 
INCLUDEPATH += /usr/include/okteta1

# Source code files
HEADERS +=  Global/EH_types.h \
			View/EH_MainWindow.h \
			View/EH_AboutDlg.h \
			View/EH_FileView.h \
			Model/EH_Model.h \
			Model/EH_ElfHunter.h \
			Model/EH_Config.h \
			Model/EH_File.h \
			Model/EH_Patch.h \
			Model/EH_Section.h \
			Model/EH_Segment.h \
			Model/EH_FileHeader.h \
			Model/EH_SectionHeader.h \
			Model/EH_ProgramHeader.h \
			Model/EH_SEG_interp.h \
			Model/EH_SEG_note.h \
			Model/EH_SEG_dyn.h \
			Model/EH_SECT_sym.h \
			Model/EH_SECT_str.h \
			Model/EH_SECT_rel.h \
			Model/EH_SECT_rela.h \
			Model/EH_SECT_hash.h

SOURCES +=  Control/ElfHunter.cpp \
			View/EH_MainWindow.cpp \
			View/EH_AboutDlg.cpp \
			View/EH_FileView.cpp \
			Model/EH_Model.cpp \
			Model/EH_Config.cpp \
			Model/EH_ElfHunter.cpp \
			Model/EH_File.cpp \
			Model/EH_Patch.cpp \
			Model/EH_Section.cpp \
			Model/EH_Segment.cpp \
			Model/EH_FileHeader.cpp \
			Model/EH_SectionHeader.cpp \
			Model/EH_ProgramHeader.cpp \
			Model/EH_SEG_interp.cpp \
			Model/EH_SEG_note.cpp \
			Model/EH_SEG_dyn.cpp \
			Model/EH_SECT_sym.cpp \
			Model/EH_SECT_str.cpp \
			Model/EH_SECT_rel.cpp \
			Model/EH_SECT_rela.cpp \
			Model/EH_SECT_hash.cpp

# Qt modules
#QT += xml

# Okteta widget needs these
LIBS += -lokteta1core -lokteta1gui

# Resources
FORMS += ElfHunter.ui \
		 EH_AboutDlg.ui \
		 EH_FileView.ui
RESOURCES += ElfHunter.qrc

#DESTDIR = ../
