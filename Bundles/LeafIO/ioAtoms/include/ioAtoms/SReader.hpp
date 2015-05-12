/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __IOATOMS_SREADER_HPP__
#define __IOATOMS_SREADER_HPP__

#include <map>
#include <set>

#include <io/IReader.hpp>

#include "ioAtoms/config.hpp"

namespace fwMemory
{
class IPolicy;
}

namespace ioAtoms
{

/**
 * @brief Atoms reader. Service to load data from Atoms format
 * @class SReader
 * @date 2013
 */
class IOATOMS_CLASS_API SReader : public ::io::IReader
{

public:

    fwCoreServiceClassDefinitionsMacro( (SReader)(::io::IReader) );

    /// Does nothing
    SReader();

    /// Does nothing
    virtual ~SReader() throw()
    {
    }

    /// Propose to choose a medical data file (*.json,*.jsonz,*.xml or *.xmlz)
    IOATOMS_API void configureWithIHM();

    /// Maps file extension to format name.
    typedef std::map< std::string, std::string > FileExtension2NameType;

    /// Managed file extensions
    static const FileExtension2NameType s_EXTENSIONS;

protected:

    /// Does nothing
    IOATOMS_API void starting() throw(::fwTools::Failed);

    /// Does nothing
    IOATOMS_API void stopping() throw(::fwTools::Failed);

    /**
     * @brief Configures the reader.
     * @verbatim
       <config>
        <inject>ReadData</inject>
        <uuidPolicy>Strict|Change|Reuse</uuidPolicy>
        <patcher context="..." version="..." />

        <archive backend="json">
            <extension>.j</extension>
        </archive>

        <archive backend="jsonz">
            <extension>.vpz</extension>
        </archive>

        <archive backend="hdf5">
            <extension>.f4s</extension>
            <extension>.mw</extension>
        </archive>

        <extensions>
            <extension label="XML">.xml</extension>
            <extension label="Zipped XML>.xmlz</extension>
            <extension>.f4s</extension>
            <extension>.j</extension>
            <extension label="Medical workspace">.mw</extension>
            <extension>.vpz</extension>
        </extensions>

       </config>
       @endverbatim
     *
     * archive : defines custom file extensions. The file to be read with an extension given in 'archive' tag will be
     * processed with the given backend in archive tag (the 'backend' attribute is mandatory). Extensions must begin
     * with '.'.
     * Available 'backend' values are json, xml, jsonz, xmlz, and hdf5.
     *
     * extensions : defines available extensions displayed in dialog to read file. If the 'extensions' is empty or not
     * specified, all the extensions (.json, .xml, .jsonz, .xmlz, .hdf5 extensions and custom extensions) are available.
     * The attribute label (not mandatory) allows to display a label in front of extension when the file dialog is
     * shown.
     *
     * @see ::io::IReader
     * @throw ::fwTools::Failed
     */
    IOATOMS_API void configuring() throw(::fwTools::Failed);

    /**
     * @brief Tests file extension, applies the good atom reader, and converts atom in fwData::Composite
     * @note  Before reading, set dump policy to 'barrier dump' if policy is 'never dump', then reset old policy.
     */
    IOATOMS_API void updating() throw(::fwTools::Failed);

    /// Returns managed path type, here service manages only single file
    IOATOMS_API ::io::IOPathType getIOPathType() const;

private:

    /// Notify modification on associated object if reading succeeded
    void notificationOfUpdate();

    /// Initial dump policy
    SPTR(::fwMemory::IPolicy) m_oldPolicy;

    /// fwAtomsConversion uuid policy
    std::string m_inject;

    /// fwAtomsConversion uuid policy
    std::string m_uuidPolicy;

    /// To enable atom patcher
    bool m_useAtomsPatcher;

    /// Defines context of data
    std::string m_context;

    /// Current version of format
    std::string m_version;

    /// Allowed file extensions
    std::set< std::string > m_allowedExts;

    /// IFilter name used to make an atom compliant with current context
    std::string m_filter;

    /// Maps custom extensions to knwon format.
    FileExtension2NameType m_customExts;

    /// Labels shown in file dialog for each allowed extension
    FileExtension2NameType m_allowedExtLabels;
};

} // namespace ioAtoms

#endif // __IOATOMS_SREADER_HPP__

