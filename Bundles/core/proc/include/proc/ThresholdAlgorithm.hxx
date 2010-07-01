/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Initial authors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */
#ifndef PROC_THRESHOLDALGORITHM_HXX_
#define PROC_THRESHOLDALGORITHM_HXX_

namespace proc
{

/**
 * @brief Thresholding algorithm core
 *
 * The operator() implements the thresholding operation
 * @note Invoked by Threshold
 */
struct ThresholdAlgorithm
{
	/// Parameters
    struct Parameter
    {
        float in,out,low,high;
        ::boost::shared_ptr< ::fwData::Image >  imageIn;
        ::boost::shared_ptr< ::fwData::Image >  imageOut;
    };

    template<class PIXELTYPE>
    void operator()(Parameter &param)
    {
        assert( param.imageIn->getSize().size()==3 );
        param.imageOut->deepCopy(param.imageIn); //inclut copie buffer (par rapport à shallowCopy)

        PIXELTYPE *buffer1 = (PIXELTYPE *)param.imageIn->getBuffer();
        PIXELTYPE *buffer2 = (PIXELTYPE *)param.imageOut->getBuffer();
        const unsigned int NbPixels = param.imageIn->getSize()[0] * param.imageIn->getSize()[1] * param.imageIn->getSize()[2];
        const PIXELTYPE low = ( PIXELTYPE )param.low;
        const PIXELTYPE high = ( PIXELTYPE )param.high;
        unsigned int i;
        for( i=0 ; i<NbPixels ; ++i , ++buffer1, ++buffer2 )
        {
            if ( low < *buffer1  && *buffer1 < high )
            {
            	*buffer2 = ( PIXELTYPE )param.in;
            }
            else
            {
            	*buffer2 =  ( PIXELTYPE )param.out;
            }
        }
    }
};

} //namespace proc

#endif /* PROC_THRESHOLDALGORITHM_HXX_ */
