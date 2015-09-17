#include <iostream>
#include "..\..\..\..\..\[testModule]\input.h"
#include <time.h>
/*
返回值：
	0		正常
	-1		程序要求停止（无更多数据）
	else	崩溃
*/
int main(int argc, char** argv) {
	Input input(argc,argv);
	if (input._seed>10) return -1;
	int a=input.json["a"]=rand();
	int b=input.json["b"]=rand();
	printf("%d %d",a,b);
	return 0;
}
