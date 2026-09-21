<div align="center">

<img src="docs/banniere.png" alt="Lift Hill, montée du train" width="100%">

</div>

La montée est la méthode classique pour donner au train l'énergie dont il vivra tout le reste du circuit. Une chaîne l'accroche en bas, le hisse jusqu'en crête, puis le relâche. Le module pilote le moteur d'entraînement, surveille la vitesse réelle et détecte l'arrivée en haut.

Comme les autres modules, il se configure au premier démarrage par portail captif, puis rejoint le serveur en WebSocket.

**Version 0.1.0**

<img src="docs/sections/s01.png" alt="01 Principe" width="100%">

Un capteur à effet Hall sur l'arbre donne une impulsion par tour. Cette mesure sert deux fois : elle asservit la vitesse à la consigne, et elle détecte un blocage. Une chaîne qui force sans avancer ne produit plus d'impulsions, et le module coupe.

<img src="docs/schemas/etats.png" alt="ENGAGED : train détecté en pied de montée, la chaîne peut l'accrocher. LIFTING : montée en cours, vitesse asservie sur les impulsions du capteur Hall. CREST : crête atteinte, décélération puis relâche de la chaîne, avant le train suivant. FAULT : le capteur Hall se tait plus de STALL_TIMEOUT_MS, la chaîne force sans avancer, le moteur coupe." width="100%">

Les rampes de `RAMP_MS` évitent le démarrage sec : sans elles, la chaîne claque et le train est secoué au départ comme à l'arrivée.

<img src="docs/sections/s02.png" alt="02 Sécurité" width="100%">

**L'anti-retour est mécanique.** Le firmware le surveille, il ne le remplace pas. Un cliquet empêche la redescente, et aucune ligne de code ne doit servir de substitut à cette pièce.

**Une perte de liaison ne coupe pas le moteur en cours de montée.** C'est le seul module où l'état sûr n'est pas l'arrêt : un train immobilisé en pleine pente redescend. Si le WebSocket tombe pendant un `LIFTING`, la montée se termine, puis le module passe en attente.

**Deux délais bornent l'opération.** `STALL_TIMEOUT_MS` déclare le blocage si le capteur Hall se tait. `LIFT_TIMEOUT_MS` déclare le défaut si la crête n'est jamais atteinte.

<img src="docs/sections/s03.png" alt="03 Matériel" width="100%">

<img src="docs/schemas/brochage.png" alt="Sorties : GPIO 25 moteur PWM pour la vitesse de la chaîne, GPIO 26 sens de rotation, GPIO 27 activation du moteur, GPIO 2 LED marche pour l'état LIFTING, GPIO 4 LED défaut pour l'état FAULT. Entrées : GPIO 34 capteur Hall, une impulsion par tour, GPIO 35 capteur bas pour la présence en pied de montée, GPIO 32 capteur crête pour l'arrivée en haut." width="100%">

<img src="docs/sections/s04.png" alt="04 Réglages" width="100%">

<img src="docs/schemas/reglages.png" alt="LIFT_SPEED_PERCENT : vitesse de montée visée, trop basse la chaîne patine sous charge. RAMP_MS : douceur du départ et de l'arrivée en crête. STALL_TIMEOUT_MS : silence toléré du capteur Hall avant de déclarer le blocage. LIFT_TIMEOUT_MS : durée maximale d'une montée avant de passer en défaut." width="100%">

Une vitesse trop basse fait patiner la chaîne sous charge, une vitesse trop haute rend l'arrivée en crête sèche. `RAMP_MS` rattrape la seconde, pas la première.

<img src="docs/sections/s05.png" alt="05 Mise en service" width="100%">

Nécessite [PlatformIO](https://platformio.org/) dans Visual Studio Code.

<img src="docs/blocs/01.png" alt="Terminal bash : compiler et téléverser" width="100%">

```bash
pio run                  # compilation
pio run -t upload        # téléversement du firmware
pio run -t uploadfs      # téléversement du portail vers LittleFS
pio device monitor       # console série, 115200 bauds
```

1. Alimenter le module. Il crée un point d'accès WiFi.
2. S'y connecter et ouvrir `http://192.168.4.1`.
3. Renseigner le réseau de destination.
4. Le module redémarre, rejoint le réseau et s'annonce auprès du serveur.

Les identifiants restent en mémoire du module, jamais dans le dépôt.

<img src="docs/sections/s06.png" alt="06 Écosystème" width="100%">

Le squelette, le brochage et la machine à états sont posés dans `src/main.cpp`. Restent à écrire l'asservissement de vitesse, la détection de blocage sur interruption et la remontée de télémétrie.

Le socle commun à tous les modules est le [WiFi Manager](https://github.com/Microcoaster/MicroCoaster_WifiManager). L'autre manière de donner son énergie au train est le [Launch Track](https://github.com/Microcoaster/Launch-Track). Le pilotage se fait depuis la [WebApp](https://github.com/Microcoaster/MicroCoasterWebApp).

---

<sub>MicroCoaster · Auteur : Cybertrist</sub>
