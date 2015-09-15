#include "gpio.h"

static inline void *get_gpio_base(int bank)
{
	const unsigned int offset[4] = {0, 4, 8, 0x100};
	/* gpio register bank offset - refer Appendix A.36 */
	return (struct gpio_reg *)(ARMD1_GPIO_BASE + offset[bank]);
}

int gpio_request(unsigned gpio, const char *label)
{
	if (gpio >= MV_MAX_GPIO) {
		return -1;
	}
	return 0;
}

int gpio_free(unsigned gpio)
{
	return 0;
}

int gpio_direction_input(unsigned gpio)
{
	struct gpio_reg *gpio_reg_bank;

	if (gpio >= MV_MAX_GPIO) {
		return -1;
	}

	gpio_reg_bank = get_gpio_base(GPIO_TO_REG(gpio));
	writel(GPIO_TO_BIT(gpio), &gpio_reg_bank->gcdr);
	return 0;
}

int gpio_direction_output(unsigned gpio, int value)
{
	struct gpio_reg *gpio_reg_bank;

    unsigned int addr;
    unsigned int reg_val;

	if (gpio >= MV_MAX_GPIO) {
		return -1;
	}

	gpio_reg_bank = get_gpio_base(GPIO_TO_REG(gpio));

	addr = (unsigned int)&gpio_reg_bank->gsdr;
	reg_val = GPIO_TO_BIT(gpio);

	writel(GPIO_TO_BIT(gpio), &gpio_reg_bank->gsdr);
	gpio_set_value(gpio, value);
	return 0;
}

int gpio_get_value(unsigned gpio)
{
	struct gpio_reg *gpio_reg_bank;
	UINT32 gpio_val;

	if (gpio >= MV_MAX_GPIO) {
		return -1;
	}

	gpio_reg_bank = get_gpio_base(GPIO_TO_REG(gpio));
	gpio_val = readl(&gpio_reg_bank->gplr);

	return GPIO_VAL(gpio, gpio_val);
}

int gpio_set_value(unsigned gpio, int value)
{
	struct gpio_reg *gpio_reg_bank;

    unsigned int addr;
    unsigned int reg_val;

	if (gpio >= MV_MAX_GPIO) {
		return -1;
	}

	gpio_reg_bank = get_gpio_base(GPIO_TO_REG(gpio));
	if (value)
		writel(GPIO_TO_BIT(gpio), &gpio_reg_bank->gpsr);
	else
	{

        addr = (unsigned int)&gpio_reg_bank->gpcr;
        reg_val = GPIO_TO_BIT(gpio);

	    writel(GPIO_TO_BIT(gpio), &gpio_reg_bank->gpcr);

	}


	return 0;
}

void test_gpio(void)
{
   *(volatile unsigned int *) (0xD401905C) = 0x100000;
   *(volatile unsigned int *) (0xD401902C) = 0x100000;
}
