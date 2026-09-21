<div align="center">

<img src="docs/banniere.png" alt="Lift Hill, montée du train" width="100%">

</div>

La montée est la méthode classique pour donner au train l'énergie dont il vivra tout le reste du circuit. Une chaîne l'accroche en bas, le hisse jusqu'en crête, puis le relâche. Le module pilote le moteur d'entraînement, surveille la vitesse réelle et détecte l'arrivée en haut.

Comme les autres modules, il se configure au premier démarrage par portail captif, puis rejoint le serveur en WebSocket.

## Principe

Un capteur à effet Hall sur l'arbre donne une impulsion par tour. Cette mesure sert deux fois : elle asservit la vitesse à la consigne, et elle détecte un blocage. Une chaîne qui force sans avancer ne produit plus d'impulsions, et le module coupe.

```
Train en pied      état ENGAGED
Ordre reçu         rampe d'accélération, état LIFTING
Crête atteinte     rampe de décélération, chaîne relâchée, état CREST
Plus d'impulsion   coupure immédiate, état FAULT
```

Les rampes de `RAMP_MS` évitent le démarrage sec : sans elles, la chaîne claque et le train est secoué au départ comme à l'arrivée.

## Sécurité

**L'anti-retour est mécanique.** Le firmware le surveille, il ne le remplace pas. Un cliquet empêche la redescente, et aucune ligne de code ne doit servir de substitut à cette pièce.

**Une perte de liaison ne coupe pas le moteur en cours de montée.** C'est le seul module où l'état sûr n'est pas l'arrêt : un train immobilisé en pleine pente redescend. Si le WebSocket tombe pendant un `LIFTING`, la montée se termine, puis le module passe en attente.

**Deux délais bornent l'opération.** `STALL_TIMEOUT_MS` déclare le blocage si le capteur Hall se tait. `LIFT_TIMEOUT_MS` déclare le défaut si la crête n'est jamais atteinte.

## Matériel

| Élément | Broche | Rôle |
|:--|:--|:--|
| Moteur, rapport cyclique | 25 | Vitesse de la chaîne |
| Moteur, sens | 26 | Sens de rotation |
| Moteur, activation | 27 | Coupure de puissance |
| Capteur Hall | 34 | Une impulsion par tour |
| Capteur bas | 35 | Présence en pied de montée |
| Capteur crête | 32 | Arrivée en haut |
| LED marche | 2 | État `LIFTING` |
| LED défaut | 4 | État `FAULT` |

## Compiler et téléverser

Nécessite [PlatformIO](https://platformio.org/) dans Visual Studio Code.

```bash
pio run                  # compilation
pio run -t upload        # téléversement du firmware
pio run -t uploadfs      # téléversement du portail vers LittleFS
pio device monitor       # console série, 115200 bauds
```

## Première mise en service

1. Alimenter le module. Il crée un point d'accès WiFi.
2. S'y connecter et ouvrir `http://192.168.4.1`.
3. Renseigner le réseau de destination.
4. Le module redémarre, rejoint le réseau et s'annonce auprès du serveur.

Les identifiants restent en mémoire du module, jamais dans le dépôt.

## État

Version `0.1.0`. Le squelette, le brochage et la machine à états sont posés dans `src/main.cpp`. Restent à écrire l'asservissement de vitesse, la détection de blocage sur interruption et la remontée de télémétrie.

---

<sub>MicroCoaster · Auteurs : CyberSpaceRS, Yamakajump</sub>
