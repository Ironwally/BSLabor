## Nr. 1)
### a)
```
Es gilt Little-Endian by .bmp Dateien, bei A.bmp wäre Dateigröße so:
0x00 0x00 0x01 0x78 (4 Bytes für Dateigröße aus 1tem Header (File Header))
```

### b)
```
A.bmp BITMAPINFOHEADER (2ter Header):
Header-Größe (4 Bytes): 0x00 0x00 0x00 0x28
Bildbreite (4 Bytes, signed): 0x00 0x00 0x00 0x0A
Bildhöhe (4 Bytes, signed): 0x00 0x00 0x00 0x0A
Anzahl der Farbebenen (2 Bytes): 0x00 0x01
Bits pro Pixel (2 Bytes): 0x00 0x18
Kompressionsmethode (4 Bytes): 0x00 0x00 0x00 0x00
Bildgröße (Größe des Bildbereichs in Bytes) (4 Bytes): 0x00 0x00 0x01 0x42
Horizontale Auflösung (DPI oder PPM) (4 Bytes, signed): 0x00 0x00 0x1E 0xC2
Vertikale Auflösung (DPI oder PPM) (4 Bytes, signed): 0x00 0x00 0x1E 0xC2
Anzahl der Farben in der Palette (4 Bytes): 0x00 0x00 0x00 0x00
Anzahl der wichtigen Farben (4 Bytes): 0x00 0x00 0x00 0x00
```

### c)
```
C_1.bmp:
Bildbreite: 0x00 0x00 0x00 0x0C
Bildhöhe: 0x00 0x18 0x00 0x01

C_2.bmp:
Bildbreite: 0x0FF 0xFF 0xFF 0xF4
Bildhöhe: 0x00 0x18 0x00 0x01

Bildhöhe ist bei beiden .bmp Dateien gleich, aber die Bildbreite ist unterschiedlich.
Es könnte sein, dass ein Fehler bei der C_2.bmp Datei vorliegt, da die Bildbreite verdächtig
aussieht.
```

### d)
```
C_3.bmp BITMAPINFOHEADER (2ter Header):
- Die Dateigröße ist viel kleiner (112 Bytes gegenüber 488 Bytes).
- Die Höhe des Bildes ist negativ (-12 statt 12), was bedeutet, dass das Bild von 
oben nach unten gespeichert wird.
- Die Anzahl der Bits pro Pixel ist 1 statt 24, was bedeutet, dass das Bild monochrom ist 
und nur zwei Farben hat.
- Die Farbpalette enthält nur zwei Einträge: Schwarz (0x00) und Weiß (0xFF).
- Das Bild ist komprimiert, da die biSizeImage kleiner ist als die erwartete Größe für 
ein 12x12 Pixel großes Bild (32 Bytes statt 432 Bytes).
```

## Nr. 2)
### a)
```
A.bmp BITMAPINFOHEADER (2ter Header):
Bildbreite (4 Bytes, signed): 0x00 0x00 0x00 0x0A
Bildhöhe (4 Bytes, signed): 0x00 0x00 0x00 0x0A

B.bmp BITMAPINFOHEADER (2ter Header):
Bildbreite (4 Bytes, signed): 0x00 0x00 0x00 0x09
Bildhöhe (4 Bytes, signed): 0x00 0x00 0x00 0x09

Bildbreite und Bildhöhe sind im BITMAPINFOHEADER.
A.bmp hat eine Breite und Höhe von 10 Pixeln, B.bmp hat eine Breite und Höhe von 9 Pixeln.
Pixeldaten beginnen nach Daten-Offset, der im BITMAPINFOHEADER ist.
Bei beiden ist Daten-Offset 54 Bytes (0x00 0x00 0x00 0x36), was bedeutet, dass Pixeldaten direkt 
nach dem BITMAPINFOHEADER beginnen.

Pixeldaten sind in BGR-Format (Blue-Green-Red) gespeichert, d.h. jedes Pixel besteht aus drei Bytes, 
die die Intensität der Blau-, Grün- und Rot-Komponente angeben. 
Pixeldaten sind zeilenweise von unten nach oben und von links nach rechts gespeichert. 
Das heißt, das erste Pixel in Pixeldaten ist das untere linke Pixel im Bild, das zweite Pixel 
ist das Pixel rechts daneben, usw. 
Das letzte Pixel in den Pixeldaten ist das obere rechte Pixel im Bild.

Da Pixeldaten eine Vielfache von vier Bytes lang sein müssen, werden am Ende jeder Zeile 
eventuell Padding-Bytes hinzugefügt, um diese Bedingung zu erfüllen. 
Anzahl Padding-Bytes pro Zeile kann aus Bildbreite und Anzahl der Bits pro Pixel berechnet werden. 
Bei beiden sind die Bits pro Pixel 24 (0x00 0x18), was bedeutet, dass jedes Pixel 3 Bytes belegt. 
Anzahl der Padding-Bytes pro Zeile ist dann 4 - (Bildbreite * 3) % 4. 
Für A.bmp: 4 - (10 * 3) % 4 = 2 Padding-Bytes pro Zeile. 
Für B.bmp: 4 - (9 * 3) % 4 = 3 Padding-Bytes pro Zeile.

Auffallend ist, dass Pixeldaten von A.bmp und B.bmp sehr ähnlich sind, obwohl Bildgrößen 
unterschiedlich sind. 
Liegt daran, dass die Bilder die gleichen Farben haben, nur in unterschiedlicher Anordnung. 
A.bmp hat vier Reihen blaue Pixel, gefolgt von drei Reihen grüne Pixel und drei Reihen rote Pixel. 
B.bmp hat drei Reihen blaue Pixel, gefolgt von drei Reihen grüne Pixel und drei Reihen rote Pixel. 
Pixeldaten von A.bmp sind länger als die von B.bmp, weil A.bmp mehr Pixel und mehr Padding-Bytes hat.

(BGR-Wert von 0x00, 0x00, 0xFF entspricht blauen Pixel, 0x00, 0xFF, 0x00 entspricht grünen Pixel 
und 0xFF, 0x00, 0x00 entspricht roten Pixel.)
```

### b)
```
C_1.bmp und C_2.bmp:
Art und Weise, wie Farben dargestellt werden, von Farbtiefe des Bildes abhängig. 
Beide Bilder haben Farbtiefe von 24 Bit, bedeutet, dass jede Farbe durch 3 Bytes repräsentiert wird.

Computer kann Bild in beiden Fällen gleich darstellen, weil er die Pixeldaten liest und 
entsprechend der im Bitmap-Header angegebenen Infos interpretiert. 
Unabhängig davon, wie Pixeldaten gespeichert sind, solange Computer die im Header angegebenen 
Infos korrekt interpretiert, kann er Bild korrekt darstellen.

Bitmaps wurden so festgelegt, weil sie einfache und direkte Darstellung von Bildern ermöglichen. 
Jedes Pixel im Bild wird durch feste Anzahl von Bits repräsentiert, was Manipulation und 
Darstellung des Bildes vereinfacht. 
```

### c)
```
C_3.bmp:
Ist kleiner als C_1.bmp und C_2.bmp, weil eine andere Farbtiefe verwendet. 
C_3.bmp Farbtiefe von 1 Bit (0x00 0x01) verwendet, was bedeutet, dass jedes Pixel nur eine 
von zwei möglichen Farben haben kann. 
Reduziert Menge an Speicherplatz, die benötigt wird, um das Bild zu speichern, stark.

Computer kann feststellen, dass Bild komprimiert ist, indem er Wert des Daten-Offsets 
im Fileheader überprüft. 
In Bitmap-Datei gibt Daten-Offset an, wo Bilddaten im Dateispeicher beginnen.
Datenoffset bei C_1.bmp und C_2.bmp: 0x36 (54 Dezimal), Bilddaten beginnen ab Byte 54 und
bei C_3.bmp: 0x3E (62 Dezimal), Bilddaten beginnen ab Byte 62.
In C_3.bmp ist Daten-Offset kleiner als in C_1.bmp und C_2.bmp, was bedeutet, 
dass weniger Speicherplatz für Farbpalettenteil des Headers verwendet wird. 
Hinweis darauf, dass Bild eine geringere Farbtiefe hat und daher komprimiert ist.
```
