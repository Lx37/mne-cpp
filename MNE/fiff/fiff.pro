#--------------------------------------------------------------------------------------------------------------
#
# @file     fiff.pro
# @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
#           Matti H�m�l�inen <msh@nmr.mgh.harvard.edu>
# @version  1.0
# @date     July, 2012
#
# @section  LICENSE
#
# Copyright (C) 2012, Christoph Dinh and Matti Hamalainen. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that
# the following conditions are met:
#     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
#       following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
#       the following disclaimer in the documentation and/or other materials provided with the distribution.
#     * Neither the name of the Massachusetts General Hospital nor the names of its contributors may be used
#       to endorse or promote products derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MASSACHUSETTS GENERAL HOSPITAL BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
#
# @brief    ToDo Documentation...
#
#--------------------------------------------------------------------------------------------------------------


TEMPLATE = lib

QT       -= gui

DEFINES += FIFF_LIBRARY

CONFIG(debug, debug|release) {
    TARGET = fiffd
    unix:DESTDIR = $$PWD/../../lib/unix/debug
    win32:DESTDIR = $$PWD/../../lib/win32/debug
    win32:QMAKE_POST_LINK += xcopy /y "..\\..\\..\\mne-cpp\\lib\\win32\\debug\\fiffd.dll" "..\\..\\..\\mne-cpp\\bin\\win32\\debug\\"
}
else {
    TARGET = fiff
    unix:DESTDIR = $$PWD/../../lib/unix/release
    win32:DESTDIR = $$PWD/../../lib/win32/release
    win32:QMAKE_POST_LINK += xcopy /y "..\\..\\..\\mne-cpp\\lib\\win32\\release\\fiff.dll" "..\\..\\..\\mne-cpp\\bin\\win32\\release\\"
}


SOURCES += fiff.cpp \
#    src/fiff_parser.cpp \
    src/fiff_tag.cpp \
    src/fiff_dir_tree.cpp \
    src/fiff_solution.cpp \
    src/fiff_coord_trans.cpp

HEADERS += fiff.h \
        fiff_global.h \
    include/fiff_types.h \
#    include/fiff_parser.h \
#    include/fiff_file.h \
#    include/allocs.h \
#    include/dot.h \
    include/fiff_constants.h \
    include/fiff_tag.h \
    include/fiff_dir_tree.h \
    include/fiff_solution.h \
    include/fiff_coord_trans.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE05B73F1
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = fiff.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}