// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h" 
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Geometry/Surface/interface/BoundSurface.h"


// ======= specific includes =======
#include "RecoTracker/TkDetLayers/interface/TOBLayerBuilder.h"
#include "RecoTracker/TkDetLayers/interface/GeometricSearchTracker.h"
#include "RecoTracker/TkDetLayers/interface/GeometricSearchTrackerBuilder.h"

using namespace edm;
using namespace std;


//
//
// class decleration
//

class myAnalyzer : public EDAnalyzer {
   public:
      explicit myAnalyzer( const ParameterSet& );
      ~myAnalyzer();


      virtual void analyze( const Event&, const EventSetup& );
   private:
      // ----------member data ---------------------------
};


//
myAnalyzer::myAnalyzer( const ParameterSet& iConfig )
{
   //now do what ever initialization is needed

}


myAnalyzer::~myAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
myAnalyzer::analyze( const Event& iEvent, const EventSetup& iSetup )
{
  std::cout << "Here I am" << std::endl;
  
  ESHandle<TrackerGeometry> pTrackerGeometry;
  iSetup.get<TrackerDigiGeometryRecord>().get( pTrackerGeometry ); 

  ESHandle<GeometricDet> pDD;
  iSetup.get<IdealGeometryRecord>().get( pDD );     
  cout << " Top node is  "<< &(*pDD) << endl;   
  cout << " And Contains  Daughters: "<< (*pDD).components().size() << endl;   


  // -------- here it constructs only a TOBLayer -------------------------
  vector<const GeometricDet*> geometricDetLayers = (*pDD).components();
  const GeometricDet* geometricDetTob=0;
  
  for(vector<const GeometricDet*>::const_iterator it=geometricDetLayers.begin();
      it!=geometricDetLayers.end(); it++){
    if(  (*it)->type() == GeometricDet::TOB ) {
      cout << "found TOB geometricDet!" << endl;
      geometricDetTob = (*it);
    }
  }
  
  cout << "Tob geometricDet has " << geometricDetTob->components().size() << " daughter" << endl;
  const GeometricDet* geometricDetTOBlayer = geometricDetTob->components()[1];

  cout << "this Tob layer has: " << geometricDetTOBlayer->components().size() << " daughter" << endl;

  /*
    vector<const GeometricDet*> geometricDetTOBlayer3Strings = geometricDetTOBlayer3->components();
    for(vector<const GeometricDet*>::const_iterator it=geometricDetTOBlayer3Strings.begin();
    it!=geometricDetTOBlayer3Strings.end(); it++){
    
    cout << "string phi position: " << (*it)->positionBounds().phi()  << endl;
    cout << "string r position:   " << (*it)->positionBounds().perp() << endl;
    cout << "string z position:   " << (*it)->positionBounds().z() << endl;
    cout << endl;
    }
  */
  
  TOBLayerBuilder myTOBBuilder;
  TOBLayer* testTOBLayer = myTOBBuilder.build(geometricDetTOBlayer,&(*pTrackerGeometry));

  cout << "testTOBLayer: " << testTOBLayer << endl;
  // ------------- END -------------------------



  
  // -------- here it constructs the whole GeometricSearchTracker --------------
  GeometricSearchTrackerBuilder myTrackerBuilder;
  GeometricSearchTracker* testTracker = myTrackerBuilder.build( &(*pDD),&(*pTrackerGeometry));
  cout << "testTracker: " << testTracker << endl;
  // ------------- END -------------------------
  

}

//define this as a plug-in
DEFINE_FWK_MODULE(myAnalyzer)
