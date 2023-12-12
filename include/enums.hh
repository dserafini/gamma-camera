# ifndef ENUMS_HH
# define ENUMS_HH 1

namespace Tuples {
  enum {
    kGeneration,
    kScintillator,
    kOptical,
    kSiPM
  };
}

namespace TGeneration {
  enum {
    kVertexX,
    kVertexY,
    kVertexZ
  };
}

namespace TScintillator {
  enum {
    kEdep,
    kGammaX,
    kGammaY,
    kGammaZ
  };
}

namespace TOptical {
  enum {
    kNumber,
    kMeanX,
    kMeanY,
    kMeanZ,
    kSigmaX,
    kSigmaY,
    kSigmaR
  };
}

# endif
