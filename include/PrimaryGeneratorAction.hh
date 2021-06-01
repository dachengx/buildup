#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4GeneralParticleSource.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued 
/// in front of the phantom across 80% of the (X,Y) phantom size.

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();    
    ~PrimaryGeneratorAction();

    const G4GeneralParticleSource* GetGeneralParticleSource() const { return particleGun; }

    void GeneratePrimaries(G4Event* anEvent);
  
  private:
    G4GeneralParticleSource* particleGun;
};

#endif