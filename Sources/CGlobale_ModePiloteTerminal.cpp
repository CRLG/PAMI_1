/*! \file CGlobale_ModePiloteTerminal.cpp
    \brief Classe qui contient toute l'application pour le mode de fonctionnement pilote via Anaconbot
*/
#include <stdio.h>
#include "CGlobale.h"
#include "OTOS_XYTeta.h"

//___________________________________________________________________________
/*!
   \brief Gestion du mode piloté via Anaconbot

   \param --
   \return --
*/
void CGlobale::ModePiloteTerminal(void)
{
    while(1) {
        if (tick) {
            tick = 0;
            SequenceurModePiloteTerminal();
        }
    }
}

// _____________________________________________________________
void CGlobale::ReceiveRS232_ModePiloteTerminal(void)
{
    char rxData;
    //rxData = _rs232_pc_rx.getc();
}


//___________________________________________________________________________
/*!
   \brief Sequenceur de taches en mode autonome

   \param --
   \return --
*/
void CGlobale::SequenceurModePiloteTerminal(void)
{
    static unsigned int cpt1msec = 0;
    static unsigned int cpt10msec = 0;
    static unsigned int cpt20msec = 0;
    static unsigned int cpt50msec = 0;
    static unsigned int cpt100msec = 0;
    static unsigned int cpt200msec = 0;
    static unsigned int cpt500msec = 0;
    static unsigned int cpt1sec = 0;

    static unsigned char compteur=1;
    static unsigned char toggle=0;

    // ______________________________
    cpt1msec++;
    if (cpt1msec >= TEMPO_1msec) {
        cpt1msec = 0;
    }

    // ______________________________
    cpt10msec++;
    if (cpt10msec >= TEMPO_10msec) {
        cpt10msec = 0;

    }

    // ______________________________
    cpt20msec++;
    if (cpt20msec >= TEMPO_20msec) {
        cpt20msec = 0;

        m_telemetre.periodicCall();
        //m_asservissement.CalculsMouvementsRobots();
        m_asservissement.executerAsservissement();
    }


    // ______________________________
    cpt50msec++;
    if (cpt50msec >= TEMPO_50msec) {
        cpt50msec = 0;
    }

    // ______________________________
    cpt100msec++;
    if (cpt100msec >= TEMPO_100msec) {
        cpt100msec = 0;

        otos_error_t err;
    	otos_pose2d_t pos;
    	err = Application.m_otos_xyteta.getPosition(pos);
    	printf("{%f, %f, %f}\n\r", pos.x, pos.y, pos.h);
    }

    // ______________________________
    cpt200msec++;
    if (cpt200msec >= TEMPO_200msec) {
        cpt200msec = 0;
    }
    // ______________________________
    cpt500msec++;
    if (cpt500msec >= TEMPO_500msec) {
        cpt500msec = 0;

        toggleLedBuiltin();
    }
    // ______________________________
    cpt1sec++;
    if (cpt1sec >= TEMPO_1sec) {
        cpt1sec = 0;
    }

}
