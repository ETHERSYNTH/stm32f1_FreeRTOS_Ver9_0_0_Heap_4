#include <stdio.h>
#include <stdint.h> // Platform-independent integer types

// Armcc v5.06 update 7
	struct __attribute__((packed)) modbus_reg{
		uint16_t address;
		uint16_t *data;
		enum{
			COIL = 8000,
			DISCRETE,
			INPUT,
			HOLDING}type;
		uint8_t size;
	};	

struct modbus_reg modbus_reg_var;
volatile uint32_t offset;
struct modbus_reg reg;

extern void structure_learn(void);

void structure_learn(void){


	volatile uint32_t sizestr = sizeof(modbus_reg_var);
	
	// OK!!! delta between address
	offset = (char *)&reg.address - (char*)&reg;	// offset = 0  / with packed = 0
	offset = (char *)&reg.data - (char*)&reg;		// offset = 4  / with packed = 2
	offset = (char*)&reg.type - (char*)&reg;		// offset = 8  / with packed = 6
	offset = (char*)&reg.size - (char*)&reg;		// offset = 9  / with packed = 7

	/*
		w/o atribute
		Sizeof: 12
		
		Bytes:  0   1    2   3   4   5   6   7    8      9    10  11
			  [address] [ pad ]  [  data ptr  ] [type] [size] [ pad ]
		

	
		__attribute__((packed))
		Sizeof: 8
		
		Bytes:   0   1     2   3   4   5     6     7   
			   [address] [   data ptr   ] [type][size]
		 
		
		
		Optimization Level0 [-O 0] or Level3 [-O 3] no matter
	*/
	 

	int g = 4;
	int* ptr = &g;
	
	// printf("ptr = %u\n", ptr);
	// The value of a pointer varies depending on the size of the data for which it is defined
	ptr++;// before address(ptr)=6487832; after address(ptr)=6487836;
	
	// Target of pointer is enumeration of data values for which it is defined
	// OK: ptr++; ptr--; ptr+=3; ptr-=4; --ptr; ++ptr; // by blocks plus or minus. Actual for arrays (das Massiv).  
	// NOK: ptr = ptr*n; ptr /= n;
	
	short ar[10]; // Important
	short *ptr_1 = ar;
	short *p = &ar[3];
	
	//printf("ptr = %u, p = %u\n", ptr_1, p);
	
	// p += ptr; // NOK!!! Prohibit sum of some pointer 
	
	int res = p - ptr_1; // OK!!! delta between address
	
	// res = (106 - 100) / 2  =  3. (sizeof(short) = 2 ). Actual for arrays (das Massiv).
	
	int gg = 476789;
	char *ptr_2 =  (char*)&g;
	 
	for(int i = 0; i < sizeof(int); ++i)
	{
		printf("%d ", *ptr_2++); // = *(ptr_2++). Means at first read value by address and than incriment address.
		// Priority of unary operation from right to left
		// Unary operations is more priority then bitwises operations
		// int x = *ptr + 1; // That change value of "g" variable
		// int x = *(ptr + 1); // That make pointer increment and return value by incremented address
	}
	
	
}