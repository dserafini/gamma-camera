START=$(date +%s)

./sim macro/lenaVial.mac

END=$(date +%s)
DURATION=$((END-START))

telegram_notify \
"Simulazione finita su $(hostname)
Durata: $((DURATION/3600))h $(((DURATION%3600)/60))m"

