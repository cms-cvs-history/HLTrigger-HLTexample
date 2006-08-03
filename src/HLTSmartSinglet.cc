/** \class HLTSmartSinglet
 *
 * See header file for documentation
 *
 *  $Date: 2006/07/27 08:09:07 $
 *  $Revision: 1.3 $
 *
 *  \author Martin Grunewald
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "HLTrigger/HLTexample/interface/HLTSmartSinglet.h"

#include "FWCore/Framework/interface/Handle.h"

#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/HLTReco/interface/HLTFilterObject.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

//
// constructors and destructor
//
template<typename T>
HLTSmartSinglet<T>::HLTSmartSinglet(const edm::ParameterSet& iConfig) :
  select_(iConfig)
{
  inputTag_ = iConfig.template getParameter< edm::InputTag >("inputTag");
  cut_      = iConfig.template getParameter< std::string>("cut");
  Min_N_    = iConfig.template getParameter<int>("MinN");

   LogDebug("") << "Input/cut/ncut : " << inputTag_.encode() << " " << cut_<< " " << Min_N_ ;

   //register your products
   produces<reco::HLTFilterObjectWithRefs>();
}

template<typename T>
HLTSmartSinglet<T>::~HLTSmartSinglet()
{
}

//
// member functions
//

// ------------ method called to produce the data  ------------
template<typename T> 
bool
HLTSmartSinglet<T>::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
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


   // get hold of collection of objects
   Handle<TCollection> objects;
   iEvent.getByLabel (inputTag_,objects);

   // look at all objects, check cuts and add to filter object
   int n(0);
   typename TCollection::const_iterator i ( objects->begin() );
   for (; i!=objects->end(); i++) {
     if (select_(*i)) {
       n++;
       ref=RefToBase<Candidate>(TRef(objects,distance(objects->begin(),i)));
       filterproduct->putParticle(ref);
     }
   }

   // filter decision
   bool accept(n>=Min_N_);

   // put filter object into the Event
   iEvent.put(filterproduct);

   return accept;
}
