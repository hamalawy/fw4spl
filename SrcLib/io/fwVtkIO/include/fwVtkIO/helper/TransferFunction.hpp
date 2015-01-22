/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _VTKIO_HELPER_TRANSFERFUNCTION_HPP_
#define _VTKIO_HELPER_TRANSFERFUNCTION_HPP_

#include <vtkSmartPointer.h>
#include <vtkLogLookupTable.h>

#include <fwData/TransferFunction.hpp>

#include "fwVtkIO/config.hpp"

namespace fwVtkIO
{

namespace helper
{

/**
 * @brief   Helper to manage vtk data from ::fwData::TransferFunction.
 * @class   TransferFunction
 * 
 * @date    2012.
 *
 */
class FWVTKIO_CLASS_API TransferFunction
{

public :

    /*!
     * @brief Convert a ::fwData::TransferFunction to a vtkLookupTable.
     *
     * @param[in] tf ::fwData::TransferFunction
     * @param[in] allowTransparency Enable conversion of ::fwData::TransferFunction alpha channel
     * @param[in] size lookup table size
     * @param[out] lt vtkLookupTable
     */
    FWVTKIO_API static void toVtkLookupTable(
            ::fwData::TransferFunction::sptr tf,
             vtkSmartPointer<vtkLookupTable>  lt,
            bool allowTransparency = false,
            unsigned int size = 256 );

    /*!
     * @brief Convert the range of an Image or a TransferFunction to a black and white vtkLookupTable.
     *
     * @param[in] rangeMin
     * @param[in] rangeMax
     * @param[in] size lookup table size
     * @param[out] lt vtkLookupTable
     *
     */
    FWVTKIO_API static void toBWVtkLookupTable(   double rangeMin, double rangeMax,
                                                vtkSmartPointer<vtkLookupTable> lt,
                                                unsigned int size = 256 );

  };

} // namespace helper
} // namespace fwVtkIO

#endif // _VTKIO_HELPER_TRANSFERFUNCTION_HPP_
