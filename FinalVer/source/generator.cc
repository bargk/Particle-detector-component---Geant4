#include "generator.hh"
#include "Randomize.hh"
#include "G4RandomDirection.hh"
#include "G4Run.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1); //how many particles per event

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable(); //storing all the particles in particleTable
    G4ParticleDefinition* particle = particleTable->FindParticle("geantino");

    
    //G4ThreeVector pos(1*cm, 0, 0);  //starting position of the particle
    //G4ThreeVector mom(-1, 0, 0);
    fParticleGun->SetParticleEnergy(0 * MeV);
    /*fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);*/
    fParticleGun->SetParticleDefinition(particle); 
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent) //this function create particle for each event
{
    G4ParticleDefinition* particle = fParticleGun->GetParticleDefinition();
    if (particle == G4Geantino::Geantino())
    {
        G4int Z = 88;
        G4int A = 226;

        G4double charge = 0 *eplus;
        G4double energy = 0 * keV;

        G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z, A, energy);

        
        fParticleGun->SetParticleDefinition(ion);
        fParticleGun->SetParticleCharge(charge);


    }

    //define uniform distribution over a box of width and lengh of "envsize" cm
   // This is to take into account the dimensions of the source
    G4double envsize = 2;
    G4double y0 = (envsize * (G4UniformRand() - 0.5)) * cm;
        G4double z0 = (envsize * (G4UniformRand() - 0.5)) * cm;
    G4double x0 = 2 * cm;

   

   fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
   G4ThreeVector momentumUnitVector = G4RandomDirection(); //random vector of mangnitude unity
   fParticleGun->SetParticleMomentumDirection(momentumUnitVector);
   fParticleGun->GeneratePrimaryVertex(anEvent);


}