#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TSystem.h>

#include <iostream>
#include <string>

// needed for timing studies
#include "DataFormats/HLTReco/interface/ModuleTiming.h"
// needed for trigger studies
#include "DataFormats/Common/interface/TriggerResults.h"
// needed for event-id info
#include "DataFormats/Common/interface/EventID.h"
//
#include "FWCore/FWLite/src/AutoLibraryLoader.h"

#include "HLTrigger/HLTexample/test/AnalyzeTriggerResults.h"
#include "HLTrigger/HLTexample/test/AnalyzeTriggerTiming.h"

using std::cout; using std::endl; using std::string;


// Christos Leonidopoulos, July 2006


/* usage: analyzeHLT <ROOT file> <analyze timing info> <analyze triggers>
   where, 
   <ROOT file>: file with HLT information to be analyzed (default: hlt.root)
   <analyze timing info>: 1 to produce timing plots (default: 1)
   <analyze triggers>: 1 to get trigger rates, ovelaps among paths, etc (default: 1)
*/
int main(int argc, char ** argv)
{
  // load libraries
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();

  // default arguments
  string filename = "hlt.root";
  bool doTiming = true;
  bool doTriggers = true;
  // get parameters from command line
  if(argc >= 2) filename = argv[1];
  if(argc >= 3) doTiming = bool(atoi(argv[2]));
  if(argc >= 4) doTriggers = bool(atoi(argv[3]));

  // open file
  TFile file(filename.c_str());
  if(file.IsZombie()) 
    {
      cout << " *** Error opening file " << filename << endl;
      exit(-1);
    }
  TTree * events = dynamic_cast<TTree *>(file.Get("Events") );
  assert(events);

  TBranch * TBevtTime = 0;
  TBranch * TBtrigRes = 0;
  //  TBranch * TBevtid = events->GetBranch("id_");
  
  // structure holding the timing info
  edm::EventTime evtTime;
  // structure holding the trigger decision
  edm::TriggerResults trigRes;

#if 0
  // structure holding event information
  edm::EventID evtid;

  TBevtid->SetAddress((void *)&evtid);
#endif

  AnalyzeTriggerResults * tr = 0;
  AnalyzeTriggerTiming * tt = 0;

  if(doTiming)
    {
      TBevtTime = events->GetBranch("edmEventTime_myTimer__PRODHLT.obj");
      assert(TBevtTime);
      TBevtTime->SetAddress((void*)&evtTime);
      tt = new AnalyzeTriggerTiming(100, 0, 1000);
    }

  if(doTriggers)
    {
      TBtrigRes = events->GetBranch("edmTriggerResults_TriggerResults__PRODHLT.obj");
      assert(TBtrigRes);
      TBtrigRes->SetAddress((void*)&trigRes);
      tr = new AnalyzeTriggerResults();
    }

  int n_evts = events->GetEntries();
  
  for(int i = 0; i != n_evts; ++i)
    {

#if 0 
      TBevtid->GetEntry(i);
      cout << " Run # = " << evtid.run() 
	   << " event # = " << evtid.event() 
	   << " entry # = " << i << "\n";
#endif

      if(doTiming)
	{
	  TBevtTime->GetEntry(i);
	  tt->analyze(evtTime);
	}

      if(doTriggers)
	{
	  TBtrigRes->GetEntry(i);
	  tr->analyze(trigRes);
	}

    } // loop over all events


  // get results, do cleanup

  if(doTiming)
    {
      tt->getResults();
      delete tt;
    }
  if(doTriggers)
    {
      tr->getResults();
      delete tr;
    }
  
  return 0;
}
