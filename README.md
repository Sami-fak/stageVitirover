# stageVitirover
Approche logicielle à la détection d’anomalies mécaniques pour la micro-robotique agricole. Stage de deux mois.

La lecture du rapport et notamment de la partie II et III est recommandé pour la compréhension du problème et de la méthode de résolution.

### Installation de la librairie d'écoute et de calcul des TF
Il faut installer FFT Block : https://github.com/hosackm/FFT_Block
et remplacer les fichiers main.c, fft_block.c, fft_block.h par ceux qui sont fournis sur ce github. Les différences sont minimes. GNU plot est notamment enlevé pour diminuer la consommation d'énergie.
Chaque enregistrement récupère des données dans un premier fichier, sobrement appelé "log.txt", qui sont ensuite analysées par le fichier python.

### Processus en python.
Le processus d'analyse écrit en python est contenu dans le fichier process.py. Il retourne la variation de fréquence, d'amplitude, la vitesse moyenne de rotation des moteurs par minutes, la variance des pics en amplitudes ainsi que le nombre de pics en amplitude, dans un deuxième fichier log appelé "log_variation.txt". Un "log_variation" est associé à chaque robot. Il retourne aussi un compteur qui compte sur les 5 derniers enregistrements le nombre de fois où il entend un disque cassé. Si ce compteur dépasse 3 (arbitraire pour l'instant) alors un message est envoyé au berger.

### Utilisation sur raspberry pi
Pour l'utiliser sur Raspberry pi, je recommande l'installation de berryconda (https://github.com/jjhelmus/berryconda), qui permet d'avoir tous les modules nécessaires à l'analyse. Il faut ensuite créer un environnement virtuel sur sa mahcine :

```
conda create -n environnement python=python3.x matplotlib numpy
```
