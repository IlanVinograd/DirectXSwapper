.data
EXTERN wrapPtr:QWORD 

.code
OPTION PROLOGUE:NONE
OPTION EPILOGUE:NONE

call_original proc
    jmp qword ptr [wrapPtr]
call_original endp

end