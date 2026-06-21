#!/bin/bash

START=$(date +%s)
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
LOGFILE="run_lena_Zscan_${TIMESTAMP}.log"

NOTHREADS="10"
REFLECTIVITY="0.94"
COUPLER="2"
NEVENTS="100000"
NEVENTSPROG=$(( NEVENTS / 1000 ))

# scan Z (in cm nel nome, ma mm nella macro)
for ZPOS in $(seq 0 1 10); do

    echo ">>> [START] Z = ${ZPOS} mm"

    TEMP_MACRO="macro_Z${ZPOS}.mac"

    OUTFILE="lena_${NEVENTS}eventsSmallVialReflectivity${REFLECTIVITY}coupling${COUPLER}mmZ${ZPOS}mm.root"
    BASE_NOEXT="${OUTFILE%.root}"

    cat > $TEMP_MACRO <<EOF
/run/numberOfThreads ${NOTHREADS}

################################################################################
# define scintillator
################################################################################

/scintillator/pixel matrix
/control/alias reflectivityPar ${REFLECTIVITY}
/scintillator/surfaceReflectivity {reflectivityPar}
/run/numberOfThreads 10

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
/coupler/thickness {COUPLER} mm

################################################################################
# change geometry moby
################################################################################
/moby/build no

################################################################################
# change geometry vial
################################################################################
/vial/exist true
/control/alias vialDistY 0
/control/alias vialPosY -{vialDistY}
/control/alias vialDistX 0
/control/alias vialPosX {vialDistX}
/vial/positionX {vialPosX} mm
/vial/positionY {vialPosY} mm
/control/alias sourceRadius 3 
/control/multiply sourceDiameter {sourceRadius} 2 
/control/add outerVialDiameter {sourceDiameter} 2 
/vial/innerDiameter {sourceDiameter} mm
/vial/outerDiameter {outerVialDiameter} mm
/vial/height 11 mm
/vial/base 1 mm

/run/reinitializeGeometry

# initalize if not done in the main
/run/initialize

################################################################################
# choose gps source
################################################################################
/particleSource/sourceType gps
# Ag-111
/gps/particle ion
/gps/ion 47 111 0 0.
/gps/pos/type Volume
/gps/pos/shape Cylinder
/gps/pos/centre {vialPosX} {vialPosY} -6 mm
/gps/pos/halfz 5 mm
/gps/pos/radius {sourceRadius} mm
/gps/direction 0 0 1
/gps/energy 0 eV
#/gps/particle gamma
#/gps/energy 342 keV
#/gps/ang/type iso

################################################################################
# tumore
################################################################################
/run/printProgress 100000000

#/tracking/verbose 2
/control/alias nEvents 2000000000
/analysis/setFileName lenaVialSourceDiameter{sourceDiameter}mmWwater{nEvents}eventsReflectivity{reflectivityPar}yPos{vialDistY}mmxPos{vialDistX}mmAttenuationLengthGagg645mm.root
/run/beamOn {nEvents}

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