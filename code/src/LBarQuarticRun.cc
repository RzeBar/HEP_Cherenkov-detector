#include "LBarQuarticRun.hh"
#include "LBarQuarticPhotonHit.hh"

#include "G4Event.hh"
#include "globals.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"
#include <iomanip>
#include "G4SDManager.hh"
#include "G4THitsMap.hh"
#include "G4THitsCollection.hh"

#include <algorithm>    // std::sort
#include <vector>       // std::vector



LBarQuarticRun::LBarQuarticRun()
{
	G4SDManager* manager = G4SDManager::GetSDMpointer();


	fCollectionId = manager->GetCollectionID("Photon/PhotonCollection");
	G4cout << "Log:  LBarQuarticRun constructor: index of photon collection : " << fCollectionId << G4endl;
	 //..Output root file
	fRootFile = new TFile("tree.root","RECREATE");
	//..NTuple definitions
	fNtuple =  new TNtuple("ntuple","photon collection","e:x:y:z:gt:lt");
	//Histograms
	fLambda = new TH1D("Wavelength","Wavelength vs Entries",100,200.,800.);
	fGTime = new TH1D("Global Time","photon track timing",100,0.,12000.);
	fGTimeVsEntries = new TH1D("GTime vs nPhotons","global time [ps] vs number of photoelectrons [#Sigma Entries]",100,0,12000);
	fTimeResolution = new TH1D("Time Resolution","time for x photons detection",20000,0.,20000.); // name ?
	fNPhotons = new TH1D("NPhotons vs Entries","Number of photons in one event",1000,0,10e6);
	//Multibars plots
	fcopyEntries = new TH1C("Entries in copy number","Entries in copy number",10,0,10);
	fGTimeA = new TH1D("Global Time - A","photon track timing A",100,0.,12000.);
	fGTimeB = new TH1D("Global Time - B","photon track timing B",100,0.,12000.);
	fGTimeC = new TH1D("Global Time - C","photon track timing C",100,0.,12000.);
	fGTimeD = new TH1D("Global Time - D","photon track timing D",100,0.,12000.);
}

LBarQuarticRun::~LBarQuarticRun()
{
	//.. write tree and close the file
	fRootFile->Write();
	fRootFile->Close();
}

void LBarQuarticRun::RecordEvent(const G4Event* evt)
{
  numberOfEvent++;

  G4HCofThisEvent* hce = evt->GetHCofThisEvent();

  G4THitsCollection<LBarQuarticPhotonHit>* photonCollection =
      dynamic_cast<G4THitsCollection<LBarQuarticPhotonHit>*> (hce->GetHC(fCollectionId));
	
G4cout << "Log: (event "<< numberOfEvent <<"): photons = "<< photonCollection->entries() << G4endl;

std::vector<double> GlobalTimeCollection;// time resolution

//######################################//
//set number of photons - time resolution

	unsigned res = 100; 

//######################################//

fNPhotons->Fill(photonCollection->entries());

  for( G4int i = 0; i< photonCollection->entries(); i++) {
	float e = (*photonCollection)[i]->GetEnergy()/eV;
	float x = (*photonCollection)[i]->GetPosition().x()/cm;
	float y = (*photonCollection)[i]->GetPosition().y()/cm;
	float z = (*photonCollection)[i]->GetPosition().z()/cm;
	float gt = (*photonCollection)[i]->GetGlobalTime()/picosecond; // unit [ps] is not working :)
	float lt = (*photonCollection)[i]->GetLocalTime()/picosecond;
	int id = (*photonCollection)[i]->GetCopyID();

	fNtuple->Fill(e,x,y,z,gt,lt);

	double lamb = 663*3./1.6/e; // unit [nm]
	fLambda->Fill(lamb);

	fcopyEntries->Fill(id);

	fGTime->Fill(gt);
	if(id==0){   fGTimeA->Fill(gt);   }
	if(id==1){   fGTimeB->Fill(gt);   }
	if(id==2){   fGTimeC->Fill(gt);   }
	if(id==3){   fGTimeD->Fill(gt);   }





	GlobalTimeCollection.push_back(gt); //time resolution
     }
	if( GlobalTimeCollection.size()>=res ){
		std::sort(GlobalTimeCollection.begin(),GlobalTimeCollection.end());
		fTimeResolution->Fill(GlobalTimeCollection[res-1]);
	}

		fGTimeVsEntries->Reset();
		int total = 0;
 		for (int i=0;i<100;i++){
			total += fGTime->GetBinContent(i);
     			for (int k=0;k<total;k++){
				fGTimeVsEntries->AddBinContent(i);
			}
		}
}

