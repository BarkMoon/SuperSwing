#include "DxLib.h"
#include "math.h"
#include "header.h"
#include "Resource.h"
 
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

int Key[256];
 
int GetHitKeyStateAll_2(int GetHitKeyStateAll_InputKey[]){
    char GetHitKeyStateAll_Key[256];
    GetHitKeyStateAll( GetHitKeyStateAll_Key );
    for(int i=0;i<256;i++){
        if(GetHitKeyStateAll_Key[i]==1) GetHitKeyStateAll_InputKey[i]++;
        else                            GetHitKeyStateAll_InputKey[i]=0;
    }
    return 0;
}
 
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){
    ChangeWindowMode(TRUE);//ウィンドウモード
	SetMainWindowText("超・ブランコ");
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 16);
	SetWindowIconID(101);
	SetOutApplicationLogValidFlag(FALSE);

	Player PL;
	Player Chara;
	Cloud clouds[RealX/CLOUD];

	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK) != 0) return -1;//初期化と裏画面化

	ImgSet();
	Set(&PL, clouds);
	Count = 0;
	Total = 0;
	NowTime = GetNowCount();
 
    while(ProcessMessage()==0 && ClearDrawScreen()==0 && GetHitKeyStateAll_2(Key)==0 && Key[KEY_INPUT_ESCAPE]==0){
          //↑ﾒｯｾｰｼﾞ処理       　  ↑画面をｸﾘｱ    　　　    ↑入力状態を保存　　　　　　　↑ESCが押されていない
	
		stringstream title;
		title << "超・ブランコ 起動時間:" << (GetNowCount() - NowTime) / 1000 << "秒";
		SetMainWindowText(title.str().c_str());

		switch (mode) {
			case DEMO:
				Demo();
				break;
			case TITLE:
				if (scene == -1) {
					SceneFrame = 0;
					++scene;
				}
				TitleBack(Titleback.y, Titleback.fall, Titleback.img[0]);
				for (int b = 0; b < 4; b++)
					TitleLetter(letter[b].x, letter[b].y, letter[b].fall, letter[b].direction, b, letter[b].img[0]);	//すべて関数内描画で
				for (int a = 0; a < 2; a++)
					TitleBird(Bird[a].x, Bird[a].y, Bird[a].direction, Bird[a].surpriseflag, a, Bird[0].img[0], Bird[0].img[1], Bird[0].img[2]);
				if (Key[KEY_INPUT_Z] == 1)
					mode = MODESELECT;
				break;
			case MODESELECT:
				ModeSelect(&PL, clouds);
				break;
			case STORY:
				Story();
				break;
			case FREEPLAY:
				FreePlay(&PL, clouds);
				break;
			case WAZACHART:
				WazaChart(&PL, clouds);
				break;
			/*case FINF:
				DrawGraph(0, 0, thx, FALSE);
				if (Key[KEY_INPUT_Z] == 1)
					mode = FIND;
				break;*/
		}

		if (mode == FIND)
			break;

		if (Key[KEY_INPUT_F1] == 1)
			Set(&PL, clouds);

		++Frame;
 
		ScreenFlip();
    }
    DxLib_End();
    return 0;
}
void ImgSet() {
	BlackBack = LoadGraph("CGs/黒画面.png");
	ResBack = LoadGraph("CGs/結果発表.png");
	SelBack[0] = LoadGraph("CGs/セレクトバック.png");
	SelBack[1] = LoadGraph("CGs/上空.png");
	logo[0] = LoadGraph("CGs/展示ロゴ.png");
	logo[1] = LoadGraph("CGs/ロゴ.png");  // LoadGraph("CGs/もょもとロゴ.png");

	bg = LoadGraph("CGs/背景.png");
	for (int i = 0; i < 50; i += 10) {
		stringstream blow_str;
		blow_str << "CGs/集中線" << i / 10 << ".png";
		blow[i] = LoadGraph(blow_str.str().c_str());
		for (int j = 1; j < 10; ++j)
			blow[i + j] = blow[i];
	}
	for (int i = 0; i < 4; ++i) {
		stringstream cloud_str;
		cloud_str << "CGs/雲" << i / 10 << ".png";
		climg[i]=LoadGraph(cloud_str.str().c_str());
	}
	Bra[0].img[0] = LoadGraph("CGs/木ブ枠.png");
	Bra[0].img[1] = LoadGraph("CGs/木ブ座.png");
	Bra[0].chain = LoadGraph("CGs/ヒモ1.png");
	Cha[0].img[TAKEOFF] = LoadGraph("CGs/ヒト座.png");
	Cha[0].img[LAND] = LoadGraph("CGs/ヒト着.png");
	Cha[0].img[NGLAND] = LoadGraph("CGs/ヒト着失.png");
	Cha[0].img[FLYING] = LoadGraph("CGs/ヒト飛.png");
	for (int i = UP; i < FUNBARI; ++i)
		Cha[0].img[i] = Cha[0].img[FLYING];
	Cha[0].img[FUNBARI] = LoadGraph("CGs/ヒト足なし.png");
	for (int i = FUNLU; i < DROP; ++i)
		Cha[0].img[i] = Cha[0].img[FUNBARI];
	Cha[0].img[DROP]=LoadGraph("CGs/ヒト落ち.png");
	for (int i = DRHOP; i < END; ++i)
		Cha[0].img[i] = Cha[0].img[DROP];
	Cha[0].img[HOP] = Cha[0].img[DROP];
	Effects[FSPIN] = LoadGraph("CGs/回転エフェクト.png");
	for (int i = BFLIP; i < BACK; ++i)
		Effects[i] = Effects[FSPIN];
	Effects[FUNBARI] = LoadGraph("CGs/バタ足.png");
	for (int i = FUNLU; i < DROP; ++i)
		Effects[i] = Effects[FUNBARI];
	Str[0] = LoadGraph("CGs/Waza.png");
	Str[1] = LoadGraph("CGs/Activated.png");
	Str[2] = LoadGraph("CGs/Lv.2.png");
	Str[3] = LoadGraph("CGs/Super.png");
	Str[4] = LoadGraph("CGs/Hyper.png");
	Meter[0] = LoadGraph("CGs/メーター.png");
	Meter[1] = LoadGraph("CGs/メーター無し.png");
	Star = LoadGraph("CGs/星.png");
	Arr = LoadGraph("CGs/矢印.png");

	Bird[0].img[0] = LoadGraph("CGs/鳥0.png");
	Bird[0].img[1] = LoadGraph("CGs/鳥1.png");
	Bird[0].img[2] = LoadGraph("CGs/鳥2.png");
	letter[0].img[0] = LoadGraph("CGs/ブ.png");
	letter[1].img[0] = LoadGraph("CGs/ラ.png");
	letter[2].img[0] = LoadGraph("CGs/ン.png");
	letter[3].img[0] = LoadGraph("CGs/コ.png");
	surprise = LoadGraph("CGs/びっくり！.png");
	speech = LoadGraph("CGs/鳥、しゃべる！.png");
	Titleback.img[0] = LoadGraph("CGs/たいとるはいけい.png");
	Cb = LoadGraph("CGs/Cb.png");
	Ques = LoadGraph("CGs/Chart/？.png");
	//thx = LoadGraph("CGs/Thank_you!.png");
	Font = CreateFontToHandle("HG創英角ﾎﾟｯﾌﾟ体", 50, -1, DX_FONTTYPE_NORMAL);
}
void Set(Player *pl, Cloud cl[]) {
	Frame = 0;
	mode = DEMO;
	phase = 0;
	viewmode = 0;
	choice = 0;
	pl->vx = 0;
	pl->vy = 0;
	pl->ax = 0;
	pl->ay = 0;
	pl->aday = 0;
	pl->rotA = 0;
	pl->rotV = 0;
	pl->direction = 2;
	pl->cooltime = 0;
	pl->rad = PI * 3.0 / 2.0;
	pl->state = TAKEOFF;
	Keylock = 0;
	memset(Keylog, 0, sizeof(Keylog));

	pl->Bimgnum = 0;			//仮置き
	pl->Cimgnum = 0;

	int t = -BackX;
	for (int i = 0; i < RealX / CLOUD; ++i) {
		cl[i].x = t + GetRand(CLOUD - 1);
		t += CLOUD;
		cl[i].y = RealY-GetRand(RealY*7/8);
		cl[i].ext = static_cast<double>(GetRand(20)) * 6;
		cl[i].type = GetRand(3);
	}

	for (int i = 0; i < 3; ++i)
		efnum[i] = 0;

	scene = -1;
	Count = 0;
	SceneFrame = 0;
	QuakeFrame = 10000;
	Titleback.x = 0;
	Titleback.y = 0;
	letter[3].y = -150;
	letter[2].y = -550;
	letter[1].y = -950;
	letter[0].y = -1350;
	Bird[0].x = 800;
	Bird[0].y = 225;
	Bird[1].x = -50;
	Bird[1].y = 225;
	for (int b = 0; b < 2; ++b)
		Bird[b].surpriseflag = 0;
	for (int a = 0; a < 4; ++a) {
		letter[a].fall = 0;
		Titleback.fall = 0;
		letter[a].direction = 0;
		letter[a].x = 325;
	}
	shakable = 0;
	quakeflag = 0;

	++Count;
}
void Demo() {
	static int phase = 0;
	switch (phase) {
		case 0:
			Logo(phase);
			break;
	}
}
void Title() {
	/*switch(phase)
		{
		case 0:
			Logo();
			break;
		}*/
}
void ModeSelect(Player* pl, Cloud cl[]) {
	string ch[2];
	if (Key[KEY_INPUT_UP] == 1)
		choice = (choice >= 1) ? choice - 1 : 1;
	if (Key[KEY_INPUT_DOWN] == 1)
		choice = (choice < 1) ? choice + 1 : 0;
	DrawGraph(0, 0, SelBack[1], FALSE);
	for (int i = 0; i < 2; ++i) {
		if (i == choice)
			ch[i] = "　▼　";
		else
			ch[i] = "　　　";
		stringstream strstr;
		strstr << ch[i] << MoSel[i];
		DrawFormatStringToHandle(0, 200 + i * 200, GetColor(0, 0, 160), Font, "%s", strstr.str().c_str());
	}
	if (Key[KEY_INPUT_Z] == 1) {
		if (choice == 0) {
			phase = 0;
			Set(pl, cl);
			mode = FREEPLAY;
		}
		else {
			choice = 0;
			phase = 0;
			Set(pl, cl);
			mode = WAZACHART;
		}
	}
}
void Story() {
}
void FreePlay(Player *pl,Cloud cl[]) {
	const int ground = WINDOW_HEIGHT * 1 / 4 - 100;
	const int start = WINDOW_WIDTH * 25;
	const int ExtX = 800, ExtY = 200;
	const int ROTSPD = 8;
	const int RES[2][2] = { {WINDOW_WIDTH / 2 - 300,WINDOW_HEIGHT / 2 - 200},{WINDOW_WIDTH / 2 + 300,WINDOW_HEIGHT / 2 + 200} };
	static int NoControl = 0;
	static double Gorad = 0;
	static int res[2][2];
	static int Wait = 0;

	int cut[2];

	KeySave();
	
	switch (phase) {
		case 0:
			StartFrame = Frame;
			for (int i = 0; i < 4; ++i)
				res[i / 2][i % 2] = MeterXY[i / 2][i % 2];
			for (int i = 0; i < END; ++i)
				Wnum[i] = 0;
			Wait = 0;
			++phase;
			break;
		case 1:
			double add;
			if (pl->rad > PI / 2.0 && pl->rad < PI * 3.0 / 2.0) {
				pl->rotA = (sin(pl->rad) + 1) * PI / 720;
				pl->direction = 1;
			}
			else if (pl->rad<PI / 2 || pl->rad>PI * 3 / 2) {
				pl->rotA = (sin(pl->rad) + 1) * (-PI / 720);
				pl->direction = 0;
			}
			else
				pl->rotA = 0;

			if (Key[KEY_INPUT_LEFT] == 1)
				add = PI / 540;
			else if (Key[KEY_INPUT_RIGHT] == 1)
				add = -PI / 540;
			else
				add = 0;
			pl->rotA += add;

			pl->rotV += pl->rotA;
			if (pl->rotV >= PI / 10)
				pl->rotV = PI / 10;
			pl->rad += pl->rotV;
			if (pl->rad >= PI * 2)
				pl->rad -= PI * 2;
			if (pl->rad < 0)
				pl->rad += PI * 2;

			pl->x = WINDOW_WIDTH * 1 / 4 + cos(pl->rad) * 100;			//右から測って
			pl->y = WINDOW_HEIGHT * 1 / 3 + sin(pl->rad) * 100;

			DrawGraph(0, 0, bg, FALSE);
			DrawBox(0, WINDOW_HEIGHT - ground, WINDOW_WIDTH + 1, WINDOW_HEIGHT + 1, GetColor(189, 66, 8), 1);

			DrawRotaGraph(WINDOW_WIDTH - (pl->x), WINDOW_HEIGHT - (pl->y), 1.0, pl->rad + PI / 2, Bra[pl->Bimgnum].img[1], TRUE);
			DrawRotaGraph(WINDOW_WIDTH - (pl->x), WINDOW_HEIGHT - (pl->y), 1.0, pl->rad + PI / 2, Cha[pl->Cimgnum].img[TAKEOFF], TRUE);
			DrawRotaGraph(WINDOW_WIDTH * 3 / 4, WINDOW_HEIGHT * 2 / 3, 1.0, pl->rad + PI / 2, Bra[pl->Bimgnum].chain, TRUE);
			DrawRotaGraph(WINDOW_WIDTH * 3 / 4, WINDOW_HEIGHT * 2 / 3, 1.0, 0, Bra[pl->Bimgnum].img[0], TRUE);
			SetFontSize(25);
			DrawString(100, 100, "←キー:左向き(時計回り)に加速。連打デキマス。", GetColor(0, 0, 0));
			DrawString(100, 125, "→キー:右向き(反時計回り)に加速。連打デキマス。", GetColor(0, 0, 0));
			DrawString(100, 150, "Zキー:ジャンプ！", GetColor(0, 0, 0));
			SetFontSize(16);

			if (pl->rad >= PI) {
				if (pl->rotV > 0)
					DrawRotaGraph(WINDOW_WIDTH - (WINDOW_WIDTH * 1 / 4 + 100), WINDOW_HEIGHT * 2 / 3 - 50, 1.0, 0, Arr, TRUE);
				if (pl->rotV < 0)
					DrawRotaGraph(WINDOW_WIDTH - (WINDOW_WIDTH * 1 / 4 - 100), WINDOW_HEIGHT * 2 / 3 - 50, 1.0, 0, Arr, TRUE, TRUE);
			}

			if (Key[KEY_INPUT_Z] == 1) {
				pl->vx = (cos(pl->rad) - cos(pl->rad - pl->rotV) + 0.2) * 2500;
				pl->vy = (sin(pl->rad) - sin(pl->rad - pl->rotV) + 0.2) * 2000;
				pl->rad = PI / 4;
				pl->state = FLYING;
				NoControl = 0;
				Gorad = 0;
				++phase;
			}
			break;
		case 2:
			if (Key[KEY_INPUT_SPACE] == 1)
				viewmode = (viewmode < 1) ? viewmode + 1 : 0;
			pl->ay = -GRAVITY + pl->aday;
			pl->vx += pl->ax;
			pl->x += pl->vx;
			pl->vy += pl->ay;
			pl->y += pl->vy;
			pl->cooltime = (pl->cooltime > 0) ? pl->cooltime - 1 : 0;
			pl->motiontime = (pl->motiontime > 0) ? pl->motiontime - 1 : 0;
			if (pl->y < ground + 30) {
				pl->y = ground + 30;
				pl->vy = 0;
				pl->vx = 0;
				quake[0] = -50;
				quake[1] = 0;
				++phase;
			}
			WX = pl->x + WINDOW_WIDTH / 2;
			WY = pl->y + WINDOW_HEIGHT / 2;

			if (Key[KEY_INPUT_UP] == 1) {
				NoControl = 0;
				for (int i = 14; i > 0; --i)
					Keylog[i] = Keylog[i - 1];
				Keylog[0] = UP;
				
				if (pl->motiontime == 0) {
					pl->state = UP;
					Gorad = 0;
					Keylock = 0;
				}
			}
			if (Key[KEY_INPUT_DOWN] == 1) {
				NoControl = 0;
				for (int i = 14; i > 0; --i)
					Keylog[i] = Keylog[i - 1];
				Keylog[0] = DOWN;
				
				if (pl->motiontime == 0) {
					pl->state = DOWN;
					Gorad = PI;
					Keylock = 0;
				}
			}
			if (Key[KEY_INPUT_LEFT] == 1) {
				NoControl = 0;
				for (int i = 14; i > 0; --i)
					Keylog[i] = Keylog[i - 1];
				Keylog[0] = LEFT;
				
				if (pl->motiontime == 0) {
					pl->state = LEFT;
					Gorad = PI * 3.0 / 2.0;
					Keylock = 0;
				}
			}
			if (Key[KEY_INPUT_RIGHT] == 1) {
				NoControl = 0;
				for (int i = 14; i > 0; --i)
					Keylog[i] = Keylog[i - 1];
				Keylog[0] = RIGHT;
				
				if (pl->motiontime == 0) {
					pl->state = RIGHT;
					Gorad = PI / 2.0;
					Keylock = 0;
				}
			}

			pl->state = WazaCheck(pl->state, pl->cooltime);

			if (Gorad != pl->rad && pl->state >= UP && pl->state <= RIGHT) {
				if (pl->rad == 0)
					pl->rad = (Gorad == PI / 2) ? ((pl->rad + PI / ROTSPD <= Gorad) ? pl->rad + PI / ROTSPD : Gorad) : ((pl->rad - PI / ROTSPD >= Gorad || pl->rad - PI / ROTSPD < 0) ? pl->rad - PI / ROTSPD : Gorad);
				else if (Gorad == 0)
					pl->rad = (pl->rad >= PI) ? ((pl->rad + PI / ROTSPD <= PI * 2) ? pl->rad + PI / ROTSPD : 0) : ((pl->rad - PI / ROTSPD >= 0) ? pl->rad - PI / ROTSPD : 0);
				else
					pl->rad = (Gorad - pl->rad > 0) ? ((pl->rad + PI / ROTSPD <= Gorad) ? pl->rad + PI / ROTSPD : Gorad) : ((pl->rad - PI / ROTSPD >= Gorad) ? pl->rad - PI / ROTSPD : Gorad);
				if (pl->rad < 0)
					pl->rad += PI * 2;
				if (pl->rad >= PI * 2)
					pl->rad -= PI * 2;
			}

			if (Key[KEY_INPUT_UP] == 0 && Key[KEY_INPUT_DOWN] == 0 && Key[KEY_INPUT_LEFT] == 0 && Key[KEY_INPUT_RIGHT] == 0 && pl->motiontime == 0)
				++NoControl;

			if (NoControl >= 30 && pl->motiontime == 0) {
				pl->state = FLYING;
				for (int i = 0; i < 10; ++i)
					Keylog[i] = 0;
			}
			if (pl->state == FLYING)
				pl->rad = atan2(-pl->vx, pl->vy);

			Waza(pl);

			
			DrawGraph(0, 0, bg, FALSE);

			if (viewmode == 1) {
				DrawBox(0, WINDOW_HEIGHT - 4, WINDOW_WIDTH + 1, WINDOW_HEIGHT + 1, GetColor(189, 66, 8), 1);
				DrawCircle((WINDOW_WIDTH * ExtX - WINDOW_WIDTH - start) / ExtX, (WINDOW_HEIGHT * ExtY - WINDOW_HEIGHT) / ExtY, 2, GetColor(0, 255, 0));
				DrawCircle((WINDOW_WIDTH * ExtX - pl->x - start) / ExtX, (pl->y / ExtY >= 4) ? (WINDOW_HEIGHT * ExtY - pl->y) / ExtY : WINDOW_HEIGHT - 4, 2, GetColor(255, 0, 0));
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
				for (int i = 0; i < 40; ++i) {
					int t = i * MeterPerDot * 100 - BackX;
					if (t == 0)
						DrawLine((WINDOW_WIDTH * ExtX - t - start) / ExtX, 0, (WINDOW_WIDTH * ExtX - t - start) / ExtX, 595, GetColor(0, 0, 0), 3);
					else if (i % 5 == 0)
						DrawLine((WINDOW_WIDTH * ExtX - t - start) / ExtX, 0, (WINDOW_WIDTH * ExtX - t - start) / ExtX, 595, GetColor(255, 0, 0), 1);
				}
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
				for (int i = 0; i < RealX / CLOUD; ++i)
					DrawRotaGraph((WINDOW_WIDTH * ExtX - cl[i].x - start) / ExtX, (WINDOW_HEIGHT * ExtY - cl[i].y) / ExtY, (cl[i].ext) / 200, 0, climg[cl[i].type], TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);
			}
			if (viewmode == 0) {
				if (pl->vx != 0)
					DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 1.0, atan2(-pl->vx, pl->vy), blow[Frame % 50], TRUE);
				DrawBox(0, WY - (WINDOW_HEIGHT / 4 - 100), WINDOW_WIDTH + 1, (WY >= WINDOW_HEIGHT) ? WY + 1 : WINDOW_HEIGHT, GetColor(189, 66, 8), 1);
				DrawPlayerWithEffect(WX, WY, pl);

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
				for (int i = 0; i < RealX / CLOUD; ++i)
					DrawRotaGraph(WX - cl[i].x, WY - cl[i].y, cl[i].ext, 0, climg[cl[i].type], TRUE);
				
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
				DrawBox(Overview[0][0], Overview[1][0], Overview[0][1], Overview[1][1], GetColor(0, 0, 0), TRUE);
				DrawBox(Overview[0][0], Overview[1][0] + (WINDOW_HEIGHT - 4) * (Overview[1][1] - Overview[1][0]) / WINDOW_HEIGHT, Overview[0][1] + 1, Overview[1][1] + 1, GetColor(189, 66, 8), 1);
				DrawCircle(Overview[0][0] + (WINDOW_WIDTH * ExtX - WINDOW_WIDTH - start) / ExtX * (Overview[0][1] - Overview[0][0]) / WINDOW_HEIGHT, Overview[1][0] + (WINDOW_HEIGHT * ExtY - WINDOW_HEIGHT) / ExtY * (Overview[1][1] - Overview[1][0]) / WINDOW_HEIGHT, 1, GetColor(0, 255, 0));
				DrawCircle(Overview[0][0] + (WINDOW_WIDTH * ExtX - pl->x - start) / ExtX * (Overview[0][1] - Overview[0][0]) / WINDOW_WIDTH, Overview[1][0] + ((pl->y / ExtY >= 4) ? (WINDOW_HEIGHT * ExtY - pl->y) / ExtY : WINDOW_HEIGHT - 4) * (Overview[1][1] - Overview[1][0]) / WINDOW_HEIGHT, 1, GetColor(255, 0, 0));
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);

				DrawExtendGraph(MeterXY[0][0], MeterXY[0][1], MeterXY[1][0], MeterXY[1][1], Meter[0], FALSE);
				if (pl->x <= MeterPerDot * 1000) {
					cut[0] = (pl->x * 100) / MeterPerDot;
					stringstream strstr;
					strstr << cut[0] / 100 << "." << ((cut[0] >= 0) ? cut[0] % 100 : (-cut[0]) % 100) << "m";
					pl->Metx = strstr.str();
				}
				else {
					cut[0] = (pl->x * 10) / MeterPerDot;
					stringstream strstr;
					strstr << cut[0] / 10 << "." << cut[0] % 10 << "m";
					pl->Metx = strstr.str();
				}
				{
					cut[1] = ((pl->y - 80) * 100) / MeterPerDot;
					stringstream strstr;
					strstr << cut[1] / 100 << "." << cut[1] % 100 << "m";
					pl->Mety = strstr.str();
				}
				SetFontSize(36);
				DrawFormatString(MeterXY[1][0] - pl->Metx.length() * 18 - 4, MeterXY[0][1] + 5, GetColor(128, 255, 0), "%s", pl->Metx.c_str());
				DrawFormatString(MeterXY[1][0] - pl->Mety.length() * 18 - 4, MeterXY[1][1] - 5 - 36, GetColor(128, 255, 0), "%s", pl->Mety.c_str());
				SetFontSize(16);
				SetFontSize(25);
				DrawString(200, 125, "十字キーをガチャれ！！！", GetColor(255, 0, 0));
				SetFontSize(16);
			}
			break;
		case 3:
			if (pl->state == UP || pl->state == FLYING || pl->state == LAND) {
				pl->state = LAND;
				pl->rad = 0;
			}
			else {
				pl->state = NGLAND;
				if (pl->rad != 0 && pl->rad != -PI / 2)
					pl->rad = 0;
			}
			DrawGraph(0, 0, bg, FALSE);
			DrawBox(0, WY - (WINDOW_HEIGHT / 4 - 100) + quake[0], WINDOW_WIDTH + 1, WINDOW_HEIGHT + quake[0], GetColor(189, 66, 8), 1);
			DrawPlayerWithEffect(WX, WY + 30 + quake[0], pl);
			if (quake[1] == 6)
				quake[0] *= -1;
			if (quake[1] >= 11)
				quake[0] = 0;
			if (Key[KEY_INPUT_Z] == 1)
				phase = (viewmode == 0) ? phase + 1 : phase + 2;
			++quake[1];
			break;
		case 4:
			bool pp;
			pp = false;
			DrawGraph(0, 0, bg, FALSE);
			DrawBox(0, WY - (WINDOW_HEIGHT / 4 - 100), WINDOW_WIDTH + 1, (WY >= WINDOW_HEIGHT) ? WY + 1 : WINDOW_HEIGHT, GetColor(189, 66, 8), 1);
			DrawPlayerWithEffect(WX, WY + 30, pl);
			for (int i = 0; i < 2; ++i) {
				res[i][0] += (RES[i][0] - MeterXY[i][0]) / 30;
				res[i][1] += (RES[i][1] - MeterXY[i][1]) / 30;
				if (res[i / 2][i % 2] >= RES[i / 2][i % 2])
					pp = true;
			}
			DrawExtendGraph(res[0][0], res[0][1], res[1][0], res[1][1], Meter[1], FALSE);
			if (pp) {
				if (pl->x >= 1000 * MeterPerDot) {
					for (int i = FSPIN; i <= HEELUP; i++)
						++Wnum[i];
					++Wnum[BACK];
					++Wnum[DASH];
					++Wnum[DROP];
				}
				if (pl->x >= 1500 * MeterPerDot) {
					for (int i = FSPIN; i < SUPER; i++)
						++Wnum[i];
					for (int i = BACK; i < END; i++)
						++Wnum[i];
				}
				if (pl->x >= 2500 * MeterPerDot) {
					++Wnum[SUPER];
					++Wnum[HYPER];
				}
				for (int i = UP; i < END; ++i) {
					stringstream chart_str;
					chart_str << "CGs/Chart/" << i << ".txt";
					ofstream ofs(chart_str.str());
					//fscanf(fp,"%d",&Wflag[i]);
					Wflag[i] = (Wnum[i] >= 1 && Wflag[i] == 0) ? 1 : Wflag[i] + Wnum[i];  // (i<=RIGHT) ? 2 : 0;
					ofs << Wflag[i] << endl;
					for (int j = 0; j < 10; ++j) {
						ofs << Wstr[i][j];
					}
				}
				++phase;
			}
			break;
		case 5:
			DrawGraph(0, 0, bg, FALSE);
			if (viewmode == 1) {
				DrawBox(0, WINDOW_HEIGHT - 4, WINDOW_WIDTH + 1, WINDOW_HEIGHT + 1, GetColor(189, 66, 8), 1);
				DrawCircle((WINDOW_WIDTH* ExtX - WINDOW_WIDTH - start) / ExtX, (WINDOW_HEIGHT* ExtY - WINDOW_HEIGHT) / ExtY, 2, GetColor(0, 255, 0));
				DrawCircle((WINDOW_WIDTH* ExtX - pl->x - start) / ExtX, (pl->y / ExtY >= 4) ? (WINDOW_HEIGHT * ExtY - pl->y) / ExtY : WINDOW_HEIGHT - 4, 2, GetColor(255, 0, 0));
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
				for (int i = 0; i < 40; ++i) {
					int t = i * MeterPerDot * 100 - BackX;
					if (t == 0)
						DrawLine((WINDOW_WIDTH * ExtX - t - start) / ExtX, 0, (WINDOW_WIDTH * ExtX - t - start) / ExtX, 595, GetColor(0, 0, 0), 3);
					else if (i % 5 == 0)
						DrawLine((WINDOW_WIDTH * ExtX - t - start) / ExtX, 0, (WINDOW_WIDTH * ExtX - t - start) / ExtX, 595, GetColor(255, 0, 0), 1);
				}
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
				for (int i = 0; i < RealX / CLOUD; ++i)
					DrawRotaGraph((WINDOW_WIDTH * ExtX - cl[i].x - start) / ExtX, (WINDOW_HEIGHT * ExtY - cl[i].y) / ExtY, (cl[i].ext) / 200, 0, climg[cl[i].type], TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);
			}
			if (viewmode == 0) {
				DrawBox(0, WY - (WINDOW_HEIGHT / 4 - 100), WINDOW_WIDTH + 1, (WY >= WINDOW_HEIGHT) ? WY + 1 : WINDOW_HEIGHT, GetColor(189, 66, 8), 1);
				DrawPlayerWithEffect(WX, WY + 30, pl);
			}

			DrawExtendGraph(RES[0][0], RES[0][1], RES[1][0], RES[1][1], Meter[1], FALSE);
			cut[0] = (pl->x * 100) / MeterPerDot;
			if (abs(cut[0] % 100) >= 10) {
				stringstream strstr;
				strstr << cut[0] / 100 << "." << ((cut[0] >= 0) ? cut[0] % 100 : -(cut[0] % 100)) << "m";
				pl->Metx = strstr.str();
			}
			else {
				stringstream strstr;
				strstr << cut[0] / 100 << ".0" << ((cut[0] >= 0) ? cut[0] % 100 : -(cut[0] % 100)) << "m";
				pl->Metx = strstr.str();
			}
			if (Wait >= 30) {
				SetFontSize(ResSize);
				DrawFormatString(WINDOW_WIDTH / 2 - (pl->Metx.length() * ResSize / 4), (WINDOW_HEIGHT - ResSize) / 2, GetColor(255, 0, 0), "%s", pl->Metx.c_str());
				SetFontSize(16);
			}
			Wait++;
			if (Key[KEY_INPUT_Z] == 1) {
				if (Wait >= 30) {
					Wait = 0;
					phase = (pl->state == NGLAND) ? phase + 1 : phase + 2;
				}
				else
					Wait = 30;
			}
			break;
		case 6:
			DrawGraph(0, 0, bg, FALSE);
			if (viewmode == 1) {
				DrawBox(0, WINDOW_HEIGHT - 4, WINDOW_WIDTH + 1, WINDOW_HEIGHT + 1, GetColor(189, 66, 8), 1);
				DrawCircle((WINDOW_WIDTH * ExtX - WINDOW_WIDTH - start) / ExtX, (WINDOW_HEIGHT * ExtY - WINDOW_HEIGHT) / ExtY, 2, GetColor(0, 255, 0));
				DrawCircle((WINDOW_WIDTH * ExtX - pl->x - start) / ExtX, (pl->y / ExtY >= 4) ? (WINDOW_HEIGHT * ExtY - pl->y) / ExtY : WINDOW_HEIGHT - 4, 2, GetColor(255, 0, 0));
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
				for (int i = 0; i < 40; ++i) {
					int t = i * MeterPerDot * 100 - BackX;
					if (t == 0)
						DrawLine((WINDOW_WIDTH * ExtX - t - start) / ExtX, 0, (WINDOW_WIDTH * ExtX - t - start) / ExtX, 595, GetColor(0, 0, 0), 3);
					else if (i % 5 == 0)
						DrawLine((WINDOW_WIDTH * ExtX - t - start) / ExtX, 0, (WINDOW_WIDTH * ExtX - t - start) / ExtX, 595, GetColor(255, 0, 0), 1);
				}
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
				for (int i = 0; i < RealX / CLOUD; ++i)
					DrawRotaGraph((WINDOW_WIDTH * ExtX - cl[i].x - start) / ExtX, (WINDOW_HEIGHT * ExtY - cl[i].y) / ExtY, (cl[i].ext) / 200, 0, climg[cl[i].type], TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);
			}
			if (viewmode == 0) {
				DrawBox(0, WY - (WINDOW_HEIGHT / 4 - 100), WINDOW_WIDTH + 1, (WY >= WINDOW_HEIGHT) ? WY + 1 : WINDOW_HEIGHT, GetColor(189, 66, 8), 1);
				DrawPlayerWithEffect(WX, WY + 30, pl);
			}

			DrawExtendGraph(RES[0][0], RES[0][1], RES[1][0], RES[1][1], Meter[1], FALSE);
			SetFontSize(ResSize);
			DrawFormatString(WINDOW_WIDTH / 2 - (6 * ResSize / 4), (WINDOW_HEIGHT - ResSize) / 2, GetColor(255, 0, 0), "-50");
			SetFontSize(16);
			++Wait;
			if (Key[KEY_INPUT_Z] == 1) {
				pl->state = LAND;
				pl->x -= MeterPerDot * 50;
				--phase;
			}
			break;
		case 7:
			int stnum;
			DrawGraph(0, 0, bg, FALSE);
			if (viewmode == 1) {
				DrawBox(0, WINDOW_HEIGHT - 4, WINDOW_WIDTH + 1, WINDOW_HEIGHT + 1, GetColor(189, 66, 8), 1);
				DrawCircle((WINDOW_WIDTH* ExtX - WINDOW_WIDTH - start) / ExtX, (WINDOW_HEIGHT* ExtY - WINDOW_HEIGHT) / ExtY, 2, GetColor(0, 255, 0));
				DrawCircle((WINDOW_WIDTH* ExtX - pl->x - start) / ExtX, (pl->y / ExtY >= 4) ? (WINDOW_HEIGHT * ExtY - pl->y) / ExtY : WINDOW_HEIGHT - 4, 2, GetColor(255, 0, 0));
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
				for (int i = 0; i < 40; ++i) {
					int t = i * MeterPerDot * 100 - BackX;
					if (t == 0)
						DrawLine((WINDOW_WIDTH * ExtX - t - start) / ExtX, 0, (WINDOW_WIDTH * ExtX - t - start) / ExtX, 595, GetColor(0, 0, 0), 3);
					else if (i % 5 == 0)
						DrawLine((WINDOW_WIDTH * ExtX - t - start) / ExtX, 0, (WINDOW_WIDTH * ExtX - t - start) / ExtX, 595, GetColor(255, 0, 0), 1);
				}
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
				for (int i = 0; i < RealX / CLOUD; ++i)
					DrawRotaGraph((WINDOW_WIDTH * ExtX - cl[i].x - start) / ExtX, (WINDOW_HEIGHT * ExtY - cl[i].y) / ExtY, (cl[i].ext) / 200, 0, climg[cl[i].type], TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);
			}
			if (viewmode == 0) {
				DrawBox(0, WY - (WINDOW_HEIGHT / 4 - 100), WINDOW_WIDTH + 1, (WY >= WINDOW_HEIGHT) ? WY + 1 : WINDOW_HEIGHT, GetColor(189, 66, 8), 1);
				DrawPlayerWithEffect(WX, WY + 30, pl);
			}
			DrawExtendGraph(RES[0][0], RES[0][1], RES[1][0], RES[1][1], Meter[1], FALSE);
			cut[0] = (pl->x * 100) / MeterPerDot;
			if (abs(cut[0] % 100) >= 10) {
				stringstream strstr;
				strstr << cut[0] / 100 << "." << ((cut[0] >= 0) ? cut[0] % 100 : -(cut[0] % 100)) << "m";
				pl->Metx = strstr.str();
			}
			else {
				stringstream strstr;
				strstr << cut[0] / 100 << ".0" << ((cut[0] >= 0) ? cut[0] % 100 : -(cut[0] % 100)) << "m";
				pl->Metx = strstr.str();
			}
			
			stnum = (cut[0] / 100 >= 2500) ? 4 : ((cut[0] / 100 >= 1000) ? 3 : ((cut[0] / 100 >= 500) ? 2 : 1));
			for (int i = 0; i < stnum; ++i)
				if (Wait >= i * 30) {
					double ra, ex, al;
					if (Wait >= (i + 1) * 30)
						DrawRotaGraph((i == 3) ? WINDOW_WIDTH / 2 : WINDOW_WIDTH / 2 + (i - 1) * 160, WINDOW_HEIGHT / 2, (i == 3) ? 3.0 : 1.5, 0, Star, TRUE);
					else {
						ra = (Wait - i * 30.0) * PI * 20.0 / 30.0;
						al = (Wait - i * 30.0) * 8.0;
						if (i == 3) {
							ex = 10.0 - (Wait - i * 30.0) * 7.0 / 30.0;
							SetDrawBlendMode(DX_BLENDMODE_ALPHA, al);
							DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, ex, ra, Star, TRUE);
							SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
							}
						else {
							ex = 10.0 - (Wait - i * 30.0) * 8.5 / 30.0;
							SetDrawBlendMode(DX_BLENDMODE_ALPHA, al);
							DrawRotaGraph(WINDOW_WIDTH / 2 + (i - 1) * 160, WINDOW_HEIGHT / 2, ex, ra, Star, TRUE);
							SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
						}
					}
				}
			if (Key[KEY_INPUT_Z] == 1) {
				if (Wait < (stnum - 1) * 30)
					Wait = (stnum - 1) * 30;
				
				else if (Count == 0) {
					Total += cut[0];
					Set(pl, cl);
					mode = FREEPLAY;
				}
			
				else {
					Total += cut[0];
					Count = Frame;
					++phase;
				}
				
				//else
				//	phase++;
			}
			++Wait;
			break;
		/*case 8:
			int evnum=0;
			if(Total>=700*100)
				evnum++;
			if(Total>=1200*100)
				evnum++;
			if(Total>=2500*100)
				evnum++;
			if(Total>=5000*100)
				evnum++;
			int n=((Frame-Count)/45<=evnum) ? (Frame-Count)/45 : evnum;

			DrawGraph(0,0,ResBack,FALSE);
			SetFontSize(100);
			DrawFormatString(WINDOW_WIDTH/2-strlen(Eval[n])*120/4,WINDOW_HEIGHT/2-120/2,GetColor(217,38,38),"%s",Eval[n]);
			SetFontSize(16);
			if(Key[KEY_INPUT_Z]==1 && (Frame-Count)/45>evnum)
				mode=FINF;
			break;*/
	case 8:
		string str[3];
		DrawGraph(0, 0, bg, FALSE);
		if (viewmode == 1) {
			DrawBox(0, WINDOW_HEIGHT - 4, WINDOW_WIDTH + 1, WINDOW_HEIGHT + 1, GetColor(189, 66, 8), 1);
			DrawCircle((WINDOW_WIDTH* ExtX - WINDOW_WIDTH - start) / ExtX, (WINDOW_HEIGHT* ExtY - WINDOW_HEIGHT) / ExtY, 2, GetColor(0, 255, 0));
			DrawCircle((WINDOW_WIDTH* ExtX - pl->x - start) / ExtX, (pl->y / ExtY >= 4) ? (WINDOW_HEIGHT * ExtY - pl->y) / ExtY : WINDOW_HEIGHT - 4, 2, GetColor(255, 0, 0));
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			for (int i = 0; i < 40; i++) {
				int t = i * MeterPerDot * 100 - BackX;
				if (t == 0)
					DrawLine((WINDOW_WIDTH * ExtX - t - start) / ExtX, 0, (WINDOW_WIDTH * ExtX - t - start) / ExtX, 595, GetColor(0, 0, 0), 3);
				else if (i % 5 == 0)
					DrawLine((WINDOW_WIDTH * ExtX - t - start) / ExtX, 0, (WINDOW_WIDTH * ExtX - t - start) / ExtX, 595, GetColor(255, 0, 0), 1);
			}
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
			for (int i = 0; i < RealX / CLOUD; i++)
				DrawRotaGraph((WINDOW_WIDTH * ExtX - cl[i].x - start) / ExtX, (WINDOW_HEIGHT * ExtY - cl[i].y) / ExtY, (cl[i].ext) / 200, 0, climg[cl[i].type], TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);
		}
		if (viewmode == 0) {
			DrawBox(0, WY - (WINDOW_HEIGHT / 4 - 100), WINDOW_WIDTH + 1, (WY >= WINDOW_HEIGHT) ? WY + 1 : WINDOW_HEIGHT, GetColor(189, 66, 8), 1);
			DrawPlayerWithEffect(WX, WY + 30, pl);
		}
		DrawExtendGraph(RES[0][0], RES[0][1], RES[1][0], RES[1][1], Meter[1], FALSE);
		cut[0] = (pl->x * 100) / MeterPerDot;
		if (abs(cut[0] % 100) >= 10) {
			stringstream strstr;
			strstr << cut[0] / 100 << "." << ((cut[0] >= 0) ? cut[0] % 100 : -(cut[0] % 100)) << "m";
			pl->Metx = strstr.str();
		}
		else {
			stringstream strstr;
			strstr << cut[0] / 100 << ".0" << ((cut[0] >= 0) ? cut[0] % 100 : -(cut[0] % 100)) << "m";
			pl->Metx = strstr.str();
		}
		
		stnum = (cut[0] / 100 >= 2500) ? 4 : ((cut[0] / 100 >= 1000) ? 3 : ((cut[0] / 100 >= 500) ? 2 : 1));
		for (int i = 0; i < stnum; i++)
			if (Wait >= (i + 1) * 30)
				DrawRotaGraph((i == 3) ? WINDOW_WIDTH / 2 : WINDOW_WIDTH / 2 + (i - 1) * 160, WINDOW_HEIGHT / 2, (i == 3) ? 3.0 : 1.5, 0, Star, TRUE);
		
		if (Key[KEY_INPUT_UP] == 1)
			choice = (choice >= 1) ? choice - 1 : 2;
		if (Key[KEY_INPUT_DOWN] == 1)
			choice = (choice < 2) ? choice + 1 : 0;

		DrawGraph(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 150, SelBack[0], FALSE);
		SetFontSize(24);
		for (int i = 0; i < 3; i++) {
			if (i == choice)
				str[i] = "▼　";
			else
				str[i] = "　　";
			str[i] += Sel[i];
			DrawFormatString(WINDOW_WIDTH / 2 - 175, WINDOW_HEIGHT / 2 - 72 + i * 72, GetColor(0, 0, 0), "%s", str[i].c_str());
		}
		if (Key[KEY_INPUT_Z] == 1) {
			phase = 0;
			if (choice == 0) {
				Set(pl, cl);
				mode = FREEPLAY;
			}
			else if (choice == 1)
				mode = MODESELECT;
			else {
				scene = 4;
				SceneFrame = Frame;
				Titleback.y = -2400;
				letter[0].x = 100;
				letter[0].y = 230;
				letter[1].x = 250;
				letter[1].y = 230;
				letter[2].x = 100;
				letter[2].y = 380;
				letter[3].x = 250;
				letter[3].y = 380;
				mode = TITLE;
			}
			choice = 0;
		}
		SetFontSize(16);
		break;
	}
	if (phase >= 3) {
		SetFontSize(32);
		DrawString(0, WINDOW_HEIGHT - 32, "Zキーで進む", GetColor(255, 255, 255));
		SetFontSize(16);
	}
}
void WazaChart(Player* pl, Cloud cl[]) {
	static int Eff[END];
	static int m, t, n;
	static double Wait;
	switch(phase) {
		case 0:		
			t = 0;
			m = 0;
			n = 0;
			for (int i = 0; i < END; i++)
				Eff[i] = -1;
			WX = -400;
			WY = -300;
			choice = 0;
			Wimg[FLYING] = LoadGraph("CGs/Chart/1.png");
			for (int i = UP; i < END; i++) {
				string chart_img, chart_data;
				stringstream chart_n_str;
				chart_n_str << "CGs/Chart/" << i;
				chart_img = chart_n_str.str() + ".png";
				Wimg[i]=LoadGraph(chart_img.c_str());
				
				chart_data = chart_n_str.str() + ".txt";
				ifstream ifs(chart_data);
				ifs >> Wflag[i];
				/*for(int j=0;j<10;j++)
					fgets(Wstr[i][j],100,fp);*/
				if (Wflag[i] == 1 && i >= FSPIN) {
					Eff[t] = i;
					++t;
				}
			}
			W[UP][0] = 0;	W[UP][1] = -1;
			W[DOWN][0] = 0;	W[DOWN][1] = 1;
			W[LEFT][0] = -1;	W[LEFT][1] = 0;
			W[RIGHT][0] = 1;	W[RIGHT][1] = 0;
			W[FUNBARI][0] = 0;	W[FUNBARI][1] = -2;
			W[DROP][0] = 0;	W[DROP][1] = 2;
			W[BACK][0] = -2;	W[BACK][1] = 0;
			W[DASH][0] = 2;	W[DASH][1] = 0;
			W[FSPIN][0] = -1;	W[FSPIN][1] = 1;
			W[BFLIP][0] = 1;	W[BFLIP][1] = -1;
			W[KICKUP][0] = -1;	W[KICKUP][1] = -1;
			W[HEELUP][0] = 1;	W[HEELUP][1] = 1;
			W[HOP][0] = 0;	W[HOP][1] = -3;
			W[DRHOP][0] = 0;	W[DRHOP][1] = 3;
			W[TURNF][0] = -3;	W[TURNF][1] = 0;
			W[NEGA][0] = 3;	W[NEGA][1] = 0;
			W[DOUBLEF][0] = -2;	W[DOUBLEF][1] = 2;
			W[DOUBLEB][0] = 2;	W[DOUBLEB][1] = -2;
			W[FUNLU][0] = 2;	W[FUNLU][1] = 2;
			W[SUPER][0] = 0;	W[SUPER][1] = -4;
			W[SPINDU][0] = -1;	W[SPINDU][1] = 3;
			W[SUPERF][0] = -3;	W[SUPERF][1] = 3;
			W[SUPERB][0] = 3;	W[SUPERB][1] = -3;
			W[SUPERGO][0] = -4;	W[SUPERGO][1] = 0;
			W[SUPERDR][0] = 0;	W[SUPERDR][1] = 4;
			W[HYPER][0] = 0;	W[HYPER][1] = -5;
			Wait = 0;
			phase += (Eff[0] == -1) ? 2 : 1;
			break;
		case 1:
			++Wait;
			WX = 325 * W[Eff[m]][0] - WINDOW_WIDTH / 2;
			WY = 175 * W[Eff[m]][1] - WINDOW_HEIGHT / 2;

			if (Key[KEY_INPUT_Z] == 1 && Wait >= 45) {
				stringstream chart_str;
				chart_str << "CGs/Chart/" << Eff[m] << ".txt";
				ofstream ofs(chart_str.str());
				Wflag[Eff[m]]=2;
				ofs << Wflag[Eff[m]] << endl;
				if (m + 1 < t) {
					++m;
					Wait = 0;
				}
				else
					++phase;
			}

			DrawRotaGraph(-WX, -WY, 4.0, 0, logo[0], FALSE);
			DrawRotaGraph(-WX, -WY, 1.0, 0, Wimg[FLYING], FALSE);
			for (int i = UP; i < END; i++) {
				if (Wflag[i] >= 2 || i <= RIGHT)
					DrawRotaGraph(325 * W[i][0] - WX, 175 * W[i][1] - WY, 1.0, 0, Wimg[i], FALSE);
				else
					DrawRotaGraph(325 * W[i][0] - WX, 175 * W[i][1] - WY, 1.0, 0, Ques, FALSE);
			}
			double ra, ex, al;
			ra = Wait * PI * 20 / 120;
			if (ra >= 20 * PI)
				ra = 2 * PI;
			al = Wait * 255 / 120;
			if (al >= 255)
				al = 255;
			ex = Wait / 120;
			if (ex >= 1.0)
				ex = 1.0;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, al);
			DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, ex, ra, Wimg[Eff[m]], FALSE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			break;
		case 2:
			if (Key[KEY_INPUT_UP] >= 1)
				WY = (WY - 10 >= -1200) ? WY - 10 : -1200;
			if (Key[KEY_INPUT_DOWN] >= 1)
				WY = (WY + 10 <= 600) ? WY + 10 : 600;
			if (Key[KEY_INPUT_LEFT] >= 1)
				WX = (WX - 10 >= -1600) ? WX - 10 : -1600;
			if (Key[KEY_INPUT_RIGHT] >= 1)
				WX = (WX + 10 <= 800) ? WX + 10 : 800;
			DrawRotaGraph(-WX, -WY, 4.0, 0, logo[0], FALSE);
			DrawRotaGraph(-WX, -WY, 1.0, 0, Wimg[FLYING], FALSE);
			for (int i = UP; i < END; i++) {
				int x, y;
				x = 325 * W[i][0] - WX;
				y = 175 * W[i][1] - WY;
				if (Wflag[i] >= 2 || i <= RIGHT)
					DrawRotaGraph(x, y, 1.0, 0, Wimg[i], FALSE);
				else
					DrawRotaGraph(x, y, 1.0, 0, Ques, FALSE);
				if (300 <= x && x <= 500 && y >= 250 && y <= 350) {
					if (Wflag[i] >= 2)
						n = i;
					else
						n = 0;
				}
			}
			if (n != 0) {
				DrawGraph(50, 0, Cb, FALSE);
				SetFontSize(24);
				DrawString(60, 44, "コマンド:", GetColor(0, 0, 0));
				SetFontSize(48);
				DrawFormatString(425 - strlen(Com[n]) * 12, 38, GetColor(0, 0, 0), "%s", Com[n]);
				SetFontSize(16);
			}
			break;
	}
	if (Key[KEY_INPUT_X] == 1)
		mode = MODESELECT;
}
void Logo(int &ph) {
	static int scene = 0, Alpha = 0;
	DrawGraph(0, 0, logo[0], FALSE);

	DrawGraph(0, 0, BlackBack, FALSE);
	for (int i = 0; i < 2; i++)
		if ((scene <= 1 && i == 0) || (scene >= 2 && i == 1))
			switch (scene % 2) {
				case 0:
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, (Alpha < 255) ? Alpha : 255);
					DrawGraph(0, 0, logo[i], FALSE);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
					Alpha += 1;
					if (Key[KEY_INPUT_Z] == 1)
						Alpha = 316;
					if (Alpha >= 316) {
						Alpha = 255;
						++scene;
					}
					break;
				case 1:
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, (Alpha < 0) ? 0 : Alpha);
					DrawGraph(0, 0, logo[i], FALSE);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
					Alpha -= 1;
					if (Key[KEY_INPUT_Z] == 1)
						Alpha = -31;
					if (Alpha <= -31) {
						Alpha = 0;
						++scene;
						if (scene == 4) {
							scene = 0;
							//++ph;
							mode = TITLE;  // 展示用ならFREEPLAY。
						}
					}
					break;
			}
}
int WazaCheck(int st, int cooltime) {
	int rand[3];
	rand[0] = GetRand(3) + 3;
	rand[1] = GetRand(3) + 100;  //NW
	rand[2] = GetRand(3) + 1000;  //NW
	++efnum[1];

	if (Keylog[11] == LEFT && Keylog[10] == DOWN && Keylog[9] == RIGHT && Keylog[8] == UP && Keylog[7] == LEFT && Keylog[6] == DOWN && Keylog[5] == RIGHT && Keylog[4] == UP && Keylog[3] == LEFT && Keylog[2] == DOWN && Keylog[1] == RIGHT && Keylog[0] == UP && Keylock != SUPERF) {
		for (int i = 0; i < 2; i++)
			efnum[i] = 0;
		efnum[2] = rand[1];
		Keylock = SUPERF;
		return SUPERF;
	}
	if (Keylog[11] == UP && Keylog[10] == RIGHT && Keylog[9] == DOWN && Keylog[8] == LEFT && Keylog[7] == UP && Keylog[6] == RIGHT && Keylog[5] == DOWN && Keylog[4] == LEFT && Keylog[3] == UP && Keylog[2] == RIGHT && Keylog[1] == DOWN && Keylog[0] == LEFT && Keylock != BFLIP && Keylock != DOUBLEB && Keylock != SUPERB) {
		for (int i = 0; i < 2; i++)
			efnum[i] = 0;
		efnum[2] = rand[1];
		Keylock = SUPERB;
		return SUPERB;
	}
	if (Keylog[9] == UP && Keylog[8] == UP && Keylog[7] == DOWN && Keylog[6] == DOWN && Keylog[5] == LEFT && Keylog[4] == RIGHT && Keylog[3] == LEFT && Keylog[2] == RIGHT && Keylog[1] == DOWN && Keylog[0] == RIGHT && Keylock != HEELUP && Keylock != HYPER) {
		for (int i = 0; i < 2; i++)
			efnum[i] = 0;
		efnum[2] = rand[2];
		Keylock = HYPER;
		return HYPER;
	}
	if (Keylog[7] == LEFT && Keylog[6] == DOWN && Keylog[5] == RIGHT && Keylog[4] == UP && Keylog[3] == LEFT && Keylog[2] == DOWN && Keylog[1] == RIGHT && Keylog[0] == UP && Keylock != DOUBLEF && Keylock != SUPERF) {
		for (int i = 0; i < 2; i++)
			efnum[i] = 0;
		efnum[2] = rand[0];
		Keylock = DOUBLEF;
		return DOUBLEF;
	}
	if (Keylog[7] == UP && Keylog[6] == RIGHT && Keylog[5] == DOWN && Keylog[4] == LEFT && Keylog[3] == UP && Keylog[2] == RIGHT && Keylog[1] == DOWN && Keylog[0] == LEFT && Keylock != BFLIP && Keylock != DOUBLEB && Keylock != SUPERB) {
		for (int i = 0; i < 2; i++)
			efnum[i] = 0;
		efnum[2] = rand[0];
		Keylock = DOUBLEB;
		return DOUBLEB;
	}
	if (Keylog[7] == UP && Keylog[6] == UP && Keylog[5] == DOWN && Keylog[4] == DOWN && Keylog[3] == LEFT && Keylog[2] == RIGHT && Keylog[1] == LEFT && Keylog[0] == RIGHT && Keylock != BACK && Keylock != SUPER) {
		for (int i = 0; i < 2; i++)
			efnum[i] = 0;
		efnum[2] = rand[1];
		Keylock = SUPER;
		return SUPER;
	}
	if (Keylog[5] == LEFT && Keylog[4] == DOWN && Keylog[3] == RIGHT && Keylog[2] == UP && Keylog[1] == DOWN && Keylog[0] == UP && Keylock != SPINDU) {
		for (int i = 0; i < 2; i++)
			efnum[i] = 0;
		efnum[2] = rand[0];
		return Keylock = SPINDU;
	}
	if (Keylog[4] == DOWN && Keylog[3] == DOWN && Keylog[2] == DOWN && Keylog[1] == DOWN && Keylog[0] == DOWN && Keylock != SUPERDR) {
		for (int i = 0; i < 2; i++)
			efnum[i] = 0;
		efnum[2] = rand[1];
		return Keylock = SUPERDR;
	}
	if (Keylog[3] == UP && Keylog[2] == UP && Keylog[1] == DOWN && Keylog[0] == DOWN && Keylock != HOP && Keylock != DRHOP) {
		for (int i = 0; i < 2; i++)
			efnum[i] = 0;
		efnum[2] = rand[0];
		Keylock = HOP;
		return HOP;
	}
	if (Keylog[3] == DOWN && Keylog[2] == RIGHT && Keylog[1] == DOWN && Keylog[0] == RIGHT && Keylock != FUNLU) {
		for (int i = 0; i < 2; i++)
			efnum[i] = 0;
		efnum[2] = rand[0];
		return Keylock = FUNLU;
	}
	if (Keylog[3] == DOWN && Keylog[2] == DOWN && Keylog[1] == UP && Keylog[0] == UP && Keylock != HOP && Keylock != DRHOP) {
		for (int i = 0; i < 2; i++)
			efnum[i] = 0;
		efnum[2] = rand[0];
		return Keylock = DRHOP;
	}
	if (Keylog[2] == LEFT && Keylog[1] == RIGHT && Keylog[0] == LEFT && Keylock != TURNF) {
		for (int i = 0; i < 2; i++)
			efnum[i] = 0;
		efnum[2] = rand[0];
		return Keylock = TURNF;
	}
	if(cooltime==0) {
		if (Keylog[5] == RIGHT && Keylog[4] == RIGHT && Keylog[3] == RIGHT && Keylog[2] == RIGHT && Keylog[1] == RIGHT && Keylog[0] == RIGHT && Keylock != NEGA) {
			for (int i = 0; i < 2; i++)
				efnum[i] = 0;
			efnum[2] = 1;
			return Keylock = NEGA;
		}
		if (Keylog[4] == LEFT && Keylog[3] == LEFT && Keylog[2] == LEFT && Keylog[1] == LEFT && Keylog[0] == LEFT && Keylock != SUPERGO) {
			for (int i = 0; i < 2; i++)
				efnum[i] = 0;
			efnum[2] = 1;
			return Keylock = SUPERGO;
		}
		if (Keylog[3] == LEFT && Keylog[2] == DOWN && Keylog[1] == RIGHT && Keylog[0] == UP && Keylock != FSPIN && Keylock != DOUBLEF && Keylock != SUPERF) {
			for (int i = 0; i < 2; i++)
				efnum[i] = 0;
			efnum[2] = 1;
			Keylock = FSPIN;
			return FSPIN;
		}
		if (Keylog[3] == UP && Keylog[2] == RIGHT && Keylog[1] == DOWN && Keylog[0] == LEFT && Keylock != BFLIP && Keylock != DOUBLEB && Keylock != SUPERB) {
			for (int i = 0; i < 2; i++)
				efnum[i] = 0;
			efnum[2] = 1;
			Keylock = BFLIP;
			return BFLIP;
		}
		if (Keylog[1] == LEFT && Keylog[0] == UP && Keylock != KICKUP) {
			for (int i = 0; i < 2; i++)
				efnum[i] = 0;
			efnum[2] = 1;
			return Keylock = KICKUP;
		}
		if (Keylog[1] == DOWN && Keylog[0] == RIGHT && Keylock != HEELUP && Keylock != FUNLU && Keylock != HYPER) {
			for (int i = 0; i < 2; i++)
				efnum[i] = 0;
			efnum[2] = 1;
			return Keylock = HEELUP;
		}
		if (Keylog[1] == LEFT && Keylog[0] == RIGHT && Keylock != BACK && Keylock != SUPER) {
			for (int i = 0; i < 2; i++)
				efnum[i] = 0;
			efnum[2] = 1;
			return Keylock = BACK;
		}
		if (Keylog[1] == RIGHT && Keylog[0] == LEFT && Keylock != DASH) {
			for (int i = 0; i < 2; i++)
				efnum[i] = 0;
			efnum[2] = 1;
			return Keylock = DASH;
		}
		if (Keylog[1] == UP && Keylog[0] == UP && Keylock != FUNBARI) {
			for (int i = 0; i < 2; i++)
				efnum[i] = 0;
			efnum[2] = 1;
			Keylock = FUNBARI;
			return FUNBARI;
		}
		if (Keylog[1] == DOWN && Keylog[0] == DOWN && Keylock != DROP && Keylock != HOP && Keylock != SUPERDR) {
			for (int i = 0; i < 2; i++)
				efnum[i] = 0;
			efnum[2] = 1;
			Keylock = DROP;
			return DROP;
		}
	}
	return st;
	/*
	if(Keylog[3]== && Keylog[2]== && Keylog[1]== && Keylog[0]== && Keylock!=)
			{
			for(int i=0;i<2;i++)
				efnum[i]=0;
			efnum[2]=1;
			return Keylock=;
			}
	*/
}
void Waza(Player *pl) {
	if (efnum[0] == 0) {
		++Wnum[pl->state];
		switch (pl->state) {
			case FSPIN:			
				pl->vx = 1300;
				pl->vy -= 6;
				pl->rad = 0;
				pl->cooltime = 45;
				pl->motiontime = 30;
				break;
			case BFLIP:
				pl->vy = (pl->vy < 0) ? pl->vy * (-0.9) : pl->vy + 300;
				pl->vx *= -1;
				pl->cooltime = 60;
				pl->motiontime = 45;
				break;
			case KICKUP:
				pl->vy *= 0.5;
				pl->rad = 0;
				pl->cooltime = 30;
				pl->motiontime = 30;
				break;
			case HEELUP:
				pl->vx *= 1.1;
				pl->rad = 0;
				pl->cooltime = 30;
				pl->motiontime = 30;
				break;
			case HOP:
				pl->vy = 100;
				pl->aday -= 0.5;
				pl->rad = 0;
				pl->cooltime = 90;
				pl->motiontime = 60;
				break;
			case DOUBLEF:	
				pl->vx *= 1.3;
				pl->vy -= 12;
				pl->aday -= 1.0;
				pl->rad = 0;
				pl->cooltime = 45;
				pl->motiontime = 45;
				break;
			case SUPERF:			
				pl->vx *= 1.3;
				pl->vy -= 18;
				pl->aday = 0;
				pl->rad = 0;
				pl->cooltime = 45;
				pl->motiontime = 45;
				break;
			case SPINDU:			
				pl->vx *= 1.2;
				pl->vy = -PI / 4;
				pl->aday -= 0.5;
				pl->cooltime = 60;
				pl->motiontime = 60;
				break;
			case DOUBLEB:			
				pl->vy = (pl->vy < 0) ? pl->vy * (-1.2) : pl->vy + 450;
				pl->vx *= -1;
				pl->cooltime = 90;
				pl->motiontime = 90;
				break;
			case SUPERB:			
				pl->vy = (pl->vy < 0) ? pl->vy * (-1.5) : pl->vy + 600;
				pl->vx *= -1;
				pl->cooltime = 90;
				pl->motiontime = 90;
				break;
			case SUPER:
				pl->vx = (pl->vx > 0) ? pl->vx * 1.2 : 1200;
				pl->vy = 50;
				pl->rad = -PI / 3;
				pl->cooltime = 90;
				pl->motiontime = 90;
				break;
			case HYPER:
				pl->vx *= 1.7;
				pl->vy = 150;
				pl->rad = -PI / 3;
				pl->cooltime = 90;
				pl->motiontime = 90;
				break;
			case BACK:
				pl->vx = (pl->vx > 0) ? pl->vx * (-1) : pl->vx * 1.5;
				pl->cooltime = 30;
				pl->motiontime = 30;
				break;
			case TURNF:
				pl->vx = (pl->vx > 0) ? pl->vx * 1.5 : pl->vx * (-1.5);
				pl->cooltime = 60;
				pl->motiontime = 45;
				break;
			case SUPERGO:
				pl->vx = (pl->vx > 0) ? pl->vx * 2.5 : 1500;
				pl->vy = (pl->vy > 0) ? pl->vy -= 30 : pl->vy *= 1.2;
				pl->cooltime = 90;
				pl->motiontime = 90;
				break;
			case DASH:
				pl->vx = (pl->vx < 0) ? 800 : pl->vx * 1.2;
				pl->cooltime = 45;
				pl->motiontime = 45;
				break;
			case NEGA:
				pl->vx = 0;
				pl->vy = 1000;
				pl->aday -= 2.0;
				pl->cooltime = 90;
				pl->motiontime = 90;
				break;
			case FUNBARI:
				pl->vy = (pl->vy < 0) ? pl->vy *= 0 : pl->vy + 100;
				pl->rad = 0;
				pl->cooltime = 75;
				pl->motiontime = 30;
				break;
			case FUNLU:
				pl->vy = (pl->vy < 0) ? 0 : pl->vy + 100;
				pl->vx += 100;
				pl->aday -= 0.5;
				pl->rad = -PI / 4;
				pl->cooltime = 75;
				pl->motiontime = 30;
				break;
			case DROP:
				pl->vy = (pl->vy < 0) ? pl->vy *= 1.1 : pl->vy - 30;
				pl->rad = 0;
				pl->cooltime = 30;
				pl->motiontime = 30;
				break;
			case DRHOP:
				pl->vy = 200;
				pl->aday -= 1.0;
				pl->rad = 0;
				pl->cooltime = 90;
				pl->motiontime = 60;
				break;
			case SUPERDR:
				pl->vy = (pl->vy < 0) ? pl->vy *= 2 : pl->vy - 50;
				pl->rad = 0;
				pl->cooltime = 30;
				pl->motiontime = 30;
				break;
		}
		efnum[0] = 1;
	}

	switch(pl->state) {
		case FSPIN:
			pl->rad -= PI / 7.5;
			if (efnum[1] == 30)
				pl->state = UP;
			break;
		case BFLIP:
			pl->rad += PI * 4.0 / 45.0;
			if(efnum[1]==45) {
				pl->vx *= -1;
				pl->state = RIGHT;
			}
			break;
		case KICKUP:
			pl->rad += PI / 60;
			if (efnum[1] == 30)
				pl->state = RIGHT;
			break;
		case HEELUP:
			pl->rad -= PI / 60;
			if (efnum[1] == 30)
				pl->state = LEFT;
			break;
		case DOUBLEF:
			pl->rad -= PI * 8 / 45;
			if (efnum[1] == 45)
				pl->state = UP;
			break;
		case SUPERF:
			pl->rad -= PI * 16 / 45;
			if (efnum[1] == 45)
				pl->state = UP;
			break;
		case DOUBLEB:
			pl->rad += PI * 8 / 90;
			if(efnum[1]==90) {
				pl->vx *= -1.3;
				pl->aday -= 0.5;
				pl->state = RIGHT;
			}
			break;
		case SUPERB:
			pl->rad += PI * 16 / 90;
			if (efnum[1] == 90) {
				pl->vx *= -1.5;
				pl->aday -= 1.0;
				pl->state = RIGHT;
			}
			break;
		case SUPER:
			pl->rad -= PI * 24 / 90;
			if (efnum[1] == 90) {
				pl->aday -= 1.0;
				pl->state = LEFT;
			}
			break;
		case HYPER:
			pl->rad -= PI * 32 / 90;
			if (efnum[1] == 90) {
				pl->aday -= 1.0;
				pl->state = UP;
			}
			break;
		case BACK:
		case TURNF:
		case SUPERGO:
		case DASH:
			if (efnum[1] == 30)
				pl->state = LEFT;
			break;
		case NEGA:
			if (efnum[1] == 90)
				pl->state = UP;
			break;
		case FUNBARI:
		case FUNLU:
		case DROP:
		case SUPERDR:
			if (efnum[1] == 30)
				pl->state = UP;
			break;
		case SPINDU:
		case HOP:
		case DRHOP:
			if (efnum[1] == 60)
				pl->state = UP;
			break;
	}
	/*
	case :
				pl->cooltime=;
				pl->motiontime=30;
				break;
	*/
}
void DrawPlayerWithEffect(int WX, int WY, Player* pl) {
	Activated(pl->state);
	DrawRotaGraph(WX - pl->x, WY - pl->y, 1.0, pl->rad, Cha[pl->Cimgnum].img[pl->state], TRUE);
	if (pl->motiontime > 0) {
		if (pl->state == FSPIN || pl->state == DOUBLEF || pl->state == SUPERF || pl->state == SUPER || pl->state == HYPER)
			DrawRotaGraph(WX - pl->x, WY - pl->y, 1.0, pl->rad, Effects[pl->state], TRUE);
		if (pl->state == BFLIP || pl->state == DOUBLEB || pl->state == SUPERB)
			DrawRotaGraph(WX - pl->x, WY - pl->y, 1.0, pl->rad, Effects[pl->state], TRUE, TRUE);
		if (pl->state == KICKUP)
			DrawRotaGraph(WX - pl->x, WY - pl->y, 1.0, pl->rad + PI, Effects[pl->state], TRUE, TRUE);
		if (pl->state == HEELUP)
			DrawRotaGraph(WX - pl->x, WY - pl->y, 1.0, pl->rad + PI, Effects[pl->state], TRUE);
		if (pl->state == FUNBARI)
			DrawRotaGraph(WX - pl->x, WY - pl->y, 1.0, -PI / 6 + (Frame % 7) * (PI / 18), Effects[pl->state], TRUE);
		if (pl->state == FUNLU)
			DrawRotaGraph(WX - pl->x, WY - pl->y, 1.0, -PI * 5 / 12 + (Frame % 7) * (PI / 18), Effects[pl->state], TRUE);
	}
}
void Activated(int st) {
	const int X[3] = { 250,550,400 };
	const int Y = 120;
	static int x[5];
	static double ext, rad;
	static int turn;
	switch (efnum[2]) {
		case 0:
			for (int i = 0; i < 4; i++)
				x[i] = WINDOW_WIDTH;
			ext = 1.0;
			rad = 0;
			turn = 0;
			break;
		case 1:
			x[0] -= 30;
			DrawRotaGraph((x[0] < X[0]) ? X[0] : x[0], Y, 1.0, 0, Str[0], TRUE);
			if (X[0] - x[0] > 150)
				efnum[2] = 2;
			break;
		case 2:
			x[1] -= 40;
			DrawRotaGraph(X[0], Y, ext, 0, Str[0], TRUE, turn);
			DrawRotaGraph((x[1] <= X[1]) ? X[1] : x[1], Y, 1.0, 0, Str[1], TRUE);
			if (X[1] - x[1] > 1200)
				efnum[2] = 0;
			break;
		case 3:
			x[2] = (x[2] - 50 >= X[2]) ? x[2] - 50 : X[2];
			ext += 0.05;
			if (ext > 1.7)
				efnum[2] = 30;
			DrawRotaGraph(X[0], Y, ext, 0, Str[0], TRUE, turn);
			DrawRotaGraph(X[1], Y, 1.0, 0, Str[1], TRUE);
			DrawRotaGraph(x[2], Y + 60, 1.0, 0, Str[2], TRUE);
			break;
		case 30:
			x[2] = (x[2] - 50 >= X[2]) ? x[2] - 50 : X[2];
			ext -= 0.05;
			if (ext < -0.5)
				efnum[2] = 0;
			DrawRotaGraph(X[0], Y, (ext < 1.0) ? 1.0 : ext, 0, Str[0], TRUE, turn);
			DrawRotaGraph(X[1], Y, 1.0, 0, Str[1], TRUE);
			DrawRotaGraph(x[2], Y + 60, 1.0, 0, Str[2], TRUE);
			break;
		case 4:
			x[2] = (x[2] - 50 >= X[2]) ? x[2] - 50 : X[2];
			turn = ((efnum[1] >= 20 /*&& efnum[1]<=20) || (efnum[1]>=30*/ && efnum[1] <= 40)) ? 1 : 0;
			if (efnum[1] >= 70)
				efnum[2] = 0;
			DrawRotaGraph(X[0], Y, (turn == 1) ? 1.2 : 1.0, 0, Str[0], TRUE, turn);
			DrawRotaGraph(X[1], Y, 1.0, 0, Str[1], TRUE);
			DrawRotaGraph(x[2], Y + 60, 1.0, 0, Str[2], TRUE);
			break;
		case 5:
			x[2] = (x[2] - 50 >= X[2]) ? x[2] - 50 : X[2];
			rad += PI * 2 / 30;
			if (rad >= 2 * PI)
				efnum[2] = 0;
			DrawRotaGraph(X[0], Y, 1.2, rad, Str[0], TRUE);
			DrawRotaGraph(X[1], Y, 1.0, 0, Str[1], TRUE);
			DrawRotaGraph(x[2], Y + 60, 1.0, 0, Str[2], TRUE);
			break;
		case 6:
			x[2] = (x[2] - 50 >= X[2]) ? x[2] - 50 : X[2];
			rad -= PI * 2 / 30;
			if (rad <= -2 * PI)
				efnum[2] = 0;
			DrawRotaGraph(X[0], Y, 1.2, rad, Str[0], TRUE);
			DrawRotaGraph(X[1], Y, 1.0, 0, Str[1], TRUE);
			DrawRotaGraph(x[2], Y + 60, 1.0, 0, Str[2], TRUE);
			break;
		case 100:
			x[3] = (x[3] - 50 >= X[2]) ? x[3] - 50 : X[2];
			ext += 0.05;
			if (ext > 1.7)
				efnum[2] = 110;
			DrawRotaGraph(X[0], Y, ext, 0, Str[0], TRUE, turn);
			DrawRotaGraph(X[1], Y, 1.0, 0, Str[1], TRUE);
			DrawRotaGraph(x[3], Y + 60, 1.0, 0, Str[3], TRUE);
			break;
		case 110:
			x[3] = (x[3] - 50 >= X[2]) ? x[3] - 50 : X[2];
			ext -= 0.05;
			if (ext < -0.5)
				efnum[2] = 0;
			DrawRotaGraph(X[0], Y, (ext < 1.0) ? 1.0 : ext, 0, Str[0], TRUE, turn);
			DrawRotaGraph(X[1], Y, 1.0, 0, Str[1], TRUE);
			DrawRotaGraph(x[3], Y + 60, 1.0, 0, Str[3], TRUE);
			break;
		case 101:
			x[3] = (x[3] - 50 >= X[2]) ? x[3] - 50 : X[2];
			turn = ((efnum[1] >= 20 /*&& efnum[1]<=20) || (efnum[1]>=30*/ && efnum[1] <= 40)) ? 1 : 0;
			if (efnum[1] >= 140)
				efnum[2] = 0;
			DrawRotaGraph(X[0], Y, (turn == 1) ? 1.2 : 1.0, 0, Str[0], TRUE, turn);
			DrawRotaGraph(X[1], Y, 1.0, 0, Str[1], TRUE);
			DrawRotaGraph(x[3], Y + 60, 1.0, 0, Str[3], TRUE);
			break;
		case 102:
			x[3] = (x[3] - 50 >= X[2]) ? x[3] - 50 : X[2];
			rad += PI * 2 / 30;
			if (rad >= 2 * PI)
				efnum[2] = 0;
			DrawRotaGraph(X[0], Y, 1.2, rad, Str[0], TRUE);
			DrawRotaGraph(X[1], Y, 1.0, 0, Str[1], TRUE);
			DrawRotaGraph(x[3], Y + 60, 1.0, 0, Str[3], TRUE);
			break;
		case 103:
			x[3] = (x[3] - 50 >= X[2]) ? x[3] - 50 : X[2];
			rad -= PI * 2 / 30;
			if (rad <= -2 * PI)
				efnum[2] = 0;
			DrawRotaGraph(X[0], Y, 1.2, rad, Str[0], TRUE);
			DrawRotaGraph(X[1], Y, 1.0, 0, Str[1], TRUE);
			DrawRotaGraph(x[3], Y + 60, 1.0, 0, Str[3], TRUE);
			break;
		case 1000:
			x[4] = (x[4] - 50 >= X[2]) ? x[4] - 50 : X[2];
			ext += 0.05;
			if (ext > 1.7)
				efnum[2] = 110;
			DrawRotaGraph(X[0], Y, ext, 0, Str[0], TRUE, turn);
			DrawRotaGraph(X[1], Y, 1.0, 0, Str[1], TRUE);
			DrawRotaGraph(x[3], Y + 60, 1.0, 0, Str[4], TRUE);
			break;
		case 1010:
			x[4] = (x[4] - 50 >= X[2]) ? x[4] - 50 : X[2];
			ext -= 0.05;
			if (ext < -0.5)
				efnum[2] = 0;
			DrawRotaGraph(X[0], Y, (ext < 1.0) ? 1.0 : ext, 0, Str[0], TRUE, turn);
			DrawRotaGraph(X[1], Y, 1.0, 0, Str[1], TRUE);
			DrawRotaGraph(x[3], Y + 60, 1.0, 0, Str[4], TRUE);
			break;
		case 1001:
			x[4]=(x[4]-50>=X[2]) ? x[4]-50 : X[2];
			turn = ((efnum[1] >= 20 /*&& efnum[1]<=20) || (efnum[1]>=30*/ && efnum[1] <= 40)) ? 1 : 0;
			if (efnum[1] >= 210)
				efnum[2] = 0;
			DrawRotaGraph(X[0], Y, (turn == 1) ? 1.2 : 1.0, 0, Str[0], TRUE, turn);
			DrawRotaGraph(X[1], Y, 1.0, 0, Str[1], TRUE);
			DrawRotaGraph(x[4], Y + 60, 1.0, 0, Str[4], TRUE);
			break;
		case 1002:
			x[4] = (x[4] - 50 >= X[2]) ? x[4] - 50 : X[2];
			rad += PI * 2 / 30;
			if (rad >= 2 * PI)
				efnum[2] = 0;
			DrawRotaGraph(X[0], Y, 1.2, rad, Str[0], TRUE);
			DrawRotaGraph(X[1], Y, 1.0, 0, Str[1], TRUE);
			DrawRotaGraph(x[4], Y + 60, 1.0, 0, Str[4], TRUE);
			break;
		case 1003:
			x[4] = (x[4] - 50 >= X[2]) ? x[4] - 50 : X[2];
			rad -= PI * 2 / 30;
			if (rad <= -2 * PI)
				efnum[2] = 0;
			DrawRotaGraph(X[0], Y, 1.2, rad, Str[0], TRUE);
			DrawRotaGraph(X[1], Y, 1.0, 0, Str[1], TRUE);
			DrawRotaGraph(x[4], Y + 60, 1.0, 0, Str[4], TRUE);
			break;
	}
}
void KeySave() {
	int f = Frame - StartFrame;
	static int t = 0;
	static int oldKey[5];
	if (f == 0)
		t = 0;
	if (f >= 1)
		for (int i = 0; i < 5; i++)
			if ((Key[KEY_INPUT_UP] != oldKey[0] && i == 0) || (Key[KEY_INPUT_DOWN] != oldKey[1] && i == 1) || (Key[KEY_INPUT_LEFT] != oldKey[2] && i == 2) || (Key[KEY_INPUT_RIGHT] != oldKey[3] && i == 3) || (Key[KEY_INPUT_Z] != oldKey[4] && i == 4))
				rep.KeyChangeSave[t][i] = f;
	oldKey[0] = Key[KEY_INPUT_UP];
	oldKey[1] = Key[KEY_INPUT_DOWN];
	oldKey[2] = Key[KEY_INPUT_LEFT];
	oldKey[3] = Key[KEY_INPUT_RIGHT];
	oldKey[4] = Key[KEY_INPUT_Z];
}
void TitleBack(int &y,int &fall,int image) {
	switch (scene) {
		case 1:
			if (Frame - SceneFrame == 1)
				y = -150;
			break;
		case 2:
			y -= fall;
			fall += GRAVITY;
			if (fall >= 100)
				fall = 100;
			if (Count >= 3) {
				if (y <= -2000) {
					y = -2400;
					fall = 0;
					SceneFrame = Frame;
					Count = 0;
					++scene;
				}
			}
			else if (y <= -300) {
				y = -150;
				fall = 50;
			}
			break;
		case 3:
			if (Frame - QuakeFrame >= 5)
				quakeflag++;

			if (quakeflag == 1) {
				shakable = 30;
				QuakeFrame = Frame;
				quakeflag++;
			}
			if (quakeflag == 3) {
				shakable = -30;
				QuakeFrame = Frame;
				++quakeflag;
			}
			if (quakeflag == 5) {
				shakable = 0;
				quakeflag = 0;
				QuakeFrame = 10000;
				if (Count <= 3)
					++Count;
				if (Count == 4)
					++scene;
			}
			break;
		case 5:					//
			break;				//
	}
	if (scene != 5)
		DrawGraph(0, y + shakable, image, FALSE);
}
void TitleLetter(int& x, int& y, int& fall, int& direction, int i, int image) {
	switch(scene) {
		case 0:
			y += fall;
			fall += GRAVITY;
			if (fall >= 30)
				fall = 30;
			if (i == 0 && y >= 900) {
				y = 0;
				fall = 30;
				SceneFrame = Frame;
				++scene;
			}
			break;
		case 2:
			if (i == 0) {
				if (y < 225) {
					y += fall;
					fall += GRAVITY;
					if (fall >= 50)
						fall = 50;
				}
				if (x <= 250) {
					direction = 1;
					++Count;
				}
				if(x>=350) {
					direction = 0;
					++Count;
				}
				if (direction == 0)
					x -= 2;
				if (direction == 1)
					x += 2;
			}
			break;
		case 3:
			if (Frame - SceneFrame == 0) {
				switch(i) {
					case 0:
					case 2:
						x = 100;
						y = -250;
						break;
					case 1:
					case 3:
						x = 250;
						y = -250;
						break;
				}
			}
			if (Frame - SceneFrame >= 40) {
				switch (Count) {
					case 0:
						if (i == 3) {
							y += fall;
							fall += GRAVITY;
							if (y >= 380) {
								y = 380;
								if (quakeflag == 0) {
									quakeflag = 1;
									QuakeFrame = Frame;
								}
							}
						}
						break;
					case 1:
						if (i == 2) {
							y += fall;
							fall += GRAVITY;
							if (y >= 380) {
								y = 380;
								if (quakeflag == 0) {
									quakeflag = 1;
									QuakeFrame = Frame;
								}
							}
						}
						break;
					case 2:
						if (i == 1) {
							y += fall;
							fall += GRAVITY;
							if (y >= 230) {
								y = 230;
								if (quakeflag == 0) {
									quakeflag = 1;
									QuakeFrame = Frame;
								}
							}
						}
						break;
					case 3:
						if (i == 0) {
							y += fall;
							fall += GRAVITY;
							if (y >= 230) {
								y = 230;
								if (quakeflag == 0) {
									quakeflag = 1;
									QuakeFrame = Frame;
								}
							}
						}
						break;
				}
			}
			break;
	}
	if (scene != 1 && scene != 5)
		DrawGraph(x, y + shakable, image, FALSE);
}
void TitleBird(int& x, int& y, int& direction, int& surpriseflag, int i, int img1, int img2, int img3) {
	switch(scene) {
		case 1:
			if (i == 0) {
				if (Frame - SceneFrame <= 60) {
					direction = 0;
					x -= 5;
				}
				else if (Frame - SceneFrame <= 90)
					surpriseflag = 1;
				else if (Frame - SceneFrame <= 120) {
					surpriseflag = 0;
					direction = 1;
					x += 10;
				}
				else if (Frame - SceneFrame <= 150) {
					direction = 0;
					x = 800;
					y = 50;
				}
			}
			if (i == 1) {
				if (Frame - SceneFrame <= 60) {
					direction = 1;
					x += 5;
				}
				else if (Frame - SceneFrame <= 90)
					surpriseflag = 1;
				else if (Frame - SceneFrame <= 120) {
					surpriseflag = 0;
					direction = 0;
					x -= 10;
				}
				else if (Frame - SceneFrame <= 150) {
					direction = 0;
					x = -50;
					y = 150;
					SceneFrame = Frame;
					++scene;
					SF = Frame;
				}
			}
			break;
		case 4:
			if (i == 0) {
				x -= 3;
				if (x <= -300)
					x = 800;
			}
			if (Key[KEY_INPUT_Z] == 1) {
				Count = 0;
				scene = 0;
				mode = MODESELECT;
			}
			break;
	}
	if (scene != 5)
		switch (Frame % 9) {
			case 0:
			case 1:
			case 2:
				if (direction == 0)
					DrawGraph(x, y, img1, TRUE);
				if (direction == 1)
					DrawTurnGraph(x, y, img2, TRUE);
				break;
			case 3:
			case 4:
			case 5:
				if (direction == 0)
					DrawGraph(x, y, img2, TRUE);
				if (direction == 1)
					DrawTurnGraph(x, y, img3, TRUE);
				break;
			case 6:
			case 7:
			case 8:
				if (direction == 0)
					DrawGraph(x, y, img3, TRUE);
				if (direction == 1)
					DrawTurnGraph(x, y, img1, TRUE);
				break;
		}
	if (surpriseflag == 1) {
		if (i == 0)
			DrawGraph(x - 20, y - 50, surprise, TRUE);
		if (i == 1)
			DrawGraph(x + 50, y - 50, surprise, TRUE);
	}
	if (scene == 4 && i == 0) {
		DrawGraph(x + 50, y, speech, TRUE);
	}
	if (scene == 4)
		if (Frame - SF >= 360) {
			SetFontSize(300);
			DrawString(100, WINDOW_HEIGHT - 370, "超", GetColor(255, 128, 0));
			SetFontSize(16);
		}
}