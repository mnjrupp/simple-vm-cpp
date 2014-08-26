
#pragma once
#include "simple-vm.h"

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

int main()
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