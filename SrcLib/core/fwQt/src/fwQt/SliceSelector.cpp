/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include <fwCore/base.hpp>

#include "fwQt/SliceSelector.hpp"
#include <iostream>

enum Slice {Sagittal, Frontal, Axial};


namespace fwQt
{


SliceSelector::SliceSelector(QWidget* const parent ) throw()
{
    m_fctChangeIndexCallback = ::boost::bind( &::fwQt::SliceSelector::printIndex, this, _1);
    m_fctChangeTypeCallback = ::boost::bind( &::fwQt::SliceSelector::printType, this, _1);
    
  //  QHBoxLayout *layout = new QHBoxLayout(parent);
    
    m_sliceType = new QComboBox();
    m_pSliceIndexText = new QTextEdit();
    m_sliceIndex = new QSlider(Qt::Horizontal);
    layout = new QHBoxLayout();
//       
     m_sliceType->addItem(QObject::tr("Sagittal"),Sagittal);
     m_sliceType->addItem(QObject::tr("Frontal"),Frontal);
     m_sliceType->addItem(QObject::tr("Axial"), Axial);
     m_sliceType->setMaximumWidth(100);
     // harmobniser height avec les autres widget
     
     m_sliceType->setMinimumHeight(30);
     m_sliceIndex->setFixedHeight(20);
     
    m_sliceIndex->setRange(0, 121);
    m_sliceIndex->setValue(121/2);
    m_sliceIndex->setTickPosition(QSlider::TicksBothSides);
    m_sliceIndex->setTickInterval(10);
    
     m_pSliceIndexText->setFixedHeight(20);
     m_pSliceIndexText->setFixedWidth(90);
     m_pSliceIndexText->setReadOnly(true);

    m_sliceType->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
    m_sliceIndex->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_pSliceIndexText->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);

     layout->setContentsMargins(0,0,0,0);

     layout->addWidget(m_sliceType);
     layout->addWidget(m_sliceIndex);
     layout->addWidget(m_pSliceIndexText);

  parent->setLayout(layout);   
}

//------------------------------------------------------------------------------

SliceSelector::~SliceSelector() throw()
{

}

//------------------------------------------------------------------------------

void SliceSelector::setSliceRange( int min, int max )
{ 
    this->m_sliceIndex->setRange(min, max);
}

//------------------------------------------------------------------------------

void SliceSelector::setSliceValue( int index )
{
    m_sliceIndex->setValue(index);
    std::stringstream ss;
    ss << index << " / " << m_sliceIndex->maximum();
    std::string str;
    ss>>str;
    m_pSliceIndexText->setText((ss.str()).c_str());
//    this->m_pSliceIndexText->Update();
}

//------------------------------------------------------------------------------

void SliceSelector::setSliceIndexText()
{
  // connect(slide, SIGNAL(valueChanged(int)), this, SLOT());
  m_currentSliderIndex = m_sliceIndex->value();
 // m_ss = m_currentSliderIndex <<" / "<<m_maxSliderIndex;
  m_ss.append(m_currentSliderIndex);
  m_ss.append("/");
  m_ss.append(m_maxSliderIndex);

  m_pSliceIndexText->setText(m_ss);
}

//------------------------------------------------------------------------------

void SliceSelector::setTypeSelection( int type )
{

 //   this->m_sliceType->SetSelection(type);

}

//------------------------------------------------------------------------------

void SliceSelector::setChangeIndexCallback(ChangeIndexCallback fct)
{
    m_fctChangeIndexCallback = fct;
}

//------------------------------------------------------------------------------

void SliceSelector::setChangeTypeCallback(ChangeTypeCallback fct)
{
    m_fctChangeTypeCallback = fct;
}

//------------------------------------------------------------------------------

void SliceSelector::printIndex(int index)
{
    OSLM_TRACE("index= "<<index);
}

//------------------------------------------------------------------------------

void SliceSelector::printType(int type)
{
    OSLM_TRACE("type= "<<type);
}



} // fwWX

