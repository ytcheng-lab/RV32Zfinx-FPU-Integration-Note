static inline uint32_t read_cycle() {
	uint32_t cycle;
	asm volatile ("rdcycle %0" : "=r"(cycle));
	return cycle;
}

