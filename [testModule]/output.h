#include "json/src/json.hpp"
#include"String.h"
/*
..\..\%2% "!inputcfg!" "!loopcfg!" "!resultfile!" !dtime! !el! "!tmp!" <output.txt
stdin:程序结果输出

argv1:input程序的附加信息(json)
argv2:程序附加信息 

输出:
	2:要求退出 
	1:错误
	0:正确 
	-1:要求对比文件 
	else 崩溃 
*/
struct Output
{
	int argc;
	char** argv;
	std::string err;
	nlohmann::json obj;
	Output(int Inargc,char **Inargv)
	{
		argc=Inargc;argv=Inargv;
		//input程序的附加信息 
		if (argc>1)
		{
			FILE* fp=fopen(argv[1],"rb");
			if (fp)
			{
				fseek(fp, 0, SEEK_END);unsigned int n=ftell(fp);fseek(fp, 0, SEEK_SET );
				if (n) 			
				{
					unsigned char *s_ch=new unsigned char[n+1];		
					if (!s_ch) {printf("Output.h:ERROR when new s_ch\n");exit(4);}		
					fread(s_ch, 1, n, fp);s_ch[n]=0;						
					obj=nlohmann::json::parse((char*)s_ch);
					
					fclose(fp);
					delete s_ch;		
				}
			}
		}	
		//运行程序时附加信息(JSON) 
		if (argc>2)
		{
			FILE* fp=fopen(argv[2],"r");
			if (fp)
			{
				fseek(fp, 0, SEEK_END);unsigned int n=ftell(fp);fseek(fp, 0, SEEK_SET );
				if (n) 			
				{
					unsigned char *s_ch=new unsigned char[n+1];		
					if (!s_ch) {printf("Output.h:ERROR when new s_ch\n");exit(4);}		
					fread(s_ch, 1, n, fp);s_ch[n]=0;						
					nlohmann::json json=nlohmann::json::parse((char*)s_ch);						
					if (err.length()) {printf("Output.h:error when parse loopcfg:%s\n",err.c_str());exit(5);}
					if (!json.is_object()) {fprintf(stderr,"Output.h:error loopcfg format\n");exit(5);}
					for (nlohmann::json::iterator it = json.begin(); it != json.end(); ++it) obj[it.key()]=it.value();					
					//Json::object obj2=json.object_items();for (auto &k: obj2) obj[k.first]=k.second;
					fclose(fp);
					delete s_ch;		
				}
			}
		}
		//处理运行时间(来自bat) 
		if (argc>4)	obj["_runtime"]=atoi(argv[4])*1.0/1000;		 
		//处理返回值(来自bat) 
		if (argc>5)	obj["_return"]=atoi(argv[5]);			 
		//处理程序名(来自bat) 
		if (argc>6)	obj["_name"]=argv[6];		 
				
	}
	nlohmann::json &json(){return obj;}
	~Output()
	{
		//打印JSON 
		if (argc>3)
		{
			FILE* fp=fopen(argv[3],"a");
			fprintf(fp,",%s",obj.dump().c_str());
			fclose(fp);
		}			
	}
	int checkExitStatus()
	{	
		if (argc<=5) return false;
		return atoi(argv[5]);
	}
	void Warn(const char *msg=NULL)
	{
		if(msg)	printf("||||||||||||||||||||||[JUDGER:%s]||||||||||||||||||||||\n"); 
	}
	void Error(const char *msg=NULL,int status=2)
	{
		Warn(msg);exit(status);
	}	
};
