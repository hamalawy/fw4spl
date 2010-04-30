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


#include "uiReconstruction/OrganMaterialEditor.hpp"

 #include <QWidget>
 #include <QVBoxLayout>
 #include <QLabel>
 #include <QSlider>
 #include <QPushButton>
   #include <QColorDialog>

 #include <QPalette>
 
namespace uiReconstruction
{

REGISTER_SERVICE( ::guiQt::editor::IEditor , ::uiReconstruction::OrganMaterialEditor , ::fwData::Reconstruction ) ;


OrganMaterialEditor::OrganMaterialEditor() throw()
{
    handlingEventOff();
}

//------------------------------------------------------------------------------

OrganMaterialEditor::~OrganMaterialEditor() throw()
{}

//------------------------------------------------------------------------------

void OrganMaterialEditor::starting() throw(::fwTools::Failed)
{
  
  std::cout<<" OrganMaterialEditor::starting() \n";
    SLM_TRACE_FUNC();
    ::guiQt::editor::IEditor::starting();
    
    QVBoxLayout *layout = new QVBoxLayout();
    
    QLabel *label = new QLabel(QObject::tr("Transparency :"),m_container);

    m_colourButton = new QPushButton(m_container);
    QObject::connect(m_colourButton, SIGNAL(clicked()), this, SLOT(colorButton())); 
    
    m_opacitySlider = new QSlider(Qt::Horizontal);
    m_opacitySlider->setRange(0, 100);
    QObject::connect(m_opacitySlider, SIGNAL(valueChanged(int)), this, SLOT(opacitySlider()));

    
    layout->addWidget(m_colourButton);
    layout->addWidget(label);
    layout->addWidget(m_opacitySlider);
    
    m_container->setLayout(layout);
    
    this->updating();
}

//------------------------------------------------------------------------------

void OrganMaterialEditor::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
   
    ::guiQt::editor::IEditor::stopping();
}

//------------------------------------------------------------------------------

void OrganMaterialEditor::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
    ::guiQt::editor::IEditor::configuring();
}

//------------------------------------------------------------------------------

void OrganMaterialEditor::updating() throw(::fwTools::Failed)
{
    this->refreshMaterial();
}

//------------------------------------------------------------------------------

void OrganMaterialEditor::swapping() throw(::fwTools::Failed)
{
    this->updating();
}

//------------------------------------------------------------------------------

void OrganMaterialEditor::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
}

//------------------------------------------------------------------------------

void OrganMaterialEditor::info( std::ostream &_sstream )
{
}


//------------------------------------------------------------------------------

void OrganMaterialEditor::opacitySlider()
{
    ::fwData::Reconstruction::sptr reconstruction = this->getObject< ::fwData::Reconstruction>();
    SLM_ASSERT("No Reconstruction!", reconstruction);

    ::fwData::Material::sptr material = reconstruction->getMaterial() ;

    float a = m_opacitySlider->value() ;
    a = (a*255)/100;
    material->ambient()->alpha() = (float) (a / 255.0) ;

    this->materialNotification();
}


void OrganMaterialEditor::materialNotification( )
{
    ::fwData::Reconstruction::sptr reconstruction = this->getObject< ::fwData::Reconstruction>();
    SLM_ASSERT("No Reconstruction!", reconstruction);

    ::fwComEd::MaterialMsg::NewSptr msg;
    msg->addEvent( ::fwComEd::MaterialMsg::MATERIAL_IS_MODIFIED ) ;
    ::fwServices::IEditionService::notify(this->getSptr(), reconstruction->getMaterial(), msg);
}


//------------------------------------------------------------------------------

void OrganMaterialEditor::colorButton()
{
    QColor color = QColorDialog::getColor(Qt::white, m_container);
    QString str = QObject::tr("background:") +color.name();
    m_colourButton->setStyleSheet(str);

  
    ::fwData::Reconstruction::sptr reconstruction = this->getObject< ::fwData::Reconstruction>();
    SLM_ASSERT("No Reconstruction!", reconstruction);

    ::fwData::Material::sptr material = reconstruction->getMaterial() ;
    
    float red = color.red()/255.0;
    float green = color.green()/255.0;
    float blue  = color.blue()/255.0;
    
        std::cout<<" RGB : "<<red<<"|"<<green<<"|"<<blue<<"\n";

    
    material->ambient()->red() = red;
    material->ambient()->green() = green;
    material->ambient()->blue() = blue;

    this->materialNotification();
}



//------------------------------------------------------------------------------

void OrganMaterialEditor::refreshMaterial( )
{
    ::fwData::Reconstruction::sptr reconstruction = this->getObject< ::fwData::Reconstruction>();
    SLM_ASSERT("No Reconstruction!", reconstruction);

   // m_container->Enable(!reconstruction->getOrganName().empty());

    ::fwData::Material::sptr material = reconstruction->getMaterial() ;
    QColor materialColor = QColor( material->ambient()->red()*255, material->ambient()->green()*255, material->ambient()->blue()*255, material->ambient()->alpha()*255);
    
    QPalette palette;
    palette.setColor(QPalette::ButtonText, materialColor);
    m_colourButton->setPalette(palette);

    int a = (int) (material->ambient()->alpha()*255) ;
    a = (a*100)/255;
    m_opacitySlider->setValue( a ) ;
}


}

