	ORG	$0

INIT	LV	$0	; clear accumulator
	MM	STATE	; clear STATE
	JP	BEGIN	; jump to program start

STATE	CONST	$0	; variable, contains error code, if any
WRITE1	CONST	$0	; writes first instruction byte, modified at runtime 
    LD	INSTR0	; reads second instruction byte
WRITE0	CONST	$0	; writes second instruction byte, modified at runtime

BEGIN	GD		; read first address byte
	MM	ADDR1	; save to ADDR1
	GD		; read second address byte
	MM	ADDR0	; save to ADD0
	GD		; read first instruction byte
	MM	INSTR1	; save to INSTR1
	GD		; read second instruction byte
	MM	INSTR0	; save INSTR0
	GD		; read checksum byte
	MM	SUM	; save to SUM
	ADD	ADDR1	;
	ADD	ADDR0	; calculate
	ADD	INSTR1	; checksum
	ADD	INSTR0	;
	JZ	OK	; jump if correct
	LV	$1	; else save 1
	MM	STATE	; to STATE
	HM	INIT	; and halt machune

OK	SC	CHECK	; call subroutine to check if ending of program was found
	SC 	INC	; call subroutine to increment read address, save to AUX1 and AUX0
	LD	ADDR1	; load first address byte
	ADD	MMOP	; add MM instruction offset
	MM	$8	; save to MS byte of WRITE1	
	LD	ADDR0	; load second address byte
	MM	$9	; save to LS byte of WRITE1
	LD	AUX1	; load first incremented address byte
	ADD	MMOP	; add MM instruction offset
	MM	$C	; save to MS byte of WRITE0
	LD	AUX0	; laod second incremented address byte
	MM	$D	; save to LS byte of WRITE0
	LD	INSTR1	; load first instruction byte
	JP	WRITE1	; jump to writing section


; --------------------------------------------------------------------
; CHECK subroutine: asserts address is not zero
; address equaling zero means end of program reached
; --------------------------------------------------------------------

CHECK	CONST	$0	; modified at runtime, contains return address
	LD	ADDR1	; load first address byte
	JZ	CHECK2	; jump if zero
	RS	CHECK	; else return

CHECK2	LD	ADDR0	; load second address byte
	JZ	EOP	; jump if also zero
	RS	CHECK	; else return

EOP	HM	INIT	; halt machine

; --------------------------------------------------------------------
; INC subroutine: increment address saved at (ADDR1,ADDR0)
; and save result to (AUX1,AUX0)
; --------------------------------------------------------------------

INC	CONST	$0	; modified at runtime, contains return address
	LV	$1	; load 1 to accumulator
	ADD	ADDR0	; add with ADDR0
	MM	AUX0	; save to AUX0
	JZ	OVFLW	; jump if accumulator overflowed
	LD	ADDR1	; else load ADDR1
	MM	AUX1	; and save it to AUX1
	RS	INC	; return

OVFLW	LV	$1	; load 1 to accumulator
	ADD	ADDR1	; add with ADDR1
	MM	AUX1	; save to AUX1
	RS	INC	; return


; ------------------------
; 	DATA AREA
; ------------------------
MMOP	CONST	$90
INSTR1	CONST	$0
INSTR0	CONST	$0
ADDR1	CONST	$0
ADDR0	CONST	$0
AUX1	CONST	$0
AUX0	CONST	$0
SUM	CONST	$0
	
	END
