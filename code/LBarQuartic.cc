#include "LBarQuarticDetectorConstruction.hh"
#include "LBarQuarticPhysicsList.hh"
#include "LBarQuarticPrimaryGeneratorAction.hh"
#include "LBarQuarticRunAction.hh"
#include "G4RunManager.hh"
#include "G4String.hh"
#include "G4UImanager.hh"


#ifdef G4UI_USE
  #include "G4UIExecutive.hh"
#endif

#ifdef G4VIS_USE
  #include "G4VisExecutive.hh"
#endif

int main(int argc,char** argv) {
 
  // Run manager
  G4RunManager * runManager = new G4RunManager;
 
  // Mandatory initialization classes
  runManager->SetUserInitialization(new LBarQuarticDetectorConstruction);
  runManager->SetUserInitialization(new LBarQuarticPhysicsList);
   
  // User action classes
  runManager->SetUserAction(new LBarQuarticPrimaryGeneratorAction());
  runManager->SetUserAction(new LBarQuarticRunAction());
  // Initialize G4 kernel
  runManager->Initialize();
   
  // Start a run

// Get the pointer to the User Interface manager
G4UImanager* UI = G4UImanager::GetUIpointer(); 

if ( argc == 1) {

#ifdef G4VIS_USE
  // Visualization manager
   G4VisManager* visManager = new G4VisExecutive;
   visManager->Initialize();
#endif

#ifdef G4UI_USE
    G4UIExecutive * ui = new G4UIExecutive(argc,argv);     
    ui->SessionStart();
    delete ui;
#endif

#ifdef G4VIS_USE
   delete visManager;
#endif
}
  else {

// Execute macro argv[1] from c++ code
    G4String macro = argv[1];
    UI->ApplyCommand("/control/execute "+macro);
  }
  // Job termination
  delete runManager;
 
  return 0;
}
















