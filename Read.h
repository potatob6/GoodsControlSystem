#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "Structures.h"

#define FLASHBACK_OFFSET 1//折回时要加上\n

//读取下一条商品
Good* readNextGood(FILE *f_goods) {
	char nextStr[50];
	fgets(nextStr, 50, f_goods);
	if (nextStr[0] == '#') {
		return NULL;
	}
	//指针折回
	fseek(f_goods, -(long)(strlen(nextStr)+FLASHBACK_OFFSET), SEEK_CUR);
	Good* g = (Good*)calloc(1, sizeof(Good));
	fscanf(f_goods, "%s", g->index); //读取商品编号
	fscanf(f_goods, "%d", &(g->type)); //读取商品类型
	fscanf(f_goods, "%s", g->name); //读取商品名字
	fscanf(f_goods, "%d", &(g->count)); //读取商品库存数量
	fscanf(f_goods, "%f", &(g->value)); //读取商品单价
	return g;
}

//读取下一条销售信息
Order* readNextOrder(FILE* f_orders) {
	char nextStr[50];
	fgets(nextStr, 50, f_orders);
	if (nextStr[0] == '#') {
		return NULL;
	}
	//指针折回
	fseek(f_orders, -(long)(strlen(nextStr) + FLASHBACK_OFFSET), SEEK_CUR);
	Order* o = (Order*)calloc(1, sizeof(Order));
	fscanf(f_orders, "%d", &(o->index));  //读取销售单号
	fscanf(f_orders, "%s", o->orderIndex);       //读取销售商品编号
	fscanf(f_orders, "%d", &(o->salesVolume)); //读取销售额
	fscanf(f_orders, "%f", &(o->unitPrice));   //读取单价
	fscanf(f_orders, "%f", &(o->totalSales));  //总额
	return o;
}

//读取下一条类型
Category* readNextCategory(FILE* f_category) {
	char nextStr[50];
	fgets(nextStr, 50, f_category);
	if (nextStr[0] == '#') {
		return NULL;
	}
	fseek(f_category, -(long)(strlen(nextStr) + FLASHBACK_OFFSET), SEEK_CUR);
	Category* c = (Category*)calloc(1, sizeof(Category));
	fscanf(f_category, "%d", &(c->index)); //读取类型编号
	fscanf(f_category, "%s", c->name);  //读取类型名字
	return c;
}

//读取所有的商品信息,并且返回的是链表头
Good* readAllGood(FILE *f_good,int *LEN) {
	if (feof(f_good)) return NULL;
	//链表头
	Good* firstGood=NULL;
	while (!feof(f_good)) {
		firstGood = readNextGood(f_good);
		if (firstGood == NULL) continue;
		else break;
	}
	if (!strcmp(firstGood->index,""))
	{
		free(firstGood);
		return NULL;
	}
	(*LEN)++;
	Good* lastGood = NULL;
	while (!feof(f_good)) {
		Good* nextGood = readNextGood(f_good);
		if (nextGood == NULL) continue;
		if (strcmp(nextGood->index, "") == 0) {
			break;
		}
		//链表连接操作
		if (lastGood != NULL)
			lastGood->nextGood = nextGood;
		else
			firstGood->nextGood = nextGood;
		//计数+1
		(*LEN)++;
		lastGood = nextGood;
	}
	return firstGood;
}

//读取所有的类型信息,并且返回的是链表头
Category* readAllCategory(FILE* f_category,int *LEN) {
	if (feof(f_category)) return NULL;
	Category* firstCategory=NULL;
	while (!feof(f_category)) {
		firstCategory = readNextCategory(f_category);
		if (firstCategory == NULL) continue;
		else break;
	}
	if (firstCategory->index == 0) {
		free(firstCategory);
		return NULL;
	}
	(*LEN)++;
	Category* lastCategory = NULL;
	while (!feof(f_category)) {
		Category* nextCategory = readNextCategory(f_category);
		if (nextCategory == NULL) continue;
		if (nextCategory->index == 0) {
			break;
		}
		//链表连接操作
		if (lastCategory != NULL)
			lastCategory->nextCategory = nextCategory;
		else
			firstCategory->nextCategory = nextCategory;
		//计数+1
		(*LEN)++;
		lastCategory = nextCategory;
	}
	return firstCategory;
}

//从文件中读取全部的销售信息
Order* readAllOrder(FILE* f_order,int *LEN,int *maxIndex) {
	if (feof(f_order))
		return NULL;
	Order* firstOrder = NULL;
	while (!feof(f_order)) {
		firstOrder = readNextOrder(f_order);
		if (firstOrder == NULL) continue;
		else break;
	}
	
	if (firstOrder->index == 0)
	{
		free(firstOrder);
		return NULL;
	}
	(*LEN)++;
	Order* lastOrder = NULL;
	while (!feof(f_order)) {
		Order* nextOrder = readNextOrder(f_order);
		if (nextOrder == NULL) continue;
		if (nextOrder->index == 0) {
			break;
		}
		//链表连接操作
		if (lastOrder != NULL)
			lastOrder->nextOrder = nextOrder;
		else
			firstOrder->nextOrder = nextOrder;
		//计数+1
		(*LEN)++;
		//更新最大订单号,方便后续订单号生成
		if (*maxIndex < nextOrder->index)
			(*maxIndex) = nextOrder->index;
		lastOrder = nextOrder;
	}
	return firstOrder;
}