#include <stdlib.h>
#include <stdio.h>

//Alloue un espace mémoire pour stocker le point et renvoie l'adresse de cet espace
	PointLink* AllocPointLink(float x, float y, int r, int v, int b){
		PointLink* new = (PointLink*)malloc(sizeof(PointLink));
		if (new==NULL){
			fprintf(stderr, "alloc : Erreur lors de l'allocation mémoire\n");
			return NULL;
		}
		new->x=x;
		new->y=y;
		new->r=r;
		new->v=v;
		new->b=b;
		new->next=NULL;	
		return new;
	}
