#include "wowcore.inc"
#include "topology.inc"
#include "graphics.inc"
#include "motion_sensor.inc"
#include "math.inc"
#include "console.inc"



Menu()
{
    MenuFacelet.module = SELF_ID;

    for(new screenNumber = 0; screenNumber<3; screenNumber++)
    {
        GFX_setRenderTarget(screenNumber);
        GFX_clear(0xFF000000);

        MenuFacelet.screen = screenNumber;

        MenuPlace = TOPOLOGY_getPlace(MenuFacelet);

        new TOPOLOGY_orientation: orientation = TOPOLOGY_getFaceletOrientation(MenuFacelet);

        //if(orientation == ORIENTATION_UP)
        if(MenuPlace.face == ORIENTATION_UP)
        {
            new angle = 0;
            switch(MenuPlace.position)
            {
                case 0:
                    angle = 0;

                case 1:
                    angle = 270;

                case 2:
                    angle = 180;

                case 3:
                    angle = 90;
            }

            if(MenuPlace.position == 3)
            {
                GFX_drawText([100, 120], 20, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "%d", currentLevel);
                GFX_drawText([160, 120], 20, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "LEVEL");
                //GFX_drawText([20, 120], 6, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "Double Tap to Start");
            }

            if(MenuPlace.position == 0)
            {
                GFX_drawImage([50,120], 0xFF, 0, angle, MIRROR_BLANK, ImageStar.flood);
                GFX_drawImage([120,120], 0xFF, 0, angle, MIRROR_BLANK, ImageStar.flood);
                GFX_drawImage([190,120], 0xFF, 0, angle, MIRROR_BLANK, ImageStar.flood);
            }

            if(MenuPlace.position == 2)
            {
                GFX_drawText([120, 155], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "TWIST TO");
                GFX_drawText([120, 115], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "START");
                GFX_drawText([120, 75], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "LEVEL");
            }

            if(MenuPlace.position == 1)
            {
                GFX_drawImage([120,120], 0xFF, 0, angle, MIRROR_BLANK, ImageArrows);
                
            }

        }

        GFX_render();
    }

}


WinScreen()
{
    //printf("Drawing Win screen \n");
    new FaceletW[TOPOLOGY_FACELET];
    new PlaceW[TOPOLOGY_PLACE];

    FaceletW.module = SELF_ID;

    for(new screenNumber = 0; screenNumber<3; screenNumber++)
    {
        GFX_setRenderTarget(screenNumber);
        GFX_clear(0xFF000000);

        FaceletW.screen = screenNumber;
        PlaceW = TOPOLOGY_getPlace(FaceletW);


        new angle = 0;
        switch(PlaceW.position)
        {
            case 0:
            {
                angle = 0;
                GFX_drawText([120, 100], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "LEVEL %d", currentLevel);
                GFX_drawText([120, 140], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "PASSED");
                //GFX_drawText([120, 180], 6, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "Coins %d out of %d", CoinsNow, CoinsAtLevel);
            }
            case 1:
            {
                angle = 270;
                GFX_drawText([100, 120], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "LEVEL %d", currentLevel);
                GFX_drawText([140, 120], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "PASSED");
                //GFX_drawText([180, 120], 6, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "Coins %d out of %d", CoinsNow, CoinsAtLevel);
            }
            case 2:
            {
                angle = 180;
                GFX_drawText([120, 140], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "LEVEL %d", currentLevel);
                GFX_drawText([120, 100], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "PASSED");
                //GFX_drawText([120, 60], 6, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "Coins %d out of %d", CoinsNow, CoinsAtLevel);
            }
            case 3:
            {
                angle = 90;
                GFX_drawText([140, 120], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "LEVEL %d", currentLevel);
                GFX_drawText([100, 120], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "PASSED");
                //GFX_drawText([60, 120], 6, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "Coins %d out of %d", CoinsNow, CoinsAtLevel);
            }

        }

        GFX_render();
    }


}

GameOverScreen()
{
    //printf("Drawing GameOver screen \n");
    new FaceletW[TOPOLOGY_FACELET];
    new PlaceW[TOPOLOGY_PLACE];

    FaceletW.module = SELF_ID;

    for(new screenNumber = 0; screenNumber<3; screenNumber++)
    {
        GFX_setRenderTarget(screenNumber);
        GFX_clear(0xFF000000);

        FaceletW.screen = screenNumber;
        PlaceW = TOPOLOGY_getPlace(FaceletW);


        new angle = 0;
        switch(PlaceW.position)
        {
            case 0:
            {
                angle = 0;
                GFX_drawText([120, 100], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "GAME");
                GFX_drawText([120, 140], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "OVER");
            }
            case 1:
            {
                angle = 270;
                GFX_drawText([100, 120], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "GAME");
                GFX_drawText([140, 120], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "OVER");
            }
            case 2:
            {
                angle = 180;
                GFX_drawText([120, 100], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "GAME");
                GFX_drawText([120, 140], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "OVER");
            }
            case 3:
            {
                angle = 90;
                GFX_drawText([100, 120], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "GAME");
                GFX_drawText([140, 120], 14, angle, 0, TEXT_ALIGN_CENTER, 0xFFFFFFFF, "OVER")
            }

        }

        GFX_render();
    }


}