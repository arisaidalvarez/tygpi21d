#ifndef DOSJUN_CODE_H
#define DOSJUN_CODE_H

/* I N C L U D E S /////////////////////////////////////////////////////// */

/* D E F I N E S ///////////////////////////////////////////////////////// */

#define MAX_GLOBALS		20
#define MAX_LOCALS		20
#define MAX_TEMPS		20
#define MAX_CONSTS		200

typedef enum {
	coNOP,
	/* Stack */
	coPushGlobal = 0x10,
	coPushLocal = 0x11,
	coPushTemp = 0x12,
	coPushLiteral = 0x1F,
	coPopGlobal = 0x20,
	coPopLocal = 0x21,
	coPopTemp = 0x22,
	/* Arithmetic */
	coAdd = 0x30,
	coSub = 0x31,
	coMul = 0x32,
	coDiv = 0x33,
	/* Comparison */
	coEQ = 0x40,
	coNEQ = 0x41,
	coLT = 0x42,
	coLTE = 0x43,
	coGT = 0x44,
	coGTE = 0x45,
	/* Branching */
	coJump = 0x50,
	coJumpFalse = 0x51,
	coReturn = 0x52,
	/* Builtins */
	coCombat = 0xA0,
	coPcSpeak = 0xA1,
	coText = 0xA2,
	coUnlock = 0xA3
} code_op;

/* S T R U C T U R E S /////////////////////////////////////////////////// */

/* P R O T O T Y P E S /////////////////////////////////////////////////// */

#endif