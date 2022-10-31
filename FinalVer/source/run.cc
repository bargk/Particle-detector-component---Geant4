#include "run.hh"

MyRunAction::MyRunAction()
{
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->CreateH1("Energy VS entries", "Cherenkov energy distribution", 100, 0., 5. * MeV);   //The units has to be the same like
	//man->CreateH1("Angular Distribution", "Angular Distribution  300-650 nm", 100, 0., 90. * rad); //Those from preStepPoint
	

	fMessenger = new G4GenericMessenger(this, "/filename/", "file name");
	fMessenger->DeclareProperty("name", filename, "the name of file name");
	filename = "output.root";
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{	
	

	G4AnalysisManager* man = G4AnalysisManager::Instance();
	
	/*G4int runID = run->GetRunID();
	std::stringstream strRunID;
	strRunID << runID;

	G4String fileName = "ra226_"+strRunID.str()+".root";*/
	man->OpenFile(filename);

	//man->CreateH1("Energy VS entries", "Cherenkov energy distribution", 100, 0., 5. * MeV);
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->Write();
	man->CloseFile();
}