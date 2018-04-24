#include "world.h"
#include <time.h>

int main() {
	srand(time(NULL));
	int width, height;
	cout << "Write width and height of new world:\n";
	scanf_s("%i %i", &width, &height);
	system("cls");
	World earth(width, height);
	//earth.drawOrganisms();
	while(earth.playRound());
	
	return 0;
}
