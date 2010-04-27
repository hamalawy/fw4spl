/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _UIQTIMAGE_SLICEINDEXPOSITIONEDITOR_HPP_
#define _UIQTIMAGE_SLICEINDEXPOSITIONEDITOR_HPP_

#include <fwTools/Failed.hpp>
#include <guiQt/editor/IEditor.hpp>

#include <fwComEd/helper/MedicalImageAdaptor.hpp>
#include <fwQt/SliceSelector.hpp>

#include "uiQtImage/config.hpp"

namespace uiImage
{

/**
 * @brief   SliceIndexPositionEditor service.
 * @class   SliceIndexPositionEditor.
 * @author  IRCAD (Research and Development Team).
 * @date    2010.
 */
class  SliceIndexPositionEditor : public ::guiQt::editor::IEditor, public ::fwComEd::helper::MedicalImageAdaptor
{
  Q_OBJECT

public :

    fwCoreServiceClassDefinitionsMacro ( (SliceIndexPositionEditor)(::guiQt::editor::IEditor::Baseclass) ) ;

    /// Constructor. Do nothing.
     SliceIndexPositionEditor() throw() ;

    /// Destructor. Do nothing.
     virtual ~SliceIndexPositionEditor() throw() ;
     
  public slots :
  void sliceIndexSlot();

protected:

    /// @brief The slice type: axial, frontal, sagittal.
    using ::fwComEd::helper::MedicalImageAdaptor::Orientation ;

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

     void updateSliceIndex();

     void updateSliceType(Orientation type );

     void sliceIndexNotification(unsigned int index);

     void sliceTypeNotification( int type );

private:

    /// @brief The field IDs for the slice index.
    static const std::string* SLICE_INDEX_FIELDID[ 3 ];

    ::fwQt::SliceSelector* m_sliceSelectorPanel;

};

} // uiImage

#endif /*_UIIMAGE_SLICEINDEXPOSITIONEDITOR_HPP_*/


