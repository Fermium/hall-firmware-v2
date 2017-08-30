/*!
   \file scheduler.cpp
   \brief Simple task scheduler and execution
   \author Simone Tosato
   \author Davide Bortolami
   \copyright (c) 2017 - Fermium LABS srl
 */
#include "scheduler.h"
#if DUMMY == true
  #warning "The firmware is in dummy mode. Useful just for testing on dev boards"
#endif
#ifndef DUMMY
  #define DUMMY false
#endif

static measure_t* measure; /*!< data-chan measure istance */
static bool measure_ready = false; /*!< atomic variable to flip if the measure is ready  */

static ADS1115* adc1;
static ADS1115* adc2;
static HEATER* heater;
static CGEN* cgen;
static LED* led;

static unsigned long executionCycleCounter = 0; /*!< Counter incremented each execution cycle */

/*!
   \brief main execution task, containing other sub-task
   \param Adc1 Istance of ADC1, from main.cpp
   \param Adc2 Istance of ADC2, from main.cpp
   \param heat Istance of Heater, from main.cpp
 */
void run_tasks(ADS1115* Adc1,ADS1115* Adc2,HEATER* Heater,CGEN* Cgen,LED* Led){

        //unsigned long task = timer1_millis() % 8;

        adc1 = Adc1; /*!< ADS1115 ADC 1 */
        adc2 = Adc2; /*!< ADS1115 ADC 2 */
        heater = Heater; /*!< Heater element control */
        cgen = Cgen; /*!< Current generator control */
        led = Led; /*!< Led blink control */

        executionCycleCounter++;

        static unsigned long lastExecution[NUMBER_OF_TASKS] = {0};

        switch(executionCycleCounter % NUMBER_OF_TASKS) {
        case 0:
                if (!task0(lastExecution[executionCycleCounter % NUMBER_OF_TASKS]-timer1_millis()))
                {
                        lastExecution[executionCycleCounter % NUMBER_OF_TASKS] = timer1_millis();
                }
                break;

        case 1:
                if (!task1(lastExecution[executionCycleCounter % NUMBER_OF_TASKS]-timer1_millis()))
                {
                        lastExecution[executionCycleCounter % NUMBER_OF_TASKS] = timer1_millis();
                }
                break;

        case 2:
                if (!task2(lastExecution[executionCycleCounter % NUMBER_OF_TASKS]-timer1_millis()))
                {
                        lastExecution[executionCycleCounter % NUMBER_OF_TASKS] = timer1_millis();
                }
                break;

        case 3:
                if (!task3(lastExecution[executionCycleCounter % NUMBER_OF_TASKS]-timer1_millis()))
                {
                        lastExecution[executionCycleCounter % NUMBER_OF_TASKS] = timer1_millis();
                }
                break;

        case 4:
                if (!task4(lastExecution[executionCycleCounter % NUMBER_OF_TASKS]-timer1_millis()))
                {
                        lastExecution[executionCycleCounter % NUMBER_OF_TASKS] = timer1_millis();
                }
                break;

        case 5:
                if (!task5(lastExecution[executionCycleCounter % NUMBER_OF_TASKS]-timer1_millis()))
                {
                        lastExecution[executionCycleCounter % NUMBER_OF_TASKS] = timer1_millis();
                }
                break;
        }
}

/*!
   \brief task0 - measure
   \details takes one measure from each adc. Only if the transition of the heater (future or past) is more than 4ms away
   \param fromLastExecution milliseconds past the last time the task was executed
   \return 0 if executed, 2 if not execute, 1 if errored
 */
int task0(unsigned long fromLastExecution)
{
        static uint8_t lastRead=0;
        if (!DUMMY)
        {
                // too little time has passed to take a measure
                if(fromLastExecution < fmax(adc1->getsperiod_ms(),adc2->getsperiod_ms()))
                        return 2;
                //we're too close to the transition of the heater, abort
                if (heater->time_to_transition() < 4)
                        return 2;
        }

        if(lastRead==0) {
                measure=new_nonrealtime_measure(0xFF);
        }


        _delay_ms(10);

        float mes[2];


        if (!DUMMY)
        {
                mes[0] = adc1->get_diff_read(lastRead,3);
                check_measure(mes[0],lastRead+1);
                _delay_ms(10);
                mes[1] = adc2->get_diff_read(lastRead,3);
                check_measure(mes[1],lastRead+5);
        }
        else
        {
                mes[0] =  (( executionCycleCounter % 10000) / (10000.0 / 2.0));
                _delay_ms(10);
                mes[1] =  (( executionCycleCounter % 10000) / (10000.0 / 2.0));
        }


        add_measure(measure,lastRead+1,mes[0]);
        add_measure(measure,lastRead+5,mes[1]);
        measure_ready = (lastRead == 2);
        lastRead=(lastRead+1)%3;

        return 0;

}

/*!
   \brief task1 - enqueue measures
   \param fromLastExecution milliseconds past the last time the task was executed
   \return 0 if executed, 2 if not execute, 1 if errored
 */
int task1(unsigned long fromLastExecution)
{
        if(!measure_ready) {
                return 2;
        }
        else{
                datachan_register_measure(measure);
                measure_ready=false;
                return 0;
        }

}

/*!
   \brief task2
   \details Triggers the heater transition evaluation
   \param fromLastExecution milliseconds past the last time the task was executed
   \return 0 if executed, 2 if not execute, 1 if errored
 */
int task2(unsigned long fromLastExecution)
{
        if (!DUMMY) {
                heater->evaluate();
                led->evaluate();
                cgen->evaluate();
                return 0;
        }
return 1;
}

/*!
   \brief evaluates the current cgen-in
   \param fromLastExecution milliseconds past the last time the task was executed
 */
int task3(unsigned long fromLastExecution)
{
}

/*!
   \brief task4
   \param fromLastExecution milliseconds past the last time the task was executed
   \return 0 if executed, 2 if not execute, 1 if errored
 */
int task4(unsigned long fromLastExecution)
{

}

/*!
   \brief task5
   \param fromLastExecution milliseconds past the last time the task was executed
   \return 0 if executed, 2 if not execute, 1 if errored
 */
int task5(unsigned long fromLastExecution)
{

}


/** 150°c, converted in voltage measured after the thermocouple amplifier*/
#define OVERTEMP_THRESHOLD 0.7527
/*!
   \brief Check if measure is inside parameters, take action otherwise
   \param mes input measure, in float
   \param ch channel, from 1 to 8
   \return bool if everything is fine, 1 if action was taken
 */
bool check_measure(float mes,uint8_t ch){
        switch (ch) {
        case 1: //vr
                break;
        case 2://temp
               //disable heater if overtemp is detected
                if(mes > OVERTEMP_THRESHOLD) {
                        heater->set_duty_cycle(0);
                        heater->disable();
                        return 1;
                }

                break;
        case 3://i-mes
                break;
        case 4://unused
                break;
        case 5://gauss
                break;
        case 6://vh1
                break;
        case 7://vh2
                break;
        case 8://unused
                break;

                return 0;
        }
}
