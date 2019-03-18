/*
	1.数据设计
	2.模块化设计
	3.实现过程
*/
#include <graphics.h>
#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>   // 光标设置API
#include <time.h>      // 食物随机
#include <conio.h>	   // 按键监控

//			辅助宏定义
#define MAPHEIGHT 25	  // 窗口的属性
#define MAPWIDTH  60	  // 窗口的属性
#define SNAKESIZE 50      // 蛇的最大节数

//			数据设计
// 食物的结构体
struct {
	// 如何去定位：坐标
	int x;
	int y;
}food;

//	蛇的结构体
struct {
	// 记录每一节蛇的坐标
	int x[SNAKESIZE];
	int y[SNAKESIZE];
	int len;	// 蛇的长度
	int speed;  // 蛇的移动速度
}snake;


//    全局变量
int key = 'w';    // 初始化移动方向
int changeFlag = 0; // 蛇的变化标记


//  模块化设计 ---> 功能的划分--->抽象到函数
//		怎么抽象：具体了解业务逻辑
// 0.辅助函数：光标移动
void gotoxy(int x, int y);
// 1.画地图
void drawMap();
// 2.食物的产生
void creatFood();
// 3.按键操作
void keyDown();
// 4.蛇的状态:判断是否结束游戏
int snakeStatus();

// 主函数
int main()
{
	drawMap();
	while (1) {
		creatFood();
		// 延时
		Sleep(snake.speed);
		keyDown();
		if (!snakeStatus()) {
			// 撞墙了
			break;
		}
	}
	// 把光标放到中间去，打印GAME OVER！
	gotoxy(MAPWIDTH / 2, MAPHEIGHT / 2);
	printf("GAME OVRE！\n");
	system("pause");
	return 0;
}

//////////////////////
// 0.辅助函数：光标移动
void gotoxy(int x, int y)
{
	// 调用win32的API去设置控制台的光标位置
	// 1.找到控制台的这个窗口
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	// 2.光标的结构体
	COORD coord;
	// 3.设置坐标
	coord.X = x;
	coord.Y = y;
	// 4.同步到控制台:Set Console Cursor Position
	SetConsoleCursorPosition(handle, coord);

}

//////////////////////////
// 1.画地图
void drawMap()
{	// *:食物  ■：蛇身
	srand((unsigned int)time(NULL));  // 随机函数种子
	// 1.圈地
		// 1.1 左右边框
	for (int i = 0; i <= MAPHEIGHT; i++) {
		gotoxy(0, i);
		printf("■");
		gotoxy(MAPWIDTH, i);
		printf("■");
	}
		// 1.2 上下边框
		// "■"是占两个字符
	for (int i = 0; i <= MAPWIDTH; i += 2) {
		gotoxy(i, 0);
		printf("■");
		gotoxy(i, MAPHEIGHT);
		printf("■");
	}
	// 2.画蛇
	// 2.1 确定蛇的属性
	snake.len = 3;
	snake.speed = 300;
	// 开始蛇头在屏幕中间
	snake.x[0] = MAPWIDTH / 2;
	snake.y[0] = MAPHEIGHT / 2; 
	// 2.2 画蛇
	// 画蛇头
	gotoxy(snake.x[0], snake.y[0]);
	printf("■"); 
	// 画剩下的身体
	for (int k = 1; k < snake.len; k++) {
		snake.x[k] = snake.x[k - 1] + 2;
		snake.y[k] = snake.y[k - 1];
		// 到当前坐标打印出来
		printf("■");
	}
	// 3.画食物
		// 3.1 确定坐标,并判断食物坐标是否为偶数
	while (1) {
		food.x = rand() % (MAPWIDTH - 4) + 2;   // 要取得[a,b)的值可以用：(rand() % (b-a)) + a
		food.y = rand() % (MAPHEIGHT - 2) + 1;
		if (food.x % 2 != 0) {
			// 如果不是偶数，继续产生新的食物
			continue;
		}
		else {
			// 3.2 如果是偶数，画出来
			gotoxy(food.x, food.y);
			printf("*");
			break;
		}
		
	}
}

//////////////
// 2.食物的产生
void creatFood()
{
	// 蛇把食物吃了，产生新的食物，实质是判断蛇头的坐标是否与食物坐标相等
	if (snake.x[0] == food.x && snake.y[0] == food.y) {
		srand((unsigned int)time(NULL));
		// 食物的坐标要处理一下,产生的食物不能在蛇的坐标, 并且食物的坐标要是偶数
		// 因为蛇头的宽度是偶数
		while (1) {
			int flag = 1; // 定义一个标记
			food.x = rand() % (MAPWIDTH - 4) + 2;
			food.y = rand() % (MAPHEIGHT - 2) + 1;
				// 产生的食物不能在蛇的身上
				for (int k = 0; k < snake.len; k++) {
					if (snake.x[k] == food.x && snake.y[k] == food.y) {
						// 如果相等，说明食物在蛇的身上
						flag = 0;  // 0标记食物不适合
						break;
					}
					
				}
				// 判断食物坐标是否为偶数
				if (flag && food.x % 2 == 0) {
					break;
				}
		}
		// 把食物画出来
		gotoxy(food.x, food.y);
		printf("*");
		// 蛇的长度加长
		snake.len++;
		// 蛇的标记是1
		changeFlag = 1;
	}
}


/////////////
// 3.按键操作
void keyDown()
{
	// 无按键的处理(???)
	if (_kbhit()) {
		// 有按键(???)
		fflush(stdin);
		key = _getch();
	}
	// 擦除
	if( !changeFlag){ // 无按键，意味着蛇没有方向上的变化
		gotoxy(snake.x[snake.len - 1], snake.y[snake.len - 1]);
		printf("  ");
	}
	// 后面的结束,因为蛇的移动就是后面的身体替换前面的身体，并擦掉最后的身体
	for (int i = snake.len - 1; i > 0; i--) {
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}
	// 移动方向的处理
	switch (key) {
		case 'w': // 往上走的实质是 y减
		case 'W': 
			snake.y[0]--;
			break;
		case 's': // 往下走得实质是 y加
		case 'S':
			snake.y[0]++;
			break;
		case 'a': // 往左走得实质是 x减
		case 'A':
			snake.x[0] -= 2;  // 一个符号占用两个字符
			break;
		case 'd': // 往右走得实质是 x加
		case 'D':
			snake.x[0] += 2;  // 一个符号占用两个字符
			break;
	}
	// 去到新的蛇头位置，打印出来
	gotoxy(snake.x[0], snake.y[0]);
	printf("■");
	// 每次移动完后，把移动标记置为0
	changeFlag = 0;
	// 然后把光标移开，因为移动不能一直看着光标嘛，不美观
	gotoxy(MAPHEIGHT + 2, 0);  

}

////////////////////////////
// 4.蛇的状态:判断是否结束游戏
int snakeStatus()
{
	// 判断左右边界、上下边界
	if (snake.x[0] == 0 || snake.x[0] == MAPWIDTH || snake.y[0] == 0 || snake.y[0] == MAPHEIGHT)
	{
		return 0;
	}
	// 蛇头不能撞自己
	for (int k = 1; k < snake.len; k++) {
		if (snake.x[0] == snake.x[k] && snake.y[0] == snake.y[k]){
			return 0;
		}
	}
	return 1;
}




