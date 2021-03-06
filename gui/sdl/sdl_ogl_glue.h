//
//   Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010,
//   2011 Free Software Foundation, Inc
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "sdl_glue.h"
#include "opengl/Renderer_ogl.h"

namespace gnash
{

class SdlOglGlue : public SdlGlue
{
  public:
    SdlOglGlue();
    virtual ~SdlOglGlue();

    bool init(int argc, char **argv[]);
    Renderer* createRenderHandler( int depth);
    void setInvalidatedRegions(const InvalidatedRanges& ranges);
    bool prepDrawingArea(int width, int height, std::uint32_t sdl_flags);
    void render();
};

}
