extern malloc
extern produce
extern consume
extern proberen
extern verhogen

section .text
	global init
    global producer
    global consumer

; FUNKCJA INIT
init:
    sub rsp, 8
    cmp rdi, 2147483647             ; N (edi) > 2^{31} - 1
    jg toobig
    
    cmp edi, 0                      ; N (edi) = 0
    je zero

    mov [size], edi                 ; zapisuje rozmiar bufora
	mov [wolne], edi                ; w edi jest size_t N, na tyle mam zainicjalizowac semafor wolne
    mov dword [zajete], 0           ; z kolei semafor zajete na 0
    imul rdi, 8
    call malloc                     ; liczba bajtow do malloca powinna byc w rdi, juz jest
	
    cmp rax, 0                      ; gdy alokacja sie nie uda, malloc umieszcza 0 w rax
    je not_allocated
    
    mov [index_producer], rax       ; po malloc w rax jest wskaznik na poczatek zarezerwowanej tablicy
    mov [index_consumer], rax
    mov [poczatek], rax             ; ustawiam wskaznik poczatku bufora na rax
	
    mov rax, 0                      ; poprawne wykonanie, zwracam 0      
	
    add rsp, 8
    ret

toobig:
    mov rax, -1                     ; za duze N, zwracam -1
    ret
zero:
    mov rax, -2                     ; N = 0, zwracam -2
    ret
not_allocated:
    mov rax, -3                     ; blad alokacji, zwracam -3
    ret


; FUNKCJA PRODUCER
producer:
    sub rsp, 8
    mov qword [k_producer], 0       ; ustawiam "prawdziwy" indeks producenta na 0

producer_start_loop:
    mov rdi, produkt_producer       ; wskaznik na zmienna - produkt w rdi
    call produce                    ; produce w miejscu wskazywanym przez rdi umiesci produkt

    cmp rax, 0                      ; funkcja zwraca 1, gdy nie wyprodukowala danych
    je producer_return              ; wtedy producer ma sie zakonczyc

    mov rdi, wolne                  ; w rdi umieszczam wskaznik na semafor wolne
    call proberen                   ; probuje opuscic semafor wolne, czyli zmniejszam liczbe miejsc wolnych

    mov rcx, [index_producer]       ; przenosze, gdyz "podwojny" wskaznik
    mov rax, [produkt_producer]     ; przenosze, gdyz nie mozna mov z pamieci do pamieci tylko do rejestru
    mov [rcx], rax                  ; w skorcie mov [[index]], [produkt]

    mov rdi, zajete                 ; w rdi umieszczam wskaznik na semafor zajete
    call verhogen                   ; podnosze semafor zajete, czyli zwiekszam liczbe miejsc zajetych 
    
    inc qword [k_producer]          ; zwiekszam indeks k producenta

    mov rcx, [k_producer]           ; bo w cmp jeden operand musi byc rejestrem
    cmp rcx, [size]                 ; sprawdzam czy przypadkiem nie przekroczyl rozmiaru bufora
    jge producer_cycle              ; jesli tak to skacze do producer_cycle

    add qword [index_producer], 8   ; wpp index++

producer_continue:                  ; do powrotu z producer_cycle, gdzie wyzerowalem k i ustawilem [index_producer] na [poczatek]
    jmp producer_start_loop         ; skacze do poczatku petli, bo przekierowanie na wyjscie z petli jest gdzie indziej

producer_cycle:                     ; tzn k = size, trzeba wrocic na poczatek bufora
    mov qword [k_producer], 0       ; ustawiam k na 0

    mov rcx, index_producer         
    mov rax, [poczatek]
    mov [rcx], rax                  ; w skorcie mov [[index_producer]], [poczatek] - semantyka mov tego wymaga

    jmp producer_continue

producer_return:
    add rsp, 8
    ret

; FUNKCJA CONSUMER
consumer:
    sub rsp, 8
    mov qword [k_consumer], 0

consumer_start_loop:
    mov rdi, zajete                 ; w rdi umieszczam wskaznik na semafor zajete
    call proberen                   ; probuje przejsc przez semafor zajete

    mov rax, [index_consumer]       ; w rax zapisuje produkt z bufora
    mov rcx, [rax]                  ; i przenosze go do zmiennej produkt_consumer
    mov [produkt_consumer], rcx

    mov rdi, wolne                  ; w rdi umieszczam wskaznik na semafor wolne
    call verhogen                   ; i podnosze semafor wolne

    inc qword [k_consumer]          ; zwiekszam prawdziwy indeks
    mov rcx, [k_consumer]           ; bo w cmp jednym operandem musi byc rejestr
    cmp rcx, [size]                 ; sprawdzam czy prawdziwy indeks nie przekroczyl rozmiaru
    jge consumer_cycle              ; jesli tak to musze przeskoczyc na 0

    add qword [index_consumer], 8   ; wpp. zwiekszam wskaznik na adres kolejnej komorki w buforze

consumer_continue:                  ; do powrotu z consumer_cycle, gdzie wyzerowalem k i ustawilem [index_consumer] na [poczatek]
    
    mov rdi, [produkt_consumer]     ; umieszczam w rdi, czyli w argumencie funkcji produkt
    call consume                    ; i wolam funkcje consume

    cmp rax, 1
    je consumer_start_loop          ; jesli funkcja consume zwrocila 1, to skacze do poczatku petli

    add rsp, 8
    ret                             ; wpp zwrocila 0, wiec koncze funkcje consumer

consumer_cycle:                     ; pelny cykl, tzn. zmienna k przekroczyla rozmiar bufora
    mov qword [k_consumer], 0       ; ustawiam zmienna na 0
    
    mov rcx, index_consumer  
    mov rax, [poczatek]
    mov [rcx], rax                  ; w skorcie mov [index_consumer], [poczatek] - semantyka mov tego wymaga
    
    jmp consumer_continue


section .bss
	digitSpace: resb 100
    digitSpacePos: resb 8
    index_producer: resq 1
    index_consumer: resq 1
    zajete: resd 1
    wolne: resd 1
    produkt_producer: resq 1
    produkt_consumer: resq 1
    poczatek: resq 1
    k_producer: resq 1              ; "prawdziwy" indeks producenta
    k_consumer: resq 1              ; "prawdziwy" index konsumenta
    size: resq 1