/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/filesystem/path.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string/join.hpp>

#include <fwAtomsBoostIO/Reader.hpp>

#include <fwAtomsHdf5IO/Reader.hpp>

#include <fwAtomConversion/convert.hpp>

#include <fwAtomsFilter/IFilter.hpp>
#include <fwAtomsFilter/factory/new.hpp>


#include <fwData/Object.hpp>
#include <fwData/Composite.hpp>
#include <fwData/location/SingleFile.hpp>
#include <fwData/location/Folder.hpp>

#include <fwComEd/helper/Composite.hpp>

#include <fwGui/Cursor.hpp>
#include <fwGui/dialog/LocationDialog.hpp>
#include <fwGui/dialog/MessageDialog.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwZip/ReadDirArchive.hpp>
#include <fwZip/ReadZipArchive.hpp>

#include <fwMemory/IPolicy.hpp>
#include <fwMemory/BufferManager.hpp>
#include <fwMemory/policy/BarrierDump.hpp>
#include <fwMemory/policy/NeverDump.hpp>
#include <fwMemory/tools/MemoryMonitorTools.hpp>

#include <fwAtomsPatch/PatchingManager.hpp>

#include "ioAtoms/SReader.hpp"

namespace ioAtoms
{

fwServicesRegisterMacro( ::io::IReader , ::ioAtoms::SReader , ::fwData::Object );

const SReader::FileExtension2NameType SReader::s_EXTENSIONS
    = ::boost::assign::map_list_of(".xml", "XML")
        (".xmlz", "Zipped XML")
        (".json", "JSON")
        (".jsonz", "Zipped JSON")
        (".hdf5", "HDF5");

//-----------------------------------------------------------------------------

SReader::SReader() :
        m_useAtomsPatcher(false),
        m_context ("Undefined"),
        m_version ("Undefined"),
        m_filter  ("")
{
    BOOST_FOREACH(SReader::FileExtension2NameType::value_type ext, s_EXTENSIONS)
    {
        m_allowedExts.insert(m_allowedExts.end(), ext.first);
    }
}

//-----------------------------------------------------------------------------

void SReader::starting() throw(::fwTools::Failed)
{}

//-----------------------------------------------------------------------------

void SReader::stopping() throw(::fwTools::Failed)
{}

//-----------------------------------------------------------------------------

void SReader::configuring() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    ::io::IReader::configuring();

    typedef SPTR(::fwRuntime::ConfigurationElement) ConfigurationElement;
    typedef std::vector < ConfigurationElement >    ConfigurationElementContainer;

    m_customExts.clear();
    m_allowedExtLabels.clear();

    ConfigurationElementContainer customExtsList = m_configuration->find("archive");
    BOOST_FOREACH(ConfigurationElement archive, customExtsList)
    {
        const std::string& backend = archive->getAttributeValue("backend");
        SLM_ASSERT("No backend attribute given in archive tag", backend != "");
        SLM_ASSERT("Unsupported backend '" + backend + "'", s_EXTENSIONS.find("." + backend) != s_EXTENSIONS.end());

        ConfigurationElementContainer exts = archive->find("extension");
        BOOST_FOREACH(ConfigurationElement ext, exts)
        {
            const std::string& extension = ext->getValue();
            SLM_ASSERT("No extension given for backend '" + backend + "'", !extension.empty());
            SLM_ASSERT("Extension must begin with '.'", extension[0] == '.');

            m_customExts[extension] = backend;
            m_allowedExtLabels[extension] = ext->getAttributeValue("label");
        }
    }

    ConfigurationElementContainer extensionsList = m_configuration->find("extensions");
    SLM_ASSERT("The <extensions> element can be set at most once.", extensionsList.size() <= 1);

    if(extensionsList.size() == 1)
    {
        m_allowedExts.clear();

        ConfigurationElementContainer extensions = extensionsList.at(0)->find("extension");
        BOOST_FOREACH(ConfigurationElement extension, extensions)
        {
            const std::string& ext = extension->getValue();

            // The extension must be found either in custom extensions list or in known extensions
            FileExtension2NameType::const_iterator itKnown = s_EXTENSIONS.find(ext);
            FileExtension2NameType::const_iterator itCustom = m_customExts.find(ext);

            const bool extIsKnown = (itKnown != SReader::s_EXTENSIONS.end() || itCustom != m_customExts.end());
            SLM_ASSERT("Extension '" + ext + "' is not allowed in configuration", extIsKnown);

            if(extIsKnown)
            {
                m_allowedExts.insert(m_allowedExts.end(), ext);
                m_allowedExtLabels[ext] = extension->getAttributeValue("label");
            }
        }
    }
    else
    {
        m_allowedExts.clear();

        BOOST_FOREACH(FileExtension2NameType::value_type ext, m_customExts)
        {
            m_allowedExts.insert(m_allowedExts.end(), ext.first);
        }

        BOOST_FOREACH(SReader::FileExtension2NameType::value_type ext, SReader::s_EXTENSIONS)
        {
            m_allowedExts.insert(m_allowedExts.end(), ext.first);
            m_allowedExtLabels[ext.first] = ext.second;
        }
    }

    ConfigurationElementContainer inject = m_configuration->find("inject");
    SLM_ASSERT("The <inject> element can be set at most once.", inject.size() <= 1);
    if (inject.size() == 1)
    {
        m_inject = inject.at(0)->getValue();
    }

    ConfigurationElementContainer filter = m_configuration->find("filter");
    SLM_ASSERT("The <filter> element can be set at most once.", filter.size() <= 1);
    if (filter.size() == 1)
    {
        m_filter = filter.at(0)->getValue();
    }

    ConfigurationElementContainer uuidPolicy = m_configuration->find("uuidPolicy");
    SLM_ASSERT("The <uuidPolicy> element can be set at most once.", uuidPolicy.size() <= 1);
    if (uuidPolicy.size() == 1)
    {
        m_uuidPolicy = uuidPolicy.at(0)->getValue();
        SLM_ASSERT("Unknown policy : '"
                   + m_uuidPolicy +
                   "', available policies : 'Strict','Change' or 'Reuse'.",
                   "Strict" == m_uuidPolicy || "Change" == m_uuidPolicy || "Reuse" == m_uuidPolicy );

        SLM_ASSERT("'Reuse' policy is available only with inject mode",
                   ("Reuse" == m_uuidPolicy && !m_inject.empty()) || "Reuse" != m_uuidPolicy
                  );
    }

    ConfigurationElementContainer patcher = m_configuration->find("patcher");
    SLM_ASSERT("The <patcher> element can be set at most once.", patcher.size() <= 1 );
    if (patcher.size() == 1)
    {
        m_context = patcher.at(0)->getExistingAttributeValue("context");
        m_version = patcher.at(0)->getExistingAttributeValue("version");
        m_useAtomsPatcher = true;
    }

}

//-----------------------------------------------------------------------------


struct SetDumpPolicy
{
    SetDumpPolicy()
    {
        ::fwMemory::BufferManager::sptr manager = ::fwMemory::BufferManager::getDefault();
        m_lock = ::fwCore::mt::WriteLock( manager->getMutex() );
        if( manager )
        {
            ::fwMemory::IPolicy::sptr policy = manager->getDumpPolicy();
            if( ::fwMemory::policy::NeverDump::dynamicCast(policy) )
            {
                ::fwMemory::policy::BarrierDump::sptr newDumpPolicy = ::fwMemory::policy::BarrierDump::New();
                ::fwMemory::BufferManager::BufferStats stats = manager->getBufferStats().get();
                size_t aliveMemory = stats.totalManaged - stats.totalDumped;
                size_t freeMemory = ::fwMemory::tools::MemoryMonitorTools::estimateFreeMem() / 2;
                size_t barrier = std::max( aliveMemory, std::max( freeMemory, static_cast<size_t>(500L * 1024 * 1024) ) );

                newDumpPolicy->setBarrier( barrier );
                manager->setDumpPolicy( newDumpPolicy );
                m_oldPolicy = policy;
            }
        }
    }

    ~SetDumpPolicy()
    {
        try
        {
            ::fwMemory::BufferManager::sptr manager = ::fwMemory::BufferManager::getDefault();
            if( manager && m_oldPolicy )
            {
                manager->setDumpPolicy( m_oldPolicy );
                m_oldPolicy.reset();
            }
        }
        catch(...)
        {
            SLM_ASSERT("Failed to restore old policy", 0);
        }
    }

    ::fwMemory::IPolicy::sptr m_oldPolicy;
    ::fwCore::mt::WriteLock m_lock;
};

void SReader::updating() throw(::fwTools::Failed)
{

    if(this->hasLocationDefined())
    {
        SetDumpPolicy policy;

        ::fwData::Object::sptr data = this->getObject< ::fwData::Object >();

        ::fwGui::Cursor cursor;
        cursor.setCursor(::fwGui::ICursor::BUSY);

        try
        {
            const ::boost::filesystem::path& filePath = this->getFile();
            const ::boost::filesystem::path folderPath = filePath.parent_path();
            const ::boost::filesystem::path filename = filePath.filename();
            std::string extension = ::boost::filesystem::extension(filePath);

            FW_RAISE_IF( "Unable to guess file format (missing extension)", extension.empty() );

            if(m_customExts.find(extension) != m_customExts.end())
            {
                extension = "." + m_customExts[extension];
            }

            ::fwAtoms::Object::sptr atom;
            if ( extension == ".hdf5" )
            {
                ::fwAtomsHdf5IO::Reader reader;
                atom = ::fwAtoms::Object::dynamicCast( reader.read( filePath ) );
            }
            else
            {
                // Read atom
                ::fwZip::IReadArchive::sptr readArchive;
                ::boost::filesystem::path archiveRootName;
                if ( extension == ".json" )
                {
                    readArchive = ::fwZip::ReadDirArchive::New(folderPath.string());
                    archiveRootName = filename;
                }
                else if ( extension == ".jsonz" )
                {
                    readArchive = ::fwZip::ReadZipArchive::New(filePath.string());
                    archiveRootName = "root.json";
                }
                else if ( extension == ".xml" )
                {
                    readArchive = ::fwZip::ReadDirArchive::New(folderPath.string());
                    archiveRootName = filename;
                }
                else if ( extension == ".xmlz" )
                {
                    readArchive = ::fwZip::ReadZipArchive::New(filePath.string());
                    archiveRootName = "root.xml";
                }
                else
                {
                    FW_RAISE( "This file extension '" << extension << "' is not managed" );
                }

                ::fwAtomsBoostIO::Reader reader;
                atom = ::fwAtoms::Object::dynamicCast( reader.read( readArchive, archiveRootName ) );
            }

            FW_RAISE_IF( "Invalid atoms file :'" << filePath << "'", ! atom );

            /// patch atom
            if ( m_useAtomsPatcher )
            {
                FW_RAISE_IF( "Unable to load data, found '" << atom->getMetaInfo("context")
                             << "' context, but '" << m_context << "' was excepted.",
                             atom->getMetaInfo("context") != m_context);

                ::fwAtomsPatch::PatchingManager globalPatcher(atom);
                atom = globalPatcher.transformTo( m_version );
            }

            if(!m_filter.empty())
            {
                ::fwAtomsFilter::IFilter::sptr filter = ::fwAtomsFilter::factory::New(m_filter);
                OSLM_ASSERT("Failed to create IFilter implementation '" << m_filter << "'", filter);
                filter->apply(atom);
            }


            ::fwData::Object::sptr newData ;

            if("Strict" == m_uuidPolicy)
            {
                newData = ::fwAtomConversion::convert(atom, ::fwAtomConversion::AtomVisitor::StrictPolicy());
            }
            else if("Reuse" == m_uuidPolicy)
            {
                newData = ::fwAtomConversion::convert(atom, ::fwAtomConversion::AtomVisitor::ReusePolicy());
            }
            else
            {
                newData = ::fwAtomConversion::convert(atom, ::fwAtomConversion::AtomVisitor::ChangePolicy());
            }

            FW_RAISE_IF( "Unable to load '" << filePath << "' : invalid data.", ! newData );

            if(m_inject.empty())
            {
                FW_RAISE_IF( "Unable to load '" << filePath
                        << "' : trying to load a '" << newData->getClassname()
                        << "' where a '" << data->getClassname() << "' was expected",
                        newData->getClassname() != data->getClassname() );

                data->shallowCopy(newData);
            }
            else
            {
                ::fwData::Composite::sptr composite = ::fwData::Composite::dynamicCast(data);
                SLM_ASSERT("Inject mode works only on a Composite object", composite );

                ::fwComEd::helper::Composite helper(composite);
                helper.add(m_inject, newData);
                helper.notify(this->getSptr());
            }

            this->notificationOfUpdate();
        }
        catch( std::exception & e )
        {
            OSLM_ERROR( e.what() );
            ::fwGui::dialog::MessageDialog::showMessageDialog("Atoms reader failed", e.what(),
                    ::fwGui::dialog::MessageDialog::CRITICAL);
        }
        catch( ... )
        {
            ::fwGui::dialog::MessageDialog::showMessageDialog("Atoms reader failed", "Aborting operation.",
                    ::fwGui::dialog::MessageDialog::CRITICAL);
        }

        cursor.setDefaultCursor();

    }
}

//-----------------------------------------------------------------------------

::io::IOPathType SReader::getIOPathType() const
{
    return ::io::FILE;
}

//------------------------------------------------------------------------------

void SReader::notificationOfUpdate()
{
    ::fwData::Object::sptr object = this->getObject();
    ::fwServices::ObjectMsg::sptr msg = ::fwServices::ObjectMsg::New();
    msg->addEvent( ::fwServices::ObjectMsg::UPDATED_OBJECT , object );
    ::fwServices::IEditionService::notify( this->getSptr(),  object, msg );
}

//-----------------------------------------------------------------------------

void SReader::configureWithIHM()
{
    static ::boost::filesystem::path _sDefaultPath;

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle("Enter file name");
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    dialogFile.setType(::fwGui::dialog::ILocationDialog::SINGLE_FILE);
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::READ);
    dialogFile.setOption(::fwGui::dialog::LocationDialog::FILE_MUST_EXIST);

    dialogFile.addFilter("Medical data", "*" + ::boost::algorithm::join(m_allowedExts, " *"));

    BOOST_FOREACH(const std::string& ext, m_allowedExts)
    {
        dialogFile.addFilter(m_allowedExtLabels[ext], "*" + ext);
    }

    ::fwData::location::SingleFile::sptr result
        = ::fwData::location::SingleFile::dynamicCast( dialogFile.show() );

    if (result)
    {
        _sDefaultPath = result->getPath();
        this->setFile( _sDefaultPath );
        dialogFile.saveDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath.parent_path()) );
    }
    else
    {
        this->clearLocations();
    }
}

//-----------------------------------------------------------------------------

} // namespace ioAtoms

