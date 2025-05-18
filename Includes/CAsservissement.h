/*! \file CAsservissement.h
    \brief Classe qui contient l'asservissement vitesse/position du robot
*/

#ifndef _ASSERVISSEMENT_H_
#define _ASSERVISSEMENT_H_

// -----------------------------
//! Classe de gestion des options d'exécution passees en ligne de commande
class CAsservissement {
public :
    typedef enum {
        cMODE_MANUEL = 0,
        cMODE_XY_AUTO,
        cMODE_XY_AUTO_A,
        cMODE_XY_AUTO_B,
        cMODE_DISTANCE_ANGLE,
        cMODE_XY_TETA
    } tTypeAsservissement;

    // codeur
    short ticksGauche;  //variable qui croit ou decroit selon la rotation du codeur gauche
    short ticksDroite;  //variable qui croit ou decroit selon la rotation du codeur deltaDroiteoit
    short ticksGauche_prec;
    short ticksDroite_prec;

    //calculs d'asservissement
    bool convergence_distance;
    bool convergence_angle;
    bool convergence;
    float distance_consigne;
    float angle_consigne;
    float rayonRouesCodeuses; // Rayon des roues codeuses (en m)
    float espacementRouesCodeuses; // Distance entre les roues codeuses(en m)
    int nbTicksParTourRouesCodeuses; // Résolution des codeurs

    float erreur_angle;
    float erreur_distance;

    float targetAngle;

    float dt; // Pas de temps (en secondes)

    float cde_min;
    float cde_max;
    float zm_gauche;
    float zm_droite;

    float cde_gauche;
    float cde_droite;

    float maxMotorVoltage; // Tension maximale des moteurs (en V)
    float shieldVoltage;  // Tension d'alimentation du shield (en V)

    // Constantes et variables pour le PID
    float Kp_dist, Ki_dist, Kd_dist; // PID distance
    float Kp_angle, Ki_angle, Kd_angle; // PID angle
    bool enableI_dist , enableD_dist; // Activation I/D pour distance
    bool enableI_angle , enableD_angle ; // Activation I/D pour angle

    float prevErrorDist , sumErrorDist ;
    float prevErrorAngle , sumErrorAngle ;

    // Variables pour la position et la cible
    float currentX , currentY , currentTheta ; // Position actuelle
    float targetX , targetY , targetTheta ; // Consigne



    // États
    bool coordonneesAtteintes;
    bool newTarget = false; // Indique si une nouvelle cible est définie


    //! Constructeur / destructeur
    CAsservissement();
    ~CAsservissement();

    //! Réinitilise tous les paramètres et valeurs
    void Init(void);

    // Prototype des fonctions
    void executerAsservissement();
    void CommandeMouvementXY(float x, float y);
    void setPosition_XYTeta(float x, float y, float teta);
    void setCdeMinCdeMax(int min, int max);
    void Stop_robot(void);
    void stopAsservissement();
    void Initialisation_PID(void);
protected :
    float max(float Inp1, float Inp2);
    float min(float Inp1, float Inp2);
    float SAT(float Inp_max, float Inp_min, float Inp);
    float BornageAngle(float angle);     //! Bornage de l'angle pour les mouvement distance/angle
    float calculPID(float error, float *prevError, float *sumError, float Kp, float Ki, float Kd, bool enableI, bool enableD);
    bool isTargetReached(float errorDist, float errorAngle);
};


#endif


