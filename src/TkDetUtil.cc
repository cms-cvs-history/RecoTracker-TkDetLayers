#include "RecoTracker/TkDetLayers/src/interface/TkDetUtil.h"
#include "TrackingTools/DetLayers/interface/PhiLess.h"

#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "TrackingTools/PatternTools/interface/MeasurementEstimator.h"
#include "DataFormats/GeometrySurface/interface/BoundPlane.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"


namespace tkDetUtil {

  float computeWindowSize( const GeomDet* det, 
			   const TrajectoryStateOnSurface& tsos, 
			   const MeasurementEstimator& est)
  {
    const BoundPlane& startPlane = det->surface();  
    MeasurementEstimator::Local2DVector maxDistance = 
      est.maximalLocalDisplacement( tsos, startPlane);
    return calculatePhiWindow( maxDistance, tsos, startPlane);
  }



  float 
  calculatePhiWindow( const MeasurementEstimator::Local2DVector& maxDistance, 
		      const TrajectoryStateOnSurface& ts, 
		      const BoundPlane& plane) const
  {
    
    LocalPoint start = ts.localPosition();
    GlobalPoint corners[]  =  { plane.toGlobal(LocalPoint( start.x()+maxDistance.x(), start.y()+maxDistance.y() )),
				plane.toGlobal(LocalPoint( start.x()-maxDistance.x(), start.y()+maxDistance.y() )),
				plane.toGlobal(LocalPoint( start.x()-maxDistance.x(), start.y()-maxDistance.y() )),
				plane.toGlobal(LocalPoint( start.x()+maxDistance.x(), start.y()-maxDistance.y() )) 
    };
    
    float phimin = corners[0].phi();
    float phimax = phimin;
    for ( int i = 1; i<4; i++) {
      float cPhi = corners[i].phi();
      if ( PhiLess()( cPhi, phimin)) { phimin = cPhi; }
      if ( PhiLess()( phimax, cPhi)) { phimax = cPhi; }
    }
    float phiWindow = phimax - phimin;
    if ( phiWindow < 0.) { phiWindow +=  2.*Geom::pi();}
    
    return phiWindow;
  }




}