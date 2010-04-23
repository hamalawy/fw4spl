/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _UIQTIMAGE_SLICE_LIST_EDITOR_HPP_
#define _UIQTIMAGE_SLICE_LIST_EDITOR_HPP_

#include <fwTools/Failed.hpp>
#include <guiQt/editor/IEditor.hpp>

#include "uiQtImage/config.hpp"

#include <QWidget>
#include <QPushButton>
#include <QActionGroup>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QIcon>
#include <QImage>
 
 
namespace uiImage
{

/**
 * @brief   SliceListEditor service.
 * @class   SliceListEditor.
 * @author  IRCAD (Research and Development Team).
 * @date    2010.
 */
class  SliceListEditor : public ::guiQt::editor::IEditor
{
  Q_OBJECT
  
public :

    fwCoreServiceClassDefinitionsMacro ( (SliceListEditor)(::guiQt::editor::IEditor::Baseclass) ) ;

    /// Constructor. Do nothing.
     SliceListEditor() throw() ;

    /// Destructor. Do nothing.
     virtual ~SliceListEditor() throw() ;
     
     
  public slots :
    void createPopUpMenu();

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

    QWidget *m_widget;
    QPushButton *m_button;
    QActionGroup *m_sliceGroup;
    QAction *m_oneSliceItem;
    QAction *m_threeSlicesItem;
    QMenuBar *m_menuBar;
    QMenu *m_menu;
    
    int m_buttonWidth;

private:

    std::string m_adaptorUID;
    int m_idDropDown;
    int m_nbSlice;
    
    // members
    

};

} // uiImage

#endif /*_UIIMAGE_SLICE_LIST_EDITOR_HPP_*/


