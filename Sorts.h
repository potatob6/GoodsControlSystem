#pragma once
#include <stdlib.h>
#include "Structures.h"
//排序方法头文件

Good* sortGoodBySales(Good* gFirst,int LEN_G,Order** o,int LEN_O) {
	if (gFirst == NULL) return NULL;
	Good** gList = (Good**)malloc(sizeof(Good*) * LEN_G);

	//将商品存入List
	int index = 0;
	Good* gNext = gFirst;
	while (gNext != NULL) {
		gList[index] = gNext;
		index++;
		gNext = gNext->nextGood;
	}

	//排序方法:冒泡
	int i;
	for (i = 0; i < LEN_G; i++) {
		int j;
		for (j = i; j < LEN_G; j++) {
			//找到对应的销售量
			int iVolume = 0;
			int jVolume = 0;
			int z = 0;
			for (; z < LEN_O; z++) {
				if (!strcmp(o[z]->orderIndex, gList[i]->index)) {
					iVolume += o[z]->salesVolume;
					continue;
				}
				if (!strcmp(o[z]->orderIndex, gList[j]->index)) {
					jVolume += o[z]->salesVolume;
					continue;
				}
			}

			//开始比较
			if (iVolume < jVolume) {
				//交换指针
				Good* temp = gList[i];
				gList[i] = gList[j];
				gList[j] = temp;
			}
		}
	}
	//切断链尾
	gList[LEN_G-1]->nextGood = NULL;
	Good* g1 = NULL;
	//重新连接成链表
	if (LEN_G >= 1)
		g1 = gList[0];
	i = 1;
	Good* last = g1;
	for (; i < LEN_G; i++) {
		last->nextGood = gList[i];
		last = last->nextGood;
	}
	return g1;
	free(gList);
}

//根据商品的编号排序
Good* sortGoodByIndex(Good* gFirst, int gLen) {
	if (gFirst == NULL) return NULL;
	//商品数组以更好的排序
	Good** gList = (Good**)malloc(sizeof(Good*) * gLen);

	//将商品存入List
	int index = 0;
	Good* gNext = gFirst;
	while (gNext != NULL) {
		gList[index] = gNext;
		index++;
		gNext = gNext->nextGood;
	}

	//排序方法:冒泡
	int i;
	for (i = 0; i < gLen; i++) {
		int j;
		for (j = i; j < gLen; j++) {
			if (strcmp(gList[i]->index, gList[j]->index) > 0) {
				//交换指针
				Good* temp = gList[i];
				gList[i] = gList[j];
				gList[j] = temp;
			}
		}
	}

	//切断链尾
	gList[gLen-1]->nextGood = NULL;

	Good* g1 = NULL;
	//重新连接成链表
	if (gLen >= 1)
		g1 = gList[0];
	i = 1;
	Good* last = g1;
	for (; i < gLen; i++) {
		last->nextGood = gList[i];
		last = last->nextGood;
	}
	free(gList);
	gFirst = g1;
}
