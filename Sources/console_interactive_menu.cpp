/*! \file RessoucesHardware.cpp
    \brief Déclare les ressources hardware du MBED utilisées par le reste du logiciel
*/
#include "console_interactive_menu.h"
#include "RessourcesHardware.h"
#include "stdio.h"
#include "CGlobale.h"
#include "CAsservissement.h"

CMenuApp::CMenuApp()
{
    enable_echo(false);
    DECLARE_START_PAGE(CMenuApp::page1);
}

// ______________________________________________
void CMenuApp::send_to_console(char msg[])
{
    printf("%s", msg);
}

// ============================================================================
//                             LES MENUS
// ============================================================================
void CMenuApp::page1()
{
    DECLARE_PAGE("Menu Page 1", CMenuApp::page1);
    DECLARE_OPTION('a', "Commande moteurs", CMenuApp::page_cde_moteurs);
    DECLARE_OPTION('z', "Commande servo", CMenuApp::page_servos);
    DECLARE_OPTION('e', "Capteurs", CMenuApp::page_capteurs);
    DECLARE_OPTION('r', "I2C", CMenuApp::page_i2c);
    DECLARE_OPTION('t', "Asservissement", CMenuApp::page_asservissement);
}

void CMenuApp::page_cde_moteurs()
{
    DECLARE_PAGE("COMMANDE MOTEURS", CMenuApp::page_cde_moteurs);
    DECLARE_OPTION('0', "Retour en page d'accueil", CMenuApp::page1);
    DECLARE_ACTION('a', "Arrêt moteur G", CMenuApp::action_moteurG_stop);
    DECLARE_ACTION('z', "Moteur G : 10%", CMenuApp::action_moteurG_10pct);
    DECLARE_ACTION('e', "Moteur G : 50%", CMenuApp::action_moteurG_50pct);
    DECLARE_ACTION('r', "Moteur G : 100%", CMenuApp::action_moteurG_100pct);
    DECLARE_ACTION('s', "Moteur G : -10%", CMenuApp::action_moteurG_M10pct);
    DECLARE_ACTION('d', "Moteur G : -50%", CMenuApp::action_moteurG_M50pct);
    DECLARE_ACTION('f', "Moteur G : -100%", CMenuApp::action_moteurG_M100pct);

    DECLARE_ACTION('u', "Arrêt moteur D", CMenuApp::action_moteurD_stop);
    DECLARE_ACTION('i', "Moteur D : 10%", CMenuApp::action_moteurD_10pct);
    DECLARE_ACTION('o', "Moteur D : 50%", CMenuApp::action_moteurD_50pct);
    DECLARE_ACTION('p', "Moteur D : 100%", CMenuApp::action_moteurD_100pct);
    DECLARE_ACTION('k', "Moteur D : -10%", CMenuApp::action_moteurD_M10pct);
    DECLARE_ACTION('l', "Moteur D : -50%", CMenuApp::action_moteurD_M50pct);
    DECLARE_ACTION('m', "Moteur D : -100%", CMenuApp::action_moteurD_M100pct);
}

void CMenuApp::page_capteurs()
{
    DECLARE_PAGE("Capteurs", CMenuApp::page_capteurs);
    DECLARE_OPTION('0', "Retour en page d'accueil", CMenuApp::page1);
    DECLARE_ACTION('a', "Etat des codeurs", CMenuApp::read_codeurs);
    DECLARE_ACTION('z', "Etat des entrees analogiques", CMenuApp::read_analog_inputs);
    DECLARE_ACTION('t', "Distance télémètre", CMenuApp::read_telemetre);
}

void CMenuApp::page_servos()
{
    DECLARE_PAGE("SERVO MOTEURS", CMenuApp::page_servos);
    DECLARE_OPTION('0', "Retour en page d'accueil", CMenuApp::page1);
    DECLARE_ACTION('a', "Servo1 : 1500", CMenuApp::page_servo1_1500);
    DECLARE_ACTION('z', "Servo1 : 1200", CMenuApp::page_servo1_1200);
    DECLARE_ACTION('e', "Servo1 : 1700", CMenuApp::page_servo1_1700);
    DECLARE_ACTION('r', "Servo1 : 2000", CMenuApp::page_servo1_2000);

    DECLARE_ACTION('q', "Servo2 : 1500", CMenuApp::page_servo2_1500);
    DECLARE_ACTION('s', "Servo2 : 1200", CMenuApp::page_servo2_1200);
    DECLARE_ACTION('d', "Servo2 : 1700", CMenuApp::page_servo2_1700);
    DECLARE_ACTION('f', "Servo2 : 2000", CMenuApp::page_servo2_2000);

    DECLARE_ACTION('w', "Servo3 : 1500", CMenuApp::page_servo3_1500);
    DECLARE_ACTION('x', "Servo3 : 1200", CMenuApp::page_servo3_1200);
    DECLARE_ACTION('c', "Servo3 : 1700", CMenuApp::page_servo3_1700);
    DECLARE_ACTION('v', "Servo3 : 2000", CMenuApp::page_servo3_2000);

}

void CMenuApp::page_asservissement () {
    DECLARE_PAGE("ASSERVISSEMENT", CMenuApp::page_asservissement);
    DECLARE_OPTION('0', "Retour en page d'accueil", CMenuApp::page1);
    DECLARE_ACTION('x', "STOP", CMenuApp::asser_stop);
    DECLARE_ACTION('c', "RESET ASSERVISSEMENT", CMenuApp::asser_resetPos);
    DECLARE_ACTION('a', "+10 cm", CMenuApp::asser_avant10);
    DECLARE_ACTION('z', "+50 cm", CMenuApp::asser_avant50);
    DECLARE_ACTION('e', "+100 cm", CMenuApp::asser_avant100);

    DECLARE_ACTION('r', "-10 cm", CMenuApp::asser_arriere10);
    DECLARE_ACTION('t', "-50 cm", CMenuApp::asser_arriere50);
    DECLARE_ACTION('y', "-100 cm", CMenuApp::asser_arriere100);

    DECLARE_ACTION('u', "Afficher x/y", CMenuApp::get_xy_asser);
}

void CMenuApp::page_set_param_1()
{
    DECLARE_PAGE("Forçage paramètre Param1", CMenuApp::page_set_param_1)
            DECLARE_ACTION('r', "Lecture du paramètre", CMenuApp::action_read_param1)
            DECLARE_OPTION('q', "Retour en page principale", CMenuApp::page1)
            DECLARE_ACTION_DOUBLE("Entrez une valeur pour Param1", CMenuApp::action_set_param1)
}

void CMenuApp::page_set_param_2()
{
    DECLARE_PAGE("Forçage paramètre Param2", CMenuApp::page_set_param_2)
            DECLARE_ACTION('r', "Lecture du paramètre", CMenuApp::action_read_param2)
            DECLARE_OPTION('q', "Retour en page principale", CMenuApp::page_set_param_1)
            DECLARE_ACTION_INT("Entrez une valeur pour Param2", CMenuApp::action_set_param2)
}


// ============================================================================
//                             LES ACTIONS
// ============================================================================
bool CMenuApp::action_set_param1(double val)
{
    _printf("Changement de la valeur du paramètre Param1: %f\n\r", val);
    m_param1 = val;
    return true;
}

bool CMenuApp::action_read_param1()
{
    _printf("Valeur du Param1: %f\n\r", m_param1);
    return true;
}

bool CMenuApp::action_set_param2(int val)
{
    _printf("Changement de la valeur du paramètre Param2: %d\n\r", val);
    m_param2 = val;
    return true;
}

bool CMenuApp::action_read_param2()
{
    _printf("Valeur du Param2: %d\n\r", m_param2);
    return true;
}

bool CMenuApp::action_read_params()
{
    _printf("Valeur du Param1: %f\n\r", m_param1);
    _printf("Valeur du Param2: %d\n\r", m_param2);
    return true;
}


// ===========================================================
//                  MOTEURS
// ===========================================================

bool CMenuApp::action_moteurG_stop()
{
    CdeMoteur1(0);
    return true;
}

bool CMenuApp::action_moteurG_10pct()
{
    CdeMoteur1(10);
    return true;
}

bool CMenuApp::action_moteurG_50pct()
{
    CdeMoteur1(60);
    return true;
}

bool CMenuApp::action_moteurG_100pct()
{
    CdeMoteur1(100);
    return true;
}

bool CMenuApp::action_moteurG_M10pct()
{
    CdeMoteur1(-10);
    return true;
}

bool CMenuApp::action_moteurG_M50pct()
{
    CdeMoteur1(-50);
    return true;
}

bool CMenuApp::action_moteurG_M100pct()
{
    CdeMoteur1(-100);
    return true;
}


bool CMenuApp::action_moteurD_stop()
{
    CdeMoteur2(0);
    return true;
}

bool CMenuApp::action_moteurD_10pct()
{
    CdeMoteur2(10);
    return true;
}

bool CMenuApp::action_moteurD_50pct()
{
    CdeMoteur2(59);
    return true;
}

bool CMenuApp::action_moteurD_100pct()
{
    CdeMoteur2(100);
    return true;
}

bool CMenuApp::action_moteurD_M10pct()
{
    CdeMoteur2(-10);
    return true;
}

bool CMenuApp::action_moteurD_M50pct()
{
    CdeMoteur2(-50);
    return true;
}

bool CMenuApp::action_moteurD_M100pct()
{
    CdeMoteur2(-100);
    return true;
}

// ===========================================================
//                  SERVO MOTEURS
// ===========================================================
bool CMenuApp::page_servo1_1200()
{
    CdeServo1(1200);
    return true;
}

bool CMenuApp::page_servo1_1500()
{
    CdeServo1(1500);
    return true;
}

bool CMenuApp::page_servo1_1700()
{
    CdeServo1(1700);
    return true;
}

bool CMenuApp::page_servo1_2000()
{
    CdeServo1(2000);
    return true;
}

bool CMenuApp::page_servo2_1200()
{
    CdeServo2(1200);
    return true;
}

bool CMenuApp::page_servo2_1500()
{
    CdeServo2(1500);
    return true;
}

bool CMenuApp::page_servo2_1700()
{
    CdeServo2(1700);
    return true;
}

bool CMenuApp::page_servo2_2000()
{
    CdeServo2(2000);
    return true;
}


bool CMenuApp::page_servo3_1200()
{
    CdeServo3(1200);
    return true;
}

bool CMenuApp::page_servo3_1500()
{
    CdeServo3(1500);
    return true;
}

bool CMenuApp::page_servo3_1700()
{
    CdeServo3(1700);
    return true;
}

bool CMenuApp::page_servo3_2000()
{
    CdeServo3(2000);
    return true;
}

// ===========================================================
//                  CAPTEURS
// ===========================================================

bool CMenuApp::read_codeurs()
{
    _printf("Codeur1: %d / Codeur2: %d\n\r", getCodeur1(), getCodeur2());
    return true;
}

bool CMenuApp::read_analog_inputs()
{
    _printf("Eana1=%.3fV(%d) / Eana2=%.3fV(%d) / Eana3=%.3fV(%d) / Eana4=%.3fV(%d)\n\r",
            readAnalogVolt(1), readAnalog(1),
            readAnalogVolt(2), readAnalog(2),
            readAnalogVolt(3), readAnalog(3),
            readAnalogVolt(4), readAnalog(4));
    return true;
}


bool CMenuApp::read_telemetre()
{
    unsigned long dist = Application.m_telemetre.getDistance();
    if (dist != CTelemetre::DISTANCE_ERREUR) printf("D=%ld mm\n\r", dist);
    else printf("Erreur de distance\n\r");
    return true;
}

// ===========================================================
//                  ASSERVISSEMENT
// ===========================================================
bool CMenuApp::asser_stop () {
    Application.m_asservissement.stopAsservissement();
    return true;
}

bool CMenuApp::asser_resetPos()
{
    Application.m_asservissement.stopAsservissement();
    Application.m_asservissement.Initialisation_PID();
    Application.m_asservissement.setPosition_XYTeta(0.,0.,0.);
    return true;

}

bool CMenuApp::asser_avant10 () {
    //Application.m_asservissement.setPosition_XYTeta(0.,0.,0.);
    Application.m_asservissement.CommandeMouvementXY(10.,0.);
    //Application.m_asservissement.newTarget = true;

//    // Asservissement actif tant que la target n'est pas atteinte
//    while (!CMenuApp::m_asservissement.coordonneesAtteintes) {
//        static unsigned int cpt20msec = 0;
//        cpt20msec++;
//        if (cpt20msec >= TEMPO_20msec) {
//            cpt20msec = 0;

//            toggleLedBuiltin();
//            m_asservissement.executerAsservissement();
//        }
//    }
    return true;
}

bool CMenuApp::asser_avant50 () {
    //Application.m_asservissement.setPosition_XYTeta(0.,0.,0.);
    Application.m_asservissement.CommandeMouvementXY(50.,0.);

//    // Asservissement actif tant que la target n'est pas atteinte
//    while (!CMenuApp::m_asservissement.coordonneesAtteintes) {
//        static unsigned int cpt20msec = 0;
//        cpt20msec++;
//        if (cpt20msec >= TEMPO_20msec) {
//            cpt20msec = 0;

//            toggleLedBuiltin();
//            m_asservissement.executerAsservissement();
//        }
//    }
    return true;
}

bool CMenuApp::asser_avant100 () {
    //Application.m_asservissement.setPosition_XYTeta(0.,0.,0.);
    Application.m_asservissement.CommandeMouvementXY(100.0,0.0);

//    static unsigned int cpt20msec = 0;
//    // Asservissement actif tant que la target n'est pas atteinte
//    while (!CMenuApp::m_asservissement.coordonneesAtteintes) {
//        cpt20msec++;
//        if (cpt20msec >= TEMPO_20msec) {
//            cpt20msec = 0;

//            toggleLedBuiltin();
//            m_asservissement.executerAsservissement();
//        }
//    }
    return true;
}

bool CMenuApp::asser_arriere10 () {
    Application.m_asservissement.setPosition_XYTeta(0.,0.,0.);
    Application.m_asservissement.CommandeMouvementXY(-10.,0.);
    Application.m_asservissement.newTarget = true;

//    // Asservissement actif tant que la target n'est pas atteinte
//    while (!CMenuApp::m_asservissement.coordonneesAtteintes) {
//        static unsigned int cpt20msec = 0;
//        cpt20msec++;
//        if (cpt20msec >= TEMPO_20msec) {
//            cpt20msec = 0;

//            toggleLedBuiltin();
//            m_asservissement.executerAsservissement();
//        }
//    }
    return true;
}

bool CMenuApp::asser_arriere50 () {
    Application.m_asservissement.setPosition_XYTeta(0.,0.,0.);
    Application.m_asservissement.CommandeMouvementXY(-50.,0.);

//    // Asservissement actif tant que la target n'est pas atteinte
//    while (!CMenuApp::m_asservissement.coordonneesAtteintes) {
//        static unsigned int cpt20msec = 0;
//        cpt20msec++;
//        if (cpt20msec >= TEMPO_20msec) {
//            cpt20msec = 0;

//            toggleLedBuiltin();
//            m_asservissement.executerAsservissement();
//        }
//    }
    return true;
}

bool CMenuApp::asser_arriere100 () {
    Application.m_asservissement.setPosition_XYTeta(0.,0.,0.);
    Application.m_asservissement.CommandeMouvementXY(-100.0,0.0);

//    static unsigned int cpt20msec = 0;
//    // Asservissement actif tant que la target n'est pas atteinte
//    while (!CMenuApp::m_asservissement.coordonneesAtteintes) {
//        cpt20msec++;
//        if (cpt20msec >= TEMPO_20msec) {
//            cpt20msec = 0;

//            toggleLedBuiltin();
//            m_asservissement.executerAsservissement();
//        }
//    }
    return true;
}

bool CMenuApp::get_xy_asser () {
    //m_asservissement.currentX = 10.1;
    _printf("x = %f, y = %f, theta=%f\n\r", Application.m_asservissement.currentX, Application.m_asservissement.currentY, Application.m_asservissement.currentTheta);
    _printf("erreur dist = %f\t\terreur angle = %f\n\r", Application.m_asservissement.erreur_distance, Application.m_asservissement.erreur_angle);
    _printf("cde gauche = %f\t\tcde droite = %f\n\r", Application.m_asservissement.cde_gauche, Application.m_asservissement.cde_droite);
    _printf("convergence = %d, demande mouvement = %d\n\r", Application.m_asservissement.coordonneesAtteintes, Application.m_asservissement.newTarget);
    return true;
}

// =============================================================================
//                          I2C
// =============================================================================
void CMenuApp::page_i2c()
{
    DECLARE_PAGE("I2C", CMenuApp::page_i2c);
    DECLARE_ACTION('s', "Scan I2C", CMenuApp::i2c_action_scan);
}

bool CMenuApp::i2c_action_scan()
{
    uint8_t data;
    _printf("SCAN I2C\n\r");

    // 1. présentation du scan sous une forme linéaire des calculateurs présents
    for (unsigned int addr=0; addr<0xFF; addr++)
    {
        if (HAL_I2C_Master_Transmit(&I2C_HDL_ELECTROBOT, addr, &data, 1, I2C_DEFAULT_TIMEOUT) == HAL_OK) {
            printf(" Echo on 0x%x\n\r", addr);
        }
    }

    // 1. présentation du scan sous une forme de tableau
    _printf("\n\r      ");
    for (unsigned int i=0; i<16; i++) _printf("%x   ", i);
    for (unsigned int addr=0; addr<=0xFF; addr++)
    {
        if ((addr%16) == 0) printf("\n\r0x%02x", addr);
        if (HAL_I2C_Master_Transmit(&I2C_HDL_ELECTROBOT, addr, &data, 1, I2C_DEFAULT_TIMEOUT) == HAL_OK) {
            _printf("  OK", addr);
        }
        else {
            _printf("  --", addr);
        }
    }
    _printf("\n\r");

    return true;
}
