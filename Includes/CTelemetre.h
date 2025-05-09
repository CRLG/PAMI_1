#ifndef _TELEMETRE_H_
#define _TELEMETRE_H_

#include "CTelemetre.h"


// -----------------------------
//! Classe de gestion des options d'exécution passees en ligne de commande
class CTelemetre
{
public :
    CTelemetre();
    ~CTelemetre();

    void init();
    unsigned long getDistance();
    void periodicCall();

    static const unsigned long DISTANCE_ERREUR = 0xFFFFFFFF;
    static const unsigned int NBRE_MESURES_FAUSSES_AVANT_ERREUR = 3;

private :
    void _start();
    unsigned long MoyenneGlissante(unsigned long currentVal, unsigned long *buf_oldSamples, unsigned int samplesNumbers);

    static const unsigned int TAILLE_MOYENNE_GLISSANTE_TELEMETRE = 5;  // ajuster le nombre d'échantillons pour réaliser la moyenne
    unsigned long m_buff_moy[TAILLE_MOYENNE_GLISSANTE_TELEMETRE];
    unsigned long m_distance;
    unsigned int m_cpt_erreur;
};

#endif

// END
