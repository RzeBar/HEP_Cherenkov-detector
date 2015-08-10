#include "LBarQuarticDetectorConstruction.hh"
#include "LBarQuarticPhotonSD.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"

#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

#include "G4SDManager.hh"

/*###############################################################*/
// WARIANT:
// 	0 - FRESH IDEA
// 	2 - Wlot z kwarcu, pion z powietrza + Al
//
// MIRROR:
//	1 - on  
// 	0 - off
// 
// ABSORPTION (FOR BETTER VIS)
//	1 - on 
// 	0 - off
//
// ISOLATION BETWEEN QBARS (IF NBARS >1)
//	1 - on  
// 	0 - off

			G4int variant=0;
			G4int mirror=1;
			G4int visAbsorb=0;
			G4int isolation=0; 

/*###############################################################*/
///////////////////////////////////////////////////////////////////


LBarQuarticDetectorConstruction::LBarQuarticDetectorConstruction()
  :fpWorldLogical(0)
  ,fpWorldPhysical(0)
{}

LBarQuarticDetectorConstruction::~LBarQuarticDetectorConstruction() {}

G4VPhysicalVolume* LBarQuarticDetectorConstruction::Construct()
{
  // Material Definition
  DefineMaterials();  

  // Geometry Definition
  SetupGeometry();   

  // Return world volume
  return fpWorldPhysical;  
}

void LBarQuarticDetectorConstruction::DefineMaterials()
{
  G4String symbol;             
  G4double density;     
  G4int ncomponents;
  G4double fractionmass;
    
	G4NistManager* man = G4NistManager::Instance();
	man->SetVerbose(1);
	G4Material* air  = man->FindOrBuildMaterial("G4_AIR");
	G4Material* SiO2 = man->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
//	G4Material* Al = man->FindOrBuildMaterial("G4_Al");

  // Define fusedquartz
	G4Material* fusedquartz = new G4Material("Fused Quartz", density= 2.203*g/cm3, ncomponents=1);
	fusedquartz->AddMaterial(SiO2, fractionmass=1.);

  // Define vacuum
	G4Material* vacuum = new G4Material("Vacuum", density= 1.e-5*g/cm3, ncomponents=1, kStateGas, STP_Temperature, 2.e-2*bar);
	vacuum->AddMaterial(air, fractionmass=1.);
  
  // Dump material information
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

void LBarQuarticDetectorConstruction::SetupGeometry()
{
	G4NistManager* man = G4NistManager::Instance();
	man->SetVerbose(1);
	G4Material* Al = man->FindOrBuildMaterial("G4_Al");
	G4Material* air = G4Material::GetMaterial("G4_AIR");
  
  // World volume
  G4Box* worldSolid = new G4Box("World_Solid",           // Name
				2.0*m, 2.0*m, 2.0*m);    // Half lengths
  
  fpWorldLogical = new G4LogicalVolume(worldSolid,	 // Solid
				       air,	         // Material
				       "World_Logical"); // Name
  
  fpWorldPhysical = new G4PVPlacement(0,	         // Rotation matrix pointer
				      G4ThreeVector(),   // Translation vector
				      fpWorldLogical,	 // Logical volume
				      "World_Physical",	 // Name
				      0,		 // Mother volume
				      false,		 // Unused boolean parameter
				      0);		 // Copy number
  ////////////////////////////////////////////////////////////////////////
  // LBar solid items
	G4Material* fusedquartz = G4Material::GetMaterial("Fused Quartz");


/*###############################################################*/
///////////////////////////////////////////////////////////////////

//                     PARAMETRISATION                          //

///////////////////////////////////////////////////////////////////

  G4double HorzXDim = 80 ;
  G4double HorzYDim = 3 ;
  G4double VertXDim = 3 ;
  G4double VertYDim = 80 ;
  G4double ZDim = 6 ;
  G4double RadLength = 0.1 ;
  G4double VerticalOffset = 2 ;

  G4double DetectorXDim = 1 ;
  G4double MirrorThickness = 0.01 ; //mm
  G4double nBars = 4 ;


  G4double ChAngle = 48. ; //degrees

  G4RotationMatrix* ChRot = new G4RotationMatrix(0., pi/180.*ChAngle, 0.);
  G4RotationMatrix* ElbCutRot = new G4RotationMatrix(-pi/4., 0., 0.);

  G4RotationMatrix* ChRotFrame = new G4RotationMatrix(0.,pi/2.-pi/180.*ChAngle, 0.);

				//_______________ ISOLATORS CONSTRUCTION ________________//

  G4double IsoDim=0 ;
if(isolation!=0){
  IsoDim = 0.2 ; //mm - isolation thickness, change this if isolation!=0
}

/*###############################################################*/
///////////////////////////////////////////////////////////////////


// Mother volume - frame
  G4Box* frameSolid = new G4Box("Frame_Solid",           // Name
				100.*mm, 100.*mm, 100.*mm);    // Half lengths
  
  G4LogicalVolume* fpFrameLogical = new G4LogicalVolume(frameSolid,	 // Solid
				       air,	         // Material
				       "Frame_Logical"); // Name
  


  G4PVPlacement* fpFramePhysical = new G4PVPlacement(ChRotFrame,	 // Rotation matrix POINTER !!!!
				       G4ThreeVector(0,VertYDim/2.*sin(pi/180.*ChAngle)*mm-ZDim/2.*cos(pi/180.*ChAngle)*mm-VerticalOffset*mm,VertYDim/2.*cos(pi/2.-pi/180.*ChAngle)*mm+VertXDim/2.*sin(pi/180.*ChAngle)*mm),  // Translation vector
				       fpFrameLogical,	 // Logical volume
				       "Frame_Physical", // Name
				       fpWorldLogical,	 // Mother volume
				       false,		 // Unused boolean parameter
				       0);		 // Copy number


	G4VSolid* HorizontalBarSolid = new G4Box("HorizontalBar_Solid", HorzXDim/2.*mm, HorzYDim/2.*mm, ZDim/2.*mm);
	
	G4VSolid* VerticalBarSolid = new G4Box("VerticalBar_Solid", VertXDim/2.*mm, VertYDim/2.*mm, ZDim/2.*mm);



 //FusedQuartz properties


 const G4int nEntries = 11;

  G4double PhotonEnergy[nEntries] =           {4.959368*eV,4.132807*eV,3.542406*eV,3.099605*eV,2.755204*eV,2.479684*eV,2.254258*eV,2.066403*eV,1.907449*eV,1.771203*eV,1.653123*eV};

  G4double RefractiveIndex1[nEntries] =
		{1.51,1.488,1.475,1.47,1.465,1.462,1.46,1.458,1.456,1.455,1.45};

  G4double Absorption1[nEntries] =
		{95.*cm,104.*cm,111.*cm,120.*cm,122.*cm,125.*cm,128.*cm,130.*cm,130.*cm,130.*cm,130.*cm};

  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();

  myMPT1->AddProperty("RINDEX",       PhotonEnergy, RefractiveIndex1,nEntries)
        ->SetSpline(true);
  myMPT1->AddProperty("ABSLENGTH",    PhotonEnergy, Absorption1,     nEntries)
        ->SetSpline(true);

  myMPT1->AddConstProperty("RESOLUTIONSCALE",1.0);
  myMPT1->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
  myMPT1->AddConstProperty("SLOWTIMECONSTANT",10.*ns);
  myMPT1->AddConstProperty("YIELDRATIO",0.8);

  fusedquartz->SetMaterialPropertiesTable(myMPT1);




// Air properties (MPT)



  G4double RefractiveIndex2[nEntries] =
		{ 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};



  G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
  myMPT2->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex2, nEntries);

if(visAbsorb!=0){
  	G4double Absorption2[nEntries] = {0.001*mm,0.001*mm,0.001*mm,0.001*mm,0.001*mm,0.001*mm,0.001*mm,0.001*mm,0.001*mm,0.001*mm,0.001*mm}; 
	myMPT2->AddProperty("ABSLENGTH",   PhotonEnergy, Absorption2,     nEntries);
}

  air->SetMaterialPropertiesTable(myMPT2);
  myMPT1->DumpTable();

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@




if(variant==0){

				//_______________ ITEMS CONSTRUCTION _____________//


	G4LogicalVolume* HorizontalBarLogical = new G4LogicalVolume(HorizontalBarSolid, fusedquartz, "HBar_Logical");
	new G4PVPlacement(0, G4ThreeVector(HorzXDim/2.*mm+VertXDim/2.*mm,VertYDim/2.*mm-HorzYDim/2.*mm,0.), HorizontalBarLogical, "HBar_Physical", fpFrameLogical, false, 0);

//ALONG Z MULTIPLICITY - ADDITIONAL (NOT BASE)
	new G4PVPlacement(0, G4ThreeVector(HorzXDim/2.*mm+VertXDim/2.*mm,VertYDim/2.*mm-HorzYDim/2.*mm,(ZDim+IsoDim)*1*mm), HorizontalBarLogical, "HBar_Physical", fpFrameLogical, false, 0);
	new G4PVPlacement(0, G4ThreeVector(HorzXDim/2.*mm+VertXDim/2.*mm,VertYDim/2.*mm-HorzYDim/2.*mm,(ZDim+IsoDim)*2*mm), HorizontalBarLogical, "HBar_Physical", fpFrameLogical, false, 0);
	new G4PVPlacement(0, G4ThreeVector(HorzXDim/2.*mm+VertXDim/2.*mm,VertYDim/2.*mm-HorzYDim/2.*mm,(ZDim+IsoDim)*3*mm), HorizontalBarLogical, "HBar_Physical", fpFrameLogical, false, 0);
//END

 //additional solids ---------->

	G4VSolid* ChCutSolid = new G4Box("ChCut_Solid", VertXDim*mm, nBars*nBars*ZDim*sin(pi/180.*ChAngle)*mm, nBars*ZDim*cos(pi/180.*ChAngle)*mm);
	G4VSolid* ElbowCutSolid = new G4Box("ElbCut_Solid", VertXDim*sin(pi/4.)*mm, VertYDim*mm, ZDim*mm);

	G4SubtractionSolid* PartVerticalBarSolid = new G4SubtractionSolid("FVBar_Logical",
                             VerticalBarSolid,
                             ElbowCutSolid,
                             ElbCutRot,
                             G4ThreeVector(-VertXDim/2.*mm,VertYDim/2.*mm,0) ) ;


	G4SubtractionSolid* FinalVerticalBarSolid = new G4SubtractionSolid("FVBar_Logical",
                             PartVerticalBarSolid,
                             ChCutSolid,
                             ChRot,
                             G4ThreeVector(0.,-VertYDim/2.*mm,(nBars-0.5)*ZDim*mm) ) ;


//ALONG Z MULTIPLICITY - ADDITIONAL (NOT BASE)
//A - 1
	G4SubtractionSolid* AFinalVerticalBarSolid = new G4SubtractionSolid("FVBar_Logical",
                             PartVerticalBarSolid,
                             ChCutSolid,
                             ChRot,
                             G4ThreeVector(0.,-VertYDim/2.*mm,(nBars-0.5-1)*ZDim*mm-1*IsoDim*mm) ) ;

	G4LogicalVolume* AFinalVerticalBarLogical = new G4LogicalVolume(AFinalVerticalBarSolid, fusedquartz, "FVBar_Logical");
	new G4PVPlacement(0, G4ThreeVector(0,0,(ZDim+IsoDim)*1*mm), AFinalVerticalBarLogical, "HBar_Physical", fpFrameLogical, false, 0);
//B - 2

	G4SubtractionSolid* BFinalVerticalBarSolid = new G4SubtractionSolid("FVBar_Logical",
                             PartVerticalBarSolid,
                             ChCutSolid,
                             ChRot,
                             G4ThreeVector(0.,-VertYDim/2.*mm,(nBars-0.5-2)*ZDim*mm-2*IsoDim*mm) ) ;

	G4LogicalVolume* BFinalVerticalBarLogical = new G4LogicalVolume(BFinalVerticalBarSolid, fusedquartz, "FVBar_Logical");
	new G4PVPlacement(0, G4ThreeVector(0,0,(ZDim+IsoDim)*2*mm), BFinalVerticalBarLogical, "HBar_Physical", fpFrameLogical, false, 0);

//C - 3
	G4SubtractionSolid* CFinalVerticalBarSolid = new G4SubtractionSolid("FVBar_Logical",
                             PartVerticalBarSolid,
                             ChCutSolid,
                             ChRot,
                             G4ThreeVector(0.,-VertYDim/2.*mm,(nBars-0.5-3)*ZDim*mm-3*IsoDim*mm) ) ;

	G4LogicalVolume* CFinalVerticalBarLogical = new G4LogicalVolume(CFinalVerticalBarSolid, fusedquartz, "FVBar_Logical");
	new G4PVPlacement(0, G4ThreeVector(0,0,(ZDim+IsoDim)*3*mm), CFinalVerticalBarLogical, "HBar_Physical", fpFrameLogical, false, 0);

//END



 //<------------------------

// IF NEED RADIATOR
/*
	G4SubtractionSolid* RadiatorVerticalBarSolid = new G4SubtractionSolid("RVBar_Logical",
                             FinalVerticalBarSolid,
                             VerticalBarSolid,
                             0,
                             G4ThreeVector(0,RadLength*mm,0) ) ;

	G4SubtractionSolid* AirGuideVerticalBarSolid = new G4SubtractionSolid("AVBar_Logical",
                             FinalVerticalBarSolid,
                             RadiatorVerticalBarSolid,
                             0,
                             G4ThreeVector() ) ;

	G4LogicalVolume* RadiatorVerticalBarLogical = new G4LogicalVolume(RadiatorVerticalBarSolid, fusedquartz, "RVBar_Logical");
	new G4PVPlacement(0, G4ThreeVector(), RadiatorVerticalBarLogical, "HBar_Physical", fpFrameLogical, false, 0);	

	G4LogicalVolume* AirGuideVerticalBarLogical = new G4LogicalVolume(AirGuideVerticalBarSolid, fusedquartz, "FVBar_Logical");
	new G4PVPlacement(0, G4ThreeVector(), AirGuideVerticalBarLogical, "HBar_Physical", fpFrameLogical, false, 0);	
*/


//VERTICAL PLACEMENT BASE
	G4LogicalVolume* FinalVerticalBarLogical = new G4LogicalVolume(FinalVerticalBarSolid, fusedquartz, "FVBar_Logical");
	new G4PVPlacement(0, G4ThreeVector(), FinalVerticalBarLogical, "HBar_Physical", fpFrameLogical, false, 0);
	


				//_______________ SENSITIVE DETECTOR _______________//


	G4VSolid* ScorerSolid = new G4Box("Scorer_Solid", DetectorXDim/2.*mm, HorzYDim/2.*mm, ZDim/2.*mm);
	G4LogicalVolume* ScorerLogical = new G4LogicalVolume(ScorerSolid, fusedquartz , "Scorer_Logical");
	new G4PVPlacement(0, G4ThreeVector(HorzXDim*mm+VertXDim/2.*mm+DetectorXDim/2.*mm,VertYDim/2.*mm-HorzYDim/2.*mm,0.), ScorerLogical,"Scorer_Physical",fpFrameLogical,true, 0);//  A (base)

//ALONG Z MULTIPLICITY - ADDITIONAL (NOT BASE)
	new G4PVPlacement(0, G4ThreeVector(HorzXDim*mm+VertXDim/2.*mm+DetectorXDim/2.*mm,VertYDim/2.*mm-HorzYDim/2.*mm,(ZDim+IsoDim)*1*mm), ScorerLogical,"Scorer_Physical",fpFrameLogical,true, 1);// B (copy 1)
	new G4PVPlacement(0, G4ThreeVector(HorzXDim*mm+VertXDim/2.*mm+DetectorXDim/2.*mm,VertYDim/2.*mm-HorzYDim/2.*mm,(ZDim+IsoDim)*2*mm), ScorerLogical,"Scorer_Physical",fpFrameLogical,true, 2);// C (copy 2)
	new G4PVPlacement(0, G4ThreeVector(HorzXDim*mm+VertXDim/2.*mm+DetectorXDim/2.*mm,VertYDim/2.*mm-HorzYDim/2.*mm,(ZDim+IsoDim)*3*mm), ScorerLogical,"Scorer_Physical",fpFrameLogical,true, 3);// D (copy 3)
//END
	SetupPMT(ScorerLogical); 

	// absorber //
	G4LogicalVolume* AbsLogical = new G4LogicalVolume(ScorerSolid, Al, "Abs_Logical");
	new G4PVPlacement(0, G4ThreeVector(VertXDim/2.*mm+HorzXDim*mm+DetectorXDim*mm,VertYDim/2.*mm-HorzYDim/2.*mm,0.), AbsLogical,"Abs_Physical",fpFrameLogical,true, 0);

//ALONG Z MULTIPLICITY - ADDITIONAL (NOT BASE)
	new G4PVPlacement(0, G4ThreeVector(VertXDim/2.*mm+HorzXDim*mm+DetectorXDim*mm,VertYDim/2.*mm-HorzYDim/2.*mm,(ZDim+IsoDim)*1*mm), AbsLogical,"Abs_Physical",fpFrameLogical,true, 0);
	new G4PVPlacement(0, G4ThreeVector(VertXDim/2.*mm+HorzXDim*mm+DetectorXDim*mm,VertYDim/2.*mm-HorzYDim/2.*mm,(ZDim+IsoDim)*2*mm), AbsLogical,"Abs_Physical",fpFrameLogical,true, 0);
	new G4PVPlacement(0, G4ThreeVector(VertXDim/2.*mm+HorzXDim*mm+DetectorXDim*mm,VertYDim/2.*mm-HorzYDim/2.*mm,(ZDim+IsoDim)*3*mm), AbsLogical,"Abs_Physical",fpFrameLogical,true, 0);
//END
				//_______________ MIRROR CONSTRUCTION ________________//



if(mirror!=0){

	G4VSolid* MirrorSolid = new G4Box("Mirror_Solid", MirrorThickness/2.*mm, VertXDim/2./sin(pi/4.)*mm, ZDim/2.*mm);
	G4LogicalVolume* MirrorLogical = new G4LogicalVolume(MirrorSolid, Al, "Mirror_Logical");

	G4OpticalSurface* OpMirror  = new G4OpticalSurface("Mirror",
                                                             glisur,
                                                             ground,
                                                             dielectric_metal,
                                                             1.);

	G4double Reflectivity[nEntries] =
		{ 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9}; 

	G4double Eff[nEntries] =
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	G4MaterialPropertiesTable *OpMirrorProperty = new G4MaterialPropertiesTable();
	OpMirrorProperty -> AddProperty("REFLECTIVITY",PhotonEnergy,Reflectivity,nEntries);
	OpMirrorProperty -> AddProperty("EFFICIENCY",PhotonEnergy,Eff,nEntries);
	OpMirror -> SetMaterialPropertiesTable(OpMirrorProperty);

	new G4PVPlacement(ElbCutRot,
			  G4ThreeVector(-MirrorThickness/2.*sin(pi/4.)*mm,VertYDim/2.*mm-VertXDim/2.*mm+MirrorThickness/2.*sin(pi/4.)*mm,0.),
			  MirrorLogical,
			  "Mirror_Physical",
			  fpFrameLogical,
			  false,0);

//ALONG Z MULTIPLICITY - ADDITIONAL (NOT BASE)

	new G4PVPlacement(ElbCutRot,
			  G4ThreeVector(-MirrorThickness/2.*sin(pi/4.)*mm,VertYDim/2.*mm-VertXDim/2.*mm+MirrorThickness/2.*sin(pi/4.)*mm,(ZDim+IsoDim)*1*mm),
			  MirrorLogical,
			  "Mirror_Physical",
			  fpFrameLogical,
			  false,0);

	new G4PVPlacement(ElbCutRot,
			  G4ThreeVector(-MirrorThickness/2.*sin(pi/4.)*mm,VertYDim/2.*mm-VertXDim/2.*mm+MirrorThickness/2.*sin(pi/4.)*mm,(ZDim+IsoDim)*2*mm),
			  MirrorLogical,
			  "Mirror_Physical",
			  fpFrameLogical,
			  false,0);

	new G4PVPlacement(ElbCutRot,
			  G4ThreeVector(-MirrorThickness/2.*sin(pi/4.)*mm,VertYDim/2.*mm-VertXDim/2.*mm+MirrorThickness/2.*sin(pi/4.)*mm,(ZDim+IsoDim)*3*mm),
			  MirrorLogical,
			  "Mirror_Physical",
			  fpFrameLogical,
			  false,0);

//END
	new G4LogicalSkinSurface("MirrorSurface",MirrorLogical,OpMirror);


	G4VisAttributes* MirrorVisAttributes = new G4VisAttributes(G4Colour(0.0,1.0,0.0,0.2));
	MirrorVisAttributes->SetForceSolid(true);

	MirrorLogical->SetVisAttributes(MirrorVisAttributes);
}



}




//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


				//######################################################
				//# 	WARIANT 1 : oba ramiona z kwarcu 	       #
				//######################################################


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

if(variant==1){
				//_______________ ITEMS CONSTRUCTION _____________//

	G4UnionSolid* BarSolid = new G4UnionSolid("Bar_Solid",
						  HorizontalBarSolid,
						  VerticalBarSolid,
						  0,
						  G4ThreeVector(0.,46.5*mm,(90.-46.5)*mm));

	G4LogicalVolume* BarLogical = new G4LogicalVolume(BarSolid, fusedquartz, "Bar_Logical");
	new G4PVPlacement(0, G4ThreeVector(0.,0.,45.*mm), BarLogical, "Bar_Physical", fpFrameLogical, false, 0);





				//_______________ SENSITIVE DETECTOR _______________//

	G4VSolid* ScorerSolid = new G4Box("Scorer_Solid", 1.5*mm, 0.005*mm, 1.5*mm);
	G4LogicalVolume* ScorerLogical = new G4LogicalVolume(ScorerSolid, fusedquartz , "Scorer_Logical");
	new G4PVPlacement(0, G4ThreeVector(0.,(1.5+90.+0.005)*mm,(90.-1.5)*mm), ScorerLogical,"Scorer_Physical",fpFrameLogical,true, 0);

	SetupPMT(ScorerLogical); 

	// absorber //
	G4LogicalVolume* AbsLogical = new G4LogicalVolume(ScorerSolid, Al, "Abs_Logical");
	new G4PVPlacement(0, G4ThreeVector(0.,(1.5+90.+0.015)*mm,(90.-1.5)*mm), AbsLogical,"Abs_Physical",fpFrameLogical,true, 0);



				//_______________ MIRROR CONSTRUCTION ________________//



if(mirror!=0){

	G4RotationMatrix* Rot = new G4RotationMatrix(0., -pi/4., 0.);
	G4VSolid* MirrorSolid = new G4Box("Mirror_Solid", 1.5*mm, 0.013*mm, (sqrt(2)*1.5)*mm);
	G4LogicalVolume* MirrorLogical = new G4LogicalVolume(MirrorSolid, Al, "Mirror_Logical");

	G4OpticalSurface* OpMirror  = new G4OpticalSurface("Mirror",
                                                             glisur,
                                                             ground,
                                                             dielectric_metal,
                                                             1.);

	G4double Reflectivity[nEntries] =
		{ 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.}; 

	G4double Eff[nEntries] =
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	G4MaterialPropertiesTable *OpMirrorProperty = new G4MaterialPropertiesTable();
	OpMirrorProperty -> AddProperty("REFLECTIVITY",PhotonEnergy,Reflectivity,nEntries);
	OpMirrorProperty -> AddProperty("EFFICIENCY",PhotonEnergy,Eff,nEntries);
	OpMirror -> SetMaterialPropertiesTable(OpMirrorProperty);

	new G4PVPlacement(Rot,
			  G4ThreeVector(0.,0.,(-1.5+45.)*mm),
			  MirrorLogical,
			  "Mirror_Physical",
			  BarLogical,
			  false,0);

	new G4LogicalSkinSurface("MirrorSurface",MirrorLogical,OpMirror);
}


				//_______________ VIS OPTIONS __________________//


	G4VisAttributes* BarAttributes = new G4VisAttributes(G4Colour(0.0,1.0,0.0,0.2));
	BarAttributes->SetForceSolid(true);

	BarLogical->SetVisAttributes(BarAttributes);


}



////////////////////////////////////////////////////////////////////////////

  // Invisible world volume.
  fpWorldLogical->SetVisAttributes(G4VisAttributes::Invisible);
  fpFrameLogical->SetVisAttributes(G4VisAttributes::Invisible);

//////////////////////////////////////////////////////////////////
}


void LBarQuarticDetectorConstruction::SetupPMT(G4LogicalVolume* PMTVolume)
{  

  // Get pointer to detector manager
  G4SDManager* manager = G4SDManager::GetSDMpointer();  

// Create a new sensitive detector named "Photon"
  LBarQuarticPhotonSD* photonSD =  new LBarQuarticPhotonSD("Photon");
  manager->AddNewDetector(photonSD);
  PMTVolume->SetSensitiveDetector(photonSD);

}
















