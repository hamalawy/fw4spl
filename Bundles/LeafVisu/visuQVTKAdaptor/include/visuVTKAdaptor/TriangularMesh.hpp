/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VISUQVTKADAPTOR_TRIANGULARMESH_HPP_
#define _VISUQVTKADAPTOR_TRIANGULARMESH_HPP_


#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include "visuVTKAdaptor/config.hpp"
#include "visuVTKAdaptor/MeshFactory.hpp"

class vtkCommand;
class vtkDepthSortPolyData;
class vtkAlgorithmOutput;
class vtkPolyDataNormals;
class vtkPlaneCollection;
class vtkActorCollection;
class vtkActor;
class vtkPolyData;

namespace visuVTKAdaptor
{


class VISUQVTKADAPTOR_CLASS_API TriangularMeshVtkCommand ;

class VISUQVTKADAPTOR_CLASS_API TriangularMesh: public ::fwRenderVTK::IVtkAdaptorService
{

public:
    fwCoreServiceClassDefinitionsMacro ( (TriangularMesh)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUQVTKADAPTOR_API TriangularMesh() throw();
    VISUQVTKADAPTOR_API virtual ~TriangularMesh() throw();

    VISUQVTKADAPTOR_API ::fwData::Material::sptr getMaterial();
    VISUQVTKADAPTOR_API void setMaterial(::fwData::Material::sptr material);

    VISUQVTKADAPTOR_API ::fwData::Material::sptr getUnclippedMaterial();
    VISUQVTKADAPTOR_API void setUnclippedPartMaterial(::fwData::Material::sptr material);


    VISUQVTKADAPTOR_API void setSharpEdgeAngle  ( double angle );
    VISUQVTKADAPTOR_API double getSharpEdgeAngle( );

    VISUQVTKADAPTOR_API void setShowClippedPart ( bool show );
    VISUQVTKADAPTOR_API void setClippingPlanesId( ::fwRenderVTK::VtkRenderService::VtkObjectIdType id );

    VISUQVTKADAPTOR_API void setVtkClippingPlanes               ( vtkPlaneCollection *planes );
    VISUQVTKADAPTOR_API void setMapperInput                     ( vtkAlgorithmOutput *input  );
    VISUQVTKADAPTOR_API vtkAlgorithmOutput *getMapperInput      ( );
    VISUQVTKADAPTOR_API void setActorPropertyToUnclippedMaterial( bool opt );

    VISUQVTKADAPTOR_API void updateVisibility ( bool isVisible );
    VISUQVTKADAPTOR_API bool getVisibility();

    VISUQVTKADAPTOR_API void updateOptionsMode();

#ifndef USE_DEPTH_PEELING // replacement for depth peeling
    VISUQVTKADAPTOR_API vtkDepthSortPolyData * getDepthSort();
    VISUQVTKADAPTOR_API void updateDepthSort               ();
    VISUQVTKADAPTOR_API void removeDepthSortCommand        ();
#endif


protected:

    VISUQVTKADAPTOR_API void doStart    () throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doStop     () throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doSwap   () throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate   () throw(fwTools::Failed);
    VISUQVTKADAPTOR_API void doUpdate   (::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);


    vtkActor *newActor();
    void buildPipeline();


    void updateTriangularMesh ( ::fwData::TriangularMesh::sptr mesh );
    void updateMaterial       ( ::fwData::Material::sptr material   );

    void setServiceOnMaterial(
        ::fwRenderVTK::IVtkAdaptorService::sptr &srv,
        ::fwData::Material::sptr material
        );

    void removePlaneCollectionShifterCommand();
    void removeServicesStarterCommand();
    void createServicesStarterCommand();

    void createNormalsService();
    void removeNormalsService();




    bool   m_showClippedPart;
    double m_sharpEdgeAngle;
    bool   m_manageMapperInput;

    vtkAlgorithmOutput *m_mapperInput;
    vtkPolyDataNormals *m_normals;
    vtkActor           *m_actor;
    vtkCommand         *m_depthSortCommand;

    vtkPlaneCollection *m_clippingPlanes;
    ::fwRenderVTK::VtkRenderService::VtkObjectIdType m_clippingPlanesId;

    TriangularMeshVtkCommand *m_planeCollectionShifterCallback;
    TriangularMeshVtkCommand *m_servicesStarterCallback;



    ::fwData::TriangularMesh::wptr m_triangularMesh;
    ::fwData::Material::sptr       m_material;
    ::fwData::Material::sptr       m_unclippedPartMaterial;

    ::fwRenderVTK::IVtkAdaptorService::wptr m_materialService;
    ::fwRenderVTK::IVtkAdaptorService::wptr m_unclippedPartMaterialService;
    ::fwRenderVTK::IVtkAdaptorService::wptr m_normalsService;


#ifndef USE_DEPTH_PEELING // replacement for depth peeling
    bool hasAlpha();

    vtkDepthSortPolyData *m_depthSort;
    bool m_hasAlpha;
#endif
};




} //namespace visuVTKAdaptor

#endif // _VISUQVTKADAPTOR_TRIANGULARMESH_HPP_
