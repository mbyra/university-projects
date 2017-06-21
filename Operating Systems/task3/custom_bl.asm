org 0x7c00                               ; informacja o początkowym adresie programu ('Binary File Program Origin')

WELCOME_MSG: db 'Enter your name:', 0xd, 0xa, 0x0  ; napis kończy się znakiem nowej linii (0xd, 0xa) i nullem (0x0)
BUFFER times 64 db 0                     ; inicjacja 64-bajtowego bufora

jmp 0:start         ; wyzerowanie rejestru cs

; w rejestrze al funkcja spodziewa się otrzymać argument - adres pierwszego znaku do wypisania
print:
    mov al, byte [bx]
    cmp al, 0x0     ; sprawdzam czy to juz nie koniec napisu
    je loop_end     ; jesli trafilem na znak konca '\0' to koncze wypisywanie
    mov ah, 0xe     ; w ah rodzaj rozkazu maszynowego, w al jest znak do wypisania
    int 0x10        ; i wywoluje rozkaz maszynowy 10 korzystajacy z rejestrow ah i al jako argumentow
    inc bx
    jmp print

    loop_end:       
    ret

start:
    mov ax, cs      ; wyzerowanie pozostałych rejestrów segmentowych
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x8000  ; inicjacja stosu

    mov bx, WELCOME_MSG
    call print

    times 510 - ($ - $$) db 0
    dw 0xaa55