#include "pac_man.h"

bool FrameFunc()
{
	if(hge->Input_GetKeyState(HGEK_ENTER) && !in_game){
		in_game = true;
	}
	if(in_game){
		float dt=hge->Timer_GetDelta();
		switch(hge->Input_GetKey()){
			case HGEK_ESCAPE: return true; break;
			case HGEK_LEFT:
				key_left_pressed = true;
				key_down_pressed = false;
				key_up_pressed = false;
				key_right_pressed = false;

				break;
			case HGEK_RIGHT:
				key_left_pressed = false;
				key_down_pressed = false;
				key_up_pressed = false;
				key_right_pressed = true;

				break;
			case HGEK_UP:
				key_left_pressed = false;
				key_down_pressed = false;
				key_up_pressed = true;
				key_right_pressed = false;

				break;
			case HGEK_DOWN:
				key_left_pressed = false;
				key_down_pressed = true;
				key_up_pressed = false;
				key_right_pressed = false;

				break;
		}
		move(dt);
	}else{
		float dt=hge->Timer_GetDelta();
		static float t=0.0f;
		float tx,ty;
		int id;
		static int lastid=0;

		// If ESCAPE was pressed, tell the GUI to finish
		if(hge->Input_GetKeyState(HGEK_ESCAPE)) { lastid=5; gui->Leave(); }
		
		// We update the GUI and take an action if
		// one of the menu items was selected
		id=gui->Update(dt);
		if(id == -1)
		{
			switch(lastid)
			{
				case 1:
					in_game = true;
				case 2:
				case 3:
				case 4:
					gui->SetFocus(1);
					gui->Enter();
					break;

				case 5: return true;
			}
		}
		else if(id) { lastid=id; gui->Leave(); }

		// Here we update our background animation
		t+=dt;
		tx=50*cosf(t/60);
		ty=50*sinf(t/60);

		quad.v[0].tx=tx;        quad.v[0].ty=ty;
		quad.v[1].tx=tx+800/64; quad.v[1].ty=ty;
		quad.v[2].tx=tx+800/64; quad.v[2].ty=ty+600/64;
		quad.v[3].tx=tx;        quad.v[3].ty=ty+600/64;

	}
    return false;
}

bool RenderFunc(){
	hge->Gfx_BeginScene();

    hge->Gfx_Clear(0);
	
	if(in_game){
		player->Render(x,y);
		EatCoin();
		CreateMap();
	}else{
		hge->Gfx_RenderQuad(&quad);
		gui->Render();
		font->SetColor(0xFFFFFFFF);
		font->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d", hge->Timer_GetDelta(), hge->Timer_GetFPS());
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
    

    if(hge->System_Initiate())
    {	
		tex_second = hge->Texture_Load("res/cursor.png");
		sound = hge->Effect_Load("res/menu.wav");
		quad.tex = hge->Texture_Load("res/bg.png");

		

		tex=hge->Texture_Load("res/particles.png");
		if(!tex || !tex_second || !sound){
			MessageBox(NULL, "Can't load one of the following files:\nMENU.WAV, PARTICLES.PNG, FONT1.FNT, FONT1.PNG, TRAIL.PSI", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
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

		quad.v[0].x=0; quad.v[0].y=0; 
		quad.v[1].x=800; quad.v[1].y=0; 
		quad.v[2].x=800; quad.v[2].y=600; 
		quad.v[3].x=0; quad.v[3].y=600; 

		font=new hgeFont("res/font1.fnt");
		
		cursor = new hgeSprite(tex_second,0,0,32,32);
		ground = new hgeSprite(tex,0,32,SIZE_EL,SIZE_EL);
		coin = new hgeSprite(tex,0,64,SIZE_EL,SIZE_EL);
		coin->SetHotSpot(-16,-16);

		player = new hgeSprite(tex,96, 64, SIZE_EL, SIZE_EL);

		player->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		player->SetHotSpot(16,16);

		gui = new hgeGUI();

		gui->AddCtrl(new hgeGUIMenuItem(1,font,sound,SCREEN_WIDTH/2,(SCREEN_HEIGHT/2)-50,0.0f,"Play"));
		gui->AddCtrl(new hgeGUIMenuItem(2,font,sound,SCREEN_WIDTH/2,(SCREEN_HEIGHT/2)-25,0.1f,"Options"));
		gui->AddCtrl(new hgeGUIMenuItem(3,font,sound,SCREEN_WIDTH/2,(SCREEN_HEIGHT/2),0.2f,"Instructions"));
		gui->AddCtrl(new hgeGUIMenuItem(4,font,sound,SCREEN_WIDTH/2,(SCREEN_HEIGHT/2)+25,0.3f,"Credits"));
		gui->AddCtrl(new hgeGUIMenuItem(5,font,sound,SCREEN_WIDTH/2,(SCREEN_HEIGHT/2)+50,0.4f,"Exit"));

		gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
		gui->SetCursor(cursor);
		gui->SetFocus(1);	

		hge->System_Start();
		
		
		delete player;
		delete ground;
		delete coin;
		delete font;
		delete cursor;
		hge->Texture_Free(quad.tex);
		hge->Effect_Free(sound);
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
				coin->RenderEx(j*SIZE_EL,i*SIZE_EL,0,0.5,0);
			}
		}
	}
}

void move(float dt){
	Teleport();
	Colision();
	CheckWay();
	if(is_left){
		dx = -dt*speed;
		x += dx;		
	}
	if(is_right){
		dx = dt*speed;
		x += dx;		
	}
	if(is_up){
		dy = -dt*speed;
		y += dy;		
	}
	if(is_down){
		dy = dt*speed;
		y += dy;
	}
}

void Colision(){
	for(int i = (y-16)/SIZE_EL; i<((y-16)+SIZE_EL)/SIZE_EL;i++){
		for(int j = (x-16)/SIZE_EL; j<((x-16)+SIZE_EL)/SIZE_EL;j++){
			if(map[i][j] == '#'){
				if(is_left){ 
					is_left=false;
					x = j*SIZE_EL+16+SIZE_EL;
				}
				if(is_right) {
					is_right = false;
					x = j*SIZE_EL+16-SIZE_EL;
				}
			}
		}
	}
	for(int i = (y-16)/SIZE_EL; i<((y-16)+SIZE_EL)/SIZE_EL;i++){
		for(int j = (x-16)/SIZE_EL; j<((x-16)+SIZE_EL)/SIZE_EL;j++){
			if(map[i][j] == '#'){
				if(is_up){ 
					is_up = false;
					y = i*SIZE_EL+16+SIZE_EL;
				}
				if(is_down) {
					is_down = false;
					y = i*SIZE_EL+16-SIZE_EL;
				}
			}
		}
	}
}

void EatCoin(){
	int pos_x = x/SIZE_EL;
	int pos_y = y/SIZE_EL;
	if(map[pos_y][pos_x] == ' ' && !is_ate[pos_y][pos_x]){
		is_ate[pos_y][pos_x] = true;
		hge->Effect_Play(sound);
	}
		
}

void Teleport(){
	
	if(is_left && (x-16) < 0){
		x = SCREEN_WIDTH - SIZE_EL+16;
	}
	if(is_right && (x+16) > SCREEN_WIDTH){
		x = 16;		
	}
	if(is_up && (y-16) < 0){
		y = SCREEN_HEIGHT - SIZE_EL+16;		
	}
	if(is_down && (y+16) > SCREEN_HEIGHT){
		y = 16;
	}

}

bool CorrectX(int xl){
	if(fabs(xl*SIZE_EL-(x-16)) < 0.5){
		x = xl * SIZE_EL+16;
		return true;
	}
	return false;
}
bool CorrectY(int yt){
	if(fabs(yt*SIZE_EL - (y-16)) < 0.5){
		y = yt*SIZE_EL+16;
		return true;
	}
	return false;
}

void CheckWay(){
	xl = x/SIZE_EL;
	yt = y/SIZE_EL;
	
	if(key_left_pressed){

		if(CorrectX(xl) && CorrectY(yt)){		
		is_left = true;
		is_right = false;
		is_up = false;
		is_down = false;
	
		key_left_pressed = false;
		}
	}
			
	if(key_down_pressed){

		if(CorrectX(xl) && CorrectY(yt)){

		is_left = false;
		is_right = false;
		is_up = false;
		is_down = true;

		key_down_pressed = false;
		}
	}
			
	if(key_up_pressed){
		if(CorrectX(xl) && CorrectY(yt)){	

		is_left = false;
		is_right = false;
		is_up = true;
		is_down = false;

		key_up_pressed = false;
		}
	}
			
	if(key_right_pressed){
		if(CorrectX(xl) && CorrectY(yt)){

		is_left = false;
		is_right = true;
		is_up = false;
		is_down = false;

		key_right_pressed = false;
		}
	}

}


std::string LoadMap(std::string filename,int &width,int &height){
	std::string line;
	
	int count = 0;
	ifstream map(filename);
	if(map.is_open()){
		while(std::getline(map,line){
			count++;
			*width = line.length();		
		}
		*height = count;
		static std::string level_map[height];
		count = 0;
		
		while(std::getline(map,line){
			
			count++;
				
		}
		map.close();
	}else{
		std::string map_default[10] = {
			"####  ##############",
			"#                  #",
			"# ##               #",
			"#    ### #   # ### #",
			"# ## #   ##  # #   #",
			"  ## # # # # # ###  ",
			"# ## # # #  ## #   #",
			"# ## ### #   # ### #",
			"#                  #",
			"####  ##############"};
		*width = 20;
		*height = 10;
		return map;
	}
}