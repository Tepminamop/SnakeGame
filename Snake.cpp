#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include <dos.h>
#include <time.h>

#define MAXSNAKESIZE 100
#define MAXFRAMESIZEX 119
#define MAXFRAMESIZEY 29

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);//"захватываем" консоль
COORD CursorPosition;//координата в окне консоли

void gotoxy(int x, int y)//меняем координаты у COORD CursorPosition и устанавливаем их в консоль, которую захватили хэндлом
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

class Point
{
private:
	int x;
	int y;
public:
	Point()
	{
		x = y = 10;
	}

	Point(int _x, int _y)
	{
		this->x = _x;
		this->y = _y;
	}

	void SetPoint(int _x, int _y)
	{
		this->x = _x;
		this->y = _y;
	}

	int GetX()
	{
		return this->x;
	}

	int GetY()
	{
		return this->y;
	}

	void MoveUp()
	{
		this->y--;
		if (y < 0)
			y = MAXFRAMESIZEY;
	}

	void MoveDown()
	{
		this->y++;
		if (y > MAXFRAMESIZEY)
			y = 0;
	}

	void MoveRight()
	{
		this->x++;
		if (x > MAXFRAMESIZEX)
			x = 0;
	}

	void MoveLeft()
	{
		this->x--;
		if (x < 0)
			x = MAXFRAMESIZEX;
	}

	void Draw()//рисуем змейку
	{
		gotoxy(x, y);
		std::cout << "*";
	}

	void Erase()
	{
		gotoxy(x, y);
		std::cout << " ";
	}

	void Debug()
	{
		std::cout << "(" << x << ", " << y << ")" << std::endl;
	}

	void CopyPosition(Point* p)
	{
		p->x = x;
		p->y = y;
	}

	bool isEqual(Point* p)
	{
		if (this->x == p->x && this->y == p->y)
			return true;
		return false;
	}
};

class Snake
{
private:
	Point* cell[MAXSNAKESIZE];//инициализация массива точек - это змея
	char dir = 'l';//направление движения змеи
	int size;//размер змеи
	Point fruit;//еда для змеи
	bool alive = true;//жива ли змея
public:
	Snake()
	{
		size = 1;
		*cell = new Point(20, 20);//положение головы змеи в самом начале
		for (int i = 1; i < MAXSNAKESIZE; i++)
		{
			cell[i] = nullptr;
		}
		fruit.SetPoint(rand() % MAXFRAMESIZEX, rand() % MAXFRAMESIZEY);
	}

	void AddCell(int _x, int _y)
	{
		cell[size++] = new Point(_x, _y);
	}

	void TurnUp()
	{
		if (dir != 's')
			dir = 'w';
	}

	void TurnDown()
	{
		if (dir != 'w')
			dir = 's';
	}

	void TurnLeft()
	{
		if (dir != 'd')
			dir = 'a';
	}

	void TurnRight()
	{
		if (dir != 'a')
			dir = 'd';
	}

	bool selfCollision()
	{
		for (int i = 1; i < size; i++)
		{
			if (cell[0]->isEqual(cell[i]))
				return true;
		}
		return false;
	}

	void Move()
	{
		system("cls");

		if (selfCollision())
		{
			std::cout << "You died...";
			exit(0);
		}

		for (int i = size - 1; i > 0; i--)
		{
			cell[i - 1]->CopyPosition(cell[i]);
		}

		switch (dir)
		{
		case 'w':
			cell[0]->MoveUp();
			break;
		case 'a':
			cell[0]->MoveLeft();
			break;
		case 's':
			cell[0]->MoveDown();
			break;
		case 'd':
			cell[0]->MoveRight();
			break;
		}

		if (fruit.GetX() == cell[0]->GetX() && fruit.GetY() == cell[0]->GetY())
		{
			AddCell(0, 0);
			fruit.SetPoint(rand() % MAXFRAMESIZEX, rand() % MAXFRAMESIZEY);
		}

		for (int i = 0; i < size; i++)
		{
			cell[i]->Draw();
		}
		fruit.Draw();
		Sleep(50);//после завершения изменения положения змейки ждем 100 мс, а затем снова входим в цикл while в main (это регулировка скорости змеи)
	}
};

int main()
{
	srand((unsigned)time(NULL));
	Snake snake = Snake();
	char op = 't';
	do
	{
		if (_kbhit())//если нажата клавиша
		{
			op = _getch();//считывание значение этой нажатой клавиши
		}

		switch (op)//смена направления движения змеи
		{
		case 'w':
		case 'W':
			snake.TurnUp();
			break;
		case 'a':
		case 'A':
			snake.TurnLeft();
			break;
		case 's':
		case 'S':
			snake.TurnDown();
			break;
		case 'd':
		case 'D':
			snake.TurnRight();
			break;
		}
		snake.Move();
	} while (op != 'e');//конец
	return 0;
}