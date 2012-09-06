/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/make_constructor.hpp>

#include <boost/algorithm/string/replace.hpp>

#include <vector>

#include <fwTools/Stringizer.hpp>
#include <fwCore/Demangler.hpp>
#include "fwPython/bindings/STLContainers.hpp"

namespace fwPython
{
namespace bindings
{

template <class T>
std::string getString(std::vector<T> &v)
{
    std::string res;
    res +=  fwTools::getString(v.begin(),v.end());
    return res;
}


template< typename VECTOR >
VECTOR *vectorFromList( ::boost::python::list  &ns )
{
    unsigned long  len = ::boost::python::len(ns);
    VECTOR *vect = new VECTOR( len );
    for (unsigned long i=0; i<len; ++i )
        (*vect)[i] = ::boost::python::extract<typename VECTOR::value_type> (ns[i]);
    return vect;
}




template< typename T >
void wrap_vector()
{
    using namespace boost::python;
    std::string className = ::fwCore::Demangler(typeid(typename T::value_type)).getLeafClassname();
    ::boost::algorithm::replace_all( className, " ", "");
    className += "Container";
    std::string docString = std::string( "binding of " ) + ::fwCore::Demangler(typeid(T)).getClassname();
    class_< T  >( className.c_str(), docString.c_str(), init< typename T::size_type > () )
        .def("__init__", make_constructor( vectorFromList< T > ))
        // install wrapper to __len__, __getitem__, __setitem__, __delitem__, __iter__ and __contains.
        .def(vector_indexing_suite< T >())

        // allow a pretty print
        .def("__str__", getString< typename T::value_type >)
        ;
}
//------------------------------------------------------------------------------

void export_STLContainers()
{
    using namespace boost::python;
    wrap_vector< std::vector<boost::int8_t> >();
    wrap_vector< std::vector<boost::int16_t> >();
    wrap_vector< std::vector<boost::int32_t> >();
    wrap_vector< std::vector<boost::int64_t> >();

    wrap_vector< std::vector<boost::uint8_t> >();
    wrap_vector< std::vector<boost::uint16_t> >();
    wrap_vector< std::vector<boost::uint32_t> >();
    wrap_vector< std::vector<boost::uint64_t> >();

    wrap_vector< std::vector<float> >();
    wrap_vector< std::vector<double> >();

#ifdef __MACOSX__
    wrap_vector< std::vector<size_t> >();
#endif
}

} // end namespace bindings
} // end namespace fwPython
