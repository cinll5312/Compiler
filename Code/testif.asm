;************** testif ****************
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
int_i	DW	3
_start1:
	PUSH	WORD [int_i]
	PUSH	0
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go2
	JMP	_go3
_go2:
	itostr	int_i, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
_go3:
	MOV	AX, 4C00H
	INT	21H
