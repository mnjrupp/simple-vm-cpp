#include <time.h>
#include <stdio.h>


#define DEFAULT_STACK_SIZE 1000
#define FALSE 0
#define TRUE 1

class VM;

class Bytecode{
	friend class VM;
	// INSTRUCTION BYTECODES
	public:
		  static const char *opname[19];
		  static int operands[19];
		
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

		
		
		Bytecode(){};
		~Bytecode(){delete this;}
	

} ;




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
	 
	
	bool trace;
	VM(int *_code,int length,int main, int datasize);
	~VM(){delete globals;delete stack;}
	void disassemble(int ip,int opcode);
	void dumpDataMemory();
	void dumpCodeMemory();
	void dumpStack();
	void cpu();
	void exec();
	
	 
};