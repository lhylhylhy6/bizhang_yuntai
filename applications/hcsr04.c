/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-22     lhylh       the first version
 */
/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-22     lhylh       the first version
// */
#include "hcsr04.h"
#include "stdlib.h"
#include "car.h"

#define DBG_TAG "hcsr04"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define CMP_MID_VAL    20


#define FORWARD_HC_Trig_Pin        GET_PIN(F,1)
#define FORWARD_HC_Echo_Pin        GET_PIN(F,2)

#define BACK_HC_Trig_Pin           GET_PIN(F,3)
#define BACK_HC_Echo_Pin           GET_PIN(F,4)

rt_thread_t forward_hc_thread;
rt_thread_t back_hc_thread;

float forward_val=99999;
float back_val=99999;

float forward_min_val=99999;
float back_min_val=99999;

float f_dis;



int HCSR_pin_init(void)
{
    rt_pin_mode(FORWARD_HC_Trig_Pin,PIN_MODE_OUTPUT);
    rt_pin_mode(FORWARD_HC_Echo_Pin,PIN_MODE_INPUT);

    rt_pin_mode(BACK_HC_Trig_Pin,PIN_MODE_OUTPUT);
    rt_pin_mode(BACK_HC_Echo_Pin,PIN_MODE_INPUT);
    return 0;
}
INIT_APP_EXPORT(HCSR_pin_init);

static void hcsr_forward_thread_entry(void *parameter)
{
    int count = 0 ,S = 0,i=0;
    while(1)
    {
        for(i=0;i<5;i++)
        {
            rt_pin_write(FORWARD_HC_Trig_Pin,PIN_HIGH);
            rt_hw_us_delay(10);
            rt_pin_write(FORWARD_HC_Trig_Pin,PIN_LOW);
            while(!rt_pin_read(FORWARD_HC_Echo_Pin))
            {
                int a=0;
                a++;
                rt_hw_us_delay(50);
                if(a>800)
                    goto __exit;
            }
            while(rt_pin_read(FORWARD_HC_Echo_Pin))
            {
                count++;
                rt_hw_us_delay(50);
            }
            S = 17*count +S;
            count = 0;
            rt_thread_mdelay(20);
        }
        forward_val = S/100.0;
        f_dis = forward_val;
        forward_min_val = forward_min_val<forward_val?forward_min_val:forward_val;
        //LOG_D("forward : S = %f cm\n",forward_val);
//        if(forward_val<=CMP_MID_VAL)
//        {
//
//        }
__exit:
        S=0;
        count=0;
        rt_thread_mdelay(190);
    }
}

static void hcsr_back_thread_entry(void *parameter)
{
    int count = 0 ,S = 0,i=0;

    while(1)
    {
        for(i=0;i<1;i++)
        {
            rt_pin_write(BACK_HC_Trig_Pin,PIN_HIGH);
            rt_hw_us_delay(10);
            rt_pin_write(BACK_HC_Trig_Pin,PIN_LOW);
            while(!rt_pin_read(BACK_HC_Echo_Pin))
            {
                int a=0;
                a++;
                rt_hw_us_delay(50);

                if(a>800)
                    goto __exit;
            }
            while(rt_pin_read(BACK_HC_Echo_Pin))
            {
                count++;
                rt_hw_us_delay(50);
            }
            S = 17*count +S;
            count = 0;
            rt_thread_mdelay(20);
        }
        back_val = S/20.0;
        back_min_val = back_min_val<back_val?back_min_val:back_val;
        //LOG_D("back : S = %f cm\n",back_val);
__exit:
        S=0;
        count=0;
        rt_thread_mdelay(200);
    }
}



rt_err_t HCSR_forward_init(void)
{
    rt_err_t ree = RT_EOK;
    forward_hc_thread = rt_thread_create("forward_hc", hcsr_forward_thread_entry, RT_NULL, 1024, 15, 800);
    if(forward_hc_thread)
    {
        ree = rt_thread_startup(forward_hc_thread);
        LOG_D("forward_hc_thread start ok!\r\n");
    }
    return ree;
}

rt_err_t HCSR_back_init(void)
{
    rt_err_t ree=RT_EOK;

    back_hc_thread = rt_thread_create("back_hc", hcsr_back_thread_entry, RT_NULL, 1024, 15, 800);
    if(back_hc_thread)
    {
        ree = rt_thread_startup(back_hc_thread);
        LOG_D("back_hc_thread start ok!\r\n");
    }

    return ree;
}

