/*! \file CGlobale_ModePiloteLaBotBox.cpp
    \brief Classe qui contient toute l'application pour le mode de fonctionnement pilote via Anaconbot
*/
#include "CGlobale.h"

//___________________________________________________________________________
/*!
   \brief Gestion du mode piloté via Labotbox

   \param --
   \return --
*/
void CGlobale::ModePiloteLaBotBox(void)
{
    while(1) {
        if (tick>0) {
            tick = 0;
            SequenceurModePiloteLaBotBox();
        }
    }
}

//___________________________________________________________________________
/*!
   \brief Sequenceur de taches en mode Labotbox

   \param --
   \return --
*/
void CGlobale::SequenceurModePiloteLaBotBox(void)
{
    static unsigned int cpt1msec= 0;
    static unsigned int cpt10msec = 0;
    static unsigned int cpt20msec = 0;
    static unsigned int cpt50msec = 0;
    static unsigned int cpt100msec = 0;
    static unsigned int cpt200msec = 0;
    static unsigned int cpt500msec = 0;
    static unsigned int cpt1sec = 0;

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
        //m_asservissement.CalculsMouvementsRobots();
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
    }

    // ______________________________
    cpt200msec++;
    if (cpt200msec >= TEMPO_200msec) {
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        cpt200msec = 0;
    }
    // ______________________________
    cpt500msec++;
    if (cpt500msec >= TEMPO_500msec) {
        cpt500msec = 0;
    }
    // ______________________________
    cpt1sec++;
    if (cpt1sec >= TEMPO_1sec) {
        cpt1sec = 0;
    }

}
