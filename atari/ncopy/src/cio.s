	;; Call CIO

	.export _ciov
	
_ciov:	ASL			; IOCB in A <<
	ASL			; <<
	ASL			; <<
	ASL			; <<
	TAX			; CIOV expects IOCB << 4 in X
	JSR $E456		; Call CIOV
	TYA			; Transfer Y error to A
	RTS			; back to caller
