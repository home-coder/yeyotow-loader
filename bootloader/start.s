.global _start
.align 2
_start:
	b reset
reset:
	mov sp, #0x2000

	mov r4, #0x70000000
	orr r4, r4, #0x13@256M
	mcr p15, 0, r4, c15, c2, 4@c15=r4

	@WTCON = 0
	ldr r4, =0x7e004000
	mov r5, #0
	str r5, [r4]

        @通过协处理器告诉arm中断产生后按照实现去跳转
        mrc p15, 0, r4, c1, c0, 0
        orr r4, r4, #(1 << 24)
        mcr p15, 0, r4, c1, c0, 0

        mrs r4, cpsr
        bic r4, r4, #0x1f
        orr r4, r4, #0x12
        msr cpsr, r4

        mov sp, #0x57000000

        mrs r4, cpsr
        bic r4, r4, #0x1f
        orr r4, r4, #0x13
        msr cpsr, r4

    bl armboot

	b .
