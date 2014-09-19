// simple-vm.cpp : Defines the entry point for the console application.
//

#include "simple-vm.h"
const char *Bytecode::opname[] = {NULL,"iadd","isub","imul","ilt","ieq","br","brt",
						  "brf","iconst","load","gload","store","gstore",
						  "print","pop","halt","call","ret","init"};
		int Bytecode::operands[] ={0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,2,0,0};

VM::VM(int *_code,int length,int main, int datasize,const char* ofilename){
	code = _code;
	arr_size = length;
	nglobals = datasize;
	ip = main;								// where execution begins
	globals = new int[datasize];
	stack = new int[DEFAULT_STACK_SIZE];
    sp = -1;								// stack point starts at -1
	fp = -1;							    // frame pointer starts at -1
	trace = FALSE;
	outfilename = ofilename;
	fout = NULL;

};


 void VM::cpu(){
	 int addr,offset,nargs,rvalue,a,b;
	 if(outfilename!=NULL){
	  fout = fopen(outfilename,"a+");
	  if(fout == NULL){
		  fprintf(stderr,"Failed to open '%s' file\n", outfilename);
		  }
	 }
	  int opcode = code[ip];
		while(opcode!= Bytecode::HALT && ip<arr_size){
		  //fetch
		  const char *instr = VM::opname[opcode];
		 //char *instr = bytecode->instructions[opcode].name;
		 if(trace==1){
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
			case Bytecode::PRINT:
				if(fout){fprintf(fout,"%4u",stack[sp--]);}
				else{printf("%4u",stack[sp--]);}
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
			case Bytecode::INIT:		// Not used at this time; only used in parsing file
				break;						// Will be used once registers are implemented
			default:
				if(fout!=NULL){
				 fprintf(fout,"invalid opcode %d\n",opcode);
				}else{
				 printf("invalid opcode %d\n",opcode);
				}
			}
		    if(trace==1){
				VM::dumpStack();
			}
				opcode = code[ip];
		}
		 if(trace==1){
			 VM::disassemble(ip,code[ip]);
			 VM::dumpStack();
			 VM::dumpDataMemory();
		 }
		 //if(fout!=NULL){fclose(fout);}
	};
 void VM::exec(){
	 VM::cpu();
 }

 void VM::dumpStack(){
	 int i = 0;
	 if(fout!=NULL){
	    fprintf(fout,"      stack=[");
	 }else{
		printf("      stack=[");
	 }
	   for(i;i<=sp;i++){
		   //if(stack[i]<0) break;
		   //if (stack[i]>=0)
		   if(i>0){
			   if(fout!=NULL){
				fprintf(fout,",%d=%d",i,stack[i]);
			   }else{
				 printf(",%d=%d",i,stack[i]);
			   }
		   }else{
			   if(fout!=NULL){
			   fprintf(fout,"%d=%d",i,stack[i]);
			   }else{
				    printf("%d=%d",i,stack[i]);
			   }
		   }
	   }
	   if(fout!=NULL){
		   fprintf(fout,"]\n");
	   }else{
		   printf("]\n");
	   }
	   
 };

  void VM::disassemble(int ip,int opcode){
	   const char *instr = VM::opname[opcode];
	   int nops = VM::operands[opcode];
	   if(fout!=NULL){
		fprintf(fout,"%04d: %6s",ip,instr);
	   }else{
		printf("%04d: %6s",ip,instr);
	   }

	   if(nops==1){
		   if(fout!=NULL){
			fprintf(fout,"%4d",code[ip+1]);
		   }else{
		   printf("%4d",code[ip+1]);
		   }
	   }
	   else if(nops==2){
		   if(fout!=NULL){
			   fprintf(fout," %d %d",code[ip+1],code[ip+2]);
		   }else{
				printf(" %d %d",code[ip+1],code[ip+2]);
		   }
	   }
	   else{
		   if (opcode!=14){
			   if(fout!=NULL){
				 fprintf(fout,"    ");
			   }else{
				 printf("    ");
			   }
		   }
	   }
	   //need to print what is on the stack
	   

 }

  void VM::dumpDataMemory(){
	  
	  if(fout!=NULL){
		  fprintf(fout,"\n\tData memory\n\t---------\n");
	  }else{
		printf("\n\tData memory\n\t---------\n");
	  }

	  for(int i=0;i<nglobals;i++){
		  if(globals[i]>=0){
			if(fout!=NULL){
				fprintf(fout,"\t%04d: %d\n",i,globals[i]);
			}else{
				printf("\t%04d: %d\n",i,globals[i]);
			}
		  }
	  }
	  if(fout!=NULL){
		fprintf(fout,"\n");
	  }else{
	  printf("\n");
	  }
  }

  void VM::dumpCodeMemory(){

	  if(fout!=NULL){
		  fprintf(fout,"\n\tCode memory:\n\t---------\n");
	  }else{
		  printf("\n\tCode memory:\n\t---------\n");
	  }

	  for(int i=0;i<arr_size;i++){
		  if(code[i]>=0){
			   if(fout!=NULL){
				   fprintf(fout,"\t%04d: %d\n",i,code[i]);
			   }else{
					printf("\t%04d: %d\n",i,code[i]);
			   }
		  }
	  }
	   if(fout!=NULL){
		   fprintf(fout,"\n");
	   }else{
		printf("\n");
	   }
  }

  Parser::Parser(const char* ifilename){
	 infilename = ifilename; 

  }

  int* Parser::parse(){
	   char buf[MAX_CHARS_PER_LINE];
	   char* tk;
	   static int n = 0; // a for-loop index
	   // array to store memory addresses of the tokens in buf
        // initialize to 0
	   fin.open(infilename); // open a file
  if (!fin.good()){ 
	  token[0]=-1;
	  printf("\nError opening file. Make sure file exists in Directory.\n");
    return token; // exit if file not found
  }
 
  
  // read each line of the file
  while (!fin.eof())
  {
    // read an entire line into memory
   
    fin.getline(buf, MAX_CHARS_PER_LINE);
    
    // parse the line into comma-delimited tokens
    
    // parse the line
	if(strcspn(buf,"/")<2){					//ignore "//" comments
	}else{
	 tk = strtok(buf, DELIMITER);			// first token
	 char* lower = lowercase(tk);
	  if (strcmp(lower,"init")==0){
		  setiaddr(n);
	  }else{
			int idx  = find(tk);
			if(idx!=-1){token[n++] = idx;}
			while(tk!=NULL)							// zero if line is blank

			{
				tk = strtok(NULL,DELIMITER);
				if(tk==NULL || strcspn(tk,"/")<strlen(tk)) break; // Do not want to parse "//" comments
				token[n++] = atoi(tk); // subsequent tokens
		
			  }
	  }
    }
  }
	setszToken(n);
	return token;
  }


  int Parser::find(char* opstr){
	  int indx = -1;
	 
	  char* lower = lowercase(opstr);
	  for(int i = 1;i<OPERANDS_ARRAY_SIZE;i++){
		  int eq = strcmp(lower,opname[i]);
		 if(eq==0){
			 indx=i;break;
		 }
	  }

	  return indx;
  }

  char* Parser::lowercase(char* opstring){
	  unsigned s = strlen(opstring);
	  int j = 0;
	  char  lcase[10];
	  for(unsigned i = 0;i<s;i++){
		  if(opstring[i]=='/') break;
		  if(opstring[i]>='A' && opstring[i]<='Z'){
		  lcase[j]=tolower(opstring[i]);
		  j++;
		  }
		  lcase[j]='\0';
	  }
	  return lcase;
  }
