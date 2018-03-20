#pragma once
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<iostream>
using namespace std;

struct string_value{
	char *str1, *str2;
	string_value *next;
}**string_ptr;

void init_hmap();
int hashfunction(char *s);
void insert_node_at(char *s, int h, char *s2);
void insert_hm(char *s, char *s2);
char* find(char *s);
char * find_replace(char *s, int h, char *s2);
void hasmampi_main();
void inserthmalt(char*, char*, char*);
char* find_append(char*,int, char*, char *);
void clearhm(){
	for (int i = 0; i < 10; i++)
		string_ptr[i] = NULL;
}
void init_hmap(){
	string_ptr = (string_value**) malloc(10 * sizeof(string_value*));
	for (int i = 0; i < 10; i++)
		string_ptr[i] = NULL;
}


int hashfunction(char *str){
	int pri = 4241;
	long long hv = 0,hpv=1,i=0,maxi=10000007;
	while (str[i] != '\0'){
		hpv = (hpv*pri) % maxi;
		hv = (hv + (((str[i]) - 'a'+1)*hpv) % maxi);
		i++;
	}
	return hv % 10;
}
void insert_node_at(char *s, int h,char *s2){
	if (string_ptr[h] == NULL){
		string_value *k = (string_value*)malloc(sizeof(string_value) * 1);
		k->str1 = (char*)malloc(sizeof(char) * 15);
		k->str2 = (char*)malloc(sizeof(char) * 15);
		strcpy(k->str1, s);
		strcpy(k->str2, s2);
		k->next = NULL;
		string_ptr[h] = k;
	}
	else{
		string_value *temp = string_ptr[h];
		while (temp->next != NULL)
			temp = temp->next;
		string_value *k = (string_value*)malloc(sizeof(string_value) * 1);
		strcpy(k->str1, s);
		strcpy(k->str2, s2);
		k->next = NULL;
		temp->next = k;
	}
}
void insert_hm(char *s,char *s2){
	int h = hashfunction(s);
	if (find(s) == NULL)
		insert_node_at(s, h, s2);
	else
		find_replace(s, h, s2);
}
void insert_hmalt(char *s, char *s2,char *s3){
	int h = hashfunction(s);
	if (find(s) == NULL)
		insert_node_at(s, h, s2);
	else
		find_append(s, h, s2,s3);
}
char* find(char *s){
	int h = hashfunction(s);
	if (string_ptr[h] == NULL)
		return NULL;
	else{
		string_value *temp=string_ptr[h];
		while (temp != NULL){
			if (strcmp(s, temp->str1) == 0)
				return temp->str2;
			temp = temp->next;
		}
		return NULL;
	}
}
char* find_replace(char *s,int h,char *s2){
	if (string_ptr[h] == NULL)
		return NULL;
	else{
		string_value *temp = string_ptr[h];
		while (temp != NULL){
			if (strcmp(s, temp->str1) == 0){
				strcpy(temp->str2, s2);
				return temp->str2;
			}
			temp = temp->next;
		}
		return NULL;
	}
}
char* find_append(char *s, int h, char *s2,char *s3){
	if (string_ptr[h] == NULL)
		return NULL;
	else{
		string_value *temp = string_ptr[h];
		while (temp != NULL){
			if (strcmp(s, temp->str1) == 0){
				strcat(temp->str2, s3);
				strcat(temp->str2, s2);
				return temp->str2;
			}
			temp = temp->next;
		}
		return NULL;
	}
}
void hasmapi_main(){
	init_hmap();
	char a[10], b[10],*c;
	int i;
	while (1){
		printf("1.Insert\n2.Retrieve\n");
		cin >> i;
		if (i == 1){
			cout << "Key followed by value\n";
			cin >> a >> b;
			insert_hm(a, b);
		}
		else if (i == 2){
			cout << "Key?\n";
			cin >> a;
			c = find(a);
			cout << ((c == NULL) ? "not found" : find(a));
			cout << endl;
		}
		else
			break;
	}
}