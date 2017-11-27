; EE_INIT skal kaldes ved lap start og map
EE_INIT
		MOVLW	h'0'
		MOVWF	EE_DATA
		
		MOVLW	h'0'
		MOVWF	EE_ADR

		MOVLW	b'0000001'		; b'1', er same, men det her er visuelt
		MOVWF	EE_MASK 
		
		RETURN

EE_READ
; Move data from EEPROM-memory to W
		BSF 	STATUS, RP0		; Select bank 1
		MOVF	EE_ADR			; 
		ADDLW	d'10'			; Forward pointer

		MOVWF	EEADR			; Address to read
		BSF		EECON1, RD		; EEPROM read
		MOVF	EEDAT, W		; Move EEDAT to W
; Check if n bit is set
		ANDWF	EE_MASK, W		; And EEDAT w/ mask
		BTFSC	STATUS, Z		; If EEDAT AND MASK is nonzero
		MOVLW	d'1'			; If 1, return 0
		BTFSS	STATUS, Z		;
		MOVLW	d'0'			; If 0, return 0
; Bitshift mask
		RLF		EE_ADR, F		; Bit shift Mask, ex. 001 -> 010
		BTFSS	EE_ADR, 8		; If last bit is set
		RLF		EE_ADR, F		; Roll again to fix "carry-skip"

		INCF	EE_ADR, F		; Icrement pointer

		RETURN
		
EE_WRITE
; *** Writes data from W to EEPROM memory at adress EE_ADR ***
; Load data from W and EE_ADR
		BSF		STATUS, RP0		; Select bank 1 
		MOVWF	EEDAT			; Move W to EEDAT
		
		BCF		STATUS, RP0		; Select bank 0 
		MOVF	EE_ADR, W		; Move EE_ADR to W

		BSF		STATUS, RP0		; Select bank 1 
		MOVWF	EEADR			; Move W to EEADR
		
; Writing procedure
		BSF		EECON1, WREN	; Write enable
		BCF		INTCON, GIE		; Disable interupts

; Required sequence
		MOVLW	h'55'			; Unlock write
		MOVWF	EECON2			;
		MOVLW	h'AA'			; Confirm write
		MOVWF	EECON2			;
		BSF		EECON1, WR		; Start write

; End of writing procedure

		BSF		INTCON, GIE		; Enable interupts
		INCF	EE_ADR, 1		; Increment pointer

		RETURN