#ifndef PAC_MAN_H
#define PAC_MAN_H

#include <math.h>
#include <string>
#include <iostream>
#include <hge.h>
#include <hgesprite.h>
#include <hgegui.h>
#include <hgefont.h>
#include "menu.h"

HGE *hge=0;

hgeSprite *player;
hgeSprite *cursor;
hgeSprite *ground;
hgeSprite *coin;

hgeFont *font;
hgeGUI *gui;
hgeQuad	quad;
HEFFECT sound;

HTEXTURE	tex;
HTEXTURE	tex_second;

const int MAP_WIDTH = 0;
const int MAP_HEIGHT = 0;
const int SIZE_EL = 32;
const int SCREEN_WIDTH = MAP_WIDTH*SIZE_EL;
const int SCREEN_HEIGHT = MAP_HEIGHT*SIZE_EL;

const float speed = 96.0f;

bool is_ate[MAP_HEIGHT][MAP_WIDTH];

float x=64.0f+16.0f, y=64.0f+16.0f;
float dx=0.0f, dy=0.0f;

int xl,xr;
int yt,yb;

bool is_up = false;
bool is_down = false;
bool is_left = false;
bool is_right = false;
bool in_game = false;

bool key_up_pressed = false;
bool key_down_pressed = false;
bool key_left_pressed = false;
bool key_right_pressed = false;

void move(float dt);
void Colision();
void Teleport();
void CreateMap();
void CheckWay();
void EatCoin();
std::string LoadMap(std::string filename,int &width,int &height);

bool CorrectX(int xl);
bool CorrectY(int yt);

#endif