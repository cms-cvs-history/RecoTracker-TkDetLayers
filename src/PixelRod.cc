#include "RecoTracker/TkDetLayers/interface/PixelRod.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TrackingTools/PatternTools/interface/MeasurementEstimator.h"
#include "TrackingTools/DetLayers/interface/DetLayerException.h"


using namespace std;

typedef GeometricSearchDet::DetWithState DetWithState;

PixelRod::PixelRod(vector<const GeomDet*>& theInputDets):
  DetRodOneR(theInputDets.begin(),theInputDets.end())
{
  theBinFinder = BinFinderType(theDets.begin(),theDets.end());

  //--------- DEBUG INFO --------------
  LogDebug("TkDetLayers") << "==== DEBUG PixelRod ====="; 
  for (vector<const GeomDet*>::const_iterator i=theDets.begin();
       i != theDets.end(); i++){
    LogDebug("TkDetLayers") << "PixelRod's Det pos z,perp,eta,phi: " 
			    << (**i).position().z() << " , " 
			    << (**i).position().perp() << " , " 
			    << (**i).position().eta() << " , " 
			    << (**i).position().phi() ;
  }
  LogDebug("TkDetLayers") << "==== end DEBUG PixelRod ====="; 
  //--------------------------------------


}

PixelRod::~PixelRod(){

} 

const vector<const GeometricSearchDet*>& 
PixelRod::components() const{
  throw DetLayerException("PixelRod doesn't have GeometricSearchDet components");
}
 
 
pair<bool, TrajectoryStateOnSurface>
PixelRod::compatible( const TrajectoryStateOnSurface& ts, const Propagator&, 
		      const MeasurementEstimator&) const{
  edm::LogError("TkDetLayers") << "temporary dummy implementation of PixelRod::compatible()!!" ;
  return pair<bool,TrajectoryStateOnSurface>();
}


vector<DetWithState> 
PixelRod::compatibleDets( const TrajectoryStateOnSurface& startingState,
			  const Propagator& prop, 
			  const MeasurementEstimator& est) const
{  
  typedef MeasurementEstimator::Local2DVector Local2DVector;
  TrajectoryStateOnSurface ts = prop.propagate( startingState, specificSurface());
  if (!ts.isValid()) return vector<DetWithState>();  

  GlobalPoint startPos = ts.globalPosition();

  vector<DetWithState> result;

  int closest = theBinFinder.binIndex(startPos.z());
  pair<bool,TrajectoryStateOnSurface> closestCompat = 
    theCompatibilityChecker.isCompatible(theDets[closest],startingState, prop, est);
  if ( closestCompat.first) {
    result.push_back( DetWithState( theDets[closest], closestCompat.second));
  }

  const BoundPlane& closestPlane( theDets[closest]->specificSurface() );


  Local2DVector maxDistance = 
    est.maximalLocalDisplacement( closestCompat.second, closestPlane);
  
  float detHalfLen = theDets[closest]->surface().bounds().length()/2.;
  
  // explore neighbours
  for (size_t idet=closest+1; idet < theDets.size(); idet++) {
    LocalPoint nextPos( theDets[idet]->surface().toLocal( closestCompat.second.globalPosition()));
    if (fabs(nextPos.y()) < detHalfLen + maxDistance.y()) {
      if ( !add(idet, result, startingState, prop, est)) break;
    } else {
      break;
    }
  }

  for (int idet=closest-1; idet >= 0; idet--) {
    LocalPoint nextPos( theDets[idet]->surface().toLocal( closestCompat.second.globalPosition()));
    if (fabs(nextPos.y()) < detHalfLen + maxDistance.y()) {
      if ( !add(idet, result, startingState, prop, est)) break;
    } else {
      break;
    }
  }

  return result;
}


vector<DetGroup> 
PixelRod::groupedCompatibleDets( const TrajectoryStateOnSurface& startingState,
				 const Propagator& prop,
				 const MeasurementEstimator& est) const
{
  LogDebug("TkDetLayers") << "dummy implementation of PixelRod::groupedCompatibleDets()" ;
  return vector<DetGroup>();
}



