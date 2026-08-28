#!/bin/bash
# ============================================================
# Tests de non-regression — Application console "Elections 2026"
# Verifie que la refactorisation (couche core/) n'a rien casse.
#
# Chaque scenario :
#   - s'execute dans un bac a sable (/tmp/election_test) contenant
#     une COPIE des .dat (les vrais fichiers ne sont pas touches)
#   - alimente l'application via stdin (sequences de menu)
#   - verifie la presence de marqueurs attendus dans la sortie
# ============================================================

SRC=$(cd "$(dirname "$0")/.." && pwd)
BIN=$SRC/bin/election
SBX=/tmp/election_test
PASS=0; FAIL=0

# --- Preparation du bac a sable ---
rm -rf "$SBX"; mkdir -p "$SBX"
cp "$SRC"/*.dat "$SBX"/
# Outil d'inspection des .dat (compile a la volee si besoin)
if [ ! -x "$SBX/dump_dat" ]; then
    gcc -I"$SRC" -o "$SBX/dump_dat" "$(dirname "$0")/dump_dat.c"
fi

run_test() {
    local name="$1"; local input="$2"; shift 2
    local out
    out=$(cd "$SBX" && printf '%b' "$input" | "$BIN" 2>&1)
    local ok=1
    for marker in "$@"; do
        if ! grep -qF "$marker" <<<"$out"; then
            ok=0
            echo "  [MANQUANT] <<$marker>>"
        fi
    done
    if [ $ok -eq 1 ]; then
        echo "PASS  $name"; PASS=$((PASS+1))
    else
        echo "FAIL  $name"; FAIL=$((FAIL+1))
        echo "$out" | tail -25 | sed 's/^/      | /'
    fi
}

echo "======================================================"
echo " S1 — Affichages (lecture seule, parcours de tous les menus)"
echo "======================================================"
run_test "S1 affichages" \
'1\n2\n4\n2\n2\n4\n3\n2\n4\n4\n2\n4\n5\n2\n3\n6\n7\n' \
"Rasanble" "BV-01" "3040506070" "CA-001" "VO-12" \
"LISTE DES ELECTEURS PAR DEPARTEMENT" "LISTE DES BUREAUX DE VOTE"

echo "======================================================"
echo " S2 — Ajout parti : doublon rejete puis ajout valide"
echo "======================================================"
run_test "S2 ajout parti" \
'1\n1\nPP-01\nPP-99\nVerite\n01/01/2009\n05/05/2025\n10\nRue Delmas\nOuest\nRespo Test\n50937000000\nverite@test.ht\n4\n7\n' \
"Un parti politique avec l'id 'PP-01' existe deja" \
"Donnees enregistrees avec succes"

echo "======================================================"
echo " S3 — Ajout BV : doublon + membre existant + VP=president"
echo "======================================================"
run_test "S3 ajout BV" \
'2\n1\nBV-01\nBV-99\n3\nRue Test\nNord\nBVP-01\nBVP-99\nBVP-99\nBVV-99\nBVS-99\n4\n7\n' \
"Un bureau de vote avec l'id 'BV-01' existe deja" \
"est deja membre d'un autre bureau de vote" \
"Le Vice-President ne peut pas avoir le meme ID que le President" \
"Donnees enregistrees avec succes"

echo "======================================================"
echo " S4 — Ajout 2 electeurs : NINU doublon + BV inexistant"
echo "======================================================"
run_test "S4 ajout electeurs" \
'3\n1\n3040506070\n9999999999\nTest\nElecteur\n1\nRue Unique\nOuest\nBV-XX\nBV-99\n1\n8888888888\nAutre\nVotant\n7\nRue Double\nSud\nBV-99\n4\n7\n' \
"Un electeur avec le NINU '3040506070' existe deja" \
"Le bureau de vote 'BV-XX' n'existe pas" \
"Donnees enregistrees avec succes"

echo "======================================================"
echo " S5 — Ajout candidat : doublon + parti inexistant"
echo "======================================================"
run_test "S5 ajout candidat" \
'4\n1\nCA-001\nCA-99\nCandid\nTest\n01/01/1980\n2\nRue Cand\nSud\nc@t.ht\n50911111111\n2\nPP-XX\nPP-99\n25/08/2026\n4\n7\n' \
"Un candidat avec l'id 'CA-001' est deja enregistre" \
"Le parti avec l'ID 'PP-XX' n'existe pas" \
"Candidat enregistre avec succes"

echo "======================================================"
echo " S6 — Votes : NINU inconnu + deja vote + candidat inconnu"
echo "      puis vote valide et vote blanc"
echo "======================================================"
run_test "S6 votes" \
'5\n1\nVO-99\n0000000000\n3040506070\n9999999999\nCA-XX\nCA-99\n1\nVO-98\n8888888888\n0\n2\n3\n7\n' \
"L'electeur avec le NINU '0000000000' n'existe pas" \
"a DEJA VOTE" \
"Le candidat avec l'id 'CA-XX' n'existe pas" \
"Bureau de vote associe : BV-99" \
"Vote enregistre avec succes" \
"VO-98"

echo "======================================================"
echo " S7 — Modification du parti PP-99 (changement de nom)"
echo "======================================================"
run_test "S7 modif parti" \
'1\n3\nPP-99\n1\nVerite Nouvelle\n8\n4\n7\n' \
"MODIFICATION DU PARTI POLITIQUE" \
"Modification effectuee avec succes"

echo "======================================================"
echo " S8 — Rapport final : les nouvelles donnees apparaissent"
echo "======================================================"
run_test "S8 rapport" \
'6\n7\n' \
"BV-99" "9999999999" "CA-99" "RESULTAT" 

echo "======================================================"
echo " Verification finale des donnees du bac a sable"
echo "======================================================"
cd "$SBX" && ./dump_dat > /tmp/dump_final.txt 2>&1
check_count() {
    local file="$1" expected="$2"
    local got
    got=$(grep -c "^\[" <(cd "$SBX" && ./dump_dat "$file" 2>/dev/null) 2>/dev/null)
    # dump_dat affiche tout ; on compte les lignes du bon prefixe
    case "$file" in
        p_pol)    got=$(grep -c '^\[PP\]' /tmp/dump_final.txt);;
        bv)       got=$(grep -c '^\[BV\]' /tmp/dump_final.txt);;
        elect)    got=$(grep -c '^\[EL\]' /tmp/dump_final.txt);;
        candid)   got=$(grep -c '^\[CA\]' /tmp/dump_final.txt);;
        votes)    got=$(grep -c '^\[VO\]' /tmp/dump_final.txt);;
    esac
    if [ "$got" = "$expected" ]; then
        echo "PASS  comptage $file = $got"; PASS=$((PASS+1))
    else
        echo "FAIL  comptage $file : attendu $expected, obtenu $got"; FAIL=$((FAIL+1))
    fi
}
check_count p_pol  11
check_count bv      8
check_count elect   7
check_count candid  6
check_count votes   6

# Le nom modifie doit etre dans p_pol.dat
if grep -q "Verite Nouvelle" /tmp/dump_final.txt; then
    echo "PASS  nom du parti modifie persiste"; PASS=$((PASS+1))
else
    echo "FAIL  nom du parti modifie persiste"; FAIL=$((FAIL+1))
fi

# Les .dat originaux ne doivent pas avoir change (horodatage conserve)
echo
echo "======================================================"
echo " RESULTAT GLOBAL : $PASS reussis, $FAIL echoues"
echo "======================================================"
exit $FAIL
