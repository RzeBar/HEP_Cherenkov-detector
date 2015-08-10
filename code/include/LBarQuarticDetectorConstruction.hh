
#ifndef MYDETECTORCONSTRUCTION_HH
#define MYDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;

class LBarQuarticDetectorConstruction : public G4VUserDetectorConstruction {

public:
  
  // Constructor
  LBarQuarticDetectorConstruction();
  
  // Destructor
  virtual ~LBarQuarticDetectorConstruction();
  
  // Method
  virtual G4VPhysicalVolume* Construct();
  
private:

  // Helper methods
  void DefineMaterials();
  void SetupGeometry();
  void SetupPMT(G4LogicalVolume*);
  
  // World logical and physical volumes
  G4LogicalVolume*   fpWorldLogical;
  G4VPhysicalVolume* fpWorldPhysical;
  
};

#endif

