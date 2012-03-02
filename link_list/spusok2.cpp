// spusok.cpp : Defines the entry point for the console application.
//��������� �����'����� ������ ������ �� ���� ���. ������������� �� ���������� ��� ������. 
//����������� ��������� � ������ ��� ������, ���� ���� >50
#include "stdafx.h"
#include<iostream>
#include<string.h>
#include <stdio.h>
#include <stdarg.h>
#include<iomanip>
using namespace std;
#define MAX_LEN 20

struct data{			//��������� ��������������  ����
	char* tov;			// ����� ������
	double price;		// ���� ������
};

struct list{			//�������� �������� ������
	data inf;
	list* prev, *next;
};

	list* last;			// ��������� ������� � ���� ������
	list* first;
	char* InputData(data* info);				   //�������� ����� � ��������� data
	void InsertElem(data& inform);					 //��������� � ������ �� ������� ����
	void AddElem(list *pnew, list *pold);			 // ��������� ������ �������� � ���������
	double AddFew(int n,...);
	void PrintList();								 // ������� ������
	list* DeleteElem (list* pdel);					//��������� ��������, �� ���� ����� pdel
	void DeleteElem(char* s);						//��������������. ��������� �������� � ������ �� ������ ������(����� ������� ����������)
	void DeleteMore(unsigned int maxbord=50);		// ��������� �������� �� ���� � ������ �� maxbord (�� ������������� 50)
	void Del();										//��������� ��

int _tmain(int argc, _TCHAR* argv[])
{
	data input;  
	char* (*func) (data*);		// ���������� ��������� �� �������
	func=InputData;				// ����������� ���������
	cout<<" \n\t Entering data about products. \n\t To exit press 0\n";
	while(strcmp(func(&input),"0"))		//���� ���������� ������. ������ ������-������� ���� � ����� ������
		InsertElem(input);
	cout<<"\n\t The list: ";
	PrintList();		//��������� ������������ ������
	DeleteMore();		//��������� � ������ �������� �� ���� >50
	cout<<"\n\t List after removal: ";
	PrintList();		//���������� ������
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

// �������� ����� �������� ������
char* InputData(data* info)
{
	static int num=0;
	char buf[MAX_LEN];
	cout<<++num<<"-element. Product name : ";
	cin>>buf;
	if(!strcmp(buf,"0"))	 //�i���� �������� - ����� 0
		return buf;			
	info->tov= new char;	//���� �������� �� ������ - ������� ���� � ��
	strcpy(info->tov, buf);	// � ������� ���� ���� ������
	cout<<" Price: ";
	cin>>info->price;
	return info->tov;
}

// ��������� ������ �������� � ������� ��������� ���
void InsertElem(data& inform)
{
	list *pel, *pins;
	pel= new list;		//�������� ��
	pel->inf=inform;	//���������� ���� ������
	if(first==NULL)		//���� � ������ �� �� ���� �������� 
	{
		pel->next=pel->prev=NULL;
		first=last=pel;	//������ ������� ������
		return;
	}
	pins=first;
	while(pins!=NULL && inform.price>pins->inf.price)	//���� ������ ������� ���������� ������ ��������
		pins=pins->next;
	AddElem(pel, pins);		//��������� ������ �������� ����� *pins
}

// ��������� �� ������ �������� *pnew ����� ��������� *pold
void AddElem(list* pnew, list *pold)
{
	if(pold==first) //����� ������� ��� ������ � ������
	{
		first->prev=pnew;
		pnew->next=first;
		pnew->prev=NULL;
		first=pnew;
		return;
	}
	if(pold!=NULL) // ����� ������� ��������� � �������� ������
	{
		pnew->next=pold;
		pnew->prev=pold->prev;
		pold->prev->next=pnew;
		pold->prev=pnew;
	}
	else //����� ������� ��� ������� � ������
	{
		last->next=pnew;
		pnew->prev=last;
		pnew->next=NULL;
		last=pnew;
	}
}
//��������� �������� ������
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
// ���� n ����� ���� double
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

// ��������� ��������(������� �������� �� ��������� �� ��������� �������)
list* DeleteElem(list* pdel)
{
	if(pdel==first) //��������� ������� �������� ������
	{
		first=first->next;
		first->prev=NULL;
		delete pdel;
		return first;;
	}
	if(pdel==last) //��������� ���������� �������� ������
	{
		last=last->prev;
		last->next=NULL;
		delete pdel;
		return NULL;
	}
	else //��������� ����������� �������� ������
	{
		list* pnext;
		pdel->next->prev=pdel->prev;
		pnext=pdel->prev->next=pdel->next;
		delete pdel;
		return pnext;
	}
}

// ��������� ��� �������� ������ �� ���� ����� 50
void DeleteMore(unsigned int maxborder)
{
	list* pel=first;
	while(pel!=NULL)
		if(pel->inf.price>maxborder)
			pel=DeleteElem(pel);
		else
			pel=pel->next;
}

//��������� �������� ������ �� ������ ������
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