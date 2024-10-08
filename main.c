#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "hardware/adc.h"
#include "i2c-display-lib.h"

void blink_led(int num_times){
    int blink = 0;
    while(blink != num_times){
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(300);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(300);
        blink++;
    }
}

#define THERMISTOR_NOMINAL 10000    // Resistance at 25 degrees C
#define TEMPERATURE_NOMINAL 25     // Temperature at nominal resistance
#define B_COEFFICIENT 3950          // Beta coefficient of the thermistor
#define SERIES_RESISTOR 10000       // Value of the series resistor
#define ADC_MAX 4095                // Max ADC value (12-bit resolution)
#define V_REF 3.3                   // Reference voltage of the ADC


char* intToString(int number){
    char* adc1 = (char*)malloc(12 * sizeof(char));
    sprintf(adc1, "%d", number);
    return adc1;
}

char* floatToString(float number){
    char* adc1 = (char*)malloc(12 * sizeof(char));
    sprintf(adc1, "%.2f", number);
    return adc1;
}
// Function to convert ADC reading to temperature
float calculate_temperature(int adc_value) {
    lcd_print("adc: ");
    lcd_print("          ");
    lcd_setCursor(1, 5);
    lcd_print(intToString(adc_value));
    sleep_ms(1000);
    // Convert ADC reading to voltage
    float voltage = ((float)adc_value /ADC_MAX) * V_REF;
    lcd_setCursor(1,0);
    lcd_print("vol: ");
    lcd_print("          ");
    lcd_setCursor(1, 5);
    lcd_print(floatToString(voltage));
    sleep_ms(1000);

    // Calculate the resistance of the thermistor
    float thermistor_resistance = SERIES_RESISTOR * (V_REF / voltage - 1);
    lcd_setCursor(1,0);
    lcd_print("res: ");
    lcd_print("          ");
    lcd_setCursor(1, 5);
    lcd_print(floatToString(thermistor_resistance));
    sleep_ms(1000);

    // Apply the Beta equation
    float beta;
    beta = thermistor_resistance / THERMISTOR_NOMINAL;  // (R/Ro)
    beta = log(beta);                             // ln(R/Ro)
    beta /= B_COEFFICIENT;                             // 1/B * ln(R/Ro)
    beta += 1.0 / (TEMPERATURE_NOMINAL + 273.15);      // + (1/To)
    beta = 1.0 / beta;                            // Invert
    beta -= 273.15;                                    // Convert to Celsius
    
    lcd_setCursor(1,0);
    lcd_print("tem: ");
    lcd_print("          ");
    lcd_setCursor(1, 5);
    lcd_print(floatToString(beta));
    sleep_ms(1000);
    return beta;
}

// void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t val){
//     //write the bits to the data pin
//     for(int i = 0; i < 8; i++) {
//         gpio_put(clockPin, 0);

//         gpio_put(dataPin, (val & 0x80) ? 1 : 0 );
//         val <<= 1;

//         gpio_put(clockPin, 1);
//     }
// }

// void writeTo595(uint16_t _data ) {
//   // Output low level to latchPin
//   gpio_put(LATCH_PIN, 0);

//   //Send higher byte (second register first)
//   shiftOut(DATA_PIN, CLK_PIN, _data >> 8);

//   // Send serial data to 74HC595
//   shiftOut(DATA_PIN, CLK_PIN, _data & 0xFF);
  
//   // Output high level to latchPin, and 74HC595 will update the data to the parallel output port.
//   gpio_put(LATCH_PIN, 1);

// }

void main() {
    stdio_init_all();

    lcd_init(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN); // sda and scl

    lcd_clear();
    lcd_home();
    lcd_print("a");

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        lcd_clear();
        lcd_home();
        lcd_print("wifi failed");
        lcd_setCursor(1,0);
        lcd_print("exiting...");
        return;
    }    
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);                                                                         
    
    // Set clk pin as output
    // gpio_init(CLK_PIN);
    // gpio_set_dir(CLK_PIN, GPIO_OUT);
    
    // // Set data pin as output
    // gpio_init(DATA_PIN);
    // gpio_set_dir(DATA_PIN, GPIO_OUT);

    // // Set latch pin as output
    // gpio_init(LATCH_PIN);
    // gpio_set_dir(LATCH_PIN, GPIO_OUT);
    // on_board_led_init();

    // Initialize the ADC
    adc_init();
    
    // Select ADC input 0 (GP26)
    adc_gpio_init(27);
    while(true){

        adc_select_input(1);
        uint16_t adc_value = adc_read();
        lcd_setCursor(0,0);
        lcd_print("adc1: ");
        char adc1[16];
        sprintf(adc1, "%d", adc_value);
        lcd_print(adc1);
        lcd_setCursor(1,0);
        float tenTemps = 0;
        float temperature;
        
        // for( int i = 0; i < 100; i++){
        //     // Calculate temperature from ADC value
             temperature = calculate_temperature(adc_value);
        // }

        //temperature = tenTemps / 100;
       
        // Print temperature
        printf("Temperature: %.2f°C\n", temperature);
        char tempC[16];
        char tempF[16];
        // double rounded = round(temp_f);
        // double result = rounded / 6;
        // int resultInt = (int)result;
        sprintf(tempC, "%.2f", temperature);
        // lcd_home();
        // lcd_print("Temp: ");
        // lcd_print("          ");
        // lcd_setCursor(0,6);
        // lcd_print(tempC);
        // lcd_print(" C");

        // float fTemp = (temperature * 9 / 5) + 32;
        // lcd_setCursor(1, 0);
        // lcd_print("Temp: ");
        // lcd_print("          ");
        // lcd_setCursor(1,6);
        // sprintf(tempF, "%.2f", fTemp);
        // lcd_print(tempF);
        // lcd_print(" F");

        // Wait for 5 second
        sleep_ms(5000);
    }
}
