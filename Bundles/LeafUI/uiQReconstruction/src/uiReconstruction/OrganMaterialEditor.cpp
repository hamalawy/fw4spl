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
    SLM_TRACE_FUNC();
    ::guiQt::editor::IEditor::starting();
    
   // QWidget *widget = new QWidget(m_container);

    m_layout = new QVBoxLayout();
    
//    m_label = new QLabel(QObject::tr("Transparency :"));
    m_label = new QLabel();
    

    m_colourButton = new QPushButton();
    QObject::connect(m_colourButton, SIGNAL(clicked()), this, SLOT(colorButton())); 
    
    m_opacitySlider = new QSlider(Qt::Horizontal);
    m_opacitySlider->setRange(0, 100);
    QObject::connect(m_opacitySlider, SIGNAL(valueChanged(int)), this, SLOT(opacitySlider()));

    

    m_layout->addWidget(m_colourButton);
    m_layout->addWidget(m_label);
    m_layout->addWidget(m_opacitySlider);
    
     m_container->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
     m_container->setMinimumSize(300, 80);
    
     m_container->setLayout(m_layout);
     

    
   // m_container->setEnabled(false);
    
    this->updating();
}

//------------------------------------------------------------------------------

void OrganMaterialEditor::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    // Deleate all
    m_label->deleteLater();
    m_colourButton->deleteLater();
    m_opacitySlider->deleteLater();
    m_layout->deleteLater();
    
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
    if(!m_container->isVisible())
    {
      m_container->setVisible(true);
    }
   
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
    
    if(color.isValid())
    {
      m_colourButton->setStyleSheet(str);
  
    ::fwData::Reconstruction::sptr reconstruction = this->getObject< ::fwData::Reconstruction>();
    SLM_ASSERT("No Reconstruction!", reconstruction);

    ::fwData::Material::sptr material = reconstruction->getMaterial() ;
    
    float red = color.red()/255.0;
    float green = color.green()/255.0;
    float blue  = color.blue()/255.0;
    
   
    material->ambient()->red() = red;
    material->ambient()->green() = green;
    material->ambient()->blue() = blue;

    this->materialNotification();
    }
}



//------------------------------------------------------------------------------

void OrganMaterialEditor::refreshMaterial( )
{
    ::fwData::Reconstruction::sptr reconstruction = this->getObject< ::fwData::Reconstruction>();
    SLM_ASSERT("No Reconstruction!", reconstruction);

    m_container->setEnabled(!reconstruction->getOrganName().empty());
    
    ::fwData::Material::sptr material = reconstruction->getMaterial() ;
    QColor materialColor = QColor( material->ambient()->red()*255, material->ambient()->green()*255, material->ambient()->blue()*255, material->ambient()->alpha()*255);
    QString str = QObject::tr("background:") +materialColor.name();
    m_colourButton->setStyleSheet(str);

    int a = (int) (material->ambient()->alpha()*255) ;
    a = (a*100)/255;
    m_opacitySlider->setValue( a ) ;
}


}

