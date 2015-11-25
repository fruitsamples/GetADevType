/*	Sample code to demonstrate use of the LAP Manager call to determine	the current connection type i.e LocalTalk, EtherTalk phase 1/2, or	TokenTalk phase 2.	Rich Kubota 	DTS		Requires the following Assembler glue routine to make the LAP Manager call;*******************;File: CallLAPMgr.a;*******************; C prototype ; pascal long CallLAPMgr(short selector);; Pascal prototype; Function CallLAPMgr(selector : integer): longint;*******************LAPMgrPtr   EQU 		$B18   		; This points to our start (???ATalkHk2?)LAPMgrCall  EQU 		2     		; Offset to make LAP manager callsCallLAPMgr	PROC	EXPORT		LINK		A6,#0			; set up stack frame		MOVE.W		8(A6),D0		; move selector parameter into D0		MOVE.L		A2,-(A7)		; store A2 on stack		MOVEA.L		LAPMgrPtr,A2	; Set A2 to address of LAP Mgr.		JSR			LAPMgrCall(A2)	; Call LAP Manager		MOVE.L		D1,$A(A6)		; Place result onto stack		MOVE.L		(A7)+,A2		; Restore A2		UNLK		A6				; restore stack frame		MOVEA.L		(A7)+,A0		; put return address into A0		ADDQ.W		#$2,A7			; clear off the parameter		JMP			(A0)			; return to caller		RTS		ENDP				END;*******************;End of file;**************************************Make Instructions*******************Asm -case off CallLAPMgr.aC -r  'GetADEVType.c'Link -d -c 'MPS ' -t MPST �	CallLAPMgr.a.o 'Kubota:Desktop Folder:GetADEVType.c.o' �	"{CLibraries}"StdClib.o �	"{Libraries}"Stubs.o �	"{Libraries}"Runtime.o �	"{Libraries}"Interface.o �	-o GetADEVType*******************	end of comments********************/#include <Types.h>#include <stdio.h>#define LGetATalkInfo  0x09   /* Get AppleTalk info *//* 'atlk' resource ID's of Apple supplied driver software. */#define	LTalk		0#define	ETalkPh1	2#define	TTalkPh2	5#define	ETalkPh2	10pascal long CallLAPMgr( short selector);main(){	long	result;	char	adevType;	char	slot;		Debugger();	result = CallLAPMgr(LGetATalkInfo); 	adevType = result & 0x000000FF;  /* atlk resource id is return in LSB */	slot = result>>24;				 /* card slot returned in MSB */		switch (adevType) {		case LTalk:				printf("LocalTalk is the current connection.");				break;		case ETalkPh1:				printf("EtherTalk phase 1 is the current connection in slot ");				break;		case ETalkPh2:				printf("EtherTalk phase 2 is the current connection in slot ");				break;		case TTalkPh2:				printf("TokenTalk is the current connection in slot ");				break;		default:				printf("Unrecognized connection in slot ");				break;		}	if (slot == 0)		printf("\n");	else		printf("%d.\n", slot);}