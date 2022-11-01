# ESP32 Kamera

In diesem Beispiel seht ihr wie man ein ESP Kamera Modul benutzen kann um eventuell eine Fernübertragung über WiFi zu machen?

Der Code und eine genaue Anleitung auf Englisch ist hier zu finden: [https://github.com/easytarget/esp32-cam-webserver](https://github.com/easytarget/esp32-cam-webserver)

## Übersicht

1. Kamera Modul

   Das Modul sieht wie folgt aus

   <img src="doc/th-1994321534.png">

   Ihr habt einen USB Anschluss zur programmierung.

2. Configuration

   Ihr müsst die Arduino Entwicklungsumgebung etwas umkonfigurieren.

   <img src="doc/ota-board-selection.png">

   Wechselt in das Verzeichnis `esp32-cam-webserver` und öffnet jetzt die Datei `esp32-cam-webserver.ino`

   Um das WLAN zu konfigurieren kopiert ihr die Datei `myconfig.sample.h` nach `myconfig.h` und ersetzt die folgende Zeile:

   ```
   struct station stationList[] = {{"my_ssid","my_password", true}};
   ```

   mit dieser

   ```
   struct station stationList[] = {{"MINT-iot","PASSWORT STEHT AN DER TAFEL", true}};
   ```

   Danach könnt ihr den Code auf das ESP Modul überspielen.

   Auf dem Seriellen Monitor `Tools -> Serial Monitor` werden die IP Adresse und die URL zum Videobild ausgegeben.


## Und jetzt?

* Ihr könnt ein Gehäuse für die Kamera drucken um diese auf dem Auto zu befestigen
* Stellt die Steuerung des Fahrzeugs auf WiFi um und versucht im gesamten Gebäude zu fahren?