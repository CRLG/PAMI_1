#ifndef __MAIN_APP_H_
#define __MAIN_APP_H_

#ifdef __cplusplus
extern "C" {
#endif

int main_app(void);
void irq_uart2();
void irq_uart_stlink();

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_APP_H_ */
