#include "detector.hh"
#include "G4SystemOfUnits.hh"
#include <math.h>

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)//the name of the detector
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
	G4Track* track = aStep->GetTrack();

	track->SetTrackStatus(fStopAndKill); //the photon stop imidietely when enter the detector

	G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint* postStepPoint = aStep->GetPostStepPoint();

	G4double PhotonEnergy = preStepPoint->GetKineticEnergy(); // MeV
	G4cout << "Photon energy : " << PhotonEnergy << " MeV" << G4endl;

	//Angular distribution

	//G4double photonPosZ = preStepPoint->GetPosition().z() -3.5*cm; // r_z
	//G4double photonPosX = preStepPoint->GetPosition().x(); // r_x
	//G4double photonPosY = preStepPoint->GetPosition().y(); // r_y
	//G4double photonPosMag = ((photonPosZ * photonPosZ) + (photonPosX * photonPosX) + (photonPosY * photonPosY)); // |r|
	//G4double Theta_rad = acos(sqrt(photonPosZ* photonPosZ) / photonPosMag); //radians
	//G4double Theta = Theta_rad * 180 / 3.14159265358979323846;
	//G4cout << "Photon angle : " << Theta << " " << G4endl;

	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->FillH1(0, PhotonEnergy *1000000); //convert to eV
	//man->FillH1(0, Theta); 
	
	return true;
}
