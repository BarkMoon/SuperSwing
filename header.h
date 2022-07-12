#include <string>

#define GRAVITY 7.0
#define PI 3.141592

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define RealX 640000
#define BackX 80000
#define RealY 120000

#define MeterPerDot 280

#define CLOUD 6400

enum{DEMO,TITLE,MODESELECT,STORY,FREEPLAY,WAZACHART,FINF,FIND}mode;
enum{TAKEOFF,FLYING,LAND,NGLAND,UP,DOWN,LEFT,RIGHT,FSPIN,BFLIP,KICKUP,HEELUP,HOP,DOUBLEF,SUPERF,SPINDU,DOUBLEB,SUPERB,SUPER,HYPER,BACK,TURNF,SUPERGO,DASH,NEGA,FUNBARI,FUNLU,DROP,DRHOP,SUPERDR,END};

int Frame;
int StartFrame;
int phase;
//int Count;				//展示用
//int Total;

int viewmode;
const int Overview[2][2]={{520,760},{390,570}};
const int MeterXY[2][2]={{0,0},{250,80}};
const int ResSize=170;
const char Eval[5][50]={"ナイスプレイ！","光るモノあり","やりますねぇ！","しゅんごーい ^q^","神"};
const char Sel[3][50]={"もう一度遊ぶ","モードセレクトに戻る","タイトルに戻る"};
const char MoSel[2][500]={"ブランコに乗る","ワザ・チャート"};
const char Com[END][50]={"","","","","↑","↓","←","→","←↓→↑","↑→↓←","←↑","↓→","↑↑↓↓","←↓→↑←↓→↑","←↓→↑←↓→↑←↓→↑","←↓→↑↓↑","↑→↓←↑→↓←","↑→↓←↑→↓←↑→↓←","↑↑↓↓←→←→","↑↑↓↓←→←→↓→","←→","←→←","←←←←←","→←","→→→→→→","↑↑","↓→↓→","↓↓","↓↓↑↑","↓↓↓↓↓"};
int choice;
int WX,WY;
int NowTime;

int Cb;

int Keylog[15];
int Keylock;

typedef struct{
	int img[20];
	}CData;

typedef struct{
	int img[2];
	int chain;
	}BData;

typedef struct{
	int Cimgnum;
	int Bimgnum;
	int state;
	double rad,rotA,rotV;
	int direction;
	int x,y;
	std::string Metx, Mety;
	double vx,vy,ax,ay;
	double aday;
	int m;
	int cooltime,motiontime;
	int command;
	}Player;

typedef struct{
	int rot;
	int spd;
	}Branko;

typedef struct{
	int x,y,type;
	double ext;
	}Cloud;

typedef struct{
	Cloud Csave;
	int KeyChangeSave[1000][5];
	}Replay;

typedef struct{
	int WazaSave[END];
	int HighScore[10];
	Replay Rep[10];
	}Save;

typedef struct{
	int x,y,img[3],fall,direction,surpriseflag;
	}object;

object Titleback;
object Bird[2];
object letter[4];
int surprise;
int speech;
int shakable;	//揺れるエフェクト用
int quakeflag;
int QuakeFrame;
int scene;
int SceneFrame;
int Count;
int SF;
//int thx;

int Total;

int BlackBack;
int ResBack;
int SelBack[2];
int logo[2];

int bg;
int blow[50];
int man;
int climg[4];
int Effects[END];
int Str[5];
int Meter[2];
int Star;
int Arr;
int Font;
int Ques;

int Wimg[END];
int W[END][2];
char Wstr[END][10][100];
int Wflag[END];

int Wnum[END];

int efnum[3];
int quake[2];
BData Bra[5];
CData Cha[10];
Replay rep;
Save sav;

int Unlock[END];

void ImgSet();
void Set(Player*,Cloud[]);
void Demo();
void Title();
void ModeSelect(Player*,Cloud[]);
void Story();
void FreePlay(Player*,Cloud[]);
void WazaChart(Player*,Cloud[]);
void Logo(int&);
int WazaCheck(int,int);
void Waza(Player*);
void DrawPlayerWithEffect(int,int,Player*);
void Activated(int);
void KeySave();

void TitleBack(int&,int&,int);
void TitleLetter(int&,int&,int&,int&,int,int);
void TitleBird(int&,int&,int&,int&,int,int,int,int);