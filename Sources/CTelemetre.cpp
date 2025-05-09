/*! \file CTelemetre.cpp
    \brief Classe qui contient les méthodes pour le dialogue avec la capteur ultrason SRF08
*/
#include "RessourcesHardware.h"
#include "CTelemetre.h"
#include "vl53l0x_api.h"

//___________________________________________________________________________
/*!
   \brief Constructeur

   \param --
   \return --
*/
CTelemetre::CTelemetre()
{
}

//___________________________________________________________________________
/*!
   \brief Destructeur

   \param --
   \return --
*/
CTelemetre::~CTelemetre()
{
}

// __________________________________________________
//!
//! \brief Fonction à appeler une fois au démarrage de l'application,
//!   une fois l'I2C configuré
//!
void CTelemetre::init()
{
    VL53L0X_Init(&I2C_HDL_ELECTROBOT);
    _start();
    for (unsigned int i=0; i<TAILLE_MOYENNE_GLISSANTE_TELEMETRE;i++) {
        m_buff_moy[i] = DISTANCE_ERREUR;
    }
}

// __________________________________________________
//!
//! \brief Fonaction appelée pour lancer les mesures
//!
void CTelemetre::_start()
{
    VL53L0X_StartContinuous(&I2C_HDL_ELECTROBOT);
    m_cpt_erreur = 0;
}

// __________________________________________________
//!
//! \brief Récupère la mesure de distance mise en forme
//! \return La distance en [mm] si tout s'est bien passé / DISTANCE_ERREUR en cas d'erreur
//!
unsigned long CTelemetre::getDistance()
{
    if (m_cpt_erreur >= NBRE_MESURES_FAUSSES_AVANT_ERREUR) return DISTANCE_ERREUR;
    return m_distance;
}

// __________________________________________________
//!
//! \brief Fonction à appeler périodiquement
//! Supprime les valeurs étranges observées en pratique
//! Moyenne la mesure
//! Redémarre les mesures si besoin
//!
void CTelemetre::periodicCall()
{
    uint16_t distance = VL53L0X_ReadDistanceMM();
    // cas particulier observé en pratique : on reçoit aléatoirement "0"
    // et une fois dans cet état, on reçoit toujours "O" -> solution : redémarrer les mesures
    if (distance == 0) {
        _start();
        if (m_cpt_erreur < 0xFFFF) m_cpt_erreur++;
    }
    else if (distance == 20) { // 20, c'est la valeur (fausse) obtenue aléatoirement
        if (m_cpt_erreur < 0xFFFF) m_cpt_erreur++;
    }
    else {
        m_distance = MoyenneGlissante(distance,
                                      m_buff_moy,
                                      TAILLE_MOYENNE_GLISSANTE_TELEMETRE);
        m_cpt_erreur = 0;
    }
}


//___________________________________________________________________________
/*!
   \brief Filrtage des donnees brutes des capteurs ultrason

   \param currentVal la valeur du dernier échantillon pour le calcul de la moyenne
   \param buf_oldSamples le buffer contenant les derniers échantillons de mesures
   \param samplesNumbers le nombre de point pour la moyenne glissante
   \return la valeur moeyennée
*/
unsigned long CTelemetre::MoyenneGlissante(unsigned long currentVal, unsigned long *buf_oldSamples, unsigned int samplesNumbers)
{
    unsigned long moy=currentVal;
    int i=0;  // Attention : doit être un "int" et non un "unsigned int" à cause du test de fin dans le "for"

    // Traite tous les échantillons sauf le 1er (index 0 du tableau) qui est un cas particulier
    for (i=(samplesNumbers-2); i>0; i--) {
        moy = moy + buf_oldSamples[i];
        buf_oldSamples[i] = buf_oldSamples[i-1];
    }

    // Cas particulier pour la 1ère case du tableau où la nouvelle valeur ne provient pas de l'index précédent du tableau mais du nouvel échantillon
    moy = moy + buf_oldSamples[0];
    buf_oldSamples[0] = currentVal;

    moy = moy / samplesNumbers;

    return(moy);
}


// END



