/** \class HLTAnalFiltNDM
 *
 * See header file for documentation
 *
 *  $Date: 2007/12/04 19:57:42 $
 *  $Revision: 1.1 $
 *
 *  \author Martin Grunewald
 *
 */

#include "HLTrigger/HLTexample/interface/HLTAnalFiltNDM.h"

#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include<typeinfo>

//
// constructors and destructor
//
 
HLTAnalFiltNDM::HLTAnalFiltNDM(const edm::ParameterSet& iConfig) :
  inputTag_(iConfig.getParameter<edm::InputTag>("inputTag"))
{
  LogDebug("") << "Input: " << inputTag_.encode();
}

HLTAnalFiltNDM::~HLTAnalFiltNDM()
{
}

//
// member functions
//

// ------------ method called to produce the data  ------------
void
HLTAnalFiltNDM::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace std;
   using namespace edm;
   using namespace reco;
   using namespace trigger;

   // get hold of (single?) TriggerResults object
   vector<Handle<TriggerResults> > trhv;
   iEvent.getManyByType(trhv);
   const unsigned int n(trhv.size());
   LogDebug("") << "Number of TriggerResults objects found: " << n;
   for (unsigned int i=0; i!=n; i++) {
     LogDebug("") << "TriggerResult object " << i << " bits: " << *(trhv[i]);
   }

   // get hold of requested filter object
   Handle<TriggerFilterObjectWithRefs> ref;
   try {iEvent.getByLabel(inputTag_,ref);} catch(...) {;}
   if (ref.isValid()) {
     LogDebug("") << inputTag_.encode() + " Size = g/e/m/j/C/M/H "
		  << ref->photonIds().size() << " "
		  << ref->electronIds().size() << " "
		  << ref->muonIds().size() << " "
		  << ref->jetIds().size() << " " 
		  << ref->compositeIds().size() << " " 
		  << ref->metIds().size() << " " 
		  << ref->htIds().size();
     const unsigned int n(ref->electronIds().size());
     for (unsigned int i=0; i!=n; i++) {
       // some Xchecks
       Particle particle=*(ref->electronRefs().at(i));
       LogTrace("") << i << " E: "
		    << particle.energy() << " " 
		    << ref->electronRefs().at(i)->energy();
     }
     //
   } else {
     LogDebug("") << "Filterobject " + inputTag_.encode() + " not found!";
   }

   return;
}
