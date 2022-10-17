#include "run.hh"

MyRunAction::MyRunAction()
{
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->CreateH1("Energy VS entries", "Cherenkov energy distribution", 100, 0., 5. * MeV);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	
	G4int runID = run->GetRunID();
	std::stringstream strRunID;
	strRunID << runID;

	G4String fileName = "output_"+strRunID.str()+".root";
	man->OpenFile(fileName);

}

void MyRunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->Write();
	man->CloseFile();
}