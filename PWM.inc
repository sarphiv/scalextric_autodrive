;PWM module
;Subroutines for PWM
    
PWM_INIT
    BSF	    STATUS, RP0		;Change to bank 1
    MOVLW   d'255'		;PWM period
    MOVWF   PR2			;
     
    BCF	    STATUS, RP0		;Change to bank 0

    MOVLW   d'0'		;PWM timer - Time high in a period
    MOVWF   CCPR1L		;Default value = 0
    
    MOVLW   b'00001100'		;Enable PWM on PORTC5
    MOVWF   CCP1CON		;
    
    BCF	    PIR1, TMR2IF	;Clear TIMER2 interrupt flag
    
    MOVLW   b'00000111'		;Enable TIMER2 and set prescaler to 16
    MOVWF   T2CON		;
    
    
    BCF	    STATUS, RP0		;Change to bank 0
PWM_WAIT
    BTFSS   PIR1, TMR2IF	;Wait for TIMER2 to overflow
    GOTO    PWM_WAIT
    
    
    BSF	    STATUS, RP0		;Change to bank 1
    MOVLW   b'11011111'		;Set C5 (PWM pin) to output
    ANDWF   TRISC, F
    
    RETURN
   
PWM_SET
    BCF	    STATUS, RP0		;Change to bank 0

    MOVWF   CCPR1L		;Load new PWM on time to CCPR1L

    RETURN