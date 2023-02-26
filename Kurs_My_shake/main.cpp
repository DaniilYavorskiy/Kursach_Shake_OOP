#include <iostream>
#include <conio.h>
#include <windows.h>
#include <initializer_list>

using namespace std;

class set { //абстрактный класс
protected:
	virtual void Setup() = 0;
};

class field {
public:
	int width, height;
	field(){ // перегрузки
	this -> width = { 40 }; //инициализация 
	this -> height = { 20 }; //инициализация 
	}	
};

bool printTail;
bool gameOver;
int x, y, fruitX, fruitY, fruit2X, fruit2Y, fruit3X, fruit3Y, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN }; //направления змейки
eDirection dir;
enum ConsoleColor { Cyan = 1, Red = 2, Green = 3 };

// устанавливаем цвет текста и подложки в консоли
void SetColor(int text, int background)  // конструктор
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

class set_snake : public field { // унаследуем от класса field новый класс set_shake 
public:
	void Setup() {
		gameOver = false;
		dir = STOP;
		x = width / 2; //спаун головы змеи в середине поля
		y = height / 2; //спаун головы змеи в середине поля
		fruitX = rand() % width;
		fruitY = rand() % height;
		fruit2X = rand() % width;
		fruit2Y = rand() % height;
		fruit3X = rand() % width;
		fruit3Y = rand() % height;
		score = 0; //начальное значение очков игрока
	}
};

class body : public set_snake {  // унаследуем от класса set_shake новый класс body
public:
	void Draw(int i, int j) { // конструктор
		Sleep(250);    //скорость игры
		system("cls");
		for (i = 0; i < width + 3; i++)
		cout << "|";
		cout << endl;

		for ( i = 0; i < height; i++) {
			for ( j = 0; j < width; j++) {
				if (j == 0 || j == width - 1)
					cout << "||";
				if (i == y && j == x)
					cout << "0";
				else if (i == fruitY && j == fruitX)
					cout << "*";
				else if (i == fruit2Y && j == fruit2X)
					cout << "*";
				else if (i == fruit3Y && j == fruit3X)
					cout << "*";
				else {
					printTail = false;
					for (int k = 0; k < nTail; k++) {
						if (tailX[k] == j && tailY[k] == i) {
							printTail = true;
							cout << "o";
						}
					}
					if (!printTail)
						cout << " ";
				}
			}
			cout << endl;
		}

		for (i = 0; i < width + 3; i++)
			cout << "|";
		cout << endl;
		for (j = 0; j < 1; j++)
		{
			SetColor(3, j);
			cout << "		YOUR SCORE: " << score << endl;
		}

	}
};


class CONTROL {
public:
	void Input() {
		if (_kbhit()) {
			switch (_getch()) {
				// управление с помощью стрелок
			case 72:
				dir = UP;
				break;
			case 80:
				dir = DOWN;
				break;
			case 75:
				dir = LEFT;
				break;
			case 77:
				dir = RIGHT;
				break;

				// управление с помощью букв
			case 'a':
				dir = LEFT;
				break;
			case 'd':
				dir = RIGHT;
				break;
			case 'w':
				dir = UP;
				break;
			case 's':
				dir = DOWN;
				break;
			case 'x': // клавиша для выхода в меню
				gameOver = true;
				break;
			case 'A':
				dir = LEFT;
				break;
			case 'D':
				dir = RIGHT;
				break;
			case 'W':
				dir = UP;
				break;
			case 'S':
				dir = DOWN;
				break;
			case 'X': // клавиша для выхода в меню
				gameOver = true;
				break;
			}
		}

	}
};

class LOGIKA : public field {  // унаследуем от класса field новый класс LOGIKA
private:
	int prevX{}, prevY{};
public:	
	LOGIKA() : prevX{ tailX[0] }, prevY{ tailY[0] } {} // список иницилизации
		void Logic() {
		tailX[0] = x; 
		tailY[0] = y; 
		int prev2X, prev2Y;

		for (int i = 1; i < nTail; i++) {
			prev2X = tailX[i];
			prev2Y = tailY[i];
			tailX[i] = prevX;
			tailY[i] = prevY;
			prevX = prev2X;
			prevY = prev2Y;
		}

		switch (dir) {
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case UP:
			y--;
			break;
		case DOWN:
			y++;
			break;
		}

		//игра кончается при достижении границ змейкой
		if (x >= width - 1 || x < 0 || y >= height - 1 || y < 0)
		gameOver = true;

		if (x >= width)
			x = 0;
		else if (x < 0)
			x = width - 1;

		if (y >= height)
			y = 0;
		else if (y < 0)
			y = height - 1;

		for (int i = 0; i < nTail; i++) {
			if (tailX[i] == x && tailY[i] == y)
				gameOver = true;
		}

		if (x == fruitX && y == fruitY) {
			score += 10;
			fruitX = rand() % width;
			fruitY = rand() % height;
			nTail++;
		}

		if (x == fruit2X && y == fruit2Y) {
			score += 10;
			fruit2X = rand() % width;
			fruit2Y = rand() % height;
			nTail++;
		}

		if (x == fruit3X && y == fruit3Y) {
			score += 10;
			fruit3X = rand() % width;
			fruit3Y = rand() % height;
			nTail++;
		}
	}
};


class MENU : public CONTROL {  // Публично унаследуем от класса CONTROL новый класс MENU
private:
	int i, j;
public:
	void menu() {
		for (i = 0; i < 2; i++)
		{
			for (j = 0; j < 2; j++)
			{
				SetColor(i, 2);
				cout << "\n						       || GAME SNAKE ||      \n";
				cout << endl;
				cout << endl;
				SetColor(3, j);
				cout << "						MENU (";
				SetColor(2, j);
				cout << "press number for select";
				SetColor(3, j);
				cout << "): \n";
				cout << endl;
				SetColor(2, j);
				cout << "						        1.";
				SetColor(3, j);
				cout << " New Game \n";
				cout << endl;
				SetColor(2, j);
				cout << "						         2. ";
				SetColor(3, j);
				cout << " Exit";
				cout << endl;
				while (1) {
					if (_kbhit()) {
						switch (_getch()) {
						case '1': {
							set_snake objSetup;
							objSetup.Setup();
							while (!gameOver) {
								SetColor(4, j);
								body objSetup;
								objSetup.Draw(i, j);
								CONTROL control;
								control.Input();
								LOGIKA logika;
								logika.Logic();
							}
							system("cls");
							gameOver = false;
							SetColor(4, j);
							cout << "\n					      GAME OVER: YOU KILLED YOUR SNAKE!!!\n" << endl;
							SetColor(2, j);
							cout << "							YOUR SCORE:  " << score << endl;
							menu();}
							break;

						case '2': {
							exit(0); 
						}
							break;
						}
					}
				}
			}
		}
	}
};

int main() {
	MENU objSetup;
	objSetup.menu();
}