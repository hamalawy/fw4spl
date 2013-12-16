/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <QVBoxLayout>
#include <QSlider>
#include <QString>

#include <fwData/Integer.hpp>
#include <fwData/Vector.hpp>
#include <fwData/String.hpp>

#include <fwMedData/ModelSeries.hpp>

//#include <fwServices/helper.hpp>
#include <fwServices/Base.hpp> // new
#include <fwServices/IEditionService.hpp> // new
#include <fwServices/macros.hpp>
#include <fwServices/op/Add.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwGuiQt/container/QtContainer.hpp>
#include <fwCore/spyLog.hpp>

#include "opSofa/SofaMeshEditorSrv.hpp"
#include "opSofa/KeyEvent.hpp"



namespace opSofa
{

fwServicesRegisterMacro(::gui::editor::IEditor, ::opSofa::SofaMeshEditorSrv, ::fwMedData::ModelSeries);

/**
 * @brief Constructor
 */
SofaMeshEditorSrv::SofaMeshEditorSrv() throw()
{
}


/**
 * @brief Destructor
 */
SofaMeshEditorSrv::~SofaMeshEditorSrv() throw()
{
}


/**
 * @brief Used to define the service parameters and analyze its configuration.
 */
void SofaMeshEditorSrv::configuring() throw ( ::fwTools::Failed )
{
    this->initialize();
}


/**
 * @brief Used to launch the service.
 */
void SofaMeshEditorSrv::starting() throw ( ::fwTools::Failed )
{
    //this->create();
    ::fwGuiQt::container::QtContainer::sptr qtContainer =  ::fwGuiQt::container::QtContainer::dynamicCast( this->getContainer() );
    QWidget* const container = qtContainer->getQtContainer();
    assert(container);

    QSlider *slider = new QSlider(Qt::Horizontal, container);
    slider->setToolTip(tr("Select strength"));
    slider->setRange(0, 2000);
    slider->setTickInterval(500);
    slider->setTickPosition(QSlider::TicksBelow);

    KeyEvent *e = new KeyEvent();
    e->setParent(container);

    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->addWidget(slider, 0);
    layout->addWidget(e, 1);

    container->setLayout(layout);

    QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onStrengthSlider(int)));
    QObject::connect(e, SIGNAL(keyPress(QKeyEvent*)), this, SLOT(moveOrgan(QKeyEvent*)));
}


/**
 * @brief Used to stop the service.
 */
void SofaMeshEditorSrv::stopping() throw ( ::fwTools::Failed )
{
    this->getContainer()->clean();
    //this->destroy();
}


/**
 * @brief Called to do an action on the data associated to the service.
 */
void SofaMeshEditorSrv::updating() throw ( ::fwTools::Failed )
{
}


/**
 * @brief Called by a data to notify a service.
 *
 * @param msg : Incoming message
 */
void SofaMeshEditorSrv::receiving( ::fwServices::ObjectMsg::csptr msg ) throw ( ::fwTools::Failed )
{
    if (msg->hasEvent("NEW_RECONSTRUCTION_SELECTED")) {
        m_idReconstruction = ::fwData::String::dynamicConstCast(msg->getDataInfo("NEW_RECONSTRUCTION_SELECTED"));
    }
}

void SofaMeshEditorSrv::onStrengthSlider(int value)
{
    if (m_idReconstruction) {
        // Get acquisition
        ::fwMedData::ModelSeries::sptr ms = this->getObject< ::fwMedData::ModelSeries >();
        SLM_ASSERT("Invalid object", ms);

        ::fwData::Vector::sptr data = ::fwData::Vector::New();
        ::fwData::String::sptr v1 = ::fwData::String::New(m_idReconstruction->value());
        ::fwData::Integer::sptr v2 = ::fwData::Integer::New(value);
        data->getContainer().push_back(v2);
        data->getContainer().push_back(v1);

        // Notification
        ::fwServices::ObjectMsg::sptr msg = ::fwServices::ObjectMsg::New();
        msg->addEvent("EDITOR_MESH_SOFA", data);
        ::fwServices::IEditionService::notify(this->getSptr(), ms, msg);
    }
}

void SofaMeshEditorSrv::moveOrgan(QKeyEvent* event)
{
    ::fwMedData::ModelSeries::sptr ms = this->getObject< ::fwMedData::ModelSeries >();
    SLM_ASSERT("Invalid object", ms);

    static int x = 0;
    static int y = 0;
    static int z = 0;

    if (event->key() == Qt::Key_Up) y += 1;
    if (event->key() == Qt::Key_Down) y -= 1;
    if (event->key() == Qt::Key_Left) x -= 1;
    if (event->key() == Qt::Key_Right) x += 1;
    if (event->key() == Qt::Key_A) z += 1;
    if (event->key() == Qt::Key_Q) z -= 1;

    ::fwData::Vector::sptr data = ::fwData::Vector::New();
    ::fwData::String::sptr v1 = ::fwData::String::New(m_idReconstruction->value());
    ::fwData::Integer::sptr v2 = ::fwData::Integer::New(x);
    ::fwData::Integer::sptr v3 = ::fwData::Integer::New(y);
    ::fwData::Integer::sptr v4 = ::fwData::Integer::New(z);
    data->getContainer().push_back(v1);
    data->getContainer().push_back(v2);
    data->getContainer().push_back(v3);
    data->getContainer().push_back(v4);

    // Notification
    ::fwServices::ObjectMsg::sptr msg = ::fwServices::ObjectMsg::New();
    msg->addEvent("MOVE_MESH_SOFA", data);
    ::fwServices::IEditionService::notify(this->getSptr(), ms, msg);
}

}

