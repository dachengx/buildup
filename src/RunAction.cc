#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4GeneralParticleSource.hh"

RunAction::RunAction()
: G4UserRunAction(),
  fTotalParticlesN(0),
  fHitDetectorParticlesN(0),
  fEdep(0.)
{
  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fTotalParticlesN);
  accumulableManager->RegisterAccumulable(fHitDetectorParticlesN);
  accumulableManager->RegisterAccumulable(fEdep);
}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{ 
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();

}

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables 
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const PrimaryGeneratorAction* generatorAction = static_cast<const PrimaryGeneratorAction*>(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  G4double particleEnergy;
  if (generatorAction)
  {
    const G4GeneralParticleSource* particleGun = generatorAction->GetGeneralParticleSource();
    runCondition += particleGun->GetCurrentSource()->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy, "Energy");
  }

  if (IsMaster()) {
    G4cout
     << G4endl
     << "-----End of Global Run-----";
  }
  else {
    G4cout
     << G4endl
     << "-----End of Local Run------";
  }
  const DetectorConstruction* detectorConstruction = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4double mass = detectorConstruction->GetScoringVolume()->GetMass();
  G4double edep  = fEdep.GetValue();
  G4double dose = edep / mass;
  G4cout
  << G4endl
  << " The run consists of " << nofEvents << " " << runCondition
  << G4endl
  << " Cumulated Hits per run : " 
  << fHitDetectorParticlesN.GetValue()
  << " Cumulated dose per run : " 
  << G4BestUnit(dose, "Dose")
  << G4endl
  << "---------------------------"
  << G4endl;
  std::ofstream outFile;
  outFile.open("result/" + detectorConstruction->GetArguments() + ".csv", std::ios::app);
  outFile << particleEnergy << "," << fHitDetectorParticlesN.GetValue() << "," << dose << G4endl;
  outFile.close();
}

void RunAction::AddHit(G4int nparticles)
{
  fHitDetectorParticlesN += nparticles;
}

void RunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
}