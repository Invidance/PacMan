#include "enemy.h"

Enemy::Enemy(){
	hge = 0;
}

void Enemy::EnemyInit(hgeSprite *player,hgeSprite *empty_player, float posX, float posY,
			   HEFFECT sound, int SCREEN_WIDTH, int SCREEN_HEIGHT, int SIZE_EL, float speed,HGE *hge){
    this->hge = hge;
	this->posX = posX;
	this->posY = posY;

	start_posX = posX;
	start_posY = posY;

	sprite = player;
	empty_sprite = empty_player;
	sound_kill = sound;

	this->SCREEN_HEIGHT = SCREEN_HEIGHT;
	this->SCREEN_WIDTH = SCREEN_WIDTH;
	size = SIZE_EL;
	this->speed = speed;
	way = LEFT;
	last_way = RIGHT;
	delay = 0;
	dead_delay = 0;
}

void Enemy::RenderSprite(){
	if(!is_dead){
		sprite->RenderEx(posX,posY,0,1,0);
	}else{
		empty_sprite->RenderEx(posX,posY,0,1,0);
	}
}

void Enemy::NullDeadDelay(){
	dead_delay = 0;
}

void Enemy::move(float dt, std::string map[], float end_x, float end_y){
	Teleport();
	bool check;
	int xl = posX/size;
	int yt = posY/size;
	if(is_dead){
		speed = 64.0f;
		dead_delay += dt;
	}
	delay += dt;
	if(delay >=0.02){
		if(posX > 32 && posX < SCREEN_WIDTH - 32){
			if(posY > 32 && posY < SCREEN_HEIGHT - 32){
				if(abs((yt*size) - (posY-16)) < 1){
					if(abs((xl*size)-(posX-16)) < 1){
						int count = 0;
						do{
							posX = xl * size+16;
							posY = yt*size+16;
							if(count == 0 && !is_dead){
								ShortWay(end_x,end_y,map);
								count++;
							}else{
								CheckWay();
							}
							check = set_direction(map);
						}while(!check);
						count = 0;
						delay = 0;
					}
				}
			}
		}
		
	}
	
	if(is_dead && dead_delay >= 5){
		is_dead = false;
		dead_delay = 0;
	}

	Colision(map);
	
	if(way == LEFT){
		dx = -dt*speed;
		posX += dx;		
	}
	if(way == RIGHT){
		dx = dt*speed;
		posX += dx;		
	}
	if(way == UP){
		dy = -dt*speed;
		posY += dy;		
	}
	if(way == DOWN){
		dy = dt*speed;
		posY += dy;
	}
	
	last_way = way;
}

void Enemy::Teleport(){
	
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

void Enemy::Colision(std::string map[]){
	
	for(int i = (posY-16)/size; i<((posY-16)+size)/size;i++){
		for(int j = (posX-16)/size; j<((posX-16)+size)/size;j++){
			if(map[i][j] == '#'){
				if(way == LEFT){ 
					
					posX = j*size+16+size;
					CheckWay();
					break;
					

				}
				if(way == RIGHT) {
					
					posX = j*size+16-size;
					CheckWay();
					break;
					

				}
			}
			
		}
	}
	for(int i = (posY-16)/size; i<((posY-16)+size)/size;i++){
		for(int j = (posX-16)/size; j<((posX-16)+size)/size;j++){
			if(map[i][j] == '#'){
				if(way == UP){ 
					
					posY = i*size+16+size;
					CheckWay();
					break;
					

				}
				if(way == DOWN) {
					
					posY = i*size+16-size;
					CheckWay();
					break;
					
				}
			}
			
		}
	}
	
}

bool Enemy::CorrectX(int xl){
		if((float)(xl)-(posX-16) < 0.03){
			posX = xl+16;
			return true;
		}
		return false;
	}
bool Enemy::CorrectY(int yt){
	if((float)(yt) - (posY-16) < 0.03){
		posY = yt+16;
		return true;
	}
	return false;
}

void Enemy::CheckWay(){
	
	do{
	int change = rand()%3;
	
	switch(way){
		case LEFT:
			
			if(change == 0){
				way = DOWN;
				break;
			}else if(change == 1){
				way = UP;
				break;
			}else if(change == 2){
				way = LEFT;
				break;
			}
			break;
		case RIGHT:
			
			if(change == 0){
				way = DOWN;
				
				break;
			}else if(change == 1){
				way = UP;
				
				break;
			}else if(change == 2){
				way = RIGHT;
				
				break;
			}
			break;
		case UP:
			
			if(change == 0){
				way = RIGHT;
				
				break;
			}else if(change == 1){
				
				way = UP;
				
				break;
			}else if(change == 2){
				way = LEFT;
				
				break;
			}
			break;
		case DOWN:
			
			if(change == 0){
				way = DOWN;
				break;
			}else if(change == 1){
				way = RIGHT;
				
				break;
			}else if(change == 2){
				way = LEFT;
				break;
			}
			break;
	}
	}while(abs(last_way - way) == 2);
}

void Enemy::KillPlayer(){
	
	hge->Effect_Play(sound_kill);
	
}

void Enemy::ShortWay(float end_x, float end_y, std::string map[]){
	
	int i,j;

	float start_x = posX;

	float start_y = posY;
	
	DotMap left;
	left.x = start_x - size;
	left.y = start_y;

	j = left.x/size;
	i = left.y/size;
	if(map[i][j] == '#'){
		left.role = WALL;
	}else if(map[i][j] == 'c'){
		left.role = BIG_COIN;
	}else{
		left.role = COIN;
	}

	if(left.role != WALL){
		left.weight = CalculateWeight(left.x, left.y, end_x, end_y);
	}else{
		left.weight = 1000;
	}

	DotMap right;
	right.x = start_x + size;
	right.y = start_y;

	j = right.x/size;
	i = right.y/size;
	if(map[i][j] == '#'){
		right.role = WALL;
	}else if(map[i][j] == 'c'){
		right.role = BIG_COIN;
	}else{
		right.role = COIN;
	}

	if(right.role != WALL){
		right.weight = CalculateWeight(right.x, right.y, end_x, end_y);
	}else{
		right.weight = 1000;
	}

	DotMap up;
	up.x = start_x;
	up.y = start_y - size;

	j = up.x/size;
	i = up.y/size;
	if(map[i][j] == '#'){
		up.role = WALL;
	}else if(map[i][j] == 'c'){
		up.role = BIG_COIN;
	}else{
		up.role = COIN;
	}

	if(up.role != WALL){
		up.weight = CalculateWeight(up.x, up.y, end_x, end_y);
	}else{
		up.weight = 1000;
	}

	DotMap down;
	down.x = start_x;
	down.y = start_y + size;

	j = down.x/size;
	i = down.y/size;
	if(map[i][j] == '#'){
		down.role = WALL;
	}else if(map[i][j] == 'c'){
		down.role = BIG_COIN;
	}else{
		down.role = COIN;
	}

	if(down.role != WALL){
		down.weight = CalculateWeight(down.x, down.y, end_x, end_y);
	}else{
		down.weight = 1000;
	}


//main logic
	if(left.weight <= right.weight){
		if(down.weight <= up.weight){
			if(left.weight <= down.weight){
				way = LEFT;
			}else{
				way = DOWN;
			}
		}else{
			if(left.weight <= up.weight){
				way = LEFT;
			}else{
				way = UP;
			}
		}
	}else{
		if(down.weight <= up.weight){
			if(right.weight <= down.weight){
				way = RIGHT;
			}else{
				way = DOWN;
			}
		}else{
			if(right.weight <= up.weight){
				way = RIGHT;
			}else{
				way = UP;
			}
		}
	}
	
	if(abs(last_way - way) == 2){
		way = last_way;
	}
}

int Enemy::CalculateWeight(float start_x, float start_y, float end_x, float end_y){
	int start_dot_x = start_x/size;
	int start_dot_y = start_y/size;

	int end_dot_x = end_x/size;
	int end_dot_y = end_y/size;

	int lenght_way = 0;

	if( start_dot_x - end_dot_x >= 0){
		lenght_way += start_dot_x - end_dot_x;
	}else if( start_dot_x - end_dot_x < 0){
		lenght_way += end_dot_x - start_dot_x ;
	}

	if( start_dot_y - end_dot_y >= 0){
		lenght_way += start_dot_y - end_dot_y;
	}else if( start_dot_y - end_dot_y < 0){
		lenght_way += end_dot_y - start_dot_y;
	}
	return lenght_way;
}

bool Enemy::set_direction(std::string map[]){
	int xl = posX/size;
	int yt = posY/size;
	
	if(map[yt][xl-1] == '#' && way == LEFT){
		return false;
	}
	else if(map[yt][xl+1] == '#' && way == RIGHT){
		return false;
	}
	else if(map[yt-1][xl] == '#' && way == UP){
		return false;
	}
	else if(map[yt+1][xl] == '#' && way == DOWN){
		return false;
	}
	return true;

	
}


void Enemy::SetPos(float x, float y){
	posX = x;
	posY = y;
}

void Enemy::SetStartPos(){
	posX = start_posX;
	posY = start_posY;
}

float Enemy::getX(){
	return posX;
}

float Enemy::getY(){
	return posY;
}


void Enemy::NewGame(float posX,float posY){
	SetPos(posX,posY);
	start_posX = posX;
	start_posY = posY;
	is_dead = false;
}

void Enemy::NewGame(std::string map[],float posX,float posY){
	int i,j;
	int player_i = posY/size;
	int player_j = posX/size;
	do{
		i = (rand() % (SCREEN_HEIGHT/size-2)+1);
		j = (rand() % (SCREEN_WIDTH/size-2)+1);
	}while(map[i][j] == '#' && (abs(i-player_i) <= 3 && abs(j-player_j) <= 5));
	is_dead = false;
	SetPos(j*size+16,i*size+16);
}

void Enemy::SetSpeed(float speed){
	this->speed = speed;
}

void Enemy::SetDeadMode(){
	is_dead = true;
}

bool Enemy::GetIsDead(){
	return is_dead;
}