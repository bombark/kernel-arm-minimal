
/*==  HEADER =================================================================*/

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#define false 0
#define true  1


char GLOBAL='a';

/*----------------------------------------------------------------------------*/



/*==  UTILS  =================================================================*/

// copied from https://www.geeksforgeeks.org/implement-itoa/
char* itoa(int num, char* str, int base) {
	char buf[64];
	int i = 0;
	int isNegative = false;

	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}
	if (num < 0 && base == 10) {
		isNegative = true;
		num = -num;
	}
	while (num != 0) {
		int rem = num % base;
		buf[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
		num = num/base;
	}
	if (isNegative)
		buf[i++] = '-';
	buf[i] = '\0'; // Append string terminator

	int di = 0;
	for (; i>0; i--){
		str[di++] = buf[i-1];
	}
	str[di] = '\0';
	return str;
}

/*----------------------------------------------------------------------------*/



/*==  PRINTF  ================================================================*/

volatile unsigned int* const UART0DR = (unsigned int *)0x101f1000;

void sys_putchar(char c) {
	*UART0DR = (unsigned int) c;
}

void sys_puts(const char* str){
	while (1) {
		char c = *str++;
		if ( c == '\0' )
			break;
		sys_putchar(c);
	}
}

void sys_printf(const char* format, ...) {
	va_list argp;
	va_start(argp, format);
	while (1) {
		char c = *format;
		if ( c == '\0' )
			break;
		if (*format == '%') {
			format++;
			c = *format;
			if ( c == '\0' ){
				break;
			} else if ( c == '%') {
				sys_putchar('%');
			} else if ( c == 'c') {
				char p1 = va_arg(argp, int);
				sys_putchar(p1);
			} else if ( c == 'd') {
				char tmp[64];
				int p1 = va_arg(argp, int);
				itoa(p1, tmp, 16);
				sys_puts(tmp);
			}
		} else {
			sys_putchar(*format);
		}
		format++;
	}
	va_end(argp);
}


/*----------------------------------------------------------------------------*/



/*==  INTERRUPTIONS  =========================================================*/
/*
based:
https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu/
http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/ka3540.html
http://www.state-machine.com/doc/Building_bare-metal_ARM_with_GNU.pdf
https://static.docs.arm.com/ddi0198/e/DDI0198E_arm926ejs_r0p5_trm.pdf
*/


typedef struct {
	int    mode;
	void** cpu_table;
} InterruptSys;


void InterruptSys_boot(InterruptSys* self){
	self->cpu_table = 0;
}

void InterruptSys_addCallback(InterruptSys* self, int id, void(*_cb)(void)){
	self->cpu_table[id] = (void*) _cb;
}

void InterruptSys_updateMode(InterruptSys* self){
	int mode = 0;
	asm("STMDB SP!,  {R1}");         /* save work register */
	asm("MRS   R1,   CPSR");         /* get current program status */
	asm("STR   R1, [fp, #-8]");
	//asm("STR   R1, [%0]" : "=r"(mode));
	asm("LDMIA SP!,  {R1}");         /* restore work register */
	sys_printf("mode %d\n",mode);
	self->mode = mode;
}

void InterruptSys_enable(InterruptSys* self){
	asm("STMDB SP!,  {R1}");         /* save work register */
	asm("MRS   R1,   CPSR");         /* get current program status */
	asm("BIC   R1,   R1, #0x80");    /* clear IRQ disable bit flag */
	asm("MSR   CPSR, R1");           /* store updated program status */
	asm("LDMIA SP!,  {R1}");         /* restore work register */
	InterruptSys_updateMode(self);
}

void InterruptSys_disable(InterruptSys* self){
	asm("STMDB SP!,  {R1}");         /* save work register */
	asm("MRS   R1,   CPSR");         /* get current program status */
	asm("ORR   R1,   R1, #0x80");    /* OR with new value (variable NewState is in R0) */
	asm("MSR   CPSR, R1");           /* store updated program status */
	asm("LDMIA SP!,  {R1}");         /* restore work register */
	InterruptSys_updateMode(self);
}

/*----------------------------------------------------------------------------*/



/*==  SYSTEM  ================================================================*/

void callback_irq0(void) {
	GLOBAL += 1;
}

void boot(){
	static InterruptSys interrupt_sys;
	InterruptSys_boot(&interrupt_sys);
	InterruptSys_addCallback(&interrupt_sys, 0, callback_irq0);
	InterruptSys_enable(&interrupt_sys);
}

void shutdown(){

}

void main() {
	boot();
	for (int i=0; i<100; i++){
		sys_printf("Hello world %d\n", GLOBAL);
	}
	sys_puts("END!!!!\n");
	shutdown();
}

/*----------------------------------------------------------------------------*/
