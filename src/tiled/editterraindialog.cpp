/*
 * editterraindialog.cpp
 * Copyright 2012, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 *
 * This file is part of Tiled.
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
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "editterraindialog.h"
#include "ui_editterraindialog.h"

#include "addremoveterrain.h"
#include "mapdocument.h"
#include "terrain.h"
#include "terrainmodel.h"
#include "tileset.h"

#include <QUndoStack>

using namespace Tiled;
using namespace Tiled::Internal;

// TODO: Terrain delegate

EditTerrainDialog::EditTerrainDialog(MapDocument *mapDocument,
                                     Tileset *tileset,
                                     QWidget *parent)
    : QDialog(parent)
    , mUi(new Ui::EditTerrainDialog)
    , mMapDocument(mapDocument)
    , mTileset(tileset)
{
    mUi->setupUi(this);

    mUi->tilesetView->setEditTerrain(true);
    mUi->tilesetView->setMapDocument(mapDocument);
    mUi->tilesetView->setModel(new TilesetModel(mTileset, mUi->tilesetView));

    mTerrainModel = mapDocument->terrainModel();
    const QModelIndex rootIndex = mTerrainModel->index(tileset);

    mUi->terrainList->setModel(mTerrainModel);
    mUi->terrainList->setRootIndex(rootIndex);

    QHeaderView *terrainListHeader = mUi->terrainList->header();
#if QT_VERSION >= 0x050000
    terrainListHeader->setSectionResizeMode(0, QHeaderView::ResizeToContents);
#else
    terrainListHeader->setResizeMode(0, QHeaderView::ResizeToContents);
#endif

    QItemSelectionModel *selectionModel = mUi->terrainList->selectionModel();
    connect(selectionModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(selectedTerrainChanged(QModelIndex)));

    if (mTerrainModel->rowCount(rootIndex) > 0) {
        selectionModel->select(mTerrainModel->index(0, 0, rootIndex),
                               QItemSelectionModel::SelectCurrent |
                               QItemSelectionModel::Rows);
    }

    connect(mUi->addTerrainTypeButton, SIGNAL(clicked()),
            SLOT(addTerrainType()));
    connect(mUi->removeTerrainTypeButton, SIGNAL(clicked()),
            SLOT(removeTerrainType()));
}

EditTerrainDialog::~EditTerrainDialog()
{
    delete mUi;
}

void EditTerrainDialog::selectedTerrainChanged(const QModelIndex &index)
{
    int terrainId = -1;
    if (Terrain *terrain = mTerrainModel->terrainAt(index))
        terrainId = terrain->id();

    mUi->tilesetView->setTerrainId(terrainId);
    mUi->removeTerrainTypeButton->setEnabled(terrainId != -1);
}

void EditTerrainDialog::addTerrainType()
{
    Terrain *terrain = new Terrain(mTileset->terrainCount(), mTileset,
                                   QString(), -1);

    mMapDocument->undoStack()->push(new AddTerrain(mMapDocument, terrain));
}

void EditTerrainDialog::removeTerrainType()
{
    const QModelIndex currentIndex = mUi->terrainList->currentIndex();
    if (!currentIndex.isValid() || currentIndex.row() == 0)
        return;

    Terrain *terrain = mTerrainModel->terrainAt(currentIndex);
    mMapDocument->undoStack()->push(new RemoveTerrain(mMapDocument, terrain));
}
