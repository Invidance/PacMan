#pragma once

#include <hge.h>
#include <string>
#include <hgeSprite.h>
#include <hgeanim.h>
#include <hgeFont.h>
#include <math.h>

static enum Status{
	IN_GAME,
	MAIN_MENU,
	WIN,
	LOSE,
};


class Player{
public:
	Player();
	void PlayerInit(HTEXTURE tex,float posX, float posY, hgeFont *font, HEFFECT sound
		,int SCREEN_WIDTH,int SCREEN_HEIGHT,int SIZE_EL,float speed,HGE *hge);

	enum KeyDir {
		NONE,
		KEY_LEFT,
		KEY_UP,
		KEY_RIGHT,
		KEY_DOWN,
	};

	enum Direction{
		LEFT,
		UP,
		RIGHT,
		DOWN,	
		NO_WAY,
	};

	void set_direction(KeyDir key);

	void RenderSprite(int rotate);
	void SetPos(float x, float y);
	float getX();
	float getY();

	void move(float dt,std::string map[]);
	void Teleport();
	void Colision(std::string map[]);
	void CheckWay(std::string map[]);
	void EatCoin(bool is_ate[10][20],int all_coin,std::string map[],Status &status_game);

	Direction get_direction();

	bool CorrectX(int xl);
	bool CorrectY(int yt);

	void NewGame(bool is_ate[10][20],int height,int width,float posX,float posY);
	void StartPosition();

	int GetLife();
	int GetCoin();
	int GetScore();
	
	void Dead(Status &status_game);

	void SetBigCoin(bool ate);
	bool GetBigCoin();
	void UpdateScore(int score);

	void Player::UpdateSprite(float dt);
private:	
	HEFFECT sound_eat;
	HGE *hge;

	HTEXTURE tex;

	hgeSprite *sprite;
	hgeAnimation *animat;
	hgeFont *font;

	float speed;
	int size;
	
	int SCREEN_WIDTH,SCREEN_HEIGHT;

	float posX,posY;
	float start_posX,start_posY;
	float dx,dy;
	int ate_coin;
	int life;
	int score;

	bool ate_big_coin;

	Direction way;
	Direction last_way;
	KeyDir key_pressed;
};