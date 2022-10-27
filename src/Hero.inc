#include "Variables.inc"
#include "LevelMapping.inc"

HeroLogic()
{
    currentTime = getTime();
    deltaTime = currentTime - timer.time;
    timer.time = currentTime;
    if(SELF_ID == HeroFacelet.module)
    {
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
                if (MAP(HeroFacelet.screen)(HeroGridPos.yPos)(i) == 0)
                {
                    HeroGridTargetPos.xPos = i;
                }
                if (MAP(HeroFacelet.screen)(HeroGridPos.yPos)(i) != 0)
                {
                    break;
                }
            }
        }
        if(Direction == DIRECTION_X && Accel < 0)
        {
            if (HeroGridPos.xPos == 0)
            {
                chekWallAtNeighbor(LEFT_TOP, HeroGridPos.xPos, HeroGridPos.yPos, HeroFacelet);
            }
            HeroGridTargetPos.yPos = HeroGridPos.yPos;
            for(new i = HeroGridPos.xPos; i >= 0; i--)
            {
                if (MAP(HeroFacelet.screen)(HeroGridPos.yPos)(i) == 0)
                {
                    HeroGridTargetPos.xPos = i;
                }
                if (MAP(HeroFacelet.screen)(HeroGridPos.yPos)(i) != 0)
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
                if (MAP(HeroFacelet.screen)(i)(HeroGridPos.xPos) == 0)
                {
                    HeroGridTargetPos.yPos = i;
                }
                if (MAP(HeroFacelet.screen)(i)(HeroGridPos.xPos) != 0)
                {
                    break;
                }
            }
        }
        if(Direction == DIRECTION_Y && Accel < 0)
        {
            if (HeroGridPos.yPos == 0)
            {
                chekWallAtNeighbor(TOP_LEFT, HeroGridPos.xPos, HeroGridPos.yPos, HeroFacelet);
            }
            HeroGridTargetPos.xPos = HeroGridPos.xPos;
            for(new i = HeroGridPos.yPos; i >= 0; i--)
            {
                if (MAP(HeroFacelet.screen)(i)(HeroGridPos.xPos) == 0)
                {
                    HeroGridTargetPos.yPos = i;
                }
                if (MAP(HeroFacelet.screen)(i)(HeroGridPos.xPos) != 0)
                {
                    break;
                }
            }
        }


        if (HeroGridPos.xPos != HeroGridTargetPos.xPos || HeroGridPos.yPos != HeroGridTargetPos.yPos )
        {
            HeroMoveStatus = true;
            printf("MOVING\n");
            printf("Accel = %d, Direction: %d\n", Accel, Direction);
            printf("HeroGridTargetPos: x: %d, y: %d\n", HeroGridTargetPos.xPos, HeroGridTargetPos.yPos);
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
                    chekWallAtNeighbor(LEFT_TOP, HeroGridPos.xPos, HeroGridPos.yPos, HeroFacelet);
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
                    if(MAP(HeroFacelet.screen)(HeroGridPos.yPos+1)(HeroGridPos.xPos) == 3)
                    {
                        printf("Game Over \n");
                    }
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
                    chekWallAtNeighbor(TOP_LEFT, HeroGridPos.xPos, HeroGridPos.yPos, HeroFacelet);
                }
                HeroMoveStatus = false;
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

    if (MODULES_MAP[neighbor.module * SCREEN_MAX + neighbor.screen][HeroGridPos.xPos][HeroGridPos.yPos] == 0)
    {
        HeroFacelet.module = neighbor.module;
        HeroFacelet.screen = neighbor.screen;

        MoveHeroToNeighbor(neighbor.module, MOVE_FLAG, neighbor.screen, HeroGridPos.xPos, HeroGridPos.yPos);
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