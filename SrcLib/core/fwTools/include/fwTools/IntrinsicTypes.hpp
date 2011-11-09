/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef IntrinsicTypes_hpp
#define IntrinsicTypes_hpp

#include <boost/mpl/push_back.hpp>

#include "fwTools/IntegerTypes.hpp"


namespace fwTools {

#ifdef DEBUG
typedef boost::mpl::push_back< IntegerTypes, float  >::type IntrinsicTypes;
#else
typedef boost::mpl::push_back< boost::mpl::push_back< IntegerTypes, float >::type, double >::type IntrinsicTypes;
#endif

} // end namespace fwTools {

#endif
