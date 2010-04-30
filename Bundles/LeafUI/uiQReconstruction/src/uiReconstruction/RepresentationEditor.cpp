#include <fwCore/base.hpp>

#include <fwTools/Object.hpp>

#include <fwData/Reconstruction.hpp>

#include <fwComEd/MaterialMsg.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/operations.hpp>

#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>
#include <fwServices/IService.hpp>
#include <fwServices/op/Get.hpp>

#include "uiReconstruction/RepresentationEditor.hpp"

#include <fwData/Material.hpp>


 #include <QWidget>
 #include <QGroupBox>
 #include <QCheckBox>
#include <QVBoxLayout>
#include <QTextEdit>
 #include <QButtonGroup>
 #include <QRadioButton>
 

namespace uiReconstruction
{

REGISTER_SERVICE( ::guiQt::editor::IEditor , ::uiReconstruction::RepresentationEditor , ::fwData::Reconstruction ) ;


RepresentationEditor::RepresentationEditor() throw()
{
    handlingEventOff();
}

//------------------------------------------------------------------------------

RepresentationEditor::~RepresentationEditor() throw()
{}

//------------------------------------------------------------------------------

void RepresentationEditor::starting() throw(::fwTools::Failed)
{
    std::cout<<" RepresentationEditor::starting() \n";

  
    SLM_TRACE_FUNC();
    ::guiQt::editor::IEditor::starting();


    
    m_groupRepresentation = new QGroupBox(QObject::tr("Representation"), m_container);
    m_groupShading = new QGroupBox(QObject::tr("Shading"), m_container);

    
    m_normalsCheckBox = new QCheckBox(QObject::tr("Show normals"), m_container);
 //   m_normalsCheckBox->setChecked(false);
    QObject::connect(m_normalsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(showNormals()));


    QVBoxLayout *layout = new QVBoxLayout();
    
    QVBoxLayout *repLayout = new QVBoxLayout();
    QVBoxLayout *shadingLayout = new QVBoxLayout();

    
    QRadioButton *surface = new QRadioButton(QObject::tr("Surface"));
    QRadioButton *point = new QRadioButton(QObject::tr("Poinr"));
    QRadioButton *wireframe = new QRadioButton(QObject::tr("Wireframe"));
    QRadioButton *edge = new QRadioButton(QObject::tr("Edges"));
    
    QRadioButton *flat = new QRadioButton(QObject::tr("Flat"));
    QRadioButton *gouraud = new QRadioButton(QObject::tr("Gouraud"));
    QRadioButton *phong = new QRadioButton(QObject::tr("Phong"));
    
    m_groupButtonRepresentation = new QButtonGroup();
    m_groupButtonRepresentation->addButton(surface,0);
    m_groupButtonRepresentation->addButton(point,1);
    m_groupButtonRepresentation->addButton(wireframe,2);
    m_groupButtonRepresentation->addButton(edge,3);    
    surface->setChecked(true);        
    QObject::connect(m_groupButtonRepresentation, SIGNAL(buttonClicked(int)), this, SLOT(changeRepresentation()));
    
    m_groupButtonShading = new QButtonGroup();
    m_groupButtonShading->addButton(flat,0);
    m_groupButtonShading->addButton(gouraud,1);
    m_groupButtonShading->addButton(phong,2);
    phong->setChecked(true);        
    QObject::connect(m_groupButtonShading, SIGNAL(buttonClicked(int)), this, SLOT(changeShading()));

    

    repLayout->addWidget(surface);
    repLayout->addWidget(point);
    repLayout->addWidget(wireframe);
    repLayout->addWidget(edge);
    
    shadingLayout->addWidget(flat);
    shadingLayout->addWidget(gouraud);
    shadingLayout->addWidget(phong);

    
    m_groupRepresentation->setLayout(repLayout);
    m_groupShading->setLayout(shadingLayout);
    


    layout->addWidget(m_groupRepresentation);
    layout->addWidget(m_groupShading);
    layout->addWidget(m_normalsCheckBox);

    m_container->setLayout(layout);
    this->updating();   
}

//------------------------------------------------------------------------------

void RepresentationEditor::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
  

    ::guiQt::editor::IEditor::stopping();
}

//------------------------------------------------------------------------------

void RepresentationEditor::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
    ::guiQt::editor::IEditor::configuring();
}

//------------------------------------------------------------------------------

void RepresentationEditor::updating() throw(::fwTools::Failed)
{
    ::fwData::Reconstruction::sptr reconstruction = this->getObject< ::fwData::Reconstruction>();
    SLM_ASSERT("No Reconstruction!", reconstruction);

    m_material = reconstruction->getMaterial() ;
 //   m_container->Enable(!reconstruction->getOrganName().empty());

    this->refreshRepresentation();
    this->refreshNormals();
    this->refreshShading() ;
    
}

//------------------------------------------------------------------------------

void RepresentationEditor::swapping() throw(::fwTools::Failed)
{
    this->updating();
}

//------------------------------------------------------------------------------

void RepresentationEditor::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
}

//------------------------------------------------------------------------------

void RepresentationEditor::info( std::ostream &_sstream )
{
}


//------------------------------------------------------------------------------

void RepresentationEditor::showNormals()
{
  
    if ( m_normalsCheckBox->isChecked() )
    {
        m_material->setOptionsMode( ::fwData::Material::MODE_NORMALS );
    }
    else
    {
        m_material->setOptionsMode( ::fwData::Material::MODE_STANDARD );
    }

    this->notifyTriangularMesh();
}

//------------------------------------------------------------------------------

void RepresentationEditor::notifyTriangularMesh()
{
    ::fwData::Reconstruction::sptr reconstruction = this->getObject< ::fwData::Reconstruction>();

    ::fwComEd::MaterialMsg::NewSptr msg;
    msg->addEvent( ::fwComEd::MaterialMsg::MATERIAL_IS_MODIFIED ) ;
    ::fwServices::IEditionService::notify(this->getSptr(), reconstruction->getTriangularMesh(), msg);
}

//------------------------------------------------------------------------------

void RepresentationEditor::refreshNormals()
{
    m_normalsCheckBox->setChecked(m_material->getOptionsMode() == ::fwData::Material::MODE_NORMALS);
}



//------------------------------------------------------------------------------

void RepresentationEditor::changeRepresentation()
{
  
    std::cout<<" Bouton cliked :  "<<m_groupButtonRepresentation->checkedId()<<"\n";
  
  
  
    int choix = m_groupButtonRepresentation->checkedId();
    ::fwData::Material::REPRESENTATION_MODE selectedMode = ::fwData::Material::MODE_SURFACE;

    switch(choix)
    {
        case 0 :
        {
            selectedMode = ::fwData::Material::MODE_SURFACE;
            break ;
        }
        case 1 :
        {
            selectedMode = ::fwData::Material::MODE_POINT;
            break ;
        }
        case 2 :
        {
            selectedMode = ::fwData::Material::MODE_WIREFRAME;
            break ;
        }
        case 3 :
        {
            selectedMode = ::fwData::Material::MODE_EDGE;
            break ;
        }
    }

    m_material->setRepresentationMode( selectedMode );
    this->notifyMaterial();
    
    
}


//------------------------------------------------------------------------------

void RepresentationEditor::changeShading()
{
      std::cout<<" Bouton cliked :  "<<m_groupButtonShading->checkedId()<<"\n";

    int choix = m_groupButtonShading->checkedId();
    ::fwData::Material::SHADING_MODE selectedMode = ::fwData::Material::MODE_PHONG;

    switch(choix)
    {
        case 0 :
        {
            selectedMode = ::fwData::Material::MODE_FLAT;
            break ;
        }
        case 1 :
        {
            selectedMode = ::fwData::Material::MODE_GOURAUD;
            break ;
        }
        case 2 :
        {
            selectedMode = ::fwData::Material::MODE_PHONG;
            break ;
        }
    }

    m_material->setShadingMode( selectedMode );
    this->notifyMaterial();
    
}



//------------------------------------------------------------------------------

void RepresentationEditor::notifyMaterial()
{
    ::fwData::Reconstruction::sptr reconstruction = this->getObject< ::fwData::Reconstruction>();

    ::fwComEd::MaterialMsg::NewSptr msg;
    msg->addEvent( ::fwComEd::MaterialMsg::MATERIAL_IS_MODIFIED ) ;
    ::fwServices::IEditionService::notify(this->getSptr(), reconstruction->getMaterial(), msg);
}


//------------------------------------------------------------------------------

void RepresentationEditor::refreshRepresentation()
{/*
    int representationMode = m_material->getRepresentationMode();

    switch(representationMode)
    {
    case ::fwData::Material::MODE_SURFACE:
    {
        m_radioBoxRepresentation->SetSelection(0);
        break ;
    }
    case ::fwData::Material::MODE_POINT:
    {
        m_radioBoxRepresentation->SetSelection(1);
        break ;
    }
    case ::fwData::Material::MODE_WIREFRAME:
    {
        m_radioBoxRepresentation->SetSelection(2);
        break ;
    }
    case ::fwData::Material::MODE_EDGE:
    {
        m_radioBoxRepresentation->SetSelection(3);
        break ;
    }
    default :
        m_radioBoxRepresentation->SetSelection(0);
    }
*/
}

//------------------------------------------------------------------------------

void RepresentationEditor::refreshShading()
{/*
    int shadingMode = m_material->getShadingMode();

    switch(shadingMode)
    {
    case ::fwData::Material::MODE_FLAT:
    {
        m_radioBoxShading->SetSelection(0);
        break ;
    }
    case ::fwData::Material::MODE_GOURAUD:
    {
        m_radioBoxShading->SetSelection(1);
        break ;
    }
    case ::fwData::Material::MODE_PHONG:
    {
        m_radioBoxShading->SetSelection(2);
        break ;
    }
    default :
        m_radioBoxShading->SetSelection(2);
    }
    */
}


}

