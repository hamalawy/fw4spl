/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#include <string>

#include <wx/wx.h>
#include <wx/window.h>
#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/combobox.h>

#include <boost/lexical_cast.hpp>

#include <fwCore/base.hpp>

#include <fwServices/helper.hpp>
#include <fwServices/macros.hpp>

#include "proc/Threshold.hpp"
#include "proc/ThresholdEditor.hpp"

REGISTER_SERVICE( ::gui::editor::IEditor , ::proc::ThresholdEditor , ::proc::Threshold ) ;

namespace proc
{
/**
 * @brief Trigger thresholding
 */
struct buttonHandler : public wxEvtHandler
{
	buttonHandler(::proc::Threshold::wptr _threshold ) throw() : m_threshold( _threshold ) {}

	DECLARE_EVENT_TABLE()
	::proc::Threshold::wptr m_threshold;
	void onButton(wxCommandEvent & event)
	{
		//Do refresh parameters
		m_threshold.lock()->update();
	}

};

BEGIN_EVENT_TABLE(buttonHandler, wxEvtHandler)
	EVT_BUTTON(-1, buttonHandler::onButton)
END_EVENT_TABLE()

/**
 * @brief Handler for text control
 *
 * @note The parameter type is used to identify the threshold parameter concerned by the text control
 */
struct ThTextCtrlHandler: public wxEvtHandler
{
	typedef enum
	{
		Lower,
		Upper,
		Inside,
		Outside
	} ParameterType;

	///Constructor
	ThTextCtrlHandler(wxTextCtrl * _textCtrl, ::proc::Threshold::wptr _threshold, ParameterType _parameterType) throw() :
		m_textCtrl(_textCtrl),m_threshold( _threshold ), m_parameterType( _parameterType)
		{}

	// Overrides
	void onEnter(wxCommandEvent & event)
	{
		assert( m_textCtrl == wxDynamicCast( event.GetEventObject(), wxTextCtrl ) );
		wxString textEntered = m_textCtrl->GetValue();
		std::stringstream ss;
		ss << textEntered;
		std::string textInString = ss.str();
		OSLM_TRACE("Entered: " << textInString );
		int param = ::boost::lexical_cast< int >(textInString);
		switch( m_parameterType )
		{
		case Lower :
			m_threshold.lock()->getLowerTh()->value() = param;
			break;
		case Upper :
			m_threshold.lock()->getUpperTh()->value() = param;
			break;
		case Inside :
			m_threshold.lock()->getInsideValue()->value() = param;
			break;
		case Outside :
			m_threshold.lock()->getOutsideValue()->value() = param;
			break;
		default:
			assert(false);
			break;
		}

	}

	DECLARE_EVENT_TABLE()
	::proc::Threshold::wptr m_threshold;
	ParameterType	m_parameterType;
	wxTextCtrl * m_textCtrl;
};

BEGIN_EVENT_TABLE(ThTextCtrlHandler, wxEvtHandler)
	EVT_TEXT_ENTER(-1, ThTextCtrlHandler::onEnter)
END_EVENT_TABLE()


REGISTER_SERVICE( ::gui::editor::IEditor , ::proc::ThresholdEditor , ::proc::Threshold ) ;


ThresholdEditor::ThresholdEditor() throw()
: m_rootWindow(0),m_contentSizer(0),m_lowerThTextControl(0),m_upperThTextControl(0),m_insideValueTextControl(0),m_outsideValueTextControl(0),m_thresholdButtonControl(0)
{}

ThresholdEditor::~ThresholdEditor() throw()
{}

void ThresholdEditor::starting() throw(::fwTools::Failed)
{
    ::gui::editor::IEditor::starting();

    m_threshold = this->getObject< ::proc::Threshold >() ;
	assert( !m_threshold.expired() );
	///Prepare windw
	this->prepareWindow();

	wxString s;
	////////////////////
	// Text control
	////////////////////
	m_lowerThTextControl = new wxTextCtrl(m_rootWindow, -1, _T(""), wxDefaultPosition, wxDefaultSize,wxTE_PROCESS_ENTER );
	m_lowerThTextControl->PushEventHandler( new ThTextCtrlHandler(m_lowerThTextControl,m_threshold,ThTextCtrlHandler::Lower) );
	s.Clear();
	s << m_threshold.lock()->getLowerTh()->value();
	m_lowerThTextControl->SetValue(s);
	m_contentSizer->Add( new wxStaticText( m_rootWindow, -1, _T("Lower Th") ), 0, wxALIGN_CENTER_VERTICAL );
	m_contentSizer->Add( m_lowerThTextControl, 0, wxGROW|wxHORIZONTAL );
	////////////////////
	// Text control
	////////////////////
	m_upperThTextControl = new wxTextCtrl(m_rootWindow, -1, _T(""), wxDefaultPosition, wxDefaultSize,wxTE_PROCESS_ENTER );
	m_upperThTextControl->PushEventHandler( new ThTextCtrlHandler(m_upperThTextControl,m_threshold,ThTextCtrlHandler::Upper) );
	s.Clear();
	s << m_threshold.lock()->getUpperTh()->value();
	m_upperThTextControl->SetValue(s);
	m_contentSizer->Add( new wxStaticText( m_rootWindow, -1, _T("Upper Th") ), 0, wxALIGN_CENTER_VERTICAL );
	m_contentSizer->Add( m_upperThTextControl, 0, wxGROW|wxHORIZONTAL );
	////////////////////
	// Text control
	////////////////////
	m_insideValueTextControl = new wxTextCtrl(m_rootWindow, -1, _T(""), wxDefaultPosition, wxDefaultSize,wxTE_PROCESS_ENTER );
	m_insideValueTextControl->PushEventHandler( new ThTextCtrlHandler(m_insideValueTextControl,m_threshold,ThTextCtrlHandler::Inside) );
	s.Clear();
	s << m_threshold.lock()->getInsideValue()->value();
	m_insideValueTextControl->SetValue(s);
	m_contentSizer->Add( new wxStaticText( m_rootWindow, -1, _T("Inside Value") ), 0, wxALIGN_CENTER_VERTICAL );
	m_contentSizer->Add( m_insideValueTextControl, 0, wxGROW|wxHORIZONTAL );
	////////////////////
	// Text control
	////////////////////
	m_outsideValueTextControl = new wxTextCtrl(m_rootWindow, -1, _T(""), wxDefaultPosition, wxDefaultSize,wxTE_PROCESS_ENTER );
	m_outsideValueTextControl->PushEventHandler( new ThTextCtrlHandler(m_outsideValueTextControl,m_threshold,ThTextCtrlHandler::Outside) );
	s.Clear();
	s << m_threshold.lock()->getOutsideValue()->value();
	m_outsideValueTextControl->SetValue(s);
	m_contentSizer->Add( new wxStaticText( m_rootWindow, -1, _T("Outside Value") ), 0, wxALIGN_CENTER_VERTICAL );
	m_contentSizer->Add( m_outsideValueTextControl, 0, wxGROW|wxHORIZONTAL );
	////////////////////
	// Button control
	////////////////////
	m_thresholdButtonControl = new wxButton( m_rootWindow, -1, _T("Threshold") );
	m_thresholdButtonControl->PushEventHandler( new buttonHandler(m_threshold) );
	m_contentSizer->Add( m_thresholdButtonControl, 0, wxGROW|wxHORIZONTAL );
	m_contentSizer->Add( new wxStaticText( m_rootWindow, -1, _T(" ") ), 0, wxALIGN_CENTER_VERTICAL );
	///////////////////
	// Refresh
	///////////////////
	m_container->Layout() ;
}
void ThresholdEditor::stopping() throw(::fwTools::Failed)
{
    ::gui::editor::IEditor::stopping();
}
void ThresholdEditor::configuring() throw( ::fwTools::Failed )
{
	::gui::editor::IEditor::configuring();
}
void ThresholdEditor::prepareWindow()
{
	// Clears all allocated structures.
	m_container->DestroyChildren();

	// Creates the root window.
	wxBoxSizer * rootSizer = new wxBoxSizer(wxVERTICAL);
	m_rootWindow = new wxScrolledWindow(m_container);
	m_container->SetSizer(rootSizer);
	rootSizer->Add(m_rootWindow, 1, wxGROW|wxALL);
	m_rootWindow->SetScrollRate(10, 10);

	// Prepares the root window content.
	m_contentSizer = new wxFlexGridSizer(2);
	m_contentSizer->SetHGap(10);
	m_contentSizer->SetVGap(5);
	m_rootWindow->SetSizer(m_contentSizer);
}

void ThresholdEditor::updating( ::boost::shared_ptr< const fwServices::ObjectMsg > _mssg ) throw(::fwTools::Failed)
{
	OSLM_TRACE("Message: " << _mssg->getGeneralInfo());
	if( _mssg->hasEvent( ::proc::ThresholdMsg::LOWERTH ) )
	{
		wxString s;
		s << m_threshold.lock()->getLowerTh()->value();
		m_lowerThTextControl->SetValue(s);
	}
	if( _mssg->hasEvent( ::proc::ThresholdMsg::UPPERTH ) )
	{
		wxString s;
		s << m_threshold.lock()->getUpperTh()->value();
		m_upperThTextControl->SetValue(s);
	}
}
}
