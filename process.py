import matplotlib.pyplot as plt
import numpy as np
from datetime import datetime
import os
import sys
os.nice(19)

def determination_fondamentale(frequence):
    plt.clf()
    counts, bins = np.histogram(frequence)
    plt.hist(frequence)
    plt.title("Histogramme Frequence "+ datetime.now().strftime("%d-%m-%Y %H:%M:%S"))
    variance = round(np.var(frequence), 3)
    mediane = round(np.median(frequence), 3)
    mids = 0.5*(bins[1:] + bins[:-1])
    moy = np.average(mids, weights=counts)
    bande = (moy - 25, moy + 25)
    plt.savefig("images/HistoFreq"+datetime.now().strftime("%d-%m-%Y--%H-%M-%S")+".png")
    return bande, mediane, variance

def determination_amplitude(amplitudes):
    """récupere l'amplitude médiane"""
    plt.clf()
    counts, bins = np.histogram(amplitudes)
    plt.hist(amplitudes)
    plt.title("Histogramme Amplitude "+ datetime.now().strftime("%d-%m-%Y %H:%M:%S"))
    variance = round(np.var(amplitudes), 3)
    mediane = round(np.median(amplitudes), 3)
    mids = 0.5*(bins[1:] + bins[:-1])
    moy = np.average(mids, weights=counts)

    plt.savefig("images/HistoAmp"+datetime.now().strftime("%d-%m-%Y--%H-%M-%S")+".png")
    return mediane, variance

def vitesse_de_rotation(frequence_fondamental):
    return round(frequence_fondamental*60/3, 2)

def variation_frequence(bande_de_reference, fondamentale_de_reference, bande_test, fondamentale_test):
    return round(abs((fondamentale_de_reference-fondamentale_test)/fondamentale_de_reference)*100,3) #donne la variation en %

def variation_amplitude(amplitudes_de_reference, amplitudes_a_tester):
    return round(abs(amplitudes_de_reference-amplitudes_a_tester)/amplitudes_de_reference*100,3)

def ouverture_log(fichier_log):
    with open(fichier_log, "r") as f:
        lignes = f.readlines()

    freq, amplitudes, variations_frequence, variations_amplitude, vit_de_rotation, variances, nbr_peak_amplitudes = [], [], [], [], [], [], []
    compteur, count = 0, 0

    for i in lignes[1:]:
        if i == "-----debut nouvel enregistrement------\n" or i == lignes[-1]:
            if count == 0:
                if freq: #verifie que l'enregistrement a bien fonctionne
                    bande_de_reference, fondamental_de_reference, variance_freq_ref = determination_fondamentale(freq)
                    amplitude_de_reference, variance_amp_ref = determination_amplitude(amplitudes)
                    variations_frequence.append( 0 )
                    variations_amplitude.append( 0 )
                    variances.append(variance_amp_ref)
                    vit_de_rotation.append(vitesse_de_rotation(fondamental_de_reference))
                    freq = [] #RAZ
                    amplitudes = []
                    nbr_peak_amplitudes.append(compteur)
                    compteur = 0
                    count+=1
                    continue;
                else:
                    f.close()
                    log_destruction()
                    sys.exit("Aucune donnee enregistree. Destruction du log. Relancer l'enregistrement.")
            else:
                if freq:
                    bande_test, fondamental_test, vairance_freq_test = determination_fondamentale(freq)
                    variations_frequence.append( variation_frequence(bande_de_reference, fondamental_de_reference, bande_test, fondamental_test) )
                    amp_test, variance_amp_test = determination_amplitude(amplitudes)
                    variations_amplitude.append( variation_amplitude(amplitude_de_reference, amp_test) )
                    vit_de_rotation.append(vitesse_de_rotation(fondamental_test))
                    nbr_peak_amplitudes.append(compteur)
                    variances.append(variance_amp_test)
                    compteur = 0
                    freq = [] #RAZ
                    amplitudes = []
                    count+=1
                    continue;
                else:
                    f.close()
                    log_destruction()
                    sys.exit("Aucune donnee enregistree. Destruction du log. Relancer l'enregistrement.")
        else : 
            freq.append(round(float(i.split(',')[1]), 3))
            amplitudes.append(round(float(i.split(',')[0]),3))
            compteur+=1
    print(nbr_peak_amplitudes)
    return variations_frequence, variations_amplitude, vit_de_rotation, nbr_peak_amplitudes, variances

def log_variation(variation_amplitude,variation_frequence, vit_de_rotation, nbr_peak_amplitudes, variances):
    date = datetime.now().strftime("%d/%m/%Y %H:%M:%S")
    with open("log_variation.txt","a") as f:
        f.write("------------------------------\n")
        f.write("{:<20}".format(date+"\n"))
        f.write("{:<15} {:<15} {:<15} {:<10}".format("", "Disque 1", "Disque 2", "Disque 3 \n"))
        f.write("{:<15} {:<15} {:<15} {:<15}".format("Variation f :", str(variation_frequence[0]), str(variation_frequence[1]), str(variation_frequence[2])+"              \n"))
        f.write("{:<15} {:<15} {:<15} {:<15}".format("Variation A :", str(variation_amplitude[0]), str(variation_amplitude[1]), str(variation_amplitude[2])+"              \n"))
        f.write("{:<15} {:<15} {:<15} {:<15}".format("Vit. Rot. :", str(vit_de_rotation[0]), str(vit_de_rotation[1]), str(vit_de_rotation[2])+"              \n"))
        f.write("{:<15} {:<15} {:<15} {:<15}".format("Variance :", str(variances[0]), str(variances[1]), str(variances[2])+"              \n"))
        f.write("{:<15} {:<15} {:<15} {:<15}".format("Peak :", str(nbr_peak_amplitudes[0]), str(nbr_peak_amplitudes[1]), str(nbr_peak_amplitudes[2])+"              \n"))
    return None

def regle_decision():
    variations_frequence = {"disque 1" : [], "disque 2": [], "disque 3" : []}
    variations_amplitude = {"disque 1" : [], "disque 2": [], "disque 3" : []}
    vit_rotation = {"disque 1" : [], "disque 2": [], "disque 3" : []}
    nbr_peak_amplitudes = {"disque 1" : [], "disque 2": [], "disque 3" : []}
    variances = {"disque 1" : [], "disque 2": [], "disque 3" : []}
    
    with open("log_variation.txt","r") as f:
        lignes = []
        for line in f:
            lignes.append(line)
            
    for j in range(len(lignes)):
        if "Disque" in lignes[j] or "/" in lignes[j] or "-" in lignes[j]:
            continue
        elif "f" in lignes[j]:
            variations_frequence["disque 1"].append(float(lignes[j][16:21]))
            variations_frequence["disque 2"].append(float(lignes[j][31:36]))
            variations_frequence["disque 3"].append(float(lignes[j][47:52]))
        elif "A" in lignes[j]:
            variations_amplitude["disque 1"].append(float(lignes[j][16:21]))
            variations_amplitude["disque 2"].append(float(lignes[j][31:36]))
            variations_amplitude["disque 3"].append(float(lignes[j][47:52]))
        elif "Rot." in lignes[j]:
            vit_rotation["disque 1"].append(float(lignes[j][16:24]))
            vit_rotation["disque 2"].append(float(lignes[j][31:39]))
            vit_rotation["disque 3"].append(float(lignes[j][47:55]))
        elif "Variance" in lignes[j]:
            variances["disque 1"].append(float(lignes[j][16:24]))
            variances["disque 2"].append(float(lignes[j][31:39]))
            variances["disque 3"].append(float(lignes[j][47:55]))
        elif "Peak" in lignes[j]:
            nbr_peak_amplitudes["disque 1"].append(float(lignes[j][16:24]))
            nbr_peak_amplitudes["disque 2"].append(float(lignes[j][31:39]))
            nbr_peak_amplitudes["disque 3"].append(float(lignes[j][47:55]))
      
    compteur = 0
    k = 0
    liste_5_derniers = []
    while k<len(list(variations_frequence.values())[0]):
        for q in range(3):
            liste_5_derniers.append([list(variations_frequence.values())[q][k], list(variations_amplitude.values())[q][k], list(vit_rotation.values())[q][k], list(variances.values())[q][k], list(nbr_peak_amplitudes.values())[q][k]])
        k+=1
    print(list(reversed(liste_5_derniers))[:15])

    for i in list(reversed(liste_5_derniers))[:15]:
        indx = liste_5_derniers.index(i)
        print("i = {} ".format(indx))
        print("reste = {} ".format(indx % 3))
        if ((indx % 3 == 0) and (indx+2 < len(liste_5_derniers)) and (i[0] > 7 or liste_5_derniers[indx+1][0] > 7 or liste_5_derniers[indx+2][0] > 7)) :
            if ((indx+2 < len(liste_5_derniers)) and (i[1] > 7 or liste_5_derniers[indx+1][1] > 7 or liste_5_derniers[indx+2][1] > 7)) :    
                if ((i[3] > 200  and i[4] > 60) or (liste_5_derniers[indx+1][3] > 200 and liste_5_derniers[indx+1][4] > 60) or (liste_5_derniers[indx+2][3] > 200 and liste_5_derniers[indx+2][4] > 60)) :
                    compteur+=1;

        elif i[2] < 2000:
            return "Erreur Donnée", compteur
    
    if compteur > 3:
        print(compteur)
        print("Appelez un berger")
        return "Berger", compteur
    print(compteur)
    return "Pas berger", compteur


def log_destruction():
    os.remove('log.txt')
    return None

if __name__ == '__main__':
	
	fichier = "log.txt"
	freq, amp, vit, peak, var = ouverture_log(fichier)
	print(freq, amp, vit, peak, var)
	log_variation(amp, freq, vit, peak, var)
	print(regle_decision())

	log_destruction()
