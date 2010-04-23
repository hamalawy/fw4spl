/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _UIQTIMAGE_SHOW_SCAN_EDITOR_HPP_
#define _UIQTIMAGE_SHOW_SCAN_EDITOR_HPP_

#include <fwTools/Failed.hpp>
#include <guiQt/editor/IEditor.hpp>

#include "uiQtImage/config.hpp"
#include <QPushButton>
#include <QIcon>
#include <QImage>
 
 
namespace uiImage
{

/**
 * @brief   ShowScanEditor service.
 * @class   ShowScanEditor.
 * @author  IRCAD (Research and Development Team).
 * @date    2010.
 */
class  ShowScanEditor : public ::guiQt::editor::IEditor
{
  Q_OBJECT

public :

    fwCoreServiceClassDefinitionsMacro ( (ShowScanEditor)(::guiQt::editor::IEditor::Baseclass) ) ;

    /// Constructor. Do nothing.
     ShowScanEditor() throw() ;

    /// Destructor. Do nothing.
     virtual ~ShowScanEditor() throw() ;
     
  public slots :
    void changeScanMode();

protected:

    typedef ::fwRuntime::ConfigurationElement::sptr Configuration;

    ///This method launches the IEditor::starting method.
    virtual void starting() throw(::fwTools::Failed);

    ///This method launches the IEditor::stopping method.
    virtual void stopping() throw(::fwTools::Failed);

    /// Managment of observations ( overrides )
    virtual void updating( ::boost::shared_ptr< const fwServices::ObjectMsg > _msg ) throw(::fwTools::Failed);

    virtual void updating() throw(::fwTools::Failed);

    virtual void swapping() throw(::fwTools::Failed);

    virtual void configuring() throw(fwTools::Failed);

    /// Overrides
    virtual void info( std::ostream &_sstream ) ;


private:

    std::string m_adaptorUID;


    bool m_scanAreShown;
    
    QPushButton *m_showScanButton;
    QImage m_imageShowScan;
    QImage m_imageHideScan;
    QIcon icon;
    int m_buttonWidth;

    

};

} // uiImage

#endif /*_UIIMAGE_SHOW_SCAN_EDITOR_HPP_*/


