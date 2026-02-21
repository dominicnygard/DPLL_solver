# Toteutus dokumentti

Ojelmalla on Solver luokka joka sisältää kaikki DPLL algoritmin liittyvää, se sisältää myös datan esikäsittelyä. Tämä olisi myös voinut laittaa toiseen paikkaan, mutta käytän ainoastaan puhtaan literaalin poisto. Tiedoston lukija on omassa namespacessa, niin että se erottuu. Tiedosto struktuurit joka käytetään on myös omassa tiedostossa, niin että sitä pystyy käyttämään siellä missä sitä tarvitaan. Main funktio pysyy puhtaana, koska käytetään IO namespace joka hoitaa kaikki tiedoston lataaminen ja ratkaisijan ajaaminen. Kaikki ratkaisijan metodit pystyy myös ajamaan itsenäisesti, niin että niitä pystyy helposti testata.

## Saavutetut aika- ja tilavaativuudet

### solve() - Pääalgoritmi

**Aikavaativuus: O(2^n)** eksponentiaalisesti pahimmassa tapauksessa

Pseudokoodi:

```
function solve():
    if not propagate():
        return false

    var = etsi_ensimmainen_asettamaton_muuttuja()
    if var == -1:
        return true  // Löytyi ratkaisu

    // yritä muuttujaa true
    aseta(var) = 1
    if solve():
        return true
    kumoa(var)

    // Yritä muuttujaa false
    aseta(var) = 0
    if solve():
        return true
    kumoa(var)

    return false
```

**Analyysi:**

- Algoritmi rakentaa rekursiivisen puun, jossa jokaisella tasolla on 2 haaraa (true/false)
- Syvyys on max n muuttujaa
- Pahimmassa tapauksessa puu on täynnä 2^n lehtiä
- Jokainen rekursion kutsu kutsuu propagate():ia.
- **Kokonais aikavaativuus: O(2^n)** pahimmassa tapauksessa
- Watched literals ja unit propagation varmistaa, että jokainen muuttujan prosessointi on tehokas, tämän takia käytänössä todellinen suoritusaika on parempi
- L on jokainen kohta missä literaali esintyy, ja n on muuttujien määrä. Tilavaativuus algoritmille on O(n+L), koska tallennetaan jokainen literaali klausuuliin ja jokainen muuttuja assignment:iin.

## Suorituskyky

Kun käytän suorituskyky testiä joka käyttää sat ongelma tiedostossa prime4294967297.cnf, niin se kestää omalla tietokoneellani, 5.78 sekuntia. Käyttäen sama tiedosto Cadical ratkaisijalla se kestää 1.52 sekuntia, elikä ratkaisijani on suunnilleen 4 kertaa hitaampi.

### parannukset

Ohjelman voisi muuttamalla algoritmia CDCL algoritmiksi parantaa. Ohjelma voisi vielä myös optimoida.
