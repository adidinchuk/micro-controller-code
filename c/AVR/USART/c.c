// Very helpful tutorial on utilizing USI for UART http://becomingmaker.com/usi-serial-uart-attiny85/

#define CYCLES_PER_BIT      ( F_CPU / BAUDRATE )


#if (CYCLES_PER_BIT > 255)
    #define DIVISOR             8
    #define PRESCALE            2
#else
    #define DIVISOR             1
    #define PRESCALE            1
#endif

#define FULL_BIT_TICKS      ( CYCLES_PER_BIT / DIVISOR )
#define HALF_BIT_TICKS      ( FULL_BIT_TICKS / 2 )
// number of CPU cycles the interrupt vector and the beginning of the start bit and starting the USI.
#define START_DELAY         ( 65 + 42 )
#define TIMER_START_DELAY   ( START_DELAY  / DIVISOR )


DDRB &= ~(1 << DDB0);          // Set pin PB0 to input
PORTB |= 1 << PB0;             // Enable internal pull-up on pin PB0

// USICR – USI Control Register 
USICR = 0;                     // Disable USI

// GIMSK – General Interrupt Mask Register
// PCMSK – Pin Change Mask Register
GIMSK |= 1<<PCIE;               // Enable pin change interrupts
PCMSK |= 1<<PCINT0;             // Enable pin change on pin PB0

// Interupt on pin 0
ISR (PCINT0_vect)
{
  uint8_t pinbVal = PINB;
  if (!(pinbVal & 1<<PINB0))   // Trigger if DI is Low
  {
    onSerialPinChange();
  }
}

ISR (TIMER0_COMPA_vect) {
  TIMSK &= ~(1<<OCIE0A);          // Disable COMPA interrupt

  TCNT0 = 0;                      // Count up from 0
  OCR0A = FULL_BIT_TICKS;         // Shift every bit width
}

void onSerialPinChange() {
  GIMSK &= ~(1<<PCIE);            // Disable pin change interrupts
  
  // TCCR0A – Timer/Counter Control Register A
  // WGM00 - 0, WGM01 - 1, WGM02 (B) - 0
  TCCR0A = 2<<WGM00;              // CTC mode
  
  
  // TCCR0B – Timer/Counter Control Register B
  TCCR0B = CLOCKSELECT;           // Set prescaler to cpu clk or clk/8

  //GTCCR – General Timer/Counter Control Register - PSR0: Prescaler Reset Timer/Counter0
  GTCCR |= 1 << PSR0;             // Reset prescaler
  // TCNT0 – Timer/Counter Register
  TCNT0 = 0;                      // Count up from 0

  //OCR0A – Output Compare Register A
  OCR0A = HALF_BIT_TICKS - TIMER_START_DELAY;

  //TIFR – Timer/Counter Interrupt Flag Register
  //OCF0A: Output Compare Flag 0 A
  TIFR = 1 << OCF0A;              // Clear output compare interrupt flag - 

  // TIMSK – Timer/Counter Interrupt Mask Register
  // OCIE0A: Timer/Counter0 Output Compare Match A Interrupt Enable
  TIMSK |= 1<<OCIE0A;             // Enable output compare interrupt
}