#ifndef RUN_HH
#define RUN_HH

#include "G4Run.hh"
#include "G4THitsMap.hh"
#include "TNtuple.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

class G4Event;

class LBarQuarticRun : public G4Run {

public:

  // Constructor 
  LBarQuarticRun();

  // Destructor
  virtual ~LBarQuarticRun();

  //Method
  void RecordEvent(const G4Event*);

  //Data
  G4int fCollectionId;
  TFile   *fRootFile;
  TNtuple  *fNtuple;
  //Histograms
  TH1 *fLambda;
  TH1 *fGTime;
  TH1 *fGTimeVsEntries;
  TH1 *fTimeResolution;
  TH1 *fcopyEntries;
  TH1 *fNPhotons;
  //Multibars plots
  TH1 *fGTimeA;
  TH1 *fGTimeB;
  TH1 *fGTimeC;
  TH1 *fGTimeD;
};

#endif
