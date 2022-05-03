Dieses Release kann unter Windows 10/11 folgendermaßen installiert werden:
- falls noch nicht geschehen: Das gesamte zip in ein Verzeichnis entpacken
- das Verzeichnis, in das entpackt wurde, öffnen
- rechte Maustaste auf Build-knxprod.ps1
- "Mit PowerShell ausführen" wählen, ggf. die Sicherheitswarnung mit "Datei öffnen" bestätigen
    (jetzt wird eine zum Release passende Produktdatenbank *.knxprod gebaut)


- je nachdem, ob man eine Hardware mit einem SAMD oder einem RP2040 Prozessor hat:
  (Hinweis: Es darf nur ein ARDUINO am USB stecken)

SAMD
    - Hardware an den USB-Port stecken, 
    - beim Sensormodul dieses auch zusätzlich mit dem KNX Bus verbinden
    - Je nachdem, of man ein Sensormodul v31 oder v30 hat, muss das passende "Upload-Firmware-SAMD-vXX.ps1" Skript gewählt werden!
    - rechte Maustaste auf "Upload-Firmware-SAMD-vXX.ps1" 
    - "Mit PowerShell ausführen" wählen
        (jetzt wird die Firmware auf die Hardware geladen)
    - die erfolgreiche Detektierung der Schnittstelle und Programmierung im Screen der Powershell prüfen
    - falls der SAMD am USB nicht gefunden wurde, die Reset-Taste 2 mal drücken (die LED muss pulsieren) und 
        dann nochmal "Mit Powershell ausführen" wählen.

RP2040
    - Hardware an den USB-Port stecken,
    - rechte Maustaste auf "Upload-Firmware-RP2040.ps1"
    - "Mit PowerShell ausführen" wählen
        (jetzt wird die Firmware auf die Hardware geladen)
    - sobald die Firmware erfolgreich hochgeladen wurde, startet sich das Modul neu

Jetzt kann man die erzeugte knxprod in die ETS über den Katalog importieren und
danach wie gewohnt zuerst die Physikalische Adresse und nach der Parametrierung die Applikation programmieren.
Bitte noch die Applikationsbeschreibung beachten, dort stehen Hinweise zum update (ob man z.B. nur Firmware- oder nur ETS-Update braucht, normalerweise braucht man beides).
Fertig.
