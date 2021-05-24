#pragma once
#include <stdio.h>
#include <string.h>

void crypt(char* s);
void decode(char* s);
//密码修改
void changePwd() {
	while (1)
	{
		char newPwd[20];
		char againCheck[20];
		printf("请输入新的密码:");
		for (int i = 0; i < 20; i++) {
			char ch = getch();
			if (ch == 13) {
				newPwd[i] = '\0';
				break;
			}
			newPwd[i] = ch;
			printf("*");
		}
		printf("\n");
		printf("请再次确认您的密码:");
		for (int i = 0; i < 20; i++) {
			char ch = getch();
			if (ch == 13) {
				againCheck[i] = '\0';
				break;
			}
			againCheck[i] = ch;
			printf("*");
		}
		printf("\n\n");
		if (strcmp(newPwd, againCheck) == 0) {
			//密码输入正确
			FILE* f = fopen("data\\pwd.txt", "w");
			crypt(newPwd);
			fprintf(f, "%s", newPwd);
			printf("密码修改成功,请牢记您的密码.\n");
			fclose(f);
			return;
		}
		else {
			//第二次密码输入错误
			printf("第二次密码错误,请重新修改密码...\n");
		}
	}
}

//加密函数(简单加密)
void crypt(char* s) {
	int i = 0;
	while (s[i] != '\0') {
		s[i] += 10;
		i++;
	}
}
//解密函数
void decode(char* s) {
	int i = 0;
	while (s[i] != '\0') {
		s[i] -= 10;
		i++;
	}
}