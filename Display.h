#pragma once
#include <stdio.h>
#include "Structures.h"


//菜单显示
void displayMenu() {
	printf("******商品销售系统菜单*******\n");
	printf("1. 按商品编号排序\n");//已完成
	printf("2. 销售商品\n");//已完成
	printf("3. 删除指定订单\n");//已完成
	printf("4. 给定类别的商品销售明细\n");//已完成
	printf("5. 所有商品销售情况统计\n");//已完成
	printf("6. 按商品销售量排序\n");//已完成
	printf("7. 输出指定商品信息\n");//已完成
	printf("8. 修改密码\n");//已完成
	printf("0. 退出系统\n");//已完成
	printf("*****************************\n");
}

//给用户供选择的商品显示页
void dispAllGoodToUsers(Good* firstGood) {
	printf("商品类型\t商品名称\t商品价格\n");
	printf("=============================================\n");
	Good* next = firstGood;
	while (next != NULL) {
		printf("%d\t\t%s\t\t%.2f\n", next->type, next->name, next->value);
		next = next->nextGood;
	}
	printf("=============================================\n");
}

//显示一个销售信息 
void dispOrder(Order* o,Good* first) {
	char goodName[20];
	//搜索商品信息
	Good* next = first;
	while (next != NULL) {
		if (!strcmp(o->orderIndex, next->index))
			strcpy(goodName, next->name);
		next = next->nextGood;
	}
	printf("%04d\t\t%s\t%s\t\t%d\t%.2f\t%.2f\n", o->index, o->orderIndex,goodName, o->salesVolume, o->unitPrice, o->totalSales);
}

void dispUserOrder(Good* link,int orderIndex) {
	printf("第%04d号订单内容如下:\n", orderIndex);
	printf("序号\t名称\t单价\t数量\t总价\n");
	printf("===============================================\n");
	int countGood = 0;
	float countValue = 0;
	int index = 1;
	Good* next = link;
	while (next != NULL) {
		printf("%d:\t%s\t%.2f\t%d\t%.2f\n", index, next->name, next->value, next->count, next->value * next->count);
		countGood++;
		countValue += next->value * next->count;
		next = next->nextGood;
		index++;
	}
	printf("===============================================\n");
	printf("总共%d件商品,总共%.2f元\n", countGood, countValue);
	printf("===============================================\n");
	
}


//按类别显示一个商品
void dispGoodByCategory(int categoryIndex,Good* firstGood,Order** oList,int LEN_ORDER) {
	printf("销售单号\t销售商品id\t销售量\t单价\t总额\n");
	printf("======================================================\n");
	//遍历商品链表
	Good* next = firstGood;
	while (next != NULL) {
		if (next->type == categoryIndex) {
			//找到对应的订单号,商品编号,商品名称,销售数量,单价,总金额

			int i;
			for (i = 0; i < LEN_ORDER; i++) {
				if (!strcmp(next->index, oList[i]->orderIndex)) {
					dispOrder(oList[i],firstGood);
				}
			}
		}
		next = next->nextGood;
	}
	printf("======================================================\n");
}

//显示一个商品
void dispGood(Good* g) {
	printf("%8s\t%d\t%8s\t%d\t%.2f\n",g->index,g->type,g->name,g->count,g->value);
}

//仅仅显示简单的商品id和商品名字
void dispGoodSimple(Good* g) {
	printf("%s\t%s\t\n", g->index, g->name);
}

//显示商品的名字并且会显示其销售额
void dispAllGoodUseSalesVolume(Good* firstGood, Order** oList,int O_LEN) {
	printf("%10s\t%10s%10s%8s\t%6s\n", "商品id", "商品名称", "销售数量", "单价", "总金额");
	printf("==================================================================\n");
	//遍历链表
	Good* gNext = firstGood;
	while (gNext != NULL) {
		int salesVolume = 0;
		float unitPrice = 0;
		float total = 0;
		int i = 0;
		for (; i < O_LEN; i++) {
			if (!strcmp(oList[i]->orderIndex, gNext->index)) {
				salesVolume += oList[i]->salesVolume;
				unitPrice = oList[i]->unitPrice;
			}
		}
		total = unitPrice * salesVolume;
		printf("%10s\t%10s\t%d\t%.2f\t%.2f\n", gNext->index, gNext->name, salesVolume, unitPrice,total);
		gNext = gNext->nextGood;
	}
	printf("==================================================================\n");
}

//显示商品的信息并且会显示其商品类型名字
void dispAllGoodUseTypeName(Good* firstGood,Category** cList) {
	printf("%10s\t%10s\t%10s\t%6s%6s\n", "商品id", "商品类型名称", "商品名字", "库存量", "价格");
	printf("==================================================================\n");
	//遍历链表
	Good* gNext = firstGood;
	while (gNext != NULL) {
		printf("%10s\t%10s\t%10s\t%d\t%.2f\n", gNext->index, cList[gNext->type - 1]->name, gNext->name, gNext->count, gNext->value);
		gNext = gNext->nextGood;
	}
	printf("==================================================================\n");
}

//显示一个类型
void dispCategory(Category* c) {
	printf("%d\t%s\n",c->index,c->name);
}


//将一整个商品链表显示出来
void dispAllGood(Good* g) {
	printf("商品id\t商品类型\t商品名字\t库存量\t价格\n");
	printf("==================================================\n");
	Good* gNext = g;
	while (gNext != NULL) {
		dispGood(gNext);
		gNext = gNext->nextGood;
	}
}

//将一整个商品链表以商品编号和商品名字显示出来
void dispAllGoodSimple(Good* g) {
	printf("商品id\t\t商品名字\n");
	printf("=======================\n");
	Good* gNext = g;
	while (gNext != NULL) {
		dispGoodSimple(gNext);
		gNext = gNext->nextGood;
	}
	printf("=======================\n");
}

//将一整个类型链表显示出来
void dispAllCategory(Category* c) {
	printf("类型id\t类型名字\n");
	printf("============================\n");
	Category* cNext = c;
	while (cNext != NULL) {
		dispCategory(cNext);
		cNext = cNext->nextCategory;
	}
	printf("============================\n");
}

//将一整个销售链表显示出来
void dispAllOrder(Good* firstGood,Order* o,int *MAX_INDEX) {
	*MAX_INDEX = 1;
	printf("销售单号\t销售商品id\t商品名称\t销售量\t单价\t总额\n");
	printf("================================================================================\n");
	Order* oNext = o;
	while (oNext != NULL) {
		if (oNext->index > * MAX_INDEX)
			*MAX_INDEX = oNext->index;
		dispOrder(oNext, firstGood);
		oNext = oNext->nextOrder;
	}
	printf("================================================================================\n");
}

//显示一个商品的信息,并且信息内有对应的销售额
void dispGoodWithSalesV(Good* g,int v) {
	printf("%8s%8s\t%d\t%d\t\t%.2f\n", g->index, g->name, g->count,v, g->value);
}


//显示特定商品的所有信息
void dispSpecifiGood(Good* first,Order** oList,int oLen) {
	//先显示全部商品的编号和商品名字
	dispAllGoodSimple(first);
	//让用户输入商品编号或商品名字
	printf("请输入您要查找的商品的编号或名字:");
	char searchStr[20];
	scanf("%s", searchStr);

	

	printf("商品id\t商品名字\t库存量\t销售数量\t单价\n");
	printf("=======================================================================\n");
	//链表遍历搜索
	Good* g1 = first;
	while (g1 != NULL) {
		if (strcmp(g1->index, searchStr) == 0 || strcmp(g1->name, searchStr) == 0) {
			//从销售额数组中找到对应的销售额
			int v = 0;
			int i;
			for (i = 0; i < oLen; i++) {
				if (strcmp(g1->index, oList[i]->orderIndex) == 0) {
					v += oList[i]->salesVolume;
				}
			}
			dispGoodWithSalesV(g1,v);
		}
		g1 = g1->nextGood;
	}
	printf("=======================================================================\n");
	return;
}