#include "Variables.inc"
#include "LevelMapping.inc"

HeroLogic()
{
    currentTime = getTime();
    deltaTime = currentTime - timer.time;
    timer.time = currentTime;
    if(SELF_ID == HeroFacelet.module)
    {
        HeroPreviosGridPos.xPos = HeroRect.x / (SIZE_SCREEN/SIZE_GRID_MAP);
        HeroPreviosGridPos.yPos = HeroRect.y / (SIZE_SCREEN/SIZE_GRID_MAP);
        if (!HeroMoveStatus)
        {
            new Dir = 0;
            new AccelX = MS_getFaceAccelX(HeroFacelet.screen);
            new AccelY = MS_getFaceAccelY(HeroFacelet.screen);
        
            Dir = (ABS(AccelX) > ABS(AccelY)) ? DIRECTION_X : DIRECTION_Y;
            new Accel = (ABS(AccelX) > ABS(AccelY)) ? AccelX : AccelY;
            

            chekHeroMovingWay(Dir, Accel);

        }
        else
        {
            MoveHero();
        }
        ChekCoinsCollision();
    }
}

ChekCoinsCollision()
{

    new CurrentGridMAxisPos;
    new StartPos;
    new EndPos;
    //new PreviosPos;

    if(HeroRect.x / (SIZE_SCREEN/SIZE_GRID_MAP) != HeroPreviosGridPos.xPos || HeroRect.y / (SIZE_SCREEN/SIZE_GRID_MAP) != HeroPreviosGridPos.yPos)
    {

        switch (Direction)
        {
            case DIRECTION_X:
            {
                CurrentGridMAxisPos = HeroRect.x / (SIZE_SCREEN/SIZE_GRID_MAP);

                if(CurrentGridMAxisPos - HeroPreviosGridPos.xPos > 0)
                {
                    StartPos = HeroPreviosGridPos.xPos;
                    EndPos = CurrentGridMAxisPos;
                }
                else
                {
                    StartPos = CurrentGridMAxisPos;
                    EndPos = HeroPreviosGridPos.xPos;
                }

                for(new i = StartPos; i <= EndPos; i++)
                {
                    CollectCoin(HeroGridPos.yPos, i);
                }

                //printf("Collect coin Axis X from %d to %d\n", StartPos, EndPos);

            }
            case DIRECTION_Y:
            {
                CurrentGridMAxisPos = HeroRect.y / (SIZE_SCREEN/SIZE_GRID_MAP);

                if(CurrentGridMAxisPos - HeroPreviosGridPos.yPos > 0)
                {
                    StartPos = HeroPreviosGridPos.yPos;
                    EndPos = CurrentGridMAxisPos;
                }
                else
                {
                    StartPos = CurrentGridMAxisPos;
                    EndPos = HeroPreviosGridPos.yPos;
                }

                for(new i = StartPos; i <= EndPos; i++)
                {
                    CollectCoin(i, HeroGridPos.xPos);
                }
                //printf("Collect coin Axis Y from %d to %d\n", StartPos, EndPos);
            }
        }
    }
    else
    {
        CollectCoin(HeroGridPos.yPos, HeroGridPos.xPos);
    }

}

CollectCoin(yPos, xPos)
{
    if (MAP(HeroFacelet.screen)(yPos)(xPos) == 4 || MAP(HeroFacelet.screen)(yPos)(xPos) == 5)
    {
        MAP(HeroFacelet.screen)(yPos)(xPos) = 0;
        CoinsNow += 1;
    }
 
}


WayIsFree(Cell)
{
    new bFree = false;
    switch (Cell)
    {
        case 0:
            bFree = true;

        case 2:
            bFree = true;

        case 4:
            bFree = true;

        case 5:
            bFree = true;

    }

    return bFree;
}

chekHeroMovingWay(DirectionF, Accel)
{
    //Direction = DirectionF;
    if (ABS(Accel) > MIN_ACCEL) 
    {
        Direction = DirectionF;
        ViewAxis = Accel;
        if (Direction == DIRECTION_X && Accel > 0)
        {

            HeroGridTargetPos.yPos = HeroGridPos.yPos;
            for(new i = HeroGridPos.xPos; i < SIZE_GRID_MAP; i++)
            {
                if (WayIsFree(MAP(HeroFacelet.screen)(HeroGridPos.yPos)(i)))
                {
                    HeroGridTargetPos.xPos = i;
                }
                else
                {
                    break;
                }
            }
        }
        if(Direction == DIRECTION_X && Accel < 0)
        {
            if (HeroGridPos.xPos == 0)
            {
                CollectCoin(HeroGridPos.yPos, HeroGridPos.xPos);
                chekWallAtNeighbor(LEFT_TOP, HeroGridPos.xPos, HeroGridPos.yPos, HeroFacelet);
            }
            HeroGridTargetPos.yPos = HeroGridPos.yPos;
            for(new i = HeroGridPos.xPos; i >= 0; i--)
            {
                if (WayIsFree(MAP(HeroFacelet.screen)(HeroGridPos.yPos)(i)))
                {
                    HeroGridTargetPos.xPos = i;
                }
                else
                {
                    break;
                }
            }
        }


        if(Direction == DIRECTION_Y && Accel > 0)
        {

            HeroGridTargetPos.xPos = HeroGridPos.xPos;
            for(new i = HeroGridPos.yPos; i < SIZE_GRID_MAP; i++)
            {
                if (WayIsFree(MAP(HeroFacelet.screen)(i)(HeroGridPos.xPos)))
                {
                    HeroGridTargetPos.yPos = i;
                }
                else
                {
                    break;
                }
            }
        }
        if(Direction == DIRECTION_Y && Accel < 0)
        {
            if (HeroGridPos.yPos == 0)
            {
                CollectCoin(HeroGridPos.yPos, HeroGridPos.xPos);
                chekWallAtNeighbor(TOP_LEFT, HeroGridPos.xPos, HeroGridPos.yPos, HeroFacelet);
            }
            HeroGridTargetPos.xPos = HeroGridPos.xPos;
            for(new i = HeroGridPos.yPos; i >= 0; i--)
            {
                if (WayIsFree(MAP(HeroFacelet.screen)(i)(HeroGridPos.xPos)))
                {
                    HeroGridTargetPos.yPos = i;
                }
                else
                {
                    break;
                }
            }
        }


        if (HeroGridPos.xPos != HeroGridTargetPos.xPos || HeroGridPos.yPos != HeroGridTargetPos.yPos )
        {
            HeroMoveStatus = true;
            //printf("MOVING\n");
            //printf("Accel = %d, Direction: %d\n", Accel, Direction);
            //printf("HeroGridTargetPos: x: %d, y: %d\n", HeroGridTargetPos.xPos, HeroGridTargetPos.yPos);
        }
    }
}

MoveHero()
{
    new distance = 0;

    if(Direction == DIRECTION_X)
    {
        if (HeroGridTargetPos.xPos*(SIZE_SCREEN/SIZE_GRID_MAP) - HeroRect.x > 0)
        {
            distance = HeroGridTargetPos.xPos*(SIZE_SCREEN/SIZE_GRID_MAP) - HeroRect.x;
            if (distance > deltaTime)
            {
                HeroRect.x = HeroRect.x + 1*deltaTime; 
            }
            else
            {
                HeroRect.x = HeroGridTargetPos.xPos*(SIZE_SCREEN/SIZE_GRID_MAP);
                HeroGridPos.xPos = HeroGridTargetPos.xPos;
                //CollectCoin(HeroGridPos.yPos, HeroGridPos.xPos - 1);
                HeroMoveStatus = false;
                if(HeroGridPos.xPos < SIZE_GRID_MAP-1)
                {
                    chekSpickeGameOver(Direction, 1);
                }
            }
        }
        else
        {
            distance = HeroGridTargetPos.xPos*(SIZE_SCREEN/SIZE_GRID_MAP) - HeroRect.x;
            if (distance < -deltaTime)
            {
                HeroRect.x = HeroRect.x - 1*deltaTime; 
            }
            else
            {
                HeroRect.x = HeroGridTargetPos.xPos*(SIZE_SCREEN/SIZE_GRID_MAP);
                HeroGridPos.xPos = HeroGridTargetPos.xPos;
                //CollectCoin(HeroGridPos.yPos, HeroGridPos.xPos + 1);
                HeroMoveStatus = false;
                if(HeroGridPos.xPos == 0)
                {
                    //CollectCoin(HeroGridPos.yPos, HeroGridPos.xPos);
                    ChekCoinsCollision();
                    chekWallAtNeighbor(LEFT_TOP, HeroGridPos.xPos, HeroGridPos.yPos, HeroFacelet);
                } else
                {
                    chekSpickeGameOver(Direction, -1);
                }
                //HeroMoveStatus = false;
            }
        }
    }
    else if (Direction == DIRECTION_Y)
    {
        if (HeroGridTargetPos.yPos*(SIZE_SCREEN/SIZE_GRID_MAP) - HeroRect.y > 0)
        {
            distance = HeroGridTargetPos.yPos*(SIZE_SCREEN/SIZE_GRID_MAP) - HeroRect.y;
            if (distance > deltaTime)
            {
                HeroRect.y = HeroRect.y + 1*deltaTime; 
            }
            else
            {
                HeroRect.y = HeroGridTargetPos.yPos*(SIZE_SCREEN/SIZE_GRID_MAP);
                HeroGridPos.yPos = HeroGridTargetPos.yPos;
                //CollectCoin(HeroGridPos.yPos - 1, HeroGridPos.xPos);
                HeroMoveStatus = false;
                if(HeroGridPos.yPos < SIZE_GRID_MAP-1)
                {
                    chekSpickeGameOver(Direction, 1);
                }
            }
        }
        else
        {
            distance = HeroGridTargetPos.yPos*(SIZE_SCREEN/SIZE_GRID_MAP) - HeroRect.y;
            if (distance < -deltaTime)
            {
                HeroRect.y = HeroRect.y - 1*deltaTime; 
            }
            else
            {
                HeroRect.y = HeroGridTargetPos.yPos*(SIZE_SCREEN/SIZE_GRID_MAP);
                HeroGridPos.yPos = HeroGridTargetPos.yPos;
                //CollectCoin(HeroGridPos.yPos + 1, HeroGridPos.xPos);
                HeroMoveStatus = false;
                if(HeroGridPos.yPos == 0)
                {
                    //CollectCoin(HeroGridPos.yPos, HeroGridPos.xPos);
                    ChekCoinsCollision();
                    chekWallAtNeighbor(TOP_LEFT, HeroGridPos.xPos, HeroGridPos.yPos, HeroFacelet);
                } else
                {
                    chekSpickeGameOver(Direction, -1);
                }
                //HeroMoveStatus = false;
            }
        }
    }
    chekWictoryOrGameOver();
}

chekWictoryOrGameOver()
{
    //printf("HeroScreen: %d, HeroGridPos.yPos: %d, HeroGridPos.xPos: %d\n", HeroFacelet.screen, HeroGridPos.yPos, HeroGridPos.xPos);
    // switch(MAP(HeroFacelet.screen)(HeroGridPos.yPos)(HeroGridPos.xPos))
    // {
    //     case 2:
    //     {
    //         new data[MESSAGE_SIZE] = [0, ...];

    //         data[0] = (e_MESSAGE_WICTORY << 0) | (0 << 8) | (0 << 16) | (0 << 24);
    //         broadcastMessage(data);
    //         //currentLevel +=1;
    //         GameStatus = e_GAME_STATUS_WIN;

    //     }
    //     case 3:
    //     {
            
    //     }
    // }

    if(MAP(HeroFacelet.screen)(HeroGridPos.yPos)(HeroGridPos.xPos) == 2)
    {
        //printf("is finish\n");
        IsFinish = true;

    }

}


chekSpickeGameOver(Direction, AxisDir)
{
    switch(Direction)
    {
        case DIRECTION_X:
        {
            if(MAP(HeroFacelet.screen)(HeroGridPos.yPos)(HeroGridPos.xPos+AxisDir) == 3)
            {
                //printf("Game Over \n");
                HeroMoveStatus = false;
                SendEvent(e_MESSAGE_GAMEOVER);
            }
        }

        case DIRECTION_Y:
        {
            if(MAP(HeroFacelet.screen)(HeroGridPos.yPos+AxisDir)(HeroGridPos.xPos) == 3)
            {
                //printf("Game Over \n");
                HeroMoveStatus = false;
                SendEvent(e_MESSAGE_GAMEOVER);
            }
        }
    }
}


chekWallAtNeighbor(MOVE_FLAG, GridxPos, GridyPos, HeroFacelet[TOPOLOGY_FACELET])
{
    new neighbor[TOPOLOGY_FACELET_INFO];

    switch (MOVE_FLAG)
    {
        case LEFT_TOP:
            neighbor = TOPOLOGY_getAdjacentFacelet(HeroFacelet, NEIGHBOR_LEFT);

        case TOP_LEFT:
            neighbor = TOPOLOGY_getAdjacentFacelet(HeroFacelet, NEIGHBOR_TOP);
    }

    if(neighbor.connected)
    {
        if (WayIsFree(MODULES_MAP[neighbor.module * SCREEN_MAX + neighbor.screen][HeroGridPos.xPos][HeroGridPos.yPos]))
        {
            HeroFacelet.module = neighbor.module;
            HeroFacelet.screen = neighbor.screen;

            MoveHeroToNeighbor(neighbor.module, MOVE_FLAG, neighbor.screen, HeroGridPos.xPos, HeroGridPos.yPos);
        }
        if (MODULES_MAP[neighbor.module * SCREEN_MAX + neighbor.screen][HeroGridPos.xPos][HeroGridPos.yPos] == 3)
        {
            SendEvent(e_MESSAGE_GAMEOVER);
        }
        HeroMoveStatus = false;
    }
    else
    {
        HeroMoveStatus = true;
    }
  
  
}


MoveHeroToNeighbor(MODULE_DEST, MOVE_FLAG, MODULE_DEST_SCREEN, HERO_X_POS, HERO_Y_POS)
{
    new data[MESSAGE_SIZE] = [0, ...];

    sendPacketIndex[MODULE_DEST] += 1;

    data[0] = (e_MESSAGE_MOVE_HERO_MTM << 0) | (MODULE_DEST << 8) | (MODULE_DEST_SCREEN << 16) | (SELF_ID << 24);
    data[1] = (sendPacketIndex[MODULE_DEST] << 0) | (MOVE_FLAG << 8) | (HERO_X_POS << 16) | (HERO_Y_POS << 24);

    broadcastMessage(data);
}

DrawHero(screenNumber)
{
    new centerX = HeroRect.x + (SIZE_SCREEN/SIZE_GRID_MAP)/2;
    new centerY = HeroRect.y + (SIZE_SCREEN/SIZE_GRID_MAP)/2;
    new Angle;
    new Mirror = MIRROR_BLANK;


    if(!HeroMoveStatus)
    {
        if(!IsFinish)
        {
            switch (Direction)
            {
                case DIRECTION_X:
                {
                    if(ViewAxis > 0)
                    {
                        Angle = 270;
                    }
                    else if (ViewAxis < 0)
                    {
                        Angle = 90;
                    }
                }
                case DIRECTION_Y:
                {
                    if(ViewAxis > 0)
                    {
                        Angle = 0;
                    }
                    else if (ViewAxis < 0)
                    {
                        Angle = 180;
                    }
                }
            }


            if (currentTime - HeroAnimationTimer.time > HeroAnimationTimer.delay)
            {
                HeroAnimationTimer.time = currentTime;

                if(HeroAnimIter == 4)
                {
                    HeroAnimIter = 0;
                }
                HeroAnimIter++;

            }

        
            //printf("Angle = %d, Mirror: %d\n", Angle, Mirror);
            //GFX_drawImageXY(centerX, centerY, 0xFF, 0, Angle, Mirror, HeroSprites[HeroAnimIter]);
            GFX_drawRectangle(HeroRect, 0xFFFFFF00);
        }
        else
        {
            //printf("IsFinish\n");
            if(FinishAnimationScene > 0)
            {
                FinishAnimationScene -= deltaTime;
                new Fixed: percent = 0;
                // percent = fdiv(fixed(FinishAnimationScene),fixed(FINISH_SCENE_TIME));
                percent = fdiv(FinishAnimationScene, FINISH_SCENE_TIME);
                //printf("percent = %q\n", percent);
                new  size = fint((SIZE_SCREEN/SIZE_GRID_MAP)/2*percent);
                //printf("size = %d\n", size);
                GFX_drawSolidCircleXY(centerX, centerY , size, 0xFF78DBE2);
            }
            else
            {
                FinishAnimationScene = FINISH_SCENE_TIME;
                IsFinish = false;

                SendEvent(e_MESSAGE_WICTORY);
                GameStatus = e_GAME_STATUS_WIN;

            }
        }

    }
    else 
    {
        GFX_drawSolidCircleXY(centerX, centerY , (SIZE_SCREEN/SIZE_GRID_MAP)/2, 0xFFFFFF00);
    }
}