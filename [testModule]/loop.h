#include "stdio.h"
#include "json/src/json.hpp"
#include <windows.h>
#include <psapi.h>
#include <time.h>
#pragma message("need link args -lpsapi")
/*
..\..\%%x !loopcfg! !cd!\input.txt <input.txt >output.txt
*/

/*
The working set is the amount of memory physically mapped to the process context at a given time.
Memory in the paged pool is system memory that can be transferred to the paging file on disk (paged) when it is not being used.
Memory in the nonpaged pool is system memory that cannot be paged to disk as long as the corresponding objects are allocated.
The pagefile usage represents how much memory is set aside for the process in the system paging file. When memory usage is too high
, the virtual memory manager pages selected memory to disk. 
When a thread needs a page that is not in memory, the memory manager reloads it from the paging file.
*/
struct Loop
{
	nlohmann::json json;
	char * input;
	double starttime;
	unsigned long startmemory;
	Loop(int Inargc,char **Inargv)
	{
		argc=Inargc;argv=Inargv;
		json ={};
		starttime=this->gettime(true);
		startmemory=0;startmemory=this->perkmemory();
		input=NULL;
		if (argc>2) input=argv[2];
	}
	~Loop()
	{
		this->json["_memory"]=(int)(this->perkmemory(true));
		this->json["_loop__runtime"]=this->gettime(true)-starttime;		
		//打印JSON 
		if (argc>1)
		{
			FILE* fp=fopen(argv[1],"w");
			if (fp)			
			fprintf(fp,"%s",json.dump().c_str());
			fclose(fp);
		}			
	}	
	long old,now,cnl;
	double cancletime(bool mode=false)
	{
		if (mode) old+=clock()-cnl;
		cnl=clock();
			
	}
	double gettime(bool mode=false)
	{			
		old=now;now=clock();
		if (mode) return now*1.0/CLOCKS_PER_SEC;
		return (now-old)*1.0/CLOCKS_PER_SEC;
	}
	void memoryinfo()
	{
		static PROCESS_MEMORY_COUNTERS pmc;static unsigned long now,old;
		if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)) ) return;
		printf("===================================\n");
		printf( "\tPageFaultCount:             %dk\n", pmc.PageFaultCount/1024 );
        printf( "\tPeakWorkingSetSize:         %dk\n", pmc.PeakWorkingSetSize/1024 );   
        printf( "\tWorkingSetSize:             %d\n", pmc.WorkingSetSize/1024 );	//使用的物理内存总量
        printf( "\tQuotaPeakPagedPoolUsage:    %dk\n", pmc.QuotaPeakPagedPoolUsage/1024 );
        printf( "\tQuotaPagedPoolUsage:        %dk\n", pmc.QuotaPagedPoolUsage/1024 );
        printf( "\tQuotaPeakNonPagedPoolUsage: %dk\n", pmc.QuotaPeakNonPagedPoolUsage/1024 );
        printf( "\tQuotaNonPagedPoolUsage:     %dk\n", pmc.QuotaNonPagedPoolUsage/1024 );
        printf( "\tPagefileUsage:              %dk\n", pmc.PagefileUsage/1024 ); 	//使用的物理内存中可换出的部分 
        printf( "\tPeakPagefileUsage:          %dk\n", pmc.PeakPagefileUsage/1024 );
	}
	SIZE_T perkmemory(int mode=false)
	{
		static PROCESS_MEMORY_COUNTERS pmc;static SIZE_T now=0,old=0;
		if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)) ) return -1;
		old=now;now	=pmc.PeakWorkingSetSize		
					//+pmc.PeakPagefileUsage								
					//+pmc.QuotaPeakPagedPoolUsage
					//+pmc.QuotaPeakNonPagedPoolUsage
					;
		if (mode==2) return now;
		if (mode) return now-startmemory;
		return now-old;		
	}
	void Warn(const char *msg=NULL)
	{
		if(msg)	fprintf(stderr,"||||||||||||||||||||||[JUDGER:%s]||||||||||||||||||||||\n"); 
	}
	void Error(const char *msg=NULL,int status=2)
	{
		Warn(msg);exit(status);
	}	
	private:		
		int argc;
		char** argv;
		std::string err;
};
