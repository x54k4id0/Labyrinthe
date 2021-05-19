#include <SDL.h>
#include "Pile.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//gcc main\main.c -o bin\Labyrinthe.exe -I include -L lib -lmingw32 -lSDL2main -lSDL2
//gcc -o bin/Labyrinthe main/main.c `sdl2-config --cflags --libs`


//---------------------------------------Initaliation des couleurs-------------------------------------//


int murColor[] = {150,0,0};
int labColor[] = {100,100,100};
int curseurColor[] = {150,0,0};
int sortieColor[] = {0,121,101};
int entreeColor[] = {162,10,10};


//-----------------------------------------------------------------------------------------------------//

int mod(int a, int b){														// Retourne le modulo de a et b

    int r = a % b;
    return r < 0 ? r + b : r;
}

#define taille  50															// la taille des cases c'est a dire la largeur du passage
#define debut  0															// La case de debut.



#define colonnes  10														// Le nombre des colonnes c'est a dire la longueur de le labyrinthe. (Il fault qui soit 10 tjrs car le menu n'est pas dynamique)
#define lignes  10															// Le nombre des lignes c'est a dire la largeur de le labyrinthe. (Il fault qui soit 10 tjrs car le menu n'est pas dynamique)

#define width lignes*taille + 1												// la largeur de la fenetre SDL
#define height colonnes*taille + 1											// la longueur de la fenetre SDL



int tempsDattent = 50;														//Temps de raffraichisement de l'ecran 

int i,j,k=0;

int indice(int i, int j){										// Calcule le numero de la case <=> transmettre une matrice (n,p) vers un tableau de n*p elements.
	if (i<0 || j<0 || i>lignes-1 || j>colonnes-1) return -1;	// Si les coordonnees d'une case ne sont pas dans la matrice on retourne -1 , c'est a dire que cette case n'est pas dans le labyrinthe.
	return i+j*colonnes;										// Si les indices sont valides on retourne le numero de la case dans le nouvel tableau.
}


Cellule *labyrinthe[lignes*colonnes];							// Creer un tableau de (lignes * colonnes) cellules

Bool enCours = true;                                          // Condition d'execution du jeu



void CreerTableauLabyrinthe(){
	for (j=0,k=0; j< colonnes; j++){					// On veut creer lignes*colonnes cellules
		for (i=0; i< lignes; i++,k++){					
				labyrinthe[k] = creerCellule(i,j);		// Creer la K-ieme cellule.
		}
	}
}


void AfficherBackground(SDL_Window *window,SDL_Renderer *renderer,int i){												// L'affichage des frames
			char *src[]={"Resources/Entre1et2.bmp","Resources/Menu1.bmp","Resources/Menu2.bmp","Resources/FIN.bmp"};	// Les menus sont stockees dans le repertoire Resources , et leurs destinations dans un tableau src
			SDL_Rect position;

			position.x = 0;
			position.y = 0;
			position.h = height;
			position.w = width;



			SDL_Surface *Background = SDL_LoadBMP(src[i]);																// Charger le Background
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,Background);									// Creer une texture du Background

			SDL_FreeSurface(Background);																				// Liberer la surface Background 
			SDL_QueryTexture(texture,NULL,NULL,&position.w,&position.h);
			SDL_RenderCopy(renderer,texture,NULL,&position);
			SDL_RenderPresent(renderer);																				// Afficher
		}



int choix=-1;
int positionCurseur=0;
int frame=1;

void TestEvent(SDL_Window* window,SDL_Renderer *renderer){							// La fonciton qui va gerer les evennements au cours du Jeu
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
	    switch(event.type) {
	    	case SDL_QUIT : {														// Si on a appuye sur Fermer la fenetre , le programme doit detruire le rendu puis la fenetre pour quitter
		    	tempsDattent = 0;
		      	enCours = false;
	    	  	SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				SDL_Quit();

		       	break;
		    }
		    case SDL_KEYDOWN : {													// Si on ait dans le frmae 3 et on appuye sur Fermer la fenetre , le programme doit detruire le rendu puis la fenetre pour quitter
		    	if(frame==3){
		    		tempsDattent = 0;
			      	enCours = false;
		    	  	SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);
					SDL_Quit();

		    	}
		    	break;
		    }
		}
	}
}

void TestEventMenu(SDL_Window* window,SDL_Renderer *renderer){							// La fonctton qui va gerer les evennements au cours de l'affichage du menu du jeu
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
	    switch(event.type) {
	    	case SDL_QUIT : {														// Si on a appuye sur Fermer la fenetre , le programme doit detruire le rendu puis la fenetre pour quitter
		    	tempsDattent = 0;
		      	enCours = false;
	    	  	SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				SDL_Quit();

		       	break;
		      	}
		   	case SDL_KEYDOWN : { 
          		switch(event.key.keysym.sym) { 
               		case SDLK_UP: 													// Si on a appuye sur UP , la position du curseur bascule vers le haut (on decremente positionCursuer)
               			AfficherBackground(window,renderer,frame);
               			positionCurseur = mod((positionCurseur-1),2);
               			break;	
               		case SDLK_DOWN:													// Si on a appuye sur DOWN , la position du curseur bascule vers le bas (on incremente positionCursuer) 
               			AfficherBackground(window,renderer,frame);
               			positionCurseur = mod((positionCurseur+1),2);
               			break;
               		case SDLK_RETURN:												// Si on appuye sur Entrer
               			choix=positionCurseur;
               			switch(positionCurseur){									// On test positionCurseur
               				case 0:													// Si le curseur et dans la position 1 (positionCurseur = 0), on bascule vers le Frame 2
               					SDL_Delay(tempsDattent/3);
               					frame=2;
               					AfficherBackground(window,renderer,0);
               					SDL_Delay(tempsDattent/3);
               					AfficherBackground(window,renderer,frame);
               					break;
               				case 1:													// Si le curseur et dans la position 2 (positionCurseur = 1),  le programme doit detruire le rendu puis la fenetre pour quitter
               					tempsDattent = 0;
						      	enCours = false;
					    	  	SDL_DestroyRenderer(renderer);
								SDL_DestroyWindow(window);
								SDL_Quit();

						       	break;
               			}
               		break;
          		} 
	 		}
	    }
	}
}


int *SelectEntreeEtSorties(){        																	 // choisir l'entree et les sorties;
	int * tab=malloc(sizeof(int));
	int r = rand() % 4;
	switch(r){
		case 0:
			{
				tab[0] = rand() % colonnes;															// Case dans la ligne haute
				break;
			}
		case 1:
			{
				tab[0] = lignes * (rand() % colonnes) + colonnes-1;									// Case dans la colonne droite
				break;
			}
		case 2:
			{
				tab[0]=(lignes-1)*colonnes + rand() % colonnes;										// Case dans la ligne basse
				break;
			}
		case 3:
			{
				tab[0]=lignes * (rand() % colonnes);												// Case dans la colonne gauche
				break;
			}
	}
	tab[1]=r;																						// On va profiter de se nombre pour connaitre qu'elle mur on va effacer pour etre entree ou l'une des sorties
	return tab;

}



	void CurseurMenu(SDL_Window *window, SDL_Renderer *renderer){ 									// Afficher le curseur de Menu qui est un Rectangle blanc Dynamique

		int tab[]={146,274};																		// Tableau des positions du curseur
		SDL_Rect Cursuer;
		Cursuer.x = 100;
		Cursuer.y = tab[positionCurseur];															// l'ordonne du cursuer peut etre tab[0] ou tab[1] car on a deux boutons dans le menu , donc positionCurseur est modulo 2 
		Cursuer.h = 50;
		Cursuer.w = 300;


		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);							// Couleur blanche

		SDL_RenderDrawRect(renderer,&Cursuer);
		

		SDL_RenderPresent(renderer);
	}



void Jeu(SDL_Window* window,SDL_Renderer *renderer){

	// Affichage du menu
		AfficherBackground(window,renderer,frame);
		while (frame==1 && choix!=0) {CurseurMenu(window,renderer);TestEventMenu(window,renderer);TestEvent(window,renderer);if(!enCours){tempsDattent=0;break;}} // Attendre de prendre une decision
		choix=-1;


		// Remplir la fenetre par la couleur noire

		SDL_Rect Case;																	
		Case.x = 0;
		Case.y = 0;
		Case.h = height;
		Case.w = width;


		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);							

		SDL_RenderFillRect(renderer,&Case);														



		SDL_RenderPresent(renderer);




	/* 	Pour faciliter la tache du changement de la couleur du rendu
		on defini la fonction void ChangerCouleur(int couleur[3])
		cette derniere a pour parametre un tableau de 3 entiers R,G,B
		chaque entier peut avoir une valeur entre 0 et 255
		ex : blanc : en RGB est {255,255,255}
	*/	

	void ChangerCouleur(int couleur[3]){
		SDL_SetRenderDrawColor(renderer, couleur[0], couleur[1], couleur[2], SDL_ALPHA_OPAQUE);		//	Pour changer la couleur on utilise la fonction int SDL_SetRenderDrawColor(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
																									//	qui va prendre un rendu et une couleur en trois entiers r, g et b et la valeur alpha a dans ce projet on va utiliser comme valeur alpha SDL_ALPHA_OPAQUE;
	}

	//	Afin de faciliter le remplissage des cellules on defini la fonction void RemplirCellule(int x, int y, int couleur[3])
	//	qui va prendre les coordonnes cartesiennes (x,y) de la cellule a remplir et la couleur de cette derniere

	void RemplirCellule(int x, int y, int couleur[3]){
		ChangerCouleur(couleur);							// 	On change la couleur du randu par la couleur choisi;
		SDL_Rect Case;										// 	Chaque cellule va etre representer par un rectangle de coordonnes x et y et de dimensions w et h;
		Case.x = x * taille +1;								// 	Pour eviter la coloration des murs de chaque cellule on laisse un pixel pour les murs
		Case.y = y * taille +1;								// 	donc la postion de chaque cellule doit etre ses indices i,j dans la matrice labyrinthe multipliers par sa taille et on ajout un pixel pour eviter la coloration des murs haute et gauche;
		Case.w = taille-1;									//	puisque on a ajoute un pixel a la postion de chaque cellule on doit le soustraire de ses dimensions pour eviter la coloration des murs basse et droite;
		Case.h = taille-1;									//	donc le width va etre la taille mois 1 , idem pour le height

		SDL_RenderFillRect(renderer, &Case);				//	Pour dessiner la cellule on utilise la fonction de SDL , 
															//	int SDL_RenderFillRect(SDL_Renderer* rendu, const SDL_Rect* rectangle)
															//	qui a pour arguments , le rendu , et le rectangle
	}



//-----------------------------------------------------------------------------------------------------------//


		int *sortie1 = malloc(sizeof(int)) ;
		int *sortie2 = malloc(sizeof(int)) ;
		int *entree  = malloc(sizeof(int)) ;

		sortie1 = SelectEntreeEtSorties();								// Choix de la Premiere Sortie du Jeu (On prend un element du tableau 'labyrinthe' aleatoirement).
		sortie2 = SelectEntreeEtSorties();								// Choix de la Deuxieme Sortie du Jeu  (On prend un element du tableau 'labyrinthe' aleatoirement).
		entree = SelectEntreeEtSorties();

		
		Cellule * courant = labyrinthe[debut];							// Choix de la case de debut de la generation du labyrinthe.
		Cellule * Sortie1 = labyrinthe[sortie1[0]];								// Choix de la Premiere Sortie du Jeu (On prend un element du tableau 'labyrinthe' aleatoirement).
		Cellule * Sortie2 = labyrinthe[sortie2[0]];								// Choix de la Deuxieme Sortie du Jeu  (On prend un element du tableau 'labyrinthe' aleatoirement).
		Cellule * Entree = labyrinthe[entree[0]];								// Choix de l'Entree du Jeu (On prend un element du tableau 'labyrinthe' aleatoirement).



		void AfficherCelluleDeGrid(Cellule *cell){									// La fonction de tracage des cases (avant et apres d'etre visitees).
			int x = cell->x * taille;
			int y = cell->y * taille;



			ChangerCouleur(murColor);

			if (cell->Murs[0]) SDL_RenderDrawLine(renderer, x, y, x+taille, y); // top 								// Si le mur de dessus existe on la tracer 
			if (cell->Murs[1]) SDL_RenderDrawLine(renderer, x+taille, y, x+taille, y+taille); // right				// Si le mur droite existe on la tracer
			if (cell->Murs[2]) SDL_RenderDrawLine(renderer, x+taille, y+taille, x, y+taille); // bottom				// Si le mur de dessous existe on la tracer 
			if (cell->Murs[3]) SDL_RenderDrawLine(renderer, x, y+taille, x, y); // left								// Si le mur gauche existe on la tracer

			if (cell->Visited==true) {																				// Si la case est deja visite on doit la tracer
				SDL_Rect Case;																						// C-a-d on doit creer une rectangle est la remplir

				Case.x = x+1 ;
				Case.y = y+1 ;
				Case.w = taille-1 ;
				Case.h = taille-1 ;

				ChangerCouleur(labColor);																				// On choisit la couleur de la case

				SDL_RenderFillRect(renderer, &Case);																// Affichage de la case sur le rendu
			}								

		}





		void TracerGrid(){									// La fonction qui va tracer le grid c'est a dire la matrice avant de generer le labyrinthe.
			for (int n=0;n<lignes*colonnes;n++){
				TestEvent(window,renderer);
				if (!enCours){tempsDattent=0;break;}
				AfficherCelluleDeGrid(labyrinthe[n]);		// Afficher cellule Numero n dans labyrinthe
			}													
		}



		void deleteMurs(Cellule *a,Cellule *b){									// La fonction qui supprime les murs entre les voisins (entre la case courante et la case suivante).
				int dx = a->x - b->x;											// La difference entre les coordonnes indique le mur convenable a supprimer
				int dy = a->y - b->y;

				if(dx==1){														// Supprimer le mur gauche du courant et droit du suivant
					a->Murs[3] = false;
					b->Murs[1] = false;
				}
				else if(dx==-1){												// Supprimer le mur droit du courant et gauche du suivant
					a->Murs[1] = false;
					b->Murs[3] = false;
				}
				if(dy==1){														// Supprimer le mur bottom du courant et top du suivant
					a->Murs[0] = false;
					b->Murs[2] = false;
				}
				else if(dy==-1){												// Supprimer le mur top du courant et bottom du suivant
					a->Murs[2] = false;
					b->Murs[0] = false;
				}								
			}

		
		void SupprimerMurs(Cellule *cell , int couleur[3]){											// La fonction de tracage des murs supprimees de chaque case.
			int x = cell->x * taille;
			int y = cell->y * taille;



			ChangerCouleur(couleur);																				// On choisit la couleur , generalement on va profiter de cet option et choisir la couleur du Background pour supprimer l'apparance des murs supprimees

			if (!cell->Murs[0]) SDL_RenderDrawLine(renderer, x, y, x+taille, y); // top								// Si le mur de dessus n'existe pas on le retracer , apres sa suppression de l'algorithme de generation 
			if (!cell->Murs[1]) SDL_RenderDrawLine(renderer, x+taille, y, x+taille, y+taille); // right				// Si le mur droit n'existe pas on le retracer , apres sa suppression de l'algorithme de generation 
			if (!cell->Murs[2]) SDL_RenderDrawLine(renderer, x+taille, y+taille, x, y+taille); // bottom			// Si le mur de dessous n'existe pas on le retracer , apres sa suppression de l'algorithme de generation 
			if (!cell->Murs[3]) SDL_RenderDrawLine(renderer, x, y+taille, x, y); // left							// Si le mur gauche n'existe pas on le retracer , apres sa suppression de l'algorithme de generation 		
		}

		Cellule * ChoisirSuivantPourGeneration(Cellule *cell){								// La fonction de recherche des cases voisins qui sont pas encore visites de la case courante pour la generation de labyrinthe.
			int i = cell->x;
			int j = cell->y;
			Cellule *top;   													// Initialisation des voisins de la cellule courante
			Cellule *right; 
			Cellule *bottom;
			Cellule *left;
			if (indice(i,j-1)!=-1) {top     =    labyrinthe[indice(i,j-1)];} else {top     = NULL;}					// On creer le voisin de dessus s'il est valide
			if (indice(i+1,j)!=-1) {right   =    labyrinthe[indice(i+1,j)];} else {right   = NULL;}					// On creer le voisin de droite s'il est valide
			if (indice(i,j+1)!=-1) {bottom  =    labyrinthe[indice(i,j+1)];} else {bottom  = NULL;}					// On creer le voisin de dessous s'il est valide
			if (indice(i-1,j)!=-1) {left    =    labyrinthe[indice(i-1,j)];} else {left    = NULL;}					// On creer le voisin de gauche s'il est valide


			if (top && !top->Visited) {
				cell->Voisins[0]=top;																				// On affecte le voisin de dessus au tableau des voisins de la case courante s'il n'est pas visite
			}
			else cell->Voisins[0]=NULL;

			if (right && !right->Visited) {
				cell->Voisins[1]=right;																				// On affecte le voisin de droite au tableau des voisins de la case courante s'il n'est pas visite
			}
			else cell->Voisins[1]=NULL;

			if (bottom && !bottom->Visited) {
				cell->Voisins[2]=bottom;																			// On affecte le voisin de dessous au tableau des voisins de la case courante s'il n'est pas visite
			}
			else cell->Voisins[2]=NULL;

			if (left && !left->Visited) {
				cell->Voisins[3]=left;																				// On affecte le voisin de gauche au tableau des voisins de la case courante s'il n'est pas visite
			}
			else cell->Voisins[3]=NULL;

			
			
			int r = rand() % 4;																						// On choisit un voisin aleatoirement
			int eps=0;
			do{	
				r= rand() % 4;
				eps++;
			}while(!cell->Voisins[r] && eps<100);																	// Pour ameliorer l'efficaciter de l'algorithme on ajout une boucle pour eviter la repetition de prendre la meme valeur toujours est eviter la sortance par un voisin de valeur NULL toujours
			
	
			
			return cell->Voisins[r];																				// Retourner le voisin choisi (Peut etre Valide ou Non)
		
		}





		void Signaler(Cellule *Sortie){								// Signale les cases d'entree et de sorties

			if(!Sortie){														// Aucune sortie et specifiee

				RemplirCellule(Sortie1->x,Sortie1->y,sortieColor);				// Afficher les deux sorties et l'entree par ses couleurs
				RemplirCellule(Sortie2->x,Sortie2->y,sortieColor);
				RemplirCellule(Entree->x,Entree->y,entreeColor);

				SDL_RenderPresent(renderer);
				SDL_Delay(tempsDattent*2);										// Attendre qlq instants
				RemplirCellule(Sortie1->x,Sortie1->y,labColor);					// Afficher les sorties et l'entree par la couleur du labyrinthe (Supprimer)
				RemplirCellule(Sortie2->x,Sortie2->y,labColor);
				RemplirCellule(Entree->x,Entree->y,labColor);
				SDL_RenderPresent(renderer);
				SDL_Delay(tempsDattent*2);
			}

			else{																// Une sortie et specifiee

				RemplirCellule(Sortie->x,Sortie->y,sortieColor);				// Afficher la sortie et l'entree par ses couleurs
				RemplirCellule(Entree->x,Entree->y,entreeColor);

				SDL_RenderPresent(renderer);
				SDL_Delay(tempsDattent*2);										// Attendre qlq instants
				RemplirCellule(Sortie->x,Sortie->y,labColor);					// Afficher la sortie et l'entree par la couleur du labyrinthe (Supprimer)
				RemplirCellule(Entree->x,Entree->y,labColor);
				SDL_RenderPresent(renderer);
				SDL_Delay(tempsDattent*2);
			}
		}




		Cellule * ChoisirSuivantPourSolution(Cellule *cell){								// La fonction de recherche des cases voisins qui sont pas encore visites de la case courante mais ici pour trouver la solution.
			int i = cell->x;
			int j = cell->y;
			Cellule *top;   													// Initialisation des voisins de la cellule courante
			Cellule *right; 
			Cellule *bottom;
			Cellule *left;
			if (indice(i,j-1)!=-1) {top     =    labyrinthe[indice(i,j-1)];} else {top     = NULL;}					// On creer le voisin de dessus s'il est valide
			if (indice(i+1,j)!=-1) {right   =    labyrinthe[indice(i+1,j)];} else {right   = NULL;}					// On creer le voisin de droite s'il est valide
			if (indice(i,j+1)!=-1) {bottom  =    labyrinthe[indice(i,j+1)];} else {bottom  = NULL;}					// On creer le voisin de dessous s'il est valide
			if (indice(i-1,j)!=-1) {left    =    labyrinthe[indice(i-1,j)];} else {left    = NULL;}					// On creer le voisin de gauche s'il est valide


			if (top && !top->Visited && !top->Murs[2]) {
				cell->Voisins[0]=top;																				// On affecte le voisin de dessus au tableau des voisins de la case courante s'il n'est pas visite et pas de mur qui presente un obstacle ainsi pas de passage entre eux
			}
			else cell->Voisins[0]=NULL;

			if (right && !right->Visited && !right->Murs[3]) {
				cell->Voisins[1]=right;																				// On affecte le voisin de droite au tableau des voisins de la case courante s'il n'est pas visite et pas de mur qui presente un obstacle ainsi pas de passage entre eux
			}
			else cell->Voisins[1]=NULL;

			if (bottom && !bottom->Visited && !bottom->Murs[0]) {
				cell->Voisins[2]=bottom;																			// On affecte le voisin de dessous au tableau des voisins de la case courante s'il n'est pas visite et pas de mur qui presente un obstacle ainsi pas de passage entre eux
			}
			else cell->Voisins[2]=NULL;

			if (left && !left->Visited && !left->Murs[1]) {
				cell->Voisins[3]=left;																				// On affecte le voisin de gauche au tableau des voisins de la case courante s'il n'est pas visite et pas de mur qui presente un obstacle ainsi pas de passage entre eux
			}
			else cell->Voisins[3]=NULL;

			
			
			int r = rand() % 4;																						// On choisit un voisin aleatoirement
			int eps=0;
			do{	
				r= rand() % 4;
				eps++;
			}while(!cell->Voisins[r] && eps<100);																	// Pour ameliorer l'efficaciter de l'algorithme on ajout une boucle pour eviter la repetition de prendre la meme valeur toujours est eviter la sortance par un voisin de valeur NULL toujours
			
	
			
			return cell->Voisins[r];																				// Retourner le voisin choisi (Peut etre Valide ou Non)
		
		}

		Pile *maPile = creerPile();																					// Initialiser la pile qui va stocker les elements deja visitees (pour le DFS).
		maPile = push(maPile,courant);																				// On empile la case courante dans la pile.
		

		
		void Generer(Cellule * courant){																				// La fonction qui va Generer et Tracer le labyrinthe aleatoirement.

			courant->Visited = true;																					// Marker la cellule courante comme vistee
			
			Cellule *suivant = ChoisirSuivantPourGeneration(courant);													// Choisir une cellule suivant (un voisin aleatoirement)


			while (suivant && !suivant->Visited) {																		// Si le suivant est valid et n'est pas viste (la cellule courante a des voisins et ne sont pas encore vistee)

				TestEvent(window,renderer);																				// Tester si le programe est en cours ou non

				if (!enCours){tempsDattent=0;break;}

				deleteMurs(courant,suivant);																			// Effacer les murs entre la case courant et la suivante

				suivant->Visited=true;																					// Rendre la cellule suivante vistee
				

				maPile = push(maPile,suivant);																			// Ajouter le suivant a la pile


				RemplirCellule(courant->x,courant->y,curseurColor);															// Tracer la cellule courante 
				
				SDL_Delay(tempsDattent);																				// On attend quelques secondes et apres on presente le rendu
				SDL_RenderPresent(renderer);


				RemplirCellule(courant->x,courant->y,labColor);																// On supprime la cellule courante apres quelques secondes pour avoir une animation de mouvement
				SupprimerMurs(suivant , labColor);																				// On trace les nouveaux murs (apres avoir supprime les murs entre le courant et le suivant)

				SDL_Delay(tempsDattent);
				SDL_RenderPresent(renderer);

				Generer(suivant);																						// On repete la procedure
			}

			while(!(suivant && !suivant->Visited) && pileLen(maPile)>0){												// Si le suivant n'est pas valide (deja visite) et la pile n'est pas encore vide

				TestEvent(window,renderer);																				// On teste si le programme est en cours ou non

				if (!enCours){tempsDattent=0;break;}																					// Si non on arrete

				RemplirCellule(courant->x,courant->y,labColor);																// On supprime la case courante
				
				courant = topPile(maPile);																				// On depile un element de la pile , il devient le nouveau courant


				RemplirCellule(courant->x,courant->y,curseurColor);															// On Trace le nouveau courant

				SDL_Delay(tempsDattent);																				
				SDL_RenderPresent(renderer);
				
				maPile = pop(maPile);																				

				Generer(courant);																						// On repete une autre fois
			}

			ChangerCouleur(curseurColor);
			
			SDL_Delay(tempsDattent/50);
			return ;										

		}

		TracerGrid();																									// On trace le grid

		SDL_RenderPresent(renderer);
		SDL_Delay(tempsDattent*10);
		
		Generer(courant);																								// On genere le labyrinthe aleatoirement

		RemplirCellule(courant->x,courant->y,labColor);																		// dans la fin le curseur est a la postion "Courant" qui va rester colore par la couleur du curseur donc on doit l'effacer

		SDL_RenderPresent(renderer);																					// Et on affiche le rendu
		SDL_Delay(tempsDattent*40);


		//----------------------------------------------

		

		Pile * Sol = creerPile();																						//Pile de solution


		void TracerEntreeEtSorties(){																					// C'est la fonction qui va tracer l'entree et les sorties du labyrinthe pour commencer la resolution

			Sortie1->Murs[sortie1[1]]=false;																			// Supprimer les murs de l'entree , les sorties
			Sortie2->Murs[sortie2[1]]=false;
			Entree->Murs[entree[1]]=false;

			TracerGrid();																								// Tracer le nouveau GRID et supprimer les murs graphiquement

			SupprimerMurs(Sortie1,labColor);
			SupprimerMurs(Sortie2,labColor);
			SupprimerMurs(Entree,labColor);

		}
		



		




		int couleur[] = {120,0,0};
		void Solution(Cellule *courant , Cellule * sortie){									// La fonction qui va trouver la solution allant de la case d'entree jusqu'a la case de sortie

			TracerEntreeEtSorties();
																							// On fait la meme procedure que la generation , sauf qu'on va ajouter une condition pour sortir
			courant->Visited = true;														// cette condition affirme si la cellule courante est differente de la cellule de sortie desiree
																							// on continue , sinon on doit arreter , et puisque elle s'agit d'une fonction recursive elle va
			Cellule *suivant = ChoisirSuivantPourSolution(courant);							// continuer un peu de temps , et on change le temps d'attent a 0 , pour que le programme soit 
																							// comme il a arrete (d'un cote graphique)

			if (courant->x == sortie->x && courant->y == sortie->y) {for(i=0;i<10;i++){Signaler(sortie);}tempsDattent=0;couleur[0]=100;couleur[1]=100;couleur[2]=100;return;} // Si la sortie et trouver on la signale
			

			while (suivant && !suivant->Visited) {											// tant que suivant est valide

				TestEvent(window,renderer);													// tester si le programme et encours
				if (!enCours){tempsDattent=0;break;}
				RemplirCellule(courant->x,courant->y,labColor);								// Tracer courant par labColor (supprimer pour avoir une animation , suppression du courant puis tracage du suivant)

				RemplirCellule(suivant->x,suivant->y,couleur);								// Tracer suivant (Curseur)

				SDL_Delay(tempsDattent);
				SDL_RenderPresent(renderer);

				suivant->Visited=true;														// Marker suivant visited
				courant->Visited=true;														// Marker courant visited

				Sol = push(Sol,courant);													// Empile courant dans Sol

				Solution(suivant,sortie);													// refaire la procedure pour suivant
			}

			while(!(suivant && !suivant->Visited) && pileLen(Sol)>0 ){						// Tanque suivant n'est pas valide (Pas de voisin valide pour visiter) et la Pile Sol n'est pas encore vide


				TestEvent(window,renderer);													// Tester si le programme est en cours
				if (!enCours){tempsDattent=0;break;}

				RemplirCellule(courant->x,courant->y,labColor);								// Tracer courant par labColor (Supprimer pour revenir en arreiere)

				SDL_Delay(tempsDattent);													// Attendre qlq instants
				SDL_RenderPresent(renderer);

			
				courant = topPile(Sol);														// Prendre la dernier cellule visitee comme courant

				RemplirCellule(courant->x,courant->y,couleur);								// Tracer courant (Curseur)

				SDL_Delay(tempsDattent);
				SDL_RenderPresent(renderer);
				
				Sol = pop(Sol);																// Supprimer la derniere cellule dans la Pile

				Solution(courant,sortie);													// Refaire la meme procedure pour courant

			}

			SDL_Delay(tempsDattent/50);														// Si aucune condition est verifiee on attend un peut et on quitte la fonction 
			return;	

		}



		TracerEntreeEtSorties();							// On trace l'entree et les sorties

	
		SDL_RenderPresent(renderer);

		
	

		for (i=0;i<10;i++){									// On trace l'entree et les sorties
			Signaler(NULL);
		}

		SDL_Delay(tempsDattent*60);
		AfficherBackground(window,renderer,frame);			// Affichage du deuxieme Frame
		while (frame==2 && choix!=0) {CurseurMenu(window,renderer);TestEventMenu(window,renderer);TestEvent(window,renderer);if(!enCours){tempsDattent=0;break;}}
		choix=-1;
		frame=3;


		RemplirCellule(courant->x,courant->y,labColor);			// dans la fin de la generation une cellule est encore coloree par la couleur du cursuer , dans on la colore 
		

		for(int i=0;i<lignes*colonnes;i++){						// On reset le Labyrinthe apres avoir supprimer les murs des sorites et d'entree
			TestEvent(window,renderer);
			if (!enCours){tempsDattent=0;break;}
			labyrinthe[i]->Visited = false;
			SupprimerMurs(labyrinthe[i],labColor);
			TracerGrid();
		}



		for (i=0;i<10;i++){
			Signaler(Sortie1);									// On signale l'entree et la premiere sortie
		}


		courant = Entree;										// mise a jour du cellule courante

		Sol = push(Sol,courant);								// Ajouter courant a la pile Sol


		// Resolution (Entree vers 1ere sortie)
		Solution(courant,Sortie1);
		SDL_RenderPresent(renderer);
		SDL_Delay(tempsDattent*40);

		//Changer la sortie desiree;


		

		for(int i=0;i<lignes*colonnes;i++){						// On reset le Labyrinthe apres avoir supprimer les murs des sorites et d'entree
			TestEvent(window,renderer);
			if (!enCours){tempsDattent=0;break;}
			labyrinthe[i]->Visited = false;
			RemplirCellule(labyrinthe[i]->x,labyrinthe[i]->y,labColor);
		}


		for (i=0;i<10;i++){										// On signale l'entree et la premiere sortie
			Signaler(Sortie2);
		}

		couleur[0]=120;couleur[1]=0;couleur[2]=0;				//Couleur du curseur


		tempsDattent = 50;										// Reset du tempsDattent

		courant = Entree;										// mise a jour du cellule courante

		Sol = push(Sol,courant);								// Ajouter courant a la pile Sol

		
		// Resolution (Entree vers 1ere sortie)
		Solution(courant,Sortie2);
		SDL_RenderPresent(renderer);
		SDL_Delay(tempsDattent*40);


	}



int main(int argc, char **argv){

	srand ( time(0) );	//initialiser le seed de randomation des valeurs (pour avoir un fonctionnement plus efficace dans la selection des voisins d'une maniere aleatoire)

//------------------Initialisation de le labyrinthe (un tableau de lignes * colonnes)-----------------------//
		
	CreerTableauLabyrinthe();

//-----------------------------------------------------------------------------------------------------------//

//------------------------------------Initialiser les parametres de SDL--------------------------------------//

	SDL_Window *window = NULL;													// Initialiser la fenetre.
	SDL_Renderer *renderer = NULL;												// Initialiser le rendu.

	SDL_Init(SDL_INIT_VIDEO);													// Initialiser l'affichage.

	//Creation de la fenetre : fenetre de dimensions (width,height) et va etre centree ; il va porter le titre : "Labyrinthe"
	window = SDL_CreateWindow("Labyrinthe",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,0);
	
	// Creation du rendu de la fentere
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);



	

	Jeu(window,renderer);														// l'appel de la fonction initiale Jeu
	while(enCours) {AfficherBackground(window,renderer,3);TestEvent(window,renderer);}
	

	

	SDL_RenderPresent(renderer);
	SDL_Delay(tempsDattent*20);
	/*---------------------------------------------------------*/

	SDL_DestroyRenderer(renderer);												// Detruire le rendu
	SDL_DestroyWindow(window);													// Detruire la fenetre
	SDL_Quit();																	// Quitter le SDL

	return EXIT_SUCCESS;
}

// Abdessamad KERROU
