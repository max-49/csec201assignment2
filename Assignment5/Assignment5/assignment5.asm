.386
.model flat, stdcall
.stack 4096

includelib libcmt.lib
includelib legacy_stdio_definitions.lib

extern printf:NEAR
extern scanf:NEAR

ExitProcess PROTO, dwExitCode:DWORD


.data
myList DWORD 4, 2, 4, 3, 5, 9, 2, 1, 5, 6, 8, -1 ; int myList[] = {4, 2, 4, 3, 5, 9, 2, 1, 5, 6, 8, -1} #DWORD is 4 bytes
msg1 BYTE "First number: %d", 0ah, 0 ; char msg1[] = "First number: %d\n"
msg2 BYTE "%d ", 0 ; char msg2[] = "%d"
msg3 BYTE 0ah, 0 ; char msg3[] = "\n"
msg4 BYTE "Second number: %d", 0ah, 0 ; char msg4[] = "Second number: %d\n"

.code
; Goal of program:
; Given a list myList, find the mean (First number) and median (Second number)
; Print the list as is before printing the mean and print the sorted list before printing the median

main PROC c
	;Code for class goes here
	
	push offset mylist
	call fun4 ; prints out all values in the list
	; void fun4(int myList[]) {
	;	index = 0
	;	while (myList[index] != -1) {
	;		printf("%d ", myList[index]);
	;		index++;
	;	}
	;	printf("\n");
	; }
	add esp, 4 ; correct for function parameters pushed earlier

	push offset mylist
	call fun1 ; returns (eax) the average of all the numbers in the list
	; int fun1(int myList[]) {
	;	int temp = 0;
	;	int index = 0;
	;	while (myList[index] != -1) {
	;		temp += myList[index];
	;		index++;
	;	}
	;	temp /= index;
	;	return temp;
	; }
	add esp, 4 ; correct for parameter offset

	push eax
	push offset msg1
	call printf ; printf("First number: %d\n", 4.9) -> will truncate the decimal -> First number: 4
	add esp, 8 ; correct for printf offset

	push offset mylist
	call fun2 ; sorts the list in numerical order
	; void fun2(int myList[]) {
	;	while (true) {
	;		int sorted = 0;
	;		int index = 1;
	;		while (myList[index] != -1) {
	;			if (myList[index] < myList[index - 1]) {
	;				int temp = myList[index - 1];
	;				myList[index - 1] = myList[index];
	;				myList[index] = temp;
	;				sorted = 1;
	;			}
	;			index++;
	;		}
	;		if (sorted != 1) {
	;			break;
	;		}
	;	}
	; }
	add esp, 4

	push offset mylist
	call fun4 ; prints all the values in the list
	add esp, 4

	push offset mylist
	call fun3 ; returns the middle element in the list
	; int fun3(int myList[]) {
	;	int index = 0;
	;	while (myList[index] != -1) {
	;		index++;
	;	}
	;	return myList[index/2];
	; }
	add esp, 4 ; parameter offset

	push eax
	push offset msg4
	call printf ; printf("Second number: %d\n", 4)  -> Second number: 4
	add esp, 4 ; parameter offset

	INVOKE ExitProcess,0 ; exit program

fun1: ; computes average of all numbers in myList
	; function prologue
	push ebp ; set ebp to beginning of function and store old value
	mov ebp, esp
	
	sub esp, 4 ; allocate space (4 bytes) for new local variable (subtraction goes DOWN to unused memory)
	mov eax, 0 ; eax = 0
	mov [ebp-4], eax ; put 0 in allocated space

	mov ecx, 0 ; ecx = 0
	mov eax,[ebp+8] ; eax = myList[0];
	
	looptop: ; sum all values in list
		mov edx, [eax + ecx * 4] ; (1) edx = myList[0] (2) edx = myList[1]
		cmp edx, -1 ; if edx = -1, break
		je endloop

		mov ebx, [ebp-4] ; (1) ebx = 0 (2) ebx = 4 (3) ebx = 6
		add ebx, edx ; (1) ebx += myList[0] (ebx = 0 + 4) (2) ebx += myList[1] (ebx = 4 + 2) (3) ebx = 10
		mov [ebp-4], ebx ; put value of ebx into allocated space
		add ecx, 1 ; index++
		jmp looptop

	endloop:
	
	mov eax, [ebp-4] ; eax = 49
	mov edx, 0 ; edx = 0
	div ecx ; eax = eax / ecx -> eax = 49 / 10 = 4.9

	; function epilogue
	mov esp, ebp
	pop ebp
	ret

fun2: ; sorts myList in numerical order (in place)
	; function prologue
	push ebp ; save old ebp for later and set ebp to beginning of function
	mov ebp, esp

	push 0 ; push 0 to stack
	mov eax,[ebp+8] ; eax = myList[0]

	
	looptop3:
		mov edx, 0 ; edx = 0
		mov [ebp-4], edx ; allocate new DWORD space below and hold 0
		mov ecx, 1 ; index = 1
		looptop2:
			
			mov ebx, ecx; ebx = index
			sub ebx, 1; ebx = index - 1
			mov edx, [eax + ecx * 4]; edx = myList[index]
			cmp edx, -1 ; if (edx == -1) break;
			je endloop2
			cmp edx, [eax + ebx * 4]; if (myList[index] >= myList[index - 1]) go to pastif
			jge pastif ; else (SWAP myList[index] AND myList[index - 1])
				push edx ; save old value of edx to stack
				mov edx, [eax + ebx * 4] ; edx = myList[index - 1]
				mov [eax + ecx * 4], edx ; myList[index] = myList[index - 1]
				pop edx ; edx = old value of edx (myList[index])
				mov [eax + ebx * 4], edx ; myList[index - 1] = myList[index]
				mov edx, 1 ; edx = 1
				mov [ebp-4], edx ; allocated space = 1
			pastif:
			add ecx, 1 ; index++
			jmp looptop2

		endloop2:



		mov edx, [ebp-4] ; edx = value of allocated space
		cmp edx, 1 ; if (edx == 1): go back to looptop3 (IT WILL NOT BE 1 WHEN LIST IS SORTED)
		je looptop3
	endloop3:

	; function epilogue
	mov esp, ebp
	pop ebp
	ret

fun3:
	; function prologue
	push ebp
	mov ebp, esp
	
	mov ecx, 0 ; index = 0
	mov eax,[ebp+8] ; eax = myList[0];
	
	looptop4:
		mov edx, [eax + ecx * 4] ; edx = myList[index];
		cmp edx, -1 ; if (myList[index] == -1) break;
		je endloop4
		add ecx, 1 ; index++
		jmp looptop4

	endloop4:
	
	mov ebx, eax ; ebx = myList[0];
	mov eax, ecx ; eax = index;
	mov edx, 0 ; edx = 0
	mov ecx, 2 ; ecx = 2
	div ecx ; eax = eax / 2 -> eax = index / 2
	mov ecx, eax ; ecx = index / 2
	mov eax, ebx ; eax = myList[0];
	mov eax, [eax + ecx * 4] ; eax = myList[index/2] -> middle element

	; function epilogue
	mov esp, ebp
	pop ebp
	ret

fun4:
	push ebp ; function prologue - push the old value of $ebp onto the stack, make ebp = the value of $esp
	mov ebp, esp
	
	mov eax,[ebp+8] ; eax -> base of array (4)
	mov ecx, 0 ; c = 0
	
	looptop6:
		; index = 0
		; while (myList[index] != -1) {
		;	printf("%d ", myList[index]);
		;	index++;
		; }
		mov edx, [eax + ecx * 4] ; (1) eax -> base of array, edx = 4 (index 0) --> ecx: index b/c 4 is size of DWORD
		cmp edx, -1 ; (1) no
		je endloop6 ; (1) no

		push eax ; idk why we are pushing all these
		push ebx
		push ecx
		push edx

		push edx
		push offset msg2
		call printf ; (1) printf("%d ", 4)
		add esp, 8 ; add to esp to correct for offset msg2 (4) and edx (4)
		
		pop edx
		pop ecx
		pop ebx
		pop eax

		add ecx, 1 ; increase index by 1
		
		jmp looptop6

	endloop6:
	
	push offset msg3
	call printf ; printf("\n")
	add esp, 4 ; correct for pushing value to printf

	; function epiloguie
	mov esp, ebp ; set esp back to the beginning of the function
	pop ebp ; pop that stored value from earlier
	ret ; go back

main endp
end