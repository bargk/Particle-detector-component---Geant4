#include "construction.hh"

#include "G4OpticalSurface.hh"
#include "G4SDParticleWithEnergyFilter.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4PhysicsVector.hh"
#include "G4LogicalBorderSurface.hh"

#include "detector.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	fMessenger = new G4GenericMessenger(this, "/detector/", "Detectors materials");
	fMessenger->DeclareProperty("Cone", Cone, "Enable or disable the cone (true or false)");
	fMessenger->DeclareProperty("PMT", z_PMT, "Location of the PMT along Z axis in cm");
	Cone = true;
	z_PMT = 5;

	DefineMaterial();
	
}

MyDetectorConstruction::~MyDetectorConstruction()
{
	
}

void MyDetectorConstruction::DefineMaterial()
{
	// materials
	G4NistManager* nist = G4NistManager::Instance();
	Air = nist->FindOrBuildMaterial("G4_AIR");

	SiO2 = new G4Material("Quartz", 2.201 * g / cm3, 2); //Quartz material
	SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
	SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

	StainlessSteel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");

	
}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{

	G4int num = 8;

	G4double energy[num] = { 1.23984198 * eV / 0.65, 1.23984198 * eV / 0.6, 1.23984198 * eV / 0.55, 1.23984198 * eV / 0.5,
							1.23984198 * eV / 0.45, 1.23984198 * eV / 0.4, 1.23984198 * eV / 0.35, 1.23984198 * eV / 0.3 };

	G4double rindexSiO2[num] = {
	1.5, 1.5, 1.5, 1.5,  1.5, 1.5, 1.5, 1.5 };
	G4double rindexAir[num] = {
	1.0,1.0, 1.0, 1.0,  1.0, 1.0,1.0, 1.0 };
	G4double rindexSteel[num] = { 2.0, 1.8, 1.7, 1.5, 1.4, 1.3, 1.25, 1.2 };

	G4MaterialPropertiesTable* mptSiO2 = new G4MaterialPropertiesTable();
	mptSiO2->AddProperty("RINDEX", energy, rindexSiO2, num);
	SiO2->SetMaterialPropertiesTable(mptSiO2);

	G4MaterialPropertiesTable* mptAir = new G4MaterialPropertiesTable();
	mptAir->AddProperty("RINDEX", energy, rindexAir, num);
	Air->SetMaterialPropertiesTable(mptAir);

	

	//surface
	G4MaterialPropertiesTable *fTankMPT = new G4MaterialPropertiesTable();
	G4MaterialPropertiesTable *fWorldMPT = new G4MaterialPropertiesTable();
	G4MaterialPropertiesTable *fSurfaceMPT = new G4MaterialPropertiesTable();

	G4OpticalSurface* fSurface = new G4OpticalSurface("Surface");
	fSurface->SetType(dielectric_dielectric);
	fSurface->SetFinish(ground);
	fSurface->SetModel(unified);
	fSurface->SetMaterialPropertiesTable(fSurfaceMPT);

	//Define world volume
	worldBox = new G4Box("solidWorld",  50*cm,  50 *cm, 50 *cm); //the solid world 
	worldLog = new G4LogicalVolume(worldBox, Air, "logicWorld"); //define logical world
	worldPhys = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), worldLog, "physWorld", 0, false, 0, true);

	//Define the quartz fiber properties
	G4double innerRadius2 = 0. * cm;
	G4double outerRadius2 = 0.075 * cm;
	G4double hz2 = 3.5 * cm;
	G4double startAngle2 = 0. * deg;
	G4double spanningAngle2 = 360. * deg;
	solidRadiator = new G4Tubs("solidRadiator", innerRadius2, outerRadius2, hz2, startAngle2, spanningAngle2);
	logicalRadiator = new G4LogicalVolume(solidRadiator, SiO2, "logicalRadiator");
	physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0 ), logicalRadiator , "physWorld", worldLog, false, 0, true);
	
	//surface
	G4OpticalSurface* OpSurface = new G4OpticalSurface("name");
	G4LogicalBorderSurface* surface = new G4LogicalBorderSurface("Surface", physRadiator, worldPhys, OpSurface);
	OpSurface->SetType(dielectric_dielectric);
	OpSurface->SetModel(unified);
	OpSurface->SetFinish(polished);
	G4double reflectivity_quartz[num] = {
	0.9,0.9, 0.9, 0.9,  0.9, 0.9,0.9, 0.9 };
	G4MaterialPropertiesTable* SMPT = new G4MaterialPropertiesTable();
	SMPT->AddProperty("REFLECTIVITY", energy, reflectivity_quartz,num);
	SMPT->AddProperty("RINDEX", energy, rindexSiO2,num);
	OpSurface->SetMaterialPropertiesTable(SMPT);

	//Creating the PMT
	G4double innerRadius1 = 0. * mm;
	G4double outerRadius1 = 4 * mm;
	G4double hz1 = 0.001 * cm;
	G4double startAngle1 = 0. * deg;
	G4double spanningAngle1 = 360. * deg;
	
	solidDetector = new G4Tubs("solidDetector", innerRadius1, outerRadius1, hz1, startAngle1, spanningAngle1);
	logicDetector = new G4LogicalVolume(solidDetector, SiO2, "logicDetector");
	physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0., z_PMT * cm), logicDetector, "physDetector", worldLog, false, 0, true);


	if (Cone == true)
	{
		// Cone
		G4double reflectivity[num] = {
	0.9, 0.9, 0.9, 0.9,  0.9, 0.9, 0.9, 0.9 };
		G4MaterialPropertiesTable* mptStainlessSteel = new G4MaterialPropertiesTable();
		mptStainlessSteel->AddProperty("RINDEX", energy, rindexSteel, num);
		mptStainlessSteel->AddProperty("REFLECTIVITY", energy, reflectivity, num);
		//StainlessSteel->SetMaterialPropertiesTable(mptStainlessSteel);



		G4double Rmin1 = 0.075001 * cm;			// Inside radius at -Dz
		G4double Rmin2 = 0.18 * cm;				// Inside radius at +Dz
		G4double Rmax1 = 0.085 * cm;				// Outside radius at -Dz
		G4double Rmax2 = 0.19 * cm;				// Outer radius at +Dz
		G4double Dz = 0.125 * cm;				// Half lengh in Z direction
		G4double SPhi = 0. * deg;				// Starting angle of the segment 
		G4double DPhi = 360. * deg;				// The angle of the segment 

		solidCone = new G4Cons("solidCone", Rmin1, Rmax1, Rmin2, Rmax2, Dz, SPhi, DPhi);
		logicCone = new  G4LogicalVolume(solidCone, StainlessSteel, "logicCone");
		physCone = new G4PVPlacement(0, G4ThreeVector(0, 0, 3.624 * cm), logicCone, "physCone", worldLog, false, 0, true);

		// Surface
		//G4LogicalBorderSurface* surface1 = new G4LogicalBorderSurface("Surface", physCone, worldPhys, fSurface);
		G4OpticalSurface* coneSurface = new G4OpticalSurface("ConeOptics");
		coneSurface->SetType(dielectric_metal);
		coneSurface->SetModel(unified);
		coneSurface->SetFinish(polished); //The cone is polished
		coneSurface->SetMaterialPropertiesTable(mptStainlessSteel);
		G4LogicalSkinSurface* Surface1 = new G4LogicalSkinSurface("ConeSurface", logicCone, coneSurface);

	}

	return worldPhys; //Always return to the highest mother volume
}

void MyDetectorConstruction::ConstructSDandField()
{
	//Here we make our "PMT" to be sensitive for particles and apply some filters to it

	  //accept only photons in range 1-4.5 eV
	G4SDParticleWithEnergyFilter* filter = new G4SDParticleWithEnergyFilter("photons filter");

	filter->add("opticalphoton");
	filter->SetKineticEnergy(1 * eV, 4.5 * eV);


	MySensitiveDetector* sensDet = new MySensitiveDetector("SensitiveDetector"); //creating object of MysensitiveDetector class
	sensDet->SetFilter(filter); //apply filters
	logicDetector->SetSensitiveDetector(sensDet);
	//logicalRadiator->SetSensitiveDetector(sensDet); // Enable this in case you intrested
													  // In the number of photons created in the rod
}