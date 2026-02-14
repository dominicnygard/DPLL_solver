# DPLL ratkaisin

[![GHA workflow badge](https://github.com/dominicnygard/DPLL_solver/actions/workflows/main.yml/badge.svg)](https://github.com/dominicnygard/DPLL_solver/actions)
[![codecov](https://codecov.io/gh/dominicnygard/DPLL_solver/graph/badge.svg?token=DSODJT1PCP)](https://codecov.io/gh/dominicnygard/DPLL_solver)

Algoritmit ja tekoäly -harjoitustyönä tehty DPLL ratkaisija.

## Dokumentaatio

[Määrittelydokumentti](https://github.com/dominicnygard/DPLL_solver/blob/main/docs/maarittelydokumentti.md)  
[Testi dokumentti](https://github.com/dominicnygard/DPLL_solver/blob/main/docs/testidokumentti.md)  
[Toteutus dokumentti](https://github.com/dominicnygard/DPLL_solver/blob/main/docs/toteutusdokumentti.md)

## Viikkoraportit

[Viikkoraportti 1](https://github.com/dominicnygard/DPLL_solver/blob/main/docs/viikkoraportti1.md)  
[Viikkoraportti 2](https://github.com/dominicnygard/DPLL_solver/blob/main/docs/viikkoraportti2.md)  
[Viikkoraportti 3](https://github.com/dominicnygard/DPLL_solver/blob/main/docs/viikkoraportti3.md)  
[Viikkoraportti 4](https://github.com/dominicnygard/DPLL_solver/blob/main/docs/viikkoraportti4.md)
[Viikkoraportti 5](https://github.com/dominicnygard/DPLL_solver/blob/main/docs/viikkoraportti5.md)

## Käyttö ohjeet

Projektia rakentamista varten tarvitaan cmake. Kun olet projektin juuressa.  
`mkdir build`  
`cd build`  
`cmake ..`  
`cmake --build .`  
Tämän jälkeen pystyt testata algoritmia, käytät algoritmia antamalla sille cnf tiedosto syötteenä, esimerkiksi.  
`./src/DPLL_solver ../cnf/add4.cnf`  
Tämä olettaa että olet vielä build hakemistossa. Löytyy testi tiedostoja cnf hakemistosta, joka voi käytää testaakseen ratkaisijaa. Testit pystyy myös suorittaa jos ajaa DPLL_tests, joka löytyy test hakemistosta build hakemistossa.
