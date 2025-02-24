/********************************************************************
 *@file    mmheap_demo_1.c                                                      
 *@brief   测试基于mmblk静态内存池管理机制
 *@author  Mculover666
 *@date    2020年7月25日16:15:25
*********************************************************************/

#include <tos_k.h>

k_task_t	task1;
k_stack_t	task1_stack[512];

typedef struct blk_st {
    int   id;
    char* payload;
} blk_t;

#define BLK_NUM 5

k_mmblk_pool_t mmblk_pool;
uint8_t mmblk_pool_buffer[BLK_NUM * sizeof(blk_t)];

void task1_entry(void *arg)
{   
    blk_t *ptr = NULL;
    blk_t *ptr2 = NULL;
    k_err_t err;
    uint8_t i, j;
    
    /* 打印出一个块的大小 */
    printf("block size is %d bytes\r\n", sizeof(blk_t));
    
     /* 打印出一个块的大小 */
    printf("block size is %d bytes\r\n", sizeof(blk_t));
    
    /* 测试最大数目的块 */
    for (i = 0; i < BLK_NUM; i++) {
        //申请块
        err = tos_mmblk_alloc(&mmblk_pool, (void*)&ptr);
        if (err != K_ERR_NONE) {
            printf("mmblk %d alloc fail, err = %d\r\n", i, err);
            return;
        }
        else {
            printf("mmblk %d alloc success\r\n", i);
        }
        
        //使用块
        ptr->id = i;
        ptr->payload = "hello";
        printf("mmblk id:%d payload:%s\r\n", ptr->id, ptr->payload);
    }
    
    /* 全部申请之后，测试是否可以再次申请成功 */
    err = tos_mmblk_alloc(&mmblk_pool, (void*)&ptr2);
    if (err != K_ERR_NONE) {
        printf("mmblk %d alloc fail, err = %d\r\n", i, err);
    }
    else {
        printf("mmblk %d alloc success\r\n", i);
    }
    
    for (j = 0; j < i; j++) {
        /* 使用完毕之后释放 */
        err = tos_mmblk_free(&mmblk_pool, ptr);
        if (err != K_ERR_NONE) {
            printf("mmblk %d free fail, err = %d\r\n", j, err);
            return;
        }
        else {
            printf("mmblk %d free success\r\n", j);
        }
    }
    
    /* 销毁静态内存池 */
    err = tos_mmblk_pool_destroy(&mmblk_pool);
    if (err != K_ERR_NONE) {
        printf("mmblk pool destroy fail, err = %d\r\n", err);
        return;
    }
    else {
        printf("mmblk pool destroy success\r\n");
    }
    
    
    while (1) {
        tos_task_delay(1000);
    }
}

void application_entry(void *arg)
{   
    k_err_t err;
    
    //初始化静态内存池
    err = tos_mmblk_pool_create(&mmblk_pool, mmblk_pool_buffer, BLK_NUM, sizeof(blk_t));
    if (err != K_ERR_NONE) {
        printf("mmblk pool create fail, err = %d\r\n", err);
        return;
    }
    else {
        printf("mmblk pool create success\r\n");
    }

    //创建任务
    tos_task_create(&task1, "task1", task1_entry, NULL, 2, task1_stack, sizeof(task1_stack), 10);

    return;
}

/********************************************************
result:

TencentOS-tiny Port on STM32L431RCT6 By Mculover666
mmblk pool create success
block size is 8 bytes
a mmblk alloc success
mmblk id:1 payload:hello
a mmblk free success

*********************************************************/
