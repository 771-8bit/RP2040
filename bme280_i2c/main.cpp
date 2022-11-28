#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "bme280/bme280.h"

const uint LED_PIN = 25;
BME280 bme;
#define SEALEVELPRESSURE_HPA (1013.25)

int main()
{
	stdio_init_all();

	i2c_init(i2c_default, 2000 * 1000);
	gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
	gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
	gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
	gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);
	sleep_ms(500);

	while (!bme.begin(BME280_ADDRESS_ALTERNATE))
	{
		gpio_put(LED_PIN, 0);
		sleep_ms(250);
		gpio_put(LED_PIN, 1);
		sleep_ms(250);
		printf("bme err\n");
	}

	while (1)
	{

		printf("Pressure = %.3f hPa\n", bme.readPressure() / 100.0F);

		printf("Temperature. = %.2f C\n", bme.readTemperature());

		printf("Approx. Altitude = %.2f m\n", bme.readAltitude(SEALEVELPRESSURE_HPA));

		printf("Humidity = %.2f %%\n\n", bme.readHumidity());
	}
}
