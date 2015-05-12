/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <utility>
#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <fwCore/base.hpp>

#include <fwData/String.hpp>

#include <fwServices/Base.hpp>

#include <fwComEd/helper/SeriesDB.hpp>

#include <fwMedData/SeriesDB.hpp>
#include <fwMedData/Series.hpp>
#include <fwMedData/Patient.hpp>
#include <fwMedData/Study.hpp>
#include <fwMedData/Equipment.hpp>

#include <fwTools/dateAndTime.hpp>

#include "uiMedDataQt/constants.hpp"
#include "uiMedDataQt/InsertSeries.hpp"
#include "uiMedDataQt/action/SInitNewSeries.hpp"


namespace uiMedData
{

namespace action
{

//------------------------------------------------------------------------------

fwServicesRegisterMacro(::fwGui::IActionSrv, ::uiMedData::action::SInitNewSeries, ::fwMedData::SeriesDB);

//------------------------------------------------------------------------------

SInitNewSeries::SInitNewSeries() : m_defaultInstitution("")
{
}

//------------------------------------------------------------------------------

SInitNewSeries::~SInitNewSeries() throw()
{
}

//------------------------------------------------------------------------------

void SInitNewSeries::info(std::ostream &_sstream )
{
    // Update message
    _sstream << std::string("SInitNewSeries");
}

//------------------------------------------------------------------------------

void SInitNewSeries::starting() throw(::fwTools::Failed)
{
    this->actionServiceStarting();
}

//------------------------------------------------------------------------------

void SInitNewSeries::receiving( ::fwServices::ObjectMsg::csptr msg ) throw(::fwTools::Failed)
{
}

//------------------------------------------------------------------------------

void SInitNewSeries::stopping() throw(::fwTools::Failed)
{
    this->actionServiceStopping();
}

//------------------------------------------------------------------------------

void SInitNewSeries::updating() throw(::fwTools::Failed)
{
    using namespace ::boost::posix_time;

    ::fwMedData::SeriesDB::sptr seriesDB = this->getObject< ::fwMedData::SeriesDB >();
    seriesDB->getContainer().clear();

    ::fwTools::Object::sptr obj = ::fwTools::fwID::getObject(m_seriesDBId);
    SLM_ASSERT("Didn't find object with UID '" + m_seriesDBId + "'", obj);
    ::fwMedData::SeriesDB::sptr srcSeriesDB = ::fwMedData::SeriesDB::dynamicCast(obj);

    ::fwComEd::helper::SeriesDB helper(seriesDB);

    BOOST_FOREACH(const ::fwMedData::Series::sptr& series, srcSeriesDB->getContainer())
    {
        helper.add(series);
    }

    typedef std::map< std::string, ::fwMedData::Patient::sptr> StudyToPatientType;
    typedef std::map< std::string, ::fwMedData::Study::sptr> StudyMapType;

    StudyToPatientType studyToPatient;
    StudyMapType studies;

    BOOST_FOREACH(const ::fwMedData::Series::sptr& series, seriesDB->getContainer())
    {
        const std::string& studyUID = series->getStudy()->getInstanceUID();

        studies[studyUID]        = series->getStudy();
        studyToPatient[studyUID] = series->getPatient();
    }

    ptime now = second_clock::local_time();

    const std::string date = ::fwTools::getDate(now);
    const std::string time = ::fwTools::getTime(now);

    BOOST_FOREACH(const StudyMapType::value_type& study, studies)
    {
        ::fwMedData::Series::sptr newSeries = ::uiMedData::InsertSeries::New();
        newSeries->setDescription(s_INSERT_NEW_SERIES_TEXT);
        newSeries->setDate(date);
        newSeries->setTime(time);
        newSeries->setStudy(study.second);
        newSeries->setPatient(studyToPatient[study.first]);

        helper.add(newSeries);
    }

    ::fwMedData::Study::sptr newStudy = ::fwMedData::Study::New();
    newStudy->setField(s_NEW_STUDY_FIELD_NAME, ::fwData::String::New("NEW_STUDY"));
    newStudy->setDate(date);
    newStudy->setTime(time);
    newStudy->setDescription(s_NEW_STUDY_TEXT);

    ::fwMedData::Patient::sptr newPatient = ::fwMedData::Patient::New();
    newPatient->setField(s_NEW_PATIENT_FIELD_NAME, ::fwData::String::New("NEW_PATIENT"));
    newPatient->setName(s_NEW_PATIENT_TEXT);
    newPatient->setBirthdate(date + " " + time);

    ::fwMedData::Equipment::sptr newEquipment = ::fwMedData::Equipment::New();
    newEquipment->setInstitutionName(m_defaultInstitution);

    ::fwMedData::Series::sptr newSeries = ::uiMedData::InsertSeries::New();
    newSeries->setDescription(s_INSERT_NEW_SERIES_TEXT);
    newSeries->setStudy(newStudy);
    newSeries->setPatient(newPatient);
    newSeries->setEquipment(newEquipment);
    newSeries->setDate(date);
    newSeries->setTime(time);

    helper.add(newSeries);
    helper.notify(this->getSptr());
}

//------------------------------------------------------------------------------

void SInitNewSeries::configuring() throw(::fwTools::Failed)
{
    this->::fwGui::IActionSrv::initialize();

    std::vector < ::fwRuntime::ConfigurationElement::sptr > seriesCfg = m_configuration->find("seriesDB");
    SLM_ASSERT("Missing tag 'seriesDB'", !seriesCfg.empty());

    m_seriesDBId = seriesCfg.front()->getValue();
    SLM_ASSERT("seriesDB must not be empty", !m_seriesDBId.empty());

    std::vector < ::fwRuntime::ConfigurationElement::sptr > institutionCfg = m_configuration->find("institution");
    if(!institutionCfg.empty())
    {
        m_defaultInstitution = institutionCfg.front()->getValue();
    }
}

//------------------------------------------------------------------------------

} // namespace action
}
