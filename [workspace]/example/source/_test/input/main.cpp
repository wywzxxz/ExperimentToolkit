#include <iostream>
#include "..\..\..\..\..\[testModule]\input.h"
#include <time.h>
/*
����ֵ��
	0		����
	-1		����Ҫ��ֹͣ���޸������ݣ�
	else	����
*/
int main(int argc, char** argv) {
	Input input(argc,argv);
	if (input._seed>10) return -1;
	int a=input.json["a"]=rand();
	int b=input.json["b"]=rand();
	printf("%d %d",a,b);
	return 0;
}
