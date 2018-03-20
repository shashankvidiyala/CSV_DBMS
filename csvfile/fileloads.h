#include<string.h>
#include<iostream>
#include<stdlib.h>
#include "Hasmapi.h"
using namespace std;
struct student{
	char name[50];
	int rno, m1, m2, m3,m4;
	float avg=-1, cgpa=-1;
} **globalarr;
struct studentinfo{
	int rno;
	char mobil[15], addr[50], city[20];
	long long pin;
}**ginfoarr;
struct filter{
	int flag,jk;
	char param1[50], param2[50], param3[50];
	float ki1,ki2;
	int andflag;
};
int s = 0,si=0;
int maxsz = 10,maxszi=10;
char formati[255] = "RollNo,Name ,M1,M2,M3,M4,Avg,Cgpa\n\0";
char formatf[255] = "RollNo,MobileNumber,Address,City, Pin\n\0";

void updategarr();
void updateginfoarr();
int loadfile(char *str);
void insert1rec(char *str);
void showrec();
void showinfo();
void computeavg();
void computecgpa();
int  getgpa(int);
void savefiles(char *s);
void  getcsv(student,char*);
int loadinfo(char *);
void insert1info(char *str);
void print1combin(student, studentinfo);
void njoin();
void getcsvi(studentinfo,char*);
void saveinfos(char *s);
void checkexiststudent(int *);
void checkexiststinfo(int *);
int checkfileexists(char *);
void queryevaluator(char *s);
void printstudent(student);
void printstudentwithf(student,studentinfo, bool*);
studentinfo getcorrespon(student);
student getcorresponi(studentinfo);

//query functions
void getavgquer(float,char *);
void getavgquer(float, char *);
void getm1quer(float, char *);
void getm2quer(float, char *);
void getm3quer(float, char *);
void getm4quer(float, char *);
void getcgpaquer(float, char *);
void getnamequer(char*, char *);
int compar1qur(float k1, float k2, char *);
int compar1qur(char*,char *, char *);
int compar1qur(float k1, float k2, filter *ki,int sg );
int filtergenerator(filter *ft, int *sz, char *str);
int isstudentinq(student stu, filter ft);
int isstudentinq(studentinfo stu, filter ft);
void preprocessor(char*,int);
void evaluatefil(filter *ft, int sf, bool *arr2, int ty);
void inithashlists();
void hashqueries();
void inithashlists(){
	int j;
	char buf[4];
	for (int i = 0; i < si; i++){
		sprintf(buf, "%d", ginfoarr[i]->rno);
		insert_hmalt(ginfoarr[i]->city, buf, ",");
	}
}
void hashqueries(){
	inithashlists();
	int i;
	char buf[20],*c;
	while (1){
		cout << "cityname\n";
		cin >> buf;
		c = find(buf);
		if (c != NULL)
			printf("%s\n", c);
		else
			printf("No city Found\n");
		cout << "1 continue?\n";
		cin >> i;
		if (i == 0)
			break;
	}
}
int parsestringpr(char *given, char *buf1, char *buf2, int *ty);
int endpost(char *str1,int *start,int *end){
	int st = 0, i = 0, en = 0;
	while (str1[st] != '\0'&&str1[st] == ' ' || str1[st] == '\t' || str1[st] == ',')
		st++;
	i = st;
	while (str1[i] != '\0'&&str1[i] != ' '&&str1[i] != '\t'&&str1[i] != ',')
	{
		i++;
	}
	en = i;
	while (str1[en] != '\0'&&str1[en] == ' ' || str1[en] == '\t'||str1[en] == ',')
		en++;
	*start = st;
	*end = i;
	return en - i;


}
int endpostfor(char *str1, int *start, int *end,int *flag){
	int st = 0, i = 0, en = 0;
	while (str1[st] != '\0'&&str1[st] == ' ' || str1[st] == '\t')
		st++;
	i = st;
	while (str1[i] != '\0'&&str1[i] != '&'&&str1[i] != '|')
	{
		i++;
	}
	if (str1[i] == '&')
		*flag = 1;
	else if (str1[i] == '|')
		*flag = 2;
	en = i;
	while (str1[en] != '\0'&&str1[en] == ' ' || str1[en] == '\t')
		en++;
	*start = st;
	*end = i;
	return en - i;


}
int parsestringpr(char *given, char *buf1, char *buf2, int *ty){
	char temp1[100],temp2[100],*tk;
	int st=0, en=0,k;
	strcpy(temp2, given);
	strcpy(temp1, given);
	k = endpost(temp1, &st, &en);
	temp1[en] = '\0';
	tk = temp1 + st;
	if (strcmp(tk, "select") != 0){
		printf("Query should start with select\n");
		return -1;
	}
	tk = tk + en + k;
	k=endpost(tk, &st, &en);
	tk = tk + st;
	tk[en] = '\0';
	//strcpy(buf1, tk);
	while (*tk!='\0'&&strcmp("from", tk) != 0){
		insert_hmalt("proj", tk, ",");
		tk = tk + en + k;
		k = endpost(tk, &st, &en);
		tk = tk + st;
		tk[en] = '\0';
	}
	if (strcmp(tk, "from") != 0){
		printf("Query syntax error\n");
		return -1;
	}
	*ty = -1;
	tk = tk + en + k;
	k = endpost(tk, &st, &en);
	tk = tk + st;
	tk[en] = '\0';
	while (*tk != '\0'&&strcmp(tk, "where") != 0){
		if (strcmp(tk, "marks") == 0){
			if (*ty == -1)
				*ty = 0;
			else if (*ty == 1)
				*ty = 2;
		}
		else if (strcmp(tk, "info") == 0){
			if (*ty == -1)
				*ty = 1;
			else if (*ty == 0)
				*ty = 2;
		}
		tk = tk + en + k ;
		k = endpost(tk, &st, &en);
		tk = tk + st;
		tk[en] = '\0';
		

		
	}
	/*tk = tk + en + k;
	k = endpost(tk, &st, &en);
	tk = tk + st;
	tk[en] = '\0';
	*/
	if (strcmp(tk, "where") != 0){
		printf("Query syntax error\n");
		return -1;
	}
	tk = tk + en + k;
	k = endpost(tk, &st, &en);
	tk = tk + st;
	tk[en] = '\0';
	while (*tk != '\0'&&strcmp("orderby", tk) != 0){
		insert_hmalt("cond", tk, " ");
		if (strcmp("$", tk) == 0 || strcmp("&", tk) == 0){
			tk = tk + en + k;
			k = endpost(tk, &st, &en);
			tk = tk + st;
			tk[en] = '\0';
			continue;
		}
		tk = tk + en + k;
		k = endpost(tk, &st, &en);
		tk = tk + st;
		tk[en] = '\0';
		if (*tk == '\0'){
			printf("Invalid\n");
			return -1;
		}
		insert_hmalt("cond", tk, " ");
		tk = tk + en + k;
		k = endpost(tk, &st, &en);
		tk = tk + st;
		tk[en] = '\0';
		if (*tk == '\0'){
			printf("Invalid\n");
			return -1;
		}
		insert_hmalt("cond", tk, " ");
		tk = tk + en + k;
		k = endpost(tk, &st, &en);
		tk = tk + st;
		tk[en] = '\0';
	}
	if (strcmp("orderby", tk) == 0){
		printf("Future\n");
		return -1;
	}
	tk = tk + en + k;
	
	//insert_hm("cond", tk);
	tk = tk + en + k;
	
	return 1;





}
void getavgquer(float k,char *str){
	int i = 0, l = 20;
	char c;
	system("cls");
		printf("Rno\tname\tma\tm2\tm3\tm4\tavg\tcgpa\n");
		for (; i < s; i++){
			if (compar1qur(globalarr[i]->avg,k,str)==1){
				cout << globalarr[i]->rno << "\t" << globalarr[i]->name << "\t" << globalarr[i]->m1 << "\t" << globalarr[i]->m2 << "\t" << globalarr[i]->m3 << "\t" << globalarr[i]->m4 << "\t";
				if (globalarr[i]->avg == -1)
					cout << "NA\t";
				else
					cout << globalarr[i]->avg << "\t";
				if (globalarr[i]->cgpa == -1)
					cout << "NA\n";
				else
					cout << globalarr[i]->cgpa << "\n";

			}
		}
		
	

}
void getavgquer(float k, filter *art){
	int i = 0, l = 20;
	char c;
	system("cls");
	printf("Rno\tname\tma\tm2\tm3\tm4\tavg\tcgpa\n");
	for (; i < s; i++){
		if (compar1qur(globalarr[i]->avg, k, art,0) == 1){
			cout << globalarr[i]->rno << "\t" << globalarr[i]->name << "\t" << globalarr[i]->m1 << "\t" << globalarr[i]->m2 << "\t" << globalarr[i]->m3 << "\t" << globalarr[i]->m4 << "\t";
			if (globalarr[i]->avg == -1)
				cout << "NA\t";
			else
				cout << globalarr[i]->avg << "\t";
			if (globalarr[i]->cgpa == -1)
				cout << "NA\n";
			else
				cout << globalarr[i]->cgpa << "\n";

		}
	}


}
int compar1qur(float k1, float k2, filter *ki, int sg){
	for (int i = 0; i < sg; i++){
		if (ki[i].flag == 1){
			if (compar1qur(k1, k2, ki[i].param1) == 0)
				return 0;
		}
		else{
			if (compar1qur(ki[i].param2,ki[i].param3, ki[i].param1) == 0)
				return 0;
		}
	}
}
void getm1quer(float k, char *str){
	int i = 0, l = 20;
	char c;
	system("cls");
	printf("Rno\tname\tma\tm2\tm3\tm4\tavg\tcgpa\n");
	for (; i < s; i++){
		if (compar1qur(globalarr[i]->m1, k, str) == 1){
			cout << globalarr[i]->rno << "\t" << globalarr[i]->name << "\t" << globalarr[i]->m1 << "\t" << globalarr[i]->m2 << "\t" << globalarr[i]->m3 << "\t" << globalarr[i]->m4 << "\t";
			if (globalarr[i]->avg == -1)
				cout << "NA\t";
			else
				cout << globalarr[i]->avg << "\t";
			if (globalarr[i]->cgpa == -1)
				cout << "NA\n";
			else
				cout << globalarr[i]->cgpa << "\n";

		}
	}



}
void getm2quer(float k, char *str){
	int i = 0, l = 20;
	char c;
	system("cls");
	printf("Rno\tname\tma\tm2\tm3\tm4\tavg\tcgpa\n");
	for (; i < s; i++){
		if (compar1qur(globalarr[i]->m2, k, str) == 1){
			cout << globalarr[i]->rno << "\t" << globalarr[i]->name << "\t" << globalarr[i]->m1 << "\t" << globalarr[i]->m2 << "\t" << globalarr[i]->m3 << "\t" << globalarr[i]->m4 << "\t";
			if (globalarr[i]->avg == -1)
				cout << "NA\t";
			else
				cout << globalarr[i]->avg << "\t";
			if (globalarr[i]->cgpa == -1)
				cout << "NA\n";
			else
				cout << globalarr[i]->cgpa << "\n";

		}
	}



}
void getm3quer(float k, char *str){
	int i = 0, l = 20;
	char c;
	system("cls");
	printf("Rno\tname\tma\tm2\tm3\tm4\tavg\tcgpa\n");
	for (; i < s; i++){
		if (compar1qur(globalarr[i]->m3, k, str) == 1){
			cout << globalarr[i]->rno << "\t" << globalarr[i]->name << "\t" << globalarr[i]->m1 << "\t" << globalarr[i]->m2 << "\t" << globalarr[i]->m3 << "\t" << globalarr[i]->m4 << "\t";
			if (globalarr[i]->avg == -1)
				cout << "NA\t";
			else
				cout << globalarr[i]->avg << "\t";
			if (globalarr[i]->cgpa == -1)
				cout << "NA\n";
			else
				cout << globalarr[i]->cgpa << "\n";

		}
	}



}
void getm4quer(float k, char *str){
	int i = 0, l = 20;
	char c;
	system("cls");
	printf("Rno\tname\tma\tm2\tm3\tm4\tavg\tcgpa\n");
	for (; i < s; i++){
		if (compar1qur(globalarr[i]->m4, k, str) == 1){
			cout << globalarr[i]->rno << "\t" << globalarr[i]->name << "\t" << globalarr[i]->m1 << "\t" << globalarr[i]->m2 << "\t" << globalarr[i]->m3 << "\t" << globalarr[i]->m4 << "\t";
			if (globalarr[i]->avg == -1)
				cout << "NA\t";
			else
				cout << globalarr[i]->avg << "\t";
			if (globalarr[i]->cgpa == -1)
				cout << "NA\n";
			else
				cout << globalarr[i]->cgpa << "\n";

		}
	}



}
void getcgpaquer(float k, char *str){
	int i = 0, l = 20;
	char c;
	system("cls");
	printf("Rno\tname\tma\tm2\tm3\tm4\tavg\tcgpa\n");
	for (; i < s; i++){
		if (compar1qur(globalarr[i]->cgpa, k, str) == 1){
			cout << globalarr[i]->rno << "\t" << globalarr[i]->name << "\t" << globalarr[i]->m1 << "\t" << globalarr[i]->m2 << "\t" << globalarr[i]->m3 << "\t" << globalarr[i]->m4 << "\t";
			if (globalarr[i]->avg == -1)
				cout << "NA\t";
			else
				cout << globalarr[i]->avg << "\t";
			if (globalarr[i]->cgpa == -1)
				cout << "NA\n";
			else
				cout << globalarr[i]->cgpa << "\n";

		}
	}



}
void getnamequer(char *str1, char *str2){
	int i = 0, l = 20;
	char c;
	system("cls");
	printf("Rno\tname\tma\tm2\tm3\tm4\tavg\tcgpa\n");
	for (; i < s; i++){
		if (compar1qur(globalarr[i]->name, str1, str2) == 1){
			cout << globalarr[i]->rno << "\t" << globalarr[i]->name << "\t" << globalarr[i]->m1 << "\t" << globalarr[i]->m2 << "\t" << globalarr[i]->m3 << "\t" << globalarr[i]->m4 << "\t";
			if (globalarr[i]->avg == -1)
				cout << "NA\t";
			else
				cout << globalarr[i]->avg << "\t";
			if (globalarr[i]->cgpa == -1)
				cout << "NA\n";
			else
				cout << globalarr[i]->cgpa << "\n";

		}
	}
}
int compar1qur(float k1, float k2, char *str){
	if (strcmp(str, "lt") == 0){
		if (k1 < k2)
			return 1;
		else
			return 0;
	}
	else if (strcmp(str, "gt") == 0){
		if (k1 > k2)
			return 1;
		else
			return 0;
	}
	else if (strcmp(str, "ge") == 0){
		if (k1 >= k2)
			return 1;
		else
			return 0;
	}
	else if (strcmp(str, "eq") == 0){
		if (k1 <= k2)
			return 1;
		else
			return 0;
	}
	else if (strcmp(str, "gt") == 0){
		if (k1 > k2)
			return 1;
		else
			return 0;
	}
	else
		return -1;
}
int compar1qur(char *k1, char *k2, char *str){
	if (strcmp(str, "contains") == 0){
		if (strstr(k1,k2) != NULL)
			return 1;
		else
			return 0;
	}
	else if (strcmp(str, "startswith") == 0){
		if (strstr(k1, k2) == k1)
			return 1;
		else
			return 0;
	}
	return -1;
}
int filtergenerator(filter *ft,int *sz, char *str){
	int i = -1, j = -1, m = 0;
	float k = -1;
	char *pch = str, *temp;
	if (str == NULL){
		cout << "Invalid Query\n";
		return -1;
	}
	if (pch == NULL){
		cout << "Invalid Query\n";
		return -1;
	}
	pch = strtok(str, " ");
	if (pch == NULL){
		cout << "Invalid Query\n";
		return -1;
	}
	if (strcmp(pch, "avg") == 0)
		i = 1;
	else if (strcmp(pch, "m1") == 0)
		i = 2;
	else if (strcmp(pch, "m2") == 0)
		i = 3;
	else if (strcmp(pch, "m3") == 0)
		i = 4;
	else if (strcmp(pch, "m4") == 0)
		i = 5;
	else if (strcmp(pch, "cgpa") == 0)
		i = 6;
	else if (strcmp(pch, "name") == 0)
		i = 7;
	else if (strcmp(pch, "addr") == 0)
		i = 8;
	else if (strcmp(pch, "city") == 0)
		i = 9;
	else if (strcmp(pch, "phn") == 0)
		i = 10;
	else if (strcmp(pch, "pin") == 0)
		i = 11;
	ft[*sz].flag = i;


	if (i == -1){
		cout << "Invalid Query\n";
		return -1;
	}
	pch = strtok(NULL, " ");
	if (pch == NULL){
		cout << "Invalid Query\n";
		return -1;
	}
	temp = pch;
	j = 1;
	if (j == -1){
		cout << "Invalid Query\n";
		return -1;
	}
	pch = strtok(NULL, " ");
	if (pch == NULL){
		cout << "Invalid Query\n";
		return -1;
	}
	if (i==11||(i >= 1 && i <= 6)){
		k = atof(pch);
		ft[*sz].ki2 = k;
	}
	strcpy(ft[*sz].param3, temp);
	if (i >= 7&&i<=10){
		strcpy(ft[*sz].param2, pch);
	}
	*sz=*sz+1;

	return 1;
}
int isstudentinq(student stu, filter ft){
	if (ft.flag == 1){
		return compar1qur(stu.avg, ft.ki2, ft.param3);
	}
	else if (ft.flag == 2){
		return compar1qur(stu.m1, ft.ki2, ft.param3);
	}
	else if (ft.flag == 3){
		return compar1qur(stu.m2, ft.ki2, ft.param3);
	}
	else if (ft.flag == 4){
		return compar1qur(stu.m3, ft.ki2, ft.param3);
	}
	else if (ft.flag == 5){
		return compar1qur(stu.m4, ft.ki2, ft.param3);
	}
	else if (ft.flag == 6){
		return compar1qur(stu.cgpa, ft.ki2, ft.param3);
	}
	else if (ft.flag == 7){
		return compar1qur(stu.name, ft.param2, ft.param3);
	}
	else if (ft.flag == 8){
		return compar1qur(getcorrespon(stu).addr, ft.param2, ft.param3);
	}
	else if (ft.flag == 9){
		return compar1qur(getcorrespon(stu).city, ft.param2, ft.param3);
	}
	else if (ft.flag == 10){
		return compar1qur(getcorrespon(stu).mobil, ft.param2, ft.param3);
	}
	else if (ft.flag == 11){
		return compar1qur(getcorrespon(stu).pin, ft.ki2, ft.param3);
	}
	else return -1;

	


}
int isstudentinq(studentinfo stu, filter ft){
	if (ft.flag == 8){
		return compar1qur(stu.addr, ft.param2, ft.param3);
	}
	else if (ft.flag == 9){
		return compar1qur(stu.city, ft.param2, ft.param3);
	}
	else if (ft.flag == 10){
		return compar1qur(stu.mobil, ft.param2, ft.param3);
	}
	else if (ft.flag == 11){
		return 0;//compar1qur(stu.m3, ft.ki2, ft.param3);
	}

	else
		return 0;


}
void evaluatefil(filter *ft, int sf,bool *ar2,int ty){
	if (ty == 0||ty==2){
		int i = 0;
		bool *arr = (bool*)malloc(s*sizeof(bool));
		for (int j = 0; j < s; j++)
			arr[j] = false;
		for (int j = 0; j < s; j++){
			
				for (int k = 0; k < sf; k++){
					if (isstudentinq(*globalarr[j], ft[k]) == 0){
						if (ft[k].andflag == -1)
							arr[j] = false;
						else if (ft[k].andflag == 1)
							arr[j] = false;
						
					}
					else{
						if (ft[k].andflag == -1)
							arr[j] = true;
						else if (ft[k].andflag == 2)
							arr[j] = true;

					}
				}
			
		}
		for (int j = 0; j < s; j++)
			if (arr[j] == true){
				printstudentwithf(*globalarr[j], getcorrespon(*globalarr[j]), ar2);
				printf("\n");
			}
		free(arr);
	}
	else if (ty == 1){
		int i = 0;
		bool *arr = (bool*)malloc(s*sizeof(bool));
		for (int j = 0; j < si; j++)
			arr[j] = true;
		for (int j = 0; j < si; j++){
				for (int k = 0; k < sf; k++){
					if (isstudentinq(*globalarr[j], ft[k]) == 0){
						if (ft[k].andflag == -1)
							arr[j] = false;
						else if (ft[k].andflag == 1)
							arr[j] = false;

					}
					else{
						if (ft[k].andflag == -1)
							arr[j] = true;
						else if (ft[k].andflag == 2)
							arr[j] = true;

					}
				}
			
		}
		for (int j = 0; j < si; j++)
			if (arr[j] == true){
				printstudentwithf(getcorresponi(*ginfoarr[j]),*ginfoarr[j], ar2);
				printf("\n");
			}
		free(arr);
	}
}
studentinfo getcorrespon(student st){
	studentinfo temp;
	temp.rno = -1;
	for (int i = 0; i < si; i++){
		if (st.rno == ginfoarr[i]->rno)
			return *ginfoarr[i];
	}
	return temp;
}
student getcorresponi(studentinfo st){
	student temp;
	temp.rno = -1;
	for (int i = 0; i < s; i++){
		if (st.rno == globalarr[i]->rno)
			return *globalarr[i];
	}
	return temp;
}

void printstudent(student st){
	printf("%10d%10s%10d%10d%10d%10d",st.rno,st.name,st.m1,st.m2,st.m3,st.m4);
	if (st.avg == -1)
		printf("%10s", "NA");
	else
		printf("%10f", st.avg);
	if (st.cgpa == -1)
		printf("%10s", "NA");
	else
		printf("%10f", st.cgpa);
	printf("\n");



}
void printstudentwithf(student st,studentinfo stf,bool *arr2){
	if (st.rno != -1){
		if (arr2[0])
			printf("%10d", st.rno);
		if (arr2[1])
			printf("%10s", st.name);
		if (arr2[2])
			printf("%10d", st.m1);
		if (arr2[3])
			printf("%10d", st.m2);
		if (arr2[4])
			printf("%10d", st.m3);
		if (arr2[5])
			printf("%10d", st.m4);
		if (arr2[6]){
			if (st.avg != -1)
				printf("%10.2f", st.avg);
			else
				printf("%10s", "NA");
		}
		if (arr2[7])
		{
			if (st.cgpa != -1)
				printf("%10.2f", st.cgpa);
			else
				printf("%10s", "NA");
		}
	}
	else
		printf("%10s%10s%10s%10s%10s%10s%10s%10s", "NA", "NA", "NA", "NA", "NA", "NA", "NA", "NA");
	if (stf.rno != -1){
		if (arr2[8])
		{
			printf("%32s", stf.addr);
		}
		if (arr2[9])
		{
			printf("%15s", stf.city);
		}
		if (arr2[10])
		{
			printf("%13s", stf.mobil);
		}
		if (arr2[11])
		{
			printf("%10lld", stf.pin);
		}
	}
	else
		printf("%32s%15s%13s%10s", "NA", "NA", "NA", "NA");

}

void setboolarr(bool*arr, char *str,int ty){
	if (strcmp(str, "*") == 0){
		if (ty == 0){
			for (int i = 0; i <= 7; i++)
				arr[i] = true;
		}
		else if (ty == 1){
			for (int i = 8; i <= 11; i++)
				arr[i] = true;

		}
		else{
			for (int i = 0; i <= 11; i++)
				arr[i] = true;

		}
		return;
	}
	char *pch = strtok(str, ",");
	while (pch != NULL){
		if (strcmp(pch, "rno") == 0)
			arr[0] = true;
		else if(strcmp(pch, "name") == 0)
			arr[1] = true;
		else if (strcmp(pch, "m1") == 0)
			arr[2] = true;
		else if (strcmp(pch, "m2") == 0)
			arr[3] = true;
		else if (strcmp(pch, "m3") == 0)
			arr[4] = true;
		else if (strcmp(pch, "m4") == 0)
			arr[5] = true;
		else if (strcmp(pch, "avg") == 0)
			arr[6] = true;
		else if (strcmp(pch, "cgpa") == 0)
			arr[7] = true;
		else if (strcmp(pch, "addr") == 0)
			arr[8] = true;
		else if (strcmp(pch, "city") == 0)
			arr[9] = true;
		else if (strcmp(pch, "phn") == 0)
			arr[10] = true;
		else if (strcmp(pch, "pin") == 0)
			arr[11] = true;

		pch = strtok(NULL, ",");
	}
}


void preprocessor(char *str,int type){
	filter main[10];
	bool whatp[12] = {false};
	char buf1[50], buf2[20];
	int start, en, flag = -1,k;
	clearhm();
	if (parsestringpr(str, buf1, buf2, &type) == -1){
		printf("Invalid qury\n");
		return;
	}
		
	int i = 0;
	char *pch,*pch2;
	pch = strtok(str, "|");
	pch2 = strtok(NULL, "|");
	setboolarr(whatp, find("proj"),type);
	strcpy(buf1, find("cond"));
	main[0].andflag = -1;
	pch2 = buf1;
	k = endpostfor(buf1,&start,&en,&flag);
	pch2 = pch2 + start;
	pch2[en] = '\0';
	
	while (i < 10 && *pch2 !='\0'){
		if (filtergenerator(main, &i, pch2) == -1)
			break;
		pch2 = pch2 + en + k+1;
		main[i].andflag = flag;
		k = endpostfor(pch2, &start, &en, &flag);
		pch2 = pch2 + start;
		pch2[en] = '\0';
		//pch = strtok(NULL, "$");
	}
	evaluatefil(main, i,whatp,type);
}
void queryevaluator(char *s){
	int i = -1, j = -1,m=0;
	float k = -1;
	char *pch = s,*temp;
	if (s == NULL){
		cout << "Invalid Query\n";
		return;
	}
	if (pch == NULL){
		cout << "Invalid Query\n";
		return;
	}
	pch = strtok(s, " ");
	if (pch == NULL){
		cout << "Invalid Query\n";
		return;
	}
	if (strcmp(pch, "avg") == 0)
		i = 1;
	else if (strcmp(pch, "m1") == 0)
		i = 2;
	else if (strcmp(pch, "m2") == 0)
		i = 3;
	else if (strcmp(pch, "m3") == 0)
		i = 4;
	else if (strcmp(pch, "m4") == 0)
		i = 5;
	else if (strcmp(pch, "cgpa") == 0)
		i = 6;
	else if (strcmp(pch, "name") == 0)
		i = 7;



	if (i == -1){
		cout << "Invalid Query\n";
		return;
	}
	pch = strtok(NULL, " ");
	if (pch == NULL){
		cout << "Invalid Query\n";
		return;
	}
	temp = pch;
	j = 1;
	if (j == -1){
		cout << "Invalid Query\n";
		return;
	}
	pch = strtok(NULL, " ");
	if (pch == NULL){
		cout << "Invalid Query\n";
		return;
	}
	if (i >= 1 && i <= 6){
		k = atof(pch);

	}
	else{
		
	}
	if (i == 1){
		getavgquer(k, temp);
	}
	else if (i == 2){
		getm1quer(k, temp);
	}
	else if (i == 3){
		getm2quer(k, temp);
	}
	else if (i == 4){
		getm3quer(k, temp);
	}
	else if (i == 5){
		getm4quer(k, temp);
	}
	else if (i == 6){
		getcgpaquer(k, temp);
	}
	else if (i == 7){
		getnamequer(pch, temp);
	}


}

//files
int checkfileexists(char *s){
	FILE *fp;
	if ((fp = fopen(s, "r")) != NULL){
		fclose(fp);
		return 1;
	}
	else
		return 0;
}
void checkexistsstudent(int *pref){
	if (s == 1)
		return;
	else{
		int i;
		for (i = 0; i < s - 1; i++){
			if (globalarr[i]->rno == globalarr[s - 1]->rno){
				if (*pref == -1){
					cout << "Duplicate Found 1.Override 2.ignore\n";
					cin >> *pref;
				}
				if (*pref == 1)
					globalarr[i] = globalarr[s - 1];
				s--;
				return;
			}
		}
		return;
	}
}
void checkexiststinfo(int *pref){
	if (si == 1)
		return;
	else{
		int i;
		for (i = 0; i < si - 1; i++){
			if (ginfoarr[i]->rno == ginfoarr[si - 1]->rno){
				if (*pref == -1){
					cout << "Duplicate Found 1.Override 2.ignore\n";
					cin >> *pref;
				}
				if (*pref==1)
					ginfoarr[i] = ginfoarr[si - 1];

				si--;
				return;
			}
		}
		return;
	}
}
void print1combin(student m, studentinfo k){
	cout << m.rno << "\t" << m.name << "\t" << m.m1 << "\t" << m.m2 << "\t" << m.m3 << "\t" << m.m4 << "\t";
	if (m.avg == -1)
		cout << "NA\t";
	else
		cout << m.avg << "\t";
	if (m.cgpa == -1)
		cout << "NA\t";
	else
		cout << m.cgpa << "\t";
	if (k.rno != -1)
		cout << k.rno << "\t" << k.mobil << "\t" << k.addr << "\t" << k.city << "\t" << k.pin << "\n";
	else
		cout << "NA\tNA\tNA\tNA\tNA\n";
}
void njoin(){
	int i, j,c=0,g,fl;
	studentinfo temp;
	temp.rno = -1;
	for (i = 0; i < s; i++){
		fl = 0;
		for (j = 0; j < si; j++){
			if (globalarr[i]->rno == ginfoarr[j]->rno){
				c++;
				fl = 1;
				break;
				//print1combin(globalarr[i], ginfoarr[j]);
			}
		}
		c++;
		if (c > 20){
			cout << "More? 1.Yes 2.No\n";
			cin >> g;
			if (g == 1){
				system("cls");
				c = 0;
			}
			else
				return;
		}
		if (fl == 1)
			print1combin(*globalarr[i], *ginfoarr[j]);
		else
			print1combin(*globalarr[i], temp);
	}
}
void initfls(){
	globalarr = (student**)malloc(sizeof(student*) * 10);
	ginfoarr = (studentinfo**)malloc(sizeof(studentinfo*) * 10);

}
void updategarr(){
	student **a = (student**)malloc(sizeof(student*)*maxsz * 2);
	maxsz = maxsz * 2;
	for (int i = 0; i < s; i++)
		a[i] = globalarr[i];
	/*
	for (int i = s; i < maxsz; i++)
		a[i].avg = a[i].cgpa = -1;
	*/
	free(globalarr);
	globalarr = a;
}
void updateginfoarr(){
	studentinfo **a = (studentinfo**)malloc(sizeof(studentinfo*)*maxsz * 2);
	maxszi = maxszi * 2;
	for (int i = 0; i < si; i++)
		a[i] = ginfoarr[i];
	free(ginfoarr);
	ginfoarr = a;
}
void savefiles(char *sf){
	if (sf == NULL)
	{
		cout << "Name error\n";
		return;
	}
	FILE *fp;
	char buf[256];
	if (checkfileexists(sf) == 1){
		fp = fopen(sf, "a");
	}
	else{
		fp = fopen(sf, "w");
		fputs(formati, fp);
	}
	if (fp == NULL){
		cout << "File Error\n";
		return;
	}
	for (int i = 0; i < s; i++){
		getcsv(*globalarr[i], buf);
		fputs(buf, fp);
		//checkexistsstudent();
	}
	fclose(fp);
}
void saveinfo(char *sf){
	FILE *fp;
	char buf[256];
	if (checkfileexists(sf) == 1){
		fp = fopen(sf, "a");
	}
	else{
		fp = fopen(sf, "w");
		fputs(formatf, fp);
	}
	//fputs(formati, fp);
	if (fp == NULL){
		cout << "File Error\n";
		return;
	}
	for (int i = 0; i < si; i++){
		getcsvi(*ginfoarr[i], buf);
		fputs(buf, fp);
		//checkexiststinfo();
	}
	fclose(fp);
}
void getcsv(student stu, char* buf){
	char buf2[4];
	sprintf(buf, "%d,%s,%d,%d,%d,%d,%f,%f\n\0", stu.rno, stu.name, stu.m1, stu.m2, stu.m3, stu.m4,  stu.avg, stu.cgpa);
	return;
}
void getcsvi(studentinfo stu, char* buf){
	sprintf(buf, "%d,%s,%s,%s,%d\n\0",stu.rno,stu.mobil,stu.addr,stu.city,stu.pin);
	return;
}

void computeavg(){
	for (int i = 0; i < s; i++){
		globalarr[i]->avg = (globalarr[i]->m1 + globalarr[i]->m2 + globalarr[i]->m3 + globalarr[i]->m4) / 4.0;
	}
}
int getgpa(int k){
	if (k >= 91)
		return 10;
	else if (k >= 81 && k < 91)
		return 9;
	else if (k >= 71 && k < 81)
		return 8;
	else if (k >= 61 && k < 71)
		return 7;
	else if (k >= 51 && k < 61)
		return 6;
	else if (k >= 41 && k < 51)
		return 5;
	else if (k>=31&&k < 41)
		return 4;
	else if (k >= 21 && k < 31)
		return 3;
	else if (k >= 11 && k < 21)
		return 2;
	else if (k >= 1 && k < 11)
		return 1;
	else
		return 0;


}
void computecgpa(){
	for (int i = 0; i < s; i++){
		globalarr[i]->cgpa = (getgpa(globalarr[i]->m1) + getgpa(globalarr[i]->m2) + getgpa(globalarr[i]->m3) + getgpa(globalarr[i]->m4)) / 4.0;
	}

}
void showrec(){
	
	int i = 0,l=20;
	char c;
	while (1){
		system("cls");
		printf("Rno\tname\tma\tm2\tm3\tm4\tavg\tcgpa\n");
		for (; i < s&&i < l; i++){
			cout << globalarr[i]->rno << "\t" << globalarr[i]->name << "\t" << globalarr[i]->m1 << "\t" << globalarr[i]->m2 << "\t" << globalarr[i]->m3 << "\t" << globalarr[i]->m4 << "\t";
			if (globalarr[i]->avg == -1)
				cout << "NA\t";
			else
				cout << globalarr[i]->avg << "\t";
			if (globalarr[i]->cgpa == -1)
				cout << "NA\n";
			else
				cout << globalarr[i]->cgpa << "\n";

		}
		cout << "n->next e->end\n";
		l += 20;
		if (l > s)
			break;
		else{
			i = l - 20;
			cin >> c;
			if (c == 'n')
				continue;
			else
				break;
		}
	}
}
void showinfo(){

	int i = 0, l = 20;
	char c;
	while (1){
		system("cls");
		printf("Rno\tPhonenumber\tAddress\tCity\tPin\n");
		for (; i < si&&i < l; i++){
			cout << ginfoarr[i]->rno << "\t" << ginfoarr[i]->mobil << "\t" << ginfoarr[i]->addr << "\t" << ginfoarr[i]->city << "\t" << ginfoarr[i]->pin << "\n";
			
		}
		cout << "n->next e->end\n";
		l += 20;
		if (l > si)
			break;
		else{
			i = l - 20;
			cin >> c;
			if (c == 'n')
				continue;
			else
				break;
		}
	}
}

int loadfile(char *str){
	char buf[256];
	char *temp;
	int kl = -1;
	if (str == NULL)
		return -1;
	else{
		FILE *fp;
		fp = fopen(str, "r");
		if (fp == NULL){
			cout << "FileError\n";
		}
		fgets(buf, 255, fp);
		while (!feof(fp)){
			temp=fgets(buf, 255, fp);
			if (temp == NULL)
				break;
			insert1rec(buf);
			checkexistsstudent(&kl);
		}
		fclose(fp);
	}
	return 1;
}
int loadinfo(char *str){
	char buf[256];
	char *temp;
	int kl = -1;
	if (str == NULL)
		return -1;
	else{
		FILE *fp;
		int k;
		fp = fopen(str, "r");
		if (fp == NULL){
			cout << "FileError\n";
		}
		fgets(buf, 255, fp);
		while (!feof(fp)){
			temp=fgets(buf, 255, fp);
			if (temp == NULL)
				break;
			insert1info(buf);
			checkexiststinfo(&kl);
		}
		fclose(fp);
	}
	return 1;
}

void insert1info(char *str){
	studentinfo *temp=(studentinfo*)malloc(sizeof(studentinfo)*1);
	if (si >= maxszi)
		updateginfoarr();
	int i = 0;
	while (str[i] != ',')
		i++;
	str[i] = '\0';
	temp->rno = atoi(str);
	str = str + i + 1;
	i = 0;
	while (str[i] != ',')
		i++;
	str[i] = '\0';
	strcpy(temp->mobil, str);
	str = str + i + 1;
	i = 0;
	while (str[i] != ',')
		i++;
	str[i] = '\0';
	strcpy(temp->addr, str);
	str = str + i + 1;
	i = 0;
	while (str[i] != ',')
		i++;
	str[i] = '\0';
	strcpy(temp->city, str);
	str = str + i + 1;
	i = 0;
	while (str[i] != '\n'&&str[i]!='\0')
		i++;
	str[i] = '\0';
	temp->pin=atoi(str);
	ginfoarr[si] = temp;
	si++;

}

void insert1rec(char *str){
	student *temp=(student*)malloc(sizeof(student)*1);
	if (s >= maxsz)
		updategarr();
	int i = 0;
	while (str[i] != ',')
		i++;
	str[i] = '\0';
	temp->rno = atoi(str);
	str = str + i + 1;
	while (str[i] != ',')
		i++;
	str[i] = '\0';
	strcpy(temp->name, str);
	str = str + i+1;
	i = 0;
	while (str[i] != ',')
		i++;
	str[i] = '\0';
	temp->m1 = atoi(str);
	str = str + i + 1;
	i = 0;
	while (str[i] != ',')
		i++;
	str[i] = '\0';
	temp->m2 = atoi(str);
	str = str + i + 1;
	i = 0;
	while (str[i] != ',')
		i++;
	str[i] = '\0';
	temp->m3 = atoi(str);
	str = str + i + 1;
	i = 0;

	while (str[i] != '\n'&&str[i]!='\0')
		i++;
	str[i] = '\0';
	temp->m4 = atoi(str);
	temp->avg = temp->cgpa = -1;
	str = str + i + 1;
	globalarr[s] = temp;
	s++;
	
}


/*itoa(stu.rno, buf2, 10);
strcat(buf, buf2);
strcat(buf,",");
strcat(buf, stu.name);
strcat(buf, ",");
itoa(stu.m1, buf2, 10);
strcat(buf, buf2);
strcat(buf, ",");
itoa(stu.m2, buf2, 10);
strcat(buf, buf2);
strcat(buf, ",");
itoa(stu.m3, buf2, 10);
strcat(buf, buf2);
strcat(buf, ",");
itoa(stu.m4, buf2, 10);
strcat(buf, buf2);
strcat(buf, ",");
sprintf(buf2, "%f,", stu.avg);
strcat(buf, buf2);
sprintf(buf2, "%f", stu.cgpa);
strcat(buf, buf2);
strcat(buf, "\n\0");
*/
