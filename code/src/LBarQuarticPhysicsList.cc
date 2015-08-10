#include "LBarQuarticPhysicsList.hh"
#include "G4BaryonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4ComptonScattering.hh"
#include "G4Decay.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eIonisation.hh"
#include "G4eplusAnnihilation.hh"
#include "G4GammaConversion.hh"
#include "G4hIonisation.hh"
#include "G4IonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuIonisation.hh"
#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#include "G4MuPairProduction.hh"
#include "G4ParticleTypes.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4ProcessManager.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4OpticalPhoton.hh"
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4TransitionRadiation.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpWLS.hh"
#include "HadronPhysicsQGSP_BERT.hh"


LBarQuarticPhysicsList::LBarQuarticPhysicsList()
  :G4VUserPhysicsList()
{
  defaultCutValue = 1.0*mm;
  SetVerboseLevel(1);
}

LBarQuarticPhysicsList::~LBarQuarticPhysicsList() {}

void LBarQuarticPhysicsList::ConstructParticle()
{
  // Construct all the particles we may possibly need
//  G4OpticalPhoton::Definition();
  G4OpticalPhoton::OpticalPhotonDefinition();
  //  Baryons
  G4BaryonConstructor baryons;
  baryons.ConstructParticle();

  // Bosons (including geantinos)
  G4BosonConstructor bosons;
  bosons.ConstructParticle();

  // Ions
  G4IonConstructor ions;
  ions.ConstructParticle();

  // Leptons
  G4LeptonConstructor leptons;
  leptons.ConstructParticle();

  // Construct all mesons
  G4MesonConstructor mesons;
  mesons.ConstructParticle();

  // Resonances and quarks
  G4ShortLivedConstructor shortLiveds;
  shortLiveds.ConstructParticle();
}

void LBarQuarticPhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructEM();
  ConstructGeneral();
  G4VPhysicsConstructor* proc=new HadronPhysicsQGSP_BERT("hadron",true);
  proc->ConstructProcess();
}

void LBarQuarticPhysicsList::ConstructEM()
{
  theParticleIterator->reset();
  
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma") {
      // Gamma
      pmanager->AddDiscreteProcess(new G4GammaConversion());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());      
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());
    } else if (particleName == "e-") {
      // Electron
      pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4eIonisation,         -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung,     -1, 3, 3);

    } else if (particleName == "e+") {
      // Positron
      pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4eIonisation,         -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung,     -1, 3, 3);
      pmanager->AddProcess(new G4eplusAnnihilation,   0,-1, 4);

    } else if( particleName == "mu+" ||
               particleName == "mu-"    ) {
      // Muon
      pmanager->AddProcess(new G4MuMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4MuIonisation,         -1, 2, 2);
      pmanager->AddProcess(new G4MuBremsstrahlung,     -1, 3, 3);
      pmanager->AddProcess(new G4MuPairProduction,     -1, 4, 4);
/////////////////////////////////////////////////////////////////////////////////////////////////
    } else if(particleName == "opticalphoton") {
// Optical photons
pmanager->AddDiscreteProcess(new G4OpAbsorption());
pmanager->AddDiscreteProcess(new G4OpRayleigh());
pmanager->AddDiscreteProcess(new G4OpBoundaryProcess());
pmanager->AddDiscreteProcess(new G4OpWLS());
/////////////////////////////////////////////////////////
    } else if ((!particle->IsShortLived()) &&
               (particle->GetPDGCharge() != 0.0) &&
               (particle->GetParticleName() != "chargedgeantino")) {
      // All others charged particles except geantino
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
    }
  }
}

void LBarQuarticPhysicsList::ConstructGeneral()
{
  // Add Decay Process
	G4Decay* theDecayProcess = new G4Decay();


	//LBAR- QUARTIC
	G4Cerenkov* theCerenkovProcess = new G4Cerenkov();
	G4Scintillation* theScintillationProcess = new G4Scintillation(); //do usuniecia
//	G4TransitionRadiation* theTRProcess = new G4TransitionRadiation();

//theCerenkovProcess->SetTrackSecondariesFirst(true);
// theCerenkovProcess->SetMaxNumPhotonsPerStep(1);


  theParticleIterator->reset();

  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    if (theDecayProcess->IsApplicable(*particle)) { 
      pmanager ->AddProcess(theDecayProcess);
      
      // Set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }    
////////////////____LBAR - QUARTIC _____////////////////////////

    if (theCerenkovProcess->IsApplicable(*particle)) { 
      pmanager ->AddProcess(theCerenkovProcess);
      pmanager ->SetProcessOrdering(theCerenkovProcess, idxPostStep);
    }

    if (theScintillationProcess->IsApplicable(*particle)) {
    pmanager->AddProcess(theScintillationProcess);
    pmanager->SetProcessOrdering(theScintillationProcess, idxPostStep);
    }


 //   if (theTRProcess->IsApplicable(*particle)) { 
 //    pmanager ->AddProcess(theTRProcess);
  //  pmanager ->SetProcessOrdering(theTRProcess, idxPostStep);
 // }
//////////////////////////////////////////////////////////////////////////
  }
}

void LBarQuarticPhysicsList::SetCuts()
{
  if (verboseLevel >0){
    G4cout << "LBarQuarticPhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }
  
  // Set cut values for gamma first since some e+/e- processes
  // need gamma cut values
  SetCutValue(defaultCutValue, "gamma");
  SetCutValue(defaultCutValue, "e-");
  SetCutValue(defaultCutValue, "e+");
  
  if (verboseLevel>0) DumpCutValuesTable();
}
