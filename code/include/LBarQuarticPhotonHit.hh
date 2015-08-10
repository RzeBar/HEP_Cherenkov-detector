#ifndef PHOTONHIT_HH
#define PHOTONHIT_HH

#include "G4VHit.hh"
#include "G4ThreeVector.hh"

class LBarQuarticPhotonHit : public G4VHit {

public:

  // Constructor
  LBarQuarticPhotonHit();

  // Destructor
  virtual ~LBarQuarticPhotonHit();

  // Methods 
  // void Draw()
  // void Print()

  // Photon energy
  inline void SetEnergy(G4double energy) {fEnergy = energy;}
  inline G4double GetEnergy() const {return fEnergy;}

  // Photon position
  inline void SetPosition(G4ThreeVector position) {fPosition = position;}
  inline G4ThreeVector GetPosition() const {return fPosition;}

  // Photon Global Time (Time since the event in which the track belongs is created.)
  inline void SetGlobalTime(G4double globaltime) {fGlobalTime = globaltime;}
  inline G4double GetGlobalTime() const {return fGlobalTime;}

  // Photon Local Time (Time since the track is created.)
  inline void SetLocalTime(G4double localtime) {fLocalTime = localtime;}
  inline G4double GetLocalTime() const {return fLocalTime;}

  // CopyID
  inline void SetCopyID(G4double CopyID) {fCopyID = CopyID;}
  inline G4double GetCopyID() const {return fCopyID;}

private:
  //Data Members
	G4double fEnergy;
	G4ThreeVector fPosition;
	G4double fGlobalTime;
	G4double fLocalTime;
	G4int fCopyID;
};

#endif

