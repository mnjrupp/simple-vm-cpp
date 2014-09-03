// simple-vm.cpp : Defines the entry point for the console application.
//

#include "simple-vm.h"
const char *Bytecode::opname[] = {NULL,"iadd","isub","imul","ilt","ieq","br","brt",
						  "brf","iconst","load","gload","store","gstore",
						  "print","pop","halt","call","ret"};
		int Bytecode::operands[] ={0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,2,0};

VM::VM(int *_code,int length,int main, int datasize){
	code = _code;
	arr_size = length;
	nglobals = datasize;
	ip = main;								// where execution begins
	globals = new int[datasize];
	stack = new int[DEFAULT_STACK_SIZE];
    sp = -1;								// stack point starts at -1
	fp = -1;							    // frame pointer starts at -1
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
				addr = code[ip++];
				ip = addr;
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
				stack[++sp] = ip;	// push return address after the CALL in MNEMONICS
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
		    if(trace){
				VM::dumpStack();
			}
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
	 printf("      stack=[");
	   for(i;i<=sp;i++){
		   //if(stack[i]<0) break;
		   //if (stack[i]>=0)
		   if(i>0){
			   printf(",%d=%d",i,stack[i]); 
		   }else{

			   printf("%d=%d",i,stack[i]);
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


