#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
class G4Run;

class LBarQuarticRunAction : public G4UserRunAction {

public:

  // Constructor
  LBarQuarticRunAction();

  // Destructor
  virtual ~LBarQuarticRunAction();
    
  // Method
   void BeginOfRunAction(const G4Run* );
   void EndOfRunAction(const G4Run* );
   G4Run* GenerateRun();

};

#endif

