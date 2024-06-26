// TheBestProjectEver.cpp: главный файл проекта.

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <clocale>


using namespace std;
using namespace System;
using namespace System::IO;

#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_ARROW_UP 72
#define KEY_ARROW_DOWN 80
#define KEY_HOME 71
#define KEY_END 79

#define COUNT_MENU_ELEMENTS 9

const char dan[COUNT_MENU_ELEMENTS][65]={
	"1) Какая книга имеет наибольший рейтинг?          ",
	"2) Какая книга опубликована раньше всех?          ",
	"3) Список книг, с рейтингом более N               ",
	"4) Алфавитный список всех книг                    ",
	"5) Алфавитный список всех книг, но наоборот       ",
	"6) Количества книг, в которых более N страниц   ",
	"7) Диаграмма. Процентное соотношение продаж       ",
	"8) Сложный вопрос (Set by Pages)                  ",
	"9) Выход                                          "
};

char BlankLine[80];

struct z {
	char name[30];
	char date[11];
	double rating;
	long price;
	long sales;
	long pages;
};
struct sp {
	struct sp* previous;
	char name[30];
	long sales;
	long gains;
	struct sp* next;
};

int menu(int numberOfElements);
void max(struct z* books, int NC);
void first(struct z* books, int NC);
void textData(char *,char *);
void count(struct z* books, int NC);
void alfalist(struct z* books, struct sp* list, int NC);
void backAlfalist(struct z* books, struct sp* list, int NC);
void put(struct z* books,char* name, int NC, struct sp** list);
void listing(struct z* books, int NC);
void diagram(struct z* books, struct sp* list, int NC);

void getSameByPages(struct z* books, int NC);

int main(array<System::String ^> ^args)
{
	int i,n, NC = 0;
	FILE *in;

	struct z *books;
	struct sp *list;



	setlocale(LC_CTYPE,"Russian");
	Console::CursorVisible::set(false);
	Console::BufferHeight = Console::WindowHeight;
	Console::BufferWidth = Console::WindowWidth;
	if((in=fopen("data/data.dat","r"))==NULL)
	{
		printf("\nФайл data.dat не открыт !");
		getch(); exit(1);
	}
	// Ввод из файла и контрольная печать
	fscanf(in,"%d",&NC);
	books = (struct z*) malloc(NC*sizeof(struct z));
	for(i=0;i<NC;i++)
		fscanf(in,"%s%s%lf%ld%ld%ld", books[i].name, books[i].date, &books[i].rating, &books[i].price, &books[i].sales, &books[i].pages);
	
	for (i = 0; i < NC; put(books, books[i].name, NC, &list), i++);

	while(1)
	{
		Console::ForegroundColor=ConsoleColor::Blue;
		Console::BackgroundColor=ConsoleColor::DarkBlue;
		Console::Clear();
		Console::ForegroundColor=ConsoleColor::Cyan;
		Console::BackgroundColor=ConsoleColor::Black;
		Console::CursorLeft=10;
		Console::CursorTop=4;
		printf(BlankLine);

		for(i=0;i < COUNT_MENU_ELEMENTS;i++)
		{
			Console::CursorLeft=10;
			Console::CursorTop=i+5;
			printf("%s",dan[i]);
		}
		Console::CursorLeft=10;
		Console::CursorTop=12;
		printf(BlankLine);
		n = menu(COUNT_MENU_ELEMENTS);
		switch(n) {
			case 1: max(books, NC); break;
			case 2: first(books, NC); break;
			case 3: listing(books, NC); break;
			case 4: alfalist(books, list, NC); break;
			case 5: backAlfalist(books, list, NC); break;
			case 6: count(books, NC); break;
			case 7: diagram(books, list, NC); break;
			case 8: getSameByPages(books, NC); break;
			case 9: exit(0);
			default: break;
		}

	}
	return 0;
}

int menu(int numberOfElements)
{
	int y1=0,y2 = numberOfElements - 1;
	char c=1;
	while (c!=KEY_ESC)
	{
		switch(c) {
			case KEY_ARROW_DOWN: y2=y1; y1++; break;
			case KEY_ARROW_UP: y2=y1; y1--; break;
			case KEY_ENTER: return y1+1;
			case KEY_HOME: {
					if (y1 == 0) break;
					y2 = y1;
					y1 = 0;
					break;
				}
			case KEY_END: {
					if (y1 == COUNT_MENU_ELEMENTS -1) break;
					y2 = y1;
					y1 = COUNT_MENU_ELEMENTS - 1;
					break;
				}
		}
		if(y1> numberOfElements -1){y2= numberOfElements -1;y1=0;}
		if(y1<0) {y2=0;y1=numberOfElements - 1;}

		Console::ForegroundColor=ConsoleColor::White;
		Console::BackgroundColor=ConsoleColor::Black;
		Console::CursorLeft=10;
		Console::CursorTop=y1+5;
		printf("%s", dan[y1]);
		Console::ForegroundColor=ConsoleColor::Cyan;
		Console::BackgroundColor=ConsoleColor::Black;
		Console::CursorLeft=10;
		Console::CursorTop=y2+5;
		printf("%s", dan[y2]);
		c = getch();
	}
	exit(0);
}

void max(struct z* books, int NC) {

	int i = 0; struct z *best;
	best = &books[0];
	for(i=1;i<NC;i++) {
		if (books[i].rating > best->rating) best = &books[i];
	}

	Console::ForegroundColor=ConsoleColor::Yellow;
	Console::BackgroundColor=ConsoleColor::Black;
	Console::CursorLeft=10;
	Console::CursorTop=15;
	printf("Максимальный рейтинг %.1lf баллов имеет книга %s", best->rating, best->name);
	getch();
}

void textData(char *s,char *sd) {
	char s0[3],
		month[12][9] = {
			"января","февраля","марта","апреля","мая","июня",
			"июля","августа","сентября","октября","ноября","декабря"
	};
	strcpy(s, sd+8);
	strcat(s," ");
	strncpy(s0,sd+5,2); s0[2]=0;
	strcat(s, month[ atoi(s0) - 1]);
	strcat(s," ");
	strncat(s,sd,4);
	return;
}


void first(struct z* books, int NC) {

	int i;
	char s[17];
	struct z* best = &books[0];
	for(i=1;i<NC;i++) {
		if (strcmp(books[i].date, best->date) < 0) best = &books[i];
	}

	textData(s, best->date);

	Console::ForegroundColor=ConsoleColor::Yellow;
	Console::BackgroundColor=ConsoleColor::Black;
	Console::CursorLeft=10;
	Console::CursorTop=15;
	printf("Книга %s была опубликована раньше всех, %s", best->name, s);
	getch();

}

void listing(struct z* books, int NC) {

	char s[17];
	float rating;

	Console::ForegroundColor=ConsoleColor::Yellow;
	Console::BackgroundColor=ConsoleColor::Black;
	
	Console::Write("\tВведите необходимый рейтинг: ");
	rating = Convert::ToDouble(Console::ReadLine());
	

	Console::Clear();

	for (int i = 0; i< NC; i++) {
		if (books[i].rating > rating) {
			textData(s, books[i].date);
			printf("\r\n\tКнига %s от %s с рейтингом %.1lf\r\n", books[i].name, s, books[i].rating);
		}
	}



	getch();

}

void alfalist(struct z* books, struct sp* list, int NC) {

	int i;
	struct sp* nt;
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::Gray;
	Console::Clear();
	
	Console::Clear();
	printf("\n Алфавитный список книг");
	printf("\n ===============================\n");
	for(nt=list; nt!=0; nt=nt->next)
		printf("%-30s %ld\r\n",nt->name, nt->sales);
	getch();

}

void backAlfalist(struct z* books, struct sp* list, int NC) {

	int i;
	struct sp* nt;
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::Gray;
	Console::Clear();
	
	Console::Clear();
	printf("\n Алфавитный список книг, но наоборот");
	printf("\n ===============================\n");
	for(nt=list; nt->next!=0; nt=nt->next);
	for(; nt!=0; nt=nt->previous)
		printf("%-30s %ld\r\n",nt->name, nt->sales);
	getch();

}

void put(struct z* books, char* name, int NC, struct sp **list)
{

	int i;
	struct sp *nov, *nt, *z = NULL;

	for(nt = *list; nt!= NULL && strcmp(nt->name, name)<0; z=nt, nt=nt->next);

	if(nt && strcmp(nt->name, name)==0) return;
	// z -> nov -> nt
	// z->next = nov;
	// nov->previous = z;
	nov = (struct sp *) malloc(sizeof(struct sp));
	strcpy(nov->name, name);
	nov->next = nt;
	nov->sales = 0;
	nov->gains = 0;
	nov->previous = 0;


	for(i=0;i<NC;i++) {
		if(strcmp(books[i].name, name)==0) {
			nov->sales += books[i].sales;
			nov->gains += books[i].price * books[i].pages;
		}
	}

	if(!z){
		*list=nov;
	}
	else {
		z->next = nov;
		nov->previous = z;
		if (nt) nt->previous = nov;
	}
	return;

}

void count(struct z* books, int NC)
{

	long count = 0;
	int amountOfPages = 0;

	Console::ForegroundColor=ConsoleColor::Yellow;
	Console::BackgroundColor=ConsoleColor::Black;
	Console::CursorLeft=10;
	Console::CursorTop=15;

	Console::Write("Введите необходимое количество страниц: ");

	scanf("%d", &amountOfPages);


	for (int i = 0; i < NC; i++)
		if (books[i].pages > amountOfPages) count++;


	Console::CursorLeft=10;
	Console::CursorTop=16;

	printf("Количество книг, в которых более %d страниц: %ld шт", amountOfPages, count);
	
	
	getch();

}

void diagram(struct z* books, struct sp* list, int NC)
{
	struct sp *nt;
	int len,i,NColor;
	long long totalGains = 0 ;
	char str1[30];
	char str2[30];

	System::ConsoleColor Color;
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::White;
	Console::Clear();
	
	
	for(nt = list; nt!=0; totalGains += nt->gains, nt=nt->next);

	Color=ConsoleColor::Black; NColor=0;

	for(nt=list,i=0; nt!=0; nt=nt->next, i++)
	{
		sprintf(str1,"%-30s",nt->name);
		sprintf(str2,"%3.1f%%",(nt->gains*100./totalGains));

		Console::ForegroundColor = ConsoleColor::Black;
		Console::BackgroundColor = ConsoleColor::White;
		Console::CursorLeft = 5;
		Console::CursorTop = i+1;

		printf("%s", str1);
		
		Console::CursorLeft=40;
		
		printf("%s",str2);
		
		Console::BackgroundColor=++Color; NColor++;
		Console::CursorLeft=50;
		
		for(len=0; len < nt->gains*100/totalGains; len++) printf(" ");
		if(NColor == 14) {
			Color=ConsoleColor::Black;
			NColor=0;
		}
	}
	getch();
	return ;
}

void getSameByPages(struct z* books, int NC) {
	int i, j;

	struct z *sameElement, *thisElement = 0;
	
	Console::Clear();
	Console::ForegroundColor=ConsoleColor::Yellow;
	Console::BackgroundColor=ConsoleColor::Black;
	Console::CursorLeft=10;
	Console::CursorTop=15;



	for (i = 0; i < NC; i++) {
		thisElement = &books[i];

		for (j = i + 1; j < NC; j++) {
			if (thisElement->sales == books[j].sales) {
				sameElement = &books[j];
				break;
			}
		}
		if (sameElement != 0) break;
	}
	
	if (sameElement == 0) {
		printf("Совпадений не найдено");
	} else {
		printf("По книге %s было найдено совпадение, книга %s", thisElement->name, sameElement->name);
	}

	getch();
	return;
}
