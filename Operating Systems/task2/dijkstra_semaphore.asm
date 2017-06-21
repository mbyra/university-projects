section .text
 	global verhogen
 	global proberen

verhogen:
	lock inc qword[rdi]
	ret

proberen:
	mov eax, 0				
	lock xchg [rdi], eax	; ustawiam semafor na 0, by miec dostep do sekcji krytycznej, zapisuje wartosc w eax
	cmp eax, 0				; sprawdzam czy ta wartosc zapisana na eax = 0
	je proberen 			; jesli jest rowna 0, tzn musze dalej czekac
	dec eax					; wpp zmniejszam wartosc semafora
	lock xadd [rdi], eax	; i ustawiam miejsce w pamieci na ta zmniejszona wartosc
	ret
