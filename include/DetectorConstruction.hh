#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction(const G4double, const G4double, const G4int, const G4int, const G4String);
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

    G4String GetArguments() const { return arguments; }

  protected:
    G4LogicalVolume* fScoringVolume;
    G4double thick;
    G4double distance;
    G4int collimator;
    G4int attenuator;
    G4String arguments;
};

#endif