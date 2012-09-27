/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <QDir>
#include <QStringList>
#include <QFile>
#include <QString>
#include <QTextStream>

#include <boost/bind.hpp>

#include <fwCore/base.hpp>

#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>
#include <fwRuntime/profile/Profile.hpp>

#include <fwServices/macros.hpp>

#include <fwGuiQt/App.hpp>

#include "guiQt/Plugin.hpp"

namespace guiQt
{
//-----------------------------------------------------------------------------


static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::guiQt::Plugin");

//-----------------------------------------------------------------------------

Plugin::~Plugin() throw()
{}

//-----------------------------------------------------------------------------

void Plugin::start() throw(::fwRuntime::RuntimeException)
{
    SLM_TRACE_FUNC();

    ::fwRuntime::profile::Profile::sptr profile = ::fwRuntime::profile::getCurrentProfile();
    SLM_ASSERT("Profile is not initialized", profile);
    ::fwRuntime::profile::Profile::ParamsContainer params = profile->getParams();
    m_argc = params.size();
    char** argv = profile->getRawParams();

    ::fwGuiQt::App *app;
    app = new ::fwGuiQt::App( m_argc, argv );
    m_app = app;

    QStringList libraryPaths;
    libraryPaths = app->libraryPaths();
    libraryPaths.removeFirst();
    app->setLibraryPaths(libraryPaths);

    QDir pluginDir("./qtplugins");
    if (pluginDir.exists())
    {
        app->addLibraryPath(pluginDir.absolutePath());
    }
    this->loadStyleSheet();
    ::fwRuntime::profile::getCurrentProfile()->setRunCallback(::boost::bind(&Plugin::run, this));
}

//-----------------------------------------------------------------------------

void Plugin::stop() throw()
{
    delete m_app;
}

//-----------------------------------------------------------------------------

int Plugin::run() throw()
{
    ::fwRuntime::profile::getCurrentProfile()->setup();
    int res = m_app->exec();
    ::fwRuntime::profile::getCurrentProfile()->cleanup();
    return res;
}

//-----------------------------------------------------------------------------

void Plugin::loadStyleSheet()
{
    if( this->getBundle()->hasParameter("style") )
    {
        std::string styleFile = this->getBundle()->getParameterValue("style") ;

        QFile data(QString::fromStdString(styleFile));
        QString style;
        if(data.open(QFile::ReadOnly))
        {
            QTextStream styleIn(&data);
            style = styleIn.readAll();
            data.close();
            qApp->setStyleSheet(style);
        }
    }
}

} // namespace guiQt
