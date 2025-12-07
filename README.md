# Okos Növényfigyelő Rendszer Analóg Mutatóval

## Rövid leírás

A projekt célja egy asztali, „okos” növényfigyelő állomás elkészítése Arduino mikrokontroller segítségével.
A rendszer folyamatosan méri egy szobanövény talajának nedvességét, a környezet hőmérsékletét, páratartalmát és fényintenzitását.

A mért adatok alapján különböző visszajelzéseket ad:

I2C kijelzőn megjeleníti az aktuális értékeket és esetleges figyelmeztetéseket
(például: „Túl száraz!”, „Túl sötét!”).

RGB LED vizuálisan jelzi a növény állapotát

Zöld: minden érték rendben

Piros: öntözés szükséges

Egyéb színek: egyéb figyelmeztetések

Szervomotor egy fizikai mutatót mozgat egy skálán, amely analóg műszerként mutatja a talajnedvesség szintjét („Száraz” → „Nedves”).

A rendszer számlálja, hogy hányszor jelzett „száraz” állapotot (öntözési emlékeztető funkció).
A számláló egy nyomógombbal lenullázható.

Az öntözés manuálisan történik, a rendszer csak figyelmeztet és monitoroz.

## Alkatrész lista:
* Főbb komponensek
* 1× Mikrokontroller
* (Arduino Uno vagy Arduino Nano ajánlott)
* 1× I2C kijelző
* (0.96" OLED vagy 16×2 LCD I2C modullal)
* 1× Nyomógomb
* 1× RGB LED
* (közös katódos vagy közös anódos — kódtól függően)
* Szenzorok
* 1× Kapacitív talajnedvesség-érzékelő
* 1× DHT11 vagy DHT22 hőmérséklet- és páratartalom-érzékelő
* 1× LDR (fotóellenállás) vagy fényérzékelő modul
* Meghajtóelemek
* 1× Szervomotor (SG90 vagy kompatibilis)
* Egyéb szükséges elemek
* Ellenállások (LDR-hez és/vagy RGB LED-hez)
* Breadboard és jumper kábelek
* 5V tápellátás (USB vagy külső adapter)
