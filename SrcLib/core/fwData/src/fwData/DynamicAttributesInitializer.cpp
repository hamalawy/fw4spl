#include <fwCore/base.hpp>
#include <fwTools/Factory.hpp>

#include "fwData/DynamicAttributesInitializer.hpp"
#include "fwData/Object.hpp"

class InitializerRegistrar : public ::fwCore::BaseObject
{
public :
    fwCoreClassDefinitionsWithFactoryMacro((InitializerRegistrar), (()), new InitializerRegistrar );

    InitializerRegistrar()
    {
        ::fwTools::Factory::addInitializer( ::fwData::DynamicAttributesInitializer::New() ) ;
    }

    ~InitializerRegistrar()
    {}
} ;

InitializerRegistrar::wptr reg = InitializerRegistrar::New();


namespace fwData
{

void DynamicAttributesInitializer::init( ::fwTools::Object::wptr _obj)
{
    ::fwData::Object::sptr obj = ::fwData::Object::dynamicCast(_obj.lock());
    if (obj)
    {
        obj->__FWTOOLS_ATTRIBUTES_REGISTER_FUNC_NAME();
    }
}

void DynamicAttributesInitializer::uninit( ::fwCore::LogicStamp::csptr key )
{
    //TODO
}

} //end namespace fwData
