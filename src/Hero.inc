#include "Variables.inc"
#include "LevelMapping.inc"

HeroLogic()
{
    currentTime = getTime();
    deltaTime = currentTime - timer.time;
    timer.time = currentTime;
    if(SELF_ID == HeroFacelet.module)
    {
        ChekCoinsCollision();
        if (!HeroMoveStatus)
        {
            new AccelX = MS_getFaceAccelX(HeroFacelet.screen);
            new AccelY = MS_getFaceAccelY(HeroFacelet.screen);
        
            Direction = (ABS(AccelX) > ABS(AccelY)) ? DIRECTION_X : DIRECTION_Y;
            new Accel = (ABS(AccelX) > ABS(AccelY)) ? AccelX : AccelY;


            chekHeroMovingWay(Direction, Accel);

        }
        else
        {
            MoveHero();
        }
    }
}

ChekCoinsCollision()
{
    if(HeroMoveStatus)
    {
        new StartPoint;
        new EndPoint;
        new AxisHeroPos;

        switch (Direction)
        {
            case DIRECTION_X:
            {
                StartPoint = HeroRect.x / 20;
                EndPoint = (HeroRect.x + 20) / 20;
                AxisHeroPos = HeroRect.y / 20;

                CollectCoin(AxisHeroPos, StartPoint);
                CollectCoin(AxisHeroPos, EndPoint);

            }
            case DIRECTION_Y:
            {
                StartPoint = HeroRect.y / 20;
                EndPoint = (HeroRect.y + 20) / 20;
                AxisHeroPos = HeroRect.x / 20;

                CollectCoin(StartPoint, AxisHeroPos);
                CollectCoin(EndPoint, AxisHeroPos);
            }
        }


    }
    else
    {
        CollectCoin(HeroGridPos.yPos, HeroGridPos.xPos);
    }
}

CollectCoin(xPos, yPos)
{
    if (MAP(HeroFacelet.screen)(xPos)(yPos) == 4)
    {
        MAP(HeroFacelet.screen)(xPos)(yPos) = 0;
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

    }

    return bFree;
}

chekHeroMovingWay(DirectionF, Accel)
{
    Direction = DirectionF;
    if (ABS(Accel) > MIN_ACCEL) 
    {

        if (Direction == DIRECTION_X && Accel > 0)
        {

            HeroGridTargetPos.yPos = HeroGridPos.yPos;
            for(new i = HeroGridPos.xPos; i < 12; i++)
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
            for(new i = HeroGridPos.yPos; i < 12; i++)
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
        if (HeroGridTargetPos.xPos*20 - HeroRect.x > 0)
        {
            distance = HeroGridTargetPos.xPos*20 - HeroRect.x;
            if (distance > deltaTime)
            {
                HeroRect.x = HeroRect.x + 1*deltaTime; 
            }
            else
            {
                HeroRect.x = HeroGridTargetPos.xPos*20;
                HeroGridPos.xPos = HeroGridTargetPos.xPos;
                HeroMoveStatus = false;
                if(HeroGridPos.xPos < 11)
                {
                    chekSpickeGameOver(Direction, 1);
                }
            }
        }
        else
        {
            distance = HeroGridTargetPos.xPos*20 - HeroRect.x;
            if (distance < -deltaTime)
            {
                HeroRect.x = HeroRect.x - 1*deltaTime; 
            }
            else
            {
                HeroRect.x = HeroGridTargetPos.xPos*20;
                HeroGridPos.xPos = HeroGridTargetPos.xPos;
                if(HeroGridPos.xPos == 0)
                {
                    CollectCoin(HeroGridPos.yPos, HeroGridPos.xPos);
                    chekWallAtNeighbor(LEFT_TOP, HeroGridPos.xPos, HeroGridPos.yPos, HeroFacelet);
                } else
                {
                    chekSpickeGameOver(Direction, -1);
                }
                HeroMoveStatus = false;
            }
        }
    }
    else if (Direction == DIRECTION_Y)
    {
        if (HeroGridTargetPos.yPos*20 - HeroRect.y > 0)
        {
            distance = HeroGridTargetPos.yPos*20 - HeroRect.y;
            if (distance > deltaTime)
            {
                HeroRect.y = HeroRect.y + 1*deltaTime; 
            }
            else
            {
                HeroRect.y = HeroGridTargetPos.yPos*20;
                HeroGridPos.yPos = HeroGridTargetPos.yPos;
                HeroMoveStatus = false;
                if(HeroGridPos.yPos < 11)
                {
                    chekSpickeGameOver(Direction, 1);
                }
            }
        }
        else
        {
            distance = HeroGridTargetPos.yPos*20 - HeroRect.y;
            if (distance < -deltaTime)
            {
                HeroRect.y = HeroRect.y - 1*deltaTime; 
            }
            else
            {
                HeroRect.y = HeroGridTargetPos.yPos*20;
                HeroGridPos.yPos = HeroGridTargetPos.yPos;
                if(HeroGridPos.yPos == 0)
                {
                    CollectCoin(HeroGridPos.yPos, HeroGridPos.xPos);
                    chekWallAtNeighbor(TOP_LEFT, HeroGridPos.xPos, HeroGridPos.yPos, HeroFacelet);
                } else
                {
                    chekSpickeGameOver(Direction, -1);
                }
                HeroMoveStatus = false;
            }
        }
    }
    chekWictoryOrGameOver();
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
                SendGameOver();
            }
        }

        case DIRECTION_Y:
        {
            if(MAP(HeroFacelet.screen)(HeroGridPos.yPos+AxisDir)(HeroGridPos.xPos) == 3)
            {
                //printf("Game Over \n");
                SendGameOver();
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

    if (WayIsFree(MODULES_MAP[neighbor.module * SCREEN_MAX + neighbor.screen][HeroGridPos.xPos][HeroGridPos.yPos]))
    {
        HeroFacelet.module = neighbor.module;
        HeroFacelet.screen = neighbor.screen;

        MoveHeroToNeighbor(neighbor.module, MOVE_FLAG, neighbor.screen, HeroGridPos.xPos, HeroGridPos.yPos);
    }
    if (MODULES_MAP[neighbor.module * SCREEN_MAX + neighbor.screen][HeroGridPos.xPos][HeroGridPos.yPos] == 3)
    {
        SendGameOver();
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
    new centerX = HeroRect.x + 10;
    new centerY = HeroRect.y + 10;

    GFX_drawImageXY(centerX, centerY, 0xFF, 0, 0, MIRROR_BLANK, ImageHero);
    //GFX_drawRectangle(HeroRect, GFX_fromARGB8888(255,255,255,000));
}