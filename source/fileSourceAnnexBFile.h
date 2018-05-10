/*  This file is part of YUView - The YUV player with advanced analytics toolset
*   <https://github.com/IENT/YUView>
*   Copyright (C) 2015  Institut für Nachrichtentechnik, RWTH Aachen University, GERMANY
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 3 of the License, or
*   (at your option) any later version.
*
*   In addition, as a special exception, the copyright holders give
*   permission to link the code of portions of this program with the
*   OpenSSL library under certain conditions as described in each
*   individual source file, and distribute linked combinations including
*   the two.
*   
*   You must obey the GNU General Public License in all respects for all
*   of the code used other than OpenSSL. If you modify file(s) with this
*   exception, you may extend this exception to your version of the
*   file(s), but you are not obligated to do so. If you do not wish to do
*   so, delete this exception statement from your version. If you delete
*   this exception statement from all source files in the program, then
*   also delete it here.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FILESOURCEANNEXBFILE_H
#define FILESOURCEANNEXBFILE_H

#include <QAbstractItemModel>
#include "parserAnnexB.h"
#include "fileSource.h"
#include "videoHandlerYUV.h"

using namespace YUV_Internals;

// Internally, we use a buffer which we only update if necessary
#define BUFFER_SIZE 500000

/* This class is a normal fileSource for opening of raw AnnexBFiles.
 * Basically it understands that this is a binary file where each unit starts with a start code (0x0000001)
 * TODO: The reading / parsing could be performed in a background thread in order to increase the performance
*/
class fileSourceAnnexBFile : public fileSource
{
  Q_OBJECT

public:
  fileSourceAnnexBFile();
  fileSourceAnnexBFile(const QString &filePath) : fileSourceAnnexBFile() { openFile(filePath); }
  ~fileSourceAnnexBFile();

  // Open the given file. If another file is given, 
  bool openFile(const QString &filePath) Q_DECL_OVERRIDE;

  // Is the file at the end?
  bool atEnd() const Q_DECL_OVERRIDE { return fileBufferSize < BUFFER_SIZE; }

  // --- Retrieving of data from the file ---
  // You can either read a file NAL by NAL or frame by frame. Do not mix the two interfaces.
  // TODO: We could always use the second option, right? Also for the libde265 and HM decoder this should work.

  // Get the next NAL unit (everything excluding the start code)
  // Also return the start and end position of the NAL unit in the file so you can seek to it.
  QByteArray getNextNALUnit(bool startCode=false, uint64_t *posInFile=nullptr, uint64_t *endPosInFile=nullptr);

  // Get all bytes (NAL units including header bytes) that are needed to decode the next frame (all NAL units).
  QByteArray getNextFrameNALUnits();
  
  // Seek the file to the given byte position. Update the buffer.
  bool seek(int64_t pos) Q_DECL_OVERRIDE;

protected:

  QByteArray   fileBuffer;
  uint64_t     fileBufferSize;       ///< How many of the bytes are used? We don't resize the fileBuffer
  unsigned int posInBuffer;          ///< The current position in the input buffer in bytes
  uint64_t     bufferStartPosInFile; ///< The byte position in the file of the start of the currently loaded buffer

  // The start code pattern
  QByteArray startCode;

  // load the next buffer
  bool updateBuffer();
};

#endif //FILESOURCEANNEXBFILE_H

