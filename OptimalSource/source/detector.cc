#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)//the name of the detector ( our PMT in that case)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
	G4Track* track = aStep->GetTrack();

	track->SetTrackStatus(fStopAndKill); //the photon stop imidietely when enter the PMT

	G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint* postStepPoint = aStep->GetPostStepPoint();

	G4double PhotonEnergy = preStepPoint->GetKineticEnergy(); // MeV
	G4cout << "Photon energy : " << PhotonEnergy << " MeV" << G4endl;

	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->FillH1(0, PhotonEnergy *1000000); //convert to eV
	
	return true;
}
