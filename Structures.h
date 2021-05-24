#pragma once
//类型结构
typedef struct category {
	int index; //类型id
	char name[20];  //类型名字
	struct category* nextCategory;
}Category;

//销售结构
typedef struct order {
	int index;//销售单号
	char orderIndex[20];//销售的商品id
	int salesVolume;//销售额
	float unitPrice;//单价
	float totalSales;//总额
	struct order* nextOrder;
}Order;

//商品结构
typedef struct good {
	char index[20];//商品id
	int type;//商品类型
	char name[20];//商品名字
	int count;//库存量
	float value;//价格
	struct good* nextGood;
}Good;
