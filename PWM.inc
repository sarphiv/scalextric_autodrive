;PWM module
;Subroutines for PWM
    
PWM_INIT
    BCF	    STATUS, RP0		;Change to bank 0

    MOVLW   b'00001100'		;Enable PWM on PORTC5
    MOVWF   CCP1CON		;

    MOVLW   d'0'		;PWM timer - Time high in a period
    MOVWF   CCPR1L		;Default value = 0

    MOVLW   b'00000111'		;Enable TIMER2 and set prescaler to 16
    MOVWF   T2CON		;

    BSF	    STATUS, RP0		;Change to bank 1

    MOVLW   d'255'		;PWM period
    MOVWF   PR2			;
    
    MOVLW   b'11011111'		;Set C5 (PWM pin) to output
    ANDWF   TRISC, F

    RETURN
   
PWM_SET
    BCF	    STATUS, RP0		;Change to bank 0

    MOVWF   CCPR1L		;Load new PWM on time to CCPR1L

    RETURN