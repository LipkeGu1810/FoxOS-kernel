[bits 64]
[extern _Z5patchPcm]
[global memset_fast1]
[global memcpy_fast1]
[global memset_fast2]
[global memcpy_fast2]
[global init_fast_mem]

memset_fast1:
	mov r9, rdi
	mov al, sil
	mov rcx, rdx
	rep stosb
	mov rax, r9
	ret

memcpy_fast1:
	mov rax, rdi
	mov rcx, rdx
	rep movsb
	ret

memcpy_fast2:
	mov rax, rdi
	mov rcx, rdx
	shr rcx, 3
	and edx, 7
	rep movsq
	mov ecx, edx
	rep movsb
	ret

memset_fast2:
	mov r9, rdi
	mov rcx, rdx
	shr rcx, 3
	and edx, 7
	movsx esi, sil
	mov rax, 0x0101010101010101
	imul rax, rsi
	rep stosq
	mov ecx, edx
	rep stosb
	mov rax, r9
	ret


init_fast_mem:
	mov rdi, .memcpy_str
	mov rsi, memcpy_fast2
	call _Z5patchPcm

	mov rdi, .memset_str
	mov rsi, memset_fast2
	call _Z5patchPcm

	ret

.memset_str: db "_Z6memsetPvhm", 0
.memcpy_str: db "_Z6memcpyPvPKvm", 0