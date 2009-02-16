/*
 * Tiled Map Editor (Qt)
 * Copyright 2009 Tiled (Qt) developers (see AUTHORS file)
 *
 * This file is part of Tiled (Qt).
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef TILEPAINTER_H
#define TILEPAINTER_H

namespace Tiled {

class Tile;
class TileLayer;

namespace Internal {

class MapDocument;

/**
 * The tile painter is meant for painting tiles on a tile layer. It makes sure
 * that each paint operation sends out the proper events, so that views can
 * redraw the changed parts. It also does bounds checking.
 */
class TilePainter
{
public:
    /**
     * Constructs a tile painter.
     *
     * @param mapDocument the map document to send change notifications to
     * @param tileLayer   the tile layer to edit
     */
    TilePainter(MapDocument *mapDocument, TileLayer *tileLayer);

    /**
     * Returns the tile at the given coordinates. The coordinates are relative
     * to the map origin. Returns 0 if the coordinates lay outside of the
     * layer.
     */
    Tile *tileAt(int x, int y);

    /**
     * Sets the tile for the given coordinates. The coordinates are relative to
     * the map origin.
     */
    void setTile(int x, int y, Tile *tile);

    // TODO: Add more operations (fill, copy)

private:
    MapDocument *mMapDocument;
    TileLayer *mTileLayer;
};

} // namespace Tiled
} // namespace Internal

#endif // TILEPAINTER_H