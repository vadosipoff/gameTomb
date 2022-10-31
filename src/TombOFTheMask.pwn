// This PAWN script is generated by WOWCube SDK project wizard

#define G2D

#pragma warning disable 203
#pragma warning disable 213
#pragma warning disable 229

// Includes
// -----------------------------

#include "wowcore.inc"
#include "topology.inc"
#include "graphics.inc"
#include "motion_sensor.inc"
#include "math.inc"
#include "console.inc"





#include "LevelMapping.inc"
#include "Hero.inc"
#include "LevelMenu.inc"




// WOWCube application callbacks 
// -----------------------------

//Applicaton initialization callback. Called once when CUB application starts
public ON_Init(id, size, const pkt[])
{


    ImageStar.flood = GFX_getAssetId("star_empty.png");
    ImageArrows = GFX_getAssetId("arrows.png");
    ImageWall = GFX_getAssetId("Wall5.png");
    ImageSpike = GFX_getAssetId("Spike.png");
    ImageHero = GFX_getAssetId("Tomb.png");

    ImageWallStick = GFX_getAssetId("ImageWallStick.png");
    ImageWallBack = GFX_getAssetId("ImageWallBack2.png");

    HeroSprites[0] = GFX_getAssetId("tomb1.png");
    HeroSprites[1] = GFX_getAssetId("tomb2.png");
    HeroSprites[2] = GFX_getAssetId("tomb3.png");
    HeroSprites[3] = GFX_getAssetId("tomb4.png");
    HeroSprites[4] = GFX_getAssetId("tomb5.png");


    CoinColor = 0;

    timer.time = getTime();
    CoinsAnimationTimer.time = timer.time;
    CoinsAnimationTimer.delay = 200;
    MapBackGroundAnimationTimer.time = timer.time;
    MapBackGroundAnimationTimer.delay = 100;
    HeroAnimationTimer.time = timer.time;
    HeroAnimationTimer.delay = 100;

    currentLevel = 1;

    GameStatus = e_GAME_STATUS_MENU;

}

//Saved application state data load callback. Gets called in response to loadState() function call
public ON_Load(id, size, const pkt[]) 
{
}

//Main run loop callback. Gets called recurrently by the CUB application as frequent as application code allows. 
public ON_Tick()
{

    switch (GameStatus)
    {
        case e_GAME_STATUS_MENU:
        {
            Menu();
        }
        case e_GAME_STATUS_GAME:
        {
            HeroLogic();
            DrawGame();
        }
        case e_GAME_STATUS_WIN:
        {
            WinScreen();
        }
        case e_GAME_STATUS_GAMEOVER:
        {
            GameOverScreen();
        }
    }

}

//This callback is gets called immediately after ON_Tick(). Use it for calling your rendering code. 
public ON_Render()
{
}

//The "physics" callback. Gets called recurrently with 30ms resolution. 
public ON_PhysicsTick() 
{
}

//The "inner network" callback. Gets called when WOWCube module receives a data packet from other module
public ON_Message(const pkt[MESSAGE_SIZE])
{
    
    switch(parseByte(pkt, 0))
    {
        case e_MESSAGE_MOVE_HERO_MTM:
        {
            if(SELF_ID == parseByte(pkt, 1))
            {
                if(parseByte(pkt, 4) != recvPacketIndex[parseByte(pkt, 3)])
                {
                    recvPacketIndex[parseByte(pkt, 3)] = parseByte(pkt, 4);

                    HeroGridPos.xPos = parseByte(pkt, 7);
                    HeroGridPos.yPos = parseByte(pkt, 6);

                    HeroRect.x = HeroGridPos.xPos * 20;
                    HeroRect.y = HeroGridPos.yPos * 20;

                    HeroFacelet.module = SELF_ID;
                    HeroFacelet.screen = parseByte(pkt, 2);

                    if(parseByte(pkt, 5) == LEFT_TOP)
                    {
                        chekHeroMovingWay(DIRECTION_Y, (MIN_ACCEL+1));
                        chekSpickeGameOver(DIRECTION_Y, 1);
                    }
                    if(parseByte(pkt, 5) == TOP_LEFT)
                    {
                        chekHeroMovingWay(DIRECTION_X, (MIN_ACCEL+1));
                        chekSpickeGameOver(DIRECTION_X, 1);
                    }

                }
            }
        }

        case e_MESSAGE_WICTORY:
        {
            //printf("Module %d, status Wictory\n", SELF_ID);
            //currentLevel = parseByte(pkt, 1);

            GameStatus = e_GAME_STATUS_WIN;
            
        }
        case e_MESSAGE_GAMEOVER:
        {
            GameStatus = e_GAME_STATUS_GAMEOVER;
        }
    }

}

//The cube topology change callback. Gets called when cube is twisted and its topological desctiption has been changed
public ON_Twist(twist[TOPOLOGY_TWIST_INFO]) 
{
    switch (GameStatus)
    {
        case e_GAME_STATUS_MENU:
        {
            if(twist.direction == 2 && currentLevel < e_GAME_LEVEL_MAX)
            {
                currentLevel++;
            }
            else if (twist.direction == 1 && currentLevel > 1)
            {
                currentLevel--;
            }
        }

        case e_GAME_STATUS_GAME:
        {
        }

        case e_GAME_STATUS_WIN:
        {
            currentLevel +=1;
            GameStatus = e_GAME_STATUS_MENU;
        }

        case e_GAME_STATUS_GAMEOVER:
        {
            ///////////////////////////////////////////////
            /////////////////////////////////////////////// 
            InitLevelMap(currentLevel);
            GameStatus = e_GAME_STATUS_GAME;
            ///////////////////////////////////////////////
            ///////////////////////////////////////////////
        }
     

    }
}

//Device shake detection callback.
public ON_Shake(const count) 
{
}

//Screen tap callback.
public ON_Tap(const count, const display, const bool:opposite) 
{

    switch (GameStatus)
    {
        case e_GAME_STATUS_MENU:
        {
            if(count == 2)
            {
                InitLevelMap(currentLevel);
                GameStatus = e_GAME_STATUS_GAME;
            }

        }

        case e_GAME_STATUS_GAME:
        {
        }
    }
    printf("Screen: %d\n", display);
}

//Application quit callback.
public ON_Quit()
{
    //
    //Save game data here
    //
}