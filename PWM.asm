; PWM module
; Subroutines for PWM



PWM_ARG	EQU		0x20
EE_READ	EQU		0x21



PWM_INIT
		BCF 	STATUS, RP0		;Change to bank 0

		MOVLW	b'00001100'		;Enable PWM
		MOVWF	CCP1CON			;on PORTC5

		MOVLW	d'0'			;PWM timer - Time on in a period
		MOVWF	CCPR1L			;

		MOVLW	b'00000111'		;Enable TIMER2 and set prescaler to 16
		MOVWF	T2CON			;

		BSF	 	STATUS, RP0		;Change to bank 1

		MOVLW	d'255'			;PWM period
		MOVWF	PR2				;

		RETURN

PWM_SET
		BCF 	STATUS, RP0		;Change to bank 0

		MOVLW	PWM_ARG			;Load function argument
		MOVWF	CCPR1L			;to CCPR1L

		RETURN