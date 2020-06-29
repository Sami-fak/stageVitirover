#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define safeFree(p) saferFree((void**)&(p))
#define TAILLE_MAX 400 // généralement le log dépasse rarement les 10 peaks. Donc je mets large pour l'instant meme si j'aimerai trouver une autre solution.
#define STRING_LEN 50

typedef struct VariablesLog
{
	//Tableaux de 3 parce que 3 disques.
	float variationFrequence[3];
	float variationAmplitude[3];
	int nombreDePeak[3];
	float varianceAmp[3];
	int vitesseDeRotation[3];
} VariablesLog;

void saferFree(void **pp);
int vitesseDeRotation(int *frequenceFondamentale);
int minimum(long * tableau, int taille); //retourne la valeur minimale d'un tablea
int maximum(long * tableau, int taille);
int* countsHistogram(
	long *tableau, 
	int tailleTableau, 
	float bins[], 
	int tailleBins
	);
float* binsCreation(
	long* tableau, 
	int taille, 
	int precision
	);
float moyenne(float *bins, int *counts, int tailleTableau, int tailleBins);
float variance(long* tableau, int tailleTableau, int moyenne);
float variation(int moyenneReference, int moyenneTest);
int regleDecision(VariablesLog *disques);
int prevenirBerger();
void ouvertureLog(VariablesLog *disques);
void logVariation(VariablesLog *disques, int decision);