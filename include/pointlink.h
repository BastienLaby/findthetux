typedef struct point_link {
		float x, y; // Position 2D du point
		int r, v, b; // Couleur du point
		struct point_link *next; // Suivant
	} PointLink;

	typedef PointLink* PointList;  // le premier point de la liste

PointLink* AllocPointLink(float x, float y, int r, int v, int b);
