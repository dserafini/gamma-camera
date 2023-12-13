# ifndef ENUMS_HH
# define ENUMS_HH 1

namespace Tuples {
  enum {
    kGeneration,
    kScintillator,
    kOptical,
    kSipm
  };
}

namespace TGeneration {
  enum {
    kEini,
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
    kMeanX,
    kMeanY,
    kMeanZ,
    kSigmaX,
    kSigmaY,
    kSigmaR
  };
}

namespace Tsipm {
  enum {
    kNumber,
    kMeanX,
    kMeanY,
    kMostX,
    kMostY
  };
}

# endif
