#include "[libs]/json/src/json.hpp"
/*
..\..\%1% !seed! "!inputcfg!" > input.txt <tmp.txt
*/
#include"string.h"
#include"windows.h"
struct Loop
{
	void cancletime(bool x=false){}
} *ploop;
struct Input
{
	int argc;char** argv;
	std::string err;
	nlohmann::json json;
	long _seed;void seed(long seed){json["_seed"]=_seed=seed;srand(_seed);}
	Input(int Inargc,char **Inargv)
	{		
		argc=Inargc;argv=Inargv;
		json = {};
		_seed=0;
		
		//获取随机种子 
		if (argc>1) seed(atoi(argv[1]));
		
	}	
	~Input()
	{		
	    if (argc>2)
	    {
			FILE * fp=fopen(argv[2],"w");
			fprintf(fp,"%s",json.dump().c_str());
			fclose(fp);
		}	
		fclose(stdout);
	}	
	void Warn(const char *msg=NULL)
	{
		if(msg)	printf("||||||||||||||||||||||[GENERATOR:%s]||||||||||||||||||||||\n"); 
	}
	void Error(const char *msg=NULL,int status=2)
	{
		Warn(msg);exit(status);
	}	
	char* getFullpath(char * path)
	{
		static char exeFullPath[256]; // Full path
	    std::string strPath = "";
	    GetModuleFileName(NULL,exeFullPath,256);
	    exeFullPath[	((std::string)exeFullPath).find_last_of('\\',256)+1	]=0;
	    strcat(exeFullPath,path);
	    return exeFullPath;
	}
};
