|Programm|Content|
|---------|-------|
|a2_4.cpp    |choas game|

Die zweidimensionalen Koordinaten der Eckpunkte eines gleichseitigen Dreiecks D sind bekannt: D = {(95, 435), (320, 45), (545, 435)}. 
Welche Ausgabe entsteht, wenn man beliebig vie Pixel in der Farbe an der Position 15 der Pallette nach folender Regel setzt:

0) 
Zufällige Auswahl einer der drei Punkte und Zuweisung dieses Punktes an 'act_point'

1) Zufällige Auswahl eines weiteren Punktes des Dreiecks. Ermitteln des Punktes 'm', der sich in der Mitte der Strecke zwischen diesm Punkt und 'act_point' befindet.
Der Punkt 'm' wird auf den Bildschirm gezeichnet

2) Die Koordinaten des Punktes 'm' werden 'act_point' zugewiesen. 
Die Anweisungen aus Schritt 1) werden wiederholt

# Compile

`make.sh <file.cpp>`

# Run

`application`