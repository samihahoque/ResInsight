/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2023     Equinor ASA
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
#pragma once

#include "RivTexturePartMgr.h"

#include "cafPdmPointer.h"
#include "cvfArray.h"

namespace cvf
{
class ModelBasicList;
class Part;
class DrawableGeo;
class BoundingBox;
class ShaderProgram;
class TextureImage;
} // namespace cvf

namespace caf
{
class DisplayCoordTransform;
}

namespace ZGYAccess
{
class SeismicSliceData;
}

class RimSeismicSection;
class RimSurface;
class Rim3dView;
class RivPolylinePartMgr;

class RivSeismicSectionPartMgr : public RivTexturePartMgr
{
public:
    explicit RivSeismicSectionPartMgr( RimSeismicSection* section );

    void appendGeometryPartsToModel( cvf::ModelBasicList*              model,
                                     const caf::DisplayCoordTransform* displayCoordTransform,
                                     const cvf::BoundingBox&           boundingBox );

    void appendPolylinePartsToModel( Rim3dView*                        view,
                                     cvf::ModelBasicList*              model,
                                     const caf::DisplayCoordTransform* displayCoordTransform,
                                     const cvf::BoundingBox&           boundingBox );

    void appendSurfaceIntersectionLines( cvf::ModelBasicList*              model,
                                         const caf::DisplayCoordTransform* displayCoordTransform,
                                         double                            lineThickness,
                                         const std::vector<RimSurface*>&   surfaces );

private:
    cvf::ref<cvf::DrawableGeo> createXYPlaneQuadGeoWithTexCoords( const cvf::Vec3dArray& cornerPoints );
    cvf::ref<cvf::Part>        createSingleTexturedQuadPart( const cvf::Vec3dArray& cornerPoints, cvf::ref<cvf::TextureImage> image );

protected:
    cvf::TextureImage* createImageFromData( ZGYAccess::SeismicSliceData* data );

    static std::vector<std::vector<cvf::Vec3d>> projectPolyLineOntoSurface( std::vector<cvf::Vec3d>           polyLine,
                                                                            RimSurface*                       surface,
                                                                            const caf::DisplayCoordTransform* displayCoordTransform );

private:
    caf::PdmPointer<RimSeismicSection> m_section;
    cvf::ref<RivPolylinePartMgr>       m_polylinePartMgr;
};