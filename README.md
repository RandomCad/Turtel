Main Page {#mainpage}
=========
# Turtel
Program to run a derivative of the Turtle Programming Language designed for young learners.
The following documentation will be done in german.

# Dokumentation

Dieses Projekt ist die Umsetzung der Aufgabe aus der Vorlesung „Compilerbau” an der DHGE. 
Die Festlegungen zur Sprache sowie die Aufgabe selbst sind [hier](https://www.computerix.info/comp-bau/turtle.pdf) zu finden. 
Dieses Projekt verfolgt einen leicht anderen Ansatz als in der Aufgabenstellung vorgesehen. 
Ziel ist die Erstellung eines Transpilers von der Turtle-Sprache zu C und das Compilieren dieses mit Hilfe eines üblichen C-Compilers. 
Als grafische Bibliothek soll SDL2 verwendet werden. 
Der Transpiler selbst soll keine Bestandteile von SDL2 beinhalten. 
Einzige Ausnahme ist die mitgelieferte Unit-Test-Umgebung. 
Dieser kann zur Überprüfung der korrekten Arbeitsweise SDL2 nutzen.


## Abhängigkeiten

Das Projekt ist ausschließlich unter Linux getestet und bekanterweise arbeitsfähig. 
Die folgenden `apt` packeges werden unter debian zur erfolgreichen Compilierung und ausführung benötigt. 
Bei anderen Distributionen müssen entsprechende Äquvalente Packete instaliert werden. 
Eine Internet verbindung ist ebenfals zum Comüilieren nötig, da zusätzliche bestandteile nachgeladen werden.
Folstädigkeit wird nicht garantiert.

### Pipline

Für das Compilieren werden folgende Pakete benötigt:

* git
* cmake
* C++-Compiler (gcc is fully tested clang not)
* Java (17+)
* Doxygen

### Includes/Libs

Folgende Bibliotheken werden zum linken benötigt:

* mindestens llvm-19-dev
* mindestens libclang-19-dev
* mindestens clang-19
* libsdl2-dev
* libsdl2-image-dev

Alle LLVM- und Clang-Bibliotheken sind auch in Version 20 getestet.

## Compilation

Zum Compilieren und Testen des Projektes wird das folgende Verfahren empfohlen:

1. Erstellen eines eigenen Build-Directorys: mkdir build
2. CMake ausführen: cmake ..
    * Zusätzliche Optionen wie -DCMAKE_BUILD_TYPE sind hier möglich. 
    * Warnungen können momentan vernachlässigt werden.
3. make aufrufen: make
4. Optional Docu erstellen: make doc
5. Optional UnitTests ausführen: ./Tests oder make test

Es gibt noch keinen Install-Befehl.

## Ablauf

Es soll folgender Ablauf umgesetzt werden:

```
TurtelFile -> ThisProjekt -> Binary
```

Dieser Aufbau vermittelt das Gefühl, dass es sich um einen vollwertigen Compiler handelt. 
Letztendlich hängt dies von der Sichtweise ab. 
Intern soll Folgendes umgesetzt werden:

```
TurtelFile -> ANTLR-Lexer -> ANTLR-Parser -> C-CodeGen -> Clang -> Binary
```

Wie an diesem detaillierten Ablauf zu erkennen ist, soll als Backend Clang verwendet werden. 
Dies soll möglichst vollständig durch die Verwendung der Clang-API geschehen. 
Dadurch entsteht der Eindruck, dass Trutel ein Compiler wäre, wobei die selbst entwickelten Bestandteile jedoch nur einen Transpiler bilden.
Als Frontend wird ein von ANTLR generierter Parser verwendet.

### ANTLR

Der Parser und der Lexer dieses Projekts wurden nicht von Hand geschrieben. 
Stattdessen wurde ANTLR4 als Parser- und Lexer-Generator verwendet. 
Im Folgenden wird immer von „ANTLR” gesprochen. Damit ist stets ANTLR4 gemeint.

ANTLR arbeitet auf Basis einer Grammatik, die in einer separaten Datei im ANTLR-Format definiert werden muss. 
Dieses Format ähnelt Yacc und Flex und ist auch an die EBNF angelehnt. 
Auf Basis dieser Grammatik erzeugt ANTLR einen Lexer, einen Parser und alle Strukturen, die zur Erzeugung eines Syntaxbaums nötig sind. 
Das Ergebnis des ANTLR-Parsers ist kein AST, sondern ein ST (Syntax Tree).

Zusätzlich ermöglicht ANTLR die Erstellung von zwei Arten von Baum-Betrachtern. 
Dabei ist es möglich:

1. einen Listener zu verwenden. Dieser kann jeweils über das Betreten und Verlassen einer Regel beim Parsen informiert werden.
2. einen Visitor zu verwenden. Dieser kann zur Betrachtung des ST verwendet werden.

Das Projekt verwendet beide Optionen. Dabei wird ein Listener für das Parsen der Commandline-Optionen und ein Visitor für die Verarbeitung des erzeugten ST aus dem Turtle-Programm verwendet.

Mit ANTLR können Parser und Lexer für eine Vielzahl von Sprachen erzeugt werden. 
In diesem Projekt wurde C++ verwendet. 

ANTLR erzeugt Parser nach dem ALL(\*)-(Algorithmus). 
Das bedeutet, dass der Parser mit theoretisch unendlich vielen Zeichen Vorschau arbeitet. 
Da die Turtelsprache sehr einfach ist, sollte ANTLR größtenteils mit nur einem Zeichen Vorschau entscheiden können, um welche Regel es sich handelt. 
Teilweise wurde jedoch die Flexibilität des Parsers gezielt ausgenutzt. 
So wurden viele Sonderbefehle in Turtle wie etwa die Definition der Commandline-Variablen oder der eingebauten mathematischen Funktionen über besondere Einzelregeln abgehandelt. 
Diese Flexibilität war bei der Arbeit mit ANTLR teils sehr hilfreich.

### Besonderheiten der Verwendeten Grammatik

Die Verwendete Gramatick für das Turtel Programm ergibt sich zu großen teilen aus der Definition in der Aufgaben stellung folgende veränderungen sind Interresant:

* Verwendung von Spezialregeln zur Abhandlung von Sonderfällen. Dies wurde bereits im forheriegen kapitel erklärt
* Erweiterungen werden im nachfolgenden Kapitel behandelt
* Verwendung der benanten Optionen: Es ist möglich mehrere Optionen einer Regel in ANTLR mit `#` unterschiedlich zu benenen. Dadurch werden weitere abgeleitete Kalssen erzeugt.

### Erweiterungen der Sprache

Die Ursprüngliche Turtelsprache wurde um folgende Funktionen beraupt:
* Das Schreiben auf die Delay Variable hat keinen effect. 
Aus komüatibilitäts grunden existiert sie dennoch doch sie hat keinen effect.
Das Kompilat soll so schnell wie möglich sein. Daher wäre die Existenz eines Delays nachteilhft.

Die Sprache wurde um folgenden Funktionen erweitert:
1. Save-Befehl. Mit diesem kann der Actuelle bildschirm status in eine png-Datei gespeichert werden.
2. Angabe eines Return Falues. Alle befehle zum enden des Programmes erlauben das Angeben eines Optionalen Returncodes. Dieser ergibt sich über einfaches abrunden des Angegebenen Double values.
3. Es sind grenzenlos viele Comandline Optionenen zulässig. Diese werden hingegen mit 0 indiziert.
4. Warnung for unendlichen schleifen. Wird eine Unendliche schleife bei der Analys des Programmes erkannt wird for dieser gewarnt. Diese Warnung kann durch zusätzlich optionen Deaktiviert werden. Dabei ist es sowohl möglich anzugeben, dass Unendliche schleifen immer ein Fehler sind oder immer akzeptabel sind.

### Nicht umgesetzte Funktionen

Es wurden alle Funktionen umgesetzt. Der einzige Unterschied zur Vorgabe sind die vordefinierten Variablen. Bei diesen wurden bisher die folgenden nicht umgesetzt:

* @dist: Wurde in keinem Beispiel verwendet und deswegen nicht umgesetzt. Sie wäre als Sonderregel ähnlich zu PI umzusetzen.
* @delay: Die Umsetzung dieser Variable widerspricht dem Prinzip, dass das Compilat mit maximaler Geschwindigkeit arbeiten soll.

Beide Variablen existieren und können verwendet werden. Sie haben jedoch nicht den erwarteten Effekt.

Alle weiteren Funktionen sind umgesetzt.

## Besondere Herausforderungen

Nachfolgend soll dargestellt werden, welche Teile des Programms sich in der Entwicklung als besonders schwierig herausgestellt haben. 

### LLVM und CLANG-Interface

Die erste besonders hohe Hürde bei der Entwicklung war die Erstellung einer Abstraktion der Clang-API. 
Wie aus den bisherigen Ausführungen ersichtlich sein sollte, ist das Ziel des Projekts, Clang direkt als API zu verwenden. 
Im Sommer 2024 wurde die entsprechende Schnittstelle programmiert. 
Konkret geht es um die Klasse LLVMInterface. 
Sie kapselt alle benötigten Handlungen für die Interaktion mit Clang. 
Dazu muss zunächst eine Datei für die Aufnahme des generierten Codes erstellt werden. 
Der Code muss kompiliert werden. Schlussendlich müssen temporäre Dateien aufgeräumt werden. Auch die Verwaltung der Compileroptionen ist Aufgabe der Klasse.
Die tatsächliche Implementierung der API-Verwendung wurde größtenteils aus dem Internet übernommen. Die Quelle ist leider nicht mehr bekannt. 
Da die Quelle mit einer älteren Version als der hier verwendeten arbeitet und ein anderes Ziel verfolgt, musste der Code abgeändert werden. 
Besonders schwierig war es herauszufinden, an welchen Stellen welche Elemente erzeugt werden müssen. Die ersten Versuche erzeugten häufig Segfaults in den Tiefen von Clang. 
Die genaue Arbeitsweise ist bis heute nicht vollständig verständlich. 
Da diese Schnittstelle jedoch seit eineinhalb Jahren kaum verändert wurde und fehlerfrei Arbeitet, wird ihr großes Vertrauen entgegengebracht.

Auch das dynamische Erfassen der Linker- und Compileroptionen für SDL2 erwies sich als schwierig. 
Leider musste hierfür der Umweg über die CLI genommen werden. Ein direkterer Aufruf wäre wünschenswert. 
Wie dieser aussehen könnte, ist jedoch nicht bekannt. 
Da der bestehende Ansatz funktioniert und Effizienz an dieser Stelle noch keine Priorität hat, wird er nicht optimiert.

### Unit-Testing

Ein großer Bestandteil und Hauptaufwand ist der umfangreiche Unit-Test. 
Dabei gibt es etwa dreimal so viele Zeilen Unit-Test-Code wie Anwendungscode. 
Es ist anzumerken, dass es einen sehr großen Wiederholungsanteil gibt.

Seit dem Commit e6c31b0 wird Google Test als Unit-Test-Framework verwendet. 
Die Verwendung dieses Frameworks und die damit verbundene Ablösung des eigenen Test-Frameworks war eine kluge Entscheidung. 
Nach dem Umstieg konnte eine erhebliche Beschleunigung der Testentwicklung festgestellt werden. 
Gleichzeitig war die Entwicklung des Unit-Test-Frameworks ein lehrreiches und spaßiges Unterfangen. 
Dennoch war dessen Fehleranfälligkeit zu hoch.
 

### ST-Umschreiben

Zwischenzeitlich wurde versucht, bestimmte Befehle durch das Umschreiben des ST umzusetzen. 
Hierfür würden sich beispielsweise die Befehle `walck back` oder `turn left` anbieten. 
Da der ST jedoch nicht dazu gedacht ist, geändert zu werden – da dies de facto einer Änderung der Quelldatei gleichkommt –, war dies nicht möglich. 
Schlussendlich wurde von diesem Vorgang abgesehen. 
Die Idee ist nach wie vor gut und würde einen Teil der Arbeit an anderer Stelle einsparen.

### SDL2

Es wurde entschieden, kein bestehendes SDL2-Interface zu verwenden. 
Stattdessen wird dieses direkt in den erzeugten Code hineincompiliert. 
Überraschend war hierbei die Erkenntnis, dass SDL2 zwei interne Backbuffer verwendet und diese kontinuierlich austauscht. 
Deshalb musste ein eigener interner Backbuffer verwendet werden, auf dem gearbeitet werden kann. 
Dies ist jedoch langfristig nicht effizient, da das Anzeigen jeweils das Kopieren des gesamten Bildschirminhalts erfordert. 
Momentan ist dies vertretbar. 
Gleichzeitig wurde der Save-Befehl dadurch leicht vereinfacht.

### Allgemeine Probleme

Es ist ärgerlich, dass GDB den ST nicht korrekt auflösen kann. 
Dadurch wird das Debugging häufig schwieriger als nötig, da der ST-Aufbau nicht ordentlich erkannt werden kann.

Ein weiterer Punkt, der nervt, ist, dass der von ANTLR erzeugte Visitor ausschließlich std::any zurückgeben kann. 
Dadurch wird viel Code zur Unterscheidung unterschiedlicher Rückgabetypen benötigt. 
Eine ordentliche Typisierung der Rückgabewerte wäre zwar angenehmer, gleichzeitig aber technisch erheblich schwieriger.

Zwischenzeitlich wurden die Aufgaben auf zu viele Klassen verteilt. 
Die Auflösung und Bereinigung der daraus resultierenden zyklischen Includes kostete etwa sechs Stunden.
