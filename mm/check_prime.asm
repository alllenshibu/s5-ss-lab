ORG 0400H;

MOV     AL, 0BH;

MOV     CL, AL;
DEC     CL;

LOOP:
PUSH    AX;
DIV     CL;
CMP     AH, 0H;
JZ      NOTPRIME;
DEC     CL;
CMP     CL, 01H
JE      PRIME;
POP     AX;
JMP     LOOP

NOTPRIME:
MOV AL, 0H;
MOV AH, 4CH;
INT 21H


PRIME:
MOV AL, 1H;
MOV AH, 4CH;
INT 21H

MOV AH, 4CH;
INT 21H;     
