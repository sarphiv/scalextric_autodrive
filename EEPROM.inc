; EE_INIT skal kaldes ved lap start og map
EE_INIT
		BCF	STATUS, RP0
		CLRF	EE_DATA
		CLRF	EE_ADR
		CALL	EE_READ_BYTE
		MOVWF	EE_BUFF
		
		CLRF	EE_ADR
		
		MOVLW	d'1'			; Initialize mask
		MOVWF	EE_MASK 
		
		RETURN
	
EE_READ_BYTE
; Move data from EEPROM-memory to EE_BUFF
		BCF 	STATUS, RP0		; Select bank 0
		MOVF	EE_ADR, W		; 
		
		BSF	STATUS, RP0		; Select bank 1
		MOVWF	EEADR			; Address to read
		
		BSF	EECON1, RD		; EEPROM read
		MOVF	EEDAT, W		; Move EEDAT to W
		
		BCF	STATUS, RP0
		MOVWF	EE_BUFF
		
		INCF	EE_ADR, F		; Increment memory adress pointer
		
		RETURN
		
EE_WRITE_BYTE
; *** Writes data from W to EEPROM memory at adress EE_ADR ***
; Load data from W and EE_ADR
		BSF	STATUS, RP0		; Select bank 1 
		BCF	EECON1, RD
		MOVWF	EEDAT			; Move W to EEDAT
		
		BCF	STATUS, RP0		; Select bank 0 
		MOVF	EE_ADR, W		; Move EE_ADR to W
		BSF	STATUS, RP0		; Select bank 1 
		MOVWF	EEADR			; Move W to EEADR
		
; Writing procedure
		BSF	EECON1, WREN		; Write enable
		;BCF	INTCON, GIE		; Disable interupts

; Required sequence
		MOVLW	h'55'			; Unlock write
		MOVWF	EECON2			;
		MOVLW	h'AA'			; Confirm write
		MOVWF	EECON2			;
		BSF	EECON1, WR		; Start write

; End of writing procedure
EE_WAIT		BTFSC	EECON1, WR
		GOTO	EE_WAIT
		BCF	EECON1,WREN
		
		;BSF	INTCON, GIE		; Enable interupts
		
		BCF	STATUS, RP0
		INCF	EE_ADR, F		; Increment pointer

		RETURN
		
EE_SET
		BCF	STATUS, RP0		; Change to bank 0
		MOVF	EE_MASK, W
		IORWF	EE_DATA, W
		GOTO	EE_WRITE_BIT
		
EE_CLR
		BCF	STATUS, RP0
		MOVLW	h'FF'
		XORWF	EE_MASK, W
		ANDWF	EE_DATA, W
		GOTO	EE_WRITE_BIT
		
EE_WRITE_BIT	
		MOVWF	EE_DATA			; Save buffer
		BTFSC	EE_MASK, 7		; If last bit is set
		GOTO	CDN
RTN
		RLF	EE_MASK, F		; Bit shift Mask, ex. 001 -> 010
		RETURN
		
CDN	
		BCF	STATUS, RP0		; Change to bannk 0
		RLF	EE_MASK, F		; Roll again to fix "carry-skip"
		RLF	EE_MASK, F		; Roll again to fix "carry-skip"
		MOVF	EE_DATA, W
		GOTO	EE_WRITE_BYTE		; Write buffer to EEPROM
		
EE_READ_BIT
; Check if n bit is set
		BCF	STATUS, RP0
		MOVF	EE_BUFF, W
		ANDWF	EE_MASK, W		; And EEDAT w/ mask
		BTFSC	STATUS, Z		; If EEDAT AND MASK is zero
		MOVLW	d'0'			; If true, return 0
		BTFSS	STATUS, Z		;
		MOVLW	d'1'			; If false, return 1
; Bitshift mask
		BTFSC	EE_MASK, 7
		GOTO	CDN1
RTN1		
		RLF	EE_MASK, F		; Bit shift Mask, ex. 001 -> 010
		RETURN

CDN1
		RLF	EE_MASK, F
		RLF	EE_MASK, F
		CALL	EE_READ_BYTE
		RETURN