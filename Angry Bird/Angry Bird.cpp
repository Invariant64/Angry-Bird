#include <graphics.h>
#include <cstring>
#include <conio.h>
#include <iostream>
#include <wchar.h>
#include <fstream>
#include <algorithm>
#include <cmath>

using namespace std;

const float PI = 3.14159265359f;

const int Screen_L = 1000;				//窗口长度
const int Screen_H = 600;				//窗口宽度

const float GravitySpeedChange = 0.07f;	//Animal在空中时重力对其的速度影响

const float GroundKnockReduce_K = 0.9f;	//Animal与Ground发生碰撞的速度衰减系数
const float GroundRollReduce_K = 0.8f;	//Animal在Ground上滚动的速度衰减系数
const float AnimalKnockReduce_K = 0.7f;	//Animal相互发生碰撞的速度衰减系数

const float Delta = 10.0f;				//Animal在检查碰撞时的允许误差范围
const float touchGroundDelta = 5.0f;
const float SpeedToZero = 0.05f;		//Animal的速度降为0的临界值

enum DRCT {								//表示Animal与Ground碰撞时Animal位于Ground的方向
	NOTTOUCHING, LEFT, TOP, RIGHT, DOWN
};

const wchar_t* GetWC(const char* c) {	//将 char* 转为 wchat_t*
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);
	return wc;
}


class Ground {
private:	
	wchar_t* picAddress;
	IMAGE img;
public:
	int left, top, right, down;

	Ground() {}

	Ground(int x1, int y1, int x2, int y2, wchar_t* ad) {
		left = x1, top = y1, right = x2, down = y2;
		picAddress = ad;
		
		if (picAddress) {
			loadimage(&img, picAddress, right - left, down - top, true);
		}
	}

	void draw() {
		if (picAddress) {
			putimage(left, top, &img, SRCCOPY);
		}
		else {
			setfillcolor(BLUE);
			setlinecolor(RED);
			fillrectangle(left, top, right, down);
		}
	}
};

class Animal {
private:
	int size;			//将动物视为一个圆，其半径
	float p_x, p_y;		//圆心坐标
	float v_x, v_y;		//线速度
	float v_r;			//旋转速度 设定顺时针为正向
	Ground* is_on;		//正在哪个Ground上，不在时为NULL
	bool is_rolling;	//是否正在滚动（v_y=0）

	wchar_t* pic1Address, * pic2Address;
	wchar_t* pic11Address, * pic21Address;

	IMAGE img1, img2;
	IMAGE img11, img21;

public:
	Animal() {}

	Animal(int s, float px, float py, float vx, float vy, float vr, wchar_t* pic1, wchar_t* pic2) {
		size = s;
		p_x = px, p_y = py;
		v_x = vx, v_y = vy;
		v_r = vr;

		is_rolling = false;

		is_on = NULL;

		pic1Address = pic1;
		pic2Address = pic2;

		if (pic1Address) {
			loadimage(&img1, pic1Address, size * 2, size * 2, true);
		}
		if (pic2Address) {
			loadimage(&img2, pic2Address, size * 2, size * 2, true);
		}
	}

	Animal(int s, float px, float py, float vx, float vy, float vr, wchar_t* pic1, wchar_t* pic2, wchar_t* pic11, wchar_t* pic21) {
		size = s;
		p_x = px, p_y = py;
		v_x = vx, v_y = vy;
		v_r = vr;

		is_rolling = false;

		is_on = NULL;

		pic1Address = pic1;
		pic2Address = pic2;
		pic11Address = pic11;
		pic21Address = pic21;

		if (pic1Address) {
			loadimage(&img1, pic1Address, size * 2, size * 2, true);
		}
		if (pic2Address) {
			loadimage(&img2, pic2Address, size * 2, size * 2, true);
		}

		if (pic11Address) {
			loadimage(&img11, pic11Address, size * 2, size * 2, true);
		}
		if (pic21Address) {
			loadimage(&img21, pic21Address, size * 2, size * 2, true);
		}
	}

	void move() {
		p_x += v_x;
		p_y += v_y;
	}

	void setSpeed(float vx, float vy) {			//设置速度
		v_x = vx;
		v_y = vy;
	}

	void addSpeed(float dvx, float dvy) {		//以速度的增量设置速度
		v_x += dvx;
		v_y += dvy;
	}

	void getIsOn(Ground* grd, int n_grd) {		//获取Animal现在在下方最近的Ground
		if (grd == NULL) {
			return;
		}
		Ground* closest = NULL;
		//cout << "grd[1]" << (grd + 3)->top << endl;
		int miny = 1000;
		for (int i = 1; i < n_grd; i++) {
			if (p_x + size >= (grd + i)->left && p_x - size <= (grd + i)->right && abs((grd + i)->top - p_y - size) <= 5.0f && (grd + i)->top < miny) {
				closest = grd + i;
				miny = closest->top;
				if ((grd + i)->top == 400) {
					//Sleep(200);
				}
			}
		}
		if (closest) {
			is_on = closest;
			cout << "is_on.top = " << is_on->top << endl;
		}
		else {
			is_on = NULL;
		}
	}

	void checkIsOn() {				//检查目前的is_on是否位于Animal下方
		if (is_on == NULL) {
			is_rolling = false;
			return;
		}
		if (p_x - size < is_on->left || p_x + size > is_on->right) {
			is_on = NULL;
			is_rolling = false;
		}
		else {
			cout << (p_x - size >= is_on->left) << ' ' << (p_x + size <= is_on->right) << ' ' << (abs(is_on->top - p_y - size) <= 10.0f) << ' ' << (abs(v_y) <= SpeedToZero) << endl;
			cout << v_y << ' ' << is_on->top << ' ' << p_y << ' ' << is_on->top - p_y - size << endl;
			if (abs(is_on->top - p_y - size) <= 10.0f) {
				//Sleep(1000);
			}
			if (is_on && p_x - size >= is_on->left && p_x + size <= is_on->right && abs(is_on->top - p_y - size) <= 5.0f && abs(v_y) <= 0.5f) {
				is_rolling = true;
				cout << "is rolling!" << endl;
			}
		}
	}

	void changeSpeed() {						//根据目前的状态改变速度
		if (is_rolling) {
			v_x *= GroundRollReduce_K;
			p_y = is_on->top - size;
			v_y = 0;

			if (abs(v_x) <= SpeedToZero) {
				v_x = 0;
			}
		}
		else {
			v_y += GravitySpeedChange;
		}
	}

	void draw() {
		if (pic1Address) {
			IMAGE i1;
			if (v_x > 0) {
				rotateimage(&i1, &img1, -atan(v_y / v_x), WHITE, true);
			}
			else if(v_x > 0) {
				rotateimage(&i1, &img11, -atan(v_y / v_x), WHITE, true);
			}
			else {
				rotateimage(&i1, &img11, 0, WHITE, true);
			}
			putimage(p_x - size, p_y - size, &i1, SRCCOPY);
		}
		if (pic2Address) {
			IMAGE i2;
			if (v_x < 0) {
				rotateimage(&i2, &img2, -atan(v_y / v_x), WHITE, true);
			}
			else if(v_x > 0) {
				rotateimage(&i2, &img21, -atan(v_y / v_x), WHITE, true);
			}
			else {
				rotateimage(&i2, &img21, 0, WHITE, true);
			}
			putimage(p_x - size, p_y - size, &i2, SRCCOPY);
		}
		if (is_rolling) {
			cout << p_x << ' ' << p_y << endl;
		}
	}

	void knockAnimal(Animal* anm) {				//与另一Animal发生碰撞	
		float r = sqrt((p_x - anm->p_x) * (p_x - anm->p_x) + (p_y - anm->p_y) * (p_y - anm->p_y));
		if (r <= size + anm->size + Delta) {
			float vtx, vty;
			vtx = v_x, vty = v_y;
			v_x = anm->v_x;
			v_y = anm->v_y;

			anm->v_x = vtx;
			anm->v_y = vty;

			v_x *= AnimalKnockReduce_K;
			v_y *= AnimalKnockReduce_K;
			anm->v_x *= AnimalKnockReduce_K;
			anm->v_y *= AnimalKnockReduce_K;

			//cout << "KNOCKING!" << endl;
		}
	}

	void knockGround(Ground grd) {				//与Ground发生碰撞
		/*
		if (abs(grd.left - p_x - size) <= Delta && p_y - size <= grd.down && p_y + size >= grd.top) {
			v_x = -v_x;
			cout << "left:" << p_x << ' ' << p_y << endl;
		}
		else if (abs(grd.top - p_y - size) <= Delta && p_x - size <= grd.right && p_x + size >= grd.left) {
			v_y = -v_y;
			cout << "top:" << p_x << ' ' << p_y << endl;
		}
		else if (abs(p_x - grd.right - size) <= Delta && p_y - size <= grd.down && p_y + size >= grd.top) {
			v_x = -v_x;
			cout << "right:" << p_x << ' ' << p_y << endl;
		}
		else if (abs(p_y - grd.down - size) <= Delta && p_x - size <= grd.right && p_x + size >= grd.left) {
			v_y = -v_y;
			cout << "down:" << p_x << ' ' << p_y << endl;
		}
		else {
			return;
		}*/
		
		if (p_x + size >= grd.left - Delta && p_x + size < grd.left && p_y - size <= grd.down && p_y + size >= grd.top && v_x > 0) {
			v_x = -v_x;
			//cout << "left:" << p_x << ' ' << p_y << endl;
		}
		else if (grd.top - p_y - size <= Delta && grd.top - p_y - size > 0 && p_x - size <= grd.right && p_x + size >= grd.left && v_y > 0) {
			v_y = -v_y;
			cout << "top:" << p_x << ' ' << p_y << endl;
		}
		else if (p_x - grd.right - size <= Delta && p_x - grd.right - size > 0 && p_y - size <= grd.down && p_y + size >= grd.top && v_x < 0) {
			v_x = -v_x;
			//cout << "right:" << p_x << ' ' << p_y << endl;
		}
		else if (p_y - grd.down - size <= Delta && p_y - grd.down - size > 0 && p_x - size <= grd.right && p_x + size >= grd.left && v_y < 0) {
			v_y = -v_y;
			//cout << "down:" << p_x << ' ' << p_y << endl;
		}
		else {
			return;
		}

		v_x *= GroundKnockReduce_K;
		v_y *= GroundKnockReduce_K;
	}

	bool checkAnimalKnock(Animal anm) {			//检查是否与某Animal发生碰撞
		float r = sqrt((p_x - anm.p_x) * (p_x - anm.p_x) + (p_y - anm.p_y) * (p_y - anm.p_y));
		if (r <= size + anm.size + Delta) {
			return true;
		}
		else {
			return false;
		}
	}

	DRCT checkGroundKnock(Ground grd) {			//检查是否与某Ground发生碰撞，并返回与Ground碰撞的方向
		if (grd.left - p_x <= size + Delta || grd.left - p_x >= size - Delta) {
			return LEFT;
		}
		if (grd.top - p_y <= size + Delta || grd.top - p_y >= size - Delta) {
			return TOP;
		}
		if (p_x - grd.right <= size + Delta || p_x - grd.right >= size - Delta) {
			return RIGHT;
		}
		if (p_y - grd.down <= size + Delta || p_y - grd.down >= size - Delta) {
			return DOWN;
		}
		return NOTTOUCHING;
	}

};


class Text {
private:
	LPCTSTR body;
	LPCTSTR ti;
	int color;
	int height, length;
	int x1, y1, x2, y2;
	RECT r;
	unsigned int MODE;
public:
	Text(
		LPCTSTR b,
		LPCTSTR t,
		int c,
		int h, int l,
		int xx1, int yy1, int xx2, int yy2) :
		body(b), ti(t), color(c), height(h), length(l), x1(xx1), y1(yy1), x2(xx2), y2(yy2) {
		r = { x1, y1, x2, y2 };
		MODE = DT_CENTER;
	}

	Text(
		LPCTSTR b,
		LPCTSTR t,
		int c,
		int h, int l,
		int xx1, int yy1, int xx2, int yy2, unsigned int M) :
		body(b), ti(t), color(c), height(h), length(l), x1(xx1), y1(yy1), x2(xx2), y2(yy2) {
		r = { x1, y1, x2, y2 };
		MODE = M;
	}

	void draw() {
		settextcolor(color);
		settextstyle(height, length, ti);
		drawtext(body, &r, MODE);
	}
};

class TextBox {
private:
	int left, top, right, down;
	wchar_t *text, *secret_text;
	int max_length;
	COLORREF color;
	bool isSecret;

public:

	TextBox(){}

	bool check(int x, int y) {
		if (x >= left && y >= top && x <= right && y <= down) {
			return true;
		}
		return false;
	}

	void setColor(COLORREF color) {
		this->color = color;
	}

	TextBox(int x1, int y1, int x2, int y2, int max, bool iss) {
		max_length = max;
		text = new wchar_t[max_length];
		secret_text = new wchar_t[max_length];
		text[0] = secret_text[0] = 0;
		left = x1, top = y1, right = x2, down = y2;
		isSecret = iss;
		color = LIGHTGRAY;
		//draw();
	}

	void draw() {
		setfillcolor(color);
		setlinecolor(BLACK);
		fillrectangle(left, top, right, down);
		if (isSecret) {
			outtextxy(left + 10, top + 5, secret_text);
		}
		else {
			outtextxy(left + 10, top + 5, text);
		}
	}

	wchar_t* getText() {
		return text;
	}

	void onMessage() {

		bool binput = true;
		int width;
		ExMessage msg;
		int cnt = 0;

		if (isSecret) {
			width = textwidth(secret_text);
		}
		else {
			width = textwidth(text);
		}

		draw();

		while (binput) {
			while (binput && peekmessage(&msg, EM_CHAR | EM_MOUSE)) {
				if (msg.message == WM_LBUTTONDOWN) {
					if (msg.x < left || msg.y < top || msg.x > right || msg.y > down) {
						binput = false;
						break;
					}
				}
				else if (msg.message == WM_CHAR) {
					size_t len = wcslen(text);
					switch (msg.ch) {
					case '\b':
						if (len > 0) {
							clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, down - 3);
							secret_text[len - 1] = 0;
							text[len - 1] = 0;
							if (isSecret) {
								width = textwidth(secret_text);
							}
							else {
								width = textwidth(text);
							}
							clearrectangle(left + 10 + width, top + 5, right - 1, down - 1);
						}
						break;
					case '\r':
					case '\n':
						binput = false;
						break;
					default:
						if (len < max_length - 1) {
							secret_text[len] = '*';
							secret_text[len + 1] = 0;
							text[len] = msg.ch;
							text[len + 1] = 0;
							len++;
							cnt = 0;

							clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, down - 3);

							if (isSecret) {
								width = textwidth(secret_text);
								outtextxy(left + 10, top + 5, secret_text);
							}
							else {
								width = textwidth(text);
								outtextxy(left + 10, top + 5, text);
							}
						}
					}
				}
			}

			cnt = (cnt + 1) % 32;
			if (cnt < 16)
				line(left + 10 + width + 1, top + 3, left + 10 + width + 1, down - 3);				// 画光标
			else
				clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, down - 3);
			Sleep(20);
		}

		clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, down - 3);

		color = LIGHTGRAY;

		draw();
	}

	void Running(ExMessage msg1) {
		if (check(msg1.x, msg1.y)) {
			setColor(WHITE);
			draw();
			if (msg1.message == WM_LBUTTONDOWN) {
				onMessage();
				//fillrectangle(0, 0, 100, 100);
				//while(1){}
			}
		}
		else {
			setColor(LIGHTGRAY);
			draw();
		}
	}
};

class Button {
private:
	const wchar_t *text;
	int left, top, right, down;
	bool (*func)();
public:
	Button(){}

	bool check(int x, int y) {
		if (x >= left && y >= top && x <= right && y <= down) {
			return true;
		}
		return false;
	}

	void draw() {
		setfillcolor(WHITE);
		setlinecolor(BLACK);
		fillrectangle(left, top, right, down);
		outtextxy(left + (right - left - textwidth(text) + 1) / 2, top + (down - top - textheight(text) + 1) / 2, text);
	}

	Button(int x1, int y1, int x2, int y2, bool (*f)(), const wchar_t* te) {
		left = x1, top = y1, right = x2, down = y2;
		func = f;
		text = te;
		draw();
	}

	bool onMessage() {
		if (func != NULL) {
			return func();
		}
		return false;
	}

	bool running(ExMessage msg) {
		if (check(msg.x, msg.y) && msg.message == WM_LBUTTONDOWN) {
			return onMessage();
		}
		return false;
	}
};


TextBox user_name_box = TextBox(400, 210, 670, 260, 12, false);
TextBox user_code_box = TextBox(400, 280, 670, 330, 12, true);

Button log_in_btn;
Button rgs_act_btn;


int checkAccount(wchar_t* input_name, wchar_t* input_code) {
	FILE* user_info;
	char* user_name, * user_code;

	user_info = fopen("./data/user_info.txt", "r");

	//fscanf(user_info, "%s %s", user_name, user_code);

	user_name = new char[50];
	user_code = new char[50];

	while (fscanf(user_info, "%s %s", user_name, user_code) != EOF) {
		const wchar_t* wname, * wcode;

		wname = GetWC(user_name);
		wcode = GetWC(user_code);

		if (wcscmp(input_name, wname) == 0) {
			if (wcscmp(input_code, wcode) == 0) {
				fclose(user_info);
				return 1;
			}
			else {
				fclose(user_info);
				return 0;
			}
		}
	}
	fclose(user_info);
	return 2;
}

bool log_in_btn_Click() {
	wchar_t* wname, * wcode;
	wname = user_name_box.getText();
	wcode = user_code_box.getText();

	if (wname[0] == 0) {
		MessageBox(NULL, TEXT("Please input your Name!"), TEXT("Error"), 0);
		return false;
	}

	if (wcode[0] == 0) {
		MessageBox(NULL, TEXT("Please input your Password!"), TEXT("Error"), 0);
		return false;
	}

	switch (checkAccount(wname, wcode)) {
	case 0:  //there doesn't exist the account
		MessageBox(NULL, TEXT("Your password is wrong! Please try again."), TEXT("Error"), 0);
		return false;
	case 1:  //there exists the account and the password is also correct
		//MessageBox(NULL, TEXT("Welcome!"), TEXT("Welcome"), 0);
		return true;
	case 2:  //there exists the account but the password is worng
		MessageBox(NULL, TEXT("You have no account! Please register one."), TEXT("Error"), 0);
		return false;
	}
	return false;
}

bool rgs_act_btn_Click() {
	wchar_t* wname, * wcode;
	wname = user_name_box.getText();
	wcode = user_code_box.getText();

	if (wname[0] == 0) {
		MessageBox(NULL, TEXT("Please input your Name!"), TEXT("Error"), 0);
		return false;
	}

	if (wcode[0] == 0) {
		MessageBox(NULL, TEXT("Please input your Password!"), TEXT("Error"), 0);
		return false;
	}

	if (checkAccount(wname, wcode) == 2) {
		FILE* fp;
		fp = fopen("./data/user_info.txt", "a");

		char cname[50], ccode[50];
		sprintf(cname, "%ws", wname);
		sprintf(ccode, "%ws", wcode);

		//fseek(fp, 0, SEEK_END);
		fprintf(fp, "%s %s\n", cname, ccode);

		fclose(fp);

		MessageBox(NULL, TEXT("Successfully registered!"), TEXT("Success"), 0);

		return true;
	}
	else {
		MessageBox(NULL, TEXT("This account has been registered!"), TEXT("Error"), 0);

		return false;
	}
}


void welcome() {

	ExMessage msg;

	initgraph(Screen_L, Screen_H);
	setbkmode(TRANSPARENT);

	//BeginBatchDraw();

	static IMAGE welcome_background;
	loadimage(&welcome_background, L"./resources/background/welcome.jpg", Screen_L, Screen_H, true);
	putimage(0, 0, Screen_L, Screen_H, &welcome_background, 0, 0);

	/*
	settextcolor(RED);
	settextstyle(70, 0, _T("微软雅黑"));
	RECT title = { 333, 100, 666, 300 };
	drawtext(_T("愤怒的小鸟"), &title, DT_CENTER);

	settextcolor(BLACK);
	settextstyle(30, 0, _T("微软雅黑"));
	RECT touch_to_start = { 450, 500, 550, 600 };
	*/


	Text main_title = Text(L"愤怒的小鸟 无敌版", L"微软雅黑", RED, 60, 0, 300, 100, 700, 300);
	Text touch_to_start = Text(L"...单击以开始...", L"微软雅黑", BLACK, 30, 0, 400, 370, 600, 420);

	touch_to_start.draw();
	main_title.draw();

	getmessage(&msg, EM_MOUSE);
	while (msg.message != WM_LBUTTONUP) {
		getmessage(&msg, EM_MOUSE);
	}

	closegraph();


	initgraph(Screen_L, Screen_H, SHOWCONSOLE);
	setbkmode(TRANSPARENT);

	putimage(0, 0, Screen_L, Screen_H, &welcome_background, 0, 0);

	Text log_in_title = Text(L"登录", L"微软雅黑", BLACK, 50, 0, 300, 100, 700, 300);
	Text user_name = Text(L"用户名:", L"微软雅黑", BLACK, 30, 0, 320, 220, 450, 270, DT_LEFT);
	Text user_code = Text(L" 密 码:", L"微软雅黑", BLACK, 30, 0, 320, 290, 450, 340, DT_LEFT);

	setfillcolor(0xffffdf);
	fillrectangle(300, 170, 700, 500);

	//log_in_title.draw();
	//user_name.draw();
	//user_code.draw();

	//setfillcolor(LIGHTGRAY);
	//setlinecolor(BLACK);
	//fillrectangle(400, 210, 670, 260);
	//fillrectangle(400, 280, 670, 330);

	//string userName, userCode;

	//BeginBatchDraw();

	setbkcolor(WHITE);
	putimage(0, 0, Screen_L, Screen_H, &welcome_background, 0, 0);
	setfillcolor(0xffffdf);
	fillrectangle(300, 170, 700, 500);

	log_in_title.draw();
	user_name.draw();
	user_code.draw();

	//TextBox user_name_box = TextBox(400, 210, 670, 260, 12, false);
	//TextBox user_code_box = TextBox(400, 280, 670, 330, 12, true);

	Button log_in_btn = Button(350, 365, 650, 405, log_in_btn_Click, L"登录");
	Button rgs_act_btn = Button(350, 415, 650, 455, rgs_act_btn_Click, L"注册");

	while (true) {

		msg = getmessage(EM_MOUSE);

		user_name_box.Running(msg);
		user_code_box.Running(msg);

		if (log_in_btn.running(msg)) {
			return;
		}

		rgs_act_btn.running(msg);

		/*
		setfillcolor(LIGHTGRAY);
		if (msg.x >= 400 && msg.x <= 670 && msg.y >= 280 && msg.y <= 330){
			setfillcolor(0xeeeeee);

		}
		fillrectangle(400, 280, 670, 330);

		setfillcolor(LIGHTGRAY);
		if (msg.x >= 400 && msg.x <= 670 && msg.y >= 210 && msg.y <= 260) {
			setfillcolor(0xeeeeee);

		}
		fillrectangle(400, 210, 670, 260);


		peekmessage(NULL, EM_MOUSE | EM_CHAR);*/

		//FlushBatchDraw();
	}

}

void playing() {
	cleardevice();

	int groundNumber, animalNumber;

	groundNumber = 5;
	animalNumber = 3;

	Ground grd[6];
	Animal anm[5];

	grd[0] = Ground(0, 400, 500, 500, NULL);
	grd[1] = Ground(0, 200, 300, 240, NULL);
	grd[2] = Ground(700, 400, 1000, 500, NULL);
	grd[3] = Ground(360, 0, 370, 200, NULL);
	

	anm[0] = Animal(20, 30, 150, 3, 0, 0, (wchar_t*)L"./resources/birds/redbird.jpeg", (wchar_t*)NULL);
	anm[1] = Animal(20, 600, 200, -2, 0, 0, (wchar_t*)L"./resources/birds/redbird.jpeg", (wchar_t*)NULL);
	anm[2] = Animal(20, 550, 100, 2.5f, -1, 0, (wchar_t*)L"./resources/birds/redbird.jpeg", (wchar_t*)NULL);


	for (int i = 0; i < groundNumber; i++) {
		grd[i].draw();
	}

	for (int i = 0; i < animalNumber; i++) {
		anm[i].draw();
	}

	//while(1){}

	BeginBatchDraw();

	while(true){
		cleardevice();


		for (int i = 0; i < animalNumber; i++) {
			anm[i].getIsOn(grd, groundNumber);
			anm[i].checkIsOn();

			for (int j = 0; j < groundNumber; j++) {
				anm[i].knockGround(grd[j]);
			}

			for (int j = i + 1; j < animalNumber; j++) {
				anm[i].knockAnimal(&anm[j]);
			}
		}

		for (int i = 0; i < animalNumber; i++) {
			anm[i].changeSpeed();
		}

		for (int i = 0; i < animalNumber; i++) {
			anm[i].move();
		}

		for (int i = 0; i < groundNumber; i++) {
			grd[i].draw();
		}

		for (int i = 0; i < animalNumber; i++) {
			anm[i].draw();
		}

		//Sleep(1);

		FlushBatchDraw();
	}

	EndBatchDraw();
}

void testing() {
	cleardevice();

	int groundNumber, animalNumber;

	groundNumber = 7;
	animalNumber = 3;

	Ground grd[7];
	Animal anm[5];

	grd[0] = Ground(0, 0, 10, 600, NULL);
	grd[1] = Ground(990, 0, 1000, 600, NULL);
	grd[2] = Ground(10, 0, 990, 10, NULL);
	grd[3] = Ground(10, 590, 990, 600, NULL);
	grd[4] = Ground(500, 100, 520, 550, NULL);
	grd[5] = Ground(100, 400, 400, 410, NULL);
	grd[6] = Ground(600, 400, 900, 410, NULL);

	anm[0] = Animal(20, 50, 150, 3, 0, 0, (wchar_t*)L"./resources/birds/redbird.jpeg", (wchar_t*)NULL, (wchar_t*)L"./resources/birds/redbird1.jpeg", (wchar_t*)NULL);
	anm[1] = Animal(20, 600, 200, -2, 0, 0, (wchar_t*)L"./resources/birds/redbird.jpeg", (wchar_t*)NULL, (wchar_t*)L"./resources/birds/redbird1.jpeg", (wchar_t*)NULL);
	anm[2] = Animal(10, 550, 100, 2.5f, -1, 0, (wchar_t*)L"./resources/birds/redbird.jpeg", (wchar_t*)NULL, (wchar_t*)L"./resources/birds/redbird1.jpeg", (wchar_t*)NULL);


	for (int i = 0; i < groundNumber; i++) {
		grd[i].draw();
	}

	for (int i = 0; i < animalNumber; i++) {
		anm[i].draw();
	}

	//while(1){}

	BeginBatchDraw();

	while (true) {
		cleardevice();


		for (int i = 0; i < animalNumber; i++) {
			anm[i].getIsOn(grd, groundNumber);
			anm[i].checkIsOn();

			for (int j = 0; j < groundNumber; j++) {
				anm[i].knockGround(grd[j]);
			}

			for (int j = i + 1; j < animalNumber; j++) {
				anm[i].knockAnimal(&anm[j]);
			}
		}

		for (int i = 0; i < animalNumber; i++) {
			anm[i].changeSpeed();
		}

		for (int i = 0; i < animalNumber; i++) {
			anm[i].move();
		}

		for (int i = 0; i < groundNumber; i++) {
			grd[i].draw();
		}

		for (int i = 0; i < animalNumber; i++) {
			anm[i].draw();
		}

		Sleep(1);

		FlushBatchDraw();
	}

	EndBatchDraw();
}


int main() {

	welcome();

	//playing();

	testing();
	
	return 0;
}

