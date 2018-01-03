;Motor control routines
    
MOTOR_INIT
    CALL    PWM_INIT
    CALL    HallEffect_INIT
    
    RETURN

MOTOR_UPDATE
    BCF	    STATUS, RP0		;Bank 0
    BTFSS   RSTAT, RUN		;Return if not running
    RETURN
    
    BTFSC   RSTAT, MODE		;Figure out update type
    GOTO    MOTOR_UPDATE_RECON
    ;else
    GOTO    MOTOR_UPDATE_RACE
    
    RETURN			;This line should never be reached
    
MOTOR_UPDATE_RECON
    MOVLW   SPEED_RECON_NORMAL	;Set constant speed
    CALL    PWM_SET
    
    BCF	    STATUS, RP0		;Bank 0
    BTFSC   PORTC, 2		;Check if turning
    GOTO    EE_SET		;Write 1 to EEProm
    ;else
    GOTO    EE_CLR		;Write 0 to EEProm
    
    RETURN			;This line should never be reached
    
MOTOR_UPDATE_RACE
    CALL    EE_READ_BIT		;Load data bit
    
    BCF	    STATUS, RP0		;Bank 0
    
    BCF	    STATUS, 2		;Potentially remove this safety line
    
    ANDLW   h'01'		;Figure out if turning or not
    
    BTFSS   STATUS, 2		;Turn on brake light
    BSF	    PORTC, 3
    BTFSC   STATUS, 2		;Turn off brake light
    BCF	    PORTC, 3
    
    BTFSS   STATUS, 2		;Turn on debug light
    BSF	    PORTA, 5
    BTFSC   STATUS, 2		;Turn off debug light
    BCF	    PORTA, 5
    
    BTFSS   STATUS, 2		;Load turning speed
    MOVLW   SPEED_RACE_TURN
    BTFSC   STATUS, 2		;Load straight speed
    MOVLW   SPEED_RACE_STRAIGHT
    
    CALL    PWM_SET
    
    RETURN
    
MOTOR_RESTART
    CALL    MOTOR_STOP
    CALL    MOTOR_START
    
    RETURN
    
MOTOR_START
    BCF	    STATUS, RP0		;Bank 0
    BSF	    RSTAT, RUN		;Set run state
    
    GOTO    MOTOR_UPDATE	
    
    RETURN
    
MOTOR_STOP
    BCF	    STATUS, RP0		;Bank 0
    BCF	    RSTAT, RUN		;Clear run state
    
    MOVLW   d'0'		;Set PWM time-on to 0
    CALL    PWM_SET
    
    CALL    EE_INIT		;Reset EEProm state
    
    RETURN