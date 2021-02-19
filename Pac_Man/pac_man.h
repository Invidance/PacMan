#ifndef PAC_MAN_H
#define PAC_MAN_H

#include <math.h>
#include <string>
#include <iostream>
#include <hge.h>
#include <hgesprite.h>
#include <hgegui.h>
#include <hgefont.h>
#include <vector>

#include "player.h"
#include "enemy.h"

HGE *hge=0;

enum Direction{
	NO_WAY,
	LEFT,
	UP,
	RIGHT,
	DOWN,	
};


Status status_game = MAIN_MENU;

hgeSprite *enemy1_spr;
hgeSprite *enemy2_spr;
hgeSprite *enemy3_spr;
hgeSprite *empty_enemy_spr;

hgeSprite *cursor;
hgeSprite *ground;
hgeSprite *coin;
hgeSprite *big_coin;

hgeFont *font;
hgeGUI *gui;
hgeGUI *gui_win;

hgeQuad	quad;
HEFFECT sound;
HEFFECT eat_eff;
HEFFECT kill;

Player pac_man;
Enemy enemy1;
Enemy enemy2;
Enemy enemy3;

HTEXTURE	tex;
HTEXTURE	tex_second;

const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 10;
const int SIZE_EL = 32;
const int SCREEN_WIDTH = MAP_WIDTH*SIZE_EL;
const int SCREEN_HEIGHT = MAP_HEIGHT*SIZE_EL;

bool is_ate[MAP_HEIGHT][MAP_WIDTH];

float x=0, y=0;
float delay = 0;

float enemy1_x=0, enemy1_y=0;
float enemy2_x=0, enemy2_y=0;
float enemy3_x=0, enemy3_y=0;

int all_coin = 0;

std::string map[MAP_HEIGHT] = {
	"##### ##############",
	"#c         e       #",
	"# ### ### ### #### #",
	"# #           #c   #",
	"# # # ## ## # t ## #",
	"    # ##c # # # #   ",
	"# #r   ## # # # #  #",
	"# ####  # # # # #  #",
	"#        s        c#",
	"##### ##############"};

std::string level_2[MAP_HEIGHT] = {
	"########## #########",
	"#                 c#",
	"# ### # ## ##  ### #",
	"# # e # #c  #   r# #",
	"# #  #    #   ## # #",
	"     #  # t #  #    ",
	"# #c    ## ###  c# #",
	"# ### #  # #   ### #",
	"#     #   s        #",
	"########## #########"};

void CreateMap();
bool KillPlayer(Player &player, Enemy &enemy, Status &status_game);
void UpdateEnemySpeed(Enemy &enemy);
void ChangeMap(std::string map[], std::string level[], int width, int height);
int AllCoin(std::string map[],int width, int height);


#endif