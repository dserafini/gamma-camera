#!/bin/bash

# Timestamp per log: formato YYYYMMDD_HHMMSS
START=$(date +%s)
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
LOGFILE="run_${TIMESTAMP}.log"

REFLECTIVITY="0.94"
NEVENTS="1000000"
NEVENTSPROG=$(( NEVENTS / 10 ))
XPOS="0"
YPOS="0"
NOTHREADS="10"
OPTICALCOUPLING="2"

# Intestazione del file di log con data e ora
echo "=== Log started on $(date) ===" > "$LOGFILE"
echo "=== Log filename: ${LOGFILE} ===" >> "$LOGFILE"

for gamma_energy in $(seq 50 360 410); do
    OUTFILE="lenaVialNoCollimator${NEVENTS}eventsReflectivity${REFLECTIVITY}pointSourceGammaIsotropic${gamma_energy}keV.root"
    TEMP_MACRO="macro_temp_${gamma_energy}.mac"

    echo ">>> [START] gamma energy ${gamma_energy} keV"

    ###########################################################################
    # CREA MACRO TEMPORANEA DA ZERO
    ###########################################################################
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

/collimator/exist 0
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
/coupler/thickness ${OPTICALCOUPLING} mm

################################################################################
# change geometry moby
################################################################################
/moby/build no

################################################################################
# change geometry vial
################################################################################
/vial/exist false
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
/gps/particle gamma
/gps/position 0 0 0 mm
/gps/energy ${gamma_energy} keV
#/gps/direction 0 0 1
/gps/ang/type iso

################################################################################
# tumore
################################################################################
/run/printProgress ${NEVENTSPROG}

#/tracking/verbose 2
/control/alias nEvents ${NEVENTS}
/analysis/setFileName ${OUTFILE}
/run/beamOn {nEvents}
EOF

    ###########################################################################
    # LANCIA SIMULAZIONE (output → log)
    ###########################################################################
    ./sim $TEMP_MACRO >> "$LOGFILE" 2>&1
    EXIT_CODE=$?

    if [ "$EXIT_CODE" -ne 0 ]; then
        telegram_notify "❌ Simulazione terminata con errore (exit code $EXIT_CODE)" 
    fi

    echo ">>> [END]   energy ${gamma_energy} keV"

    ###########################################################################
    # HADD (solo su log)
    ###########################################################################

    echo ">>> [HADD]  merging → ${OUTFILE}"
    BASE_NOEXT="${OUTFILE%.root}"

    # Hadd corretto
    hadd -f "${OUTFILE}" "${BASE_NOEXT}_t"*.root >> "$LOGFILE" 2>&1
    rm -f "${BASE_NOEXT}_t"*.root

    # Rimuovi i fragment temporanei
    rm -f "$TEMP_MACRO"

    echo "----------------------------------------------"

done

END=$(date +%s)
DURATION=$((END-START))

echo "########### ALL SIMULATIONS COMPLETED ###########"
echo "Simulation time $((DURATION/3600))h $(((DURATION%3600)/60))m $(((DURATION%3600)%60))s"
echo "Log saved to $LOGFILE"



telegram_notify \
"Simulazione finita su $(hostname)
Durata: $((DURATION/3600))h $(((DURATION%3600)/60))m $(((DURATION%3600)%60))s"
