# Turtel
Program to run a derivative of the Turtle Programming Language designed for young learners.
The following documentation will be done in german

# Dokumentation

Diese Projekt ist eine Umsetzung der Aufgabe im Rahmen der Forlesung Compilerbau an der DHGE. 
Die Festlegungen zur Sprache sowie die Aufgabe sind [hier](https://www.computerix.info/comp-bau/turtle.pdf) zu finden. 
Dieses Projekt nimmt einen leicht anderen weg als die in der Aufgabenstellung vorgeschlagenen wege. 
Ziel soll die erstellung eines Transpilers von der Turtel Sprache zu C sein. 
Als graphische bibliothek soll das endsystem SDL2 verwenden. 
Der Transpiler selbst soll keine Bestandteile von SDL2 beinhlaten. 
Einzige ausnahme ist der mitgeliverte UnitTest umfang. 
Dieser kann zur überprüfung der korrekten Arbeitsweise SDL2 verwenden.  

## Dependency

* Clang
* SDL2
* SDL2-Immage
* CMake
* Java (13+(?))
* c++17
* git

## Compilation

Eigenes Bulddirectory erstellen:  

`mkdir build` 

CMake ausführen:

`cmake ..`
Warnungen können momentatan vernachlässigt werden.  

make oder ninja (nur bei weiterer Configuration) ausführen:

`make`  
`ninja` 

Test ausführen.

`./Tests`

## Ablauf

Folgender Ablauf soll umgesetzt werden:

```
TurtelFile -> ThisProjekt -> Binary
```

Dieser Aufbau gibt das gefühl, dass es sich um einen Folständiegen Compiler handelt. 
Dies hängt schlussendlich von der Sichtweise ab. 
Intern soll folgendes Umgesetzt werden:

```
TurtelFile -> ANTLR-Lexer -> ANTLR-Parser -> C-CodeGen -> Clang -> Binary
```

Wie an diesem detailierten ablauf zu erkennen ist, soll als backend Clang verwendet werden. 
Dies soll möglichst folständig durch die verwendung der Clang-API geschehen. 
Auf diese wird später weiter eingegangen.
Als Front end wird ANTLR verwenden. 

### ANTLR

ANTLR ist ein Compiler generator. 
ANTLR erzeugt dabei ausschließlich Lexer und Parser. 
In diesem Projekt wurde ANTLR4 verwendet.
Es ist möglich Parser und Lexer in unterscchiedlichsten Sprachen mit ANTLR zu erzeugen. 
Dieses Projekt ist soweit in c++ geschreiben. 
Es wird demnach das Aktuelste ANTLR framwork verwendet.  

Wie bereits angegeben erzeugt ANTLR einen Lexer und Parser. 
Diese erzeugen ihrereseits einen AST. 
Dieser AST kann mit hilfe von zwei von ANTLR bereitgestellten mechanismen verarbeitet werden. 
In diesem Projekt wird ausschließlich das Visitor Pattern zur verarbeitung des AST verwendet.  

ANTLR erzeugt Parser nach dem ALL(\*) algoritmuss. 
Da die Turtelsprachen LL(1) kompatibel sein sollte, sollte der erzeugete Parser ebenfals LL(1) sein. 
Für die hinzugefügeten bestandteile kann dies natürlich nicht grantiert werden. 
Dennoch wird ein LL(1) Parser angenommen.
Die Arbeitsweise des Erzeugten Parsers ist irrelevant, da die erzeugung des Parsers durch ANTLR durchgeführt wird und die eigenen Entwicklung keine nenenswerten Schnitstellen zu diesem besitzt.
Neben dem Aufrufen des Parser interresiert sich die selbstentwickelten bestandteil nicht für innerenfunktionsweisen des Parsers. 
Einzig die Deklaration der AST-Knoten ist für die eigene Entwicklung höchst relevant. 

## Besondere schwierigkeiten

Das API-Interface mit LLVM wurde im Sommer 2024 geschreiben. 
Die entwicklung dieses Bestandteiles wahr besonders schwierig und vergleichsweise Zeitaufwändig. 
Hauptproblem lag hier in dem schlechtem verständniss welche Struckturen wie erzeugt werden müssen. 
Die umsetzung abssiert sehr stark auf einer übernahme aus dem internet, wobei die Quelle nicht mehr bekannt ist. 
Die genaue funktionsweise wird dabei immernoch nicht verstanden. Da sie aber bereits über mehrere LLVM-Versionen funktioniert besteht großes vertrauen in sie. 
Auch das dynamische erfassen der Linker und Comilereroptionen für SDL2 war nicht trivial. 
Leider musste hiefür der umweg über die CLI gegangen werden. 
Ein direkterer aufruf wäre wünschenswert. 
Wie dieser umgesetzt werden könnte ist hingegen nicht bekannt. 
Da der bestehende Ansatz funktioniert und Effizienz an dieser stell momentan noch keine Priorität ist, wird dieser nicht optimiert.

Seite dem Commit [e6c31b0](https://github.com/RandomCad/Turtel/commit/e6c31b0cf2c4676db9e5708714149faf89fdc43e) wird google-test als UnitTest framwork verwendet. 
Die verwendung dieses und die Damit verbundene ablösung des Eigenen Test Framworks war eine Kluge entscheidung, da dies erhebliche Zeit einsparungen bedeuten. 
Gleichzeitig war die entwicklung eines UnitTest-Framworkes ein Spaßieges unterfangen und lehrreich. 
Dennoch war die Fehleranfälligkeit nicht positiv.  

Der momentane versuch des Umschreibens des AST gestalten sich ebenfals schwirig. 
Besonders Problematisch ist dabei das GDB nicht zur korrekten auflösung von AST Variablen fähig ist. 
Zumindest ist dies mit den momentanen CMake settings nicht ordentlich möglich. 
Das verständniss der Erzeugung von AST-konten ist momentan noch nicht hoch genug um einen Rewrite zufriedenstellend umzusetzen.

Nervig an ANTLR ist, dass die erzeugten Vifitor auschlißlich std::any als rückgabe wert haben können. 
Eine ordentlich Typisierung der rückgabe werten wäre angenehmer gelichzeitig aber Technisch erheblich schwieriger. 

Die nicht verwendung und eigene Implementierung vom erzeugten SDL-Interface war ebenfals etwas Zeitaufwändig. 
Im vergleich zum LLVM-Intervace aber erheblich einfacher. 
Gerade die erstellung eines eigenen Screen Buffers war etwas aufwändig. 
Gleichzeitig ermöglichte dies die umsetzung des `save` befehles.

## Erweiterungen
### Save

Der `save` befehl ermöglicht das Speicher des Momentanen Programstandes in eine `.png`-Datei. 
Die umsetzung diese war nach der Implementierung eines eingene Front-buffers, welcher für die generelle funktionsweise benötigt wird, recht einfach. 
Hierfür wurde der Code von [Czipperz](https://stackoverflow.com/users/1692584/czipperz) antwort auf eine entsprechende [stackoverflow Frage](https://stackoverflow.com/questions/34255820/save-sdl-texture-to-file) verwendet. 
Dieser Funktioniert sehr gut und die Integration war sehr einfach.
