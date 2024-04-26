# ifndef ENUMS_HH
# define ENUMS_HH 1

namespace Tuples {
  enum {
    kEvents,
    kGeneration,
    kScintillator,
    kOptical,
    kSipm,
    kAllOptical,
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
    kVertexZ,
    kEventID
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

namespace Tsipm {
  enum {
    kNumber,
    kMeanX,
    kMeanY,
    kMostX,
    kMostY
  };
}

namespace TAllOptical {
  enum {
    kEventID,
    kSipmX,
    kSipmY
  };
}

# endif
