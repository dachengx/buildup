#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;

/// Event action class
///

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction* runAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddHit(G4int hit) { fnHit += hit; }
    void AddEdep(G4double edep) { fEdep += edep; }

  private:
    RunAction* fRunAction;
    G4int fnHit;
    G4double fEdep;
};

#endif