#include<iostream>
#include<easyx.h>
#include<conio.h>
#include<time.h>
#include<cstring>
#include<fstream>
#include<stdlib.h>
#include<algorithm>
#include<mmsystem.h>//������ý���豸�ӿ��ļ�
#pragma comment(lib,"winmm.lib") //���ؾ�̬��
using namespace std;
const int WIDTH = 591;
const int HEIGHT = 864;
const int BULLET_NUM = 15;
const int ENEMY_NUM = 5;
const int ENEMY_BULLT_NUM_1 = 15;
const int N = 1e5 + 10;
//����ͼ
IMAGE bk1,bk2,img_op,img_worker;
//���
IMAGE img_role[2];
//�ӵ�
IMAGE img_bullet[2];
//�л��ӵ�
IMAGE img_enemy_bullet[2];
//�л�
IMAGE img_enemy[3][2];
//�л���ը
IMAGE img_enemy_boom[3][5][2];
//��ҷɻ���ը
IMAGE img_player_boom[4][2];
//logo
IMAGE img_log[2][2];
//��Ϸ��������
IMAGE img_game_over;
//����
IMAGE img_life[2];
//���ӿ�Ͷ
IMAGE img_aridrop[3][2];
//��Ϸ����
static int score,getaridrop;
static char str_scoer[N];
static char str_best_scoer[N];
static bool game_over = false;
bool Timer(int ms, int id)
{
	static DWORD t[100];
	if (clock() - t[id] > ms)
	{
		t[id] = clock();
		return true;
	}
	return false;
}
bool EnemyTimer(int ms, int id)
{
	static DWORD t1[100];
	if (clock() - t1[id] > ms)
	{
		t1[id] = clock();
		return true;
	}
	return false;
}
bool playerTimer(int ms, int id)
{
	static DWORD t2[100];
	if (clock() - t2[id] > ms)
	{
		t2[id] = clock();
		return true;
	}
	return false;
}
class Aircraft//�ɻ���һ����
{
public:
	int x = 0;
	int y = 0;
	int speed = 0;
	int w = 0;
	int h = 0;
	int hp = 0;
	int type = 0;
	bool live = false;
	int boom_step = 0;
	int boom_id = 0;
	bool boom = false;
	int life = 0;
	bool shoot=false;

	bool live_r = false;
	bool live_l = false;
	int x_l = 0;
	int x_r = 0;
	int y_l = 0;
	int y_r = 0;

	virtual void move() = 0;



};
class Player:public Aircraft
{
public:
	bool leve = false;;
	bool add_life = false;
	Player(int xx=0, int yy=0,int ss=5,int ww=0,int hh=0,bool ll=false)
	{
		x = xx;
		y = yy;
		w = ww;
		h = hh;
		speed=ss;
		live = ll;
		boom = false;
		boom_step = 4;
		boom_id = 0;
		leve = false;
	}
	void move()
	{
		if (Timer(1,4)&&live)
		{
			if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W') || GetAsyncKeyState('w'))//����б����
				if (y > 0)
					y -= speed;
			if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S') || GetAsyncKeyState('s'))
				if (y + 95 < HEIGHT)
					y += speed;
			if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A') || GetAsyncKeyState('a'))
				if (x > -45)
					x -= speed;
			if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D') || GetAsyncKeyState('d'))
				if (x + 60 < WIDTH)
					x += speed;
		}
	}
	void Iniplayer()
	{
		boom = false;
		boom_step = 4;
		boom_id = 0;
		live = true;
		x= WIDTH / 2;
		y = HEIGHT - 120;
		life = 2;
		add_life = false;
		leve = 0;
	}
	void revive()
	{
		boom = false;
		boom_step = 4;
		boom_id = 0;
		live = true;
		leve = 0;
		add_life = false;
	}
};
Player player(WIDTH / 2, HEIGHT - 120, 1,98,123, true);
class Bullet : public Aircraft 
{
public:
	Bullet(int xx = 0, int yy = 0, int ss = 10, bool ll = false)
	{
		x = xx;
		y = yy;
		speed = ss;
		live = ll;
		live_l = false;
		live_r = false;
	}
	void move()
	{
		if (live)
		{
			y -= speed;
			if (y < -20)
			{
				live = false;
			}
		}
	}
	void movel()
	{
		if (live_l)
		{
			y_l -= speed;
			if (y_l < -20)
			{
				live_l = false;
			}
		}
	}
	void mover()
	{
		if (live_r)
		{
			y_r -= speed;
			if (y_r < -20)
			{
				live_r = false;
			}
		}
	}
	void creatBullet()
	{
		x = player.x + player.w/2;
		y = player.y;
		live = true;
	}
	void creatBulletl()
	{
		x_l = player.x+10;
		y_l = player.y;
		live_l = true;
	}
	void creatBulletr()
	{
		x_r = player.x+player.w - 10;
		y_r = player.y;
		live_r = true;
	}
	void init()
	{
		live = false;
		live_l = false;
		live_r = false;
	}
};
Bullet bullet[BULLET_NUM];
class Enemy :public Aircraft
{
public:
	Enemy(int xx = 0, int yy = 0, bool ll = false)
	{
		x = xx;
		y = yy;
		live = ll;
	}
	void move()
	{
		if (live)
		{
			y += speed;
			if (y > HEIGHT)
				live = false;
		}
	}
	void enemy_hp()
	{
		int seed = rand() %10 ;
		if (seed == 0)
		{
			hp = 5;
			w = 155;
			h = 173;
			type = 2;
			speed = 1;
			boom_step = 5;
			boom = false;
			shoot = true;
		}
		else if(seed > 0 && seed < 3)
		{
			hp = 2;
			w = 69;
			h = 88;
			type = 1;
			speed = 3;
			boom_step = 4;
			boom = false;
			shoot = true;
		}
		else
		{
			hp = 1;
			w = 52;
			h = 39;
			type = 0;
			speed = 5;
			boom_step = 4;
			boom = false;
			shoot = false;
		}
	}
	void creat()
	{
		if (!live)
		{
			live = true;
			enemy_hp();
			x = rand()%(WIDTH-60);
			y = 0;
		}
	}
	void Init()
	{ 
		live = false;
		live_l = false;
		live_r = false;
	}
};
Enemy enemy[ENEMY_NUM];
class EnemyBullet :public Aircraft
{
public:
	EnemyBullet(int xx = 0, int yy = 0, int ss = 6, bool ll = false)
	{
		x = xx;
		y = yy;
		speed = ss;
		live = ll;
		live_r = false;
		live_l = false;
		x_l = 0;
		x_r = 0;
		y_l = 0;
		y_r = 0;
	}
	void move()
	{
		if (live)
		{
			y += speed;
			if (y > HEIGHT)
			{
				live = false;
			}
		}
	}
	void movel()
	{
		if (live_l)
		{
			y_l += speed;
			x_l -= (speed-2);
			if (y_l > HEIGHT||x_l<0)
			{
				live_l = false;
			}
		}
	}
	void mover()
	{
		if (live_r)
		{
			y_r += speed;
			x_r += (speed-2);
			if (y_r > HEIGHT || x_r > WIDTH)
			{
				live_r = false;
			}
		}
	}
	void creatBullet(int xx,int yy,int ww,int hh)
	{
		x = xx + ww / 2;
		y = yy+hh;
		live = true;
	}

	void creatBulletl(int xx, int yy, int ww, int hh)
	{
		x_l = xx + 10;
		y_l = yy + hh;
		live_l = true;
	}
	void creatBulletr(int xx, int yy, int ww, int hh)
	{
		x_r = xx + ww-10;
		y_r = yy + hh;
		live_r = true;
	}
};
EnemyBullet enemybullet[ENEMY_NUM][ENEMY_BULLT_NUM_1];
class Backgrond :public Aircraft
{
public:
	Backgrond(int yy=0)
	{
		x = 0;
		y = yy;
		speed = 1;
	}
	void move()
	{
		y += speed;
	}
};
Backgrond back1,back2(-HEIGHT);
class Airdrop:public Aircraft
{
public:
	Airdrop()
	{
		x = 0;
		y = 0;
		live = false;
	}
	void creat()
	{
		int seed = rand() % 2;
		x = rand() % (WIDTH - 60);
		y = 0;
		live = true;
		speed = 6;
		if (seed == 0)
		{

			w = 50;
			h = 40;
			type = 0;
		}
		else
		{
			w = 58;
			h = 87;
			type = 1;
		}
	}
	void move()
	{
		if (live)
		{
			y += speed;
			if (y > HEIGHT)
				live = false;
		}
	}
	void init()
	{
		live = false;
	}
};
Airdrop airdrop;
class Button
{
public:
	int x;
	int y;
	int w;
	int h;
	int r;
	int g;
	int b;
	int size;
	int tr = 0;;
	int tg = 0;;
	int tb = 0;
	Button(int xx,int yy,int ww,int hh,int ss,int rr=0,int gg=0,int bb=0)
	{
		x = xx;
		y = yy;
		w = ww;
		h = hh;
		size = ss;
		r = rr;
		g = gg;
		b = bb;
	}
	void button(const char* text)
	{
		setbkmode(TRANSPARENT);
		setfillcolor(RGB(r, g, b));
		setlinestyle(PS_SOLID, 2);
		setlinecolor(RGB(0,0,0));
		roundrect(x, y, x + w, y + h,50,50);
		fillroundrect(x, y, x + w, y + h, 50, 50);
		settextstyle(size, 0, "����");
		settextcolor(RGB(tr, tg, tb));
		char arr[50];
		strcpy(arr, text);
		int height = textheight(arr);
		int width = textwidth(arr);
		outtextxy(x + (w - width) / 2, y + (h - height) / 2, arr);
	}
	void textcolor(int rr = 0, int gg = 0, int bb = 0)
	{
		tr = rr;
		tg = gg;
		tb = bb;
	}
};
bool MouseOperat(int x,int y,int w,int h, ExMessage msg)
{

	if (msg.message == WM_LBUTTONDOWN)
	{
		if (msg.x > x && msg.x<x + w && msg.y>y && msg.y < y + h)
		{
			mciSendString("close BGM_button", 0, 0, 0);
			mciSendString("open ./music//button.mp3 alias BGM_button", 0, 0, 0);
			mciSendString("play BGM_button", 0, 0, 0);
			return true;
		}
	}
	return false;
}
void LoadImg()
{
	//���ر���ͼ
	loadimage(&bk1, "./images//background.jpg");
	loadimage(&bk2, "./images//background.jpg");
	//����log
	loadimage(&img_log[0][0], "./images//zw_1.png");
	loadimage(&img_log[0][1], "./images//zw_2.png");
	loadimage(&img_log[1][0], "./images//yw_1.png");
	loadimage(&img_log[1][1], "./images//yw_2.png");
	//���ز���˵��
	loadimage(&img_op, "./images//Operating_instructions.jpg", WIDTH, HEIGHT);
	//����������Ա
	loadimage(&img_worker, "./images//Production_staff.jpg", WIDTH, HEIGHT);
	//�������ͼƬ
	loadimage(&img_life[0], "./images//life1.png");
	loadimage(&img_life[1], "./images//life2.png");
	//�������ͼƬ
	loadimage(&img_role[0], "./images//me1_1.png");
	loadimage(&img_role[1], "./images//me1_2.png");
	//�����ӵ�ͼƬ
	loadimage(&img_bullet[0], "./images//bullet1.jpg");
	loadimage(&img_bullet[1], "./images//bullet2.jpg");
	//���ص����ӵ�ͼƬ
	loadimage(&img_enemy_bullet[0], "./images//bullet2_1.png");
	loadimage(&img_enemy_bullet[1], "./images//bullet2_2.png");
	//���ز���ͼƬ
	loadimage(&img_aridrop[0][0], "./images//cure1.png");
	loadimage(&img_aridrop[0][1], "./images//cure2.png");
	loadimage(&img_aridrop[1][0], "./images//bullet_supply1.png");
	loadimage(&img_aridrop[1][1], "./images//bullet_supply2.png");
	//���صл�ͼƬ
	loadimage(&img_enemy[0][0], "./images//enemy1_1.png");
	loadimage(&img_enemy[0][1], "./images//enemy1_2.png");
	loadimage(&img_enemy[1][0], "./images//enemy2_1.png");
	loadimage(&img_enemy[1][1], "./images//enemy2_2.png");
	loadimage(&img_enemy[2][0], "./images//ememy3_1.png");
	loadimage(&img_enemy[2][1], "./images//ememy3_2.png");
	//���صл���ըͼƬ
	//1�ŷɻ�
	loadimage(&img_enemy_boom[0][0][0], "./images//enemy1_down1_1.png");
	loadimage(&img_enemy_boom[0][0][1], "./images//enemy1_down1_2.png");
	loadimage(&img_enemy_boom[0][1][0], "./images//enemy1_down2_1.png");
	loadimage(&img_enemy_boom[0][1][1], "./images//enemy1_down2_2.png");
	loadimage(&img_enemy_boom[0][2][0], "./images//enemy1_down3_1.png");
	loadimage(&img_enemy_boom[0][2][1], "./images//enemy1_down3_2.png");
	loadimage(&img_enemy_boom[0][3][0], "./images//enemy1_down4_1.png");
	loadimage(&img_enemy_boom[0][3][1], "./images//enemy1_down4_2.png");
	//2�ŷɻ�
	loadimage(&img_enemy_boom[1][0][0], "./images//enemy2_down1_1.png");
	loadimage(&img_enemy_boom[1][0][1], "./images//enemy2_down1_2.png");
	loadimage(&img_enemy_boom[1][1][0], "./images//enemy2_down2_1.png");
	loadimage(&img_enemy_boom[1][1][1], "./images//enemy2_down2_2.png");
	loadimage(&img_enemy_boom[1][2][0], "./images//enemy2_down3_1.png");
	loadimage(&img_enemy_boom[1][2][1], "./images//enemy2_down3_2.png");
	loadimage(&img_enemy_boom[1][3][0], "./images//enemy2_down4_1.png");
	loadimage(&img_enemy_boom[1][3][1], "./images//enemy2_down4_2.png");
	//3�Ż�
	loadimage(&img_enemy_boom[2][0][0], "./images//enemy3_down1_1.png");
	loadimage(&img_enemy_boom[2][0][1], "./images//enemy3_down1_2.png");
	loadimage(&img_enemy_boom[2][1][0], "./images//enemy3_down2_1.png");
	loadimage(&img_enemy_boom[2][1][1], "./images//enemy3_down2_2.png");
	loadimage(&img_enemy_boom[2][2][0], "./images//enemy3_down3_1.png");
	loadimage(&img_enemy_boom[2][2][1], "./images//enemy3_down3_2.png");
	loadimage(&img_enemy_boom[2][3][0], "./images//enemy3_down4_1.png");
	loadimage(&img_enemy_boom[2][3][1], "./images//enemy3_down4_2.png");
	loadimage(&img_enemy_boom[2][4][0], "./images//enemy3_down5_1.png");
	loadimage(&img_enemy_boom[2][4][1], "./images//enemy3_down5_2.png");
	//��ҷɻ���ը
	loadimage(&img_player_boom[0][0], "./images//me_destroy_1_1.png");
	loadimage(&img_player_boom[0][1], "./images//me_destroy_1_2.png");
	loadimage(&img_player_boom[1][0], "./images//me_destroy_2_1.png");
	loadimage(&img_player_boom[1][1], "./images//me_destroy_2_2.png");
	loadimage(&img_player_boom[2][0], "./images//me_destroy_3_1.png");
	loadimage(&img_player_boom[2][1], "./images//me_destroy_3_2.png");
	loadimage(&img_player_boom[3][0], "./images//me_destroy_4_1.png");
	loadimage(&img_player_boom[3][1], "./images//me_destroy_4_2.png");
	//��Ϸ����
	loadimage(&img_game_over, "./images//GameOver.png");

}
void gamedraw()
{
	putimage(0, back1.y, &bk1);
	putimage(0, back2.y, &bk2);
	if (player.live)
	{
		putimage(player.x, player.y, &img_role[0], NOTSRCERASE);
		putimage(player.x, player.y, &img_role[1], SRCINVERT);
	}
	if (airdrop.live)
	{
		putimage(airdrop.x,airdrop.y, &img_aridrop[airdrop.type][0], NOTSRCERASE);
		putimage(airdrop.x, airdrop.y, &img_aridrop[airdrop.type][1], SRCINVERT);
	}
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].live)
		{
			putimage(bullet[i].x, bullet[i].y, &img_bullet[0], NOTSRCERASE);
			putimage(bullet[i].x, bullet[i].y, &img_bullet[1], SRCINVERT);
		}
		if (bullet[i].live_l)
		{
			putimage(bullet[i].x_l, bullet[i].y_l, &img_bullet[0], NOTSRCERASE);
			putimage(bullet[i].x_l, bullet[i].y_l, &img_bullet[1], SRCINVERT);
		}
		if (bullet[i].live_r)
		{
			putimage(bullet[i].x_r, bullet[i].y_r, &img_bullet[0], NOTSRCERASE);
			putimage(bullet[i].x_r, bullet[i].y_r, &img_bullet[1], SRCINVERT);
		}
	}
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live&&(!enemy[i].boom))
		{
			putimage(enemy[i].x, enemy[i].y, &img_enemy[enemy[i].type][0], NOTSRCERASE);
			putimage(enemy[i].x, enemy[i].y, &img_enemy[enemy[i].type][1], SRCINVERT);
		}
		if (enemy[i].boom)
		{
			if (enemy[i].boom_id >= enemy[i].boom_step)
			{
				enemy[i].boom_id = 0;
				enemy[i].boom = false;
			}
			putimage(enemy[i].x, enemy[i].y, &img_enemy_boom[enemy[i].type][enemy[i].boom_id][0], NOTSRCERASE);
			putimage(enemy[i].x, enemy[i].y, &img_enemy_boom[enemy[i].type][enemy[i].boom_id][1], SRCINVERT);
			if (Timer(100, 6)) enemy[i].boom_id++;
		}

	}
	for (int i = 0; i < ENEMY_NUM; i++)
		for (int j = 0; j < ENEMY_BULLT_NUM_1; j++)
		{
			if (enemybullet[i][j].live)
			{
				putimage(enemybullet[i][j].x, enemybullet[i][j].y, &img_enemy_bullet[0], NOTSRCERASE);
				putimage(enemybullet[i][j].x, enemybullet[i][j].y, &img_enemy_bullet[1], SRCINVERT);
			}

			if (enemybullet[i][j].live_l)
			{
				putimage(enemybullet[i][j].x_l, enemybullet[i][j].y_l, &img_enemy_bullet[0], NOTSRCERASE);
				putimage(enemybullet[i][j].x_l, enemybullet[i][j].y_l, &img_enemy_bullet[1], SRCINVERT);
			}

			if (enemybullet[i][j].live_r)
			{
				putimage(enemybullet[i][j].x_r, enemybullet[i][j].y_r, &img_enemy_bullet[0], NOTSRCERASE);
				putimage(enemybullet[i][j].x_r, enemybullet[i][j].y_r, &img_enemy_bullet[1], SRCINVERT);
			}
		}
	if (!player.live && player.boom)
	{
		if (!player.boom_id)
		{
			mciSendString("close BGM_player_boom", 0, 0, 0);
			mciSendString("open ./music//player_boom.mp3 alias BGM_player_boom", 0, 0, 0);
			mciSendString("play BGM_player_boom", 0, 0, 0);
		}
		if (player.boom_id >= player.boom_step)
		{
			player.boom_id = 0;
			player.life--;
			if (player.life >= 0) player.revive();
			else
			{
				game_over = true;
			}
		}
		putimage(player.x, player.y, &img_player_boom[player.boom_id][0], NOTSRCERASE);
		putimage(player.x, player.y, &img_player_boom[player.boom_id][1], SRCINVERT);
		if (Timer(200, 7)) player.boom_id++;
	}
	for (int i = 1; i <= player.life; i++)
	{
		putimage(506-(i-1)*50, 29, &img_life[0], NOTSRCERASE);
		putimage(506-(i-1)*50, 29, &img_life[1], SRCINVERT);
	}
}
void InitGame()
{
	LoadImg();
}
bool tip(const char *t1,const char *t2)
{
	HWND hnd = GetHWnd();
	int is_ok = MessageBox(hnd, t1 ,t2, MB_OKCANCEL);
	if (is_ok == IDOK)
		return true;
	if (is_ok == IDCANCEL)
		return false;
}
void OpShow()
{
	BeginBatchDraw();
	putimage(0, 0, &img_op);
	Button next(WIDTH-200, HEIGHT-100,120,50,35,255,255,255);
	next.button("����");
	FlushBatchDraw();
	EndBatchDraw();
	ExMessage msg;
	while (true)
	{
		getmessage(&msg, EM_MOUSE);
		if (MouseOperat(next.x, next.y, next.w, next.h, msg))
			return;
	}
}
void WorkerShow()
{
	BeginBatchDraw();
	putimage(0, 0, &img_worker);
	Button next(WIDTH - 200, HEIGHT - 100, 120, 50, 40, 255, 255, 255);
	next.button("����");
	FlushBatchDraw();
	EndBatchDraw();
	ExMessage msg;
	while (true)
	{
		getmessage(&msg, EM_MOUSE);
		if (MouseOperat(next.x, next.y, next.w, next.h, msg))
			return;
	}
}
void menu()
{
	mciSendString("open ./music//menu.mp3 alias BGM", 0, 0, 0);//���ű�������
	mciSendString("play BGM repeat", 0, 0, 0);
	
	
	while (true)
	{
		BeginBatchDraw();
		putimage(0, 0, &bk1);//���ñ���ͼ
		putimage(WIDTH / 2 - 150, 128, &img_log[0][0], NOTSRCERASE);//������Ϸ��logo
		putimage(WIDTH / 2 - 150, 128, &img_log[0][1], SRCINVERT);
		putimage(WIDTH / 2 - 225, 185, &img_log[1][0], NOTSRCERASE);
		putimage(WIDTH / 2 - 225, 185, &img_log[1][1], SRCINVERT);
		Button start_button(WIDTH / 2 - 120, HEIGHT * 3 / 5 - 70, 250, 50, 40, 255, 255, 255);//���尴ť��һ������
		start_button.textcolor(0, 0, 0);//���ð�ť��ɫ
		Button op_button(WIDTH / 2 - 120, HEIGHT * 3 / 5, 250, 50, 40, 255, 255, 255);
		op_button.textcolor(0, 0, 0);
		Button worker_button(WIDTH / 2 - 120, HEIGHT * 3 / 5 + 70, 250, 50, 40, 255, 255, 255);
		worker_button.textcolor(0, 0, 0);
		Button exit_button(WIDTH / 2 - 120, HEIGHT * 3 / 5 + 140, 250, 50, 40, 255, 255, 255);
		exit_button.textcolor(0, 0, 0);
		start_button.button("��ʼ��Ϸ");//���ð�Ť�е�����
		op_button.button("����˵��");
		worker_button.button("������Ա");
		exit_button.button("������Ϸ");
		FlushBatchDraw();
		EndBatchDraw();
		ExMessage msg;//�����Ϣ
		bool flag1 = false;
		bool flag2 = false;
		//�ȴ��û��������Ϣ
		while (true)
		{
			getmessage(&msg, EM_MOUSE);
			if (MouseOperat(start_button.x, start_button.y, start_button.w, start_button.h, msg))
			{
				mciSendString("close BGM", 0, 0, 0);
				return;
			}
			if (MouseOperat(op_button.x, op_button.y, op_button.w, op_button.h, msg))
			{
				OpShow();
				flag1 = true;
				break;
			}
			if (MouseOperat(worker_button.x, worker_button.y, worker_button.w, worker_button.h, msg))
			{
				WorkerShow();
				flag1 = true;
				break;
			}
			if (MouseOperat(exit_button.x, exit_button.y, exit_button.w, exit_button.h, msg))
			{
				if (tip("ȷ���˳���", "��ʾ"))
				{
					exit(0);
				}
				else
				{
					msg.x = 0, msg.y = 0;
					continue;
				}
			}
		}
		if (flag1||flag2) continue;

	}
}
void moveback()
{
	if (back1.y > HEIGHT)
		back1.y = -HEIGHT;
	if(back2.y > HEIGHT)
		back2.y = -HEIGHT;
	if (Timer(5,5))
	{
		back1.move();
		back2.move();
	}
}
void AirdropPut()
{
	if (getaridrop < score / 50&& !airdrop.live)
	{
		getaridrop++;
		airdrop.creat();
	}
	if (airdrop.live && Timer(20, 99))
	{
		airdrop.move();
	}
}
void BulletPUT()
{
	if (GetAsyncKeyState(VK_SPACE)&&Timer(200,3)&&player.live)
	{
		mciSendString("close BGM_shoot", 0, 0, 0);
		mciSendString("open ./music//shoot.mp3 alias BGM_shoot", 0, 0, 0);
		mciSendString("play BGM_shoot", 0, 0, 0);
		for (int i = 0; i < BULLET_NUM; i++)
		{
			if (!bullet[i].live)
			{
				bullet[i].creatBullet();
				if (!bullet[i].live_l && player.leve)
					bullet[i].creatBulletl();
				if (!bullet[i].live_r && player.leve)
					bullet[i].creatBulletr();
				break;
			}

		}
	}
	
	if (playerTimer(10, 1))
	{
		for (int i = 0; i < BULLET_NUM; i++)
		{
			if (bullet[i].live)
				bullet[i].move();
			if (bullet[i].live_l)
				bullet[i].movel();
			if (bullet[i].live_r)
				bullet[i].mover();
		}

	}

}
void EnemyPut()
{
	if (Timer(100,1))
	{
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (!enemy[i].live&&!enemy[i].boom)
			{
				enemy[i].creat();
				break;
			}
		}
	}

	if (Timer(20, 2))
	{
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (enemy[i].live)
				enemy[i].move();
		}
	}

}
void EnemyBulletPUT()
{
	for (int i = 0; i < ENEMY_NUM;i++)
	{
		if (EnemyTimer(2000, i) && enemy[i].type && enemy[i].live)
		{
			for (int j = 0; j < ENEMY_BULLT_NUM_1; j++)
			{
				if (!enemybullet[i][j].live)
				{
					if (!enemybullet[i][j].live_l && enemy[i].type == 2)
						enemybullet[i][j].creatBulletl(enemy[i].x, enemy[i].y, enemy[i].w, enemy[i].h);

					if (!enemybullet[i][j].live_r && enemy[i].type == 2)
						enemybullet[i][j].creatBulletr(enemy[i].x, enemy[i].y, enemy[i].w, enemy[i].h);

					enemybullet[i][j].creatBullet(enemy[i].x, enemy[i].y, enemy[i].w, enemy[i].h);

					break;
				}
			}
		}
	}
	for (int i = 0; i < ENEMY_NUM; i++)
		if(EnemyTimer(20, ENEMY_NUM + i + 1))
			for (int j = 0; j < ENEMY_BULLT_NUM_1; j++)
			{
				if (enemybullet[i][j].live)
					enemybullet[i][j].move();

				if (enemybullet[i][j].live_l)
					enemybullet[i][j].movel();

				if (enemybullet[i][j].live_r)
					enemybullet[i][j].mover();
			}

}
bool CollideJudge(int x,int y,int xx,int yy,int ww,int  hh)
{
	if (x > xx && x<xx + ww && y>yy && y < yy + hh)
	{
		return true;
		
	}
	return false;
}
void DestroyJudge()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		//�������ӵ���ײ
		if (!enemy[i].live) continue;
		for (int k = 0; k < BULLET_NUM; k++)
		{
	
			if (CollideJudge(bullet[k].x, bullet[k].y, enemy[i].x, enemy[i].y, enemy[i].w, enemy[i].h)&&bullet[k].live)
			{
				enemy[i].hp--;
				bullet[k].live = false;
				if (enemy[i].hp <= 0)
				{
					mciSendString("close BGM_explode", 0, 0, 0);
					mciSendString("open ./music//explode.mp3 alias BGM_explode", 0, 0, 0);
					mciSendString("play BGM_explode", 0, 0, 0);
					enemy[i].live = false;
					enemy[i].boom = true;
					if (enemy[i].type == 2) score += 8;
					else if (enemy[i].type == 1) score += 5;
					else score += 1;
				}
			}
			if (CollideJudge(bullet[k].x_l, bullet[k].y_l, enemy[i].x, enemy[i].y, enemy[i].w, enemy[i].h) && bullet[k].live_l)
			{
				enemy[i].hp--;
				bullet[k].live_l = false;
				if (enemy[i].hp <= 0)
				{
					mciSendString("close BGM_explode", 0, 0, 0);
					mciSendString("open ./music//explode.mp3 alias BGM_explode", 0, 0, 0);
					mciSendString("play BGM_explode", 0, 0, 0);
					enemy[i].live = false;
					enemy[i].boom = true;
					if (enemy[i].type == 2) score += 8;
					else if (enemy[i].type == 1) score += 5;
					else score += 1;
				}
			}
			if (CollideJudge(bullet[k].x_r, bullet[k].y_r, enemy[i].x, enemy[i].y, enemy[i].w, enemy[i].h) && bullet[k].live_r)
			{
				enemy[i].hp--;
				bullet[k].live_r = false;
				if (enemy[i].hp <= 0)
				{
					mciSendString("close BGM_explode", 0, 0, 0);
					mciSendString("open ./music//explode.mp3 alias BGM_explode", 0, 0, 0);
					mciSendString("play BGM_explode", 0, 0, 0);
					enemy[i].live = false;
					enemy[i].boom = true;
					if (enemy[i].type == 2) score += 8;
					else if (enemy[i].type == 1) score += 5;
					else score += 1;
				}
			}
		}	
		
		//����������ײ���
		if (CollideJudge(player.x, player.y + 30, enemy[i].x, enemy[i].y, enemy[i].w, enemy[i].h) && player.live)
		{

			player.live = false;
			player.boom = true;
			enemy[i].live = false;
		}

		if (CollideJudge(player.x, player.y + player.h - 50, enemy[i].x, enemy[i].y, enemy[i].w, enemy[i].h) && player.live)
		{
			player.live = false;
			player.boom = true;
			enemy[i].live = false;
		}
		if (CollideJudge(player.x, player.y + player.h / 2, enemy[i].x, enemy[i].y, enemy[i].w, enemy[i].h) && player.live)
		{
			player.live = false;
			player.boom = true;
			enemy[i].live = false;
		}

		if (CollideJudge(player.x + player.w - 30, player.y, enemy[i].x, enemy[i].y, enemy[i].w, enemy[i].h) && player.live)
		{
			player.live = false;
			player.boom = true;
			enemy[i].live = false;
		}
		if (CollideJudge(player.x + player.w / 2, player.y, enemy[i].x, enemy[i].y, enemy[i].w, enemy[i].h) && player.live)
		{
			player.live = false;
			player.boom = true;
			enemy[i].live = false;
		}

		if (CollideJudge(player.x + player.w, player.y + player.h - 50, enemy[i].x, enemy[i].y, enemy[i].w, enemy[i].h) && player.live)
		{
			player.live = false;
			player.boom = true;
			enemy[i].live = false;
		}
		if (CollideJudge(player.x + player.w / 2, player.y + player.h / 2, enemy[i].x, enemy[i].y, enemy[i].w, enemy[i].h) && player.live)
		{
			player.live = false;
			player.boom = true;
			enemy[i].live = false;
		}

	}
	//������ӵ���ײ
	if (player.live)
	{
		for (int i = 0; i < ENEMY_NUM; i++)
			for (int j = 0; j < ENEMY_BULLT_NUM_1; j++)
			{
				if (enemybullet[i][j].live)
					if (CollideJudge(enemybullet[i][j].x, enemybullet[i][j].y, player.x, player.y, player.w, player.h))
					{
						player.live = false;
						player.boom = true;
						enemybullet[i][j].live = false;
					}
				if (enemybullet[i][j].live_l)
					if (CollideJudge(enemybullet[i][j].x_l, enemybullet[i][j].y_l, player.x, player.y, player.w, player.h))
					{
						player.live = false;
						player.boom = true;
						enemybullet[i][j].live_l = false;
					}
				if (enemybullet[i][j].live_r)
					if (CollideJudge(enemybullet[i][j].x_r, enemybullet[i][j].y_r, player.x, player.y, player.w, player.h))
					{
						player.live = false;
						player.boom = true;
						enemybullet[i][j].live_r = false;
					}
			}
	}
	//������Ͷ��ײ���

	if (airdrop.live&&player.live)
	{

		if (CollideJudge(player.x, player.y + 30, airdrop.x, airdrop.y, airdrop.w, airdrop.h))
		{
			mciSendString("close BGM_getbullet", 0, 0, 0);
			mciSendString("open ./music//getbullet.mp3 alias BGM_getbullet", 0, 0, 0);
			mciSendString("play BGM_getbullet", 0, 0, 0);
			airdrop.live = false;
			if (airdrop.type == 0)
			{
				player.add_life = true;
				return;
			}
			else if (airdrop.type == 1)
			{
				player.leve = true;
				return;
			}
		}
		if (CollideJudge(player.x, player.y + player.h - 50, airdrop.x, airdrop.y, airdrop.w, airdrop.h) )
		{
			mciSendString("close BGM_getbullet", 0, 0, 0);
			mciSendString("open ./music//getbullet.mp3 alias BGM_getbullet", 0, 0, 0);
			mciSendString("play BGM_getbullet", 0, 0, 0);
			airdrop.live = false;
			if (airdrop.type == 0)
			{
				player.add_life = true;
				return;
			}
			else if (airdrop.type == 1)
			{
				player.add_life = true;
				return;
			}
		}
		if (CollideJudge(player.x, player.y + player.h / 2, airdrop.x, airdrop.y, airdrop.w, airdrop.h))
		{
			mciSendString("close BGM_getbullet", 0, 0, 0);
			mciSendString("open ./music//getbullet.mp3 alias BGM_getbullet", 0, 0, 0);
			mciSendString("play BGM_getbullet", 0, 0, 0);
			airdrop.live = false;
			if (airdrop.type == 0)
			{
				player.add_life = true;
				return;
			}
			else if (airdrop.type == 1)
			{
				player.add_life = true;
				return;
			}
		}

		if (CollideJudge(player.x + player.w - 30, player.y, airdrop.x, airdrop.y, airdrop.w, airdrop.h))
		{
			mciSendString("close BGM_getbullet", 0, 0, 0);
			mciSendString("open ./music//getbullet.mp3 alias BGM_getbullet", 0, 0, 0);
			mciSendString("play BGM_getbullet", 0, 0, 0);
			airdrop.live = false;
			if (airdrop.type == 0)
			{
				player.add_life = true;
				return;
			}
			else if (airdrop.type == 1)
			{
				player.leve = true;
				return;
			}
		}
		if (CollideJudge(player.x + player.w / 2, player.y, airdrop.x, airdrop.y, airdrop.w, airdrop.h))
		{
			mciSendString("close BGM_getbullet", 0, 0, 0);
			mciSendString("open ./music//getbullet.mp3 alias BGM_getbullet", 0, 0, 0);
			mciSendString("play BGM_getbullet", 0, 0, 0);
			airdrop.live = false;
			if (airdrop.type == 0)
			{
				player.add_life = true;
				return;
			}
			else if (airdrop.type == 1)
			{
				player.leve = true;
				return;
			}
		}

		if (CollideJudge(player.x + player.w, player.y + player.h - 50, airdrop.x, airdrop.y, airdrop.w, airdrop.h))
		{
			mciSendString("close BGM_getbullet", 0, 0, 0);
			mciSendString("open ./music//getbullet.mp3 alias BGM_getbullet", 0, 0, 0);
			mciSendString("play BGM_getbullet", 0, 0, 0);
			airdrop.live = false;
			if (airdrop.type == 0)
			{
				player.add_life = true;
				return;
			}
			else if (airdrop.type == 1)
			{
				player.leve = true;
				return;
			}
		}
		if (CollideJudge(player.x + player.w / 2, player.y + player.h / 2, airdrop.x, airdrop.y, airdrop.w, airdrop.h))
		{
			mciSendString("close BGM_getbullet", 0, 0, 0);
			mciSendString("open ./music//getbullet.mp3 alias BGM_getbullet", 0, 0, 0);
			mciSendString("play BGM_getbullet", 0, 0, 0);
			airdrop.live = false;
			if (airdrop.type == 0)
			{
				player.add_life = true;
				return;
			}
			else if (airdrop.type == 1)
			{
				player.leve = true;
				return;
			}
		}
	}
}
void GetScore()
{
	string s0 = "";
	memset(str_scoer, '\0', sizeof str_scoer);
	int tmp = score;
	while (true)
	{
		char c = '0' + (tmp % 10);
			s0 += c;
			tmp = tmp/10;
			if (!tmp) break;
	}
		reverse(s0.begin(),s0.end());
		for (int i = 0; i < s0.size(); i++)
			str_scoer[i] = s0[i];
}
void GetBestScore(int best_score)
{
	string s0 = "";
	memset(str_best_scoer, '\0', sizeof str_best_scoer);
	int tmp = best_score;
	while (true)
	{
		char c = '0' + (tmp % 10);
		s0 += c;
		tmp = tmp / 10;
		if (!tmp) break;
	}
	reverse(s0.begin(), s0.end());
	for (int i = 0; i < s0.size(); i++)
		str_best_scoer[i] = s0[i];
}
void PutScore()
{
	settextcolor(RGB(0,0,0));
	settextstyle(30, 0, "����");
	setbkmode(TRANSPARENT);
	char str1[] = "������";
	outtextxy(30, 30, str1);
	GetScore();
	outtextxy(120, 30, str_scoer);

}
void BestScore()
{
	ifstream myin;
	myin.open("best_score.txt", ios::in);
	if (!myin)
	{
		cout << "��ʧ��" << endl;
		exit(1);
	}
	int best_score;
	myin >> best_score;
	myin.close();


	best_score = max(score, best_score);
	ofstream myout;
	myout.open("best_score.txt", ios::out);
	if (!myin)
	{
		cout << "��ʧ��" << endl;
		exit(1);
	}
	myout << best_score;
	myout.close();

	settextcolor(RGB(255, 255, 255));
	settextstyle(80, 0, "Ink Free");
	setbkmode(TRANSPARENT);
	GetBestScore(best_score);
	outtextxy(302, 322, str_best_scoer);

}
void GamePause()
{

	if (GetAsyncKeyState(VK_ESCAPE)&& Timer(200, 8))
	{
		while (true)
		{
			if (GetAsyncKeyState(VK_ESCAPE) && Timer(200, 8))
				return;
		}
	}
	return;
}
void GameOver()
{
	mciSendString("close BGM_game_over", 0, 0, 0);
	mciSendString("open ./music//game_over.mp3 alias BGM_game_over", 0, 0, 0);
	mciSendString("play BGM_game_over", 0, 0, 0);
	BeginBatchDraw();
	putimage(0, 0, &img_game_over);
	Button back_menu_button(WIDTH / 2 - 200, HEIGHT * 3 / 5-10, 400, 55, 35, 255, 255, 255);
	Button exit_button(WIDTH / 2 - 200, HEIGHT * 3 / 5 + 90, 400, 55, 35,255, 255, 255);
	back_menu_button.button("���ز˵�");
	exit_button.button("������Ϸ");

	settextcolor(RGB(255, 255, 255));
	settextstyle(80, 0, "Ink Free");
	setbkmode(TRANSPARENT);

	GetScore();
	outtextxy(130, 195, str_scoer);
	BestScore();

	FlushBatchDraw();
	EndBatchDraw();
	ExMessage msg;
	while (true)
	{
		getmessage(&msg, EM_MOUSE);
		//cout << msg.x << "-----" << msg.y << endl;
		if (MouseOperat(back_menu_button.x, back_menu_button.y, back_menu_button.w, back_menu_button.h, msg))
		{
			mciSendString("close BGM_fight", 0, 0, 0);
			return;
		}
		if (MouseOperat(exit_button.x, exit_button.y, exit_button.w, exit_button.h, msg))
		{
			if (tip("ȷ���˳���", "��ʾ"))
			{
				exit(0);
			}
			else
			{
				msg.x = 0, msg.y = 0;
					continue;
			}
		}
	}
}
void InitEnemy()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].Init();
	}
}
void InitBullet()
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bullet[i].init();
	}
}
void InitEnemyBullet()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		for (int j = 0; j < ENEMY_BULLT_NUM_1; j++)
		{
			enemybullet[i][j].live = false;
			enemybullet[i][j].live_l = false;
			enemybullet[i][j].live_r = false;
		}
	}
}
void IniGameDate()
{
	player.Iniplayer();
	airdrop.init();
	getaridrop = 0;
	InitBullet();
	InitEnemy();
	InitEnemyBullet();
	score = 0;
	game_over = false;
}
int main()
{
	initgraph(WIDTH, HEIGHT);//������Ϸ����
	InitGame();//��ʼ����Ϸ
	while(true)
	{ 
		menu();//���˵�
		cleardevice();//����
		IniGameDate();//��ʼ����Ϸ����
		mciSendString("open ./music//fight.mp3 alias BGM_fight", 0, 0, 0);//����ս������
		mciSendString("play BGM_fight repeat", 0, 0, 0);
		BeginBatchDraw();
		while (true)//��ʼ��Ϸ
		{
			gamedraw();//������Ϸ����
			moveback();//�����ƶ�
			player.move();//����ƶ�
			AirdropPut();//���ÿ�Ͷ
			BulletPUT();//�����ӵ�
			EnemyPut();//���õл�
			EnemyBulletPUT();//���õл��ӵ�
			DestroyJudge();//�˺��ж�
			if (player.add_life&& player.life<4)//�������Ƿ�ﵽ����ֵ����
			{
				player.life++;
				player.add_life = false;
			}
			PutScore();//��ʾ��ǰ����
			FlushBatchDraw();
			GamePause();//��ͣ��Ϸ
			if (game_over)//�ж��Ƿ������Ϸ
			{
				mciSendString("close BGM_fight", 0, 0, 0);//�ر�ս����������
				break;
			}
		}
		EndBatchDraw();
		GameOver();//��Ϸ�������
		mciSendString("close BGM_game_over", 0, 0, 0);
	}
	return 0;
}