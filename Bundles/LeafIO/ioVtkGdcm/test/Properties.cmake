
set( NAME ioVtkGdcmTest )
set( VERSION  )
set( TYPE TEST )
set( DEPENDENCIES fwCore fwTest fwTools fwData fwMedData fwDataCamp fwComEd fwServices fwRuntime)
set( REQUIREMENTS io ioVtkGdcm )

set(CPPUNITTEST_OPTIONS BUNDLE ioVtkGdcm WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
