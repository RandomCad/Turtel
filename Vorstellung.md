### Vorstellung Turtle

## Unterschiede 

# Architektur und Zielsetzung

- Transpiler statt Interpreter
    Die ursprüngliche Aufgabenstellung beschreibt eine Turtle-Graphics‑Sprache, deren Befehle typischerweise direkt interpretiert werden würden – also ein Interpreter, der auf Basis von Lexer, Parser und einem Syntaxbaum die Zeichnungen ausführt. Unsere Umsetzung hingegen verfolgt einen Transpiler-Ansatz: Der Quellcode wird in C übersetzt, das dann mittels Clang kompiliert wird. Dadurch entsteht ein eigenständiges Binary.
    Dies stellt einen wesentlichen Architekturunterschied dar: Während die Aufgabenstellung ein direkt ausführbares System (Interpretation des AST) propagiert, setzt wir auf Zwischenschritte (Code-Generation → Clang) und nutzen damit das Clang-Backend.

# Technologiewahl und Implementierung

- Verwendung von ANTLR und LLVM/Clang-API

    Die Aufgabenstellung erwähnt zwar, dass entweder ein handgeschriebener rekursiver Parser oder Tools wie Lex+Yacc verwendet werden dürfen, legt aber Wert auf die strikte Trennung zwischen lexikalischer und syntaktischer Analyse. Unsere Umsetzung verwendet ANTLR4 als Frontend, um Lexer und Parser zu erzeugen, und integriert anschließend die Clang-API (bzw. nutzt Clang über CMake-Konfigurationen), um den generierten C-Code zu kompilieren.

- SDL2 als Grafik-Backend

    Anstatt einen eigenen Zeichenmodus oder ein Framework zu entwickeln, benutzt wir SDL2 (und SDL2_Image) als graphische Bibliothek, um das Turtle-Grafikverhalten darzustellen. Die Aufgabenstellung hinterlässt hier mehr Spielraum, während wir eine konkrete, moderne Lösung gewählt haben.

# Sprachumfang und Erweiterungen

- Umfang der Sprache vs. Erweiterungen

    Der in der Aufgabenstellung beschriebene Sprachumfang umfasst eine Vielzahl an Befehlen (z. B. walk, jump, turn left, direction, Zuweisungen, Schleifen, Funktionen, Markierungskommandos usw.). Unsere Implementierung setzt darauf, einen Transpiler zu entwickeln, der von Turtle zu C übersetzt.

        - Dabei wird die Sprache wohl als Teilmenge bzw. abgewandelte Variante implementiert, die speziell auf junge Lernende zugeschnitten ist.
        - Darüber hinaus wurde der Befehl save als Erweiterung hinzugefügt, um den aktuellen Programmstand als .png-Datei zu speichern – ein Feature, das in der Aufgabenstellung nicht erwähnt wird.

# Build- und Testinfrastruktur

- Modernes Build-System und Unit-Tests

    Unsere Implementierung verwendet CMake als Build-System, was insbesondere für C++-Projekte üblich ist. Außerdem habt wir Google Test (über einen Wechsel vom eigenen Testframework) integriert, um die Korrektheit der Implementierung zu überprüfen.

# Besondere Herausforderungen und technologische Hürden

- Integration mit LLVM/Clang

    Die Arbeit mit der Clang‑API und das dynamische Erfassen von Linker- sowie Compileroptionen für SDL2 zu erheblichen Schwierigkeiten geführt.

- AST-Verarbeitung und Debugging

    Das Debuggen des generierten AST ist derzeit problematisch ist, da die ANTLR‑Visitor ausschließlich std::any als Rückgabetyp verwenden.

---

#### ANTLR Grammatik

### Grundaufbau einer ANTLR-Grammatik

ANTLR-Grammatiken beinhalten im Allgemeinen zwei wesentliche Teile:

1. **Lexer-Regeln (Token-Regeln):**  
   Hier werden die kleinstmöglichen Einheiten (Token) definiert, die dann vom Parser verarbeitet werden. Lexer-Regeln sind meist in Großbuchstaben geschrieben (z. B. `WALK`, `SAVE`, `NUM` etc.) und beschreiben, welche Zeichenfolgen als ein bestimmter Token erkannt werden.

2. **Parser-Regeln (Grammatikregeln):**  
   Diese Regeln definieren, wie die zuvor identifizierten Token zu höheren Struktureinheiten (Produktionen) zusammengefügt werden. Die Parser-Regeln sind in Kleinbuchstaben geschrieben und definieren die Syntax der Sprache.

---

### Übersicht der Beispielgrammatik „Scene“

Die angegebene Grammatik heißt `Scene` und ist so entworfen, dass sie eine vereinfachte Sprache für Turtle-Grafik-Befehle beschreibt. Im Folgenden wird erklärt, wie die einzelnen Bestandteile aufgebaut sind.

## 1. Lexer-Regeln

Die ersten Zeilen der Grammatik enthalten die Definition von Schlüsselwörtern, Operatoren und anderen grundlegenden Token. Beispiele:

- **Befehle als Schlüsselwörter:**  
  ```antlr
  Walk      : 'walk' ;
  Save      : 'save' ;
  Back      : 'back' ;
  Jump      : 'jump' ;
  Home      : 'home' ;
  Left      : 'left' ;
  Turn      : 'turn' ;
  Stop      : 'stop' ;
  Right     : 'right' ;
  Clear     : 'clear' ;
  Finish    : 'finish';
  Direction : 'direction' ;
  Mark      : 'mark' ;
  Store     : 'store' ;
  In        : 'in' ;
  Step      : 'step' ;
  While     : 'while' ;
  Untile    : 'untile';
  Begin     : 'begin' ;
  End       : 'end' ;
  Color     : 'color' ;

Diese Regeln teilen dem Lexer mit, dass genau diese Zeichenfolgen als Token des jeweiligen Typs erkannt werden. Die Großschreibung und genaue Übereinstimmung ist dabei ausschlaggebend.

- **Zahlen und Identifikatoren:**
```antlr
Num   : [0-9]+ ;
Float : [0-9]+ '.' [0-9]+
      | '.' [0-9]+ ;

ID    : [_a-zA-Z] [_@a-zA-Z0-9]* ;
CliID : '@' [0-9];
IncID : '@' [_@a-zA-Z0-9]* ;
```
- Num erkennt ganze Zahlen (eine oder mehrere Ziffern).

- Float erkennt Fließkommazahlen in zwei Varianten: Ziffern, gefolgt von einem Punkt und weiteren Ziffern, oder einem Punkt gefolgt von Ziffern.

- ID definiert gültige Bezeichner (Variablennamen), wobei das erste Zeichen ein Buchstabe oder Unterstrich sein muss und danach beliebig viele Zeichen aus Buchstaben, Ziffern, Unterstrich oder @ folgen dürfen.

- CliID und IncID definieren spezielle Formen von Identifikatoren, die global oder als Befehlszeilen-Parameter interpretiert werden.

- **Whitespace und Kommentare:**
```antlr
WS : [ \t\r\n]+ -> skip ;
COMMENT : '"' ~[\r\n]* -> skip ;
```
Die Regel WS fängt Leerzeichen, Tabs, Wagenrückläufe und Zeilenumbrüche ab und weist sie an, übersprungen zu werden. Ebenso werden Kommentare (alles, was nach einem Anführungszeichen bis zum Zeilenende kommt) ignoriert.

### 2. Parser-Regeln
Im Parser-Teil wird definiert, wie die Token zu einer höheren Struktur zusammengefügt werden.

# Struktur des Dateiinhalts
- **Datei und Hauptprogramm:**

    ```antlr
    file : (pathdef | calcdef)* main (pathdef | calcdef)*; 
    main : Begin statList End;
    ```
    Die oberste Regel file erlaubt, dass vor und nach dem eigentlichen Hauptprogramm (main) beliebig viele Funktions- bzw. Pfaddefinitionen (pathdef oder calcdef) stehen. Das Hauptprogramm selbst wird durch das  Schlüsselwort begin, gefolgt von einer Liste von Statements (statList), und abschließend end definiert.

- **Definitionen von Pfaden und Berechnungen:**

    ```antlr
    pathdef : 'path' ID paramlist? statList 'endpath' ;
    calcdef : 'calculation' ID paramlist statList 'returns' expr 'endcalc' ;
    paramlist : '(' ( (var ',')* var )? ')';
    ```
    Diese Regeln definieren:
    -Einen Pfad (ähnlich einer Funktion ohne Rückgabewert) mit optionalen Parametern und einem Block von Statements.
    -Eine Berechnungsdefinition, die neben den Parametern und Statements einen Rückgabewert über einen Ausdruck (expr) liefert.

- **Statements und Befehle:**

    ```antlr
    statList : stat* ;

    stat : walk | save | jump
         | walkHome | jumpHome | turnLeft
         | turnRight | direction | clear
         | stop | finish | storeVar
         | addVar | subVar | divVar
         | multVar | walkMark | jumpMark
         | mark | colorCmd | if
         | toFor | pathCall
         | for | while | doUntil
         ;
    ```
    Die Regel stat fasst alle möglichen Anweisungen zusammen, wie Bewegungsbefehle, Variablenoperationen, Schleifen und bedingte Anweisungen.

# Beispiele für spezifische Befehle
- **Bewegungsbefehle ("walk" und "jump"):**

    ```antlr
    walk : Walk expr       #WalkFront
         | Walk Back expr  #WalkBack;
    jump : Jump expr       #JumpFront
         | Jump Back expr  #JumpBack;
    ```
    Hier wird unterschieden, ob der Befehl vorwärts oder rückwärts ausgeführt wird. Die Labels (z. B. #WalkFront) ermöglichen später beim Traversieren des AST die Unterscheidung der Varianten.

- **Richtungsbefehle ("turn left"/"turn right"):**

    ```antlr
    turnLeft  : Turn Left expr;
    turnRight : Turn (Right)? expr;
    direction : Direction expr;
    ```
    Diese Regeln definieren, wie Drehbefehle interpretiert werden. Ein optionales „Right“ erlaubt es, dass bei fehlendem Schlüsselwort standardmäßig eine Rechtsdrehung ausgeführt wird.

- **Variablenbefehle:**

    ```antlr
    storeVar: 'store' expr 'in' var ;
    addVar  : 'add' expr 'to' var;
    subVar  : 'sub' expr 'from' var;
    divVar  : 'div' var 'by' expr;
    multVar : 'mul' var 'by' expr;
    ```
    Diese Regeln definieren Befehle, die den Zuweisungs- und Rechenoperationen auf Variablen zugeordnet sind.

- **Schleifen und Bedingungen:**

    ```antlr
    if      : 'if' cond 'then' stat+ else? 'endif' ;
    else    : 'else' stat+ ;
    toFor   : 'do' expr 'times' stat+ 'done';
    for     : 'counter' var 'from' expr 'to' expr 'do' stat+ 'done' #simpUpFor
            | 'counter' var 'from' expr 'to' expr Step expr 'do' stat+ 'done' #stepUpFor
            | 'counter' var 'from' expr 'downto' expr 'do' stat+ 'done' #simpDownFor
            | 'counter' var 'from' expr 'downto' expr Step expr 'do' stat+ 'done' #stepDownFor
            ;
    while   : While cond 'do' stat+ 'done';
    doUntil : 'repeat' stat+ Untile cond;
    ```
    Diese Regeln zeigen verschiedene Schleifen-Konstrukte (z. B. for-Schleifen mit Auf- und Abzählung, while-Schleifen sowie repeat-until) und die if-else-Struktur.

# 3. Ausdrücke und Bedingungen
- **Arithmetische Ausdrücke:**

    ```antlr
    expr  :
           expr '^' expr #Exp
         | expr '*' expr #Mult
         | expr '/' expr #Dife
         | expr '-' expr #Dim
         | expr '+' expr #Add
         | '|' expr '|'  #ABS
         | '-' ( number | klamKon | var)   #Negate
         | klamKon #ClamExpr
         | ID '(' ( ( expr ',')* expr)? ')'  #funcCall
         | number	      #NumExpr	
         | var	      #VarExpr
         ;
    klamKon	: '(' expr ')' ;
    number: Num     #Int
          | Float   #Float
          ; 
    var   : ID        #Variable
          | '@pi'     #piVar
          | '@max_x'  #MaxX
          | '@max_y'  #MaxY
          | CliID     #CLI
          | IncID     #GlobalVariable
          ;
    ```
    Hier werden mathematische Operatoren (Exponentiation, Multiplikation, Addition etc.) unterstützt. Durch die Verwendung von Labels (z. B. #Exp, #Add) kann beim späteren Traversieren des AST zwischen   unterschiedlichen Ausdruckstypen unterschieden werden.
    Außerdem wird der Funktionsaufruf sowie die Klammerung (klamKon) behandelt.

- **Bedingungen:**

    ```antlr
    cond  : expr '<' expr #lesThan
          | expr '>' expr #greaterThan
          | expr '<=' expr #lesEqThan
          | expr '>=' expr #greaterEqThan
          | expr '=' expr #Equal
          | expr '<>' expr #Unequal
          | '(' cond ')' #clamCond
          | 'NOT' cond #notCond
          | cond 'AND' cond #andCond
          | cond 'OR' cond #orCond
          ;
    ```
    Mit dieser Regel lassen sich komplexe logische Bedingungen formulieren, die Vergleichsoperatoren und logische Operatoren kombinieren. Auch geschachtelte Bedingungen mittels Klammern werden unterstützt.

---

### Visitor und Listener

Visitor- und Listener-Pattern sind zwei unterschiedliche Strategien, um den von ANTLR erzeugten Parse Tree (Syntaxbaum) zu durchlaufen und Aktionen auszuführen. Beide Ansätze basieren auf dem Prinzip der Trennung von Traversierung und Verarbeitung, unterscheiden sich jedoch hinsichtlich ihres Designs und ihrer Flexibilität.

## Visitor-Pattern

Das Visitor-Pattern in ANTLR beruht auf einem expliziten Rekursionsmechanismus. Für jede Parser-Regel wird eine Methode im Visitor-Interface generiert. Diese Methoden (z. B. visitWalkFront, visitAddVar oder allgemein visitExpr) sollen für den Knoten „Besuch“ finden, d. h. die Verarbeitung erfolgt, indem der AST explizit rekursiv durchlaufen wird.

**Besonderheiten und Vorteile:**
- **Explizite Rückgabewerte:**
    Die vom Visitor übergebenen Methoden können typisiert sein und Rückgabewerte liefern. Dadurch wird es möglich, Berechnungen oder Transformationen während des Durchlaufs direkt an den Knoten zu verarbeiten und Werte aggregiert zurückzugeben. Dies ist besonders nützlich, wenn komplexe semantische Analysen oder Code-Transformationen (z. B. die Generierung von C-Code) durchgeführt werden sollen.

- **Klare Hierarchie und Struktur:**
    Durch die explizite Deklaration der Besuchermethoden erhält man einen klaren Überblick darüber, welche Knoten vorhanden sind. Der Entwickler kann gezielt nur jene Methoden überschreiben, die für die eigene Logik relevant sind. So kann man beispielsweise entscheiden, den Besucher nur für mathematische Ausdrücke oder Bewegungsbefehle zu implementieren.

- **Flexible Rekursion:**
    Da der Visitor die Rekursion explizit steuert, kann man die Baumstruktur genau so durchlaufen, wie es zur Lösung der Aufgabe erforderlich ist. Beispielsweise kann man beim Traversieren des AST bestimmte Zweige überspringen oder Ergebnisse zwischen den Knoten aggregieren.

**Nachteile:**
- **Erweiterung bei Änderungen:**
    Wird die Grammatik geändert (z. B. durch Hinzufügen neuer Regeln), so muss auch der Visitor (bzw. das Interface) aktualisiert werden. Oft generiert ANTLR hier neuen Code, was den Overhead erhöht.
- **Höhere Komplexität:**
    Gerade bei sehr umfangreichen Grammatiken kann der Visitor-Ansatz zu viel Boilerplate-Code führen, da für fast jeden Knoten eine Methode vorhanden ist.

## Listener-Pattern

Das Listener-Pattern hingegen basiert auf einem ereignisbasierten Ansatz. ANTLR erzeugt für jede Parser-Regel zwei Methoden: enterXxx und exitXxx. Diese Methoden werden automatisch beim Betreten bzw. Verlassen eines Knotens im Parse Tree aufgerufen – ganz analog zum Observer-Prinzip.

**Besonderheiten und Vorteile:**

- **Ereignisgesteuert:**
Anstatt den Baum explizit zu durchlaufen, wird der Entwickler über „Enter“ und „Exit“-Methoden benachrichtigt, wenn ein bestimmter Knoten betreten oder verlassen wird. Dies führt häufig zu einem klareren Code, der auf Ereignisse reagiert.

- **Weniger Boilerplate:**
Der Listener-Ansatz benötigt oft weniger initialen Code, da man nur die Methoden implementiert, die von Interesse sind, ohne den gesamten Baum rekursiv manuell behandeln zu müssen. Das kann besonders bei einfachen Anwendungen oder wenn nur bestimmte Ereignisse abgefangen werden sollen, von Vorteil sein.

- **Automatische Traversierung:**
ANTLR übernimmt beim Listener die gesamte Traversierung des Baumes, sodass man sich nicht um rekursive Aufrufe kümmern muss. Das reduziert die Komplexität bei der Entwicklung der Baumverarbeitung.

**Nachteile:**

- **Kein direkter Rückgabewert:**
Die Listener-Methoden sind typischerweise void und liefern keine Rückgabewerte. Sollten Daten von Knoten verarbeitet und zurückgegeben werden müssen, muss man hierfür einen externen Speicher bzw. eine globale Datenstruktur einsetzen.

- **Weniger Kontrolle über den Traversierungsprozess:**
Da der Listener-Mechanismus automatisch abläuft, hat man weniger Flexibilität, beispielsweise das Überspringen bestimmter Knoten oder eine selektive Behandlung des Baumes zu implementieren. Das kann in komplexeren Anwendungen, etwa bei der Code-Generation, einschränkend wirken.



