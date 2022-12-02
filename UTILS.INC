;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
; Prints a message to the screen.
; IN
;	MSG - Pointer to message.            
;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
PRINT_MSG MACRO MSG
	PUSH		AX DX
    MOV 		AH, 09h
    LEA 		DX, MSG
    INT 		21h
	POP			DX AX
ENDM

;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
; Closes a file.
; IN
;	FILE_HANDLE - File handle.       
;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
CLOSE_FILE MACRO FILE_HANDLE
	PUSH		AX BX
    MOV 		AH, 3Eh
	MOV 		BX, FILE_HANDLE
    INT 		21h
	POP			BX AX
ENDM

;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
; Puts a char to string.
; IN
;	STR_PTR - Pointer to string.
;	CHAR - Character to put.
; OUT
;	STR_PTR - Pointer to upcoming character.   
;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
SPUT_CHAR MACRO STR_PTR, CHAR
	PUSH		BX
	MOV			BX, STR_PTR
	MOV			BYTE PTR [BX], CHAR
	INC			STR_PTR
	POP			BX
ENDM

;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
; Puts one byte number to string.
; IN
;	STR_PTR - Pointer to string.
;	NUM - Number to put.
;	ZERO_PREFIX - Set to 1 to print leading 0, when number starts with letter.
; OUT
;	STR_PTR - Pointer to upcoming character.   
;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
SPUT_BYTE MACRO STR_PTR, NUM, ZERO_PREFIX
	PUSH		AX BX DX
	MOV			AL, NUM
	MOV 		AH, 0
	MOV			BX, STR_PTR
	MOV			DL, 0
	MOV			DH, ZERO_PREFIX
	CALL		SPUT_HEX
	MOV			STR_PTR, BX
	POP			DX BX AX
ENDM

;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
; Puts two byte number to string.
; IN
;	STR_PTR - Pointer to string.
;	NUM - Number to put.
; OUT
;	STR_PTR - Pointer to upcoming character.   
;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
SPUT_WORD MACRO STR_PTR, NUM
	PUSH		AX BX DX
	MOV			AX, NUM
	MOV			BX, STR_PTR
	MOV			DL, 1
	MOV			DH, 1
	CALL		SPUT_HEX
	MOV			STR_PTR, BX
	POP			DX BX AX
ENDM

;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
; Puts one byte number to instruction buffer.
; IN
;	NUM - Number to put. 
;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
INS_BYTE MACRO NUM
	SPUT_BYTE 	INS_END_PTR, NUM, 1
	SPUT_CHAR	INS_END_PTR, "h"
ENDM

;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
; Puts two byte number to instruction buffer.
; IN
;	NUM - Number to put. 
;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
INS_WORD MACRO NUM
	SPUT_WORD 	INS_END_PTR, NUM
	SPUT_CHAR	INS_END_PTR, "h"
ENDM

;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
; Puts a character to instruction buffer.
; IN
;	CHAR - Character to put. 
;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
INS_CHAR MACRO CHAR
	SPUT_CHAR	INS_END_PTR, CHAR
ENDM

;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
; Puts a string to instruction buffer.
; IN
;	SI - Pointer to string (terminated with "$").
;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+	
PROC INS_STR
	PUSH		AX BX SI DI

	MOV			DI, INS_END_PTR
	LEA			AX, INS_BUFFER
	CMP			AX, DI ; Check if string is a mnemonic.
	MOV			AH, 0
	JNE			@@FIND_END
	MOV			AH, 1
	
	@@FIND_END:
		CMP			BYTE PTR [SI], "$"
		JE			@@ADD_SPACING
		MOV			AL, [SI]
		MOV			[DI], AL
		INC 		SI
		INC			DI
		JMP			@@FIND_END
	
	@@ADD_SPACING: ; Add spacing if string is a mnemonic.
	CMP			AH, 1
	JNE			@@RETURN
	LEA			BX, INS_BUFFER
	ADD			BX, 8
	
	@@ADD_SPACE:
		CMP			DI, BX
		JAE			@@RETURN
		MOV			BYTE PTR [DI], " "
		INC			DI
		JMP			@@ADD_SPACE
	
	@@RETURN:
	MOV			INS_END_PTR, DI
	POP			DI SI BX AX
	RET
INS_STR ENDP

;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
; Writes a hexadecimal number to a string.
; IN
;	AX - Number to write.
;	BX - Pointer to string to write.
;	DL - Set to 1 if number is 2 byte size, otherwise 1 byte size.
;	DH - Set to 1 to add leading zero when number starts with a letter.
; OUT
;	BX - Pointer to upcoming character.
;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+		
PROC SPUT_HEX
	PUSH		SI AX CX DX

	MOV 		CX, 10h
	CMP			DL, 1
	JE			@@WORD_NUM	
	MOV			SI, 2
	JMP			@@PREP_DIVIDE
	@@WORD_NUM:
	MOV			SI, 4
	
	@@PREP_DIVIDE:
	PUSH		SI
	
	@@DIVIDE:
		DEC			SI
		MOV			DX, 0
		DIV			CX
		CMP			DL, 9
		JBE			@@HEX_DIGIT
		
		@@HEX_LETTER:
		SUB			DL, 10
		ADD			DL, "A"
		JMP			@@ADD_TO_BUFFER
		
		@@HEX_DIGIT:
		ADD 		DL, "0"
		
		@@ADD_TO_BUFFER:
		MOV			[BX + SI], DL
		CMP			AX, 0
		JNE 		@@DIVIDE
		
	CMP			SI, 0
	JE			@@CHECK_SHIFT
	@@ADD_LEADING_ZEROS:
		DEC			SI
		MOV			BYTE PTR [BX + SI], "0"
		CMP			SI, 0
		JNE			@@ADD_LEADING_ZEROS
	
	@@CHECK_SHIFT:
	POP			SI DX
    MOV         CX, SI ; Number of characters written.
	CMP			DH, 1
	JNE			@@RETURN
	CMP			BYTE PTR [BX], "A"
	JB			@@RETURN
	@@SHIFT_HEX:
		DEC			SI
		MOV			AL, [BX + SI]
		MOV			[BX + SI + 1], AL
		CMP			SI, 0
		JNE			@@SHIFT_HEX
	MOV			BYTE PTR [BX], "0"
	INC			CX
	
	@@RETURN:
    ADD         BX, CX
	POP			CX AX SI
	RET
SPUT_HEX ENDP

;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
; Gets the upcoming argument, separated by a space, from command line.
; IN
;   BX - Pointer to string to write argument into.
;	SI - Pointer to current command line character.
; OUT
;   SI - Pointer to upcoming command line character.                  
;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
PROC GET_CMD_ARG
    PUSH    	AX DI
  
    MOV 		DI, 0
    @@READ_SPACES: ; Read leading spaces.
        MOV 		AL, ES:[SI]
        CMP 		AL, 13 ; Return if new line (no input found).
        JE  		@@RETURN
        CMP 		AL, " "
        JNE 		@@READ_ARG
        INC 		SI
        JMP 		@@READ_SPACES
    
    @@READ_ARG:
        MOV 		AL, ES:[SI]
        CMP 		AL, " "
        JE  		@@RETURN
        CMP 		AL, 13 ; Return if new line.
        JE  		@@RETURN 
        MOV 		[BX + DI], AL
        INC 		SI
        INC 		DI
        JMP 		@@READ_ARG
  
    @@RETURN:
    POP 		DI AX
    RET
GET_CMD_ARG ENDP

;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
; Adds an extension if a string does not have it already.
; IN
;   BX - Pointer to string (terminated with 0).
;	DX - Pointer to extension (uppercase and terminated with 0).            
;-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
PROC ADD_EXTENSION
	PUSH		AX BX CX DX SI DI

	MOV			DI, BX
	MOV			SI, DX
	MOV			CX, 0
	@@FIND_EXT_END:
		CMP			BYTE PTR [SI], 0
		JE			@@FIND_STR_END
		INC			SI
		INC			CL
		JMP			@@FIND_EXT_END

	@@FIND_STR_END:
		CMP			BYTE PTR [DI], 0
		JE			@@VALIDATE_LENGTH
		INC			DI
		INC			CH
		JMP			@@FIND_STR_END
	
	@@VALIDATE_LENGTH:
	PUSH		DI
	CMP			CH, CL
	JBE			@@NO_EXT
	MOV			CH, 0
	DEC			SI
	DEC			DI

	@@CHECK_EXT:
		MOV			AL, [SI]
		CMP			AL, [DI]
		JE			@@CONTINUE
		ADD			AL, 20h 
		CMP			AL, [DI]
		JNE			@@NO_EXT
		@@CONTINUE:
		DEC			SI
		DEC			DI
		LOOP		@@CHECK_EXT
	POP			DI
	JMP			@@RETURN

	@@NO_EXT:
	POP			DI
	MOV			SI, DX
	@@ADD_EXT:
		MOV			AL, [SI]
		MOV			[DI], AL
		INC			SI
		INC 		DI
		CMP			BYTE PTR [SI], 0
		JE			@@RETURN
		JMP			@@ADD_EXT

	@@RETURN:
	POP			DI SI DX CX BX AX
	RET
ADD_EXTENSION ENDP