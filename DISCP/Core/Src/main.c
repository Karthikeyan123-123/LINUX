#define RCC_APB1ENR   (*(volatile unsigned int*)0x40023840)
#define RCC_AHB1ENR   (*(volatile unsigned int*)0x40023830)

#define GPIOA_MODER   (*(volatile unsigned int*)0x40020000)
#define GPIOA_AFRL    (*(volatile unsigned int*)0x40020020)

#define USART2_SR     (*(volatile unsigned int*)0x40004400)
#define USART2_DR     (*(volatile unsigned int*)0x40004404)
#define USART2_BRR    (*(volatile unsigned int*)0x40004408)
#define USART2_CR1    (*(volatile unsigned int*)0x4000440C)

void UART2_Init()
{
    // 1. Enable clocks
    RCC_AHB1ENR |= (1<<0);   // GPIOA
    RCC_APB1ENR |= (1<<17);  // USART2

    // 2. PA2 → Alternate Function
    GPIOA_MODER &= ~(3<<4);
    GPIOA_MODER |=  (2<<4);

    // 3. AF7 for USART2
    GPIOA_AFRL |= (7<<8);

    // 4. Baud rate (Assume 16MHz → 9600)
    USART2_BRR = 0x0683;

    // 5. Enable TX + UART
    USART2_CR1 |= (1<<3); // TE
    USART2_CR1 |= (1<<13); // UE
}

void UART2_SendChar(char c)
{
    while(!(USART2_SR & (1<<7))); // TXE
    USART2_DR = c;
}

void UART2_SendString(char *str)
{
    while(*str)
    {
        UART2_SendChar(*str++);
    }
}

int main()
{
    UART2_Init();

    while(1)
    {
        UART2_SendString("Hello UART\r\n");

        for(volatile int i=0;i<500000;i++); // delay
    }
}
