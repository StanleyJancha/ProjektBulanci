# Projekt: Bulánci (Klon)

### Jak hru sestavit (Build)

1. Otevřete terminál v hlavní složce projektu (tam, kde je `CMakeLists.txt`).
2. Zadejte příkaz: `cmake .`
3. Následně hru zkompilujte: `cmake --build .`

---

### Jak hrát

**Hlavní menu**

* **Scoreboard:** Na levé straně uvidíte TOP 5 nejlepších hráčů podle poměru Kills/Deaths.
* **Ovládání:** Tlačítka pro zahájení zápasu (**Start**) nebo opuštění hry (**Exit**).

**Nastavení zápasu (Pre-game menu)**

* **Zadání jména:** Kliknutím označte textové pole (textfield) a zadejte jméno.
* **Boti:** Pole, která necháte prázdná, budou automaticky obsazena boty.
* **Start:** Hra se spustí tlačítkem **"Begin match"** (vyžaduje alespoň jedno zadané jméno).
* **Identifikace:** Barva postavy odpovídá čtverečku vedle vašeho textového pole. U pole jsou také vypsány klávesy pro ovládání.

**Průběh a konec hry**

* Ovládáte postavu pomocí přiřazených kláves a snažíte se eliminovat soupeře.
* Po vypršení časového limitu se zobrazí tabulka s výsledky všech hráčů i botů.
* Pokud dosáhnete dostatečného skóre, váš výsledek se uloží do globálního scoreboardu v hlavním menu.

---

### Ukládání dat

* **TOP 5:** Hra ukládá nejlepší historické výsledky podle KD.
* **Historie:** Ukládá se tabulka s výsledky jednotlivých odehraných her.

### Nastavení parametrů
V souboru main.c na řádku 42 je možné nastavit parametry hry:
* **gameLengthMinutes:** Délka trvání jednoho zápasu v minutách.
* **playerRespawnTimeSec:** Doba v sekundách, za kterou se hráč po smrti znovu narodí.
* **botTickEveryNthFrame:** Určuje frekvenci rozhodování botů (každý N-tý snímek hry).