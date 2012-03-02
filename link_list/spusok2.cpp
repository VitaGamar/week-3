// spusok.cpp : Defines the entry point for the console application.
//Динамічний двозв'язний список товарів та їхніх цін. Впорядкований за зростанням цін товарів. 
//Автоматичне видалення зі списку всіх товарів, ціна яких >50
#include "stdafx.h"
#include<iostream>
#include<string.h>
#include <stdio.h>
#include <stdarg.h>
#include<iomanip>
using namespace std;
#define MAX_LEN 20

struct data{			//структура інформаційного  поля
	char* tov;			// назва товару
	double price;		// цына товару
};

struct list{			//структра елемента списку
	data inf;
	list* prev, *next;
};

	list* last;			// вказівники початку і кінця списку
	list* first;
	char* InputData(data* info);				   //введення даних в структуру data
	void InsertElem(data& inform);					 //занесення в список на потрібне місце
	void AddElem(list *pnew, list *pold);			 // додавання нового елемента в структуру
	double AddFew(int n,...);
	void PrintList();								 // роздрук списку
	list* DeleteElem (list* pdel);					//видалення елемента, на який вказує pdel
	void DeleteElem(char* s);						//перевантаження. Видалення елементу зі списку за назвою товару(товар вводить користувач)
	void DeleteMore(unsigned int maxbord=50);		// видалення елементів де ціна є більшою за maxbord (за замовчуванням 50)
	void Del();										//звільнення ДП

int _tmain(int argc, _TCHAR* argv[])
{
	data input;  
	char* (*func) (data*);		// оголошення вказівника на функцію
	func=InputData;				// ініціалізація вказівника
	cout<<" \n\t Entering data about products. \n\t To exit press 0\n";
	while(strcmp(func(&input),"0"))		//цикл формування списку. Ознака виходу-введеня нуля в назву товару
		InsertElem(input);
	cout<<"\n\t The list: ";
	PrintList();		//виведення сформованого списку
	DeleteMore();		//видалення зі списку елементів де ціна >50
	cout<<"\n\t List after removal: ";
	PrintList();		//скорочений список
	char ch;
	do
	{
		cout<<"\n \n\t  1 - print list \n\t  + - add an item to the list \n\t  s - sum first elements "
			"\n\t  - - to remove an item from the list \n\t  0 -  exit \n\t\t Make your choice   ";
		cin>>ch;
		if(ch=='+')
			while(strcmp(InputData(&input),"0"))
					InsertElem(input);
		if(ch=='-')
		{
			char temp[MAX_LEN];
			cout<<" \n Enter the name of the product which must be removed from the list: "; 
			cin>>temp;
			DeleteElem(temp);
		}
		if(ch=='1') 
		{ 
			DeleteMore(); 
			PrintList(); 
		}
		if (ch=='s')
		{	if((first!=NULL)&&(first->next!=NULL))
			{	
				list* temp=first->next;
				cout<<"\n\t Sum of the first two elements: "<<AddFew(2,first->inf.price, temp->inf.price)<<endl;
				if(temp->next!=NULL)
					cout<<"\t Sum of the first three elements: "<<AddFew(3,first->inf.price, temp->inf.price, temp->next->inf.price)<<endl;
			}
			else
				cout<<"\n\t Not enough elements! you must have two or more elements";
		}
	}
	while(ch!='0');
	Del();
	return 0;
}

// введення даних елемента списку
char* InputData(data* info)
{
	static int num=0;
	char buf[MAX_LEN];
	cout<<++num<<"-element. Product name : ";
	cin>>buf;
	if(!strcmp(buf,"0"))	 //кiнець введення - товар 0
		return buf;			
	info->tov= new char;	//якщо значення не нульве - виділяєм місце в ДП
	strcpy(info->tov, buf);	// і копіюємо туди нашу стрічку
	cout<<" Price: ";
	cin>>info->price;
	return info->tov;
}

// приєднання нового елемента в порядку зростання цін
void InsertElem(data& inform)
{
	list *pel, *pins;
	pel= new list;		//виділення ДП
	pel->inf=inform;	//заповнення поля даними
	if(first==NULL)		//якщо в списку ще не було елементів 
	{
		pel->next=pel->prev=NULL;
		first=last=pel;	//єдиний елемент списку
		return;
	}
	pins=first;
	while(pins!=NULL && inform.price>pins->inf.price)	//цикл пошуку позиції вставлення нового елементу
		pins=pins->next;
	AddElem(pel, pins);		//приєднання нового елементу перед *pins
}

// приєднання до списку елемента *pnew перед елементом *pold
void AddElem(list* pnew, list *pold)
{
	if(pold==first) //новий елемент стає першим у списку
	{
		first->prev=pnew;
		pnew->next=first;
		pnew->prev=NULL;
		first=pnew;
		return;
	}
	if(pold!=NULL) // новий елемент вводиться у середину списку
	{
		pnew->next=pold;
		pnew->prev=pold->prev;
		pold->prev->next=pnew;
		pold->prev=pnew;
	}
	else //новий елемент стає останнім у списку
	{
		last->next=pnew;
		pnew->prev=last;
		pnew->next=NULL;
		last=pnew;
	}
}
//виведення елементів списку
void PrintList()
{
	list* pel=first;
	cout<<"\nPrice     Product\n";
	while(pel!=NULL)
	{
		cout.setf(ios::left);
		cout<<setw(10)<<pel->inf.price<<setw(15)<<pel->inf.tov<<endl;
		pel=pel->next;
	}
}
// сума n чисел типу double
double AddFew(int n,...)
{
	va_list ap;
	double sum=0;
	va_start (ap,n);
	for(int i=0;i<n;i++)
		sum+=va_arg(ap, double);
	va_end(ap);	
	return sum;
}

// видалення елемента(повертає вказівник на наступний за видаленим елемент)
list* DeleteElem(list* pdel)
{
	if(pdel==first) //видалення першого елемента списку
	{
		first=first->next;
		first->prev=NULL;
		delete pdel;
		return first;;
	}
	if(pdel==last) //видалення останнього елемента списку
	{
		last=last->prev;
		last->next=NULL;
		delete pdel;
		return NULL;
	}
	else //видалення серединного елемента списку
	{
		list* pnext;
		pdel->next->prev=pdel->prev;
		pnext=pdel->prev->next=pdel->next;
		delete pdel;
		return pnext;
	}
}

// видалення всіх елементів списку де ціна більше 50
void DeleteMore(unsigned int maxborder)
{
	list* pel=first;
	while(pel!=NULL)
		if(pel->inf.price>maxborder)
			pel=DeleteElem(pel);
		else
			pel=pel->next;
}

//видалення елементів списку за назвою товару
void DeleteElem(char* s)
{
	list* pel=first;
	if(first==last)
	{
		first=NULL;
		cout<<" Deleted! \tList is empty! ";
		return;
	}
	while(pel!=NULL) 
	{	
		if (strcmp(pel->inf.tov,s)!=0)
			pel=pel->next;
		else 
		{
			DeleteElem(pel);
			cout<<" Removed! ";
			return;
		}
	}
	cout<<" Error! Please, try again!";
}

void Del()
{
	list* cur=first;
	while(cur!=NULL)
	{
		list* temp=cur;
		cur=cur->next;
		delete temp;
	}
}