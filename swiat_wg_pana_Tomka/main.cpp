#include "world.h"
#include <time.h>

int main() {
	srand(time(NULL));
	int width, height;
	scanf_s("%i %i", &width, &height);
	//system("cls");
	World earth(width, height);
	//earth.drawOrganisms();
	earth.playRound();
	
	return 0;
}
