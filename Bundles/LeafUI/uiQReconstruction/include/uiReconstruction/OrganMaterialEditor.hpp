#ifndef _UIRECONSTRUCTION_ORGAN_MATERIAL_EDITOR_HPP_
#define _UIRECONSTRUCTION_ORGAN_MATERIAL_EDITOR_HPP_

#include <fwTools/Failed.hpp>

#include <guiQt/editor/IEditor.hpp>

#include "uiReconstruction/config.hpp"
 
 #include <QWidget>
 #include <QVBoxLayout>
 #include <QLineEdit>
 #include <QSlider>
 #include <QPushButton>
  #include <QLabel>
 #include <QVBoxLayout>

namespace uiReconstruction
{
/**
 * @brief   OrganMaterialEditor service.
 * @class   OrganMaterialEditor.
 * @author  IRCAD (Research and Development Team).
 * @date    2010.
 */
class UIRECONSTRUCTION_CLASS_API OrganMaterialEditor : public ::guiQt::editor::IEditor
{
  Q_OBJECT

public :

    fwCoreServiceClassDefinitionsMacro ( (OrganMaterialEditor)(::guiQt::editor::IEditor::Baseclass) ) ;

    /// Constructor. Do nothing.
    UIRECONSTRUCTION_API OrganMaterialEditor() throw() ;

    /// Destructor. Do nothing.
    UIRECONSTRUCTION_API virtual ~OrganMaterialEditor() throw() ;
    
    
public slots :
      void opacitySlider();
      void colorButton();


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

    void materialNotification( );
    void refreshMaterial( );

private:

    QPushButton * m_colourButton ;
    QSlider * m_opacitySlider ;
    QLabel *m_label;
    QVBoxLayout *layout;

};

} // uiReconstruction

#endif /*_UIRECONSTRUCTION_ORGAN_MATERIAL_EDITOR_HPP_*/


