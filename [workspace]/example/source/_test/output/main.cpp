#include"stdio.h"
#include "..\..\..\..\..\[testModule]\output.h"
/*
返回值：
		2		程序要求停止
		1		判定输出不等价 
		0		判定输出等价 
		-1		需要比较输出
		else	崩溃 
*/
int main(int argc, char** argv) {
	Output output(argc,argv);
	if (output.checkExitStatus())output.Error("run time error");	
	return 0;
}
