#ifndef _UIACQUISITION_ORGAN_LIST_EDITOR_HPP_
#define _UIACQUISITION_ORGAN_LIST_EDITOR_HPP_

#include <fwData/Reconstruction.hpp>

#include <fwTools/Failed.hpp>
#include <guiQt/editor/IEditor.hpp>

#include "uiAcquisition/config.hpp"

#include <QApplication>
 #include <QWidget>
 #include <QGroupBox>
 #include <QCheckBox>
#include <QVBoxLayout>
#include <QTextEdit>
 #include <QListWidget>
  #include <QListWidgetItem>

namespace uiAcquisition
{

/**
 * @brief   OrganListEditor service.
 * @class   OrganListEditor.
 * @author  IRCAD (Research and Development Team).
 * @date    2010.
 */
class  OrganListEditor : public ::guiQt::editor::IEditor
{
  Q_OBJECT

public :

    fwCoreServiceClassDefinitionsMacro ( (OrganListEditor)(::guiQt::editor::IEditor::Baseclass) ) ;

    /// Constructor. Do nothing.
     OrganListEditor() throw() ;

    /// Destructor. Do nothing.
     virtual ~OrganListEditor() throw() ;
    
    
public slots :
  void organChoiceSelection();
  void organChoiceVisibility();
  void showReconstructions();
  
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

    typedef std::map< std::string, ::fwData::Reconstruction::sptr > OrganNameReconstruction;

    void updateReconstructions();
    void notifyOrganChoiceSelection();
    
/*
    void onOrganChoiceSelection(wxCommandEvent & event );
*/
private:

    void refreshVisibility();
    QGroupBox *m_organs;
    QCheckBox* m_showCheckBox;
    OrganNameReconstruction m_map ;    
    QListWidget* m_listW;
    QListWidgetItem* m_item;
    QVBoxLayout *m_listLayout;
};

} // uiAcquisition

#endif /*_UIACQUISITION_ORGAN_LIST_EDITOR_HPP_*/


