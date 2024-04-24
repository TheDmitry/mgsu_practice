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

#define ENTER 13
#define ESC 27
#define UP 72
#define DOWN 80
#define COUNT_MENU_ELEMENTS 9

char dan[COUNT_MENU_ELEMENTS][65]={
	"1) Какая книга имеет наибольший рейтинг?          ",
	"2) Какая книга опубликована раньше всех?          ",
	"3) Список книг, с рейтингом более 8.0             ",
	"4) Алфавитный список всех книг                    ",
	"5) Алфавитный список всех книг, но наоборот       ",
	"6) Количества книг, в которых более 200 страниц   ",
	"7) Диаграмма. Процентное соотношение продаж       ",
	"8) Сложный вопрос (Set by Pages)                  ",
	"8) Выход                                          "
};

char BlankLine[80];

int NC;
/*
	

*/
struct z {
	char name[30];
	char date[11];
	float rating;
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
} *list;

int menu(int numberOfElements);
void max(struct z* books);
void first(struct z* books);
void textData(char *,char *);
void count(struct z* books);
void alfalist(struct z* books);
void backAlfalist(struct z* books);
void put(struct z*,char* books);
void listing(struct z* books);
void diagram(struct z* books);

void getEqualBySales(struct z* books);

int main(array<System::String ^> ^args)
{
	int i,n;
	FILE *in;
	struct z *books;
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
		fscanf(in,"%s%s%f%ld%ld%ld", books[i].name, books[i].date, &books[i].rating, &books[i].price, &books[i].sales, &books[i].pages);
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
			case 1: max(books); break;
			case 2: first(books); break;
			case 3: count(books); break;
			case 4: alfalist(books); break;
			case 5: backAlfalist(books); break;
			case 6: listing(books); break;
			case 7: diagram(books); break;
			case 8: getEqualBySales(books); break;
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
	while (c!=ESC)
	{
		switch(c) {
			case DOWN: y2=y1; y1++; break;
			case UP: y2=y1; y1--; break;
			case ENTER: return y1+1;
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

void max(struct z* books) {

	int i = 0; struct z *best;
	best = &books[0];
	for(i=1;i<NC;i++) {
		if (books[i].rating > best->rating) best = &books[i];
	}

	Console::ForegroundColor=ConsoleColor::Yellow;
	Console::BackgroundColor=ConsoleColor::Black;
	Console::CursorLeft=10;
	Console::CursorTop=15;
	printf("Максимальный рейтинг %.1f баллов имеет книга %s", best->rating, best->name);
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


void first(struct z* books) {

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

void listing(struct z* books) {

	char s[17];
	

	Console::ForegroundColor=ConsoleColor::Yellow;
	Console::BackgroundColor=ConsoleColor::Black;

	Console::Clear();

	for (int i = 0; i< NC; i++) {
		if (books[i].rating > 8.0) {
			textData(s, books[i].date);
			printf("\tКнига %s от %s с рейтингом %.1f\r\n", books[i].name, s, books[i].rating);
		}
	}



	getch();

}

void alfalist(struct z* books) {

	int i;
	struct sp* nt;
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::Gray;
	Console::Clear();
	if(!list) {
		for(i=0;i<NC;i++) put(books, books[i].name);
	}
	
	Console::Clear();
	printf("\n Алфавитный список книг");
	printf("\n ===============================\n");
	for(nt=list; nt!=0; nt=nt->next)
		printf("%-30s %ld\r\n",nt->name, nt->sales);
	getch();

}

void backAlfalist(struct z* books) {

	int i;
	struct sp* nt;
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::Gray;
	Console::Clear();
	if(!list) {
		for(i=0;i<NC;i++) put(books, books[i].name);
	}
	
	Console::Clear();
	printf("\n Алфавитный список книг, но наоборот");
	printf("\n ===============================\n");
	for(nt=list; nt->next!=0; nt=nt->next);
	for(; nt!=0; nt=nt->previous)
		printf("%-30s %ld\r\n",nt->name, nt->sales);
	getch();

}

void put(struct z* books, char* name)
{

	int i;
	struct sp *nov, *nt,*z = NULL;

	for(nt = list; nt!= NULL && strcmp(nt->name, name)<0; z=nt, nt=nt->next);

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
		list=nov;
	}
	else {
		z->next = nov;
		nov->previous = z;
		if (nt) nt->previous = nov;
	}
	return;

}

void count(struct z* books)
{

	long count = 0;

	for (int i = 0; i < NC; i++)
		if (books[i].pages > 200) count++;

	Console::ForegroundColor=ConsoleColor::Yellow;
	Console::BackgroundColor=ConsoleColor::Black;
	Console::CursorLeft=10;
	Console::CursorTop=15;
		
	printf("Количество книг, в которых более 200 страниц: %ld шт", count);
	
	
	getch();

}

void diagram(struct z *books)
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
	
	

	if(!list)
		for(i=0;i<NC;i++)
			put(books, books[i].name);
	
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

void getEqualBySales(struct z* books) {
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
