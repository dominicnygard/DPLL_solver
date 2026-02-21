#include <solver.h>
#include <set>
#include <algorithm>
#include <iostream>

Solver::Solver(CNF clauses)
{
    this->clauses = std::move(clauses.clauses);
    num_variables = clauses.num_vars;
    
    assignment.resize(num_variables);
    fill(assignment.begin(), assignment.end(), -1);
    
    pure_literal_elimination();
    
    set_watches();
}

/*
 *  Funktio toimii, niin että
 * 
 * Se kutsuu propagate() ja suorittaa yksikköpropagoinnin.
 * Jos konflikti löytyy, backtrack. 
 * 
 * Sen jälkeen tarkistetaan, ovatko kaikki muuttujat asetettu (var == -1).
 * Jos kyllä: löytyi ratkaisu, palauta true.
 * Jos ei: valitse seuraava vapaa muuttuja.
 *
 * Tämän jälkeen Tehdään päätöksen: yrittää ensin arvoa true (var + 1 = positive literal).
 * Tallentaa tämänhetkisen backtrack ja propagate_idx tilan.
 * Asettaa assignment[var] = 1 ja lisää (var + 1) backtrackiin.
 * Kutsuu recursively solve().
 * Jos recursive kutsu palauttaa true, niin löytyi ratkaisu, palauta true.
 *
 * Jos ykkönen ei johtanut ratkaisuun:
 * Kumoa päätös ja poista backtrack-jonosta kaikki mitä lisättiin,
 * aseta assignment[var] = -1, palauta propagate_idx.
 *
 * Sitten yritetään toista arvoa (arvoa false = -(var + 1) = negative literal).
 * Samalla logiikalla kuin ennen.
 * Jos tämä toimii, palauta true.
 *
 * Jos molemmat arvot epäonnistuvat:
 * Kumoa toinen päätös kuten kohdassa 4.
 * Palauta false (ei ratkaisua tällä haaralla).
 *
 * Lopuksi palauttaa true jos löytyi tyydyttävä sijoitus, false jos ei löytynyt.
 */
bool Solver::solve()
{
    if (!propagate())
        return false;

    int var = -1;
    for (int i = 0; i < assignment.size(); i++) {
        if (assignment[i] == -1) {
            var = i;
            break;
        }
    }

    if (var == -1)
        return true;

    size_t decision_trail_size = backtrack.size();
    size_t decision_prop_idx = propagate_idx;

    backtrack.push_back(var + 1);
    assignment[var] = 1;

    runtime_stats.decisions++;

    if (solve())
        return true;

    while (backtrack.size() > decision_trail_size) {
        int lit = backtrack.back();
        assignment[abs(lit) - 1] = -1;
        backtrack.pop_back();
    }
    propagate_idx = decision_prop_idx;

    backtrack.push_back(-(var + 1));
    assignment[var] = 0;

    if (solve())
        return true;

    while (backtrack.size() > decision_trail_size) {
        int lit = backtrack.back();
        assignment[abs(lit) - 1] = -1;
        backtrack.pop_back();
    }
    propagate_idx = decision_prop_idx;

    return false;

}

int literal_id(int literal) {
    int variable = abs(literal) - 1;
    int sign = (literal < 0);
    return 2 * variable + sign;
}

/* Laitetaan kaikki watches watch listaan */
void Solver::set_watches() {
    watch_list.resize(2 * num_variables);

    /* 
     * Mennään kaikki klausuulien läpi ja laitetaan ensinmäinen ja toinen literaali
     * watch, käytetään indeksi tähän, jos on ainoastaan yksi literaali klausuulissa, niiin
     * asennetaan ensinmäinen watch olemaan se ja toinen watch on -1.
    */
    for (int cid = 0; cid < clauses.size(); cid++) {
        Clause& c = clauses[cid];

        if (c.literals.size() >= 2) {
            c.watch1 = 0;
            c.watch2 = 1;

            watch_list[literal_id(c.literals[0])].push_back(cid);
            watch_list[literal_id(c.literals[1])].push_back(cid);
        } else if (c.literals.size() == 1) {
            c.watch1 = 0;
            c.watch2 = -1;

            watch_list[literal_id(c.literals[0])].push_back(cid);

            backtrack.push_back(c.literals[0]);
            assignment[abs(c.literals[0]) - 1] = c.literals[0] > 0 ? 1 : 0;
        }
    }
}

std::vector<int> Solver::get_assignment()
{
    return assignment;
}

Stats Solver::get_stats()
{
    return runtime_stats;
}

size_t Solver::get_num_clauses()
{
    return clauses.size();
}

/*
 * Puhtaan literaalin eliminaatio toimii, niin että
 * puhtas literaali on muuttuja joka esintyy ainoastaan positiivina tai negatiivina.
 * Jos literaali on puhdas voidaan tyydyttää kaikki klausuulit jotka sisältävät literaalin.
 */
void Solver::pure_literal_elimination()
{
    std::vector<bool> appears_positive(num_variables, false);
    std::vector<bool> appears_negative(num_variables, false);
    
    for (const auto& clause : clauses) {
        for (int literal : clause.literals) {
            int var = abs(literal) - 1;
            if (literal > 0) {
                appears_positive[var] = true;
            } else {
                appears_negative[var] = true;
            }
        }
    }
    
    for (int var = 0; var < num_variables; var++) {
        if (assignment[var] != -1) {
            continue;
        }
        
        bool is_positive = appears_positive[var];
        bool is_negative = appears_negative[var];
        
        if (is_positive && !is_negative) {
            assignment[var] = 1;
            backtrack.push_back(var + 1);
        } else if (is_negative && !is_positive) {
            assignment[var] = 0;
            backtrack.push_back(-(var + 1));
        }
    }
    
    auto new_end = std::remove_if(clauses.begin(), clauses.end(),
        [this](const Clause& clause) {
            for (int literal : clause.literals) {
                int var = abs(literal) - 1;
                if (assignment[var] != -1) {
                    bool literal_value = (literal > 0) ? (assignment[var] == 1) : (assignment[var] == 0);
                    if (literal_value) {
                        return true;
                    }
                }
            }
            return false;
        });
    clauses.erase(new_end, clauses.end());
}


/*
 *
 * Algoritmin periaate:
 * - Jokaisella lauseella on kaksi "watchia", indeksit watch1 ja watch2.
 * - watch_list[wid] sisältää listan kaikista lauseista, jotka tällä hetkellä 
 *   seuraavat (watchaavat) literaalia wid. Useilla lauseilla voi olla sama watched-literal.
 * - Kun muuttuja saa arvon, sen vastaluku tulee prosessoida.
 *
 * Pääsilmukan toiminta:
 * 1. Otetaan seuraava literal backtrack-jonosta.
 * 2. Haetaan kaikki lauseet, jotka watchaavat kyseisen literaalin vastalukua.
 * 3. Jokaiselle lauseelle yritetään siirtää watch:
 *    - Etsitään muu literaali, joka on true tai unassigned.
 *    - Jos löytyy, siirretään watch siihen ja poistetaan lauseen ID nykyisestä
 *      watch-listasta
 *    - Tämän vuoksi emme inkrementoi idx:ää, koska swap toi paikalle uuden elementin.
 * 4. Jos watchia ei voida siirtää elikä kaikki muut literaalit false:
 *    - Tarkistetaan toisen watchin tila:
 *      * Jos unassigned: tämä on unit clause -> asetetaan arvo ja lisätään backtrack-jonoon.
 *      * Jos false: palautetaan false -> backtrack vaaditaan.
 *      * Jos true: jatketaan seuraavaan lauseeseen.
 *
 * Lopuksi palauttaa true jos ei konflikti, false jos konflikti löytyi.
 */
bool Solver::propagate()
{
    runtime_stats.propagations++;
    while (propagate_idx < backtrack.size()) {
        int l = backtrack[propagate_idx++];
        int false_literal = -l;

        int wid = literal_id(false_literal);
    
        for (size_t idx = 0; idx < watch_list[wid].size(); ) {
            int cid = watch_list[wid][idx];

            Clause& clause = clauses[cid];
    
            int other_watch_index = (clause.literals[clause.watch1] == false_literal) ? clause.watch2 : clause.watch1;
            bool watch_moved = false;
    
            for (size_t i = 0; i < clause.literals.size(); i++) {
                if (i == clause.watch1 || i == clause.watch2) continue;
                int literal = clause.literals[i];
                int variable = abs(literal)-1;
    
                if (assignment[variable] == -1 || assignment[variable] == (literal > 0)) {
                    if (clause.literals[clause.watch1] == false_literal) {
                        clause.watch1 = i;
                    } else {
                        clause.watch2 = i;
                    }

                    watch_list[wid][idx] = watch_list[wid].back();
                    watch_list[wid].pop_back();
                    watch_list[literal_id(literal)].push_back(cid);
    
                    watch_moved = true;
                    break;
                }
            }
    
            if (!watch_moved) {
                int other_watch_variable = abs(clause.literals[other_watch_index]) - 1;

                if (assignment[other_watch_variable] == -1) {
                    int unit_literal = clause.literals[other_watch_index];
                    assignment[other_watch_variable] = (unit_literal > 0) ? 1 : 0;
                    backtrack.push_back(unit_literal);
                } else if (assignment[other_watch_variable] != (clause.literals[other_watch_index] > 0)) {
                    runtime_stats.backtracks++;
                    return false;
                }
                idx++;
            }
        }
    }
    return true;
}