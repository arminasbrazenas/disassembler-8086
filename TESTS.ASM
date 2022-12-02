.MODEL 	SMALL

B_SEG 	SEGMENT

ORG		100h
ASSUME 	DS:B_SEG, CS:B_SEG, SS:B_SEG

START:
    ; Unknown tests.
    DB		69h
    
    ; Segment override tests
    MOV 	AX, ES:[2h]
    ADD 	DL, CS:[0E54Fh]
    SUB 	BH, SS:[BX + DI + 45FEh]
    XCHG 	CX, DS:[BP - 54h]

    ; MOV tests
    MOV 	DX, WORD PTR ES:[645Eh]
    MOV		BX, DX
    MOV 	ES, WORD PTR CS:[0FFh]
    MOV 	SS, CX
    MOV		AX, WORD PTR DS:[3h]
    MOV		DS:[12h], AL
    MOVSB
    REPNE	MOVSW
    MOV		DX, 1234h
    MOV		BYTE PTR [BX + DI + 21h], 78h
    
    ; PUSH tests
    PUSH	DS
    PUSH	AX
    PUSHF
    PUSH	ES:[465Eh]
    
    ; POP tests
    POP		ES
    POP		CX
    POP		DS:[BX + DI + 0AAAAh]
    POPF
    
    ; ADD tests
    ADD		AX, BX
    ADD 	CX, DS:[0654h]
    ADD		DX, 6h
    ADD		WORD PTR [BX], 1352h
    
    ; SUB tests
    SUB		CX, DS:[12h]
    SUB		AX, 45h
    SUB		WORD PTR [BX + 654h], 64h
    
    ; INC tests
    INC		AX
    INC		BYTE PTR [BX + 45h]
    
    ; DEC tests
    DEC		CX
    DEC		WORD PTR ES:[645h]
    
    ; CMP tests
    CMP		DX, WORD PTR DS:[465h]
    CMP		AX, 0
    CMP		[BX + SI], 0FFFFh
    CMPSB
    CMPSW
    
    ; MUL tests
    MUL		AL
    MUL		BX
    MUL		BYTE PTR[BX + DI + 465h]
    IMUL	BL
    IMUL	DX
    IMUL	WORD PTR [BP + 1h]
    
    ; DIV tests
    DIV		CL
    DIV		DX
    DIV		BYTE PTR[BP + DI + 465h]
    IDIV	BL
    IDIV	AX
    IDIV	WORD PTR ES:[0]
    
    ; CALL tests
    PROC PROCEDURE
    PROCEDURE ENDP
    
    CALL	PROCEDURE			 	; Near direct call
    DB 		9Ah, 12h, 34h, 56h, 78h	; Workaround for far direct call
    CALL	WORD PTR CS:[0ABCDh] 	; Near indirect call
    CALL 	DWORD PTR CS:[4564h] 	; Far indirect call
    
    ; RET tests
    RET		12h
    RET
    RETF 	0ABCDh
    IRET
    
    ; Conditional jump tests
    JO 		CONDITIONAL_JMP_LABEL
    JNO 	CONDITIONAL_JMP_LABEL
    JB		CONDITIONAL_JMP_LABEL
    JAE		CONDITIONAL_JMP_LABEL
    JE		CONDITIONAL_JMP_LABEL
    JNE		CONDITIONAL_JMP_LABEL
    JBE		CONDITIONAL_JMP_LABEL
    JA		CONDITIONAL_JMP_LABEL
    JS		CONDITIONAL_JMP_LABEL
    JNS		CONDITIONAL_JMP_LABEL
    JP		CONDITIONAL_JMP_LABEL
    JNP		CONDITIONAL_JMP_LABEL
    JL		CONDITIONAL_JMP_LABEL
    JGE		CONDITIONAL_JMP_LABEL
    JLE		CONDITIONAL_JMP_LABEL
    JG		CONDITIONAL_JMP_LABEL
    CONDITIONAL_JMP_LABEL:
    
    ; JMP tests
    JMP_LABEL:
    JMP		JMP_LABEL 					; Short direct jump
    JMP		START 	 					; Near direct jump
    DB		0EAh, 78h, 56h, 34h, 12h 	; Workaround for far direct jump
    JMP		WORD PTR CS:[0DDFFh] 		; Near indirect jump
    JMP		DWORD PTR CS:[0FFh] 		; Far indirect jump
    
    ; LOOP tests
    LOOP_LABEL:
    LOOP 	LOOP_LABEL
    LOOPE 	LOOP_LABEL
    LOOPNE 	LOOP_LABEL
    JCXZ	LOOP_LABEL
    
    ; INT tests
    INT 	3
    INT 	21h
    INT 	0FFh
        
    ; Other
    AAD
    AAM
    LOCK	MOV BX, AX
    IN 		AL, 0F5h
    OUT 	12h, AX
    TEST	AX, 465h
    CLC
    STC
    CLI
    STI
    CLD
    STD
    XLAT
    LAHF
    NOP
    AAS
    AAA
    DAS
    DAA
    STOSB
    STOSW
    LODSB
    LODSW
    SCASB
    SCASW
    LAHF
    SAHF
    CBW
    CWD
    ROL 	AX, CL
    ROR		BX, 1
    RCL		BYTE PTR ES:[124h], CL
    RCR		DL, CL
    SHL		DH, 1
    SHR 	WORD PTR DS:[6454h], 1
    SAR		AX, 1
    
B_SEG 	ENDS

END		START