#include "construction.hh"
#include "G4OpticalSurface.hh"
#include "G4SDParticleWithEnergyFilter.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4PhysicsVector.hh"
#include "G4LogicalBorderSurface.hh"

#include "detector.hh"

MyDetectorConstruction::MyDetectorConstruction()
{

}

MyDetectorConstruction::~MyDetectorConstruction()
{

}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
	// materials
	G4NistManager* nist = G4NistManager::Instance();
	G4Material* Air = nist->FindOrBuildMaterial("G4_AIR");

	G4Material* SiO2 = new G4Material("Quartz", 2.201 * g / cm3, 2); //quartz material
	SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
	SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

	G4int num = 8;

	G4double energy[num] = { 1.23984198 * eV / 0.65, 1.23984198 * eV / 0.6, 1.23984198 * eV / 0.55, 1.23984198 * eV / 0.5,
							1.23984198 * eV / 0.45, 1.23984198 * eV / 0.4, 1.23984198 * eV / 0.35, 1.23984198 * eV / 0.3 };

	G4double rindexSiO2[num] = {1.5, 1.5, 1.5, 1.5,  1.5, 1.5, 1.5, 1.5 };
	G4double rindexAir[num] = {1.0,1.0, 1.0, 1.0,  1.0, 1.0,1.0, 1.0 };
	G4double rindexSteel[num] = { 2.0, 1.8, 1.7, 1.5, 1.4, 1.3, 1.25, 1.2 };

	G4MaterialPropertiesTable* mptSiO2 = new G4MaterialPropertiesTable();
	mptSiO2->AddProperty("RINDEX", energy, rindexSiO2, num);
	SiO2->SetMaterialPropertiesTable(mptSiO2);

	G4MaterialPropertiesTable* mptAir = new G4MaterialPropertiesTable();
	mptAir->AddProperty("RINDEX", energy, rindexAir, num);
	Air->SetMaterialPropertiesTable(mptAir);

	
	//Surface
	G4MaterialPropertiesTable *fTankMPT = new G4MaterialPropertiesTable();
	G4MaterialPropertiesTable *fWorldMPT = new G4MaterialPropertiesTable();
	G4MaterialPropertiesTable *fSurfaceMPT = new G4MaterialPropertiesTable();

	G4OpticalSurface* fSurface = new G4OpticalSurface("Surface");
	fSurface->SetType(dielectric_dielectric);
	fSurface->SetFinish(ground);
	fSurface->SetModel(unified);
	fSurface->SetMaterialPropertiesTable(fSurfaceMPT);

	//Define world volume

	G4Box* worldBox = new G4Box("solidWorld",  50*cm,  50 *cm, 50 *cm); //the solid world 
	G4LogicalVolume* worldLog = new G4LogicalVolume(worldBox, Air, "logicWorld"); //define logical world
	G4VPhysicalVolume* worldPhys = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), worldLog, "physWorld", 0, false, 0, true);
	
	//Define the quartz fiber properties

	G4double innerRadius2 = 0. * cm;
	G4double outerRadius2 = 0.075 * cm;
	G4double hz2 = 3.5 * cm;
	G4double startAngle2 = 0. * deg;
	G4double spanningAngle2 = 360. * deg;
	G4Tubs* solidRadiator = new G4Tubs("solidRadiator", innerRadius2, outerRadius2, hz2, startAngle2, spanningAngle2);
	G4LogicalVolume* logicalRadiator = new G4LogicalVolume(solidRadiator, SiO2, "logicalRadiator");
	G4VPhysicalVolume* physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0 ), logicalRadiator , "physWorld", worldLog, false, 0, true);

	//Surface

	G4LogicalBorderSurface* surface = new G4LogicalBorderSurface("Surface", physRadiator, worldPhys, fSurface);
	G4OpticalSurface* opticalSurface = dynamic_cast<G4OpticalSurface*>(
		surface->GetSurface(physRadiator, worldPhys)->GetSurfaceProperty());

	//Creating the PMT
	G4double innerRadius1 = 0. * mm;
	G4double outerRadius1 = 4 * mm;
	G4double hz1 = 0.001 * cm;
	G4double startAngle1 = 0. * deg;
	G4double spanningAngle1 = 360. * deg;
	G4Tubs* solidDetector = new G4Tubs("solidDetector", innerRadius1, outerRadius1, hz1, startAngle1, spanningAngle1);

	logicDetector = new G4LogicalVolume(solidDetector, SiO2, "logicDetector");

	G4VPhysicalVolume* physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0., 4 * cm), logicDetector, "physDetector", worldLog, false, 0, true);


	return worldPhys; // Always return to the highest mother volume
}

void MyDetectorConstruction::ConstructSDandField()
{
	//Here we make our "solid detector" to be sensitive and apply some filters to it

	  //accept only photons in range 1-4.5 eV
	G4SDParticleWithEnergyFilter* filter = new G4SDParticleWithEnergyFilter("photons filter");

	filter->add("opticalphoton");
	filter->SetKineticEnergy(1 * eV, 4.5 * eV); // (Emin,Emax)


	MySensitiveDetector* sensDet = new MySensitiveDetector("SensitiveDetector"); //creating object of MysensitiveDetector class
	sensDet->SetFilter(filter); //apply filters
	logicDetector->SetSensitiveDetector(sensDet);

}