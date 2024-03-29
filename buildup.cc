#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "G4RunManagerFactory.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"

int main(int argc,char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;

  if ( argc == 1 ) {
    throw std::invalid_argument("More Arguments Needed");
  }

  std::string sargv = argv[1];
  std::string delimiter = "-";
  G4double thick = std::stod(sargv.substr(0, sargv.find(delimiter))) * cm;
  sargv.erase(0, sargv.find(delimiter) + delimiter.length());
  G4double distance = std::stod(sargv.substr(0, sargv.find(delimiter))) * cm;
  sargv.erase(0, sargv.find(delimiter) + delimiter.length());
  G4int collimator = std::stoi(sargv.substr(0, sargv.find(delimiter)));
  sargv.erase(0, sargv.find(delimiter) + delimiter.length());
  G4int attenuator = std::stoi(sargv.substr(0, sargv.find(delimiter)));

  if ( argc == 2 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Optionally: choose a different Random engine...
  // G4Random::setTheEngine(new CLHEP::MTwistEngine);
  
  // Construct the default run manager
  //
  auto* runManager =
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new DetectorConstruction(thick, distance, collimator, attenuator, argv[1]));

  // Physics list
  G4VModularPhysicsList* physicsList = new QBBC(0);
  runManager->SetUserInitialization(physicsList);
    
  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization());
  
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( ! ui ) { 
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[2];
    UImanager->ApplyCommand(command+fileName);
  }
  else { 
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
  delete visManager;
  delete runManager;
}