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
  //Init();
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
    rayonRouesCodeuses = 2.4; // Rayon des roues codeuses (en cm)
    espacementRouesCodeuses = 9.5; // Distance entre les roues codeuses(en m)
    nbTicksParTourRouesCodeuses = 8192; // Résolution des codeurs
    dt = 0.02; // Pas de temps (en secondes)

    // initialisation des paramètres de l'asservissement
    // Constantes et variables pour le PID
    Kp_dist = 10.0, Ki_dist = 0.0, Kd_dist = 0.0; // PID distance
    Kp_angle = 250.0, Ki_angle = 0.0, Kd_angle = 0.0; // PID angle
    enableI_dist = false, enableD_dist = false; // Activation I/D pour distance
    enableI_angle = false, enableD_angle = false; // Activation I/D pour angle

    // Constantes matérielles
    maxMotorVoltage = 6.0; // Tension maximale des moteurs (en V)
    shieldVoltage = 6.5;  // Tension d'alimentation du shield (en V)
    zm_gauche=63.; //60
    zm_droite=63.; //59
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
    cde_gauche=0.;
    cde_droite=0.;

    //Erreurs
    erreur_angle = 0.0;
    erreur_distance = 0.0;

    //Erreurs (I & D)
    prevErrorDist = 0.0, sumErrorDist = 0.0;
    prevErrorAngle = 0.0, sumErrorAngle = 0.0;

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
    erreur_angle = 0.0;
    erreur_distance = 0.0;
    cde_gauche=0.0;
    cde_droite=0.0;
    angle_consigne=0.0;

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

void CAsservissement::stopAsservissement()
{
    coordonneesAtteintes=true;
    cde_droite=0.;
    cde_gauche=0.;
  Stop_robot();
  newTarget = false;
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
  return ((errorDist < 0.5) && (fabs(errorAngle) < 0.1)); // Tolérances
  //return abs(errorAngle)<0.1;
    return (errorDist < 0.5);
}

void CAsservissement::executerAsservissement()
{
    // Calculer la position actuelle
    short temp_d=0;
    short temp_g=0;

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
  currentTheta = BornageAngle(currentTheta);

  //reset des pas codeur
  ticksGauche=0;
  ticksDroite=0;
  angle_consigne=0.;
  erreur_distance=0.;

    //on n'utilise l'asservissement que si on demande une nouvelle coordonnées
    if(newTarget)
    {
        // Calcul de l'erreur distance
        erreur_distance = sqrt(pow(targetX - currentX, 2) + pow(targetY - currentY, 2));

        // Calcul de l'erreur distance
        //calcul de Guigui pour les 4 cadrans

        if ((targetX - currentX) >= 0)
        {
            if ((targetY - currentY) >= 0)
            {
                //cadran = 1;
                angle_consigne = asin(fabs((targetY - currentY) / erreur_distance));
            }
            else
            {
                //cadran = 2;
                angle_consigne = - asin(fabs((targetY - currentY) / erreur_distance));
            }
        }
        else
        {
            if ((targetY - currentY) >= 0)
            {
                //cadran = 3;
                angle_consigne = M_PI - asin(fabs((targetY - currentY) / erreur_distance));
            }
            else
            {
                //cadran = 4;
                angle_consigne = asin(fabs((targetY - currentY) / erreur_distance)) - M_PI;
            }
        }

        //angle_consigne=atan2(targetY - currentY,targetX - currentX);

        erreur_angle = angle_consigne - currentTheta;

        // Normaliser l'erreur angulaire entre -PI et PI
        erreur_angle = BornageAngle(erreur_angle);

        // Si le robot est proche du point de convergence et dans l'axe on impose une erreur d'angle nulle,
        //on désactive la stratégie si l'erreur en distance devient trop importante
        /*bool verouillage_approche=false;
        if ((fabs(erreur_distance) < 2) && (fabs(erreur_angle) < 0.1))
        {
            verouillage_approche = true; // Set verrouillage
        }

        if (fabs(erreur_distance) > 3)
        {
            verouillage_approche = false; // Reset le verrouillage
        }

        if (verouillage_approche)
        {
            erreur_angle = 0; // Si verrouillage, le robot est à peu près dans l'axe, reste à avancer un peu !
        }*/

      // PID pour l'angle
      float controlAngle = 0.;


      // Si l'angle est corrigé, on corrige la distance
      float controlDist = 0;
      if (fabs(erreur_angle) > 0.05) { // Tolérance sur l'angle (radians)
          controlAngle=calculPID(erreur_angle, &prevErrorAngle, &sumErrorAngle, Kp_angle, Ki_angle, Kd_angle, enableI_angle, enableD_angle);
      }
      else{
        controlDist = calculPID(erreur_distance, &prevErrorDist, &sumErrorDist, Kp_dist, Ki_dist, Kd_dist, enableI_dist, enableD_dist);
      }

      int sens_pos=(erreur_angle>0)?1:-1;

      // Commande des moteurs
      float leftSpeed = controlDist - sens_pos*controlAngle; // Différence pour tourner
      float rightSpeed = controlDist + sens_pos*controlAngle;

      cde_gauche=SAT(cde_max, cde_min, leftSpeed);
      cde_droite=SAT(cde_max, cde_min, rightSpeed);

      //correction des zones mortes
      if ((cde_droite >0.) && (cde_droite<zm_droite))
        cde_droite=zm_droite;
      if ((cde_droite <0.) && (cde_droite>((-1)*zm_droite)))
        cde_droite=((-1)*zm_droite);
      if ((cde_gauche >0.) && (cde_gauche<zm_gauche))
        cde_gauche=zm_gauche;
      if ((cde_gauche <0.) && (cde_gauche>((-1)*zm_gauche)))
        cde_gauche=((-1)*zm_gauche);


      CdeMoteur1(cde_droite);
      CdeMoteur2(cde_gauche);

      // Vérification de la position cible
      coordonneesAtteintes = ((erreur_distance<1)?true:false);

      // Arrêter les moteurs si la cible est atteinte
      if (coordonneesAtteintes)
          stopAsservissement();
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
    float ret=0.;

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
