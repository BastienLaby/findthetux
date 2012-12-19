#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL_image.h>


/* ---------------------------------------------------------------- */
/* --------------- VARIABLES GLOBALES ET STRUCTURES --------------- */
/* --------------------------------------------------------------- */
	
	/* Dimensions de la fenêtre */
	static unsigned int WINDOW_WIDTH = 800;
	static unsigned int WINDOW_HEIGHT = 600;

	/* Nombre de bits par pixel de la fenêtre */
	static const unsigned int BIT_PER_PIXEL = 32;

	/* Nombre minimal de millisecondes separant le rendu de deux images */
	static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;


/* --------------------------------------------------------------- */
/* -------------------------- FONCTIONS -------------------------- */
/* --------------------------------------------------------------- */

void dessinCarre(int full){
	GLenum type;
	if(full==0){
		type=GL_LINE_LOOP;
	}
	else{
		type=GL_QUADS;
	}
	glBegin(type);
		glVertex2f(-0.5, 0.5);
		glVertex2f(0.5, 0.5);
		glVertex2f(0.5, -0.5);
		glVertex2f(-0.5, -0.5);
	glEnd();
}

GLenum getFormat(SDL_Surface* p){
	GLenum format; // Récupération du format de l'image
	switch(p->format->BytesPerPixel){
	case 1 : 
		format = GL_RED;
		break;
	case 3 :
		format = GL_RGB;
		break;
	case 4 :
		format = GL_RGBA;
		break;
	default :
		fprintf(stderr, "Format des pixels de l'image non pris en charge \n");
		return EXIT_FAILURE;
	}
	return format;
}

int rand_a_b(int a, int b){
	return rand()%(b-a) +a;
}

int isEqual(int a, int b, int c){
	if (a == b){
		return 1;
	}
	if(a == c){
		return 1;
	}
	if(b == c){
		return 1;
	}
	return 0;
}

/* Fonction qui retourne quel carré a été cliqué parmis les trois. Renvoie 0 si carré gauche, 1 si carré milieu, 2 si carré droit, -1 si aucun carré */
int getClikedCarre(float xpos, float ypos){

	/* Carre gauche */
	if (xpos<=-3 && xpos >= -7 && ypos<=2.5 && ypos>=-2.5){
		return 0;
	}

	/* Carre mid */
	if (xpos<=2 && xpos >= -2 && ypos<=2.5 && ypos>=-2.5){
		return 1;
	}

	/* Carre droit */
	if (xpos<=7 && xpos >= 3 && ypos<=2.5 && ypos>=-2.5){
		return 2;
	}
	return -1;

}

void switchState(int* p_home, int* p_tips1, int* p_tips2, int* p_tips3, int* p_game){
	if(*p_home){
		*p_home = 0; *p_tips1 = 1;
		return;
	}
	if(*p_tips1){
		*p_tips1 = 0; *p_tips2 = 1;
		return;
	}
	if(*p_tips2){
		*p_tips2 = 0; *p_tips3 = 1;
		return;
	}
	if(*p_tips3){
		*p_tips3 = 0; *p_game = 1;
		return;
	}
}

/* --------------------------------------------------------------- */
/* ----------------------------- MAIN ---------------------------- */
/* --------------------------------------------------------------- */

	int main(int argc, char** argv) {

	  /* Initialisation de la SDL */
	  if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
	    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
	    return EXIT_FAILURE;
	  }
	  
	  /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
	  if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL)) {
	    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
	    return EXIT_FAILURE;
	  }
	  
	  /* Titre de la fenêtre */
	  SDL_WM_SetCaption("textures", NULL);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		/* le repère virtuel */
		gluOrtho2D(-8, 8, -6, 6.);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		/* Variables */
		int i;
		float xpos, ypos;
		int stateHOME = 1;
		int stateTIPS1 = 0;
		int stateTIPS2 = 0;
		int stateTIPS3 = 0;
		int stateGAME = 0;
		int stateWIN = 0;
		int stateLOOSE = 0;

		/* ---------------------------------- */
		/* -----RECUPERATION DES IMAGES------ */
		/* ---------------------------------- */
		
		char* tuxs[15];
		
		tuxs[0] = "img/1.jpg";
		tuxs[1] = "img/2.jpg";
		tuxs[2] = "img/3.jpg";
		tuxs[3] = "img/4.jpg";
		tuxs[4] = "img/5.jpg";
		tuxs[5] = "img/6.jpg";
		tuxs[6] = "img/7.jpg";
		tuxs[7] = "img/8.jpg";
		tuxs[8] = "img/9.jpg";
		tuxs[9] = "img/10.jpg";
		tuxs[10] = "img/11.jpg";
		tuxs[11] = "img/12.jpg";
		tuxs[12] = "img/13.jpg";
		tuxs[13] = "img/14.jpg";
		tuxs[14] = "img/15.jpg";
		
		char* states[6];
		
		states[0] = "img/stateHOME.JPG";
		states[1] = "img/stateTIPS1.JPG";
		states[2] = "img/stateTIPS2.JPG";
		states[3] = "img/stateTIPS3.JPG";
		states[4] = "img/stateWIN.JPG";
		states[5] = "img/stateLOOSE.JPG";
		
		char* buttons[2];
		
		buttons[0] = "img/start.JPG";
		buttons[1] = "img/launch.JPG";
		
		char* numbers[10];
		
		numbers[0] = "img/chiffres/0.JPG";
		numbers[1] = "img/chiffres/1.JPG";
		numbers[2] = "img/chiffres/2.JPG";
		numbers[3] = "img/chiffres/3.JPG";
		numbers[4] = "img/chiffres/4.JPG";
		numbers[5] = "img/chiffres/5.JPG";
		numbers[6] = "img/chiffres/6.JPG";
		numbers[7] = "img/chiffres/7.JPG";
		numbers[8] = "img/chiffres/8.JPG";
		numbers[9] = "img/chiffres/9.JPG";

		/* Chargement des images */
		
		SDL_Surface* p_tuxs[15];
		SDL_Surface* p_states[6];
		SDL_Surface* p_buttons[2];
		SDL_Surface* p_numbers[10];
		
		for (i=0; i<15; i++){
			p_tuxs[i] = IMG_Load(tuxs[i]); 
			if(p_tuxs[i] == NULL){ 	// Verification du chargement
		 	fprintf(stderr, "erreur de chargement de l'image %d\n", i);	
		 	}
		}
		
		for (i=0; i<6; i++){
			p_states[i] = IMG_Load(states[i]); 
			if(p_states[i] == NULL){ 	// Verification du chargement
				fprintf(stderr, "erreur de chargement de l'image states %d\n", i);	
		 	}
		}
		
		for (i=0; i<2; i++){
			p_buttons[i] = IMG_Load(buttons[i]); 
			if(p_buttons[i] == NULL){ 	// Verification du chargement
				fprintf(stderr, "erreur de chargement de l'image buttons %d\n", i);	
		 	}
		}
		
		for (i=0; i<10; i++){
			p_numbers[i] = IMG_Load(numbers[i]); 
			if(p_numbers[i] == NULL){ 	// Verification du chargement
				fprintf(stderr, "erreur de chargement de l'image numbers %d\n", i);	
		 	}
		}
		 
		GLuint t_tuxs[15];
		glGenTextures(15, t_tuxs); // Génération du nombre de textures voulues
		
		GLuint t_states[6];
		glGenTextures(6, t_states); // Génération du nombre de textures voulues
		
		GLuint t_buttons[2];
		glGenTextures(2, t_buttons); // Génération du nombre de textures voulues
		
		GLuint t_numbers[10];
		glGenTextures(10, t_numbers); // Génération du nombre de textures voulues
		
		/* Parametrage de toutes les images */
		GLenum format;
		
		for (i=0; i<15; i++){ // pour chaque texture, on importe l'image dans openGL
			glBindTexture(GL_TEXTURE_2D, t_tuxs[i]); // Attache d'une texture à un quai
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Application du filtre de redimension
			format = getFormat(p_tuxs[i]); // Récupération du format de l'image (fonction locale)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_tuxs[i]->w, p_tuxs[i]->h, 0, format, GL_UNSIGNED_BYTE, p_tuxs[i]->pixels); // Envoi de l'image à OpenGL (quai en question)
			glBindTexture(GL_TEXTURE_2D, 0); // Libérer le quai
			
			SDL_FreeSurface(p_tuxs[i]);
		}
		
		for (i=0; i<6; i++){ // pour chaque texture, on importe l'image dans openGL
			glBindTexture(GL_TEXTURE_2D, t_states[i]); // Attache d'une texture à un quai
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Application du filtre de redimension
			format = getFormat(p_states[i]); // Récupération du format de l'image (fonction locale)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_states[i]->w, p_states[i]->h, 0, format, GL_UNSIGNED_BYTE, p_states[i]->pixels); // Envoi de l'image à OpenGL (quai en question)
			glBindTexture(GL_TEXTURE_2D, 0); // Libérer le quai
			
			SDL_FreeSurface(p_states[i]);
		}
		
		for (i=0; i<2; i++){ // pour chaque texture, on importe l'image dans openGL
			glBindTexture(GL_TEXTURE_2D, t_buttons[i]); // Attache d'une texture à un quai
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Application du filtre de redimension
			format = getFormat(p_buttons[i]); // Récupération du format de l'image (fonction locale)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_buttons[i]->w, p_buttons[i]->h, 0, format, GL_UNSIGNED_BYTE, p_buttons[i]->pixels); // Envoi de l'image à OpenGL (quai en question)
			glBindTexture(GL_TEXTURE_2D, 0); // Libérer le quai
			
			SDL_FreeSurface(p_buttons[i]);
		}
		
		for (i=0; i<10; i++){ // pour chaque texture, on importe l'image dans openGL
			glBindTexture(GL_TEXTURE_2D, t_numbers[i]); // Attache d'une texture à un quai
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Application du filtre de redimension
			format = getFormat(p_numbers[i]); // Récupération du format de l'image (fonction locale)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_numbers[i]->w, p_numbers[i]->h, 0, format, GL_UNSIGNED_BYTE, p_numbers[i]->pixels); // Envoi de l'image à OpenGL (quai en question)
			glBindTexture(GL_TEXTURE_2D, 0); // Libérer le quai
			
			SDL_FreeSurface(p_numbers[i]);
		}
		
		/* ---------------------------------- */

		/* Generation aléatoire de nombres */
		srand(time(NULL));
		
		int rt[3]; // rt pour RandomTextures
		rt[0] = rand_a_b(0,15);
		rt[1] = rand_a_b(0,15);
		rt[2] = rand_a_b(0,15);	
		
		while (isEqual(rt[0],rt[1],rt[2])){
			rt[0] = rand_a_b(0,15);
			rt[1] = rand_a_b(0,15);
			rt[2] = rand_a_b(0,15);
		}
		
		/* Variables de jeu */
		int model = rand_a_b(0,15);	
		int score = 0;
		int score_dizaines;
		int score_unites;
		int speed = 61;
		int nb_clic = 0;
		int vies = 4;
		int sec_restantes = 60;
		int s_r_unites;
		int s_r_dizaines;
		
	  /* Boucle d'affichage */
	  int loop = 1;
	  int cpt=0;
	  int cpt2=0;
	  while(loop) {
		    /* Récupération du temps au début de la boucle */
		    Uint32 startTime = SDL_GetTicks();
		    
		    /* A décommenter si double buffering */
		    glClear(GL_COLOR_BUFFER_BIT);
		   
		    /* Placer ici le code de dessin */
		    if (stateGAME){
				sec_restantes = sec_restantes - cpt2/60;
		    }
		    
		    /* Conditions d'arrêt du jeu */
		    if (sec_restantes == 0){
				stateGAME = 0;
				stateLOOSE = 1;
			}
			
			if (vies == 0){
				stateGAME =0;
				stateLOOSE = 1;
			}
			
			if (score == 15){
				stateGAME = 0;
				stateWIN = 1;
			}
			
		    
		    /* ETATS */
		    if (stateHOME){
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, t_states[0]);
					glBegin(GL_QUADS);
							glTexCoord2f(0,0);
							glVertex2f(-8,6);
							glTexCoord2f(1,0);
							glVertex2f(8,6);
							glTexCoord2f(1,1);
							glVertex2f(8,-6);
							glTexCoord2f(0,1);
							glVertex2f(-8,-6);
					glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				glBindTexture(GL_TEXTURE_2D, t_buttons[1]);
					glBegin(GL_QUADS);
							glTexCoord2f(0,0);
							glVertex2f(-1.12,-4.78);
							glTexCoord2f(1,0);
							glVertex2f(1.12,-4.78);
							glTexCoord2f(1,1);
							glVertex2f(1.12,-5.22);
							glTexCoord2f(0,1);
							glVertex2f(-1.12,-5.22);
					glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				glDisable(GL_TEXTURE_2D);
			}
			
			if (stateTIPS1){
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, t_states[1]);
					glBegin(GL_QUADS);
							glTexCoord2f(0,0);
							glVertex2f(-8,6);
							glTexCoord2f(1,0);
							glVertex2f(8,6);
							glTexCoord2f(1,1);
							glVertex2f(8,-6);
							glTexCoord2f(0,1);
							glVertex2f(-8,-6);
					glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				glDisable(GL_TEXTURE_2D);
			}
			
			if (stateTIPS2){
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, t_states[2]);
					glBegin(GL_QUADS);
							glTexCoord2f(0,0);
							glVertex2f(-8,6);
							glTexCoord2f(1,0);
							glVertex2f(8,6);
							glTexCoord2f(1,1);
							glVertex2f(8,-6);
							glTexCoord2f(0,1);
							glVertex2f(-8,-6);
					glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				glDisable(GL_TEXTURE_2D);
			}
			
			if (stateTIPS3){
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, t_states[3]);
					glBegin(GL_QUADS);
							glTexCoord2f(0,0);
							glVertex2f(-8,6);
							glTexCoord2f(1,0);
							glVertex2f(8,6);
							glTexCoord2f(1,1);
							glVertex2f(8,-6);
							glTexCoord2f(0,1);
							glVertex2f(-8,-6);
					glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				glBindTexture(GL_TEXTURE_2D, t_buttons[0]);
					glBegin(GL_QUADS);
							glTexCoord2f(0,0);
							glVertex2f(-2.03,-2.5);
							glTexCoord2f(1,0);
							glVertex2f(2.03,-2.5);
							glTexCoord2f(1,1);
							glVertex2f(2.03,-4.9);
							glTexCoord2f(0,1);
							glVertex2f(-2.03,-4.9);
					glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				glDisable(GL_TEXTURE_2D);
			}
			
			if (stateGAME){
				
				if (cpt2 == 60){
					cpt2 = 0;
				}
				
				 if (cpt == speed){
					cpt = 0;
				}
				if (nb_clic == 1){
					cpt = 0;
					nb_clic = 0;
				}
				if (cpt == 0){
					rt[0] = rand_a_b(0,15);
					rt[1] = rand_a_b(0,15);
					rt[2] = rand_a_b(0,15);
				
					while (isEqual(rt[0],rt[1],rt[2])){
					rt[0] = rand_a_b(0,15);
					rt[1] = rand_a_b(0,15);
					rt[2] = rand_a_b(0,15);
					}
				}
				
				/* Fond */
				glBegin(GL_QUADS);
						glColor3ub(242,242,242);
						glVertex2f(-8, 6);
						glVertex2f(8, 6);
						glVertex2f(8,-6);
						glVertex2f(-8,-6);
				glEnd();
				
				glEnable(GL_TEXTURE_2D);
				
				/* TUX Modele */
				glBindTexture(GL_TEXTURE_2D, t_tuxs[model]);
				glBegin(GL_QUADS);
						glTexCoord2f(0,0);
						glVertex2f(-2,5.5);
						glTexCoord2f(1,0);
						glVertex2f(2,5.5);
						glTexCoord2f(1,1);
						glVertex2f(2,1.5);
						glTexCoord2f(0,1);
						glVertex2f(-2,1.5);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				/* TUX Gauche */
				glBindTexture(GL_TEXTURE_2D, t_tuxs[rt[0]]);
				glBegin(GL_QUADS);
						glTexCoord2f(0,0);
						glVertex2f(-7,1);
						glTexCoord2f(1,0);
						glVertex2f(-3,1);
						glTexCoord2f(1,1);
						glVertex2f(-3,-3);
						glTexCoord2f(0,1);
						glVertex2f(-7,-3);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				/* TUX Milieu */
				glBindTexture(GL_TEXTURE_2D, t_tuxs[rt[1]]);
				glBegin(GL_QUADS);
						glTexCoord2f(0,0);
						glVertex2f(-2,1);
						glTexCoord2f(1,0);
						glVertex2f(2,1);
						glTexCoord2f(1,1);
						glVertex2f(2,-3);
						glTexCoord2f(0,1);
						glVertex2f(-2,-3);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				/* TUX Droit */
				glBindTexture(GL_TEXTURE_2D, t_tuxs[rt[2]]);
				glBegin(GL_QUADS);
						glTexCoord2f(0,0);
						glVertex2f(3,1);
						glTexCoord2f(1,0);
						glVertex2f(7,1);
						glTexCoord2f(1,1);
						glVertex2f(7,-3);
						glTexCoord2f(0,1);
						glVertex2f(3,-3);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				glDisable(GL_TEXTURE_2D);

				/* Separateur noir */
				glBegin(GL_LINES);
						glColor3ub(0,0,0);
						glVertex2f(-7, -4);
						glVertex2f(7, -4);
						glColor3ub(242,242,242);
				glEnd();
				
				/* Vies */
				
				glEnable(GL_TEXTURE_2D);
				
				for(i=0; i<vies-1; i++){
					glBindTexture(GL_TEXTURE_2D, t_tuxs[model]);
					glBegin(GL_QUADS);
						glTexCoord2f(0,0);
						glVertex2f(0 + 2.5*i,-4.25);
						glTexCoord2f(1,0);
						glVertex2f(1.5 + 2.5*i,-4.25);
						glTexCoord2f(1,1);
						glVertex2f(1.5 + 2.5*i,-5.75);
						glTexCoord2f(0,1);
						glVertex2f(0 + 2.5*i,-5.75);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, 0);
				}
				
				glDisable(GL_TEXTURE_2D);				
				
				/* Score */
				
				score_dizaines = score/10;
				score_unites = score - score_dizaines*10;
				
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, t_numbers[score_unites]);
					glBegin(GL_QUADS);
						glTexCoord2f(0,0);
						glVertex2f(-3.18,-4.25);
						glTexCoord2f(1,0);
						glVertex2f(-2,-4.25);
						glTexCoord2f(1,1);
						glVertex2f(-2,-5.75);
						glTexCoord2f(0,1);
						glVertex2f(-3.18,-5.75);
					glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				glBindTexture(GL_TEXTURE_2D, t_numbers[score_dizaines]);
					glBegin(GL_QUADS);
						glTexCoord2f(0,0);
						glVertex2f(-4.36,-4.25);
						glTexCoord2f(1,0);
						glVertex2f(-3.18,-4.25);
						glTexCoord2f(1,1);
						glVertex2f(-3.18,-5.75);
						glTexCoord2f(0,1);
						glVertex2f(-4.36,-5.75);
					glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				glDisable(GL_TEXTURE_2D);
				
				/* Temps restant */
				
				s_r_dizaines = sec_restantes/10;
				s_r_unites = sec_restantes - s_r_dizaines*10;
				
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, t_numbers[s_r_unites]);
					glBegin(GL_QUADS);
						glTexCoord2f(0,0);
						glVertex2f(-5.68,-4.25);
						glTexCoord2f(1,0);
						glVertex2f(-4.5,-4.25);
						glTexCoord2f(1,1);
						glVertex2f(-4.5,-5.75);
						glTexCoord2f(0,1);
						glVertex2f(-5.68,-5.75);
					glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				glBindTexture(GL_TEXTURE_2D, t_numbers[s_r_dizaines]);
					glBegin(GL_QUADS);
						glTexCoord2f(0,0);
						glVertex2f(-6.86,-4.25);
						glTexCoord2f(1,0);
						glVertex2f(-5.68,-4.25);
						glTexCoord2f(1,1);
						glVertex2f(-5.68,-5.75);
						glTexCoord2f(0,1);
						glVertex2f(-6.86,-5.75);
					glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				glDisable(GL_TEXTURE_2D);
				
				/* separator */
				
				glBegin(GL_LINES);
					glColor3ub(0,0,0);
					glVertex2f(-4.43, -4.25);
					glVertex2f(-4.43, -5.75);
					glColor3ub(255,255,255);
				glEnd();

				cpt++;
				cpt2++;
			}
		    
		    if (stateWIN){
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, t_states[4]);
					glBegin(GL_QUADS);
							glTexCoord2f(0,0);
							glVertex2f(-8,6);
							glTexCoord2f(1,0);
							glVertex2f(8,6);
							glTexCoord2f(1,1);
							glVertex2f(8,-6);
							glTexCoord2f(0,1);
							glVertex2f(-8,-6);
					glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				/* TEMPS */
				
				s_r_dizaines = sec_restantes/10;
				s_r_unites = sec_restantes - s_r_dizaines*10;
				
				glBindTexture(GL_TEXTURE_2D, t_numbers[s_r_dizaines]);
					glPushMatrix();
						glTranslatef(3.5, -1.4, 0);
						glBegin(GL_QUADS);
							glTexCoord2f(0,0);
							glVertex2f(0,1.5);
							glTexCoord2f(1,0);
							glVertex2f(1.18,1.5);
							glTexCoord2f(1,1);
							glVertex2f(1.18,0);
							glTexCoord2f(0,1);
							glVertex2f(0,0);
						glEnd();
					glPopMatrix();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				glBindTexture(GL_TEXTURE_2D, t_numbers[s_r_unites]);
					glPushMatrix();
						glTranslatef(4.5, -1.4, 0);
						glBegin(GL_QUADS);
							glTexCoord2f(0,0);
							glVertex2f(0,1.5);
							glTexCoord2f(1,0);
							glVertex2f(1.18,1.5);
							glTexCoord2f(1,1);
							glVertex2f(1.18,0);
							glTexCoord2f(0,1);
							glVertex2f(0,0);
						glEnd();
					glPopMatrix();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				glDisable(GL_TEXTURE_2D);
			}
			
			if (stateLOOSE){
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, t_states[5]);
					glBegin(GL_QUADS);
							glTexCoord2f(0,0);
							glVertex2f(-8,6);
							glTexCoord2f(1,0);
							glVertex2f(8,6);
							glTexCoord2f(1,1);
							glVertex2f(8,-6);
							glTexCoord2f(0,1);
							glVertex2f(-8,-6);
					glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				/* SCORE */
				
				score_dizaines = score/10;
				score_unites = score - score_dizaines*10;
				
				glBindTexture(GL_TEXTURE_2D, t_numbers[score_dizaines]);
					glPushMatrix();
						glTranslatef(3.5, -1.4, 0);
						glBegin(GL_QUADS);
							glTexCoord2f(0,0);
							glVertex2f(0,1.5);
							glTexCoord2f(1,0);
							glVertex2f(1.18,1.5);
							glTexCoord2f(1,1);
							glVertex2f(1.18,0);
							glTexCoord2f(0,1);
							glVertex2f(0,0);
						glEnd();
					glPopMatrix();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				glBindTexture(GL_TEXTURE_2D, t_numbers[score_unites]);
					glPushMatrix();
						glTranslatef(4.5, -1.4, 0);
						glBegin(GL_QUADS);
							glTexCoord2f(0,0);
							glVertex2f(0,1.5);
							glTexCoord2f(1,0);
							glVertex2f(1.18,1.5);
							glTexCoord2f(1,1);
							glVertex2f(1.18,0);
							glTexCoord2f(0,1);
							glVertex2f(0,0);
						glEnd();
					glPopMatrix();
				glBindTexture(GL_TEXTURE_2D, 0);
				
				glDisable(GL_TEXTURE_2D);
			}

		   

		    /* Echange du front et du back buffer : mise à jour de la fenêtre */
		    SDL_GL_SwapBuffers();
		   
		   
		   
		    /* Boucle traitant les evenements */
		    SDL_Event e;
		    while(SDL_PollEvent(&e)) {
		    
			      /* L'utilisateur ferme la fenêtre : */
			      if(e.type == SDL_QUIT) {
					loop = 0;
					break;
			      }
			      
			      /* EVENEMENTS : */
			      switch(e.type) {
					  
					/* Clic souris*/
					case SDL_MOUSEBUTTONDOWN:
						if (e.button.button == SDL_BUTTON_LEFT){
							switchState(&stateHOME, &stateTIPS1, &stateTIPS2, &stateTIPS3, &stateGAME);
										
							if(stateGAME){
								if (nb_clic == 0){					  	
									xpos = -1+2*e.button.x/(float)WINDOW_WIDTH;
									ypos = -(-1+2*e.button.y/(float)WINDOW_HEIGHT);
									xpos = xpos*9.5;
									if (ypos>0){
										ypos = ypos*10;
									}else{
										ypos = ypos*5;
									}	
									
									int test = t_tuxs[rt[getClikedCarre(xpos, ypos)]];
									
									if (getClikedCarre(xpos, ypos) == -1){
										printf("clic en dehors des images\n");
									}else{
										nb_clic = 1;
										if (test == model+1){
										score = score +1;
										speed = speed -4;
										if (speed < 10){
											speed = 10;
										}
										}else{
											vies -= 1;
											speed = speed + 2;
											if (speed > 60){
												speed = 60;
											}
										}
									}
								}	
							}
						}
						break;  

			      } /* FIN SWITCH EVENT*/
		    } /* FIN BOUCLE */

		    /* Calcul du temps écoulé */
		    Uint32 elapsedTime = SDL_GetTicks() - startTime;
		    /* Si trop peu de temps s'est écoulé, on met en pause le programme */
		    if(elapsedTime < FRAMERATE_MILLISECONDS) {
		      SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
		    }
	  } /* FIN BOUCLE INFINIE*/
		
	  glDeleteTextures(15, t_tuxs);
	  glDeleteTextures(6, t_states);
	  glDeleteTextures(2, t_buttons);
	  glDeleteTextures(10, t_numbers);
	  
	  /* Liberation des ressources associées à la SDL */ 
	  SDL_Quit();
	  
	  return EXIT_SUCCESS;
	}








