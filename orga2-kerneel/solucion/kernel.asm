; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

global start


; COMPLETAR - Agreguen declaraciones extern según vayan necesitando
extern A20_enable
extern A20_disable
extern A20_check
extern GDT_DESC
extern screen_draw_layout
extern IDT_DESC
extern pic_enable
extern pic_reset
extern idt_init
extern mmu_init_kernel_dir
extern testxd
extern mmu_init_task_dir
extern tss_init
extern GDT_IDX_TASK_INITIAL
extern tasks_screen_draw
extern sched_init
extern tasks_init

; COMPLETAR - Definan correctamente estas constantes cuando las necesiten
%define CS_RING_0_SEL (1 << 3)
%define DS_RING_0_SEL (3 << 3)  


BITS 16
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; COMPLETAR - Deshabilitar interrupciones

    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; COMPLETAR - Imprimir mensaje de bienvenida - MODO REAL
    ; (revisar las funciones definidas en print.mac y los mensajes se encuentran en la
    ; sección de datos)

    print_text_rm start_rm_msg,start_rm_len,0x1,0,0

    ; COMPLETAR - Habilitar A20
    ; (revisar las funciones definidas en a20.asm)
    call A20_disable
    call A20_check
    call A20_enable
    call A20_check

    ; COMPLETAR - Cargar la GDT
    lgdt [GDT_DESC]

    ; COMPLETAR - Setear el bit PE del registro CR0

    mov eax, CR0
    or eax, 1
    mov CR0, eax

    ; COMPLETAR - Saltar a modo protegido (far jump)
    ; (recuerden que un far jmp se especifica como jmp CS_selector:address)
    ; Pueden usar la constante CS_RING_0_SEL definida en este archivo
    xor eax, eax
    jmp CS_RING_0_SEL:modo_protegido

BITS 32
modo_protegido:
    ; COMPLETAR - A partir de aca, todo el codigo se va a ejectutar en modo protegido
    ; Establecer selectores de segmentos DS, ES, GS, FS y SS en el segmento de datos de nivel 0
    ; Pueden usar la constante DS_RING_0_SEL definida en este archivo
    mov ax, DS_RING_0_SEL

    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    
    ; COMPLETAR - Establecer el tope y la base de la pila

    mov esp,  0x00025000
    mov ebp, esp

    ; COMPLETAR - Imprimir mensaje de bienvenida - MODO PROTEGIDO

    print_text_pm start_pm_msg,start_pm_len,0x1,2,0
    
    ; COMPLETAR - Inicializar pantalla
    
   call screen_draw_layout

    call idt_init
    lidt [IDT_DESC]
    call pic_reset
    call pic_enable

    ;PAGINACIÓN
  
    call mmu_init_kernel_dir
 
    mov cr3, eax ;eax porque el call me da la respuesta en ese registro, que es "return KERNEL_PAGE_DIR;
    
    mov eax, CR0
    or eax, 0x80000000
    mov CR0, eax

    ;TAREAS
    ;INICIAMOS LAS DOS PRIMERAS TAREAS
    
    call tss_init

    call sched_init
    call tasks_init

    ;call pic_reset

    
    ;call tasks_screen_draw
    mov ax, GDT_IDX_TASK_INITIAL ;11          ;tarea inicial?
    shl ax, 3
    ltr ax      ; 0000000001011000b ; selector de segmento: indice =11; TI =0, osea GDT; RPL = 00
    
    ;tarea idle
    jmp 0x60:0 
    ; la GDT_IDX_TASK_IDLE << 3 = 0x0060 ; salta a la tarea idle, se carga el desc de TSS de idle en TR solo, lo ahce el sched.
 
    push 0x18000
    call mmu_init_task_dir
    pop edi
    mov CR3, eax
    xchg bx, bx

    sti
    ;xor eax, eax
    ;int 88
    ;div eax
    ; Ciclar infinitamente 
    ;int 88
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
