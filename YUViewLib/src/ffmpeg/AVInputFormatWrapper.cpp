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

#include "AVInputFormatWrapper.h"

namespace FFmpeg
{

namespace
{

typedef struct AVInputFormat_56_57_58_59_60
{
  const char *                    name;
  const char *                    long_name;
  int                             flags;
  const char *                    extensions;
  const struct AVCodecTag *const *codec_tag;
  const AVClass *                 priv_class;
  const char *                    mime_type;

  // There is more but it is not part of the public ABI
} AVInputFormat_56_57_58_59_60;

} // namespace

AVInputFormatWrapper::AVInputFormatWrapper()
{
  this->fmt = nullptr;
}

AVInputFormatWrapper::AVInputFormatWrapper(AVInputFormat *f, LibraryVersion v)
{
  this->fmt    = f;
  this->libVer = v;
  this->update();
}

void AVInputFormatWrapper::update()
{
  if (this->fmt == nullptr)
    return;

  if (this->libVer.avformat.major == 56 || //
      this->libVer.avformat.major == 57 || //
      this->libVer.avformat.major == 58 || //
      this->libVer.avformat.major == 59 || //
      this->libVer.avformat.major == 60)
  {
    auto p           = reinterpret_cast<AVInputFormat_56_57_58_59_60 *>(this->fmt);
    this->name       = QString(p->name);
    this->long_name  = QString(p->long_name);
    this->flags      = p->flags;
    this->extensions = QString(p->extensions);
    this->mime_type  = QString(p->mime_type);
  }
}

} // namespace FFmpeg
