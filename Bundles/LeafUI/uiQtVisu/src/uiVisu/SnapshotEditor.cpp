/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#include <fwCore/base.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/convenience.hpp>

#include <fwTools/Object.hpp>
#include <fwData/String.hpp>
#include <fwData/Composite.hpp>

#include <fwComEd/CompositeMsg.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/operations.hpp>

#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/ObjectServiceRegistry.hpp>
#include <fwServices/IService.hpp>
#include <fwServices/op/Get.hpp>

#include "uiVisu/SnapshotEditor.hpp"

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QIcon>
#include <QImage>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QString>
#include <QDesktopWidget>

namespace uiVisu
{

REGISTER_SERVICE( ::guiQt::editor::IEditor , ::uiVisu::SnapshotEditor , ::fwTools::Object ) ;


SnapshotEditor::SnapshotEditor() throw()
{
  
     m_screenshotLabel = new QLabel;
     m_screenshotLabel->setSizePolicy(QSizePolicy::Expanding,
                                    QSizePolicy::Expanding);
     m_screenshotLabel->setAlignment(Qt::AlignCenter);
     m_screenshotLabel->setMinimumSize(240, 160);

}

//------------------------------------------------------------------------------

SnapshotEditor::~SnapshotEditor() throw()
{}

//------------------------------------------------------------------------------

void SnapshotEditor::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    ::guiQt::editor::IEditor::starting();
    
//    QWidget *mainWidget = m_globalUIDToQtContainer.find(this->getUUID())->second;
    QWidget *widget = new QWidget(m_container);
    
    QHBoxLayout *layout = new  QHBoxLayout();
    
    m_icon = QIcon();

    QString str = QObject::tr("Bundles/uiQtVisu_") + QObject::tr(UIQTVISU_VER) + QObject::tr("/camera-photo.png");

    m_imageSnap = QImage(str);

    m_snapButton = new QPushButton(widget);
    m_snapButton->setFixedWidth(35);
    m_snapButton->setFixedHeight(30);
    
    m_icon.addPixmap(QPixmap::fromImage(m_imageSnap), QIcon::Normal);
    m_snapButton->setIcon(m_icon);
    m_snapButton->setIconSize(QSize(m_snapButton->width(), m_snapButton->height()));
    
    QObject::connect(m_snapButton, SIGNAL(clicked()), this, SLOT(snapShoot())); 
    
    m_snapButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
    
    layout->addWidget( widget);
    layout->setContentsMargins(0,0,0,0);

    widget->setFixedWidth(35);
    widget->setFixedHeight(30);

    m_container->setLayout(layout);
}

//------------------------------------------------------------------------------

void SnapshotEditor::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
 //   m_container->Unbind( wxEVT_COMMAND_BUTTON_CLICKED, &SnapshotEditor::onSnapButton, this, m_idSnapButton);
    ::guiQt::editor::IEditor::stopping();
}

//------------------------------------------------------------------------------

void SnapshotEditor::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
    ::guiQt::editor::IEditor::configuring();

    std::vector < Configuration > snapConfig = m_configuration->find("snap");
    if(!snapConfig.empty())
    {
        std::vector < Configuration > sceneConfig = snapConfig.at(0)->find("scene");
        for(unsigned int i =0; i < sceneConfig.size(); i++)
        {
            OSLM_ASSERT("Wrong tag name: " << sceneConfig.at(i)->getName(), sceneConfig.at(i)->getName() == "scene");
            SLM_ASSERT("UID attribute is missing", sceneConfig.at(i)->hasAttribute("uid"));

            std::string value(sceneConfig.at(i)->getAttributeValue("uid"));
            m_scenesUID.push_back(value);
        }
    }
}

//------------------------------------------------------------------------------

void SnapshotEditor::updating() throw(::fwTools::Failed)
{
}

//------------------------------------------------------------------------------

void SnapshotEditor::swapping() throw(::fwTools::Failed)
{

}
//------------------------------------------------------------------------------

void SnapshotEditor::updating( ::fwServices::ObjectMsg::csptr _msg ) throw(::fwTools::Failed)
{
}

//------------------------------------------------------------------------------

void SnapshotEditor::info( std::ostream &_sstream )
{
}

//------------------------------------------------------------------------------

void SnapshotEditor::snapShoot()
{
    SLM_TRACE_FUNC();
    
    QWidget *mainWidget = qApp->activeWindow();
   
        for(unsigned int i=0; i < m_scenesUID.size(); i++)
        {
            ::fwServices::IService::sptr service = ::fwServices::get(m_scenesUID.at(i));
            ::fwData::Composite::sptr composite = service->getObject< ::fwData::Composite >();
            SLM_ASSERT("SnapshotEditor sceneUID " << m_scenesUID.at(i) <<" isn't a GenericScene?" , composite);

            ::fwData::Object::NewSptr dataInfo;

            ::fwData::String::NewSptr sceneID;
            sceneID->value() = m_scenesUID.at(i);
            ::fwData::String::NewSptr filename;
	    
	    m_pixmap = QPixmap();
	   // m_pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
	    m_pixmap = QPixmap::grabWindow(mainWidget->winId());
		
	    m_screenshotLabel->setPixmap(m_pixmap.scaled(m_screenshotLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));	
		
	    QString format = "png";
	    QString initialPath = QDir::currentPath() + QObject::tr("/untitled.") + format;

	    QString fileName = QFileDialog::getSaveFileName(0, tr("Save As"), initialPath,
                                tr("%1 Files (*.%2);;All Files (*)")
                                .arg(format.toUpper())
                                .arg(format));
	    if (!fileName.isEmpty())
	      m_pixmap.save(fileName, format.toAscii());
	    

	    filename->value() = fileName.toStdString();
            if(!filename->value().empty())
            {
                dataInfo->setFieldSingleElement("sceneID", sceneID);
                dataInfo->setFieldSingleElement("filename", filename);
                ::fwComEd::CompositeMsg::NewSptr compositeMsg;
                compositeMsg->addEvent( "SNAP", dataInfo );
                ::fwServices::IEditionService::notify(this->getSptr(), composite, compositeMsg);

            }
        }
    
   
}


//------------------------------------------------------------------------------

}

