#!/bin/bash

# Timestamp per log: formato YYYYMMDD_HHMMSS
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
LOGFILE="run_${TIMESTAMP}.log"

REFLECTIVITY="0.94"
NEVENTS="10000000"
NEVENTSPROG=$(( NEVENTS / 10 ))
XPOS="0"
YPOS="0"

# Intestazione del file di log con data e ora
echo "=== Log started on $(date) ===" > "$LOGFILE"
echo "=== Log filename: ${LOGFILE} ===" >> "$LOGFILE"

for thickness in $(seq 0 1 3); do
    thickness_clean=$(printf "%g" $thickness)

    OUTFILE="Ba133_${NEVENTS}eventsReflectivity${REFLECTIVITY}coupling${thickness_clean}mmXpos${XPOS}yPos${YPOS}.root"
    TEMP_MACRO="macro_temp_${thickness_clean}.mac"

    echo ">>> [START] thickness ${thickness_clean} mm"

    ###########################################################################
    # CREA MACRO TEMPORANEA DA ZERO
    ###########################################################################
    cat > $TEMP_MACRO <<EOF
/run/numberOfThreads 5

################################################################################
# define scintillator
################################################################################
/scintillator/pixel matrix
/control/alias reflectivityPar ${REFLECTIVITY}
/scintillator/surfaceReflectivity {reflectivityPar}

################################################################################
# define collimator
################################################################################
/collimator/exist 1
/collimator/case_side 37. mm
/collimator/hole_length 30. mm
/collimator/septa_thickness 1. mm
/collimator/hole_thickness 2. mm
/collimator/positionX 0. mm
/collimator/positionY 0. mm

################################################################################
# change geometry detector
################################################################################
/control/alias pThreshold 0
/detector/pixel matrix
/detector/threshold {pThreshold}
/control/alias couplerThicknessMm ${thickness_clean}
/coupler/thickness {couplerThicknessMm} mm

################################################################################
# change geometry moby
################################################################################
/moby/build no

################################################################################
# change geometry vial
################################################################################
/vial/exist false
/control/alias vialDistY ${YPOS}
/control/alias vialPosY -{vialDistY}
/control/alias vialDistX ${XPOS}
/control/alias vialPosX -{vialDistX}
/vial/positionX {vialPosX} mm
/vial/positionY {vialPosY} mm
/vial/innerDiameter 6 mm
/vial/outerDiameter 8 mm
/vial/height 11 mm
/vial/base 1 mm

/run/reinitializeGeometry
/run/initialize

################################################################################
# choose gps source
################################################################################
/particleSource/sourceType gps
/gps/particle ion
/gps/ion 56 133 0 0.
/gps/position {vialPosX} {vialPosY} 0 mm
/gps/direction 0 0 1
/gps/energy 0 eV

################################################################################
# run
################################################################################
/run/printProgress ${NEVENTSPROG}
/control/alias nEvents ${NEVENTS}
/analysis/setFileName ${OUTFILE}
/run/beamOn {nEvents}
EOF

    ###########################################################################
    # LANCIA SIMULAZIONE (output → log)
    ###########################################################################
    ./sim $TEMP_MACRO >> "$LOGFILE" 2>&1

    echo ">>> [END]   thickness ${thickness_clean} mm"
    echo ">>> [HADD]  merging → ${OUTFILE}"

    ###########################################################################
    # HADD (solo su log)
    ###########################################################################

    BASE_NOEXT="${OUTFILE%.root}"

    # Hadd corretto
    hadd -f "${OUTFILE}" "${BASE_NOEXT}_t"*.root >> "$LOGFILE" 2>&1

    # Rimuovi i fragment temporanei
    rm -f "${BASE_NOEXT}_t"*.root
    rm -f "$TEMP_MACRO"

    echo "----------------------------------------------"

done

echo "########### ALL SIMULATIONS COMPLETED ###########"
echo "Log saved to $LOGFILE"
