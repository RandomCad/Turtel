# Turtel
Program to run a derivative of the Turtle Programming Language designed for young learners.
The following documentation will be done in german.

# Dokumentation

Diese Projekt ist eine Umsetzung der Aufgabe im Rahmen der Vorlesung Compilerbau an der DHGE. 
Die Festlegungen zur Sprache sowie die Aufgabe sind [hier](https://www.computerix.info/comp-bau/turtle.pdf) zu finden. 
Dieses Projekt nimmt einen leicht anderen Weg als die in der Aufgabenstellung vorgeschlagenen Wege. 
Ziel soll die Erstellung eines Transpilers von der Turtel Sprache zu C sein. 
Als graphische Bibliothek soll das Endsystem SDL2 verwenden. 
Der Transpiler selbst soll keine Bestandteile von SDL2 beinhlaten. 
Einzige Ausnahme ist der mitgeliferte UnitTest Umfang. 
Dieser kann zur Überprüfung der korrekten Arbeitsweise SDL2 verwenden.  

## Dependency

on debian:

### Pipline

* git
* cmake
* C++-Compiler (gcc is fully tested clang not)
* Java (17+)
* Doxygen

### Includes/Libs

* llvm-19-dev
* libclang-19-dev
* Clang-19
* libsdl2-dev
* libsdl2-image-dev



* Clang-19
* SDL2
* SDL2-Immage
* CMake
* Java (17+)
* c++17
* git
* libclang-19-dev
* llvm-19
* llvm-19-dev
* libsdl2-image-dev

## Compilation

Eigenes Build directory erstellen:  

`mkdir build` 

CMake ausführen:

`cmake ..`
Warnungen können momentatan vernachlässigt werden.  

make oder ninja (nur bei weiterer Configuration) ausführen:

`make` / `ninja` 

Test ausführen.

`./Tests`

## Ablauf

Folgender Ablauf soll umgesetzt werden:

```
TurtelFile -> ThisProjekt -> Binary
```

Dieser Aufbau gibt das Gefühl, dass es sich um einen vollständigen Compiler handelt. 
Dies hängt schlussendlich von der Sichtweise ab. 
Intern soll folgendes umgesetzt werden:

```
TurtelFile -> ANTLR-Lexer -> ANTLR-Parser -> C-CodeGen -> Clang -> Binary
```

Wie an diesem detaillierten Ablauf zu erkennen ist, soll als Backend Clang verwendet werden. 
Dies soll möglichst vollständig durch die Verwendung der Clang-API geschehen. 
Auf diese wird später weiter eingegangen.
Als Frontend wird ANTLR verwendet. 

### ANTLR

ANTLR ist ein Compiler Generator. 
ANTLR erzeugt dabei ausschließlich Lexer und Parser. 
In diesem Projekt wurde ANTLR4 verwendet.
Es ist möglich Parser und Lexer in unterschiedlichsten Sprachen mit ANTLR zu erzeugen. 
Dieses Projekt ist soweit in C++ geschreiben. 
Es wird demnach das aktuellste ANTLR Framwork verwendet.  

Wie bereits angegeben, erzeugt ANTLR einen Lexer und Parser. 
Diese erzeugen ihrereseits einen AST. 
Dieser AST kann mit Hilfe von zwei von ANTLR bereitgestellten Mechanismen verarbeitet werden. 
In diesem Projekt wird ausschließlich das Visitor Pattern zur Verarbeitung des AST verwendet.  

ANTLR erzeugt Parser nach dem ALL(\*) Algorithmus. 
Da die Turtelsprachen LL(1) kompatibel sein sollte, muss der erzeugete Parser ebenfalls LL(1) sein. 
Für die hinzugefügeten Bestandteile kann dies natürlich nicht garantiert werden. 
Dennoch wird ein LL(1) Parser angenommen.
Die Arbeitsweise des erzeugten Parsers ist irrelevant, da die Erzeugung des Parsers durch ANTLR durchgeführt wird und die eigenen Entwicklung keine nenenswerten Schnittstellen zu diesem besitzen.
Neben dem Aufrufen des Parser interresiert sich die selbstentwickelten Bestandteil nicht für die inneren Funktionsweisen des Parsers. 
Einzig die Deklaration der AST-Knoten ist für die eigene Entwicklung höchst relevant. 

## Besondere schwierigkeiten

Das API-Interface mit LLVM wurde im Sommer 2024 geschreiben. 
Die Entwicklung dieses Bestandteiles wahr besonders schwierig und vergleichsweise zeitaufwändig. 
Hauptproblem lag hier in dem schlechtem Verständnis, welche Strukturen wie erzeugt werden müssen. 
Die Umsetzung basiert sehr stark auf einer Übernahme aus dem Internet, wobei die Quelle nicht mehr bekannt ist. 
Die genaue Funktionsweise wird dabei immernoch nicht verstanden. Da sie aber bereits über mehrere LLVM-Versionen funktioniert besteht großes Vertrauen in diese. 
Auch das dynamische Erfassen der Linker und Comilereroptionen für SDL2 war nicht trivial. 
Leider musste hiefür der Umweg über die CLI gegangen werden. 
Ein direkterer Aufruf wäre wünschenswert. 
Wie dieser umgesetzt werden könnte ist hingegen nicht bekannt. 
Da der bestehende Ansatz funktioniert und Effizienz an dieser Stelle momentan noch keine Priorität ist, wird dieser nicht optimiert.

Seite dem Commit [e6c31b0](https://github.com/RandomCad/Turtel/commit/e6c31b0cf2c4676db9e5708714149faf89fdc43e) wird google-test als UnitTest Framwork verwendet. 
Die Verwendung dieses und die damit verbundene Ablösung des eigenen Test Framworks war eine kluge Entscheidung, da dies erhebliche Zeiteinsparungen bedeuten. 
Gleichzeitig war die Entwicklung eines UnitTest-Framworkes ein spaßieges Unterfangen und lehrreich. 
Dennoch war die Fehleranfälligkeit nicht positiv.  

Die momentanen Versuche des Umschreibens des AST gestalten sich ebenfalls schwierig. 
Besonders problematisch ist dabei das GDB nicht zur korrekten Auflösung von AST Variablen fähig ist. 
Zumindest ist dies mit den momentanen CMake Settings nicht ordentlich möglich. 
Das verständnis der Erzeugung von AST-konten ist momentan noch nicht hoch genug um einen Rewrite zufriedenstellend umzusetzen.

Nervig an ANTLR ist, dass die erzeugten Visitor auschlißlich std::any als Rückgabewert haben können. 
Eine ordentlich Typisierung der Rückgabewerte wäre angenehmer, gleichzeitig aber technisch erheblich schwieriger. 

Die nicht Verwendung und eigene Implementierung vom erzeugten SDL-Interface war ebenfalls etwas zeitaufwändig. 
Im Vergleich zum LLVM-Intervace aber erheblich einfacher. 
Gerade die Erstellung eines eigenen Screen Buffers war etwas aufwändig. 
Gleichzeitig ermöglichte dies die umsetzung des `save` Befehles.

## Erweiterungen
### Save

Der `save` Befehl ermöglicht das Speichern des momentanen Programmstandes in eine `.png`-Datei. 
Die Umsetzung war nach der Implementierung eines eingenen Frontbuffers, welcher für die generelle Funktionsweise benötigt wird, recht einfach. 
Hierfür wurde der Code von [Czipperz](https://stackoverflow.com/users/1692584/czipperz) antwort auf eine entsprechende [stackoverflow Frage](https://stackoverflow.com/questions/34255820/save-sdl-texture-to-file) verwendet. 
Dieser funktioniert sehr gut und die Integration war sehr einfach.


Zyklische Dependencys im Function system haben sicherlich 2-3h Zeit geraubt.

### Doxygen

cmake --build . --target doc

Mit diesem Befehl kann die API-Dokumentation generiert werden
