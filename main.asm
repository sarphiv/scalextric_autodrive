; ****** forbindelse af ICSP *********************  forbindelse af kredsløb ***************
;	Programmet forventes uploadet med en PICkit2 
;	eller en PICkit3 pilen på PICkit angiver pin1 
;	Navn		PICkit	PIC16F684					Funktion		PIC16F684
;	VPP/MCLR	1		4										
;	VDD/TARGET	2		1							LED				PORTC,5	
;	VSS/GND		3		14							SWITCH			PORTA,0		
;	ICSPDAT		4		13										
;	ICSPCLK		5		12
; ******* PROCESSOR DEFINITIONER *******************************************************
    list        p=16F684    
 	INCLUDE "p16f684.inc"
    ;errorlevel  -302    ; no "register not in bank 0" warnings 
    ;errorlevel  -305    ; no "page or bank selection not needed for this device" messages
; ******* COMPILER configuration bits *****************************************************
                ; ext reset, no code or data protect, no brownout detect,
                ; no watchdog, power-up timer, int clock with I/O,
                ; no failsafe clock monitor, two-speed start-up disabled 
 __CONFIG _FCMEN_OFF & _IESO_OFF & _BOD_OFF & _CPD_OFF &    _CP_OFF & _MCLRE_OFF & _PWRTE_ON & _WDT_OFF & _INTRC_OSC_NOCLKOUT
; ******* OPSÆTNING AF VARIABLE ***********************************************************
; Skriv variable og konstanter ind her

; ******* OPSÆTNING AF PROGRAM POINTERE ***************************************************
	ORG 	0x0000          ; processor reset vector
	goto	INIT

	ORG		0x0004
	goto	ISR
; ******* SETUP *****************************************************
INIT
	;Comparator setup
	MOVLW	b'00001101'		;Enable comparator 2, disable comparator 1. Set up comparator 2.
	MOVWF	CMCON0
	
	;PWM setup
	MOVLW	b'00001100'		;Enable PWM on PORTC5
	MOVWF	CCP1CON
	
	MOVLW	d'0'			;PWM timer - Time on in a period
	MOVWF	CCPR1L

	MOVLW	b'00000111'		;Enable TIMER2 and set prescaler to 16
	MOVWF	T2CON

	BSF 	STATUS, RP0		;Change to bank 1

	MOVLW	d'255'			;PWM timer - Period time
	MOVWF	PR2
	
	;Interupt setup
	BSF		INTCON, 4		;Enable interupt on change on PORTA2
	
	BSF		INTCON, 6		;Enable peripheral interupts (for comparators)
	BSF		PIE1, 4

	BSF		INTCON, 7		;Enable global interrupt switch

	;Pin setup
	MOVLW	b'00110000'		;Change all ports to digital
							;Comparators are exempt kept analog
	MOVWF	ANSEL
	
	MOVLW	b'11000111'		;Set up input/output pins. 
							;C5 (motor), C4 (Comparator Out), C3 (LED), C2 (Turn switch), C1 C0 (Comparator -, +)
							;A2/INT (hall) input (A2 is input by default)
	MOVWF	TRISC			

	BCF 	STATUS, RP0		;Change to bank 0

; ******* LOOP *****************************************************	
LOOP
	;Test program

	MOVLW	d'160'
	MOVWF	CCPR1L


LOOPEND

  	goto 	LOOP

; ******* SUBROUTINES *****************************************************
ISR
	BTFSC	INTCON, 1		;Check if triggered by hall effect sensor
	CALL	HALLTRIG
	BTFSC	PIR1, 1
	CALL	COMPARATORTRIG

	RETFIE
	
HALLTRIG
	;Toggle interrupt to trigger on rise or fall. This allows us to capture half rotations
	BSF 	STATUS, RP0		;Change to bank 1
	MOVLW	b'01000000'		;Toggle interrupt edge select bit
	XORWF	OPTION_REG, F		;
	BCF		STATUS, RP0		;Change to bank 0
	
	;MOVLW	b'00010000'		;Change LED output
	;XORWF	PORTC, F
	
	
	BCF		INTCON, 1	;Clear hall effect sensor interrupt flag

	RETURN

COMPARATORTRIG
	;insert drift reset code for eeprom
	


	BCF		STATUS, RP0	;Change to bank 0
	BCF		PIR1, 4		;Clear comparator 2 interrupt flag

	RETURN
	
; ******* END *******************************************************************
 	end
