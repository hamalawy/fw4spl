/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWRUNTIME_IO_VALIDATOR_HPP__
#define __FWRUNTIME_IO_VALIDATOR_HPP__

#include <string>
#include <sstream>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include <libxml/tree.h>
#include <libxml/xmlschemastypes.h>

#include "fwRuntime/config.hpp"

namespace fwRuntime
{

namespace io
{



/**
 * @brief   Implements an XML validator.
 * @struct  Validator
 *
 * @warning Potential Bug : class Validator can generate bug (infinite loop) when used with other Validator class instance.
 * It seems that this class use a static ivar to save schemaValidationContext. If your call a instance of an visitor this ivar should not be
 * directly well positionned. So the recommandation is to construct Validator-Use it-destry it and never store him
 * @date    2007-2009
 *
 */
struct Validator
{

    /**
     * @brief   Copy Constructor
     *
     * Copy a validator.
     *
     * @param   validator to be copied
     */
    FWRUNTIME_API Validator( const Validator &validator );

    /**
     * @brief   Constructor
     *
     * Builds a validator with a string containing a XML shema.
     *
     * @param   schema  a string containing the XML schema
     */
    FWRUNTIME_API Validator( const std::string & schema );

    /**
     * @brief   Constructor
     *
     * Builds a validator with a path to a file containing an XML schema.
     *
     * @param   path    a path to the xml schema file to load
     */
    FWRUNTIME_API Validator( const boost::filesystem::path & path );

    /**
     * @brief   Destructor
     */
    FWRUNTIME_API ~Validator();

    /**
     * @brief   Clears the error log.
     */
    void clearErrorLog();

    /**
     * @brief   Retrieves the error log content.
     *
     * @return  a string containing the error log
     */
    FWRUNTIME_API const std::string getErrorLog() const;

    /**
     * @brief   Validates the given file.
     *
     * @param   xmlFile a path to an xml file to validate
     *
     * @return  true when the validation succeeds, false otherwise
     */
    FWRUNTIME_API bool validate( const boost::filesystem::path & xmlFile );

    /**
     * @brief   Validates the given xml node.
     *
     * @param   node    a pointer to an xml node
     *
     * @return  true when the validation succeeds, false otherwise
     */
    FWRUNTIME_API bool validate( xmlNodePtr node );

    /**
     * @brief   Returns the xsd content in string format
     *
     * @return  xsd of the validator
     */
    FWRUNTIME_API std::string getXsdContent();


    private:

        void initializeContext();

        std::string m_xsd_content;
        std::ostringstream m_errorLog;

        typedef ::boost::shared_ptr< xmlSchemaParserCtxt > SchemaParserCtxtSptr;
        typedef ::boost::shared_ptr< xmlSchema > SchemaSptr;
        typedef ::boost::shared_ptr< xmlSchemaValidCtxt > SchemaValidCtxtSptr;

        SchemaParserCtxtSptr m_schemaParserContext;
        SchemaSptr m_schema;
        SchemaValidCtxtSptr m_schemaValidContext;

        static void ErrorHandler( void * userData, xmlErrorPtr error );

};



} // namespace io

} // namespace fwRuntime



#endif /*__FWRUNTIME_IO_VALIDATOR_HPP__*/
