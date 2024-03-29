#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    virtual ~RunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void AddHit(G4int nparticles);
    void AddEdep(G4double edep);

  private:
    G4Accumulable<G4int> fTotalParticlesN;
    G4Accumulable<G4int> fHitDetectorParticlesN;
    G4Accumulable<G4double> fEdep;
};

#endif