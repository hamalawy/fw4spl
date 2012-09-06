/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWDATA_ACQUISITION_HPP_
#define _FWDATA_ACQUISITION_HPP_

#include <vector>
#include <list>

#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <fwCamp/Mapper/ValueMapper.hpp>

#include "fwData/config.hpp"
#include "fwData/Object.hpp"
#include "fwData/factory/new.hpp"
#include "fwData/Image.hpp"
#include "fwData/Reconstruction.hpp"

fwCampAutoDeclareDataMacro((fwData)(Acquisition), FWDATA_API);

namespace fwData
{

/**
 * @class   Acquisition
 * @brief   This class defines a medical firmware object.
 *
 * Acquisition represents fundamental object in the patient's Acquisition.
 * This class defines Acquisition's hospital, modality, acquisition zone...
 * An acquisition contains an image, a structAnat and reconstructions.
 *
 * @see     ::fwData::Image, ::fwData::Reconstruction
 * @author  IRCAD (Research and Development Team).
 * @date    2007-2009.
 */
class FWDATA_CLASS_API Acquisition : public Object
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (Acquisition)(::fwData::Object), (()), ::fwData::factory::New< Acquisition >) ;

    fwDataObjectMacro();

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWDATA_API Acquisition( ::fwData::Object::Key key );
    /// Destructor
    FWDATA_API virtual ~Acquisition();


    fwCampMakeFriendDataMacro((fwData)(Acquisition))

    /// Defines shallow copy
    FWDATA_API void shallowCopy( Acquisition::csptr _source );

    /// Defines deep copy
    FWDATA_API void deepCopy( Acquisition::csptr _source );

    /**
     * @brief Get/Set the image associated with the acquisition
     */
    fwDataGetSetSptrMacro(Image, ::fwData::Image::sptr);

    /**
     * @brief Get/Set the StructAnat associated with the acquisition
     */
    fwDataGetSetSptrMacro(StructAnat, ::fwData::Image::sptr);


    typedef std::vector< ::fwData::Reconstruction::sptr > ReconstructionContainerType;
    /**
     * @brief Get/Set Reconstructions associated with the acquisition
     */
    fwDataGetSetCRefMacro(Reconstructions, ReconstructionContainerType);

    /**
     * @brief Get the number of reconstructions
     */
    FWDATA_API ReconstructionContainerType::size_type getNumberOfReconstructions() const;

    /**
     * @brief add reconstruction
     */
    FWDATA_API void addReconstruction( ::fwData::Reconstruction::sptr _reconstruction );

    FWDATA_API void addDicomFileUrl(std::string dicomfileUrl);

    // Generator result---------------------------------------------------------

    fwGettersSettersDocMacro(BitsPerPixel, ui8BitsPerPixel, boost::uint8_t, the number of bits per pixel for all picture );

    fwGettersSettersDocMacro(SliceThickness, fSliceThickness, float,  slice thickness of each slices (eg : 1.0));

    fwGettersSettersDocMacro(Axe, ui8Axe, boost::uint8_t, acquisition axe (value : 0, 1 or 2));

    fwGettersSettersDocMacro(UnsignedFlag, bUnsignedFlag, bool, flag "true" if this acquisition use an unsigned value);

    fwGettersSettersDocMacro(AcquisitionIndex, ui32AcquisitionIndex, boost::uint32_t, index of this acquisition. (value : 0,1,2,3,4,5,6....));

    fwGettersSettersDocMacro(ImageType, sImageType, std::string, type of picture. (eg : ORIGINAL/PRIMARY/OTHER));

    fwGettersSettersDocMacro(ImageFormat, sImageFormat, std::string, picture format. (eg : INR, INR.GZ, INR.GZ.GPG) );

    fwGettersSettersDocMacro(CreationDate, ptCreationDate, boost::posix_time::ptime, acquisition date (eg 20010913 10 h 41 min 57 sec) );

    fwGettersSettersDocMacro(IsMain, bIsMain, bool, the flag indicating if this acquisition is the main one);

    fwGettersSettersDocMacro(IsNormalDir, bIsNormalDir, bool, the flag indicating if the reading sens of the inrimage is normal (true) or inverted (false));

    fwGettersSettersDocMacro(UID, sUID, std::string, serie UID);

    fwGettersSettersDocMacro(DbID, i32DbID, boost::int32_t, database indentifier);

    fwGettersSettersDocMacro(LaboID, ui32LaboID, boost::uint32_t, MEDICALAB ID);

    fwGettersSettersDocMacro(NetID, ui32NetID, boost::uint32_t, temporary identifier (used in radiology only));

    fwGettersSettersDocMacro(DateSendToLaboAt, ptDateSendToLaboAt, boost::posix_time::ptime, the date where this acquisition has been send to labo);

    fwGettersSettersDocMacro(DateReceiveFromLaboAt, ptDateReceiveFromLaboAt, boost::posix_time::ptime, the date where this acquisition has been received from labo);

    fwGettersSettersDocMacro(DateSendToBDDAt, ptDateSendToBDDAt, boost::posix_time::ptime, the date where this acquisition has been send to database);

    fwGettersSettersDocMacro(DateDisponibilityAt, ptDateDisponibilityAt, boost::posix_time::ptime, the date of reconstruction disponibility in the MEDICALAB);

    fwGettersSettersDocMacro(PatientSize, dPatientSize, double, the size of the patient when the acquisition has been done);

    fwGettersSettersDocMacro(PatientWeight, dPatientWeight, double, the weight of the patient when the acquisition has been done);

    fwGettersSettersDocMacro(Radiations, dRadiations, double, the number of radiations the patient took when the acquisition has been done);

    fwGettersSettersDocMacro(MedicalPrinter, sMedicalPrinter, std::string, the medicalPrinter used to make the acquisition);

    fwGettersSettersDocMacro(MedicalPrinterCorp, sMedicalPrinterCorp, std::string, the corporation of the MedicalPrinter used to make the acquisition);

    fwGettersSettersDocMacro(PatientPosition, sPatientPosition, std::string, the position of the patient when the acquisition has been done);

    fwGettersSettersDocMacro(DicomFileList, dicomFileList, std::list<std::string>, The list of Dicom file belonging to the acquisitoin);

    fwGettersSettersDocMacro(Description, description, std::string, The description of the acquisition );

    fwGettersSettersDocMacro(PathToFiles, pathToFiles, std::string, path to find Dicom files );

protected :

    //! Bits per pixel for all picture (frame). A this time, value is 8 or 16 (bits)
    ::boost::uint8_t  m_ui8BitsPerPixel;

    //! Thickness of each slices. (eg : 1.0)
    float m_fSliceThickness;

    //! Acquisition axe. (value : 0, 1 or 2)
    ::boost::uint8_t  m_ui8Axe;

    //! If this acquisition use an unsigned value.
    bool m_bUnsignedFlag;

    //! Index of this acquisition. (value : 0,1,2,3,4,5,6....)
    ::boost::uint32_t  m_ui32AcquisitionIndex;

    //! Type picture. eg : ORIGINAL\PRIMARY\OTHER
    std::string m_sImageType;

    //! picture format. eg : INR, INR.GZ, INR.GZ.GPG
    std::string m_sImageFormat;

    //! Acquisition date. eg 20010913 10 h 41 min 57 sec
    //boost::gregorian::date m_sDate;
    ::boost::posix_time::ptime m_ptCreationDate;

    //! Acquisition time. eg 104157 for 10 h 41 min 57 sec
    //boost::gregorian::date m_sTime;

    //! Flag indicating if this acquisition is the main one
    bool m_bIsMain;

    //! Flag indicating if the reading sens of the inrimage is normal (true) or inverted (false)
    bool m_bIsNormalDir;

    //! Serie UID
    std::string m_sUID;

    //! Database identifier
    ::boost::int32_t  m_i32DbID;

    //! MEDICALAB ID
    ::boost::uint32_t    m_ui32LaboID;

    //! temporary id (used in radiology only)
    ::boost::uint32_t  m_ui32NetID;

    //! Date where this acquisition has been send to labo
    ::boost::posix_time::ptime m_ptDateSendToLaboAt;

    //! Date where this acquisition has been received from labo
    ::boost::posix_time::ptime m_ptDateReceiveFromLaboAt;

    //! Date where this acquisition has been send to database
    ::boost::posix_time::ptime m_ptDateSendToBDDAt;

    //! Date of reconstruction disponibility in the MEDICALAB
    ::boost::posix_time::ptime m_ptDateDisponibilityAt;

    //! Size of the patient when the acquisition has been done
    double m_dPatientSize;

    //! Weight of the patient when the acquisition has been done
    double  m_dPatientWeight;

    //! Number of radiations the patient took when the acquisition has been done
    double  m_dRadiations;

    //! MedicalPrinter used to make the acquisition
    std::string m_sMedicalPrinter;

    //! Corporation of the MedicalPrinter used to make the acquisition
    std::string m_sMedicalPrinterCorp;

    //! Patient's position when the acquisition has been done
    std::string m_sPatientPosition;

    //! Description of the acquisition.
    std::string m_description;

    //! List of file associated with Acquisition.
    std::list<std::string> m_dicomFileList;

    //! path to find Dicom files
    std::string m_pathToFiles;


    ::fwData::Image::sptr m_attrImage;
    ::fwData::Image::sptr m_attrStructAnat;
    ReconstructionContainerType m_attrReconstructions;
};

} // namespace fwData

#endif // _FWDATA_ACQUISITION_HPP_
