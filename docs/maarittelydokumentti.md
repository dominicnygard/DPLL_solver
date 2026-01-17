# projektin määrittely

Opinto ohjelma: Tietojenkäsittelytiede, kandidaatin tutkinto

Projekti on dokumentoitu suomeksi ja toteutetaan c++ ohjelmointikielen avulla. Tiedän myös miten käyttää Python, typescript/javascript vertais arviointia varten.
Pää algoritmi joka projektissa käytetään on DPLL algoritmi (Davis-Putnam-Logemann-Loveland), tietorakenteena käytetään vektori joka sisältää vektorit. Sisänmäinen vektori käyttää kokonaislukuja joka vastaa DIMACS tiedostojen kokonaisluvut, tämä on siis lausekkeet SAT ongelmassa. Projekti ratkaisee SAT ongelma eli lauselogiikan toteutuvuusongelma, jolla pyritään selvittämään loogisen lauseen toteutettavuus. Ohjelma saa syötteenä argumenttina reitti DIMACS tiedostoon, tämä käytetään lukemaan lauseet tiedostosta ohjelman muistiin. Algoritmin tavoite aikavaativuus on huonoimillaan O(n^2) ja parhaimmillaan O(n + m), paras aika vaatimus tulee siitä jos yksikköpropagaatio ja puhtaan literaalin poisto onnistuu heti. Algoritmin tilavaativuus on O(n). Algoritmin huonoin aikavaativuus tulee siitä jos algoritmin on pakko mennä kaikkien haarojen läpi tarkastaen jos lauselogiikka on toteutuva.

Projektin hallintaa varten käytetään cmake ja testit toteutetaan catch2 avulla.

## lähteet

[Wikipedia: DPLL algorithm](https://en.wikipedia.org/wiki/DPLL_algorithm)  
[Aalto: Propositional satisfiability and SAT solvers](https://users.aalto.fi/~tjunttil/2020-DP-AUT/notes-sat/preprocessing.html)
