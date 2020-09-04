	ORG	$0

ENTRY	GD		; read current overlay
	MM	CUROVR	;
	LV	$0	; clear accumulator
	JP	BEGIN	; jump to program start

OFF1	CONST	$0	; Overlay offset
OFF0	CONST	$0	;

BEGIN	GD		; read absolute byte
	MM	ABSOL	;
	GD		; read first address byte
	MM	ADDR1	; 
	GD		; read second address byte
	MM	ADDR0	;
	GD		; read data byte
	MM	DATA	;



; --------------------------------
;	DATA AREA
; --------------------------------

MMOP	CONST	$90	; Memory modify operator
CUROVR	CONST	$0	; current overlay

ABSOL	CONST	$0	; absolute byte
ADDR1	CONST	$0	; first address byte
ADDR0	CONST	$0	; second address byte
DATA	CONST	$0	; data byte

; Overlay sizes
OVR01	CONST	$0	; 0VR01
	CONST	$0	; 0VR00
	CONST	$0	; 0VR11
	CONST	$0	; 0VR10
	CONST	$0	; ...
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
	CONST	$0
