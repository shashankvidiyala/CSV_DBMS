#include<stdio.h>
#include<iostream>
#include "fileloads.h"
#include "Hasmapi.h"
using namespace std;
/*
Feautures
1)parse csv files of student marks and info store it dynamically
2)find avg,cgpa of student marks
3)view values of tables, also joined table
4)Save Udated filses in csv form
5)Query on data as syntax select ___ from _____ where ____
eg select * from marks where avg ge 70

'*'-> select all values
schema-marks(rno,name,m1,m2,m3,m4,avg,cgpa) ,info(rno,addr,city,phn,pin)
operations:
int,floats-:ge,lt,le,gt,eq
strings-: contains,startswith


*/
int main(int argc, char* argv[]){
	int i;
	char s[100];
	initfls();
	init_hmap();
	while (1){
		printf("1.load\n2.show\n3.avg\n4cgpa\n5.save\n6.query\n7.Exit\n");
		cin >> i;
		if (i == 1){
			printf("1.Student\n2.StudentInfo\n");
			cin >> i;
			printf("File name?\n");
			cin >> s;
			if (i == 1)
				loadfile(s);
			else if (i == 2)
				loadinfo(s);
		}
		else if (i == 2){
			printf("1.Student\n2.StudentInfo\n3.join\n");
			cin >> i;
			if (i == 1)
				showrec();
			else if (i == 2)
				showinfo();
			else if (i == 3)
				njoin();
			else;
		}
		else if (i == 3){
			computeavg();
		}
		else if (i == 4){
			computecgpa();
		}
		else if (i == 5){
			printf("1.Student\n2.StudentInfo\n");
			cin >> i;
			printf("File name?\n");
			cin >> s;
			if (i == 1)
				savefiles(s);
			else if (i == 2)
				saveinfo(s);
		}
		else if (i == 6){
			cout << "Enter Query in the form: select ___ from _____ where ____\n";
			fflush(stdin);
			gets(s);
			//queryevaluator(s);
			preprocessor(s,i-1);
		}
		else if (i == 7){
			cout << "Hashing?\n";
			hashqueries();
		}
		else
			break;
	}
	fflush(stdin);
}