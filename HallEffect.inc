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
;    MOVLW	h'69'
;    CALL	EE_WRITE
;    BCF		STATUS, RP0	;Change to bank 0
;    MOVLW	b'00001000'	;Toggle test pin
;    XORWF	PORTC, F

;    GOTO	HallEffect_TRIG_END

HallEffect_TRIG_END
    BCF	INTCON, INTF		;Clear hall effect sensor interrupt flag

    RETURN