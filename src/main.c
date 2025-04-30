
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
extern uint32_t SystemCoreClock;
typedef struct LEDNode {
    uint16_t pinMask;
    struct LEDNode *next;
} LEDNode;

static void Delay(volatile uint32_t cnt)
{
	//whilein içinde parametre olarak atadýðým 32bitlik count integerinin 1er 1er azaltýlarak bir delay fonksiyonu oluþturulmasý
    while (cnt--) __NOP();
}
void RCC_Config(void)
{
    // 8 000 000 -> 168 000 000

    RCC->CR &= ~(1 << 0);          // HSI kapat
    RCC->CR |=  (1 << 16);         // HSE aç
    while(!(RCC->CR & (1 << 17))); // HSE aktif olmasýný bekle
    RCC->CR |=  (1 << 19);         //saat koruma sistemi

    RCC->PLLCFGR = 0x00000000;     // PLLCFGR reset
    RCC->PLLCFGR |= (1 << 22);     // PLL Source HSE
    RCC->PLLCFGR |= (4 << 0);      // PLL M = 4
    RCC->PLLCFGR |= (168 << 6);    // PLL N = 168

    RCC->CR |= (1 << 24);          // PLL ON
    while(!(RCC->CR & (1 << 25))); // PLL aktif olmasýný bekle

    RCC->CFGR &= ~(1 << 0);        // sistem saatini sýfýrla
    RCC->CFGR |=  (1 << 1);        // System Clock is PLL

    while(!(RCC->CFGR & (1 << 1))); // Select system clock is PLL clock

}

void GPIO_Config(void)
{
	RCC->AHB1ENR |= 1<<3;

	//12th PIN OUTPUT
	GPIOD->MODER |= 1<<24;
	GPIOD->MODER &= ~(1<<25);

	//13rd PIN OUTPUT
	GPIOD->MODER |= 1<<26;
	GPIOD->MODER &= ~(1<<27);

	//14th PIN OUTPUT
	GPIOD->MODER |= 1<<28;
	GPIOD->MODER &= ~(1<<29);

	//15th PIN OUTPUT
	GPIOD->MODER |= 1<<30;
	GPIOD->MODER &= ~(1<<31);


	//Diðer Push Pull vs her þey default kalacak ve alttaki bitwise kodu GPIOD port speedini very high olarak ayarlayacak
	GPIOD->OSPEEDR |= (1<<24) | (1<<25) | (1<<26) | (1<<27) | (1<<28) | (1<<29) | (1<<30) | (1<<31);

}


int main(void)
{
	RCC_Config();
	SystemCoreClockUpdate();
	GPIO_Config();
    //Led halkasýnýn kurulduðu kýsým 12-13-14-15-12
    LEDNode led12 = { .pinMask = (1 << 12) };
    LEDNode led13 = { .pinMask = (1 << 13) };
    LEDNode led14 = { .pinMask = (1 << 14) };
    LEDNode led15 = { .pinMask = (1 << 15) };

    led12.next = &led13;
    led13.next = &led14;
    led14.next = &led15;
    led15.next = &led12;

    LEDNode *current = &led12;
  while (1)
  {


      GPIOD->ODR |=  current->pinMask;   // yak
      Delay(1600000);
      GPIOD->ODR &= ~current->pinMask;   // söndür
      Delay(1600000);
      current = current->next;           // sýradaki LED


  }
}



void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){

  return;
}

uint16_t EVAL_AUDIO_GetSampleCallBack(void){

  return -1;
}
