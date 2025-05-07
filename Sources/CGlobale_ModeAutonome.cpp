/*! \file CGlobale_ModeAutonome.cpp
    \brief Classe qui contient toute l'application pour le mode de fonctionnement autonome
*/
#include "CGlobale.h"
#include <stdio.h>
#include "ConfigSpecifiqueCoupe.h"
//#include "ConfigSpecifiqueCoupe.h"



//___________________________________________________________________________
/*!
   \brief Point d'entrée pour l'execution de toute l'application

   \param --
   \return --
*/
void CGlobale::ModeAutonome(void)
{
    initStrategie();
    m_asservissement.Init();

    while(1) {
        if (tick) {
            tick = 0;
            SequenceurModeAutonome();
        }
    }
}

//___________________________________________________________________________
/*!
   \brief Sequenceur de taches en mode autonome

   \param --
   \return --
*/
void CGlobale::SequenceurModeAutonome(void)
{
    static unsigned int cpt1msec = 0;
    static unsigned int cpt10msec = 0;
    static unsigned int cpt20msec = 0;
    static unsigned int cpt50msec = 0;
    static unsigned int cpt100msec = 0;
    static unsigned int cpt200msec = 0;
    static unsigned int cpt500msec = 0;
    static unsigned int cpt1sec = 0;
    static bool old_match_en_cours=0;

    // ______________________________
    cpt1msec++;
    if (cpt1msec >= TEMPO_1msec) {
        cpt1msec = 0;
    }
    // ______________________________
    cpt10msec++;
    if (cpt10msec >= TEMPO_10msec) {
        cpt10msec = 0;

        //m_LaBotBox.Execute();
    }

    // ______________________________
    cpt20msec++;
    if (cpt20msec >= TEMPO_20msec) {
        cpt20msec = 0;

        toggleLedBuiltin();
        printf("Hello %ld\n\r", HAL_GetTick());

        Strategie();
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
void CGlobale::initStrategie()
{
	couleur_equipe=JAUNE;
    signe_equipe=1;
    fin_match=false;
    compteur_action=0;
    action_toggle=false;
    duree_match=-1;
    ETAPE=0;
    
    //Affichage à l'init
    m_lcd.Init();
    m_lcd.GotoXY (20,0);
    m_lcd.Puts ("CRLG", &Font_16x26, SSD1306::SSD1306_COLOR_WHITE);
    m_lcd.GotoXY (30, 40);
    if(couleur_equipe==JAUNE)
        m_lcd.Puts ("JAUNE", &Font_11x18, SSD1306::SSD1306_COLOR_WHITE);
    else
        m_lcd.Puts ("BLEU", &Font_11x18, SSD1306::SSD1306_COLOR_WHITE);
    m_lcd.UpdateScreen();
    
    
}

void CGlobale::Strategie()
{
    if (readAnalogVolt(1) < 1.)
    {
        if (duree_match < 0)
        {
            duree_match = 0; //je commence le match (init du temps t0)
            m_asservissement.setPosition_XYTeta(0.,0.,0.);
            m_asservissement.Init();
        }
        else
            duree_match = duree_match + 0.02;

        //on est à 85 secondes on lance la stratégie du PAMI
        if ((duree_match >= 85.) && (duree_match <= 100.))
        {
            //TODO: évaluer le distance de l'obstacle
            //distanceObstacles();

            //TODO: débloquer l'évitement
            //on teste les obstacles si on n'est pas hors range sur au moins un des 2
            /*
            if ((mesure_1.RangeMilliMeter <= SEUIL_OBSTACLE) || (mesure_2.RangeMilliMeter <= SEUIL_OBSTACLE))
            {
                m_asservissement.RobotStop();
            }
            else
            {
            */                
                switch (ETAPE)
                {
                    case 0:
                        if(!m_asservissement.newTarget)
                        {
                            //avancer de xx cm (jusqu'en haut de la pente)
                            m_asservissement.CommandeMouvementXY(150.,0.);
                        }
                        if(m_asservissement.coordonneesAtteintes)
                            ETAPE++;
                        break;  //fin de la première étape

                    case 1:
                        if(!m_asservissement.newTarget)
                        {
                            //tourner et avancer jusqu'au bord de la scène
                            m_asservissement.CommandeMouvementXY(150.,signe_equipe*(-30.));
                        }
                        if(m_asservissement.coordonneesAtteintes)
                            ETAPE++;
                        break;  //fin de la deuxième étape

                    default:
                        break;  //étape par défaut : on ne fait rien

                }  //fin de la machine à états

                //en dehors de la détection d'obstacle on execute toujours l'asservissement
                m_asservissement.executerAsservissement();
            /*
            }   //fin execution corps de match
            */
        }     //fin test fin de match
        else  //fin test 90 et 100 s
        {
            
            //actions à ne faire qu'une fois en fin de match
            if(fin_match==false)
            {
            	fin_match=true;
            	m_asservissement.Stop_robot();
            	
            	//Effacement de l'écran
                m_lcd.Fill(SSD1306::SSD1306_COLOR_BLACK );
                m_lcd.UpdateScreen();

                //Affichage du nombre de points
                m_lcd.GotoXY (20,0);
                m_lcd.Puts ("CRLG", &Font_16x26, SSD1306::SSD1306_COLOR_WHITE);
                m_lcd.GotoXY (30, 40);
                m_lcd.Puts ("40 PTS", &Font_11x18, SSD1306::SSD1306_COLOR_WHITE);
                m_lcd.UpdateScreen();
            }
            
            //actions à répéter en fin de match
            if(duree_match > 100.)
            {
            compteur_action=compteur_action+20;
                if (compteur_action>=500)
		{
		    compteur_action=0;
		    action_toggle=!action_toggle;
		    if (action_toggle)
		        CdeServo1(1000);
		    else
		        CdeServo1(1500);
		}
		}
        }

    }
    
    //Tant que la tirette n'est pas tirée, on peut changer de couleur
    else
    {
        //test du bouton de changement de couleur
        if(readAnalogVolt(2) < 1.)
        {
            //changement de couleur
            if(couleur_equipe==BLEU)
            {
                couleur_equipe = JAUNE;
                signe_equipe=1;
            }
            else
            {
                couleur_equipe = BLEU;
                signe_equipe=-1;
            }

            //Effacement de l'écran
            m_lcd.Fill(SSD1306::SSD1306_COLOR_BLACK );
            m_lcd.UpdateScreen();

            //Affichage de la couleur de l'équipe
            m_lcd.GotoXY (20,0);
            m_lcd.Puts ("CRLG", &Font_16x26, SSD1306::SSD1306_COLOR_WHITE);
            m_lcd.GotoXY (30, 40);
            if(couleur_equipe==JAUNE)
                m_lcd.Puts ("JAUNE", &Font_11x18, SSD1306::SSD1306_COLOR_WHITE);
            else
                m_lcd.Puts ("BLEU", &Font_11x18, SSD1306::SSD1306_COLOR_WHITE);
            m_lcd.UpdateScreen();
        }
    }
}
