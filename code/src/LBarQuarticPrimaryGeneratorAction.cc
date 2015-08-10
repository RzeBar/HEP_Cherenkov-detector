
#include "LBarQuarticPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"

LBarQuarticPrimaryGeneratorAction::LBarQuarticPrimaryGeneratorAction()
{
//######################################//
// chose gun mode & angle
// variant:
// 1 - normal shot, along z
// 2 - anormal shot, with angle

	G4int variant = 1 ;

//set refractive index
//for Cherenkov angle

	G4double ref = 1.462 ;

//######################################//
  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);
  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="proton");

  particleGun->SetParticleDefinition(particle);

  particleGun->SetParticleEnergy(7*TeV);

//normal angle
if( variant == 1 ){
	particleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.*cm));
	particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
}

//Cherenkow angle
if( variant == 2 ){
	particleGun->SetParticlePosition(G4ThreeVector(0.,0.5*cm,0.));
	particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,-tan(acos(1./ref)),1.));
}
//for optical photon - for proton polarization is generating fails
//particleGun->  SetParticlePolarization(particleGun->GetParticleMomentumDirection().cross(G4ThreeVector(0.,1.,0.)));
}

LBarQuarticPrimaryGeneratorAction::~LBarQuarticPrimaryGeneratorAction()
{
  delete particleGun;
}

void LBarQuarticPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  particleGun->GeneratePrimaryVertex(anEvent);
}
