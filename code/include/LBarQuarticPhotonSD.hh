#ifndef PHOTONSD_HH
#define PHOTONSD_HH

#include "G4VSensitiveDetector.hh"
#include "LBarQuarticPhotonHit.hh"
#include "G4THitsCollection.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

class LBarQuarticPhotonSD : public G4VSensitiveDetector {

public:

  // Constructor
  LBarQuarticPhotonSD(const G4String& name);

  // Destructor
  virtual ~LBarQuarticPhotonSD();
  
  // Methods
  void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);  
  G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);
  void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
private:
  
  // Data members
  G4THitsCollection<LBarQuarticPhotonHit>* fPhotonCollection;
  G4int fPhotonCollectionID;

};




#endif
