/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#ifndef PROC_THRESHOLDEDITOR_HPP_
#define PROC_THRESHOLDEDITOR_HPP_

#include <fwTools/Failed.hpp>
#include <gui/editor/IEditor.hpp>

#include "proc/Threshold.hpp"
#include "proc/config.hpp"

class wxWindow;
class wxScrolledWindow;
class wxFlexGridSizer;
class wxButton;
class wxTextCtrl;

namespace proc
{
/**
 * @brief GUI editor service dedicated to the modification on the parameters of the thresholding algorithm.
 */
class PROC_CLASS_API ThresholdEditor : public ::gui::editor::IEditor
{

public :


	fwCoreServiceClassDefinitionsMacro ( (ThresholdEditor)(::gui::editor::IEditor) ) ;

	///	Constructor. Do nothing.
	PROC_API ThresholdEditor() throw() ;

	///	Destructor. Do nothing.
	PROC_API virtual ~ThresholdEditor() throw() ;

protected:
    /**
     * @name General service methods
     */

    //@{

	///Initialize gui controls
	PROC_API virtual void starting() throw(::fwTools::Failed);

	///Only invokes super class stopping method (to release Gui sub-elements: the controls)
	PROC_API virtual void stopping() throw(::fwTools::Failed);

	///Only invokes super class stopping method (to retrieve the GUI container)
	PROC_API virtual void configuring() throw( ::fwTools::Failed ) ;

	///Observes the Threshold object to refresh gui controls
	PROC_API virtual void updating( ::boost::shared_ptr< const fwServices::ObjectMsg > _mssg ) throw(::fwTools::Failed) ;
    //@}

    /**
     * @name General window layout
     */

    //@{

	/// Prepare the wx Container to easily install controls
	void prepareWindow();

	///Window organization
	wxScrolledWindow * m_rootWindow;
	///Window organization
	wxFlexGridSizer * m_contentSizer;

    //@}

	///The threshold processor to which the editor will be attached to
	::proc::Threshold::wptr	m_threshold;

    /**
     * @name Specific GUI controls installed by the starting method to editor threshold parameters
     */

    //@{

	wxTextCtrl		*m_lowerThTextControl;
	wxTextCtrl		*m_upperThTextControl;
	wxTextCtrl		*m_insideValueTextControl;
	wxTextCtrl		*m_outsideValueTextControl;
	wxButton		*m_thresholdButtonControl;

    //@}
};


}

#endif /* PROC_THRESHOLDEDITOR_HPP_ */
