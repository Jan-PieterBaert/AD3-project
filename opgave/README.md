---
title: To B or not to B-tree
subtitle: Project Algoritmen en Datastructuren 2019-2020
papersize: A4
pagestyle: headings
colorlinks: true
numbersections: true
lang: nl-BE
geometry: a4paper

---

# Inleiding

Er is je gevraagd om een databank te maken voor een programma dat verschillende tijdslijnen simuleert en evenementen registreert. De databank moet heel snel het aantal evenementen binnen een bepaalde tijdspanne kunnen bepalen. Deze waarden zijn immers nodig om de simulatie verder uit te voeren.

Om dit te doen, maak je gebruik van een aangepaste B-tree die snel intervallen van data kan samenvatten zonder alle toppen te moeten overlopen. Om een zo hoog mogelijke snelheid te verkrijgen onderzoek je hoe je de toppen van de boom best opslaat om betere caching eigenschappen te hebben.

# Functionele vereisten

Schrijf een programma `history` dat evenementen bijhoudt in een **B-tree**. Je programma kan via de standaard invoer volgende commando's krijgen:

- `+`:  sla een evenement op bij een bepaald tijdstip
- `-`: verwijder een evenement
- `?`: geef de gegevens van het evenement op een gegeven tijdstip
- `#`: bepaal het aantal evenementen tussen twee tijdstippen (inclusief eindpunten)

Zorg ervoor dat je programma een goede caching strategie heeft.

## Voorstelling tijdstippen

Tijdstippen worden weergegeven als strings in [ISO 8601](https://nl.wikipedia.org/wiki/ISO_8601) notatie zonder tijdzone (de deadline van dit project wordt bijvoorbeeld genoteerd als: "`2019-12-02T13:37:42`"). Voor de eenvoud kun je gebruik maken van de lexicografische orde die ISO 8601 je geeft om tijdstippen te vergelijken. Het is niet nodig om de binnenkomende tijdstippen op eender welke manier te parsen. Je hoeft ook geen rekening te houden met tijdzones en dergelijke. Tijdstippen **moet** je voor dit project zien als gewone strings van 19 karakters.

## Commando's

Je programma moet de `+`, `-`, `?` en `#` commando's aanvaarden en daar op een juiste manier mee omgaan. Het is toegestaan om zelf extra commando's toe te voegen aan je programma zo lang die niet interfereren met de gevraagde commando's. 

### Toevoegcommando (`+`)

```
"+" DATUM " " INHOUD "\n"
```

Het toevoegcommando is een reeks met achtereenvolgens

- een `+` teken,
- een ISO 8601 representatie van een tijdstip (zonder tijdzone),
- een spatie "` `",
- de inhoud van het evenement als extended ASCII string,
- een afsluitende nieuwe lijn (`\n`)

Als een toevoegcommando wordt ingelezen, moet `INHOUD` in de databank geplaatst worden onder de sleutel `DATUM`. Als er al een waarde in de databank zit voor die `DATUM`, moet deze overschreven worden. Het toevoegcommando genereert geen output.

Voorbeelden:

```
+2019-01-01T12:10:37 Inval van Gargamel in het Smurfendorp
+2019-01-01T18:04:12 Smurfin bevalt van haar eerste zoon
+2019-04-01T12:15:14 Mopsmurf veroorzaakt het knoflookpoederfiasco
```

### Verwijdercommando (`-`)

```
"-" DATUM "\n"
```

Het verwijdercommando is een reeks met achtereenvolgens

- een `-` teken,
- een ISO 8601 representatie van een tijdstip (zonder tijdzone),
- een afsluitende nieuwe lijn (`\n`)

Bij het verwerken van een verwijdercommando verwijder je het evenement dat op het tijdstip `DATUM` in de databank zit. Bij een succesvolle verwijdering schrijf je `-\n` naar de standaard uitvoer. Bij een gefaalde verwijdering, bijvoorbeeld als het tijdstip niet in de databank voorkomt, schrijf je `?\n` uit. Gezien er maar 1 evenement is per tijdstip, is een opzoeking naar het tijdstip dat net verwijderd is altijd onsuccesvol.

Voorbeeld:

```
-2019-04-01T12:15:14
```

Output

```
-
```



### Opvraagcommando (`?`)

```
"?" DATUM
```

Een opvraagcommando is een reeks met achtereenvolgens

- een `?` teken,
- een ISO 8601 representatie van een tijdstip (zonder tijdzone),
- een afsluitende nieuwe lijn (`\n`)

Na het inlezen van een opvraagcommando, schrijf je naar standaard uitvoer 

- het teken `!` onmiddellijk gevolgd door de extended ASCII string die het evenement op dat tijdstip beschrijft  als er een evenement is op dat exacte tijdstip.
- het teken `?` als er geen evenement is op het gegeven tijdstip

gevolgd door een nieuwe lijn (`\n`) 

Voorbeeld (lijnen afwisselend input en output)

```
?2019-01-01T12:10:37
!Inval van Gargamel in het Smurfendorp
?2019-01-01T23:10:37
?
```



### Telcommando (`#`)

```
"#" DATUM " " DATUM
```

Een telcommando is een reeks met achtereenvolgens

- een `#` teken,
- een ISO 8601 representatie van een begintijdstip (zonder tijdzone),
- een spatie
- een ISO 8601 representatie van een eindtijdstip (zonder tijdzone),
- en een afsluitende nieuwe lijn (`\n`)

Na het inlezen van een telcommando schrijf je naar standaard uitvoer het aantal evenementen tussen het begintijdstip en eindtijdstip (inclusief eindpunten, ook omgekeerd). Een evenement is tussen twee tijdstippen als zijn [ISO 8601](https://nl.wikipedia.org/wiki/ISO_8601) tijdsnotatie lexicografisch tussen de [ISO 8601](https://nl.wikipedia.org/wiki/ISO_8601) tijdsnotaties van die tijdstippen is. Na het uitschrijven van dit getal schrijf je een nieuwe lijn (`\n`). *Merk dus op dat een of beide tijdstippen mogelijk niet in de databank kunnen zitten*. 

Voorbeeld (lijnen afwisselend input en output)

```
#2019-01-01T23:10:37 2020-01-01T23:10:37 
4
#2019-01-01T23:10:37 2018-01-01T23:10:37 
0
```

## Caching

Zelfbalancerende bomen, zoals B-trees, kunnen een slechte caching performantie hebben. Omdat de boom zich telkens hervormt, verplaatsen de toppen in het geheugen. De gelinkte structuur zorgt er aan de andere kant voor dat de blokken bij een naïve implementatie niet aansluitend in het geheugen zitten. Het is typisch wel zo dat als je een bepaalde top ophaalt je al weet welke top je als volgende zult bezoeken.  Moderne computers buiten dit uit door gebruik te maken van een zogenaamde cache. De cache is een stuk snel geheugen dicht bij de CPU. Recent gebruikte blokken die uit de RAM gehaald zijn worden opgeslagen in deze cache zodat ze de volgende keer sneller beschikbaar zijn. Als we dus net met een top uit de B-tree hebben gewerkt, zal die de volgende keer sneller beschikbaar zijn.

De cache is een klein stuk geheugen en zal dus niet groot genoeg zijn om heel de boom en alle data op te slaan. Onderzoek verschillende methoden om toppen op te slaan om het aantal cache hits te verhogen. Een mogelijke aanpak is om bepaalde toppen in plaats van elk apart te `malloc`en, een array van meerdere toppen in 1 keer te `malloc`en en de toppen daarin slim te positioneren. Het kan handig zijn om bij compilatie te weten wat de grootte van de cache op je systeem precies is. Om je gecompileerde programma zich hier aan te laten passen krijg je volgende defines mee tijdens het compileren (allen uitgedrukt in bytes):

- `CACHESIZE`: grootte van de cache in bytes (bijvoorbeeld 8192)
- `CACHEASSOC`: de cache associativity (zal altijd 8 zijn), 
- `CACHELINESIZE`: de cache lijn grootte (zal altijd 64B zijn)

Beschrijf in je verslag welke keuzes je hebt gemaakt en welke technieken je hebt gebruikt om het aantal cache hits te verhogen. Vergelijk het performantieverschil tussen een boom die veel wordt gewijzigd en een die vrij statisch is?

*Hint*: Gebruik [`valgrind --tool=cachegrind`](http://valgrind.org/docs/manual/cg-manual.html) om het aantal cache hits en misses te tellen. Cachegrind simuleert het gebruik van een cache met instelbare grootte. Hiervoor baseren ze zich op de cache structuur van x86 chips. Deze bestaat uit twee niveaus: een kleine niveau 1 cache (`L1`) en een grotere, maar nog steeds kleine, niveau 2 cache (`L2`). De niveau 1 cache is opgedeeld in een deel voor data `D1` en een deel voor instructies (`I1`). Voor dit project hoef je enkel rekening te houden met de hitrate van de `D1` cache. 

Met de `--D1` vlag kun je cachegrind de grootte van de L1 data cache instellen. Experimenteer met verschillende cachegroottes. Vanaf wanneer is er een significante inpakt op de hitrate en dus op de performantie? 

```bash
valgrind --tool=cachegrind --D1=CACHESIZE,CACHEASSOC,CACHELINESIZE ./history
```

Cachegrind zal gebruikt worden om de projecten te verbeteren. 

# Specificaties

## Programmeertaal

In de opleidingscommissie informatica (OCI) werd beslist dat, om meer ervaring in het programmeren in C te verwerven, het project horende bij het opleidingsonderdeel Algoritmen en Datastructuren 3 in C geïmplementeerd moet worden. Het is met andere woorden de bedoeling je implementatie in C uit te voeren. Je implementatie moet voldoen aan de ANSI-standaard. Je mag hiervoor dus gebruikmaken van de laatste features in C18, voor zover die ondersteund worden door `gcc` versie 8.3.0 op SubGIT.

Voor het project kun je de standaard libraries gebruiken; externe libraries zijn echter niet toegelaten. Het spreekt voor zich dat je normale, procedurele C-code schrijft en geen platformspecifieke APIs (zoals bv. de Win32 API) of features uit C++ gebruikt. Op Windows bestaat van enkele functies zoals `qsort` een "safe" versie (in dit geval `qsort_s`), maar om je programma te kunnen compileren op een Unix-systeem kun je die versie dus niet gebruiken.

**Wat je ontwikkelingsplatform ook mag zijn, controleer geregeld de output van SubGIT om te verifiëren dat jouw programma ook bij ons compileert!**

## Input-Output en implementatiedetails

Voor de in- en uitvoer gebruiken we de standaard `stdin` en `stdout` streams.

Je programma moet dus bijvoorbeeld als volgt gebruikt kunnen worden:

```
$ cat input
+2019-01-01T12:10:37 Inval van Gargamel in het Smurfendorp
?2019-01-01T12:10:37
$ cat input | ./history
Inval van Gargamel in het Smurfendorp
```

## Verslag

Schrijf een bondig verslag over je werk. Vertel in je verslag welke ontwerpbeslissingen je hebt genomen. 

Maak je verslag niet onnodig lang.

# Indienen

## Directorystructuur

Je ingave moet volgende structuur hebben:

- `src/` bevat alle broncode (inclusief jou eigen Makefiles, exclusief gecompileerde bestanden).

- `tests/` alle testcode.

- `extra/verslag.pdf` bevat de elektronische versie van je verslag. In deze map kun je ook eventueel extra bijlagen plaatsen.

- `sources` is een tekstbestand dat de `.c` bronbestanden voor je project oplijst, 1 per lijn relatief t.o.v. de `src` directory. Bijvoorbeeld:

  ```
  main.c
  btree/btree.c
  ```

Je directory structuur ziet er dus ongeveer zo uit:

```
|
|-- extra/
|   `-- verslag.pdf
|-- src/
|   `-- je broncode
|-- tests/
|   `-- je testcode
`-- sources
```

## Compileren

De code zal bij het indienen gecompileerd worden met de opdracht `gcc -std=c18 -lm` door SubGIT met GCC versie 8.3.0. Op UFora zal de Dockerfile en bijhorende bronbestanden staan die SubGIT gebruikt om jouw code te compileren en minimale testen op uit te voeren. Je kan deze Docker ook onmiddellijk van Dockerhub halen met volgende commando's:

```
docker pull beardhatcode/da3-project-2019-2020:latest
docker run -it --rm --mount type=bind,source={PAD},destination=/submission,readonly beardhatcode/da3-project-2019-2020:latest
```

Waarbij `{PAD}` vervangen dient te worden met het absolute pad naar jou code. 

## SubGIT

Het indienen gebeurt via het [SubGIT](https://subgit.ugent.be/) platform. Indien je hier nog geen account op hebt, dien je deze aan te maken.

### Repository afhalen

```bash
git clone git@subgit.ugent.be:2019-2020/project/{studentnr} projectDA3
```

### Opgave als upstream instellen

Je kunt de opgave en boilerplate voor het project afhalen door de opgave repository als upstream met volgende commando's in de `projectDA3` map:

```bash
git remote add upstream git@subgit.UGent.be:2019-2020/DA3/project-assignment
git pull upstream master
```

### Feedback

Als je pusht naar SubGIT, zul je in je terminal te zien krijgen of je code voldoet aan de minimumvereisten. In dat geval krijg je bij het pushen de melding dat het pushen geslaagd is:

```
remote: Acceptable submission
```

Je kunt geen code pushen naar de `master` branch als die niet compileert of niet aan de minimale IO vereisten voldoet. Je kunt echter wel pushen naar andere branches en daar zal je push wel aanvaard worden.

### `master` branch

De `master` branch op SUBGit stelt jou ingave voor. Je kunt voor de deadline zoveel pushen als je wilt. Zorg ervoor dat je voor de deadline zeker je finale versie naar de **`master`** branch hebt gepushed.

### Controleren of je zeker goed hebt ingediend

Je kunt jouw indiening bekijken door jou repository nog eens te klonen in een andere map

```bash
cd eenAndereMap
git clone git@subgit.ugent.be:2019-2020/DA3/project/{studentnr} projectDA3Controle
```

## Deadlines en belangrijke data

Zorg ervoor dat je eerste tussentijdse versie hebt gepusht hebt naar een branch[^1] op SubGIT voor **2019-11-28 om 13:37:42**. De versie die je hier upload zal niet bekeken worden, we vragen je dit te doen zodat we er zeker van zijn dat iedereen kan indienen. Indien je niets pusht voor deze deadline verlies je al je punten voor het project.

Na **2019-12-02 om 13:37:42** kun je enkel nog pushen naar de `extra` map in SubGIT. De code die op dat moment op de `master` branch staat is je finale indiening. Enkel code op de `master` branch wordt verbeterd. **Geen code op de `master` branch is nul op vier voor het project.** Je kunt na deze deadline nog steeds aan je verslag werken in de `extra` map. 

Na  **2019-12-09 om 13:37:42** kun je ook geen wijzigingen meer aanbrengen aan de `extra` map. Het verslag dat aanwezig is als PDF op de `master` branch in `extra/verslag.pdf` is je finaal verslag.

# Algemene richtlijnen

- Schrijf efficiënte code, maar ga niet over-optimaliseren: **geef de voorkeur aan elegante, goed leesbare code** Kies zinvolle namen voor methoden en variabelen en voorzie voldoende commentaar.
- Op Ufora staat een Dockerfile waarmee je een Linux container kunt bouwen die jou code compileert en minimale testen uitvoert. Als alle testen slagen zal dit programma `AANVAAARD` uitschrijven als laatste regel en stoppen met exit code 0. Code die hier niet aan voldoet zal niet aanvaard worden op SubGIT en kan ook geen punten opleveren (0/4 voor het project).
- Het project wordt gequoteerd op **4** van de 20 te behalen punten voor dit vak, en deze punten worden ongewijzigd overgenomen naar de tweede examenperiode.
- Projecten die ons niet (via de `master` branch op SubGIT) bereiken voor de deadline worden niet meer verbeterd: dit betekent het verlies van alle te behalen punten voor het project.
- Dit is een individueel project en dient dus door jou persoonlijk gemaakt te worden. Het is uiteraard toegestaan om andere studenten te helpen of om ideeën uit te wisselen, maar **het is ten strengste verboden code uit te wisselen**, op welke manier dan ook. Het overnemen van code beschouwen we als fraude (van <span>**beide**</span> betrokken partijen) en zal in overeenstemming met het examenreglement behandeld worden. Op het internet zullen ongetwijfeld ook (delen van) implementaties te vinden zijn. Het overnemen of aanpassen van dergelijke code is echter <span>**niet toegelaten**</span> en wordt gezien als fraude.
- Essentiële vragen worden **niet** meer beantwoord tijdens de laatste week voor de deadline.

# Vragen

Op Ufora zal een lijst van veelgestelde vragen staan.

Als je vragen hebt over de opgave of problemen ondervindt, dan kun je je vraag stellen tijdens het oefeningencollege. Alternatief kun je ook terecht op het forum op Ufora. Contacteer je ons per mail, stuur  dan ook je studentennummer en de branch waarop de code met het probleem zit mee. Stuur geen screenshots van code.


[^1]: Niet noodzakelijk `master`, moet zelfs niet compileren