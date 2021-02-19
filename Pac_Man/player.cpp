#include "player.h"

Player::Player(){
	hge = 0;
}

void Player::PlayerInit(HTEXTURE tex, float posX, float posY,
			   hgeFont *font, HEFFECT sound, 
			   int SCREEN_WIDTH, int SCREEN_HEIGHT, int SIZE_EL, float speed,HGE *hge){

	sprite = new hgeSprite(tex,0, 96, SIZE_EL, SIZE_EL);
	sprite->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
	sprite->SetHotSpot(16,16);

	animat = new hgeAnimation(tex,3,14,0,96,32,32);
	animat->SetHotSpot(16,16);
	animat->SetMode(HGEANIM_LOOP);


    this->hge = hge;
	this->posX = posX;
	this->posY = posY;
	start_posX = posX;
	start_posY = posY;
	ate_coin = 0;

	this->font = font;
	
	sound_eat = sound;

	this->SCREEN_HEIGHT = SCREEN_HEIGHT;
	this->SCREEN_WIDTH = SCREEN_WIDTH;
	size = SIZE_EL;
	this->speed = speed;
	score = 0;
	life = 3;
}

void Player::RenderSprite(int rotate){
	if(!animat->IsPlaying()){
		animat->Play();
	}
	animat->RenderEx(posX,posY,(rotate*6.28f)/360.0f,1,0);	
}

void Player::UpdateSprite(float dt){
	animat->Update(dt);
}

void Player::move(float dt, std::string map[]){
	Teleport();
	Colision(map);
	CheckWay(map);
	if(way == LEFT){
		dx = -dt*speed;
		posX += dx;		
	}
	else if(way == RIGHT){
		dx = dt*speed;
		posX += dx;		
	}
	else if(way == UP){
		dy = -dt*speed;
		posY += dy;		
	}
	else if(way == DOWN){
		dy = dt*speed;
		posY += dy;
	}else{
		way = last_way;
	}
	last_way = way;
}

void Player::Teleport(){
	
	if(way == LEFT && (posX-16) < 0){
		posX = SCREEN_WIDTH - size+16;
	}
	if(way == RIGHT && (posX+16) > SCREEN_WIDTH){
		posX = 16.0f;		
	}
	if(way == UP && (posY-16) < 0){
		posY = SCREEN_HEIGHT - size+16;		
	}
	if(way == DOWN && (posY+16) > SCREEN_HEIGHT){
		posY = 16.0f;
	}

}

void Player::Colision(std::string map[]){
	for(int i = (posY-16)/size; i<((posY-16)+size)/size;i++){
		for(int j = (posX-16)/size; j<((posX-16)+size)/size;j++){
			if(map[i][j] == '#'){
				if(way == LEFT){ 
					
					posX = j*size+16+size;
				}
				if(way == RIGHT) {
					
					posX = j*size+16-size;
				}
			}
		}
	}
	for(int i = (posY-16)/size; i<((posY-16)+size)/size;i++){
		for(int j = (posX-16)/size; j<((posX-16)+size)/size;j++){
			if(map[i][j] == '#'){
				if(way == UP){ 
					
					posY = i*size+16+size;
				}
				if(way == DOWN) {
					
					posY = i*size+16-size;
				}
			}
		}
	}
}

bool Player::CorrectX(int xl){
		if(fabs((float)(xl*size)-(posX-16)) < 0.08){
			posX = xl * size+16;
			return true;
		}
		return false;
	}
bool Player::CorrectY(int yt){
	if(fabs((float)(yt*size) - (posY-16)) < 0.08){
		posY = yt*size+16;
		return true;
	}
	return false;
}

void Player::CheckWay(std::string map[]){
	int xl = posX/size;
	int yt = posY/size;
	
	if((yt >0 && yt< SCREEN_HEIGHT/size) && (xl >0 && xl< SCREEN_WIDTH/size)){
		

	if(key_pressed == KEY_LEFT){

		if(CorrectX(xl) && CorrectY(yt)){	
			
			if(map[yt][xl-1] != '#'){
				last_way = way;
				way = LEFT;
				key_pressed = NONE;
			}else{way = last_way;}
		}
	}
			
	if(key_pressed == KEY_DOWN){

		if(CorrectX(xl) && CorrectY(yt)){
			if(map[yt+1][xl] != '#'){
				last_way = way;
				way = DOWN;
				key_pressed = NONE;
			}else{way = last_way;}
		}
	}
			
	if(key_pressed == KEY_UP){
		if(CorrectX(xl) && CorrectY(yt)){
			if(map[yt-1][xl] != '#'){
				last_way = way;
				way = UP;
				key_pressed = NONE;
			}else{way = last_way;}
		}
	}
			
	if(key_pressed == KEY_RIGHT){
		if(CorrectX(xl) && CorrectY(yt)){
			if(map[yt][xl+1] != '#'){
				last_way = way;
				way = RIGHT;
				key_pressed = NONE;
			}else{way = last_way;}
		}
	}
	}
}

void Player::EatCoin(bool is_ate[10][20],int all_coin, std::string map[], Status &status_game){
	int pos_x = posX/size;
	int pos_y = posY/size;
	if((map[pos_y][pos_x] != '#' && map[pos_y][pos_x] != 's' && map[pos_y][pos_x] != 'c') && !is_ate[pos_y][pos_x]){
		is_ate[pos_y][pos_x] = true;
		hge->Effect_Play(sound_eat);
		ate_coin++;
		score +=10;
	}
	if(map[pos_y][pos_x] == 'c' && !is_ate[pos_y][pos_x]){
		is_ate[pos_y][pos_x] = true;
		hge->Effect_Play(sound_eat);
		ate_coin++;
		score += 50;
		SetBigCoin(true);
	}
	if(ate_coin >= all_coin){
		status_game = WIN;
	}	
}


void Player::SetBigCoin(bool ate){
	ate_big_coin = ate;
}
bool Player::GetBigCoin(){
	return ate_big_coin;
}


void Player::set_direction(KeyDir key){
	key_pressed = key;
}


void Player::SetPos(float x, float y){
	posX = x;
	posY = y;
}

float Player::getX(){
	return posX;
}

float Player::getY(){
	return posY;
}

Player::Direction Player::get_direction(){return way;}

void Player::NewGame(bool is_ate[10][20],int height,int width,float posX,float posY){
	ate_coin = 0;
	life = 3;
	for(int i = 0;i<height;i++){
		for(int j = 0;j<width;j++){
			is_ate[i][j] = false;
		}
	}
	SetPos(posX,posY);
	score = 0;
}

void Player::UpdateScore(int score){
	this->score += score;
}

int Player::GetLife(){return life;}
int Player::GetCoin(){return ate_coin;}
int Player::GetScore(){return score;}

void Player::Dead(Status &status_game){
	
	life--;
	if(life<=0){
		status_game = LOSE;
	}
	
}

void Player::StartPosition(){
	posX = start_posX;
	posY = start_posY;
}
