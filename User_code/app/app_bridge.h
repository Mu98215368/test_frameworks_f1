#ifndef APP_BRIDGE_H
#define APP_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

/* 给 C 文件（freertos.c / main.c）用的桥接接口 */
void app_init_c(void);
void app_loop_c(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_BRIDGE_H */