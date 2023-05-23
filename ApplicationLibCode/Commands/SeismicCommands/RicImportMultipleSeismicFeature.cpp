/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2023    Equinor ASA
//
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
//
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#include "RicImportMultipleSeismicFeature.h"

#include "RiaImportEclipseCaseTools.h"

#include "RiaApplication.h"

#include "RicRecursiveFileSearchDialog.h"

#include "RimEclipseCaseCollection.h"

#include "RiuMainWindow.h"

#include "cafSelectionManager.h"

#include <QAction>
#include <QFileInfo>

CAF_CMD_SOURCE_INIT( RicImportMultipleSeismicFeature, "RicImportMultipleSeismicFeature" );

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RicImportMultipleSeismicFeature::isCommandEnabled()
{
    return true;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicImportMultipleSeismicFeature::onActionTriggered( bool isChecked )
{
    RiaApplication* app        = RiaApplication::instance();
    QString         defaultDir = app->lastUsedDialogDirectory( "SEISMIC_GRID" );

    RicRecursiveFileSearchDialogResult result =
        RicRecursiveFileSearchDialog::runRecursiveSearchDialog( nullptr,
                                                                "Import Seismic Data",
                                                                defaultDir,
                                                                m_pathFilter,
                                                                m_fileNameFilter,
                                                                { RicRecursiveFileSearchDialog::FileType::SEISMIC_SEGY,
                                                                  RicRecursiveFileSearchDialog::FileType::SEISMIC_VDS,
                                                                  RicRecursiveFileSearchDialog::FileType::SEISMIC_ZGY } );

    // Remember filters
    m_pathFilter     = result.pathFilter;
    m_fileNameFilter = result.fileNameFilter;

    if ( !result.ok ) return;

    // Remember the path to next time
    app->setLastUsedDialogDirectory( "SEISMIC_GRID", QFileInfo( result.rootDir ).absoluteFilePath() );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicImportMultipleSeismicFeature::setupActionLook( QAction* actionToSetup )
{
    actionToSetup->setIcon( QIcon( ":/Seismic16x16.png" ) );
    actionToSetup->setText( "Import Multiple Seismic Files" );
}
