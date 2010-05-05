#include <fwCore/base.hpp>

#include <fwTools/Object.hpp>
#include <fwTools/UUID.hpp>

#include <fwData/Acquisition.hpp>
#include <fwData/Boolean.hpp>
#include <fwData/String.hpp>

#include <fwComEd/ReconstructionMsg.hpp>
#include <fwComEd/AcquisitionMsg.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/operations.hpp>

#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>
#include <fwServices/IService.hpp>
#include <fwServices/op/Get.hpp>

#include "uiAcquisition/OrganListEditor.hpp"

 #include <QWidget>
 #include <QGroupBox>
 #include <QCheckBox>
#include <QVBoxLayout>

namespace uiAcquisition
{

REGISTER_SERVICE( ::guiQt::editor::IEditor , ::uiAcquisition::OrganListEditor , ::fwData::Acquisition ) ;


OrganListEditor::OrganListEditor() throw()
{
    addNewHandledEvent("ShowReconstructions");
    addNewHandledEvent("SelectReconstruction");
    addNewHandledEvent(::fwComEd::AcquisitionMsg::ADD_RECONSTRUCTION);
}

//------------------------------------------------------------------------------

OrganListEditor::~OrganListEditor() throw()
{}

//------------------------------------------------------------------------------

void OrganListEditor::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    ::guiQt::editor::IEditor::starting();
    
    QVBoxLayout *layout = new QVBoxLayout();
    QVBoxLayout *groupLayout = new QVBoxLayout();
    
    m_listLayout = new QVBoxLayout();

    m_organs = new QGroupBox(QObject::tr("Organs"), m_container);
    m_organs->setEnabled(false);
    
    m_showCheckBox = new QCheckBox(QObject::tr("Hide organs"), m_organs);
    QObject::connect(m_showCheckBox, SIGNAL(stateChanged(int)), this, SLOT(showReconstructions()));
  
    m_listW = new QListWidget(m_organs);
   
    groupLayout->addWidget(m_showCheckBox);
    groupLayout->addWidget(m_listW);
    m_organs->setLayout(groupLayout);

    layout->addWidget(m_organs);

    m_container->setLayout(layout);
    
    this->updating();
}

//------------------------------------------------------------------------------

void OrganListEditor::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
   

    ::guiQt::editor::IEditor::stopping();
}

//------------------------------------------------------------------------------

void OrganListEditor::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
    ::guiQt::editor::IEditor::configuring();
}

//------------------------------------------------------------------------------

void OrganListEditor::updating() throw(::fwTools::Failed)
{
 // std::cout<<"   \n UPDATING  \n \n";
  this->updateReconstructions();
  //  this->refreshVisibility();
}

//------------------------------------------------------------------------------

void OrganListEditor::swapping() throw(::fwTools::Failed)
{
      std::cout<<"\n\n\n OrganListEditor::SW>APPING \n\n";

    this->updating();
}
//------------------------------------------------------------------------------

void OrganListEditor::updating( ::fwServices::ObjectMsg::csptr msg ) throw(::fwTools::Failed)
{
    std::cout<<"\n\n\n OrganListEditor::updating(MSG) \n\n";

    ::fwComEd::AcquisitionMsg::csptr acquisitionMsg = ::fwComEd::AcquisitionMsg::dynamicConstCast( msg ) ;
    if ( acquisitionMsg )
    {
        if ( acquisitionMsg->hasEvent("ShowReconstructions") ||
                acquisitionMsg->hasEvent("SelectReconstruction") ||
                acquisitionMsg->hasEvent(::fwComEd::AcquisitionMsg::ADD_RECONSTRUCTION) )
        {
            this->updating();
        }
    }

}

//------------------------------------------------------------------------------

void OrganListEditor::info( std::ostream &_sstream )
{
}

//------------------------------------------------------------------------------

void OrganListEditor::updateReconstructions()
{
  std::cout<<"\n\n\n OrganListEditor::updateReconstructions() \n\n";
  std::string string;

   // m_organChoice->Clear();
    m_map.clear();
    
    ::fwData::Acquisition::sptr acq = this->getObject< ::fwData::Acquisition >();
    m_container->setEnabled(acq->getReconstructions().first != acq->getReconstructions().second);
    
    if(!m_container->isEnabled())
    {
      m_listW->clear();
    }

    if(acq->getReconstructions().first != acq->getReconstructions().second)
    {
        ::fwData::Acquisition::ReconstructionIterator iter =  acq->getReconstructions().first;

        for (; iter!=  acq->getReconstructions().second ; ++iter )
        {
            m_map[ (*iter)->getOrganName() ] = (*iter);
        }
        for( OrganNameReconstruction::iterator iter = m_map.begin(); iter != m_map.end(); ++iter )
        {
	  string = (*iter).first.data();

	  QListWidgetItem *item = new QListWidgetItem(string.c_str(), m_listW);
	  item->setCheckState(Qt::Checked);
	  //m_listW->addItem(item);
	}



	QObject::connect(m_listW, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(organChoiceSelection()));


        bool showAllRec = true;
        if (acq->getFieldSize("ShowReconstructions"))
        {
            showAllRec = acq->getFieldSingleElement< ::fwData::Boolean >("ShowReconstructions")->value();
        }
        m_showCheckBox->setChecked(!showAllRec);
        m_organs->setEnabled(!m_showCheckBox->isChecked());
    }
}

//------------------------------------------------------------------------------

void OrganListEditor::organChoiceSelection()
{
  m_item = m_listW->selectedItems().first();
  //std::cout<<" m_item : "<<m_item->text().toStdString()<<"   curent : "<<m_listW->currentItem()->text().toStdString()<<"\n";
  
  this->notifyOrganChoiceSelection();
  this->organChoiceVisibility();
}

//------------------------------------------------------------------------------

void OrganListEditor::notifyOrganChoiceSelection()
{
    std::string organSelected = m_item->text().toStdString();
    if(m_map.find(organSelected) != m_map.end())
    {
        ::fwData::Acquisition::sptr acq = this->getObject< ::fwData::Acquisition >();
        ::fwData::Reconstruction::sptr rec = m_map[organSelected] ;

        ::fwComEd::AcquisitionMsg::NewSptr msg;
        msg->addEvent( ::fwComEd::AcquisitionMsg::NEW_RECONSTRUCTION_SELECTED, ::fwData::String::New( ::fwTools::UUID::get(rec)) );
        ::fwServices::IEditionService::notify(this->getSptr(), acq, msg);
    }
    
}

//------------------------------------------------------------------------------

void OrganListEditor::organChoiceVisibility()
{
    //std::cout<<" SLOT() ---> VISIBILITY \n";
 
    std::string organSelected = m_item->text().toStdString();
    ::fwData::Reconstruction::sptr rec = m_map[organSelected] ;
    assert(rec) ;    
    
   // std::cout<<" \nOrgan SELECTED : "<<organSelected<<" Visible : "<<rec->getIsVisible()<<"\n";
    //rec->setIsVisible(!rec->getIsVisible());
    if(m_item->checkState()==Qt::Checked)
    {
      rec->setIsVisible(true);
    }
    else if(m_item->checkState()==Qt::Unchecked)
      rec->setIsVisible(false);

   // std::cout<<"           Organ SELECTED : "<<organSelected<<" Visible : "<<rec->getIsVisible()<<"\n";

    ::fwComEd::ReconstructionMsg::NewSptr msg;
    msg->addEvent( ::fwComEd::ReconstructionMsg::VISIBILITY ) ;
    ::fwServices::IEditionService::notify(this->getSptr(), rec, msg);
    
}


//------------------------------------------------------------------------------

void OrganListEditor::showReconstructions()
{
    ::fwData::Acquisition::sptr acq = this->getObject< ::fwData::Acquisition >();
    acq->setFieldSingleElement("ShowReconstructions",  ::fwData::Boolean::NewSptr(!m_showCheckBox->isChecked()) );

    ::fwComEd::AcquisitionMsg::NewSptr msg;
    msg->addEvent( "ShowReconstructions" );
    ::fwServices::IEditionService::notify(this->getSptr(), acq, msg);

    m_listW->setEnabled(!m_showCheckBox->isChecked());
}
/*
//------------------------------------------------------------------------------

void OrganListEditor::refreshVisibility()
{
    int item=0;
    for( OrganNameReconstruction::iterator iter = m_map.begin(); iter != m_map.end(); ++iter, ++item )
    {
        m_organChoice->Check( item, iter->second->getIsVisible()  );
    }
}



*/
}

