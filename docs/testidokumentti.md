# Testi dokumentti

Yksikkötesteissä testataan yksinkertaiset SAT ongelmat, testataan myös kyyhkyslakkaperiaate, sekä poikkeus tapahtuma kuten tyhjä syöte ja jos löytyy monta eri vastauksia. Testit toteutettiin Catch2 testauskirjastolla. Testattiin että ratkaisija pystyy löytää ratkaisun, kun sellainen on olemassa ja tunnistaa ristiriitaiset kaavat sekä toimia oikein kun löytyy useita ratkaisuja. Testien syötteenä on eri SAT ongelmia, sellaisessa muodossa että ne kelpaa ratkaisijalle, samanlaisesti kun cnf tiedostossa.

Yksikkö propagaatio on testattu sillä tavalla että kaikki muuttuja määräytyvät yksikkölauseiden kautta, tuloksena on sitten että ei ole yhtään päätöstä ratkaisussa, tämä varmistaa että yksikköpropagaatio toimii tehokkaasti.

On myös end to end testi, joka testaa ohjelman kyky lukea tiedoston ja kirjoittaa ulos oikean vastauksen terminaalin.

Suorituskyky testit ovat implementoitu, niin että ne tarkastaa että ratkaisija toimii, niin kuin sen pitää tarkastamalla että se käyttää oikea määrä päätöksiä, takaisinpalautuksia tai propagaatioita. Testeissä ei testata paljonko aikaa ratkaisija tarvitsee ratkaisemaan erit SAT ongelmat, tämä on koska jos käyttää heikkompi tai voimakkaampi tietokone, niin tulokset voivat olla eri. Löytyy kuitenkin myös suorituskyky testi, joka kertoo kuinka kauan kestää ratkaisijalle ratkaista prime4294967297.cnf tiedosto, tämän avulla pystyy vertailla ratkaisija muiden ratkaisijen kanssa.

Testikattavuus pystyy nähdä tästä [Testi kattavuus](https://app.codecov.io/gh/dominicnygard/DPLL_solver/tree/main/src)

Testit pystytään suorittaa ensin rakentamalla projekti käyttäen  
`cmake --build`  
`./build/test/DPLL_tests`  
tai  
`cd build/test`  
`ctest`

Jos haluaa testata suorituskykyä täytyy käyttää  
`./build/test/DPLL_tests "[benchmark]"`  
Kaikki testit pystyy suorittaa myös itsenäisesti jos käyttää niiden nimiä argumenttina kun suorittaa testit.
