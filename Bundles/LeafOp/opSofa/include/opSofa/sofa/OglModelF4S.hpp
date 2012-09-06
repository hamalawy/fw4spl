/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef OPSOFA_SOFA_OGLMODELF4S_H
#define OPSOFA_SOFA_OGLMODELF4S_H


#include <sofa/component/visualmodel/OglModel.h>
#include <fwData/Mesh.hpp>

/**
 * @brief Class inherited from OglModel and permitting the SOFA framework
 * to load directly the data without reading the .trian file
 */
class OglModelF4S : public sofa::component::visualmodel::OglModel
{

public:
    void loadMesh(::fwData::Mesh::sptr);
};


#endif // OPSOFA_SOFA_OGLMODELF4S_H
