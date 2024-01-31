﻿/**
 * @file    _modbus_rt.c
 * @brief   modbus_rt的pika层的API实现
 * @author  SenySunny (senysunny@163.com)
 * @date    2023-05-14
 *
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Ma Longwei.
 * All rights reserved.</center></h2>
 *
 */
#include "PikaObj.h"
#include "PikaStdData_List.h"
#include "TinyObj.h"
#include "_modbus_rt__rtu.h"

#include "modbus_rtu.h"

#if  MODBUS_P2P_ENABLE
#include "modbus_p2p.h"
#endif

void _modbus_rt__rtu___del__(PikaObj *self) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_del error, dev is NULL.\n");
        return ;
    }
    ret = modbus_rtu_destroy(&dev);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_destroy error, code:%d.\n", ret);
        return ;
    }
    obj_setPtr(self, "dev", NULL);
#endif
}

void _modbus_rt__rtu__init(PikaObj *self, int mode) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
#else
    rtu_modbus_device_t dev = modbus_rtu((modbus_mode_type)mode);
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu create error.\n");
    }
    obj_setInt(self, "mode", mode);
    obj_setPtr(self, "dev", dev);
    int ascii_flag = obj_getInt(self, "ascii_flag");
#if MODBUS_ASCII_SLAVE_ENABLE || MODBUS_ASCII_MASTER_ENABLE
    if(ascii_flag) {
        modbus_rtu_set_ascii_flag(dev, ascii_flag);
    }
#endif
#endif
}

int _modbus_rt__rtu__set_serial(PikaObj *self, char* devname, int baudrate, int bytesize, char* parity, int stopbits, int xonxoff) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#else    
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_set_serial error, dev is NULL.\n");
        return 0;
    }
    if(0 != dev->status) {
        pika_platform_printf("modbus_rtu_set_serial error, dev is opened.\n");
        return 0;
    }
    ret = modbus_rtu_set_serial(dev, devname, baudrate, bytesize, parity[0], stopbits, xonxoff);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_set_serial error, code:%d.\n", ret);
        return 0;
    }
    return 1;
#endif
}

int _modbus_rt__rtu__set_over_type(PikaObj *self, int over_type) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#elif (!MODBUS_SERIAL_OVER_TCP_ENABLE) && (!MODBUS_SERIAL_OVER_UDP_ENABLE)
    pika_platform_printf("modbus_rtu_set_over_type error, MODBUS_SERIAL_OVER_XXX_ENABLE is not ENABLE.\n", ret);
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_set_over_type error, dev is NULL.\n");
        return 0;
    }
    if(0 != dev->status) {
        pika_platform_printf("modbus_rtu_set_over_type error, dev is opened.\n");
        return 0;
    }

    if((OVER_NONE != over_type) && (OVER_NET != over_type)) {
        pika_platform_printf("modbus_rtu_set_over_type error, type is only for OVER_NONE or OVER_NET.\n");
        return 0;
    }
    ret = modbus_rtu_set_over_type(dev, over_type);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_set_over_type error, code:%d.\n", ret);
        return 0;
    }
    return 1;   
#endif
}

int _modbus_rt__rtu__set_net(PikaObj *self, char* ip, int port, int type) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#elif (!MODBUS_SERIAL_OVER_TCP_ENABLE) && (!MODBUS_SERIAL_OVER_UDP_ENABLE)
    pika_platform_printf("modbus_rtu_set_over_type error, MODBUS_SERIAL_OVER_XXX_ENABLE is not ENABLE.\n", ret);
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_set_net error, dev is NULL.\n");
        return 0;
    }
    if(0 != dev->status) {
        pika_platform_printf("modbus_rtu_set_net error, dev is opened.\n");
        return 0;
    }
    if(OVER_NET != dev->over_type) {
        pika_platform_printf("modbus_rtu_set_net error, over_type is not OVER_NET.\n");
        return 0;
    }
    ret = modbus_rtu_set_net(dev, ip, port, type);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_set_net error, code:%d.\n", ret);
        return 0;
    }
    return 1;
#endif
}

int _modbus_rt__rtu__set_ip(PikaObj *self, char* ip) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#elif (!MODBUS_SERIAL_OVER_TCP_ENABLE) && (!MODBUS_SERIAL_OVER_UDP_ENABLE)
    pika_platform_printf("modbus_rtu_set_over_type error, MODBUS_SERIAL_OVER_XXX_ENABLE is not ENABLE.\n", ret);
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_set_ip error, dev is NULL.\n");
        return 0;
    }
    if(0 != dev->status) {
        pika_platform_printf("modbus_rtu_set_ip error, dev is opened.\n");
        return 0;
    }
    if(OVER_NET != dev->over_type) {
        pika_platform_printf("modbus_rtu_set_ip error, over_type is not OVER_NET.\n");
        return 0;
    }
    ret = modbus_rtu_set_ip(dev, ip);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_set_ip error, code:%d.\n", ret);
        return 0;
    }
    return 1;
#endif
}

int _modbus_rt__rtu__set_port(PikaObj *self, int port) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#elif (!MODBUS_SERIAL_OVER_TCP_ENABLE) && (!MODBUS_SERIAL_OVER_UDP_ENABLE)
    pika_platform_printf("modbus_rtu_set_over_type error, MODBUS_SERIAL_OVER_XXX_ENABLE is not ENABLE.\n", ret);
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_set_port error, dev is NULL.\n");
        return 0;
    }
    if(0 != dev->status) {
        pika_platform_printf("modbus_rtu_set_port error, dev is opened.\n");
        return 0;
    }
    if(OVER_NET != dev->over_type) {
        pika_platform_printf("modbus_rtu_set_port error, over_type is not OVER_NET.\n");
        return 0;
    }
    ret = modbus_rtu_set_port(dev, port);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_set_port error, code:%d.\n", ret);
        return 0;
    }
    return 1;
#endif
}

int _modbus_rt__rtu__set_type(PikaObj *self, int type) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#elif (!MODBUS_SERIAL_OVER_TCP_ENABLE) && (!MODBUS_SERIAL_OVER_UDP_ENABLE)
    pika_platform_printf("modbus_rtu_set_over_type error, MODBUS_SERIAL_OVER_XXX_ENABLE is not ENABLE.\n", ret);
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_set_type error, dev is NULL.\n");
        return 0;
    }
    if(0 != dev->status) {
        pika_platform_printf("modbus_rtu_set_type error, dev is opened.\n");
        return 0;
    }
    if(OVER_NET != dev->over_type) {
        pika_platform_printf("modbus_rtu_set_type error, over_type is not OVER_NET.\n");
        return 0;
    }
    ret = modbus_rtu_set_type(dev, type);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_set_type error, code:%d.\n", ret);
        return 0;
    }
    return 1;
#endif
}

int _modbus_rt__rtu__set_p2p(PikaObj *self, int p2p_flag) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#elif  (!MODBUS_P2P_ENABLE)
    pika_platform_printf("MODBUS_P2P_ENABLE is not enabled.\n");
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_set_p2p error, dev is NULL.\n");
        return 0;
    }
    if(0 != dev->status) {
        pika_platform_printf("modbus_rtu_set_p2p error, dev is opened.\n");
        return 0;
    }
    ret = modbus_rtu_set_p2p_flag(dev, p2p_flag);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_set_p2p error, code:%d.\n", ret);
        return 0;
    }
    return 1;
#endif  
}

int _modbus_rt__rtu__open(PikaObj *self) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_open error, dev is NULL.\n");
        return 0;
    }
    if(0 != dev->status) {
        pika_platform_printf("modbus_rtu_open error, dev is opened.\n");
        return 0;
    }
    ret = modbus_rtu_open(dev);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_open error, code: %d.\n", ret);
        return 0;
    }
    return 1;
#endif
}

int _modbus_rt__rtu__isopen(PikaObj *self) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#else
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_isopen error, dev is NULL.\n");
        return 0;
    }
    return modbus_rtu_isopen(dev);
#endif
}

int _modbus_rt__rtu__close(PikaObj *self) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_close error, dev is NULL.\n");
        return 0;
    }
    if(0 == dev->status) {
        pika_platform_printf("modbus_rtu_close error, dev is closed.\n");
        return 0;
    }
    ret = modbus_rtu_close(dev);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_close error, code: %d.\n", ret);
        return 0;
    }
    return 1;
#endif
}

int _modbus_rt__rtu__slave_set_addr(PikaObj *self, int addr) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_set_addr error, dev is NULL.\n");
        return 0;
    }
    if(MODBUS_SLAVE != dev->mode){
        pika_platform_printf("modbus_rtu_set_addr is only for slave.\n");
        return 0;
    }
    if(0 != dev->status) {
        pika_platform_printf("modbus_rtu_set_addr error, dev is opened.\n");
        return 0;
    }
    ret = modbus_rtu_set_addr(dev,addr);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_set_addr error, code: %d.\n", ret);
        return 0;
    }
    return 1;
#endif
}

int _modbus_rt__rtu__slave_set_strict(PikaObj *self, int strict) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_set_strict error, dev is NULL.\n");
        return 0;
    }
    if(MODBUS_SLAVE != dev->mode){
        pika_platform_printf("modbus_rtu_set_strict is only for slave.\n");
        return 0;
    }
    if(0 != dev->status) {
        pika_platform_printf("modbus_rtu_set_strict error, dev is opened.\n");
        return 0;
    }
    ret = modbus_rtu_set_strict(dev,strict);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_set_strict error, code: %d.\n", ret);
        return 0;
    }
    return 1;
#endif
}

int _modbus_rt__rtu__slave_add_block(PikaObj *self, char* name, int type, int addr, int nums) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_add_block error, dev is NULL.\n");
        return 0;
    }
    if(MODBUS_SLAVE != dev->mode){
        pika_platform_printf("modbus_rtu_add_block is only for slave.\n");
        return 0;
    }
    if(0 != dev->status) {
        pika_platform_printf("modbus_rtu_add_block error, dev is opened.\n");
        return 0;
    }
    if((0 >= strlen(name)) || (0 > type) || (4 < type)){
        pika_platform_printf("modbus_rtu_add_block: the para is error.\n");
        return 0;
    }
    int len = 0;
    if((CIOLS == type) || (INPUTS == type)) {
        len = nums;
    } else if((INPUT_REGISTERS == type) || (REGISTERS == type)) {
        len = nums * 2;
    }
    obj_setBytes(self, name, NULL, len);
    uint8_t* block = obj_getBytes(self, name);
    ret = modbus_rtu_add_block(dev,type, addr, block, nums);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_add_block error, code: %d.\n", ret);
        return 0;
    }
    return 1;
#endif
}

#if (MODBUS_RTU_SLAVE_ENABLE) || (MODBUS_RTU_MASTER_ENABLE)
PikaEventListener* g_modbus_rt_rtu_event_listener = NULL;
#endif

int _modbus_rt__rtu__slave_pre_ans_handler(agile_modbus_t *ctx, int slave, int function,int addr, int quantity) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#else
    char hash_str[32] = {0};
    memset(hash_str, 0, sizeof(hash_str));
    sprintf(hash_str, "pre_ans%p", ctx);
    uint32_t eventId = hash_time33(hash_str);
    Arg* evt_obj_arg = arg_newDirectObj(New_TinyObj);
    PikaObj* evt_obj = arg_getPtr(evt_obj_arg);
    obj_setInt(evt_obj, "slave", slave);
    obj_setInt(evt_obj, "function", function);
    obj_setInt(evt_obj, "addr", addr);
    obj_setInt(evt_obj, "quantity", quantity);
    pika_eventListener_send(g_modbus_rt_rtu_event_listener, eventId, evt_obj_arg);
    return 1;
#endif
}

int _modbus_rt__rtu__slave_set_pre_ans_callback(PikaObj *self, Arg* cb) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_set_pre_ans_callback error, dev is NULL.\n");
        return 0;
    }
    if(MODBUS_SLAVE != dev->mode){
        pika_platform_printf("modbus_rtu_set_pre_ans_callback is only for slave.\n");
        return 0;
    }
    if(0 != dev->status) {
        pika_platform_printf("modbus_rtu_set_pre_ans_callback error, dev is opened.\n");
        return 0;
    }
    if(NULL == cb) {
        pika_platform_printf("modbus_rtu_set_pre_ans_callback error, cb is NULL.\n");
        return 0;
    }
    ret = modbus_rtu_set_pre_ans_callback(dev,_modbus_rt__rtu__slave_pre_ans_handler);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_set_pre_ans_callback error, code: %d.\n", ret);
        return 0;
    }
    if (NULL == g_modbus_rt_rtu_event_listener) {
        pika_eventListener_init(&g_modbus_rt_rtu_event_listener);
    }
    char hash_str[32] = {0};
    memset(hash_str, 0, sizeof(hash_str));
    sprintf(hash_str, "pre_ans%p", dev->ctx);
    uint32_t eventId = hash_time33(hash_str);
    pika_eventListener_registEventCallback(g_modbus_rt_rtu_event_listener, eventId, cb);
    return 1;
#endif
}

int _modbus_rt__rtu__slave_done_handler(agile_modbus_t *ctx, int slave, int function,int addr, int quantity) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#else
    char hash_str[32] = {0};
    memset(hash_str, 0, sizeof(hash_str));
    sprintf(hash_str, "done%p", ctx);
    uint32_t eventId = hash_time33(hash_str);
    Arg* evt_obj_arg = arg_newDirectObj(New_TinyObj);
    PikaObj* evt_obj = arg_getPtr(evt_obj_arg);
    obj_setInt(evt_obj, "slave", slave);
    obj_setInt(evt_obj, "function", function);
    obj_setInt(evt_obj, "addr", addr);
    obj_setInt(evt_obj, "quantity", quantity);
    pika_eventListener_send(g_modbus_rt_rtu_event_listener, eventId, evt_obj_arg);
    return 1;
#endif
}

int _modbus_rt__rtu__slave_set_done_callback(PikaObj *self, Arg* cb) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_set_done_callback error, dev is NULL.\n");
        return 0;
    }
    if(MODBUS_SLAVE != dev->mode){
        pika_platform_printf("modbus_rtu_set_done_callback is only for slave.\n");
        return 0;
    }
    if(0 != dev->status) {
        pika_platform_printf("modbus_rtu_set_done_callback error, dev is opened.\n");
        return 0;
    }
    if(NULL == cb) {
        pika_platform_printf("modbus_rtu_set_pre_ans_callback error, cb is NULL.\n");
        return 0;
    }
    ret = modbus_rtu_set_done_callback(dev,_modbus_rt__rtu__slave_done_handler);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_set_done_callback error, code: %d.\n", ret);
        return 0;
    }
    if (NULL == g_modbus_rt_rtu_event_listener) {
        pika_eventListener_init(&g_modbus_rt_rtu_event_listener);
    }
    char hash_str[32] = {0};
    memset(hash_str, 0, sizeof(hash_str));
    sprintf(hash_str, "done%p", dev->ctx);
    uint32_t eventId = hash_time33(hash_str);
    pika_eventListener_registEventCallback(g_modbus_rt_rtu_event_listener, eventId, cb);
    return 1;
#endif
}

int _modbus_rt__rtu__slave_set_dev_binding(PikaObj *self, int flag) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#elif (!SLAVE_DATA_DEVICE_BINDING)
    pika_platform_printf("SLAVE_DATA_DEVICE_BINDING is not activated.\n");
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_set_dev_binding error, dev is NULL.\n");
        return 0;
    }
    if(MODBUS_SLAVE != dev->mode){
        pika_platform_printf("modbus_rtu_set_dev_binding is only for slave.\n");
        return 0;
    }
    if(0 != dev->status) {
        pika_platform_printf("modbus_rtu_set_dev_binding error, dev is opened.\n");
        return 0;
    }
    ret = modbus_rtu_set_dev_binding(dev,flag);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_set_dev_binding error, code: %d.\n", ret);
        return 0;
    }
    return 1;
#endif
}


int _modbus_rt__rtu__master_set_server(PikaObj *self, char* saddr, int sport) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#elif (!MODBUS_SERIAL_OVER_TCP_ENABLE) && (!MODBUS_SERIAL_OVER_UDP_ENABLE)
    pika_platform_printf("modbus_rtu_set_over_type error, MODBUS_SERIAL_OVER_XXX_ENABLE is not ENABLE.\n", ret);
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_set_server error, dev is NULL.\n");
        return 0;
    }
    if((MODBUS_MASTER != dev->mode) || (OVER_NET != dev->over_type)){
        pika_platform_printf("modbus_rtu_set_server is only for master and OVER_NET.\n");
        return 0;
    }
    ret = modbus_rtu_set_server(dev, saddr, sport);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_set_server error, code: %d.\n", ret);
        return 0;
    }
    return 1;
#endif
}

char* _modbus_rt__rtu__master_get_saddr(PikaObj *self) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return NULL;
#elif (!MODBUS_SERIAL_OVER_TCP_ENABLE) && (!MODBUS_SERIAL_OVER_UDP_ENABLE)
    pika_platform_printf("modbus_rtu_set_over_type error, MODBUS_SERIAL_OVER_XXX_ENABLE is not ENABLE.\n", ret);
    return NULL;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_get_saddr error, dev is NULL.\n");
        return NULL;
    }
    if((MODBUS_MASTER != dev->mode) || (OVER_NET != dev->over_type)){
        pika_platform_printf("modbus_rtu_get_saddr is only for master and OVER_NET.\n");
        return NULL;
    }
    char saddr[INET_ADDRSTRLEN];
    ret = modbus_rtu_get_saddr(dev, saddr);
    if(MODBUS_RT_EOK != ret){
        pika_platform_printf("modbus_rtu_get_saddr error, code: %d.\n", ret);
        return NULL;
    }
    char* str = obj_cacheStr(self, saddr);
    return str;
#endif
}

PikaObj* _modbus_rt__rtu__slave_read_regs(PikaObj *self, int type, int addr, PikaTuple* val){
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return NULL;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_read_regs error, dev is NULL.\n");
        return NULL;
    }
    if(MODBUS_SLAVE != dev->mode){
        pika_platform_printf("modbus_rtu_read_regs is only for slave.\n");
        return NULL;
    }
    if(0 == dev->status) {
        pika_platform_printf("modbus_rtu_read_regs error, dev is not opened.\n");
        return 0;
    }
    uint8_t *dest = NULL;
    /* 确保参数正确，且值无误*/
    if(1 != pikaTuple_getSize(val)) {
        pika_platform_printf("modbus_rtu_read_regs: the para num of func is error.\n");
        return NULL;
    }
    Arg* arg_0 = pikaTuple_getArg(val, 0);
    ArgType t = arg_getType(arg_0);
    if (ARG_TYPE_INT != t) {
        pika_platform_printf("modbus_rtu_read_regs: the para type of func is error.\n");
        return NULL;
    }
    int quantity = arg_getInt(arg_0);
    if(0 >= quantity) {
        pika_platform_printf("modbus_rtu_read_regs: quantity can not be zero or negative.\n");
        return NULL;
    }
    if((CIOLS == type) || (INPUTS == type)) {
        dest = modbus_rt_calloc(1,quantity);
        if(dest == NULL) {
            pika_platform_printf("modbus_rtu_read_regs: mem is not enough.\n");
            return NULL;
        }
    } else if((INPUT_REGISTERS == type) || (REGISTERS == type)) {
        dest = modbus_rt_calloc(1,quantity * 2);
        if(dest == NULL) {
            pika_platform_printf("modbus_rtu_read_regs: mem is not enough.\n");
            return NULL;
        }
    }
    ret = modbus_rtu_excuse(dev, MODBUS_READ, type, addr, quantity, dest);
    if(MODBUS_RT_EOK != ret) {
        modbus_rt_free(dest);
        pika_platform_printf("modbus_rtu_read_regs: modbus_rtu_excuse error, code: %d.\n", ret);
        return NULL;
    }
    uint16_t *dest_r = (uint16_t *)dest;
    PikaList* list = New_PikaList();
    for(int i = 0; i < quantity; i++ ) {
        if((CIOLS == type) || (INPUTS == type)) {
            pikaList_append(list, arg_newInt(dest[i]));
        } else if((INPUT_REGISTERS == type) || (REGISTERS == type)) {
            pikaList_append(list, arg_newInt(dest_r[i]));
        }
    }
    modbus_rt_free(dest);
    return list;
#endif
}

int _modbus_rt__rtu__slave_write_regs(PikaObj *self, int type, int addr, PikaTuple* val) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_write_regs error, dev is NULL.\n");
        return 0;
    }
    if(MODBUS_SLAVE != dev->mode){
        pika_platform_printf("modbus_rtu_write_regs is only for master.\n");
        return 0;
    }
    if(0 == dev->status) {
        pika_platform_printf("modbus_rtu_write_regs error, dev is not opened.\n");
        return 0;
    }
    uint8_t *dest = NULL;
    /* 确保参数正确，且值无误*/
    if(2 != pikaTuple_getSize(val)) {
        pika_platform_printf("modbus_rtu_write_regs: the para num of func is error.\n");
        return 0;
    }
    Arg* arg_0 = pikaTuple_getArg(val, 0);
    ArgType t = arg_getType(arg_0);
    if (ARG_TYPE_INT != t) {
        pika_platform_printf("modbus_rtu_write_regs: the para0 type of func is error.\n");
        return 0;
    }
    int quantity = arg_getInt(arg_0);

    if((CIOLS == type) || (INPUTS == type)) {
        dest = modbus_rt_calloc(1,quantity);
        if(dest == NULL) {
            pika_platform_printf("modbus_rtu_write_regs: mem is not enough.\n");
            return 0;
        }
        memset(dest,0,quantity);
    } else if((INPUT_REGISTERS == type) || (REGISTERS == type)) {
        dest = modbus_rt_calloc(1,quantity * 2);
        if(dest == NULL) {
            pika_platform_printf("modbus_rtu_write_regs: mem is not enough.\n");
            return 0;
        }
        memset(dest,0,quantity * 2);
    }

    Arg* arg_1 = pikaTuple_getArg(val, 1);
    t = arg_getType(arg_1);
    if (ARG_TYPE_OBJECT != t) {
        pika_platform_printf("modbus_rtu_write_regs: the para1 type of func is error.\n");
        return 0;
    }
    PikaObj* _list = arg_getPtr(arg_1);
    int len_list = pikaList_getSize(_list);
    if(len_list > quantity) {
        len_list = quantity;
    }
    if((CIOLS == type) || (INPUTS == type)) {
        for(int i = 0; i < len_list; i++) {
            dest[i] =  pikaList_getInt(_list, i);
        }
        ret = modbus_rtu_excuse(dev, MODBUS_WRITE, type, addr, quantity, dest);
        if(MODBUS_RT_EOK != ret) {
            modbus_rt_free(dest);
            pika_platform_printf("modbus_rtu_write_regs: modbus_rtu_excuse error, code: %d.\n", ret);
            return 0;
        }
    } else if((INPUT_REGISTERS == type) || (REGISTERS == type)) {
        uint16_t *dest_r = (uint16_t *)dest;
        for(int i = 0; i < len_list; i++){
            dest_r[i] =  pikaList_getInt(_list, i);
        }
        ret = modbus_rtu_excuse(dev, MODBUS_WRITE, type, addr, quantity, dest_r);
        if(MODBUS_RT_EOK != ret) {
            modbus_rt_free(dest);
            pika_platform_printf("modbus_rtu_write_regs: modbus_rtu_excuse error, code: %d.\n", ret);
            return 0;
        }
    }
    modbus_rt_free(dest);
    return 1;
#endif
}

PikaObj* _modbus_rt__rtu__master_read_list(PikaObj *self, int slave, int fuction, int addr, PikaTuple* val) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return NULL;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_read_list error, dev is NULL.\n");
        return NULL;
    }
    if(MODBUS_MASTER != dev->mode){
        pika_platform_printf("modbus_rtu_read_list is only for master.\n");
        return NULL;
    }
    if(0 == dev->status) {
        pika_platform_printf("modbus_rtu_read_list error, dev is not opened.\n");
        return 0;
    }
    uint8_t *dest = NULL;
    /* 确保参数正确，且值无误*/
    if(1 != pikaTuple_getSize(val)) {
        pika_platform_printf("modbus_rtu_read_list: the para num of func is error.\n");
        return NULL;
    }
    Arg* arg_0 = pikaTuple_getArg(val, 0);
    ArgType t = arg_getType(arg_0);
    if (ARG_TYPE_INT != t) {
        pika_platform_printf("modbus_rtu_read_list: the para type of func is error.\n");
        return NULL;
    }
    int quantity = arg_getInt(arg_0);
    if(0 >= quantity) {
        pika_platform_printf("modbus_rtu_read_list: quantity can not be zero or negative.\n");
        return NULL;
    }

    if((AGILE_MODBUS_FC_READ_COILS == fuction) ||(AGILE_MODBUS_FC_READ_DISCRETE_INPUTS == fuction)) {
        dest = modbus_rt_calloc(1,quantity);
        if(dest == NULL) {
            pika_platform_printf("modbus_rtu_read_list: mem is not enough.\n");
            return NULL;
        }
    } else if((AGILE_MODBUS_FC_READ_HOLDING_REGISTERS == fuction) ||(AGILE_MODBUS_FC_READ_INPUT_REGISTERS == fuction)) {
        dest = modbus_rt_calloc(1,quantity * 2);
        if(dest == NULL) {
            pika_platform_printf("modbus_rtu_read_list: mem is not enough.\n");
            return NULL;
        }
    }
    ret = modbus_rtu_excuse(dev, slave, fuction, addr, quantity, dest);
    if(MODBUS_RT_EOK != ret) {
        modbus_rt_free(dest);
        pika_platform_printf("modbus_rtu_read_list: modbus_rtu_excuse error, code: %d.\n", ret);
        return NULL;
    }
    uint16_t *dest_r = (uint16_t *)dest;
    PikaList* list = New_PikaList();
    for(int i = 0; i < quantity; i++ ) {
        if((AGILE_MODBUS_FC_READ_COILS == fuction) ||(AGILE_MODBUS_FC_READ_DISCRETE_INPUTS == fuction)) {
            pikaList_append(list, arg_newInt(dest[i]));
        } else if((AGILE_MODBUS_FC_READ_HOLDING_REGISTERS == fuction) ||(AGILE_MODBUS_FC_READ_INPUT_REGISTERS == fuction)) {
            pikaList_append(list, arg_newInt(dest_r[i]));
        }
    }
    modbus_rt_free(dest);
    return list;
#endif
}

int _modbus_rt__rtu__master_write_int(PikaObj *self, int slave, int fuction, int addr, PikaTuple* val) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_write_int error, dev is NULL.\n");
        return 0;
    }
    if(MODBUS_MASTER != dev->mode){
        pika_platform_printf("modbus_rtu_write_int is only for master.\n");
        return 0;
    }
    if(0 == dev->status) {
        pika_platform_printf("modbus_rtu_write_int error, dev is not opened.\n");
        return 0;
    }
    /* 确保参数正确，且值无误*/
    if(1 != pikaTuple_getSize(val)) {
        pika_platform_printf("modbus_rtu_write_int: the para num of func is error.\n");
        return 0;
    }
    Arg* arg_0 = pikaTuple_getArg(val, 0);
    ArgType t = arg_getType(arg_0);
    if (ARG_TYPE_INT != t) {
        pika_platform_printf("modbus_rtu_write_int: the para type of func is error.\n");
        return 0;
    }
    int value = arg_getInt(arg_0);
    ret = modbus_rtu_excuse(dev, slave, fuction, addr, 1, &value);
    if(MODBUS_RT_EOK != ret) {
        pika_platform_printf("modbus_rtu_write_int: modbus_rtu_excuse error, code: %d.\n", ret);
        return 0;
    }
    return 1;
#endif
}

int _modbus_rt__rtu__master_write_list(PikaObj *self, int slave, int fuction, int addr, PikaTuple* val) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_write_list error, dev is NULL.\n");
        return 0;
    }
    if(MODBUS_MASTER != dev->mode){
        pika_platform_printf("modbus_rtu_write_list is only for master.\n");
        return 0;
    }
    if(0 == dev->status) {
        pika_platform_printf("modbus_rtu_write_list error, dev is not opened.\n");
        return 0;
    }
    uint8_t *dest = NULL;
    /* 确保参数正确，且值无误*/
    if(2 != pikaTuple_getSize(val)) {
        pika_platform_printf("modbus_rtu_write_list: the para num of func is error.\n");
        return 0;
    }
    Arg* arg_0 = pikaTuple_getArg(val, 0);
    ArgType t = arg_getType(arg_0);
    if (ARG_TYPE_INT != t) {
        pika_platform_printf("modbus_rtu_write_list: the para0 type of func is error.\n");
        return 0;
    }
    int quantity = arg_getInt(arg_0);

    if(fuction == AGILE_MODBUS_FC_WRITE_MULTIPLE_COILS) {
        dest = modbus_rt_calloc(1,quantity);
        if(dest == NULL) {
            pika_platform_printf("modbus_rtu_write_list: mem is not enough.\n");
            return 0;
        }
        memset(dest,0,quantity);
    } else if(fuction == AGILE_MODBUS_FC_WRITE_MULTIPLE_REGISTERS) {
        dest = modbus_rt_calloc(1,quantity * 2);
        if(dest == NULL) {
            pika_platform_printf("modbus_rtu_write_list: mem is not enough.\n");
            return 0;
        }
        memset(dest,0,quantity * 2);
    }

    Arg* arg_1 = pikaTuple_getArg(val, 1);
    t = arg_getType(arg_1);
    if (ARG_TYPE_OBJECT != t) {
        pika_platform_printf("modbus_rtu_write_list: the para1 type of func is error.\n");
        return 0;
    }
    PikaObj* _list = arg_getPtr(arg_1);
    int len_list = pikaList_getSize(_list);
    if(len_list > quantity) {
        len_list = quantity;
    }
    if(fuction == AGILE_MODBUS_FC_WRITE_MULTIPLE_COILS) {
        for(int i = 0; i < len_list; i++) {
            dest[i] =  pikaList_getInt(_list, i);
        }
        ret = modbus_rtu_excuse(dev, slave, fuction, addr, quantity, dest);
        if(MODBUS_RT_EOK != ret) {
            modbus_rt_free(dest);
            pika_platform_printf("modbus_rtu_write_list: modbus_rtu_excuse error, code: %d.\n", ret);
            return 0;
        }
    } else if(fuction == AGILE_MODBUS_FC_WRITE_MULTIPLE_REGISTERS) {
        uint16_t *dest_r = (uint16_t *)dest;
        for(int i = 0; i < len_list; i++){
            dest_r[i] =  pikaList_getInt(_list, i);
        }
        ret = modbus_rtu_excuse(dev, slave, fuction, addr, quantity, dest_r);
        if(MODBUS_RT_EOK != ret) {
            modbus_rt_free(dest);
            pika_platform_printf("modbus_rtu_write_list: modbus_rtu_excuse error, code: %d.\n", ret);
            return 0;
        }
    }
    modbus_rt_free(dest);
    return 1;
#endif
}

int _modbus_rt__rtu__master_download(PikaObj *self, int slave, char* file_dev, char* file_master) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#elif  (!MODBUS_P2P_ENABLE) || (!MODBUS_P2P_MASTER_ENABLE)
    pika_platform_printf("MODBUS_P2P_ENABLE and MODBUS_P2P_MASTER_ENABLE is not enabled.\n");
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_download error, dev is NULL.\n");
        return 0;
    }
    if(MODBUS_MASTER != dev->mode){
        pika_platform_printf("modbus_rtu_download is only for master.\n");
        return 0;
    }
    if(0 == dev->status) {
        pika_platform_printf("modbus_rtu_download error, dev is not opened.\n");
        return 0;
    }
    if(0 == dev->p2p_flag) {
        pika_platform_printf("modbus_rtu_download error, p2p_flag is not seted.\n");
        return 0;
    }
    ret = modbus_rtu_excuse_file(dev, slave, MODBUS_WRITE, file_dev, file_master);
    if(MODBUS_RT_EOK != ret) {
        pika_platform_printf("modbus_rtu_download error, code: %d.\n", ret);
        return 0;
    }
    return 1;
#endif
}


int _modbus_rt__rtu__master_upload(PikaObj *self, int slave, char* file_dev, char* file_master) {
#if (!MODBUS_RTU_SLAVE_ENABLE) && (!MODBUS_RTU_MASTER_ENABLE)
    pika_platform_printf("modbus rtu is not activated.\n");
    return 0;
#elif  (!MODBUS_P2P_ENABLE) || (!MODBUS_P2P_MASTER_ENABLE)
    pika_platform_printf("MODBUS_P2P_ENABLE and MODBUS_P2P_MASTER_ENABLE is not enabled.\n");
    return 0;
#else
    int ret = 0;
    rtu_modbus_device_t dev = (rtu_modbus_device_t)obj_getPtr(self, "dev");
    if(NULL == dev) {
        pika_platform_printf("modbus_rtu_upload error, dev is NULL.\n");
        return 0;
    }
    if(MODBUS_MASTER != dev->mode){
        pika_platform_printf("modbus_rtu_upload is only for master.\n");
        return 0;
    }
    if(0 == dev->status) {
        pika_platform_printf("modbus_rtu_upload error, dev is not opened.\n");
        return 0;
    }
    if(0 == dev->p2p_flag) {
        pika_platform_printf("modbus_rtu_upload error, p2p_flag is not seted.\n");
        return 0;
    }
    ret = modbus_rtu_excuse_file(dev, slave, MODBUS_READ, file_dev, file_master);
    if(MODBUS_RT_EOK != ret) {
        pika_platform_printf("modbus_rtu_upload error, code: %d.\n", ret);
        return 0;
    }
    return 1;
#endif
}
