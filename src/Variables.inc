#include "wowcore.inc"
#include "topology.inc"
#include "graphics.inc"
#include "motion_sensor.inc"
#include "math.inc"

#define GAME_VERSION "v0.0.9"

#define SIZE_SCREEN 240
#define MODULE_MAX 8
#define SCREEN_MAX 3
#define SIZE_GRID_MAP 12

#define MIN_ACCEL 15

#define DIRECTION_X 10
#define DIRECTION_Y 11

#define LEFT_TOP 101
#define TOP_LEFT 102


new MODULES_MAP [MODULE_MAX * SCREEN_MAX][SIZE_GRID_MAP][SIZE_GRID_MAP];
#define MAP(%0)(%1)(%2) MODULES_MAP[SELF_ID * SCREEN_MAX + %0][%1][%2]


new sendPacketIndex[8];
new recvPacketIndex[8];



new previousTime = 0;
new currentTime = 0;
new deltaTime = 0;


new CoinsColors[2] = [0xFFFFFF00, 0xFF78DBE2];
new CoinColor;

new timer[.delay, .time];
new HeroAnimationTimer[.delay, .time];
new CoinsAnimationTimer[.delay, .time];
new MapBackGroundAnimationTimer[.delay, .time];

new ImageWall;
new ImageSpike;
new ImageWallStick;
new ImageWallBack;

new BackGroundAlpha[5] = [0xFF, 0xF0, 0xE1, 0xD2, 0xC3];
new AlphaMapIter = 0;
new AlphaMapIterSide = 0;

new ViewAxis = 0;


new HeroAnimIter = 0;
new HeroSprites[5];
new HeroGridPos[.xPos, .yPos]; // Hero position at grid
new HeroPreviosGridPos[.xPos, .yPos];
new HeroGridTargetPos[.xPos, .yPos]; // Hero target position at grid
new HeroFacelet[TOPOLOGY_FACELET]; // Hero facelet - module and screen
new HeroRect[GFX_RECTANGLE]; // hero sprite rect

new bool: HeroMoveStatus = false; // hero move status move and static
new Direction; // direction of moving hero


const e_MESSAGES_FLAG:{
    e_MESSAGE_MOVE_HERO_MTM = 0,
    e_MESSAGE_STARTEVENT,
    e_MESSAGE_WICTORY,
    e_MESSAGE_GAMEOVER,
    e_MESSAGE_MAX
}


new currentLevel;

new MenuFacelet[TOPOLOGY_FACELET];
new MenuPlace[TOPOLOGY_PLACE];
new MenuFace;
new ImageStar[.empty, .flood];
new ImageArrows;
new ImageHero;

new GameStatus;

const e_GAME_STATUS:{
    e_GAME_STATUS_MENU = 0,
    e_GAME_STATUS_GAME,
    e_GAME_STATUS_WIN,
    e_GAME_STATUS_GAMEOVER,
    e_GAME_STATUS_MAX
}

const e_GAME_LEVEL:{
    e_GAME_LEVEL_1 = 0,
    e_GAME_LEVEL_2,
    e_GAME_LEVEL_3,
    e_GAME_LEVEL_MAX
}


#define QUERRY_CHECK 200
#define RESPONCE_CHECK 201

#define MOVING_OK 30
#define MOVING_NOT 30