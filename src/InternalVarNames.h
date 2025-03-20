#pragma once

///definition of the window variable name
#define WINDOW_NAME "&window"
///definition of the event var name
#define EVENT_NAME "&events"
///define the name of the renderer
#define RND_NAME "&rnd"
///define the backtexture
#define TEXTURE_NAME "&text"
///define the size of the window
///x:
#define WINDOW_X "&window_x"
///y:
#define WINDOW_Y "&window_y"

///As of definition of the language
#define POS_X "@x"
#define POS_Y "@y"
#define MAX_X "@max_x"
#define MAX_Y "@max_y"
#define ROTATION "@dir"
#define COLOR_R "@red"
#define COLOR_G "@green"
#define COLOR_B "@blue"

///Debuging
#define DEBUG_SURFACE "&surface"


//also define functionNames
///define the compiler internal name for the main function
#define MAIN_FUNC "\%TurtelMain"
///define the Turtelmain function name
#define TURTEL_MAIN_FUNC_NAME ENV_FUNC_PREFIX "TurtelMain"
///defines the prefix of all user functions
#define USR_FUNC_PREFIX "__usr__func_"
///defines the prefix of all env functions
#define ENV_FUNC_PREFIX "__env__func_"
