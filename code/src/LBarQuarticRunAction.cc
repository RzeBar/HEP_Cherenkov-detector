#include "LBarQuarticRunAction.hh"
#include "G4Run.hh"
#include "LBarQuarticRun.hh"
#include "globals.hh"
#include "TNtuple.h"
#include "TFile.h"


LBarQuarticRunAction::LBarQuarticRunAction() 
{}

LBarQuarticRunAction::~LBarQuarticRunAction() 
{}


void LBarQuarticRunAction::BeginOfRunAction(const G4Run* aRun)
{
	G4cout << "Log:   Begin of run." << " Expecting " << aRun->GetNumberOfEventToBeProcessed() << " events." << G4endl;
}

void LBarQuarticRunAction::EndOfRunAction(const G4Run* aRun)
{

 G4cout <<"Log:   Number of processed events:" << aRun->GetNumberOfEvent()   << " events. " <<G4endl;
 G4cout << "Log:   End of run" << G4endl;
}

G4Run* LBarQuarticRunAction::GenerateRun()
{
 return new LBarQuarticRun();
}

