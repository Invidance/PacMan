#include "pac_man.h"
#include "menu.h"

bool FrameFunc()
{
	if(status_game == IN_GAME){
		float dt = hge->Timer_GetDelta();
		switch(hge->Input_GetKey()){
			case HGEK_ESCAPE: return true; break;
			case HGEK_LEFT:
				pac_man.set_direction(Player::KEY_LEFT);
				break;
			case HGEK_RIGHT:
				pac_man.set_direction(Player::KEY_RIGHT);
				break;
			case HGEK_UP:
				pac_man.set_direction(Player::KEY_UP);
				break;
			case HGEK_DOWN:
				pac_man.set_direction(Player::KEY_DOWN);
				break;
		}
		pac_man.move(dt,map);
		pac_man.UpdateSprite(dt);

		enemy1.move(dt,map,pac_man.getX(),pac_man.getY());
		enemy2.move(dt,map,pac_man.getX(),pac_man.getY());
		enemy3.move(dt,map,pac_man.getX(),pac_man.getY());
	}else if(status_game == MAIN_MENU){
		
		if(hge->Input_GetKeyState(HGEK_ENTER)){
			status_game = IN_GAME;
			pac_man.NewGame(is_ate,MAP_HEIGHT,MAP_WIDTH,x,y);
			enemy1.NewGame(enemy1_x,enemy1_y);
			enemy2.NewGame(enemy2_x,enemy2_y);
			enemy3.NewGame(enemy3_x,enemy3_y);
		}
	
		float dt=hge->Timer_GetDelta();
		static float t=0.0f;
		float tx,ty;
		int id;
		static int lastid=0;

		//if(hge->Input_GetKeyState(HGEK_ESCAPE)) { lastid=4; gui->Leave(); }
		
		id=gui->Update(dt);
		if(id == -1)
		{
			switch(lastid)
			{
				case 1:
					status_game = IN_GAME;
					pac_man.NewGame(is_ate,MAP_HEIGHT,MAP_WIDTH,x,y);
					enemy1.NewGame(enemy1_x,enemy1_y);
					enemy2.NewGame(enemy2_x,enemy2_y);
					enemy3.NewGame(enemy3_x,enemy3_y);
				case 2:
					status_game = IN_GAME;
					pac_man.NewGame(is_ate,MAP_HEIGHT,MAP_WIDTH,x,y);
					enemy1.NewGame(enemy1_x,enemy1_y);
					enemy2.NewGame(enemy2_x,enemy2_y);
					enemy3.NewGame(enemy3_x,enemy3_y);
				case 3:
					gui->SetFocus(1);
					gui->Enter();
					break;
				case 4:
					return true;
			}
		}
		else if(id) { lastid=id; gui->Leave(); }


		t+=dt;
		tx=50*cosf(t/60);
		ty=50*sinf(t/60);

		quad.v[0].tx=tx;					quad.v[0].ty=ty;
		quad.v[1].tx=tx+SCREEN_WIDTH/64;	quad.v[1].ty=ty;
		quad.v[2].tx=tx+SCREEN_WIDTH/64;	quad.v[2].ty=ty+SCREEN_HEIGHT/64;
		quad.v[3].tx=tx;					quad.v[3].ty=ty+SCREEN_HEIGHT/64;

	}else if(status_game == WIN){
		
		if(hge->Input_GetKeyState(HGEK_ENTER)){

			ChangeMap(map,level_2,MAP_WIDTH,MAP_HEIGHT);

			for(int i =0;i<MAP_HEIGHT;i++){
				for(int j=0;j<MAP_WIDTH;j++){	
					if(map[i][j] == ' ' || map[i][j] == 'c')
						all_coin++;	
					else if(map[i][j] == 's'){
						x = j*32.0f + 16.0f;
						y = i*32.0f + 16.0f;
					}else if(map[i][j] == 'e'){
						enemy1_x = j*32.0f + 16.0f;
						enemy1_y = i*32.0f + 16.0f;
						all_coin++;
					}else if(map[i][j] == 'r'){
						enemy2_x = j*32.0f + 16.0f;
						enemy2_y = i*32.0f + 16.0f;
						all_coin++;
					}else if(map[i][j] == 't'){
						enemy3_x = j*32.0f + 16.0f;
						enemy3_y = i*32.0f + 16.0f;
						all_coin++;
					}
				}
			}

			status_game = IN_GAME;
			
			all_coin = AllCoin(map,MAP_WIDTH,MAP_HEIGHT);
			pac_man.NewGame(is_ate,MAP_HEIGHT,MAP_WIDTH,x,y);
			enemy1.NewGame(enemy1_x,enemy1_y);
			enemy2.NewGame(enemy2_x,enemy2_y);
			enemy3.NewGame(enemy3_x,enemy3_y);
		}
		if(hge->Input_GetKeyState(HGEK_ESCAPE)){
			status_game = MAIN_MENU;
		}

	}else if(status_game == LOSE){
		if(hge->Input_GetKeyState(HGEK_ENTER)){
			status_game = IN_GAME;
			pac_man.NewGame(is_ate,MAP_HEIGHT,MAP_WIDTH,x,y);
			enemy1.NewGame(enemy1_x,enemy1_y);
			enemy2.NewGame(enemy2_x,enemy2_y);
			enemy3.NewGame(enemy3_x,enemy3_y);
		}
		if(hge->Input_GetKeyState(HGEK_ESCAPE)){
			status_game = MAIN_MENU;
			
		}
	}
    return false;
}

bool RenderFunc(){
	hge->Gfx_BeginScene();

    hge->Gfx_Clear(0);
	
	if(status_game == IN_GAME){
		
		if(pac_man.get_direction() == Player::LEFT)
			pac_man.RenderSprite(180);
		else if(pac_man.get_direction() == Player::RIGHT)
			pac_man.RenderSprite(0);
		else if(pac_man.get_direction() == Player::UP)
			pac_man.RenderSprite(270);
		else if(pac_man.get_direction() == Player::DOWN)
			pac_man.RenderSprite(90);
		else
			pac_man.RenderSprite(0);
		enemy1.RenderSprite();
		enemy2.RenderSprite();
		enemy3.RenderSprite();

		CreateMap();
		pac_man.EatCoin(is_ate, all_coin, map, status_game);

		if(pac_man.GetBigCoin()){
			pac_man.SetBigCoin(false);

			enemy1.NullDeadDelay();
			enemy2.NullDeadDelay();
			enemy3.NullDeadDelay();

			enemy1.SetDeadMode();
			enemy2.SetDeadMode();
			enemy3.SetDeadMode();
			
		}

		if(KillPlayer(pac_man, enemy1,status_game) ||
			KillPlayer(pac_man, enemy2,status_game)||
			KillPlayer(pac_man, enemy3,status_game)){
			enemy1.SetStartPos();
			enemy2.SetStartPos();
			enemy3.SetStartPos();
		}
		
		
		UpdateEnemySpeed(enemy1);
		UpdateEnemySpeed(enemy2);
		UpdateEnemySpeed(enemy3);

		font->SetColor(0xFFFFFFFF);
		
		font->printf(5, 5, HGETEXT_LEFT, "Score:%d", pac_man.GetScore());
		font->printf(SCREEN_WIDTH-75, 5, HGETEXT_LEFT, "Life: %d", pac_man.GetLife());
	}else if(status_game == MAIN_MENU){
		hge->Gfx_RenderQuad(&quad);
		gui->Render();
	}else if(status_game == WIN){
		hge->Gfx_RenderQuad(&quad);
		font->SetColor(0xFFFFFFFF);
		font->printf(SCREEN_WIDTH/2, SCREEN_HEIGHT/2-100, HGETEXT_CENTER, 
			"You win!\nYour score:%d\n\nPress Enter to continue\nPress Esc to exit",pac_man.GetScore());
	}else if(status_game == LOSE){
		hge->Gfx_RenderQuad(&quad);
		font->SetColor(0xFFFFFFFF);
		font->printf(SCREEN_WIDTH/2, SCREEN_HEIGHT/2-100, HGETEXT_CENTER, "You lose!\n\n\nPress Enter to restart\nPress Esc to exit");
	}
	
    hge->Gfx_EndScene();
	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    hge = hgeCreate(HGE_VERSION);

    hge->System_SetState(HGE_LOGFILE, "hge_tut02.log");
    hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
    hge->System_SetState(HGE_TITLE, "Test program");

    hge->System_SetState(HGE_WINDOWED, true);
    hge->System_SetState(HGE_SCREENWIDTH, SCREEN_WIDTH);
    hge->System_SetState(HGE_SCREENHEIGHT, SCREEN_HEIGHT);
	hge->System_SetState(HGE_SCREENBPP, 32);
	hge->System_SetState(HGE_ZBUFFER, true);
    

    if(hge->System_Initiate())
    {	
		tex_second = hge->Texture_Load("res/cursor.png");
		sound = hge->Effect_Load("res/hover_menu.wav");
		eat_eff = hge->Effect_Load("res/eat.wav");
		kill = hge->Effect_Load("res/kill.wav");
		quad.tex = hge->Texture_Load("res/background.png");

		tex=hge->Texture_Load("res/texture.png");

		if(!tex || !tex_second || !sound || !kill){
			MessageBox(NULL, "Can't load one of the following files", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			hge->System_Shutdown();
			hge->Release();
			return 0;
		}
		
		// Set up the quad we will use for background animation
		quad.blend=BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;

		for(int i=0;i<4;i++)
		{
			// Set up z-coordinate of vertices
			quad.v[i].z=0.5f;
			// Set up color. The format of DWORD col is 0xAARRGGBB
			quad.v[i].col=0xFFFFFFFF;
		}

		for(int i =0;i<MAP_HEIGHT;i++){
			for(int j=0;j<MAP_WIDTH;j++){	
				if(map[i][j] == ' ' || map[i][j] == 'c')
					all_coin++;	
				else if(map[i][j] == 's'){
					x = j*32.0f + 16.0f;
					y = i*32.0f + 16.0f;
				}else if(map[i][j] == 'e'){
					enemy1_x = j*32.0f + 16.0f;
					enemy1_y = i*32.0f + 16.0f;
					all_coin++;
				}else if(map[i][j] == 'r'){
					enemy2_x = j*32.0f + 16.0f;
					enemy2_y = i*32.0f + 16.0f;
					all_coin++;
				}else if(map[i][j] == 't'){
					enemy3_x = j*32.0f + 16.0f;
					enemy3_y = i*32.0f + 16.0f;
					all_coin++;
				}
			}
		}

		quad.v[0].x=0; quad.v[0].y=0; 
		quad.v[1].x=SCREEN_WIDTH; quad.v[1].y=0; 
		quad.v[2].x=SCREEN_WIDTH; quad.v[2].y=SCREEN_HEIGHT; 
		quad.v[3].x=0; quad.v[3].y=SCREEN_HEIGHT; 

		font=new hgeFont("res/font1.fnt");
		
		cursor = new hgeSprite(tex_second,0,0,32,32);
		ground = new hgeSprite(tex,64,0,SIZE_EL,SIZE_EL);
		coin = new hgeSprite(tex,96,0,SIZE_EL,SIZE_EL);
		big_coin = new hgeSprite(tex,32,32,SIZE_EL,SIZE_EL);	
	

		pac_man.PlayerInit(tex,x,y,font,eat_eff,SCREEN_WIDTH,SCREEN_HEIGHT,32,96.0f,hge);

		enemy1_spr = new hgeSprite(tex,0, 64, SIZE_EL, SIZE_EL);
		enemy1_spr->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		enemy1_spr->SetZ(0.5);
		enemy1_spr->SetHotSpot(16,16);

		enemy2_spr = new hgeSprite(tex,32, 64, SIZE_EL, SIZE_EL);
		enemy2_spr->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		enemy2_spr->SetZ(0.5);
		enemy2_spr->SetHotSpot(16,16);

		enemy3_spr = new hgeSprite(tex,64, 64, SIZE_EL, SIZE_EL);
		enemy3_spr->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		enemy3_spr->SetZ(0.5);
		enemy3_spr->SetHotSpot(16,16);

		empty_enemy_spr = new hgeSprite(tex,96, 64, SIZE_EL, SIZE_EL);
		empty_enemy_spr->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		empty_enemy_spr->SetZ(0.5);
		empty_enemy_spr->SetHotSpot(16,16);

		enemy1.EnemyInit(enemy1_spr,empty_enemy_spr,enemy1_x,enemy1_y,kill,SCREEN_WIDTH,SCREEN_HEIGHT,32,60.0f,hge);
		enemy2.EnemyInit(enemy2_spr,empty_enemy_spr,enemy2_x,enemy2_y,kill,SCREEN_WIDTH,SCREEN_HEIGHT,32,60.0f,hge);
		enemy3.EnemyInit(enemy3_spr,empty_enemy_spr,enemy3_x,enemy3_y,kill,SCREEN_WIDTH,SCREEN_HEIGHT,32,60.0f,hge);

		gui = new hgeGUI();

		gui->AddCtrl(new hgeGUIMenuItem(1,font,sound,SCREEN_WIDTH/2,(SCREEN_HEIGHT/2)-50,0.0f,"Continue"));
		gui->AddCtrl(new hgeGUIMenuItem(2,font,sound,SCREEN_WIDTH/2,(SCREEN_HEIGHT/2)-25,0.1f,"New game"));
		gui->AddCtrl(new hgeGUIMenuItem(3,font,sound,SCREEN_WIDTH/2,(SCREEN_HEIGHT/2),0.2f,"Settings"));
		gui->AddCtrl(new hgeGUIMenuItem(4,font,sound,SCREEN_WIDTH/2,(SCREEN_HEIGHT/2)+25,0.3f,"Exit"));

		gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
		gui->SetCursor(cursor);
		gui->SetFocus(1);	

		hge->System_Start();
		
		
		delete enemy1_spr;
		delete enemy2_spr;
		delete enemy3_spr;
		delete empty_enemy_spr;
		
		delete ground;
		delete coin;
		delete big_coin;
		delete font;
		delete cursor;
		hge->Texture_Free(quad.tex);
		hge->Effect_Free(sound);
		hge->Effect_Free(eat_eff);
		hge->Effect_Free(kill);
		hge->Texture_Free(tex);
		hge->Texture_Free(tex_second);
		
	}
	else
    {    
        MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
    }
    hge->System_Shutdown();
    hge->Release();
	
    return 0;
}

void CreateMap(){
	for(int i =0;i<MAP_HEIGHT;i++){
		for(int j=0;j<MAP_WIDTH;j++){
			if(map[i][j] == '#'){
				is_ate[i][j] = true;
				ground->Render(j*SIZE_EL,i*SIZE_EL);
			}
			if(map[i][j] == ' ' && !is_ate[i][j]){
				coin->SetZ(1);
				coin->Render(j*SIZE_EL,i*SIZE_EL);
			}
			if(map[i][j] == 'e' && !is_ate[i][j]){
				coin->SetZ(1);
				coin->Render(j*SIZE_EL,i*SIZE_EL);
			}
			if(map[i][j] == 'r' && !is_ate[i][j]){
				coin->SetZ(1);
				coin->Render(j*SIZE_EL,i*SIZE_EL);
			}
			if(map[i][j] == 't' && !is_ate[i][j]){
				coin->SetZ(1);
				coin->Render(j*SIZE_EL,i*SIZE_EL);
			}
			if(map[i][j] == 'c' && !is_ate[i][j]){
				big_coin->SetZ(1);
				big_coin->Render(j*SIZE_EL,i*SIZE_EL);
			}
			
		}
	}
}

bool KillPlayer(Player &player, Enemy &enemy,Status &status_game){
	float player_x = player.getX();
	float player_y = player.getY();

	float dt = hge->Timer_GetDelta();

	float enemy_x = enemy.getX();
	float enemy_y = enemy.getY();
	delay += dt;
	if(delay >= 0.02){
		if(fabs(player_x - enemy_x) < 15 && fabs(player_y - enemy_y) < 15){
			if(!enemy.GetIsDead()){
				player.Dead(status_game);
				player.StartPosition();
				enemy.KillPlayer();
				delay = 0;
				return true;
			}else{
				player.UpdateScore(200);
				enemy.KillPlayer();
				enemy.NewGame(map,player_x,player_y);
				delay = 0;
			}
		}
	}
	return false;
}

void UpdateEnemySpeed(Enemy &enemy){
	int diferent = all_coin - pac_man.GetCoin();
	enemy.SetSpeed(8.6f * (10 - diferent/50));
}

void ChangeMap(std::string map[], std::string level[], int width, int height){
	for(int i = 0;i< height;i++){
		for(int j = 0;j<width;j++){
			map[i][j] = level[i][j];
		}
	}
}

int AllCoin(std::string map[],int width, int height){
	int coin = 0;
	for(int i =0;i<height;i++){
		for(int j=0;j<width;j++){	
			if(map[i][j] == '#'){
				//dot_map[i][j] = WALL;	
			}else if(map[i][j] == ' '){
				coin++;	
				//dot_map[i][j] = COIN;
			}else if(map[i][j] == 's'){
				//dot_map[i][j] = EMPTY;
			}else if(map[i][j] == 'e'){
				coin++;	
				//dot_map[i][j] = COIN;
			}else if(map[i][j] == 'r'){
				coin++;	
				//dot_map[i][j] = COIN;
			}else if(map[i][j] == 't'){
				coin++;	
				//dot_map[i][j] = COIN;
			}else if(map[i][j] == 'c'){
				coin++;	
				//dot_map[i][j] = BIG_COIN;
			}
		}
	}
	return coin;
}


