#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4GenericMessenger.hh"


class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();
	~MyDetectorConstruction();

	virtual G4VPhysicalVolume* Construct();
private:
	G4Box* worldBox;
	G4Tubs* solidRadiator, * solidDetector;
	G4Cons* solidCone;
	G4LogicalVolume* logicDetector, * worldLog, * logicalRadiator, *logicCone, *logicDetector1;
	G4VPhysicalVolume* worldPhys, * physRadiator, * physDetector, * physCone;
	
	virtual void ConstructSDandField();
	void DefineMaterial();
	G4Material* Air, * SiO2, * StainlessSteel;

	G4bool Cone;
	G4double z_PMT;
	G4GenericMessenger* fMessenger;
};
#endif