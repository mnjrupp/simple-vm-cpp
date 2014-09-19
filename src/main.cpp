#pragma once
#include "simple-vm.h"
void printhelp();

int main(int argc, char *argv[])
{
	int t1=0,t2=0;
	int datasize = 0;
	int mainip = 48;
	char *infile=NULL,*outfile=NULL;
	int *bytecode;
	int boolTrace = FALSE;
	if(argc<2){
		printhelp();
		return 0;
	}
	if(argc >= 2){
		infile = argv[1];

	for(int i = 2;i<argc;i++){

		if(strcmp(argv[i],"-d")==0){boolTrace=TRUE;}
	
		else{
			if(strcmp(argv[i],"-s")==0){
				if( i<argc-1){
					datasize=atoi(argv[i+1]);
				}else{printhelp();return 0;}
			}
		if(strcmp(argv[i],"-o")==0){
			if(i<argc-1){outfile = argv[i+1];}
			else{printhelp();return 0;}
				}
			}	
		}
	}
	Parser *par = new Parser(infile);
	bytecode = par->parse();
	// Error parsing file returned -1 in first index of bytecode
	if(bytecode[0]==-1) return -1;

	t1 = (clock() / (CLOCKS_PER_SEC / 1000));
	VM *vm4 = new VM(bytecode,par->getszToken(),par->getiaddr(),datasize,outfile);
	vm4->trace=boolTrace;
	vm4->exec();
	t2 = (clock() / (CLOCKS_PER_SEC / 1000));
	if(boolTrace==TRUE){
	vm4->dumpCodeMemory();
	}
	printf("\n\tduration = %d ms\n", (t2 - t1));

	delete par;
	delete vm4;
	return 0;
}

void printhelp(){
	printf(" inputfile [options] [outputfile]\n");
		printf("          -d debug\n");
		printf("          -s size of global data\n");
		printf("          -o outputfile \n");
}

