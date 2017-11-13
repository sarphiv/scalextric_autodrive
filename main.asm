list        p=16F684    
INCLUDE "p16f684.inc"
;errorlevel  -302    ; no "register not in bank 0" warnings 
;errorlevel  -305    ; no "page or bank selection not needed for this device" messages
; ******* COMPILER configuration bits *****************************************************
; ext reset, no code or data protect, no brownout detect,
; no watchdog, power-up timer, int clock with I/O,
; no failsafe clock monitor, two-speed start-up disabled 
 __CONFIG _FCMEN_OFF & _IESO_OFF & _BOD_OFF & _CPD_OFF &    _CP_OFF & _MCLRE_OFF & _PWRTE_ON & _WDT_OFF & _INTRC_OSC_NOCLKOUT

; ******* Vectors ***************************************************	
    ORG	    0x0000	;Processor reset vector
    goto    INIT

    ORG	    0x0004	;Interrupt service routine vector
    goto    ISR
	
; ******* MODULES *******************************************************
#INCLUDE    "PWM.inc"
#INCLUDE    "HallEffect.inc"
#INCLUDE    "LDR.inc"
#INCLUDE    "ISR.inc"
	
; ******* SETUP *****************************************************
INIT
    TEST    RES	1	;currently testing things in hall effect

    CALL    PWM_INIT
    CALL    HallEffect_INIT
    CALL    LDR_INIT

    CALL    ISR_INIT

    ;Switch pin setup (this is only temporarily left here)
    BSF	    STATUS, RP0	    ;Change to bank 1
    MOVLW   b'10111111'	    ;Change C2 (turn switch) to digital
    ANDWF   ANSEL, F

    MOVLW   b'00000100'	    ;Set C2 (turn switch) to input
    IORWF   TRISC, F			
					
    ;C5 (PWM signal), C4 (Comparator out), C3 UNUSED, C2 (Turn switch), C1 C0 (Comparator -, +)
    ;A2 (Hall effect interrupt) input, A3 (State switch) (These are both input by default)

; ******* MAIN LOOP *****************************************************	
LOOP
    ;Test program

    ;MOVLW  d'160'
    ;CALL   PWM_SET
	
    NOP

LOOPEND
    GOTO    LOOP

    END
