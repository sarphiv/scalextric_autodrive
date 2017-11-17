EE_INIT
		EE_ADR	RES	1		; Assign var to EE_ADR
		EE_DATA	RES	1		; Assign var to EE_DATA
		RETURN

EE_READ
		BSF 	STATUS, RP0		; Select bank 1
		MOVF	EE_ADR, W		; Move EE_ADR to W
		MOVWF	EEADR			; Adress to read
		BSF		EECON1, RD		; EEPROM read
		MOVF	EEDAT, W		; Move EEDAT to W
		RETURN

EE_WRITE
		MOVWF	EEDAT			; Move W to EEDAT
		MOVF	EE_ADR, W		; Move EE_ADR to W
		MOVWF	EEADR			; Move W to EEADR

		BSF		STATUS, RP0		; Select bank 1
		BSF		EECON, WREN		; Write enable
		BCF		INTCON, GIE		; Disable interupts

		; Required magic method
		MOVLW	0x55h			; Unlock write
		MOVWF	EECON2			;
		MOVLW	0xAAh			; Confirm write
		MOVWF	EECON2			;

		BSF		EECON1, WR		; Start write

		INCF	EE_ADR, F		; Icrement pointer

		BSF		INTCON, GIE		; Enable interupts

		RETURN