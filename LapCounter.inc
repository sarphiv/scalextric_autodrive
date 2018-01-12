;Keeps track of laps
    
LAP_INIT
    CALL    LDR_INIT
    
    BCF	    STATUS, RP0		;Change to bank 0
    MOVLW   h'00'
    MOVWF   LAP_Laps
    
    BCF	    RSTAT, RUN		;Set state to stopped
    
    RETURN

LAP_UPDATE
    BCF	    STATUS, RP0		;Change to bank 0
    
;    MOVLW   b'00100000'		;Toggle debug light on new lap
;    XORWF   PORTA, F
    
    INCF    LAP_Laps, F		;Increment laps by one
    
    BTFSS   RSTAT, MODE		;Figure out update type
    GOTO    LAP_UPDATE_RACE
    ;else   
    GOTO    LAP_UPDATE_RECON
    

    RETURN			;This line should never be reached

LAP_UPDATE_RECON
    BCF	    STATUS, RP0		;Change to bank 0   
    
    BCF	    STATUS, 0		;Potentially remove this safety line
    MOVF    LAP_Laps, W		;Calculate if Laps < 3 (two runs on the track)
    SUBLW   h'02'		;
    
    BTFSC   STATUS, 0		;If under 3 laps (recon mode)
    GOTO    MOTOR_START		;Make sure motor starts
    ;else
    GOTO    LAP_UPDATE_SWITCH   ;Switch to race mode and start race round
    
    RETURN
    
LAP_UPDATE_SWITCH
    BCF	    RSTAT, MODE		;Set to race mode
    
LAP_UPDATE_RACE
    BCF	    STATUS, RP0		;Change to bank 0
    
    BCF	    STATUS, 0		;Potentially remove this safety line
    MOVF    LAP_Laps, W		;Calculate if Laps <= TotalLaps
    SUBLW   TOTALLAPS		;
    
    BTFSC   STATUS, 0		;If under or equal TOTALLAPS
    GOTO    MOTOR_RESTART	;STOP STOP, this line might be wrong
    ;else
    GOTO    MOTOR_STOP		;Stop motor and reset EEPROM
    
    RETURN			