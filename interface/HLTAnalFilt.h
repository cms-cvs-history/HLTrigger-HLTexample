#ifndef HLTAnalFilt_h
#define HLTAnalFilt_h

/** \class HLTAnalFilt
 *
 *  
 *  This class is an EDAnalyzer implementing a very basic HLT filter
 *  product analysis
 *
 *  $Date: 2006/07/27 08:28:45 $
 *  $Revision: 1.1 $
 *
 *  \author Martin Grunewald
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

//
// class decleration
//

class HLTAnalFilt : public edm::EDAnalyzer {

   public:
      explicit HLTAnalFilt(const edm::ParameterSet&);
      ~HLTAnalFilt();

      virtual void analyze(const edm::Event&, const edm::EventSetup&);

   private:
      edm::InputTag inputTag_;  // input tag identifying product to analyze

};

#endif //HLTAnalFilt_h
