#include "JobSetup.h"
#include "includes.h"

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
        {ICON_HOME, LABEL_HOME},
        {ICON_JOBSETUPX, LABEL_HOME_X},
        {ICON_JOBSETUPX, LABEL_HOME_Y},
        {ICON_JOBSETUPZ, LABEL_HOME_Z},
        {ICON_JOBSETUPALL, LABEL_HOME_ALL},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACK, LABEL_BACK},
    }};

void menuJobSetup(void)
{
    KEY_VALUES key_num = KEY_IDLE;

    menuDrawPage(&jobSetupItems);
    drawXYZ();

    while (infoMenu.menu[infoMenu.cur] == menuJobSetup)
    {
        key_num = menuKeyGetValue();
        switch (key_num)
        {
        case KEY_ICON_0:
            storeCmd("G28\n");
            break;
        case KEY_ICON_1:
            jobSetupCmd(X_AXIS);
            break;
        case KEY_ICON_2:
            jobSetupCmd(Y_AXIS);
            break;
        case KEY_ICON_3:
            jobSetupCmd(Z_AXIS);
            break;
        case KEY_ICON_5:
            jobSetupCmd(TOTAL_AXIS);
            break;
        case KEY_ICON_7:
            infoMenu.cur--;
            break;

        default:
            break;
        }
        loopProcess();
        update_gantry();
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