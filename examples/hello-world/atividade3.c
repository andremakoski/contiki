/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"

#include <stdio.h> /* For printf() */
#include "dev/leds.h"

# define LED_PING_EVENT 44
# define LED_PONG_EVENT 45

/*---------------------------------------------------------------------------*/
static struct etimer et_hello;
static struct etimer et_blink;
static struct etimer et_proc3;
static uint16_t count;

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
PROCESS(blink_process, "LED blink process");
PROCESS(proc3_process, "proc3 process");
PROCESS(pong_process, "pong process");

/*---------------------------------------------------------------------------*/
AUTOSTART_PROCESSES(&hello_world_process, &blink_process, &proc3_process, &pong_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();

  etimer_set(&et_hello, 5*CLOCK_SECOND);
  PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);


  etimer_set(&et_hello, CLOCK_SECOND * 2);
  count = 0;
  while(1) {
    PROCESS_WAIT_EVENT();
    if(ev == PROCESS_EVENT_TIMER){
        process_post(&pong_process, LED_PING_EVENT, (void*)(&hello_world_process));
        printf("HELLO: PING ENVIADO!\n");
        leds_toggle(LEDS_RED);
        etimer_reset(&et_hello);
        printf("HELLO: Piscando o LED vermelho!\n");
    }
    if(ev == LED_PONG_EVENT){
        printf("HELLO: PONG RECEBIDO!\n");
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(blink_process, ev, data)
{
  PROCESS_BEGIN();

  leds_off(LEDS_ALL);
  etimer_set(&et_blink, 2*CLOCK_SECOND);
  PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

  etimer_set(&et_blink, 1*CLOCK_SECOND);
  while(1) {
    PROCESS_WAIT_EVENT();
    if(ev == PROCESS_EVENT_TIMER){
        process_post (&pong_process , LED_PING_EVENT , ( void *) (&blink_process ) ) ;
        printf("BLINK: Ping Enviado!\n");
        leds_toggle(LEDS_GREEN);
        etimer_reset(&et_blink);
        printf("BLINK: Piscando o LED verde!\n");
    }
    if(ev == LED_PONG_EVENT){
        printf("BLINK: PONG RECEBIDO!\n");
    }
  }

  PROCESS_END();
}

PROCESS_THREAD(proc3_process, ev, data)
{
  PROCESS_BEGIN();

  etimer_set(&et_proc3, 5*CLOCK_SECOND);
  PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);


  etimer_set(&et_proc3, CLOCK_SECOND * 1);
  count = 0;
  while(1) {
    PROCESS_WAIT_EVENT();
    if(ev == PROCESS_EVENT_TIMER){
        process_post(&pong_process , LED_PING_EVENT,(void*)(&proc3_process));
        printf("PROC3: PING ENVIADO!\n");
        //leds_toggle(LEDS_RED);
        etimer_reset(&et_proc3);
    }
    if(ev == LED_PONG_EVENT){
        printf("PROC3: PONG RECEBIDO!\n");
    }
  }

  PROCESS_END();
}
PROCESS_THREAD(pong_process, ev, data)
{
  PROCESS_BEGIN();

/*  etimer_set(&et_hello, 5*CLOCK_SECOND);
  PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);


  etimer_set(&et_hello, CLOCK_SECOND * 2);
  count = 0;*/
  while(1) {
    PROCESS_WAIT_EVENT();
    if(ev == LED_PING_EVENT){
        process_post (( struct process *) data , LED_PONG_EVENT , NULL ) ;

        printf("PONG RESPONDIDO: RESPOSTA DE PONG!\n");
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/



