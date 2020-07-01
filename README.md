# stageVitirover
Approche logicielle à la détection d’anomalies mécaniques pour la micro-robotique agricole. Stage de deux mois.

### Installation de la librairie d'écoute et de calcul des TF
Il faut installer FFT Block : https://github.com/hosackm/FFT_Block
et remplacer les fichiers main.c, fft_block.c, fft_block.h par ceux qui sont fournis sur ce github. Les différences sont minimes. GNU plot est notamment enlevé pour diminuer la consommation d'énergie.
Chaque enregistrement récupère des données dans un premier fichier, sobrement appelé "log.txt", qui sont ensuite analysées par le fichier python.

### Programme en C
Le programme enregistre et calcul en même temps les transformées de Fourier sur une fenêtre de 10 secondes par disque. On récupère donc la fréquence et l'amplitude de chaque pic enregistré et calculé par la librairie FFT_BLOCK, dans un fichier Log. A partir de ce fichier log, en lançant ./process, mon programme retourne quasi-instantanément le statut des disques (endommagés ou non), à partir de l'analyse de la fréquence et de l'amplitude. 

Les résultats sont présentés dans un fichier texte logVariation.txt, avec la date (pour possiblement corréler la perte d'une dent avec un autre évènement), la variation de fréquence et d'amplitude (en %) entre les différents disques, la variance en amplitude et le nombre de pics en amplitudes de chaque disques. C'est deux derniers facteurs sont complémentaires de la variation en amplitude car lorsqu'une dent est cassée, le signal émis par le disque est très bruité (augmentation du nombre de pics en amplitude), et l'amplitude est bien plus forte (augmentation > 10%), causant dans le même temps une augmentation de la variance en amplitude.

### Rapport de Stage
Rapport de Stage disponible à ce lien : https://drive.google.com/file/d/1JbJ1Q1nl8LctG7Q9n7eQ_UL36xXSfhj5/view?usp=sharing
