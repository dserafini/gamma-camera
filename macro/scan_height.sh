#!/bin/bash

START=$(date +%s)
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
LOGFILE="run_Ba133_Zscan_${TIMESTAMP}.log"

NOTHREADS="10"
REFLECTIVITY="0.94"
COUPLER="2"
NEVENTS="100000000"
NEVENTSPROG=$(( NEVENTS / 1000 ))

# scan Z (in cm nel nome, ma mm nella macro)
for ZPOS in $(seq 20 10 30); do

    echo ">>> [START] Z = ${ZPOS} mm"

    TEMP_MACRO="macro_Z${ZPOS}.mac"

    OUTFILE="Ba133_${NEVENTS}eventsReflectivity${REFLECTIVITY}coupling${COUPLER}mmZ${ZPOS}mm.root"
    BASE_NOEXT="${OUTFILE%.root}"

    cat > $TEMP_MACRO <<EOF
/run/numberOfThreads ${NOTHREADS}

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
# detector
################################################################################

/control/alias pThreshold 0
/detector/pixel matrix
/detector/threshold {pThreshold}

/control/alias couplerThicknessMm ${COUPLER}
/coupler/thickness {couplerThicknessMm} mm

################################################################################
# moby
################################################################################

/moby/build no

################################################################################
# vial + posizione variabile Z
################################################################################

/vial/exist false

/control/alias vialDistX 0
/control/alias vialPosX -{vialDistX}

/control/alias vialDistY 0
/control/alias vialPosY -{vialDistY}

/control/alias vialDistZ ${ZPOS}
/control/alias vialPosZ -{vialDistZ}

################################################################################

/run/reinitializeGeometry
/run/initialize

################################################################################
# sorgente Ba-133
################################################################################

/particleSource/sourceType gps
/gps/particle ion
/gps/ion 56 133 0 0.
/gps/position {vialPosX} {vialPosY} {vialPosZ} mm
/gps/direction 0 0 1
/gps/energy 0 eV

################################################################################

/run/printProgress ${NEVENTSPROG}

/control/alias nEvents ${NEVENTS}

/analysis/setFileName ${OUTFILE}

/run/beamOn {nEvents}
EOF

    ###########################################################################
    # RUN
    ###########################################################################
    ./sim $TEMP_MACRO >> "$LOGFILE" 2>&1
    EXIT_CODE=$?

    if [ "$EXIT_CODE" -ne 0 ]; then
        telegram_notify "❌ Errore simulazione Z=${ZPOS} (exit $EXIT_CODE)"
    fi

    echo ">>> [HADD] Z = ${ZPOS}" >> "$LOGFILE"

    hadd -f "${OUTFILE}" "${BASE_NOEXT}_t"*.root >> "$LOGFILE" 2>&1
    rm -f "${BASE_NOEXT}_t"*.root

    rm -f "$TEMP_MACRO"

    echo ">>> [END] Z = ${ZPOS}"
    echo "----------------------------------------------"

done

END=$(date +%s)
DURATION=$((END-START))

echo "########### DONE ###########"
echo "Time: $((DURATION/3600))h $(((DURATION%3600)/60))m $((DURATION%60))s"
echo "Log: $LOGFILE"

telegram_notify \
"Z-scan Ba133 completata su $(hostname)
Durata: $((DURATION/3600))h $(((DURATION%3600)/60))m $((DURATION%60))s"