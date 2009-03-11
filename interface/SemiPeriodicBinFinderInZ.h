#ifndef DetLayers_SemiPeriodicBinFinderInZ_H
#define DetLayers_SemiPeriodicBinFinderInZ_H

#include "Utilities/BinningTools/interface/BaseBinFinder.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include <cmath>
using namespace std;

/** Bin finder along the global Z for (almost) equidistant bins.
 *  The bins are computed from GeomDet positions.
 */

template <class T>
class SemiPeriodicBinFinderInZ : public BaseBinFinder<T> {
public:

  SemiPeriodicBinFinderInZ() : theNbins(0), theLocation(0), theZOffsets(0), theZStepBig(0), theZStepSmall(0), theZOffset(0) {}

  SemiPeriodicBinFinderInZ(std::vector<const GeomDet*>::const_iterator first,
		       std::vector<const GeomDet*>::const_iterator last) :
    theNbins( last-first) 
  {
    float zFirst = (**first).surface().position().z();
    theZStepSmall = ((**(first+1)).surface().position().z() - zFirst);
    float theZStepSmall2 = ((**(first+2)).surface().position().z() - zFirst);
    if(theZStepSmall < 1.) theZStepSmall = theZStepSmall2;
    // std::cout<<"SemiPeriodicBinFinderInZ: StepSmall="<<theZStepSmall<<" zFirst ="<<zFirst<<std::endl;
    
    for(std::vector<const GeomDet*>::const_iterator imod = first; imod!=last; imod++){
        theZOffsets.push_back((**imod).surface().position().z()-0.5*theZStepSmall);
       }
    theZStepBig = 4*theZStepSmall; //useless
    theZOffset = zFirst - 0.5*theZStepSmall; //useless
    // std::cout<<"SemiPeriodicBinFinderInZ: theNbins="<<theNbins<<" theLocation = "<<theLocation<<std::endl;
    // for(int g=0; g!=24; g++)
    // std::cout<<"SemiPeriodicBinFinderInZ: ZOffsets"<<theZOffsets[g]<<std::endl;
  }

  /// returns an index in the valid range for the bin that contains Z
  virtual int binIndex( T z) const {
    int k = 0;
    //int j = 0;
    while (z > theZOffsets[k+1])
    {
     //std::cout<<"SemiPeriodicBinFinderInZ: ZOffsets in binIndex="<<theZOffsets[k]<<std::endl;
     k++;
    }
    
    float dz = (z-theZOffsets[k]);
     //std::cout<<"SemiPeriodicBinFinderInZ: dz= "<<dz<<std::endl;
    /*
    if (dz <= 4*theZStepSmall )j = 4*(k-1) + int(dz/theZStepSmall);
    if (dz > theZStepSmall && dz <= 3*theZStepSmall ) j = 4*(k-1) + 3;
    if (dz > 3*theZStepSmall && dz <= 5*theZStepSmall ) j = 4*(k-1) + 4;
    */
    if (dz > 3*theZStepSmall && dz <= 5*theZStepSmall ) k++;
    
    
    //std::cout<<"SemiPeriodicBinFinderInZ: z="<<z<<" index = "<<j<<std::endl;
    return binIndex( k );
  }

  /// returns an index in the valid range
  virtual int binIndex( int i) const {
    return std::min( std::max( i, 0), theNbins-1);
  }
   
  /// the middle of the bin 
  virtual T binPosition( int ind) const {
      //std::cout<<"SemiPeriodicBinFinderInZ: zbin="<<theZOffsets[int(ind/4)] + theZStepSmall*(ind % 4)<<" index ="<<ind<<std::endl;
      //std::cout<<"SemiPeriodicBinFinderInZ: zbin="<<theZOffsets[ind] + 0.5*theZStepSmall<<" index ="<<ind<<std::endl;
    return theZOffsets[ind] + 0.5*theZStepSmall;
  }

private:

  int theNbins;
  int theLocation;
  vector<double> theZOffsets;
  T theZStepBig;
  T theZStepSmall;
  T theZOffset;

};
#endif
