// Markus Borris, 2014
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef PTR2REF_H_sjhdxfggberz38f2g36ygfn123f9v
#define PTR2REF_H_sjhdxfggberz38f2g36ygfn123f9v

#include <assert.h>


namespace too
{
    namespace mem
    {
        //! Crahes if \param pT is nullptr.
        template <typename T>
        inline T& Ptr2Ref(T* pT)
        {
            assert(pT);
            return *pT;
        }

        //! Crahes if \param pT is nullptr.
        template <typename T>
        inline const T& Ptr2RefC(const T* pT)
        {
            assert(pT);
            return *pT;
        }
    }
}

#endif
