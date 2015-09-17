#include <iostream>
#include "..\..\..\..\[testModule]\loop.h"
nlohmann::json json;
int main(int argc, char** argv) {
	Loop loop(argc,argv);
	int  a,b,c;
	scanf("%d %d",&a,&b);
	c=a+b;
	printf("%d",c);
	loop.json["c"]=c;
	loop.json["arr"]={0,1,2};
	loop.json["arr"].push_back(1);

	return 0;
}
