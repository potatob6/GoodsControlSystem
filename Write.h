#pragma once
#include "Structures.h"

//将商品链表写入到文件中
void writeGoods(FILE *f,Good* first) {
	fprintf(f, "#开头为注释。#具体内容如下：\n");
	fprintf(f, "#商品编号,所属类别编号,商品名称,库存量,单价\n");
	Good* next = first;
	while (next != NULL) {
		fprintf(f, "%s %d %s %d %f\n", next->index, next->type, next->name, next->count, next->value);
		next = next->nextGood;
	}
}

//将销售链表写入到文件中
void writeOrder(FILE* f, Order* first) {
	fprintf(f, "#开头为注释。#具体内容如下：\n");
	fprintf(f, "#订单编号,商品编号,销售量,单价,总金额\n");
	Order* next = first;
	while (next != NULL) {
		fprintf(f, "%04d %s %d %.2f %.2f\n",next->index,next->orderIndex,next->salesVolume,next->unitPrice,next->totalSales);
		next = next->nextOrder;
	}
}

//将类型链表写入到文件中
void writeCategory(FILE* f, Category* first) {
	fprintf(f,"#开头为注释。#具体内容如下：\n");
	fprintf(f,"#类别编号,类别名\n");
	Category* next = first;
	while (next != NULL) {
		fprintf(f, "%d %s\n",next->index,next->name);
		next = next->nextCategory;
	}
}