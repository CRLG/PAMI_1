/*! \file CGlobale.h
        \brief Classe qui contient toute l'application
*/
#ifndef _GLOBALE_H_
#define _GLOBALE_H_

#include "RessourcesHardware.h"
#include "console_interactive_menu.h"
#include "ssd1306.h"
#include "CAsservissement.h"
#include "CTelemetre.h"

typedef enum {
    MODE_AUTONOME = 0,
    MODE_PILOTE_LABOTBOX,
    MODE_PILOTE_TERMINAL
}tModeFonctionnement;

// Pour le séquenceur de tâche
#define PERIODE_TICK    (1)
#define TEMPO_1msec     (1/PERIODE_TICK)
#define TEMPO_10msec    (10/PERIODE_TICK)
#define TEMPO_20msec    (20/PERIODE_TICK)
#define TEMPO_50msec    (50/PERIODE_TICK)
#define TEMPO_100msec   (100/PERIODE_TICK)
#define TEMPO_200msec   (200/PERIODE_TICK)
#define TEMPO_500msec   (500/PERIODE_TICK)
#define TEMPO_1sec      (1000/PERIODE_TICK)
#define TEMPO_2sec      (2000/PERIODE_TICK)
#define TEMPO_5sec      (5000/PERIODE_TICK)
#define TEMPO_10sec     (10000/PERIODE_TICK)
#define TEMPO_15sec     (15000/PERIODE_TICK)

#define POWER_ELECTROBOT_I2C_ADDR 0x54

// -----------------------------
//! Classe de gestion des options d'exécution passees en ligne de commande
class CGlobale {
public :
    //! Le mode de fonctionnement (autonome ou piloté via Anaconbot)
    unsigned int ModeFonctionnement;

    CGlobale();
    ~CGlobale();

    //! Point d'entree pour lancer l'application
    void Run(void);

    CMenuApp m_menu_interactive;
    SSD1306 m_lcd;
    CAsservissement m_asservissement;
    CTelemetre m_telemetre;


private : 

    //variables pour la stratégie
    int couleur_equipe;
    bool fin_match;
    int compteur_action;
    bool action_toggle;
    float duree_match;
    int ETAPE;
    float signe_equipe;

    //! Initialisation de la stratégie
    void initStrategie();
    //! pas élémentaire d'execution de la stratégie
    void Strategie();

    //! Gestion du mode autonome
    void ModeAutonome(void);
    //! Sequenceur de taches en mode autonome
    void SequenceurModeAutonome(void);
    //! Sequenceur de taches en mode pilote par Anaconbot
    void SequenceurModePiloteLaBotBox(void);
    //! Sequenceur de taches en mode pilote par teminal RS232
    void SequenceurModePiloteTerminal(void);



    //! Gestion du mode piloté via Anaconbot
    void ModePiloteLaBotBox(void);

    //! Gestion du mode piloté par terminal
    void ModePiloteTerminal(void);
    //! Reception RS232 en IRQ
    void ReceiveRS232_ModePiloteTerminal(void);

    //! Lecture eeprom
    void readEEPROM();
};

extern CGlobale Application;

#endif 



