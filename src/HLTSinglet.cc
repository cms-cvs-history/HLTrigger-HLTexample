/** \class HLTSinglet
 *
 * See header file for documentation
 *
 *  $Date: 2006/06/28 01:41:22 $
 *  $Revision: 1.2 $
 *
 *  \author Martin Grunewald
 *
 */

#include "HLTrigger/HLTexample/interface/HLTSinglet.h"

#include "FWCore/Framework/interface/Handle.h"

#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/HLTReco/interface/HLTFilterObject.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

//
// constructors and destructor
//
template<typename T>
HLTSinglet<T>::HLTSinglet(const edm::ParameterSet& iConfig)
{
  inputTag_ = iConfig.getParameter< edm::InputTag >("inputTag");
  Min_Pt_   = iConfig.getParameter<double>("MinPt");
  Max_Eta_  = iConfig.getParameter<double>("MaxEta");
  Min_N_    = iConfig.getParameter<int>("MinN");

   LogDebug("") << "Input/ptcut/etacut/ncut : " << inputTag_.encode() << " " << Min_Pt_ << " " << Max_Eta_ << " " << Min_N_ ;

   //register your products
   produces<reco::HLTFilterObjectWithRefs>();
}

template<typename T>
HLTSinglet<T>::~HLTSinglet()
{
}

//
// member functions
//

// ------------ method called to produce the data  ------------
template<typename T> 
bool
HLTSinglet<T>::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace std;
   using namespace edm;
   using namespace reco;

   typedef vector<T> TCollection;
   typedef Ref<TCollection> TRef;

   // All HLT filters must create and fill an HLT filter object,
   // recording any reconstructed physics objects satisfying (or not)
   // this HLT filter, and place it in the Event.

   // The filter object
   auto_ptr<HLTFilterObjectWithRefs>
     filterproduct (new HLTFilterObjectWithRefs(path(),module()));
   // Ref to Candidate object to be recorded in filter object
   RefToBase<Candidate> ref;


   // get hold of jets
   Handle<TCollection> objects;
   iEvent.getByLabel (inputTag_,objects);

   // look at all objects, check cuts and add to filter object
   int n(0);
   typename TCollection::const_iterator i ( objects->begin() );
   for (; i!=objects->end(); i++) {
     if ( (i->pt() >= Min_pt_) && (abs(i->eta()) <= Max_eta_) ) {
       n++;
       ref=RefToBase<Candidate>(TRef(objects,distance(i->begin(),i)));
       filterproduct->putParticle(ref);
     }
   }

   // filter decision
   bool accept(n>=Min_N_);

   // put filter object into the Event
   iEvent.put(filterproduct);

   return accept;
}
