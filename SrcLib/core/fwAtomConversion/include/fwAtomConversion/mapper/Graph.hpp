/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWATOMCONVERSION_MAPPER_GRAPH_HPP__
#define __FWATOMCONVERSION_MAPPER_GRAPH_HPP__


#include "fwAtomConversion/AtomVisitor.hpp"
#include "fwAtomConversion/config.hpp"
#include "fwAtomConversion/mapper/Base.hpp"

namespace fwAtoms
{
    class Object;
}

namespace fwData
{
    class Object;
}

namespace fwAtomConversion
{
namespace mapper
{

/**
 * @brief Specific mapper used to convert a ::fwData::Graph.
 * @class Graph
 * @date 2012-2013
 *
 * The attribute ::fwData::Graph::m_connections ( of type ::fwData::Graph::ConnectionContainer ) is
 * represented in fwAtoms world like a fwAtoms::Sequence that contains fwAtoms::Object with three
 * attributes "edge", "source" and "destination".
 */
class FWATOMCONVERSION_CLASS_API Graph : public Base
{
public:

    /**
     * @brief Convert a ::fwData::Object to a ::fwAtoms::Object.
     * @param object data to convert. It must be a ::fwData::Graph.
     * @param cache  cache to register the data already converted, used when a data is referenced multiple times.
     */
    FWATOMCONVERSION_API virtual SPTR(::fwAtoms::Object) convert ( SPTR(::fwData::Object) object,
                                                                   DataVisitor::AtomCacheType & cache );

    /**
     * @brief Convert a ::fwAtoms::Object to a ::fwData::Object (which represent a ::fwData::Graph).
     * @param object atom to convert
     * @param cache  cache to register the atoms already converted, used when an atom is referenced multiple times.
     * @param uuidPolicy AtomVisitor policy
     */
    FWATOMCONVERSION_API virtual SPTR(::fwData::Object) convert ( SPTR(::fwAtoms::Object) atom,
                                                                  AtomVisitor::DataCacheType & cache,
                                                                  const AtomVisitor::IReadPolicy &uuidPolicy
                                                                  );

};

}
}

#endif /*  __FWATOMCONVERSION_MAPPER_GRAPH_HPP__*/
