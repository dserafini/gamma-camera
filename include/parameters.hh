#ifndef PARAMETERS_HH
#define PARAMETERS_HH

namespace Collimator {
    constexpr G4int pinhole_side_number = 23;
}

namespace Scintillator {
    constexpr G4double gagg_thickness = 17.00*mm;
    constexpr G4double gagg_side = 1.00*mm;
    constexpr G4double baso4_side = 0.20*mm;
    constexpr G4double aluminum_side = 0.03*mm;
    constexpr G4int matchstick_side_number = 23;
}

namespace Sipm {
    constexpr G4int channel_side_number = 8;
}

#endif