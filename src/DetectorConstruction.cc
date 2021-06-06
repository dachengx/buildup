#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

DetectorConstruction::DetectorConstruction(G4double t, G4double d, G4int c, G4int a, G4String ss)
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{
  thick = t;
  distance = d;
  collimator = c;
  attenuator = a;
  arguments = ss;
}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double world_sizeXY = 20*cm, world_sizeZ = 30*cm;
  G4double density = universe_mean_density;
  G4double temperature = 0.1*kelvin;
  G4double pressure = 1.e-19*pascal;
  G4Material* world_mat = new G4Material("Galactic", 1., 1.01*g/mole, density, kStateGas, temperature, pressure);
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  
  G4Box* solidWorld =    
    new G4Box("World", world_sizeXY, world_sizeXY, world_sizeZ);
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld, world_mat, "World");
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  //     
  // Source
  //  
  G4ThreeVector pos0 = G4ThreeVector(0, 0, -10.*cm);

  G4double source_pRMin =  0.*cm, source_pRMax = 0.635*cm;
  G4double source_pDz =  0.5*cm;
  G4double source_pSPhi = 0.*deg, source_pDPhi = 360.*deg;
  G4Tubs* solidSource =    
    new G4Tubs("Source", 
    source_pRMin, source_pRMax, source_pDz, source_pSPhi, source_pDPhi);
                      
  G4LogicalVolume* logicSource =                         
    new G4LogicalVolume(solidSource,         //its solid
                        world_mat,           //its material
                        "Source");           //its name
               
  new G4PVPlacement(0,                       //no rotation
                    pos0,                    //at position
                    logicSource,             //its logical volume
                    "Source",                //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  //     
  // Collimator
  //  
  if ( collimator ) {
    G4Material* collimator_mat = nist->FindOrBuildMaterial("G4_Pb");
    G4ThreeVector pos1 = G4ThreeVector(0, 0, -3.*cm);

    G4double collimator_pRMin =  0.5*cm, collimator_pRMax = 20.*cm;
    G4double collimator_pDz =  3.*cm;
    G4double collimator_pSPhi = 0.*deg, collimator_pDPhi = 360.*deg;
    G4Tubs* solidCollimator =    
      new G4Tubs("Collimator", 
      collimator_pRMin, collimator_pRMax, collimator_pDz, collimator_pSPhi, collimator_pDPhi);
                        
    G4LogicalVolume* logicCollimator =                         
      new G4LogicalVolume(solidCollimator,         //its solid
                          collimator_mat,          //its material
                          "Collimator");           //its name
                
    new G4PVPlacement(0,                       //no rotation
                      pos1,                    //at position
                      logicCollimator,         //its logical volume
                      "Collimator",            //its name
                      logicWorld,              //its mother  volume
                      false,                   //no boolean operation
                      0,                       //copy number
                      checkOverlaps);          //overlaps checking
  }

  //     
  // Attenuator
  //  
  if ( attenuator ) {
    G4Element* Bi = nist->FindOrBuildElement(83);
    G4Element* Pb = nist->FindOrBuildElement(82);
    G4Element* Sn = nist->FindOrBuildElement(50);
    G4Material* MCP_96 = new G4Material("MCP_96", 9.72*g/cm3, 3);
    MCP_96->AddElement(Bi, 0.525);
    MCP_96->AddElement(Pb, 0.32);
    MCP_96->AddElement(Sn, 0.155);

    G4double attenuator_pX = 2.5*cm, attenuator_pY = 2.5*cm, attenuator_pZ = thick / 2;
    G4ThreeVector pos2 = G4ThreeVector(0, 0, attenuator_pZ);
    G4Box* solidAttenuator =    
      new G4Box("Attenuator", 
      attenuator_pX, attenuator_pY, attenuator_pZ);
                        
    G4LogicalVolume* logicAttenuator =                         
      new G4LogicalVolume(solidAttenuator,     //its solid
                          MCP_96,              //its material
                          "Attenuator");       //its name
                
    new G4PVPlacement(0,                       //no rotation
                      pos2,                    //at position
                      logicAttenuator,         //its logical volume
                      "Attenuator",            //its name
                      logicWorld,              //its mother  volume
                      false,                   //no boolean operation
                      0,                       //copy number
                      checkOverlaps);          //overlaps checking
  }

  //     
  // Detector
  //  
  G4Material* detector_mat = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");

  G4double detector_pX = 2.54*cm, detector_pY = 2.54*cm, detector_pZ = 4.*cm;
  G4ThreeVector pos3 = G4ThreeVector(0, 0, detector_pZ + thick + distance);
  G4Box* solidDetector =    
    new G4Box("Detector", 
    detector_pX, detector_pY, detector_pZ);
                      
  G4LogicalVolume* logicDetector =                         
    new G4LogicalVolume(solidDetector,     //its solid
                        detector_mat,      //its material
                        "Detector");       //its name
               
  new G4PVPlacement(0,                       //no rotation
                    pos3,                    //at position
                    logicDetector,           //its logical volume
                    "Detector",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  fScoringVolume = logicDetector;

  //
  //always return the physical World
  //
  return physWorld;
}