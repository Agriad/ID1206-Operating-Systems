bits 32
global start

section .text

start:
mov dword [0xb8000], 0x2f4b2f4f       ; print 'OK' to screen
;mov dword [0xb8000], 0x2f57
;mov dword [0xb800]
hlt