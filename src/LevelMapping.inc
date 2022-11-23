#include "Variables.inc"

DrawGame()
{

    currentTime = getTime();
    if (currentTime - CoinsAnimationTimer.time > CoinsAnimationTimer.delay)
    {
        CoinsAnimationTimer.time = currentTime;
        CoinColor = (CoinColor == 0) ? 1 : 0;

        if(BigCoinsAnimationIterSide == 0)
        {
            BigCoinsAnimationIter++;
            if(BigCoinsAnimationIter == 3)
            {
                BigCoinsAnimationIterSide = 1;
            }
        }
        else if(BigCoinsAnimationIterSide == 1)
        {
            BigCoinsAnimationIter--;
            if(BigCoinsAnimationIter == 0)
            {
                BigCoinsAnimationIterSide = 0;
            }
        }
        //printf("BigCoinsAnimationIter = %d\n", BigCoinsAnimationIter);
    }
    
    // if (currentTime - MapBackGroundAnimationTimer.time > MapBackGroundAnimationTimer.delay)
    // {
    //     MapBackGroundAnimationTimer.time = currentTime;

    //     if(AlphaMapIterSide == 0)
    //     {
    //         AlphaMapIter++;
    //         if(AlphaMapIter == 4)
    //         {
    //             AlphaMapIterSide = 1;
    //         }
    //     }
    //     else if(AlphaMapIterSide == 1)
    //     {
    //         AlphaMapIter--;
    //         if(AlphaMapIter == 0)
    //         {
    //             AlphaMapIterSide = 0;
    //         }
    //     }
    //     //printf("AlphaMapIter: %d\n", AlphaMapIter);
        
    // }

    if (currentTime - FinishAnimationTimer.time > FinishAnimationTimer.delay)
    {
        FinishAnimationTimer.time = currentTime;
        new bufColor = FinishColors[0];
        FinishColors[0] = FinishColors[1];
        FinishColors[1] = bufColor;
    }


    for(new screenNumber = 0; screenNumber<3; screenNumber++)
    {
        GFX_setRenderTarget(screenNumber);
        
        GFX_drawBakedImage([120, 120], BackGroundAlpha[AlphaMapIter], 0x0, 0, MIRROR_BLANK, screenNumber);

        DrawCoins(screenNumber);
        DrawFinishPoint(screenNumber);

        if(SELF_ID == HeroFacelet.module && screenNumber == HeroFacelet.screen)
        {
            DrawHero();
        }


        GFX_drawText([200, 230], 4, 0, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, GAME_VERSION);
        GFX_render();

    }
}

DrawCoins(screenNumber)
{

    for(new i = 0; i < SIZE_LOGIC_GRID; i++)
    {
        for(new j = 0; j < SIZE_LOGIC_GRID; j++)
        {
            if (MAP(screenNumber)(i)(j) == 4)
            {
                new rect[GFX_RECTANGLE];
                rect.h = 4;
                rect.w = 4;

                rect.x = j * (SIZE_SCREEN/SIZE_GRID_MAP) + (SIZE_SCREEN/SIZE_GRID_MAP)/2 - rect.w/2 - INDENT;
                rect.y = i * (SIZE_SCREEN/SIZE_GRID_MAP) + (SIZE_SCREEN/SIZE_GRID_MAP)/2 - rect.h/2 - INDENT;

                GFX_drawRectangle(rect, CoinsColors[CoinColor]);
            }

            if (MAP(screenNumber)(i)(j) == 5)
            {
                new Mirror = MIRROR_BLANK;
                new CenterX = j * (SIZE_SCREEN/SIZE_GRID_MAP) + (SIZE_SCREEN/SIZE_GRID_MAP)/2;
                new CenterY = i * (SIZE_SCREEN/SIZE_GRID_MAP) + (SIZE_SCREEN/SIZE_GRID_MAP)/2;
                if (BigCoinsAnimationIterSide == 0)
                {
                    Mirror = MIRROR_BLANK;
                }
                else
                {
                    Mirror = MIRROR_X;
                }
                //printf("Mirror = %d\n", Mirror);
                GFX_drawImageXY(CenterX-INDENT, CenterY-INDENT, 0xFF, 0, 0, Mirror, BigCoinsImages[BigCoinsAnimationIter]);
            }
        }
    }

}

DrawFinishPoint(screenNumber)
{
    if(FinishFacelet.module == SELF_ID && FinishFacelet.screen == screenNumber)
    {
        new rectF[GFX_RECTANGLE];

        new step = 4;
        new color;
        for(new i = 0; i < 10; i++)
        {
            rectF.h = (SIZE_SCREEN/SIZE_GRID_MAP) - i*step;
            rectF.w = (SIZE_SCREEN/SIZE_GRID_MAP) - i*step;
            rectF.x = FinishGridPos.xPos * (SIZE_SCREEN/SIZE_GRID_MAP) + ((SIZE_SCREEN/SIZE_GRID_MAP) - rectF.w)/2 - INDENT;
            rectF.y = FinishGridPos.yPos * (SIZE_SCREEN/SIZE_GRID_MAP) + ((SIZE_SCREEN/SIZE_GRID_MAP) - rectF.h)/2 - INDENT;


            if (i == 0)
            {
                color = 0xFFFFFF00;
            }
            else
            {
                if((i % 2) == 0)
                {
                    color = FinishColors[0];
                }
                else
                {
                    color = FinishColors[1];
                }
            }
            

            GFX_drawRectangle(rectF, color);
        }

    }
    
}


SendEvent(e_MESSAGE_EVENT)
{
    new data[MESSAGE_SIZE] = [0, ...];

    switch(e_MESSAGE_EVENT)
    {
        case e_MESSAGE_EVENT_START:
        {
            data[0] = (e_MESSAGE_EVENT_START << 0) | (0 << 8) | (0 << 16) | (SELF_ID << 24);
            data[1] = (0 << 0) | (0 << 8) | (0 << 16) | (0 << 24);
        }
        case e_MESSAGE_LEVEL_INIT_COMPLETE:
        {
            if(SELF_ID == 0)
            {
                LEVEL_MODULE_INIT[SELF_ID] = 1;
            }
            else
            {
                new MODULE_DEST = 0;

                sendPacketIndex[MODULE_DEST] += 1;

                data[0] = (e_MESSAGE_LEVEL_INIT_COMPLETE << 0) | (MODULE_DEST << 8) | (0 << 16) | (SELF_ID << 24);
                data[1] = (sendPacketIndex[MODULE_DEST] << 0) | (0 << 8) | (0 << 16) | (0 << 24);

            }
        }
        case e_MESSAGE_LEVEL_INIT:
        {  
            LevelInitPacket += 1;

            data[0] = (e_MESSAGE_LEVEL_INIT << 0) | (0 << 8) | (0 << 16) | (SELF_ID << 24);
            data[1] = (LevelInitPacket << 0) | (0 << 8) | (0 << 16) | (0 << 24);
            
        }
        case e_MESSAGE_GAMEOVER:
        {
            new data[MESSAGE_SIZE] = [0, ...];
            data[0] = (e_MESSAGE_GAMEOVER << 0) | (0 << 8) | (0 << 16) | (0 << 24);
            //currentLevel +=1;
            broadcastMessage(data);
            GameStatus = e_GAME_STATUS_GAMEOVER;
        }
        case e_MESSAGE_WICTORY:
        {
            data[0] = (e_MESSAGE_WICTORY << 0) | (CoinsNow << 8) | (CoinsAtLevel << 16) | (0 << 24);
        }
    }
    broadcastMessage(data);
}


BakeStaticLevelMap()
{
    FinishFacelet.module = -1;
    FinishFacelet.screen = -1;
    for(new screenNumber = 0; screenNumber<3; screenNumber++)
    {
        GFX_bakeImage(screenNumber, SIZE_SCREEN, SIZE_SCREEN, FORMAT_RGB565);
        GFX_clear(0xFF000000);
        new centerX = 0;
        new centerY = 0;
        new rect[GFX_RECTANGLE];
        rect.h = (SIZE_SCREEN/SIZE_GRID_MAP);
        rect.w = (SIZE_SCREEN/SIZE_GRID_MAP);

        for(new i = 0; i < SIZE_LOGIC_GRID; i++)
        {
            for(new j = 0; j < SIZE_LOGIC_GRID; j++)
            {
                if (MAP(screenNumber)(i)(j) == 1)
                {
                    rect.x = j * (SIZE_SCREEN/SIZE_GRID_MAP);
                    rect.y = i * (SIZE_SCREEN/SIZE_GRID_MAP);
                    centerX = rect.x + (SIZE_SCREEN/SIZE_GRID_MAP)/2;
                    centerY = rect.y + (SIZE_SCREEN/SIZE_GRID_MAP)/2;
                    //GFX_drawRectangle(rect, GFX_fromARGB8888(255, 153, 000, 204));
                    DrawCellWall(screenNumber, i, j, ImageWallStick);
                    //GFX_drawImageXY(centerX, centerY, 0xFF, 0, 0, MIRROR_BLANK, ImageWall);
                }

                if (MAP(screenNumber)(i)(j) == 2)
                {
                    FinishFacelet.module = SELF_ID;
                    FinishFacelet.screen = screenNumber;
                    FinishGridPos.xPos = j;
                    FinishGridPos.yPos = i;
                }

                if (MAP(screenNumber)(i)(j) == 3)
                {

                    rect.x = j * (SIZE_SCREEN/SIZE_GRID_MAP);
                    rect.y = i * (SIZE_SCREEN/SIZE_GRID_MAP);
                    centerX = rect.x + (SIZE_SCREEN/SIZE_GRID_MAP)/2;
                    centerY = rect.y + (SIZE_SCREEN/SIZE_GRID_MAP)/2;
                    //GFX_drawRectangle(rect, GFX_fromARGB8888(255, 153, 000, 204));
                    DrawCellWall(screenNumber, i, j, ImageSpikes);
                    //GFX_drawImageXY(centerX, centerY, 0xFF, 0, 0, MIRROR_BLANK, ImageWall);
                }
                if (MAP(screenNumber)(i)(j) == 4)
                {
                    CoinsAtLevel += 1;
                }
            }
        }

        GFX_render();

    }
    printf("END backed\n");
    GameStatus = e_GAME_STATUS_GAME;

}


DrawCellWall(screenNumber, i, j, Image)
{
    new PosX = j*(SIZE_SCREEN/SIZE_GRID_MAP)+(SIZE_SCREEN/SIZE_GRID_MAP)/2 - INDENT;
    new PosY = i*(SIZE_SCREEN/SIZE_GRID_MAP)+(SIZE_SCREEN/SIZE_GRID_MAP)/2 - INDENT;

    GFX_drawImageXY(PosX, PosY, 0xFF, 0, 0, MIRROR_BLANK, ImageWallBack);


    
    if(j == 0)
    {
        GFX_drawImageXY(PosX, PosY, 0xFF, 0, 270, MIRROR_BLANK, ImageWallStick);
    }
    if(j == 11)
    {
        GFX_drawImageXY(PosX, PosY, 0xFF, 0, 90, MIRROR_BLANK, ImageWallStick);
    }
    if(i == 0)
    {
        GFX_drawImageXY(PosX, PosY, 0xFF, 0, 0, MIRROR_BLANK, ImageWallStick);
    }
    if(i == 11)
    {
        GFX_drawImageXY(PosX, PosY, 0xFF, 0, 180, MIRROR_BLANK, ImageWallStick);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////

    if(MAP(screenNumber)(i)(j) == 1)
    {
        if((i - 1 >= 0) && (j - 1 >= 0))
        {
            if(!WayIsFree(MAP(screenNumber)(i-1)(j)) && !WayIsFree(MAP(screenNumber)(i)(j-1)) && WayIsFree(MAP(screenNumber)(i-1)(j-1)))
            {
                GFX_drawImageXY(PosX, PosY, 0xFF, 0, 0, MIRROR_BLANK, ImageWallAngleSingle);
            }
        }
        if((i - 1 >= 0) && (j + 1 < SIZE_GRID_MAP))
        {
            if(!WayIsFree(MAP(screenNumber)(i-1)(j)) && !WayIsFree(MAP(screenNumber)(i)(j+1)) && WayIsFree(MAP(screenNumber)(i-1)(j+1)))
            {
                GFX_drawImageXY(PosX, PosY, 0xFF, 0, 90, MIRROR_BLANK, ImageWallAngleSingle);
            }
        }

        if((i + 1 < SIZE_GRID_MAP) && (j + 1 < SIZE_GRID_MAP))
        {
            if(!WayIsFree(MAP(screenNumber)(i+1)(j)) && !WayIsFree(MAP(screenNumber)(i)(j+1)) && WayIsFree(MAP(screenNumber)(i+1)(j+1)))
            {
                GFX_drawImageXY(PosX, PosY, 0xFF, 0, 180, MIRROR_BLANK, ImageWallAngleSingle);
            }
        }
        if((i + 1 < SIZE_GRID_MAP) && (j - 1 >= 0))
        {
            if(!WayIsFree(MAP(screenNumber)(i+1)(j)) && !WayIsFree(MAP(screenNumber)(i)(j-1)) && WayIsFree(MAP(screenNumber)(i+1)(j-1)))
            {
                GFX_drawImageXY(PosX, PosY, 0xFF, 0, 270, MIRROR_BLANK, ImageWallAngleSingle);
            }
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////

    if(i + 1 < SIZE_GRID_MAP)
    {
        if(WayIsFree(MAP(screenNumber)(i+1)(j)))
        {
            //GFX_drawImageXY(PosX, PosY, 0xFF, 0, 180, MIRROR_BLANK, ImageWallAngle);
            GFX_drawImageXY(PosX, PosY, 0xFF, 0, 180, MIRROR_BLANK, Image);
        }
    }
    if(i - 1 >= 0)
    {
        if(WayIsFree(MAP(screenNumber)(i-1)(j)))
        {
            //GFX_drawImageXY(PosX, PosY, 0xFF, 0, 0, MIRROR_BLANK, ImageWallAngle);
            GFX_drawImageXY(PosX, PosY, 0xFF, 0, 0, MIRROR_BLANK, Image);
        }
    }

    if(j + 1 < SIZE_GRID_MAP)
    {
        if(WayIsFree(MAP(screenNumber)(i)(j+1)))
        {
            //GFX_drawImageXY(PosX, PosY, 0xFF, 0, 90, MIRROR_BLANK, ImageWallAngle);
            GFX_drawImageXY(PosX, PosY, 0xFF, 0, 90, MIRROR_BLANK, Image);
        }
    }
    if(j - 1 >= 0)
    {
        if(WayIsFree(MAP(screenNumber)(i)(j-1)))
        {
            //GFX_drawImageXY(PosX, PosY, 0xFF, 0, 270, MIRROR_BLANK, ImageWallAngle);
            GFX_drawImageXY(PosX, PosY, 0xFF, 0, 270, MIRROR_BLANK, Image);
        }
    }

}



InitLevelMap(LEVEL)
{
    IsFinish = false;
    CoinsAtLevel = 0;
    CoinsNow = 0;
    FinishAnimationScene = FINISH_SCENE_TIME;

    HeroFacelet.module = 0;
    HeroFacelet.screen = 0;
    Direction = DIRECTION_Y;
    ViewAxis = 20;


    BigCoinAnimationMiror = 0;
    BigCoinsAnimationIter = 0;
    BigCoinsAnimationIterSide = 0;

    HeroRect.w = (SIZE_SCREEN/SIZE_GRID_MAP);
    HeroRect.h = (SIZE_SCREEN/SIZE_GRID_MAP);

    if(LEVEL == 1)
    {
        HeroGridPos.xPos = 1;
        HeroGridPos.yPos = 1;
        HeroRect.x =  HeroGridPos.xPos * (SIZE_SCREEN/SIZE_GRID_MAP);
        HeroRect.y =  HeroGridPos.yPos * (SIZE_SCREEN/SIZE_GRID_MAP);

        new MODULES_MAP_1[MODULE_MAX*SCREEN_MAX][SIZE_LOGIC_GRID][SIZE_LOGIC_GRID] = 
        [
            /// MODULE 0 ////////////////////////////////////////////////////////////////////////////////////
            [
            [1, 1, 1, 5, 1, 1, 1],
            [1, 4, 5, 4, 4, 4, 1],
            [1, 1, 1, 5, 1, 5, 1],
            [1, 4, 4, 4, 1, 4, 1],
            [1, 5, 1, 1, 1, 4, 1],
            [1, 4, 4, 5, 5, 4, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            /////////////////////////////////////////////////////////////////////////////////////////////////

            /// MODULE 1 ////////////////////////////////////////////////////////////////////////////////////
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            /////////////////////////////////////////////////////////////////////////////////////////////////

            /// MODULE 2 ////////////////////////////////////////////////////////////////////////////////////
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 4, 4, 5, 4, 2, 1],
            [1, 4, 1, 1, 1, 1, 1],
            [1, 5, 4, 4, 5, 4, 1],
            [1, 1, 1, 1, 1, 4, 1],
            [4, 5, 5, 4, 4, 4, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            /////////////////////////////////////////////////////////////////////////////////////////////////

            /// MODULE 3 ////////////////////////////////////////////////////////////////////////////////////
            [
            [1, 1, 1, 1, 1, 5, 1],
            [1, 1, 5, 4, 4, 4, 1],
            [1, 1, 4, 1, 1, 1, 1],
            [4, 5, 4, 5, 4, 4, 1],
            [1, 1, 4, 1, 1, 5, 1],
            [1, 1, 5, 4, 4, 4, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            /////////////////////////////////////////////////////////////////////////////////////////////////

            /// MODULE 4 ////////////////////////////////////////////////////////////////////////////////////
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            /////////////////////////////////////////////////////////////////////////////////////////////////

            /// MODULE 5 ////////////////////////////////////////////////////////////////////////////////////
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            /////////////////////////////////////////////////////////////////////////////////////////////////

            /// MODULE 6 ////////////////////////////////////////////////////////////////////////////////////
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            /////////////////////////////////////////////////////////////////////////////////////////////////

            /// MODULE 7 ////////////////////////////////////////////////////////////////////////////////////
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ],
            [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 1, 1, 1, 1, 1]
            ]
            /////////////////////////////////////////////////////////////////////////////////////////////////
        ]
        MODULES_MAP = MODULES_MAP_1;
    }


    BakeStaticLevelMap();
    //SendEvent(e_MESSAGE_LEVEL_INIT_COMPLETE);
}