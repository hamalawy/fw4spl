#ifndef _UIRECONSTRUCTION_ORGAN_MATERIAL_EDITOR_HPP_
#define _UIRECONSTRUCTION_ORGAN_MATERIAL_EDITOR_HPP_

#include <fwTools/Failed.hpp>

#include <guiQt/editor/IEditor.hpp>

#include "uiReconstruction/config.hpp"

#include <fwData/Material.hpp>

 #include <QWidget>
 #include <QGroupBox>
 #include <QCheckBox>
#include <QVBoxLayout>
#include <QTextEdit>
 #include <QButtonGroup>

namespace uiReconstruction
{

/**
 * @brief   RepresentationEditor service.
 * @class   RepresentationEditor.
 * @author  IRCAD (Research and Development Team).
 * @date    2010.
 */
class UIRECONSTRUCTION_CLASS_API RepresentationEditor : public ::guiQt::editor::IEditor
{
  Q_OBJECT

public :

    fwCoreServiceClassDefinitionsMacro ( (RepresentationEditor)(::guiQt::editor::IEditor::Baseclass) ) ;

    /// Constructor. Do nothing.
    UIRECONSTRUCTION_API RepresentationEditor() throw() ;

    /// Destructor. Do nothing.
    UIRECONSTRUCTION_API virtual ~RepresentationEditor() throw() ;
    
    
public slots :
      void showNormals();
      void changeRepresentation();
      void changeShading();
      
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
    
    void notifyTriangularMesh();   
    void notifyMaterial();

private:

    void refreshNormals();
    
    void refreshRepresentation() ;
    void refreshShading() ;
  
    QGroupBox *m_groupRepresentation;
    QGroupBox *m_groupShading;
    QButtonGroup *m_radioBoxRepresentation;
    QButtonGroup *m_radioBoxShading;
    QCheckBox *m_normalsCheckBox;
    QButtonGroup *m_groupButtonRepresentation;
    QButtonGroup *m_groupButtonShading;
    QVBoxLayout *layout;    
    QVBoxLayout *repLayout;
    QVBoxLayout *shadingLayout;

    ::fwData::Material::sptr m_material ;

};

} // uiReconstruction

#endif /*_UIRECONSTRUCTION_ORGAN_MATERIAL_EDITOR_HPP_*/


