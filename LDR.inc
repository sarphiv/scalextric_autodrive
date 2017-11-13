;Light dependent resistor module
    
LDR_INIT
    BCF	    STATUS, RP0		;Change to bank 0
    MOVLW   b'00001101'		;Enable comparator 2, disable comparator 1. Set up comparator 2.
    MOVWF   CMCON0
    
    BSF	    STATUS, RP0		;Change to bank 1
    BSF	    INTCON, PEIE	;Enable peripheral interupts (for comparators)
    BSF	    PIE1, C2IE		;Enable comparator 2 interrupts
    
    MOVLW   b'00110000'		;Make C0 and C1 (comparator pins) analog.
    IORWF   ANSEL, F		
    MOVLW   b'00000011'		;Set C0 and C1 (comparator pins) to input. 
    IORWF   TRISC, F
    MOVLW   b'11101111'		;Set C4 (comparator output) to output
    ANDWF   TRISC, F
    
    RETURN

LDR_TRIG
    BCF	    STATUS, RP0		;Change to bank 0
    BTFSS   CMCON0, C2OUT	;Check state of comparator
    GOTO    LDR_TRIG_END	;Jump to end if not on rising edge of light because interrupt fires on any change

    ;Implement reset code for car position/state

LDR_TRIG_END
    BCF		STATUS, RP0	;Change to bank 0
    BCF		PIR1, C2IF	;Clear comparator 2 interrupt flag

    RETURN