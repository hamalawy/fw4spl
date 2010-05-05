/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWQT_SLICESELECTOR_HPP_
#define _FWQT_SLICESELECTOR_HPP_

#include <boost/function.hpp>


#include "fwQt/config.hpp"

#include <QWidget>
#include <QHBoxLayout>
#include <QSlider>
#include <QComboBox>
#include <QTextEdit>
#include <QString>

namespace fwQt
{

/**
 * @brief A wxWidgets panel used to control a VTK 2D Negatoscope view.
 * @author IRCAD (Research and Development Team).
 */
class SliceSelector : public QObject
{
  Q_OBJECT
  
public:
    /**
     * @brief Constructor.
     * @param parent The parent window.
     * @param id     The window ID.
     */
     SliceSelector( QWidget*  parent) throw();


    /// @brief Destructor.
     virtual ~SliceSelector() throw();

     void setSliceRange( int min, int max );

     void setSliceValue( int index );

     void setTypeSelection( int type );


    typedef ::boost::function1<void, int > ChangeIndexCallback;
     void setChangeIndexCallback(ChangeIndexCallback fct);

    typedef ::boost::function1<void, int > ChangeTypeCallback;
     void setChangeTypeCallback(ChangeTypeCallback fct);

     QHBoxLayout *layout;
     QStringList m_sliceTypes;
     QComboBox* m_sliceType;
     QSlider*   m_sliceIndex;
     QTextEdit* m_pSliceIndexText;
     QString m_ss;
    
public slots :
    void setSliceIndexText();
    void sliceTypeChange();
     
protected:

     int m_currentSliderIndex;
     int m_maxSliderIndex;

private:

//     QStringList m_sliceTypes;
//     QComboBox* m_sliceType;

    /// @brief Slice type names as a string array for the choice widget.
  //  static const wxString m_sliceTypes[];

    /// @brief Slice type names as a wxWidgets string array.
 //   static const wxArrayString m_sliceTypesArray;

    /// @brief The slice index slider widget.
//     QSlider*   m_sliceIndex;
//     QTextEdit* m_pSliceIndexText;
//     QString m_ss;

    void printIndex(int index);
    void printType(int type);

    ChangeIndexCallback m_fctChangeIndexCallback;
    ChangeTypeCallback m_fctChangeTypeCallback;
};

} // fwWX

#endif // _FWWX_SLICESELECTOR_HPP_
