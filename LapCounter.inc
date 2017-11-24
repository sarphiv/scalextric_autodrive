;Keeps track of laps
    
TotalLaps   EQU 4		;Total number of laps minus 1
				;Because start is included as a lap
    
LAP_INIT
    Laps    RES	    1		;Initialize lap counter
    MOVLW   h'00'
    MOVWF   Laps
    
    BCF	    RSTAT, RUN		;Set state to stopped
    
    RETURN

LAP_UPDATE
    BTFSS   RSTAT, MODE
    GOTO    LAP_UPDATE_RACE
    ;else
    GOTO    LAP_UPDATE_RECON
    

    RETURN			;This line should never be reached

LAP_UPDATE_RECON
    INCF    Laps, F
    SUBLW   h'02'
    
    BTFSS   STATUS, 2
    BSF	    RSTAT, RUN
    BTFSC   STATUS, 2
    BCF	    RSTAT, RUN
    
    RETURN
    
LAP_UPDATE_RACE
    INCF    Laps, F		;Increment laps by one
    
    SUBLW   TotalLaps		;Calculate if Laps = TotalLaps
    BTFSS   STATUS, 2		;Update RUN bit in RSTAT
    BSF	    RSTAT, RUN
    BTFSC   STATUS, 2
    BCF	    RSTAT, RUN
    
    RETURN