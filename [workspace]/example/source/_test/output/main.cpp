#include"stdio.h"
#include "..\..\..\..\..\[testModule]\output.h"
/*
����ֵ��
		2		����Ҫ��ֹͣ
		1		�ж�������ȼ� 
		0		�ж�����ȼ� 
		-1		��Ҫ�Ƚ����
		else	���� 
*/
int main(int argc, char** argv) {
	Output output(argc,argv);
	if (output.checkExitStatus())output.Error("run time error");	
	return 0;
}
