#include "SteppingAction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4LogicalVolume.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!fScoringVolume) { 
    const DetectorConstruction* detectorConstruction
      = static_cast<const DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();
  }

  G4VPhysicalVolume* postpvolume 
    = step->GetPostStepPoint()->GetPhysicalVolume();
  
  if (!postpvolume) {
    return;
  } else {
    G4LogicalVolume* postlvolume = postpvolume->GetLogicalVolume();
    if (postlvolume != fScoringVolume || step->GetTotalEnergyDeposit() == 0) {
    // if (postlvolume != fScoringVolume) {
      return;
    } else {
      fEventAction->AddHit(1);
      G4double edepStep = step->GetTotalEnergyDeposit();
      fEventAction->AddEdep(edepStep);
    }
  }
}