#include <time.h>
#include <stdio.h>
#include <iostream>
using std::ios;
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

#include <cstring>


#define DEFAULT_STACK_SIZE 1000
#define OPERANDS_ARRAY_SIZE 20
#define FALSE 0
#define TRUE 1

class VM;

class Bytecode{
	friend class VM;
	// INSTRUCTION BYTECODES
	public:
		  static const char *opname[20];
		  static int operands[20];
		
		static const unsigned short IADD = 1;    //int add
		static const unsigned short ISUB = 2;
		static const unsigned short IMUL = 3;
		static const unsigned short ILT = 4;	//int less than
		static const unsigned short IEQ = 5;	//int equal
		static const unsigned short BR = 6;		//branch
		static const unsigned short BRT = 7;	// branch if true
		static const unsigned short BRF = 8;	// branch if false
		static const unsigned short ICONST = 9;	// push const int
		static const unsigned short LOAD = 10;
		static const unsigned short GLOAD = 11;
		static const unsigned short STORE = 12;
		static const unsigned short GSTORE = 13;
		static const unsigned short PRINT = 14;
		static const unsigned short POP = 15;
		static const unsigned short HALT = 16;
		static const unsigned short CALL = 17;
		static const unsigned short RET = 18;
		static const unsigned short INIT = 19;

		
		
		Bytecode(){};
		//~Bytecode();
	

} ;

#define MAX_CHARS_PER_LINE  512
#define MAX_TOKENS_PER_FILE  1024
#define DELIMITER  ","

class Parser:Bytecode{
private:
	
	//char* token[MAX_TOKENS_PER_LINE];
	int token[MAX_TOKENS_PER_FILE];
	const char* infilename;
	//const char* outfilename;
	ifstream fin;
	int find(char* opstr);
	char* lowercase(char*opstring);
	void setiaddr(int i){iaddr = i;}		//Set starting index based on the INIT oprand found during parsing file
	void setszToken(int s){szToken = s;}
public:
	int iaddr;								// hold starting index
	int szToken;							// length of token array
	Parser(const char *ifilename);
	int* parse();
	int getiaddr(){return iaddr;}							// Get the array index of starting operand
	int getszToken(){return szToken;}

};

class VM : Bytecode{
public:
	int *globals;
	int *code;
	int *stack;

	int ip;
	int sp;
	int fp;

	int arr_size;
	int nglobals;	
	 
	
	int trace;
	VM(int *_code,int length,int main, int datasize,const char* ofilename);
	~VM(){/*delete [] globals;delete [] stack;*/if(fout!=NULL){fclose(fout);}}
	void disassemble(int ip,int opcode);
	void dumpDataMemory();
	void dumpCodeMemory();
	void dumpStack();
	void cpu();
	void exec();
private:
	const char* outfilename;
	FILE *fout;
	 
};