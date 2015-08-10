#ifndef MYPHYSICSLIST_HH
#define MYPHYSICSLIST_HH

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class LBarQuarticPhysicsList: public G4VUserPhysicsList {

public:

  // Constructor
  LBarQuarticPhysicsList();

  // Destructor
  virtual ~LBarQuarticPhysicsList();
  
protected:

  // Construct particles and physics processes
  void ConstructParticle();
  void ConstructProcess();
  
  void SetCuts();
  
private:
  
  // Helper methods
  void ConstructGeneral();
  void ConstructEM();

};

#endif



