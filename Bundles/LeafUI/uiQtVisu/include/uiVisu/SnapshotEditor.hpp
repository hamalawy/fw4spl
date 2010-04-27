/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _UIVISU_SNAPSHOTEDITOR_HPP_
#define _UIVISU_SNAPSHOTEDITOR_HPP_

#include <fwTools/Failed.hpp>
#include <guiQt/editor/IEditor.hpp>

#include "uiVisu/config.hpp"

#include <QPixmap>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QImage>
#include <QIcon>



namespace uiVisu
{

/**
 * @brief   SnapshotEditor service.
 * @class   SnapshotEditor.
 * @author  IRCAD (Research and Development Team).
 * @date    2010.
 */
class  SnapshotEditor : public ::guiQt::editor::IEditor
{
  Q_OBJECT

public :

    fwCoreServiceClassDefinitionsMacro ( (SnapshotEditor)(::guiQt::editor::IEditor::Baseclass) ) ;

    /// Constructor. Do nothing.
     SnapshotEditor() throw() ;

    /// Destructor. Do nothing.
     virtual ~SnapshotEditor() throw() ;

     
private slots:
     void snapShoot();


protected:

    typedef ::fwRuntime::ConfigurationElement::sptr Configuration;

    ///This method launches the IEditor::starting method.
    virtual void starting() throw(::fwTools::Failed);

    ///This method launches the IEditor::stopping method.
    virtual void stopping() throw(::fwTools::Failed);

    /// Managment of observations ( overides )
    virtual void updating( ::boost::shared_ptr< const fwServices::ObjectMsg > _msg ) throw(::fwTools::Failed);

    virtual void updating() throw(::fwTools::Failed);

    virtual void swapping() throw(::fwTools::Failed);

    virtual void configuring() throw(fwTools::Failed);

    /// Overrides
    virtual void info( std::ostream &_sstream ) ;

private:

    std::vector< std::string > m_scenesUID;
    
     QPixmap m_pixmap;
     QLabel *m_screenshotLabel;
     QPushButton *m_snapButton;
     QImage m_imageSnap;
     QIcon m_icon;



};

} // uiVisu

#endif /*_UIVISU_SNAPSHOTEDITOR_HPP_*/


