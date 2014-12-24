#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdlib.h>

#define SPRITE_HEIGHT 72
#define SPRITE_WIDTH 72
#define MAN_SPRITE_STAND_X 0
#define MAN_SPRITE_STAND_Y SPRITE_HEIGHT*6
#define MAN_SPRITE_MOVE_X SPRITE_WIDTH*2
#define MAN_SPRITE_MOVE_Y SPRITE_HEIGHT*6

/*build with gcc main.c -D_REENTRANT -I/usr/include/SDL2 -lSDL2 -lpthread*/

int globaltime = 0;

/** structure for man */
typedef struct {
	int x, y, spriteX, spriteY;
	float dy;
	short life;
	char *name;
	int alive;
	int currentSprite, walking, facingLeft, shooting, visible;
} Man;

typedef struct {
	int x, y;
} Star;

typedef struct {
	Man man; /* players */
	SDL_Texture *spritesTexture; /* stars */
	Star stars[100];
	SDL_Renderer *renderer;
} GameState;

/** loadGame
 * \param gameState containing game data during the game
 * \return void
 * */
void loadGame(GameState *gameState) {
	gameState->man.x = 320 - 40;
	gameState->man.y = 240 - 40;
	gameState->man.alive = 1;
	gameState->man.visible = 1;
	gameState->man.facingLeft = 0;
	gameState->man.spriteX = MAN_SPRITE_STAND_X;
	gameState->man.spriteY = MAN_SPRITE_STAND_Y;
	gameState->man.currentSprite = 0;


	/* load sprites */
	SDL_Surface *sheetSurface;
	sheetSurface = IMG_Load("sprites.png");
	if (sheetSurface == NULL) {
		printf("Cannot load star.png\n");
		SDL_Quit();
		exit(1);
	}
	gameState->spritesTexture= SDL_CreateTextureFromSurface(gameState->renderer, sheetSurface);
	SDL_FreeSurface(sheetSurface);

	/* init stars */
	/*int i = 0;*/
	/*for (i = 0; i < 100; i++) {*/
		/*gameState->stars[i].x = random() % 640;*/
		/*gameState->stars[i].y = random() % 480;*/
	/*}*/
}

int processEvents(SDL_Window *window, GameState *gameState) {
	SDL_Event event;
	int done = 0;
	while(SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_WINDOWEVENT_CLOSE:
				if (window) {
					SDL_DestroyWindow(window);
					window = NULL;
					done = 1;
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						done = 1;
						break;
				}
				break;
			case SDL_QUIT:
				done = 1;
				break;
		}
	}

	gameState->man.spriteX = MAN_SPRITE_STAND_X;
	gameState->man.spriteY = MAN_SPRITE_STAND_Y;

	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LEFT]) {
		gameState->man.walking = 1;
		gameState->man.facingLeft = 1;
		if ((gameState->man.currentSprite++)%2 == 0) {
			gameState->man.spriteX = MAN_SPRITE_MOVE_X;
			gameState->man.spriteY = MAN_SPRITE_MOVE_Y;
		} else {
			gameState->man.spriteX = MAN_SPRITE_STAND_X;
			gameState->man.spriteY = MAN_SPRITE_STAND_Y;
		}
		gameState->man.currentSprite %= 2;

		gameState->man.x -= 10;
	} else if (state[SDL_SCANCODE_RIGHT]) {
		gameState->man.walking = 1;
		gameState->man.facingLeft = 0;
		if ((gameState->man.currentSprite++)%6 == 0) {
			gameState->man.spriteX = MAN_SPRITE_MOVE_X;
			gameState->man.spriteY = MAN_SPRITE_MOVE_Y;
		} else {
			gameState->man.spriteX = MAN_SPRITE_STAND_X;
			gameState->man.spriteY = MAN_SPRITE_STAND_Y;
		}
		gameState->man.currentSprite %= 2;

		gameState->man.x += 10;
	} else if (state[SDL_SCANCODE_UP]) {
		gameState->man.y -= 10;
	} else if (state[SDL_SCANCODE_DOWN]) {
		gameState->man.y += 10;
	}

	return done;
}

void updateLogic(GameState *gameState) {
	globaltime++;
}

void doRender(SDL_Renderer *renderer, GameState *gameState) {
	/*set drawing color to blue*/
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	/*clear the screen to blue*/
	SDL_RenderClear(renderer);

	/*set drawing color to white*/
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	/*man*/
    SDL_Rect srcRect = { gameState->man.spriteX, gameState->man.spriteY , SPRITE_HEIGHT, SPRITE_WIDTH};
    SDL_Rect rect = { gameState->man.x, gameState->man.y, SPRITE_HEIGHT, SPRITE_WIDTH};
    SDL_RenderCopyEx(renderer, gameState->spritesTexture, &srcRect, &rect, 0, NULL, gameState->man.facingLeft);

	/*draw star*/
	/*int i;*/
	/*for (i = 0; i < 100; i++) {*/
		/*SDL_Rect starRect = {gameState->stars[i].x, gameState->stars[i].y, 64, 64};*/
		/*SDL_RenderCopy(renderer, gameState->star, NULL, &starRect);*/
	/*}*/


	/*we are done drawing, show the screen what we've drawn*/
	SDL_RenderPresent(renderer);
}

int main(int argc, char **argv) {

	SDL_Window *window;
	SDL_Renderer *renderer;

	SDL_Init(SDL_INIT_VIDEO);
	srandom(time(NULL));

	window = SDL_CreateWindow("GameWindow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	GameState gameState;
	gameState.renderer = renderer;
	loadGame(&gameState);

	int done = 0;
	while(!done) {
		/*check events*/
		done = processEvents(window, &gameState);

		/*Render*/
		doRender(renderer, &gameState);

		/*update logic*/
		updateLogic(&gameState);

		SDL_Delay(10); /*100s per frame*/
	}

	SDL_DestroyTexture(gameState.spritesTexture);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}
