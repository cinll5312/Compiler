;************** testwhile ****************
;
	ORG	100H
	JMP	_start1
_intstr	DB	'     ','$'
_buf	TIMES 256 DB ' '
	DB 13,10,'$'
%include	"dispstr.mac"
%include	"itostr.mac"
%include	"readstr.mac"
%include	"strtoi.mac"
%include	"newline.mac"
int_i	DW	2
_start1:
_go2:
	PUSH	WORD [int_i]
	PUSH	0
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go3
	JMP	_go4
_go3:
	itostr	int_i, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	PUSH	WORD [int_i]
	PUSH	1
	POP	BX
	POP	AX
	SUB	AX, BX
	PUSH	AX
	POP	AX
	MOV	[int_i], AX
	JMP	_go2
_go4:
	MOV	AX, 4C00H
	INT	21H
