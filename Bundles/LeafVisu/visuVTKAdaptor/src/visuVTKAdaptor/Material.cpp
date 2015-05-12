/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/Material.hpp"

#include <fwComEd/MaterialMsg.hpp>

#include <fwData/Material.hpp>
#include <fwData/mt/ObjectReadLock.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/Base.hpp>

#include <fwTools/fwID.hpp>

#include <fwVtkIO/vtk.hpp>

#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkTexture.h>


fwServicesRegisterMacro( ::fwRenderVTK::IVtkAdaptorService, ::visuVTKAdaptor::Material, ::fwData::Material );

namespace visuVTKAdaptor
{


//------------------------------------------------------------------------------

Material::Material() throw()
{
    m_manageProperty = true;
    m_property       = vtkProperty::New();
    //addNewHandledEvent( ::fwComEd::MaterialMsg::MATERIAL_IS_MODIFIED );
}

//------------------------------------------------------------------------------

Material::~Material() throw()
{
    if (m_manageProperty)
    {
        m_property->Delete();
    }
    m_property = NULL;
}

//------------------------------------------------------------------------------

void Material::setVtkProperty(vtkProperty *property)
{
    if (m_manageProperty)
    {
        m_property->Delete();
        m_property = NULL;
    }

    if (property)
    {
        m_manageProperty = false;
        m_property       = property;
    }
    else
    {
        m_manageProperty = true;
        m_property       = vtkProperty::New();
    }
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

vtkProperty *Material::getVtkProperty()
{
    return m_property;
}

//------------------------------------------------------------------------------

void Material::configuring() throw(fwTools::Failed)
{

    SLM_TRACE_FUNC();

    assert(m_configuration->getName() == "config");
}

//------------------------------------------------------------------------------

void Material::doStart() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void Material::doSwap() throw(fwTools::Failed)
{
    SLM_TRACE("SWAPPING Material");
    this->doUpdate();
}

//------------------------------------------------------------------------------

void Material::doUpdate() throw(fwTools::Failed)
{
    ::fwData::Material::sptr material = this->getObject < ::fwData::Material >();

    updateMaterial( material );
}

//------------------------------------------------------------------------------

void Material::doReceive( ::fwServices::ObjectMsg::csptr msg ) throw(::fwTools::Failed)
{
    ::fwComEd::MaterialMsg::csptr materialMsg = ::fwComEd::MaterialMsg::dynamicConstCast(msg);
    if( materialMsg && materialMsg->hasEvent(::fwComEd::MaterialMsg::MATERIAL_IS_MODIFIED) )
    {
        this->doUpdate();
    }
}

//------------------------------------------------------------------------------

void Material::doStop() throw(fwTools::Failed)
{
    //this->removeAllPropFromRenderer();
    //this->unregisterServices();
}

//------------------------------------------------------------------------------

void Material::updateMaterial( SPTR(::fwData::Material)material )
{
    ::fwData::Color::sptr color = material->ambient();
    m_property->SetColor( color->red(),
                          color->green(),
                          color->blue());

    //3DVSP-like rendering
    m_property->SetSpecularColor(1.,1.,1.);
    m_property->SetSpecularPower(100.); //Shininess
    m_property->SetAmbient(.05);
    m_property->SetDiffuse(1.);
    m_property->SetSpecular(1.);
    m_property->SetOpacity( color->alpha() );

    // set texture
    ::fwData::Image::sptr diffTex = material->getDiffuseTexture();

    if(diffTex != NULL)
    {
        ::fwData::mt::ObjectReadLock lock(diffTex);

        if (diffTex->getSizeInBytes() != 0)
        {
            vtkSmartPointer< vtkImageData > vtkImage = vtkSmartPointer< vtkImageData >::New();
            ::fwVtkIO::toVTKImage( diffTex, vtkImage );

            vtkSmartPointer<vtkTexture> vtkTex = vtkSmartPointer< vtkTexture >::New();
            vtkTex->SetInputData(vtkImage);

            ::fwData::Material::WrappingType wrapping = material->getDiffuseTextureWrapping();
            vtkTex->SetRepeat( wrapping == ::fwData::Material::REPEAT );
            vtkTex->SetEdgeClamp( wrapping == ::fwData::Material::CLAMP );
            m_property->RemoveTexture("diffuse");
            m_property->SetTexture("diffuse", vtkTex);
        }
    }

    switch(material->getRepresentationMode())
    {
        case ::fwData::Material::MODE_SURFACE:
            m_property->SetRepresentationToSurface();
            m_property->EdgeVisibilityOff();
            break;

        case ::fwData::Material::MODE_EDGE:
            m_property->SetRepresentationToSurface();
            m_property->EdgeVisibilityOn();
            break;

        case ::fwData::Material::MODE_WIREFRAME:
            m_property->SetRepresentationToWireframe();
            break;

        case ::fwData::Material::MODE_POINT:
            m_property->SetRepresentationToPoints();
            break;

        default:
            OSLM_ASSERT("Unknown material representation mode : " << material->getRepresentationMode(), false );
    }

    switch(material->getShadingMode())
    {
        /// Sets ShadingMode
        case ::fwData::Material::MODE_PHONG:
            m_property->SetInterpolationToPhong();
            break;

        case ::fwData::Material::MODE_GOURAUD:
            m_property->SetInterpolationToGouraud();
            break;

        case ::fwData::Material::MODE_FLAT:
            m_property->SetInterpolationToFlat();
            break;

        default:
            OSLM_ASSERT("Unknown shading mode : " << material->getShadingMode(), false );
    }

    m_property->Modified();
    this->setVtkPipelineModified();
}

} //namespace visuVTKAdaptor
