#pragma once
#include "simple-vm.h"
//////////////////////////////////////////////////////////////////
// an implementation of the Euclid's Algorithm
// This is used to find the greatest common divisor
// between two numbers. This was taken from Robert Sedgewick's
// "Algorithms in C first edition"
// C emplementation


		//int gcd(int u,int v)
		//{
		//	int t;
		//	while(u>0){
		//		if(u<v)
		//		{t=u;u=v;v=t;}
		//
		//		u = u-v;
		//
		//	}
		//	return v;
		//}

int euclid[] = {
	//.def euclid: ARGS=2, LOCALS=1		ADDRESS
//	RETURN GCD(N,I)
//  LOAD smaller number at top of stack
			Bytecode::LOAD, -3,			// 0
//  LOAD larger number         
            Bytecode::LOAD, -4,			// 2
			Bytecode::GSTORE,0,			// 4
			Bytecode::GSTORE,1,			// 6
//  LOAD 0 top of stack
			Bytecode::ICONST,0,			// 8
			Bytecode::GLOAD, 0,         // 10
//While 0<N 
   			Bytecode::ILT,				// 12
			Bytecode::BRF, 32,			// 13
// IF N<I
			Bytecode::GLOAD, 0,			// 15
			Bytecode::GLOAD, 1,			// 17
			Bytecode::ILT,				// 19
			Bytecode::BRF,35,			// 20 N<I so assign T=N;N=I;I=T;

			Bytecode::GLOAD, 0,			// 22
			Bytecode::GLOAD, 1,			// 24
			Bytecode::GSTORE,0,			// 26
			Bytecode::GSTORE,1,			// 28
			Bytecode::BR,8,				// 30

			Bytecode::GLOAD,1,		    // 32
			Bytecode::RET,				// 34

			Bytecode::GLOAD, 0,			// 35
			Bytecode::GLOAD, 1,			// 37			
			Bytecode::ISUB,				// 39
			Bytecode::GSTORE,0,			// 40
			Bytecode::BR,8,				// 42
			Bytecode::GLOAD,1,			// 44
			Bytecode::GSTORE,1,			// 46
			
//.DEF MAIN: ARGS=0, LOCALS=0
// PRINT GCD(nn)
			Bytecode::ICONST, 461952,		// 48    <-- MAIN METHOD! 1st Arg
            Bytecode::ICONST, 116298,		// 50	 2nd Arg
         	Bytecode::CALL, 0, 2,			// 52  ;Push 2 Args
			Bytecode::PRINT,				// 55  This should print 18
			Bytecode::HALT					// 56
};

int main()
{
	int t1=0,t2=0;
	int datasize = 0;
	int mainip = 48;
	
	t1 = (clock() / (CLOCKS_PER_SEC / 1000));
	VM *vm4 = new VM(euclid,57,mainip,datasize);
	vm4->trace=true;
	vm4->exec();
	t2 = (clock() / (CLOCKS_PER_SEC / 1000));
	vm4->dumpCodeMemory();
	printf("duration = %d ms\n", (t2 - t1));
	return 0;
}