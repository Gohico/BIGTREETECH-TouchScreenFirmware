#include "JobSetup.h"
#include "includes.h"

KEY_VALUES key_num = KEY_IDLE;

#define X_HOME_GCODE "G92 X0\n"
#define Y_HOME_GCODE "G92 Y0\n"
#define Z_HOME_GCODE "G92 Z0\n"
#define ALL_HOME_GCODE "G92 X0 Y0 Z0\n"

//1 title, ITEM_PER_PAGE items(icon+label)
const MENUITEMS jobSetupItems = {
    //   title
    LABEL_JOBSETUP,
    // icon                       label
    {
        {ICON_JOBSETUPX, LABEL_HOME_X},
        {ICON_JOBSETUPX, LABEL_HOME_Y},
        {ICON_JOBSETUPZ, LABEL_HOME_Z},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_JOBSETUPALL, LABEL_HOME_ALL},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACK, LABEL_BACK},
    }};

void menuJobSetup(void)
{
    menuDrawPage(&jobSetupItems);

    while (infoMenu.menu[infoMenu.cur] == menuJobSetup)
    {
        key_num = menuKeyGetValue();
        switch (key_num)
        {
        case KEY_ICON_0:
            jobSetupCmd(X_AXIS);
            break;
        case KEY_ICON_1:
            jobSetupCmd(Y_AXIS);
            break;
        case KEY_ICON_2:
            jobSetupCmd(Z_AXIS);
            break;

        case KEY_ICON_4:
            jobSetupCmd(TOTAL_AXIS);
            break;

        case KEY_ICON_7:
            infoMenu.cur--;
            break;

        default:
            break;
        }
        loopProcess();
    }
}

void jobSetupCmd(AXIS xyz)
{
    switch (xyz)
    {
    case X_AXIS:
        storeCmd(X_HOME_GCODE);
        break;
    case Y_AXIS:
        storeCmd(Y_HOME_GCODE);
        break;
    case Z_AXIS:
        storeCmd(Z_HOME_GCODE);
        break;
    case TOTAL_AXIS:
    default:
        storeCmd(ALL_HOME_GCODE);
        break;
    }
}