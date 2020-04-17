#include "includes.h"
#include "Rotate.h"

const MENUITEMS rotateItems = {
    //   title
    LABEL_ROTATE,
    // icon                       label
    {
        {ICON_MOVE150, LABEL_BACKGROUND},
        {ICON_MOVE300, LABEL_BACKGROUND},
        {ICON_MOVE450, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_MOVEY150, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACK, LABEL_BACK},
    }};

void menuRotate(void)
{
    KEY_VALUES key_num = KEY_IDLE;
    menuDrawPage(&rotateItems);
    drawXYZ();

    while (infoMenu.menu[infoMenu.cur] == menuRotate)
    {
        key_num = menuKeyGetValue();
        switch (key_num)
        {
        case KEY_ICON_0:
            storeCmd("G1 X150\n");
            break;
        case KEY_ICON_1:
            storeCmd("G1 X300\n");
            break;
        case KEY_ICON_2:
            storeCmd("G1 X450\n");
            break;
        case KEY_ICON_4:
            storeCmd("G1 Y150\n");
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
