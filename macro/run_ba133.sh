START=$(date +%s)

./sim macro/ba133.mac

END=$(date +%s)
DURATION=$((END-START))

# Invio notifica Telegram
EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then 

    telegram_notify \
    "Simulazione finita su $(hostname)
    Durata: $((DURATION/3600))h $(((DURATION%3600)/60))m"

else 
    telegram_notify "❌ Simulazione terminata con errore (exit code $EXIT_CODE)" 
fi



