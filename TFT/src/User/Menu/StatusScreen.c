#include "StatusScreen.h"
#include "includes.h"
#include "GUI.h"
//1 title, ITEM_PER_PAGE items (icon + label)
const MENUITEMS StatusItems = {
    // title
    LABEL_READY,
    // icon                       label
    {
        {ICON_MOVE, LABEL_BACKGROUND}, //was ICON_STATUSNOZZLE ICON_HOME
        {ICON_JOBSETUP, LABEL_BACKGROUND},
        {ICON_ROTATE, LABEL_BACKGROUND},
        {ICON_CNC, LABEL_BACKGROUND}, // was ICON_STATUS_SPEED ICON_MOVE
        {ICON_MAINMENU, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_STOP, LABEL_BACKGROUND},
    }};

// const ITEM ToolItems[3] = {
//     // icon                       label
//     {ICON_STATUSNOZZLE, LABEL_BACKGROUND},
//     {ICON_STATUSJOBSETUP, LABEL_BACKGROUND},
//     {ICON_STATUSFAN, LABEL_BACKGROUND},
// };
// const ITEM SpeedItems[2] = {
//     // icon                       label
//     {ICON_MOVE, LABEL_BACKGROUND}, // ICON_STATUS_SPEED
//     {ICON_STATUS_FLOW, LABEL_BACKGROUND},
// };

static u32 nextTime = 0;
static u32 update_time = 2000; // 1 seconds is 1000
SCROLL     msgScroll;
static int lastConnection_status = -1;
static bool msgNeedRefresh = false;

static char msgtitle[20];
static char msgbody[MAX_MSG_LENGTH];

uint8_t current_tool = NOZZLE0;
int current_fan = 0;
int current_speedID = 0;
const char* SpeedID[2] = SPEED_ID;
// text position rectangles for Live icons
//icon 0
const GUI_POINT ss_title_point = {ICON_WIDTH - BYTE_WIDTH/2, SSICON_NAME_Y0};
const GUI_POINT ss_val_point   = {ICON_WIDTH/2, SSICON_VAL_Y0};

//info rectangle
const GUI_RECT RectInfo = {START_X + 1 * ICON_WIDTH + 1 * SPACE_X,  ICON_START_Y +  1 * ICON_HEIGHT + 1 * SPACE_Y,
                           START_X + 3 * ICON_WIDTH + 2 * SPACE_X,  ICON_START_Y +  2 * ICON_HEIGHT + 1 * SPACE_Y};

const  GUI_RECT msgRect ={START_X + 1 * ICON_WIDTH + 1 * SPACE_X + 2,   ICON_START_Y +  1 * ICON_HEIGHT + 1 * SPACE_Y + STATUS_MSG_BODY_YOFFSET,
                          START_X + 3 * ICON_WIDTH + 2 * SPACE_X - 2,   ICON_START_Y +  2 * ICON_HEIGHT + 1 * SPACE_Y - STATUS_MSG_BODY_BOTTOM};

const GUI_RECT RecGantry = {START_X,                        1*ICON_HEIGHT+0*SPACE_Y+ICON_START_Y + STATUS_GANTRY_YOFFSET,
                            4*ICON_WIDTH+3*SPACE_X+START_X, 1*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y - STATUS_GANTRY_YOFFSET};


/*set status icons */
/* void set_status_icon(void)
{
  StatusItems.items[0] = ToolItems[0];
  StatusItems.items[1] = ToolItems[1];
  StatusItems.items[2] = ToolItems[2];
  StatusItems.items[3] = SpeedItems[0];

} */

// void drawTemperature(void)
// {
//   //icons and their values are updated one by one to reduce flicker/clipping

//   char tempstr[100];
//   GUI_SetTextMode(GUI_TEXTMODE_TRANS);
//   GUI_SetColor(HEADING_COLOR);
//   menuDrawIconOnly(&ToolItems[0], 0);                                                //Ext icon
//   GUI_DispStringRight(pointID[0].x, pointID[0].y, (u8 *)heatDisplayID[current_Ext]); //Ext label

//   // GUI_SetColor(VAL_COLOR);
//   // my_sprintf(tempstr, "%d/%d", heatGetCurrentTemp(current_Ext), heatGetTargetTemp(current_Ext));
//   // GUI_DispStringInPrect(&rectB[0], (u8 *)tempstr); //Ext value

//   GUI_SetColor(HEADING_COLOR);
//   menuDrawIconOnly(&ToolItems[1], 1);                                        //Bed icon
//   GUI_DispStringRight(pointID[1].x, pointID[1].y, (u8 *)heatDisplayID[POS]); //Bed label
//   GUI_SetColor(VAL_COLOR);
//   my_sprintf(tempstr, "%d/%d", xaxis, yaxis);
//   GUI_DispStringInPrect(&rectB[1], (u8 *)tempstr); //Bed value

//   GUI_SetColor(HEADING_COLOR);
//   menuDrawIconOnly(&ToolItems[2], 2);                                        //Fan icon
//   GUI_DispStringRight(pointID[2].x, pointID[2].y, (u8 *)fanID[current_fan]); //Fan label
//   GUI_SetColor(VAL_COLOR);

//   u8 fs;
// #ifdef SHOW_FAN_PERCENTAGE
//   fs = (fanGetSpeed(current_fan) * 100) / 255;
//   my_sprintf(tempstr, "%d%%", fs);
// #else
//   fs = fanSpeed[current_fan];
//   my_sprintf(tempstr, "%d", fs);
// #endif
//   GUI_DispStringInPrect(&rectB[2], (u8 *)tempstr); //Fan value

//   /*
// Text updates for ICON_STATUS_SPEED
// */
//   // GUI_SetColor(HEADING_COLOR);
//   // menuDrawIconOnly(&SpeedItems[current_speedID], 3);                               //Speed / flow icon
//   // GUI_DispStringRight(pointID[3].x, pointID[3].y, (u8 *)SpeedID[current_speedID]); //Speed / flow label
//   // GUI_SetColor(VAL_COLOR);
//   // my_sprintf(tempstr, "%d%s", speedGetPercent(current_speedID), "%");
//   // GUI_DispStringInPrect(&rectB[3], (u8 *)tempstr); //Speed / Flow value

//   GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
//   GUI_SetColor(GANTRYLBL_COLOR);
//   GUI_SetBkColor(GANTRYLBL_BKCOLOR);
//   my_sprintf(tempstr, "   X: %.2f   Y: %.2f   Z: %.2f   ", xaxis, yaxis, zaxis);
//   GUI_DispStringInPrect(&RecGantry, (u8 *)tempstr);

//   GUI_RestoreColorDefault();
// }

void updateGantry()
{
  char tempstr[100];
  GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
  GUI_SetColor(GANTRYLBL_COLOR);
  GUI_SetBkColor(infoSettings.status_xyz_bg_color);
  sprintf(tempstr, "   X: %.2f   Y: %.2f   Z: %.2f   ", coordinateGetAxisActual(X_AXIS), coordinateGetAxisActual(Y_AXIS), coordinateGetAxisActual(Z_AXIS));
  GUI_DispStringInPrect(&RecGantry,(u8 *)tempstr);

  GUI_RestoreColorDefault();
}

void statusScreen_setMsg(const uint8_t *title, const uint8_t *msg)
{
  strncpy(msgtitle, (char *)title, sizeof(msgtitle));
  strncpy(msgbody, (char *)msg, sizeof(msgbody));
  msgNeedRefresh = true;
}

void statusScreen_setReady(void)
{
  strncpy(msgtitle, (char *)textSelect(LABEL_STATUS), sizeof(msgtitle));
  if(infoHost.connected == false){
    strncpy(msgbody, (char *)textSelect(LABEL_UNCONNECTED), sizeof(msgbody));
  }
  else{
    strncpy(msgbody, (char *)machine_type, sizeof(msgbody));
    strcat(msgbody, " ");
    strcat(msgbody, (char *)textSelect(LABEL_READY));
  }

  msgNeedRefresh = true;
}

void drawStatusScreenMsg(void)
{
//GUI_ClearRect(RectInfo.x0,RectInfo.y0,RectInfo.x1,RectInfo.y1);
  GUI_SetTextMode(GUI_TEXTMODE_TRANS);

  ICON_CustomReadDisplay(RectInfo.x0,RectInfo.y0,INFOBOX_WIDTH,INFOBOX_HEIGHT,INFOBOX_ADDR);
  GUI_SetColor(INFOMSG_BKCOLOR);
  GUI_DispString(RectInfo.x0 + STATUS_MSG_ICON_XOFFSET, RectInfo.y0 + STATUS_MSG_ICON_YOFFSET,IconCharSelect(ICONCHAR_INFO));

  GUI_DispString(RectInfo.x0 + BYTE_HEIGHT+ STATUS_MSG_TITLE_XOFFSET,RectInfo.y0 + STATUS_MSG_ICON_YOFFSET,(u8*)msgtitle);
  GUI_SetBkColor(INFOMSG_BKCOLOR);
  GUI_FillPrect(&msgRect);

  Scroll_CreatePara(&msgScroll, (u8 *)msgbody, &msgRect);

  GUI_RestoreColorDefault();

  msgNeedRefresh = false;
}

void scrollMsg(void){
  GUI_SetBkColor(INFOMSG_BKCOLOR);
  GUI_SetColor(INFOMSG_COLOR);
  Scroll_DispString(&msgScroll,CENTER);
  GUI_RestoreColorDefault();
}

void toggleTool(void)
{
  if (OS_GetTimeMs() > nextTime)
  {
    if (infoSettings.hotend_count > 1)
    {
      current_tool = (current_tool+1) % infoSettings.hotend_count;
    }
    if (infoSettings.fan_count > 1)
    {
      current_fan = (current_fan + 1) % infoSettings.fan_count;
    }
    current_speedID = (current_speedID + 1) % 2;
    nextTime = OS_GetTimeMs() + update_time;
    //drawTemperature();

    coordinateQuery();
    speedQuery();
  }
}

void menuStatus(void)
{
  storeCmd("M106 S100\n"); //Turn fan on

  KEY_VALUES key_num = KEY_IDLE;
  GUI_SetBkColor(infoSettings.bg_color);
  //set_status_icon();
  menuDrawPage(&StatusItems);
  GUI_SetColor(infoSettings.status_xyz_bg_color);
      //GUI_ClearPrect(&RecGantry);
  GUI_FillPrect(&RecGantry);
  //drawTemperature();
  updateGantry();
  drawStatusScreenMsg();

  while (infoMenu.menu[infoMenu.cur] == menuStatus)
  {
    if(infoHost.connected != lastConnection_status){
      statusScreen_setReady();
      lastConnection_status = infoHost.connected;
    }
    if (msgNeedRefresh) {
      drawStatusScreenMsg();
    }
    scrollMsg();
    key_num = menuKeyGetValue();
    switch (key_num)
    {
    case KEY_ICON_0:
      infoMenu.menu[++infoMenu.cur] = menuMove;
      break;
    case KEY_ICON_1:
      infoMenu.menu[++infoMenu.cur] = menuJobSetup;
      break;
    case KEY_ICON_2:
      infoMenu.menu[++infoMenu.cur] = menuRotate;
      break;
    case KEY_ICON_3:
      infoMenu.menu[++infoMenu.cur] = menuPrint; //was menuMove
      break;
    case KEY_ICON_4:
      infoMenu.menu[++infoMenu.cur] = unifiedMenu;
      break;
    case KEY_ICON_7:
      storeCmd("M112 \n"); //Emergency stop
      //infoMenu.menu[++infoMenu.cur] = menuPrint;
      break;

      default:break;
    }
    toggleTool();
    loopProcess();
  }
}
