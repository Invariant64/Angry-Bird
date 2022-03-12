#include "head.h"
#include <cstring>
#include <conio.h>
#include <iostream>
#include <wchar.h>
#include <fstream>

using namespace std;

const int Screen_L = 1000;
const int Screen_H = 600;


const wchar_t* GetWC(const char* c) {
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}


struct Text {
	LPCTSTR body;
	LPCTSTR ti;
	int color;
	int height, length;
	int x1, y1, x2, y2;
	RECT r;
	unsigned int MODE;

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


void Knock(Animals *anm, Ground grd) {
	//if (anm.x >= grd.p[0].x && anm.x <= grd.p[1].x && (abs(anm.y - grd.p[0].y) <= 1 || abs(anm.y - grd.p[3].y) <= 1))
	//	anm.v_y = -anm.v_y * REDUCE_K;
	//if (anm.y >= grd.p[3].y && anm.y <= grd.p[0].y && (abs(anm.x - grd.p[0].x) <= 1 || abs(anm.x - grd.p[3].x) <= 1))
	//	anm.v_x = -anm.v_x * REDUCE_K;
	//outtextxy(0, 0, 'Y');
	anm->v_y = -anm->v_y * 0.9;
	if (anm->v_y < 0.1) {
		anm->y = grd.p[0].y - anm->r;
	}
	//while(1){}
}

void welcome();

int main() {

	welcome();

	//while(1){}

	//initgraph(1000, 500);
	//BeginBatchDraw();
	int key = 0;

	RedBirds rd[5];
	Ground grd[5];
	grd[1] = Ground(0, 250, 1000, 250, 0, 500, 1000, 500);
	grd[2] = Ground(0, 50, 100, 50, 0, 70, 100, 70);
	//rd1 = RedBirds();

	rd[1] = RedBirds(15, 0, 20, 15, 10000, 0, 0);
	rd[2] = RedBirds(-3, 0, 50, 15, 10000, 500, 100);

	//rd[1] = RedBirds(1, 0, 20, 15, 10000, 0, 0);
	//rd[2] = RedBirds(-3, 0, 50, 15, 10000, 500, 100);

	//rd1.drawObj(1, 1);
	//IMAGE img;
	//loadimage(&img, _T("redbird.jpeg"));
	//putimage(100, 100, &img);

	//rd1.draw();

	setfillcolor(BLUE);
	
	char c = 'Y';

	while (1) {
		key++;
		cleardevice();

		/*
		if (rd1.isTouchGrd(grd2)) {
			if (rd1.key % 3 == 0) {
				//return 0;
				Knock(&rd1, grd2);
				//Knock(&rd1, grd)
				//Sleep(1000);
				//
			}


		}

		*/
		//rd1.key++;

		for (int i = 1; i <= 2; i++) {
			rd[i].speedChange();
			rd[i].move();
		}

		for (int j = 1; j <= 2; j++) {
			for (int i = 1; i <= 2; i++) {
				if (rd[j].isTouchGrd(&grd[i])) {
					//rd[j].nearGrd = &grd[i];
					Knock(&rd[j], grd[i]);
				
				}
			}
		}

		for (int i = 1; i <= 2; i++) {
			for (int j = 1; j < 2; j++) {
				if (rd[i].isTouchAnm(&rd[j])) {
					//return 0;
					rd[i].AnmKnock(&rd[j]);
				}
			}
		}


		if (rd[2].isRolling) {
			outtextxy(0, 0, 'Y');
		}

		for (int i = 1; i <= 2; i++) {
			grd[i].draw();
		}

		for (int i = 1; i <= 2; i++) {
			rd[i].draw();
		}

		//rd1.draw();
		FlushBatchDraw();
		Sleep(20);
	}

	return 0;
}

//TextBox user_name_box = TextBox();
//TextBox user_code_box = TextBox();

TextBox user_name_box = TextBox(400, 210, 670, 260, 12, false);
TextBox user_code_box = TextBox(400, 280, 670, 330, 12, true);

int checkAccount(wchar_t* input_name, wchar_t* input_code);

Button log_in_btn;
Button rgs_act_btn;

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
		MessageBox(NULL, TEXT("Welcome!"), TEXT("Welcome"), 0);
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


	initgraph(Screen_L, Screen_H);
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

	while(true){

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


int checkAccount(wchar_t* input_name, wchar_t* input_code) {
	FILE *user_info;
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
