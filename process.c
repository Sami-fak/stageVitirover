#include "process.h"

int main(int argc, char const *argv[])
{
	VariablesLog disques;

	ouvertureLog(&disques);
	int decision = regleDecision(&disques);
	logVariation(&disques, decision);
	int prevent = prevenirBerger();
	printf("Prevenir berger : %d\n", prevent);
	int destruction = remove("log_c.txt");
}

int vitesseDeRotation(int *frequenceFondamentale) {
	return *frequenceFondamentale*60/3;
	//On divise par 3 car on regarde la troisieme harmonique
}

int minimum(long* tableau, int taille) {
	int min = tableau[0];
	for (int i = 0; i < taille; ++i)
	{
		if (tableau[i]<min)
		{	
			min = tableau[i];
		}
	}

	return min;
}

int maximum(long* tableau, int taille) {
	int max = tableau[0];
	for (int i = 0; i < taille; ++i)
	{
		if (tableau[i]>max)
		{	
			max = tableau[i];
		}
	}

	return max;
}

float* binsCreation(long* tableau, int taille, int precision) {
	int min = minimum(tableau, taille);
	int max = maximum(tableau, taille);
	int range = max-min;
	float *bins = (float*)malloc((precision+1)*sizeof(float));
	if(!bins)
		return NULL;
	float decoupe = (float)range/precision;
	for (int i = 0; i < precision+1; ++i)
	{
		bins[i] =  min + (i * decoupe);
	}
	return bins;
}

float moyenne(float *bins, int *counts, int tailleTableau, int tailleBins) {
	float somme = 0;
	int taille = 0;
	for (int i = 0; i < tailleBins; ++i)
	{
		if (bins[i] > 10 && counts[i] != 0) {
			somme = somme + bins[i]*counts[i];
			taille = taille + counts[i];
		}
	}

	return somme/taille;
}

float variance(long* tableau, int tailleTableau, int moyenne) {
	float somme = 0;
	for (int i = 0; i < tailleTableau; ++i)
	{
		if (tableau[i] != 0)	
		{
			somme = somme + ((tableau[i] - moyenne) * (tableau[i] - moyenne));
		}
	}

	return somme/(tailleTableau);
}

int* countsHistogram(long *tableau, int tailleTableau, float bins[], int tailleBins) {
	int* counts = (int*)calloc(tailleBins+1, sizeof(int));
	if(!counts)
		return NULL;

	for (int i = 0; i < tailleTableau; ++i)
	{
		for (int j = 0; j < tailleBins-1; ++j)
		{
			if (tableau[i] >= bins[j] && tableau[i] <= bins[j+1])	
			{ 
				counts[j] += 1;
				break;
			} else {
				continue;
			}
		}
	}

	return counts;
}

float variation(int moyenneReference, int moyenneTest) {
	int difference = moyenneReference - moyenneTest;
	return abs(difference)/(float)moyenneReference*100;
}

void ouvertureLog(VariablesLog *disques) {
	long *frequences = (long*)calloc(TAILLE_MAX, sizeof(long));
	long *amplitudes = (long*)calloc(TAILLE_MAX, sizeof(long));
	char *ptr = NULL;
	char nouvelEnregistrement[STRING_LEN] = "-----debut nouvel enregistrement------\n";
	int i=0;
	int disque = 0;
	int precision = 30;
	int compteurPeak = 0;

	//declaration des variables
	int frequenceReference = 0;
	int amplitudeReference = 0;
		//Variables à comparer
	int frequenceTest = 0;
	int amplitudeTest = 0;
	float varianceAmplitude = 0;
	int tailleFrequenceTest = 0;
	int tailleAmplitudesTest = 0;


	//permet de savoir si on a affaire à une frequence (ligne paire) ou une amplitude (ligne impaire)
	FILE *f = NULL;
	f = fopen("log_c.txt", "r");
	if (!f) {
		printf("Fichier introuvable. Relancer l'enregistrement.\n");
		exit(-1);
	}

	char chaine[TAILLE_MAX];

	while (fgets(chaine, TAILLE_MAX, f) != NULL) {
		if (strcmp(chaine, nouvelEnregistrement) == 0) {
			if (disque == 0) {
				if (frequences[0] != 0)
				{	
					//taille des tableaux freq et amp
					int tailleFrequence = TAILLE_MAX;
					int tailleAmplitudes = TAILLE_MAX;

					//Creation des histogrammes respectifs
						//frequence
					float *binsFrequence = binsCreation(frequences, tailleFrequence, precision);
					int *countsFrequence = countsHistogram(frequences, tailleFrequence, binsFrequence, precision);

						//amplitudes
					float *binsAmplitude = binsCreation(amplitudes, tailleAmplitudes, precision);
					int *countsAmplitude = countsHistogram(amplitudes, tailleAmplitudes, binsAmplitude, precision);

					//Determination des frequences et amplitudes moyennes à laquelle nous allons comparer les autres disques.
					frequenceReference = moyenne(binsFrequence, countsFrequence, tailleFrequence, precision);
					amplitudeReference = moyenne(binsAmplitude, countsAmplitude, tailleAmplitudes, precision);
					//Calcul de la Variance des amplitudes
					varianceAmplitude = variance(amplitudes, tailleAmplitudes, amplitudeReference);

					//Ajout des valeurs dans la structure.
					disques->variationFrequence[disque] = 0;
					disques->variationAmplitude[disque] = 0;
					disques->nombreDePeak[disque] = compteurPeak;
					disques->varianceAmp[disque] = varianceAmplitude;
					disques->vitesseDeRotation[disque] = vitesseDeRotation(&frequenceReference);

					//RAZ pour prochains enregistrements.
					for (int j = 0; j < TAILLE_MAX; ++j)
					{
						frequences[j] = 0;
						amplitudes[j] = 0;
					}

					//Passage au prochain disque/enregistrement
					disque++;
					compteurPeak = 0;
					i = 0;
					safeFree(binsFrequence);
					safeFree(binsAmplitude);
					safeFree(countsFrequence);
					safeFree(countsAmplitude);
					
				} else {
					printf("Aucunes donnees enregistrees.\n");
					int destruction = remove("log_c.txt");
					exit(-1);
				}
			} else {
				if (frequences[0] != 0) {
					//taille des tableaux freq et amp
					tailleFrequenceTest = TAILLE_MAX;
					tailleAmplitudesTest = TAILLE_MAX;

					//Creqtion des histogrammes respectifs
						//frequence
					float *binsFrequenceTest = binsCreation(frequences, tailleFrequenceTest, precision);
					int *countsFrequenceTest = countsHistogram(frequences, tailleFrequenceTest, binsFrequenceTest, precision);

						//amplitudes
					float *binsAmplitudeTest = binsCreation(amplitudes, tailleAmplitudesTest, precision);
					int *countsAmplitudeTest = countsHistogram(amplitudes, tailleAmplitudesTest, binsAmplitudeTest, precision);

					//Determination des frequences et amplitudes moyennes à laquelle nous allons comparer les autres disques.
					frequenceTest = moyenne(binsFrequenceTest, countsFrequenceTest, tailleFrequenceTest, precision);
					amplitudeTest = moyenne(binsAmplitudeTest, countsAmplitudeTest, tailleAmplitudesTest, precision);

					//Calcul de la Variance des amplitudes
					varianceAmplitude = variance(amplitudes, tailleAmplitudesTest, amplitudeTest);

					//Ajout des valeurs dans la structure.
					disques->variationFrequence[disque] = variation(frequenceReference, frequenceTest);
					disques->variationAmplitude[disque] = variation(amplitudeReference, amplitudeTest);
					disques->nombreDePeak[disque] = compteurPeak;
					disques->varianceAmp[disque] = varianceAmplitude;
					disques->vitesseDeRotation[disque] = vitesseDeRotation(&frequenceTest);

					//RAZ pour prochains enregistrements.
					for (int j = 0; j < TAILLE_MAX; ++j)
					{
						frequences[j] = 0;
						amplitudes[j] = 0;
					}

					//Passage au prochain disque/enregistrement
					disque++;
					compteurPeak = 0;
					i = 0;
					safeFree(binsFrequenceTest);
					safeFree(binsAmplitudeTest);
					safeFree(countsFrequenceTest);
					safeFree(countsAmplitudeTest);
				} else {
					printf("Aucunes donnees enregistrees.\n");
					int destruction = remove("log_c.txt");
					exit(-1);
				}
			}
		} else {
			if (i % 2 == 0)
			{
				frequences[i] = strtol(chaine, &ptr, 10);
				ptr = NULL;
			} else {
				amplitudes[i] = strtol(chaine, &ptr, 10);
				ptr = NULL;
				compteurPeak++;
			}
			i++;
		}
	}	

	fclose(f);
	safeFree(frequences);
	safeFree(amplitudes);
}

void logVariation(VariablesLog *disques, int decision) {
	FILE *f = NULL;
	f = fopen("logVariation.txt", "a");
	if (!f) 
		exit(-1);
	int hours, minutes, seconds, day, month, year;
	time_t now;
	time(&now);
	fprintf(f, "--------------------\n");
	fprintf(f, "%s\n", ctime(&now));
	fprintf(f, "\t\t ");fprintf(f, "Disque 1 \t");fprintf(f, "Disque 2 \t");fprintf(f, "Disque 3 \n");
	fprintf(f, "Variation f. :\t"); fprintf(f, "%f\t", disques->variationFrequence[0]); fprintf(f, "%f\t", disques->variationFrequence[1]);fprintf(f, "%f\n", disques->variationFrequence[2]);
	fprintf(f, "Variation Amp :\t");fprintf(f, "%f\t", disques->variationAmplitude[0]); fprintf(f, "%f\t", disques->variationAmplitude[1]);fprintf(f, "%f\n", disques->variationAmplitude[2]);
	fprintf(f, "Vit. Rot. :\t");fprintf(f, "%d\t\t", disques->vitesseDeRotation[0]); fprintf(f, "%d\t\t", disques->vitesseDeRotation[1]);fprintf(f, "%d\n", disques->vitesseDeRotation[2]);
	fprintf(f, "Variance Amp :\t");fprintf(f, "%f\t", disques->varianceAmp[0]); fprintf(f, "%f\t", disques->varianceAmp[1]);fprintf(f, "%f\n", disques->varianceAmp[2]);
	fprintf(f, "Nbr Peak :\t");fprintf(f, "%d\t\t", disques->nombreDePeak[0]); fprintf(f, "%d\t\t", disques->nombreDePeak[1]);fprintf(f, "%d\n", disques->nombreDePeak[2]);
	fprintf(f, "Decision : %d\n", decision);
	fclose(f);
	printf("Donnees enregistrees\n");
}

//Retourne 1 si un probleme est detecté, 0 sinon.
int regleDecision(VariablesLog *disques) {
	if ((disques->variationAmplitude[1] > 14 || disques->variationAmplitude[2] > 14) 
		&& (disques->nombreDePeak[0] > 60 || disques->nombreDePeak[1] > 60 || disques->nombreDePeak[2] > 60) 
		&& (disques->varianceAmp[0] > 130 || disques->varianceAmp[1] > 130 || disques->varianceAmp[2] > 130)) {
		return 1;
	} else {
		return 0;
	}
}

int prevenirBerger() {
	FILE *f = NULL;
	int compteur = 0;
	f = fopen("logVariation.txt", "r");
	if (!f) 
		return -1;
	char chaine[TAILLE_MAX];
	while(fgets(chaine, TAILLE_MAX, f) != NULL) {
		if (strcmp(chaine, "Decision : 1\n") == 0)
		{
			compteur++;
		}
	}
	fclose(f);

	if (compteur > 3) {
		return 1;
	} else {
		return 0;
	}
}

void saferFree(void **pp) {
	if (pp != NULL && *pp != NULL) {
		free(*pp);
		*pp = NULL;
	}
}

