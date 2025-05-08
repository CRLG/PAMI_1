//			Asservissement Générique
//
//	Modifications à valider ou calibrer :
//
//	Diag de blocage à calibrer
//	Calcul du coeff facteur_correction_avance_angle ne fonctionne pas correctement en passant pas la carto ?
//	Correction batterie à valider
//	Mouvement XY_TETA à valider
//	Initialisation des zones mortes suppr (en commentaire)
//	Flag de convergence rapide à valider
//	Todo : Diag de blocage par écart de boucle ?

/*! \file CAsservissement.cpp
	\brief Classe qui contient toute l'application
*/
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include "math.h"
#include "CAsservissement.h"
#include "CGlobale.h"

//___________________________________________________________________________
 /*!
   \brief Constructeur

   \param --
   \return --
*/
CAsservissement::CAsservissement()
{
  Init();
}


//___________________________________________________________________________
 /*!
   \brief Destructeur

   \param --
   \return --
*/
CAsservissement::~CAsservissement()
{

}

//___________________________________________________________________________
 /*!
   \brief Initialisation

   \param --
   \return --
*/
void CAsservissement::Init(void)
{
    //Initialisations des variables dimensionnantes pour l'asservissement
    rayonRouesCodeuses = 0.04625/2.0; // Rayon des roues codeuses (en m)
    espacementRouesCodeuses = 0.224; // Distance entre les roues codeuses(en m)
    nbTicksParTourRouesCodeuses = 1200; // Résolution des codeurs
    dt = 0.02; // Pas de temps (en secondes)

    // initialisation des paramètres de l'asservissement
    // Constantes et variables pour le PID
    Kp_dist = 7.0, Ki_dist = 0.0, Kd_dist = 0.0; // PID distance
    Kp_angle = 1.0, Ki_angle = 0.0, Kd_angle = 0.0; // PID angle
    enableI_dist = false, enableD_dist = false; // Activation I/D pour distance
    enableI_angle = false, enableD_angle = false; // Activation I/D pour angle

    // Constantes matérielles
    maxMotorVoltage = 6.0; // Tension maximale des moteurs (en V)
    shieldVoltage = 12.0;  // Tension d'alimentation du shield (en V)
    cde_min=-80;
    cde_max=80;

    // codeur
    ticksGauche = 0;  //variable qui croit ou decroit selon la rotation du codeur gauche
    ticksDroite = 0;  //variable qui croit ou decroit selon la rotation du codeur deltaDroiteoit
    ticksGauche_prec = 0;
    ticksDroite_prec = 0;

    //calculs d'asservissement
    convergence_distance = true;
    convergence_angle = true;
    convergence = true;
    distance_consigne = 0.;
    angle_consigne = 0.;

    //Erreurs
    erreur_gauche = 0.0;  // erreur pour le codeur 1
    erreur_droite = 0.0;  // erreur pour le codeur 2
    erreur_angle = 0.0;

    //Erreurs (I & D)
    prevErrorDist = 0.0, sumErrorDist = 0.0;
    prevErrorAngle = 0.0, sumErrorAngle = 0.0;

    targetAngle = 0;

    // Variables pour la position et la cible
    //currentX = 0, currentY = 0, currentTheta = 0; // Position actuelle
    //targetX = 10, targetY = 0, targetTheta = 0; // Consigne

    // États
    coordonneesAtteintes = true;
    newTarget = false; // Indique si une nouvelle cible est définie
}

//*******************************************************************************************
//									Initialisation des PID
//*******************************************************************************************

void CAsservissement::Initialisation_PID(void)
{
    //calculs d'asservissement
    coordonneesAtteintes = false;
    newTarget=false;

    //Erreurs
    erreur_gauche = 0.0;  // erreur pour le codeur 1
    erreur_droite = 0.0;  // erreur pour le codeur 2
    erreur_angle = 0.0;

    //Erreurs (I & D)
    prevErrorDist = 0.0, sumErrorDist = 0.0;
    prevErrorAngle = 0.0, sumErrorAngle = 0.0;			// TODO

    ticksDroite_prec=getCodeur1();
    ticksGauche_prec=getCodeur2();
}


//*******************************************************************************************
//								Arrêt de la commande brutale
//*******************************************************************************************

void CAsservissement::Stop_robot(void)
{
    CdeMoteur1(0);
    CdeMoteur2(0);
}


// Fonction de calcul du PID
float CAsservissement::calculPID(float error, float *prevError, float *sumError, float Kp, float Ki, float Kd, bool enableI, bool enableD) {
  float P = Kp * error;
  float I = enableI ? Ki * (*sumError) : 0.0;
  float D = enableD ? Kd * (error - *prevError) / dt : 0.0;

  *sumError += error * dt;
  *prevError = error;

  return P + I + D;
}

// Fonction pour vérifier si la cible est atteinte
bool CAsservissement::isTargetReached(float errorDist, float errorAngle) {
  return errorDist < 0.05 && abs(errorAngle) < 0.1; // Tolérances
  //return abs(errorAngle)<0.1;
}

void CAsservissement::executerAsservissement()
{
    // Calculer la position actuelle
    long temp_d=0;
    long temp_g=0;
    temp_d=getCodeur1();
    temp_g=getCodeur2();
    ticksDroite=temp_d-ticksDroite_prec;
    ticksGauche=temp_g-ticksGauche_prec;
    ticksDroite_prec=temp_d;
    ticksGauche_prec=temp_g;

      float deltaGauche = (ticksGauche * 2 * M_PI * rayonRouesCodeuses) / nbTicksParTourRouesCodeuses;
      float deltaDroite = (ticksDroite * 2 * M_PI * rayonRouesCodeuses) / nbTicksParTourRouesCodeuses;
      float dTheta = (deltaDroite - deltaGauche) / espacementRouesCodeuses;
      float dX = (deltaGauche + deltaDroite) / 2.0 * cos(currentTheta + dTheta / 2.0);
      float dY = (deltaGauche + deltaDroite) / 2.0 * sin(currentTheta + dTheta / 2.0);

      currentX += dX;
      currentY += dY;
      currentTheta += dTheta;

      //reset des pas codeur
      ticksGauche=0;
      ticksDroite=0;

      //on n'utilise l'asservissement que si on demande une nouvelle coordonnées
      if(newTarget)
      {
          // Calcul des erreurs
          float errorDist = sqrt(pow(targetX - currentX, 2) + pow(targetY - currentY, 2));
          float targetAngle = atan2(targetY - currentY, targetX - currentX);
          float errorAngle = targetAngle - currentTheta;

          // Normaliser l'erreur angulaire entre -PI et PI
          errorAngle = BornageAngle(errorAngle);

          // PID pour l'angle
          float controlAngle = calculPID(errorAngle, &prevErrorAngle, &sumErrorAngle, Kp_angle, Ki_angle, Kd_angle, enableI_angle, enableD_angle);


          // Si l'angle est corrigé, corrigez la distance
          float controlDist = 0;
          if (abs(errorAngle) < 0.1) { // Tolérance sur l'angle (radians)
            controlDist = calculPID(errorDist, &prevErrorDist, &sumErrorDist, Kp_dist, Ki_dist, Kd_dist, enableI_dist, enableD_dist);
          }

          int sens_pos=(errorAngle>0)?1:-1;

          // Commande des moteurs
          float leftSpeed = controlDist - sens_pos*controlAngle; // Différence pour tourner
          float rightSpeed = controlDist + sens_pos*controlAngle;

          leftSpeed=SAT(cde_max, cde_min, leftSpeed);
          rightSpeed=SAT(cde_max, cde_min, rightSpeed);

          CdeMoteur1(leftSpeed);
          CdeMoteur2(rightSpeed);

          // Vérification de la position cible
          coordonneesAtteintes = isTargetReached(errorDist, errorAngle);

          // Arrêter les moteurs si la cible est atteinte
          if (coordonneesAtteintes) {
            Stop_robot();
            newTarget = false;
          }
      }
}



// ************************************************************************************
// Fonctions externes de calcul 
// ************************************************************************************

float CAsservissement::max(float Inp1, float Inp2)
{
	float Outp;

	if (Inp1 >= Inp2)		Outp=Inp1;
	else		Outp=Inp2;

	return Outp;
}

float CAsservissement::min(float Inp1, float Inp2)
{
	float Outp;

	if (Inp1 <= Inp2)		Outp=Inp1;
	else		Outp=Inp2;

	return Outp;
}

float CAsservissement::SAT(float Inp_max, float Inp_min, float Inp)
{
	return 	min(Inp_max,max(Inp,Inp_min));
}


float  CAsservissement::BornageAngle(float angle){
    float ret=0;

    if (angle > M_PI){
        ret = angle - (2.0*M_PI);
    }
    else if (angle <= (-1.0*M_PI)){
        ret = angle + (2*M_PI);
    }
    else{ // pas de saturation
        ret = angle;
    }

    return(ret);
}

void CAsservissement::CommandeMouvementXY(float x, float y)
{
    Initialisation_PID();	// Permet de ne pas avoir l'effet mémoire de la commande du mouvement précédent
    coordonneesAtteintes=false;
    newTarget=true;
    targetX = x;
    targetY = y;
    targetTheta = 0;
}

// -----------------------------------------------------------------------------------------------------------------------
// Fonction qui initialise la position du robot avec des valeurs données
void CAsservissement::setPosition_XYTeta(float x, float y, float teta)
{
    currentX =x;
    currentY =y;
    currentTheta =teta;
}


// -----------------------------------------------------------------------------------------------------------------------
// Fixe les paramètres cde_min et cde_max de l'asserv
void CAsservissement::setCdeMinCdeMax(int min, int max)
{
    cde_min = min;
    cde_max = max;
}
