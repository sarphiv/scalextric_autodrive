;Hall effect module
    
HallEffect_INIT
    BSF	    STATUS, RP0		;Change to bank 1
    BSF	    INTCON, INTE	;Enable interupt on change on PORTA2
    
    MOVLW   b'11111011'		;Make hall effect pin digital
    ANDWF   ANSEL, F
    MOVLW   b'00000100'		;Set A2 (hall effect interrupt pin) to input
    IORWF   TRISA, F
    
    RETURN

HallEffect_TRIG
    ;Toggle interrupt to trigger on rise or fall. This allows us to capture half rotations
    BSF 	STATUS, RP0		;Change to bank 1
    MOVLW	b'01000000'		;Toggle interrupt edge select bit
    XORWF	OPTION_REG, F		;

    ;Test code
;    BCF		STATUS, RP0
;    MOVLW	b'00001000'
;    XORWF	PORTC, F
    
;    CALL	EE_SET			;
;    CALL	EE_CLR
;    CALL	EE_SET			;
;    CALL	EE_CLR
;    CALL	EE_SET			;
;    CALL	EE_CLR
;    CALL	EE_SET			;
;    CALL	EE_CLR
    
;    MOVLW	d'255'
;    CALL	EE_WRITE_BYTE
;    
;    MOVLW	d'140'
;    CALL	EE_WRITE_BYTE
;    
;    MOVLW	d'55'
;    CALL	EE_WRITE_BYTE
;    
;    MOVLW	d'200'
;    CALL	EE_WRITE_BYTE
    
    CALL    EE_READ_BYTE
    MOVF    EE_BUFF, W
    CALL    PWM_SET
    INCF    EE_ADR, F
;    BCF		STATUS, RP0	;Change to bank 0
;    BCF		PORTC, 3
;    CALL	EE_READ_BIT	; 
;    ANDLW	b'1'
;    BCF		STATUS, RP0
;    BTFSS	STATUS, Z
;    BSF		PORTC, 3

    GOTO	HallEffect_TRIG_END

HallEffect_TRIG_END
    BCF	INTCON, INTF		;Clear hall effect sensor interrupt flag

    RETURN