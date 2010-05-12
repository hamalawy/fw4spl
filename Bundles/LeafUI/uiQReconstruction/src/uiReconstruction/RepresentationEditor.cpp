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
    std::cout<<"\n\n ==========> RepresentationEditor::starting()  \n \n";

  
    SLM_TRACE_FUNC();
    ::guiQt::editor::IEditor::starting();


    
    m_groupRepresentation = new QGroupBox(QObject::tr("Representation"), m_container);
    m_groupShading = new QGroupBox(QObject::tr("Shading"), m_container);

    
    m_normalsCheckBox = new QCheckBox(QObject::tr("Show normals"), m_container);
 //   m_normalsCheckBox->setChecked(false);
    QObject::connect(m_normalsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(showNormals()));


    layout = new QVBoxLayout();    
    repLayout = new QVBoxLayout();
    shadingLayout = new QVBoxLayout();

    
    QRadioButton *surface = new QRadioButton(QObject::tr("Surface"));
    QRadioButton *point = new QRadioButton(QObject::tr("Point"));
    QRadioButton *wireframe = new QRadioButton(QObject::tr("Wireframe"));
    QRadioButton *edge = new QRadioButton(QObject::tr("Edge"));
    
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
    
    //m_container->setEnabled(false);
     
    this->updating();   
}

//------------------------------------------------------------------------------

void RepresentationEditor::stopping() throw(::fwTools::Failed)
{
   SLM_TRACE_FUNC();
   std::cout<<"\n\n ==========> RepresentationEditor::stopping()  \n \n";
 
   QList<QAbstractButton *> buttons = m_groupButtonRepresentation->buttons();
       
      for (int i = 0; i < buttons.count(); i++) 
      {
		buttons.at(i)->deleteLater();
      }
    
    m_groupRepresentation->deleteLater();
    m_groupShading->deleteLater();
    m_groupButtonRepresentation->deleteLater();
    m_groupButtonShading->deleteLater();
    
    m_normalsCheckBox->deleteLater();
    
    layout->deleteLater();
    repLayout->deleteLater();
    shadingLayout->deleteLater();

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

    std::cout<<"\n\n ==========> RepresentationEditor::updating(FAILED)   \n \n";

    m_material = reconstruction->getMaterial() ;
    m_container->setEnabled(!reconstruction->getOrganName().empty());
    
     if(!m_container->isEnabled())
    {
      std::cout<<"==============> REP : NOT ENABLED \n\n";
    }
    else
     std::cout<<" ====================> REP : ENABLED \n\n";

    this->refreshRepresentation();
    this->refreshNormals();
    this->refreshShading() ;
    
}

//------------------------------------------------------------------------------

void RepresentationEditor::swapping() throw(::fwTools::Failed)
{
    std::cout<<"\n\n ==========> RepresentationEditor::swapping()   \n \n";

    this->updating();
}

//------------------------------------------------------------------------------

void RepresentationEditor::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
      std::cout<<"\n\n ==========> RepresentationEditor::updating(MSG)   \n \n";
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
  
  //  std::cout<<" Bouton cliked :  "<<m_groupButtonRepresentation->checkedId()<<"\n";

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
 //     std::cout<<" Bouton cliked :  "<<m_groupButtonShading->checkedId()<<"\n";

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
{
    int representationMode = m_material->getRepresentationMode();

    switch(representationMode)
    {
    case ::fwData::Material::MODE_SURFACE:
    {
        m_groupButtonRepresentation->button(0)->setChecked(true);
        break ;
    }
    case ::fwData::Material::MODE_POINT:
    {
        m_groupButtonRepresentation->button(1)->setChecked(true);
        break ;
    }
    case ::fwData::Material::MODE_WIREFRAME:
    {
        m_groupButtonRepresentation->button(2)->setChecked(true);
        break ;
    }
    case ::fwData::Material::MODE_EDGE:
    {
        m_groupButtonRepresentation->button(3)->setChecked(true);
        break ;
    }
    default :
        m_groupButtonRepresentation->button(0)->setChecked(true);
    }

}

//------------------------------------------------------------------------------

void RepresentationEditor::refreshShading()
{
    int shadingMode = m_material->getShadingMode();

    switch(shadingMode)
    {
    case ::fwData::Material::MODE_FLAT:
    {
        m_groupButtonShading->button(0)->setChecked(true);
        break ;
    }
    case ::fwData::Material::MODE_GOURAUD:
    {
        m_groupButtonShading->button(1)->setChecked(true);
        break ;
    }
    case ::fwData::Material::MODE_PHONG:
    {
        m_groupButtonShading->button(2)->setChecked(true);
        break ;
    }
    default :
        m_groupButtonShading->button(2)->setChecked(true);
    
    }
    
}


}

