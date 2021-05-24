/*
	王耀权 陈子兴 文一乐 姚锌涛 李鸿坚
*/
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "Structures.h"
#include "Display.h"
#include "Read.h"
#include "PasswordChange.h"
#include "Sorts.h"
#include "Write.h"

Good* firstGood;//商品链表头
Order* firstOrder;//销售链表头
Category* firstCategory;//类型链表头

//计数变量 用于数组创建更好知道创建的长度
int LEN_GOODS = 0;
int LEN_CATEGORY = 0;
int LEN_ORDER = 0;
int MAX_ORDER_INDEX = 0;

//更新订单信息数组,传入值为订单的链表头
Order** updateOrder(Order* firstOrder) {
	Order** oList = (Order**)malloc(LEN_ORDER * sizeof(Order*));
	int oIndex = 0;
	Order* olast = firstOrder;
	while (olast != NULL) {
		oList[oIndex] = olast;
		olast = olast->nextOrder;
		oIndex++;
	}
	return oList;
}

//更新库存容量
void updateCount(Good* firstGood,Order* userLink) {
	//寻找对应的商品
	Order* oNext = userLink;
	while (oNext != NULL) {
		Good* gNext = firstGood;
		while (gNext != NULL) {
			if (!strcmp(gNext->index, oNext->orderIndex))
				gNext->count -= oNext->salesVolume;
			gNext = gNext->nextGood;
		}
		oNext = oNext->nextOrder;
	}
}

Good* findGoodByName(Good* firstGood,char *s)
{
	int flag = 0;
	//链表遍历
	Good* next = firstGood;
	while (next != NULL) {
		if (!strcmp(next->name, s)) {
			flag = 1;
			break;
		}
		next = next->nextGood;
	}
	if (!flag) return NULL;
	return next;
}

//完整复制一个Good结构体,除了nextGood成员属性以及count属性
//count属性为用户购买的数量
Good* copyNew(Good* ori,int c)
{
	Good* g = (Good*)malloc(sizeof(Good));
	g->count = c;
	strcpy(g->name, ori->name);
	strcpy(g->index, ori->index);
	g->type = ori->type;
	g->value = ori->value;
	g->nextGood = NULL;
	return g;
}

void orderAdd(Order* firstOrder,Order* userlink,int* LEN) {
	int userLinkLength = 0;
	//添加订单列表
	//定位到订单尾
	Order* next = firstOrder;
	while (next != NULL) {
		if (next->nextOrder == NULL)
			break;
		next = next->nextOrder;
	}

	Order* userNext = userlink;
	while (userNext != NULL) {
		userLinkLength++;
		LEN_ORDER++;
		userNext = userNext->nextOrder;
	}
	if (firstOrder == NULL) {
		firstOrder = userlink;
	}
	else
	{
		next->nextOrder = userlink;
	}

	
}


//free的各种函数定义
void linkListFreeOrder(Order* g);
void linkListFreeCategory(Category* g);
void linkListFreeGood(Good* g);

int main() {
	FILE* f;//用户密码文件
	f = fopen("data\\pwd.txt", "r");
	if (f == NULL) {
		//文件不存在,创建文件并让用户输入密码
		char pwd[20];

		printf("您好,第一次使用本系统,请设置您的密码:");
		for (int i = 0; i < 20; i++) {
			char ch = getch();
			if (ch == 13) {
				pwd[i] = '\0';
				break;
			}
			pwd[i] = ch;
			printf("*");
		}
		crypt(pwd);
		printf("\n您的密码已经设置成功,请牢记您的密码\n");
		//创建文件并将密码放到data\pwd.txt文件中
		f = fopen("data\\pwd.txt", "w");
		fprintf(f, "%s", pwd);
	}
	else {
		int errorCount = 0; //记录密码错误次数,超过3次失败
		while (1) {
			//已经有pwd文件,让用户输入密码
			char pwd[20];
			char correctPwd[20];
			printf("请输入您的密码:");
			for (int i = 0; i < 20; i++) {
				char ch = getch();
				if (ch == 13) {
					pwd[i] = '\0';
					break;
				}
				pwd[i] = ch;
				printf("*");
			}
			fscanf(f, "%s", correctPwd);
			decode(correctPwd);
			printf("\n\n");
			int cmpResult = strcmp(pwd, correctPwd);
			if (!cmpResult) {
				printf("密码正确,正在进入系统\n\n\n\n\n\n\n\n");
				//输入正确
				break;
			}
			else {
				//密码错误
				errorCount++;
				printf("密码错误,你还有%d次机会输入\n",3-errorCount);
				if (errorCount == 3) {
					printf("3次密码输入失败,程序退出\n");
					return 0;
				}
			}
		}
	}
	fclose(f);

	
	//开始读取各个文件的值
	FILE* r_goods = fopen("data\\goods.txt", "r"); 	//商品目录
	FILE* r_category = fopen("data\\category.txt", "r"); //类型目录
	FILE* r_order = fopen("data\\order.txt", "r"); //销售目录

	if (r_goods == NULL) printf("商品文件不存在\n");
	if (r_category == NULL) printf("类型文件不存在\n");
	if (r_order == NULL) printf("销售文件不存在\n");


	//写入到链表头并且记录每个链表的长度
	firstGood = readAllGood(r_goods,&LEN_GOODS);
	firstOrder = readAllOrder(r_order,&LEN_ORDER,&MAX_ORDER_INDEX);
	firstCategory = readAllCategory(r_category,&LEN_CATEGORY);
	
	//关闭所有文件
	fclose(r_goods);
	fclose(r_category);
	fclose(r_order);


	//类型数组 目的是更好的找到对应的名字
	Category** cList = (Category**)malloc(LEN_CATEGORY * sizeof(Category*));
	//将类型存入数组
	int cIndex = 0;
	Category* clast = firstCategory;
	while (clast != NULL) {
		cList[cIndex] = clast;
		clast = clast->nextCategory;
		cIndex++;
	}
	//订单数组 目的是更好找到对应的销售额等信息
	Order** oList = updateOrder(firstOrder);

	
	//程序进入死循环以让用户可以多次输入
	while (1) {
		//显示菜单
		displayMenu();
		//开始接收用户输入的值
		int input;
		scanf("%d", &input);

		switch (input)
		{
		case 1://按商品编号排序
			firstGood = sortGoodByIndex(firstGood,LEN_GOODS);
			dispAllGoodUseTypeName(firstGood, cList);

			break;
		case 2://销售商品
			{
			int countIndex = 0;//用户购买清单计数
			//用户购买生成的链表
			Good* userBuy = NULL;
			Good* lastBuy = NULL;
			//订单链表
			Order* userOrder = NULL;
			Order* lastOrder = NULL;

			//放出所有的商品供用户选择
			dispAllGoodToUsers(firstGood);
			//提示用户输入想要购买的商品
			printf("请输入您想购买的商品,输入 0 以结算全部购买的商品\n");
			while (1) {
				int count = 0;
				char input[20];
				
				printf("请输入第%d件商品:", countIndex +1);
				scanf("%s", &input);
				

				if (!strcmp(input, "0")) {
					//退出购买,开始结算
					break;
				}
				
				//寻找对应的商品
				Good* g = findGoodByName(firstGood,input);
				if (g == NULL)
				{
					printf("没有找到对应的商品\n");
					continue;
				}
				printf("请输入购买第%d件商品的数量:", countIndex + 1);
				scanf("%d", &count);
					
				if (g->count < count) 
				{
					printf("数量太多了,库存没有了\n");
					continue;
				}
					
				countIndex++;
				//购买链表加入
				if (lastBuy == NULL) {
					//清单中的第一个
					userBuy = copyNew(g,count);
					lastBuy = userBuy;
					//订单链表
					userOrder = (Order*)malloc(sizeof(Order));

					userOrder->index = MAX_ORDER_INDEX + 1;
					strcpy(userOrder->orderIndex, g->index);
					userOrder->salesVolume = count;
					userOrder->unitPrice = g->value;
					userOrder->nextOrder = NULL;
					userOrder->totalSales = count * g->value;

					lastOrder = userOrder;
				}
				else {
					lastBuy->nextGood = copyNew(g,count);
					lastBuy = lastBuy->nextGood;
					//订单链表
					lastOrder->nextOrder = (Order*)malloc(sizeof(Order));

					lastOrder->nextOrder->index = MAX_ORDER_INDEX + 1;
					strcpy(lastOrder->nextOrder->orderIndex, g->index);
					lastOrder->nextOrder->salesVolume = count;
					lastOrder->nextOrder->unitPrice = g->value;
					lastOrder->nextOrder->nextOrder = NULL;
					lastOrder->nextOrder->totalSales = count * g->value;

					lastOrder = lastOrder->nextOrder;
				}
			}
			//TODO:订单结算
			//显示清单内容
			dispUserOrder(userBuy, MAX_ORDER_INDEX + 1);
			printf("请问是否确认订单? 输入Y或N\n");
			char confirm[3];
			scanf("%s", confirm);
			if (!strcmp("Y", confirm)) {
				//订单链加入
				int userLength = 0;
				orderAdd(firstOrder, userOrder,&userLength);
				if (userLength != 0) MAX_ORDER_INDEX++;
				updateCount(firstGood, userOrder);
				//更新对应销售数组
				free(oList);
				oList = updateOrder(firstOrder);
				printf("商品购买成功!\n");
			}
			else {
				printf("已经为您取消订单.\n");
				linkListFreeOrder(userOrder);
			}
			//free掉用户生成的链表
			linkListFreeGood(userBuy);
			break;
			}
			
		case 3://删除指定订单
			//先显示所有的订单供用户查看
			dispAllOrder(firstGood,firstOrder,&MAX_ORDER_INDEX);
			printf("请输入要删除的订单号:");
			int orderIndex;
			scanf("%d", &orderIndex);
			//遍历链表并删除释放对应的Order*
			Order* lastOrder = NULL;
			Order* oNext = firstOrder;
			while (oNext != NULL) {
				if (oNext->index == orderIndex) {
					//更新对应的库存
					//遍历商品,增加对应的库存量
					Good* gNext = firstGood;
					while (gNext != NULL) {
						if (strcmp(gNext->index, oNext->orderIndex) == 0) {
							gNext->count += oNext->salesVolume;
						}
						gNext = gNext->nextGood;
					}
					//删除一个订单,并且连接下一个
					if (lastOrder == NULL) {
						//如果这个是链表头
						//创建新的链表头
						firstOrder = oNext->nextOrder;
						//更新lastOrder和oNext
						lastOrder = NULL;
						
						//释放oNext
						free(oNext);
						oNext = NULL;
						//oNext重新指向
						oNext = firstOrder;
					}
					else {
						//不是链表头
						lastOrder->nextOrder = oNext->nextOrder;
						//更新lastOrder和oNext
						free(oNext);
						oNext = NULL;
						//重新指向oNext
						oNext = lastOrder->nextOrder;

					}
					//更新Order的长度
					LEN_ORDER--;
					
				}
				else {
					lastOrder = oNext;
					oNext = oNext->nextOrder;
				}

			}
			//更新对应销售数组
			free(oList);
			oList = updateOrder(firstOrder);
			//再次显示订单链表
			dispAllOrder(firstGood,firstOrder,&MAX_ORDER_INDEX);
			printf("已经成功删除所有关于订单号为:%04d的订单了,目前最大订单号为:%d\n" , orderIndex,MAX_ORDER_INDEX);
			break;
		case 4://给定类别的商品销售明细
			//先输出类别供用户选择
			dispAllCategory(firstCategory);
			printf("请输入想要查询的类别对应的 编号 :");
			int searchIndex;
			scanf("%d", &searchIndex);
			dispGoodByCategory(searchIndex, firstGood, oList, LEN_ORDER);
			break;
		case 5://所有商品销售情况统计
			dispAllGood(firstGood);
			break;
		case 6://按商品销售量排序
			firstGood = sortGoodBySales(firstGood, LEN_GOODS, oList, LEN_ORDER);
			dispAllGoodUseSalesVolume(firstGood,oList,LEN_ORDER);
			break;
		case 7://输出指定商品信息
			dispSpecifiGood(firstGood,oList,LEN_ORDER);
			break;
		case 8://修改密码
			changePwd();
			break;
		case 0://退出系统
			printf("===========感谢使用本系统===========\n");

			//收尾工作
			//将链表重新写回文件
			FILE* w_goods = fopen("data\\goods.txt", "w");
			FILE* w_order = fopen("data\\order.txt", "w");
			FILE* w_category = fopen("data\\category.txt", "w");
			
			writeGoods(w_goods, firstGood);
			writeOrder(w_order, firstOrder);
			writeCategory(w_category, firstCategory);

			fclose(w_goods);
			fclose(w_order);
			fclose(w_category);
			
			//释放全部malloc出来的
			linkListFreeGood(firstGood);
			linkListFreeCategory(firstCategory);
			linkListFreeOrder(firstOrder);
			//free掉类型数组和订单数组
			free(oList);
			free(cList);

			return 0;
			break;
		default:
			printf("对不起,您输入的编号有误,请检查后再次输入.\n");
			break;
		}
	}
	
	
	return 0;

}

//free掉链表
void linkListFreeOrder(Order* g) {
	Order* last = NULL;
	Order* next = g;
	while (next != NULL) {
		last = next;
		next = next->nextOrder;
		free(last);
	}
}

//free掉链表
void linkListFreeCategory(Category* g) {
	Category* last = NULL;
	Category* next = g;
	while (next != NULL) {
		last = next;
		next = next->nextCategory;
		free(last);
	}
}

//free掉链表
void linkListFreeGood(Good* g) {
	Good* last = NULL;
	Good* next = g;
	while (next != NULL) {
		last = next;
		next = next->nextGood;
		free(last);
	}
}