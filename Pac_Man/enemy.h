#pragma once

#include <hge.h>
#include <math.h>
#include <string>
#include <hgesprite.h>

#include "player.h"

class Enemy{
public:
	Enemy();
	void EnemyInit(hgeSprite *player,hgeSprite *empty_player,float posX, float posY, HEFFECT sound
		,int SCREEN_WIDTH,int SCREEN_HEIGHT,int SIZE_EL,float speed,HGE *hge);


enum Direction{
	LEFT,
	UP,
	RIGHT,
	DOWN,	
	NO_WAY,
};

enum Dot{
	WALL,
	COIN,
	BIG_COIN,
	EMPTY,
};

struct DotMap{
	float x;
	float y;

	Dot role;
	Direction dir;

	int weight;
	int distance;
	int way_num;
	
};

	bool set_direction(std::string map[]);

	void RenderSprite();
	void SetPos(float x, float y);
	void SetStartPos();
	float getX();
	float getY();

	void move(float dt,std::string map[], float end_x, float end_y);
	void Teleport();
	void Colision(std::string map[]);
	void CheckWay();
	void KillPlayer();

	bool CorrectX(int xl);
	bool CorrectY(int yt);

	void NewGame(float posX,float posY);
	void NewGame(std::string map[],float posX,float posY);
	void SetSpeed(float speed);

	void ShortWay(float end_x, float end_y, std::string map[]);
	int CalculateWeight(float start_x, float start_y, float end_x, float end_y);

	void SetDeadMode();
	bool GetIsDead();
	void NullDeadDelay();
private:	
	HEFFECT sound_kill;
	HGE *hge;

	hgeSprite *sprite;
	hgeSprite *empty_sprite;
	
	float speed;
	int size;
	int SCREEN_WIDTH,SCREEN_HEIGHT;

	float posX,posY;
	float start_posX,start_posY;
	float last_posX,last_posY;
	float dx,dy;

	Direction way;
	Direction last_way;

	float delay;
	float dead_delay;
	bool is_dead;
};