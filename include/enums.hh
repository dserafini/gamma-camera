# ifndef ENUMS_HH
# define ENUMS_HH 1

namespace Tuples {
  enum {
    kEvents,
    kGeneration,
    kScintillator,
    kOptical,
    kSipm
  };
}

namespace TEvents {
  enum {
    kEvents,
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
    kGammaZ,
    kProc
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

namespace Tsipm {
  enum {
    kNumber,
    kMeanX,
    kMeanY,
    kMostX,
    kMostY
  };
}

namespace Pgamma {
  enum {
    kDefault,
    kCompt,
    kPhoto,
    kNoElectron,
    kNoProcess
  };
}

# endif
