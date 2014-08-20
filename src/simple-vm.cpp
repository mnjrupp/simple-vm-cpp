// simple-vm.cpp : Defines the entry point for the console application.
//

#include "simple-vm.h"
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


const char *Bytecode::opname[] = {NULL,"iadd","isub","imul","ilt","ieg","br","brt",
						  "brf","iconst","load","gload","store","gstore",
						  "print","pop","halt","call","ret"};
		int Bytecode::operands[] ={0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,2,0};

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
VM::VM(int *_code,int length,int main, int datasize){
	code = _code;
	arr_size = length;
	nglobals = datasize;
	ip = main;								// where execution begins
	globals = new int[datasize];
	stack = new int[DEFAULT_STACK_SIZE];
    sp = -1;
	fp = -1;
	trace = false;
	

};


 void VM::cpu(){
	 int addr,offset,nargs,rvalue,a,b;
	  int opcode = code[ip];
		while(opcode!= Bytecode::HALT && ip<arr_size){
		  //fetch
		  const char *instr = VM::opname[opcode];
		 //char *instr = bytecode->instructions[opcode].name;
		 if(trace){
			 VM::disassemble(ip,opcode);
		  }
		 ip++; //inc instruction pointer
		 switch(opcode){
			 case Bytecode::IADD:
				b = stack[sp--];   			// 2nd opnd at top of stack
				a = stack[sp--]; 			// 1st opnd 1 below top
				stack[++sp] = a + b;      	// push result
				break;
			 case Bytecode::ISUB:
				 b= stack[sp--];
				 a = stack[sp--];
				 stack[++sp] = a-b;
				 break;
			 case Bytecode::IMUL:
				 b= stack[sp--];
				 a = stack[sp--];
				 stack[++sp] = a * b;
				 break;
			 case Bytecode::ILT:
				 b = stack[sp--];
				 a = stack[sp--];
				 stack[++sp] = (a < b) ? TRUE : FALSE;
				break;
			 case Bytecode::IEQ:
				 b = stack[sp--];
				 a = stack[sp--];
				 stack[++sp] = (a == b) ? TRUE : FALSE;
				 break;
			case Bytecode::BR:
				ip = code[ip++];
				break;
			case Bytecode::BRT:
				addr = code[ip++];
				if(stack[sp--]==TRUE) ip=addr;
				break;
			case Bytecode::BRF:
				addr = code[ip++];
				if(stack[sp--]==FALSE) ip=addr;
				break;
			case Bytecode::ICONST:
				stack[++sp] = code[ip++]; // push operand
				break;
			case Bytecode::LOAD:	//load local or arg; 1st local is fp+1, args are fp-3, fp-4, fp-5, ...
				offset = code[ip++];
				stack[++sp] = stack[fp+offset];
				break;
			case Bytecode::PRINT:
				printf("%4u",stack[sp--]);
				break;
			case Bytecode::GLOAD:
				addr = code[ip++];
				stack[++sp] = globals[addr];
				break;
			case Bytecode::STORE:
				offset = code[ip++];
				stack[fp+offset] = stack[sp--];
				break;
			case Bytecode::GSTORE:
				addr = code[ip++];
				globals[addr] = stack[sp--];
				break;
			case Bytecode::POP:
				--sp;
				break;
			case Bytecode::HALT:
				break;
			case Bytecode::CALL:
				//expects all arguments on stack
				addr = code[ip++];	//target address of function
				nargs = code[ip++]; //how many args got pushed
				stack[++sp] = nargs; // save num args
				stack[++sp] = fp;	// save fp
				stack[++sp] = ip;	// push return address
				fp = sp;			// fp points at RET address on stack
				ip = addr;			// jump to function
				// code preamble of func must push space for locals
				break;
			case Bytecode::RET:
				rvalue = stack[sp--];	// pop return value
				sp = fp;				// jump over locals to fp
				ip = stack[sp--];		// pop return address, jmp to it
				fp = stack[sp--];		// restore fp
				nargs = stack[sp--];	// how many args to throw away?
				sp -= nargs;			// pop args
				stack[++sp] = rvalue;	// leave result on stack
				break;
			default:
				printf("invalid opcode %d\n",opcode);
			}
				VM::dumpStack();
				opcode = code[ip];
		}
		 if(trace){
			 VM::disassemble(ip,code[ip]);
			 VM::dumpStack();
			 VM::dumpDataMemory();
		 }
	};
 void VM::exec(){
	 VM::cpu();
 }

 void VM::dumpStack(){
	 int i = 0;
	 printf("  stack=[");
	   for(i;i<=sp;i++){
		   //if(stack[i]<0) break;
		   //if (stack[i]>=0)
		   if(i>0){
			   printf(",%d",stack[i]); 
		   }else{

			   printf("%d",stack[i]);
		   }
	   }
		   printf("]\n");
	   
 };

  void VM::disassemble(int ip,int opcode){
	   const char *instr = VM::opname[opcode];
	   int nops = VM::operands[opcode];
	   printf("%04d: %6s",ip,instr);

	   if(nops==1){
		   printf("%4d",code[ip+1]);
	   }
	   else if(nops==2){
		   printf(" %d %d",code[ip+1],code[ip+2]);
	   }
	   else{
		   if (opcode!=14){
			  printf("    ");
		   }
	   }
	   //need to print what is on the stack
	   

 }

  void VM::dumpDataMemory(){
	  
	  printf("\tData memory\n\t---------\n");

	  for(int i=0;i<nglobals;i++){
		  if(globals[i]>=0){
		  printf("\t%04d: %d\n",i,globals[i]);
		  }
	  }
	  printf("\n");
  }

  void VM::dumpCodeMemory(){
	 
	  printf("\tCode memory:\n\t---------\n");

	  for(int i=0;i<arr_size;i++){
		  if(code[i]>=0){
		  printf("\t%04d: %d\n",i,code[i]);
		  }
	  }
	   printf("\n");
  }
int Hello[]={
	Bytecode::ICONST,1,
	Bytecode::ICONST,2,
	Bytecode::IADD,
	Bytecode::PRINT,
	Bytecode::HALT
};

int loop[] = {
	// .GLOBALS 2; N, I
	// N = 10						ADDRESS
			Bytecode::ICONST, 10,				// 0
			Bytecode::GSTORE, 0,				// 2
	// I = 0
			Bytecode::ICONST, 0,				// 4
			Bytecode::GSTORE, 1,				// 6
	// WHILE I<N:
	// START (8):
			Bytecode::GLOAD, 1,				// 8
			Bytecode::GLOAD, 0,				// 10
			Bytecode::ILT,					// 12
			Bytecode::BRF, 24,				// 13
	//     I = I + 1
			Bytecode::GLOAD, 1,				// 15
			Bytecode::ICONST, 1,				// 17
			Bytecode::IADD,					// 19
			Bytecode::GSTORE, 1,				// 20
			Bytecode::BR, 8,					// 22
	// DONE (24):
	// PRINT "LOOPED "+N+" TIMES."
			Bytecode::HALT					// 24
	};

int factorial[] = {
//.def fact: ARGS=1, LOCALS=0		ADDRESS
//	IF N < 2 RETURN 1
			Bytecode::LOAD, -3,				// 0
			Bytecode::ICONST, 2,			// 2
			Bytecode::ILT,					// 4
			Bytecode::BRF, 10,				// 5
			Bytecode::ICONST, 1,			// 7
			Bytecode::RET,					// 9
//CONT:
//	RETURN N * FACT(N-1)
			Bytecode::LOAD, -3,				// 10
			Bytecode::LOAD, -3,				// 12
			Bytecode::ICONST, 1,			// 14
			Bytecode::ISUB,					// 16
			Bytecode::CALL, 0, 1,			// 17
			Bytecode::IMUL,					// 20
			Bytecode::RET,					// 21
//.DEF MAIN: ARGS=0, LOCALS=0
// PRINT FACT(10)
			Bytecode::ICONST, 2,			// 22    <-- MAIN METHOD!
			Bytecode::CALL, 0, 1,			// 24
			Bytecode::PRINT,				// 27
			Bytecode::HALT					// 28
	};

int _tmain(int argc, _TCHAR* argv[])
{
	int t1=0,t2=0;
	int datasize = 1;
	int mainip = 0;
	t1 = (clock() / (CLOCKS_PER_SEC / 1000));
	VM *vm = new VM(Hello,7,mainip,datasize);
	vm->trace=true;
	vm->exec();
	t2 = (clock() / (CLOCKS_PER_SEC / 1000));
	vm->dumpCodeMemory();
	
	printf("duration = %d ms\n\n", (t2 - t1));

	datasize = 0;
	mainip = 0;
	t1 = (clock() / (CLOCKS_PER_SEC / 1000));
	VM *vm2 = new VM(loop,25,mainip,datasize);
	vm2->trace=true;
	vm2->exec();
	t2 = (clock() / (CLOCKS_PER_SEC / 1000));
	vm2->dumpCodeMemory();
	printf("duration = %d ms\n\n", (t2 - t1));

	datasize = 0;
	mainip = 22;
	t1 = (clock() / (CLOCKS_PER_SEC / 1000));
	VM *vm3 = new VM(factorial,29,mainip,datasize);
	vm3->trace=true;
	vm3->exec();
	t2 = (clock() / (CLOCKS_PER_SEC / 1000));
	vm3->dumpCodeMemory();
	printf("duration = %d ms\n", (t2 - t1));
	return 0;
}

