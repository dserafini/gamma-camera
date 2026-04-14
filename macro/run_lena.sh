./sim macro/lenaVial.mac

# Invio notifica Telegram
EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then 
    telegram_notify "✅ Simulazione completata con successo" 
else 
    telegram_notify "❌ Simulazione terminata con errore (exit code $EXIT_CODE)" 
fi