///////////////////////////////////////////////////////////////////////////////
// Copyright 2019 Alan Chambers (unicycle.bloke@gmail.com)
//
// This file is part of yagl.
//
// yagl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// yagl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with yagl. If not, see <https://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////
#pragma once


// Wrapper for the various formats of PNG sprite sheets. 
class SpriteSheet
{

};


// Maintains a pool of open sprite sheets so that sprites can read their 
// pixels without opening and closing files a bazillion times.
class SpriteSheetReader
{



private:
    // Maps filename to SpriteSheet wrapper. Entries created on demand
    // and cached for re-use by later sprites.
    std::map<std::string, SpriteSheet> m_sprite_sheets;
};