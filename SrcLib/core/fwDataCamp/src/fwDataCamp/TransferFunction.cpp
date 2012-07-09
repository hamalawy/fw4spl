#include "fwDataCamp/Factory.hpp"
#include <fwData/camp/mapper.hpp>
#include "fwDataCamp/TransferFunction.hpp"

fwCampImplementDataMacro((fwData)(TransferFunction))
{
    builder
        .tag("object_version", "1")
        .tag("lib_name", "fwData")
        .base< ::fwData::Object>()
        .property("level", &::fwData::TransferFunction::m_attrLevel)
        .property("window", &::fwData::TransferFunction::m_attrWindow)
        .property("name", &::fwData::TransferFunction::m_attrName)
        .property("background_color", &::fwData::TransferFunction::m_attrBackgroundColor)
        .property("tf_data", &::fwData::TransferFunction::m_tfData)
        .property("interpolation_mode", &::fwData::TransferFunction::m_attrInterpolationMode)
        .property("is_clamped", &::fwData::TransferFunction::m_attrIsClamped)
        ;


}

fwCampImplementEnumMacro((fwData)(TransferFunction)(InterpolationMode))
{
    builder
        .value("LINEAR",  ::fwData::TransferFunction::LINEAR)
        .value("NEAREST",  ::fwData::TransferFunction::NEAREST)
        ;
}
