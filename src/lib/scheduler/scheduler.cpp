/*!
   \file scheduler.cpp
   \brief Simple task scheduler and execution
   \author Simone Tosato
   \author Davide Bortolami
   \copyright (c) 2017 - Fermium LABS srl
 */
#include "scheduler.h"

static measure_t* measure; /*!< data-chan measure istance */
static bool measure_ready=false;

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
   \param Heater Istance of HEATER, from main.cpp
   \param Led Instance of LED, from main.cpp
 */
void run_tasks(ADS1115* Adc1,ADS1115* Adc2,HEATER* Heater,CGEN* Cgen,LED* Led){

//unsigned long task = timer1_millis() % 8;

        adc1=Adc1; /*!< ADS1115 ADC 1 */
        adc2=Adc2; /*!< ADS1115 ADC 2 */
        heater=Heater;
        cgen = Cgen;
        led = Led;
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
   \details takes one measure from each adc
   \param fromLastExecution milliseconds past the last time the task was executed
   \return 0 if executed, 2 if not execute, 1 if errored
 */
int task0(unsigned long fromLastExecution)
{
  static uint8_t lastRead=0;

  // if too little time passed from the heater transition or the ADC did not have time to sample the measures accordingly
  if(fromLastExecution < fmax(adc1->getsperiod_ms() , adc2->getsperiod_ms()) && heater->time_to_transition() < 4){
    return 2;
  }

  //if the last read was resetted, allocate a new data-chan measure
  if(lastRead==0){
    measure=new_nonrealtime_measure(0xFF);
  }

  _delay_ms(10);

  float mes;

  /*
  Data-chan structure channels
  1 - differential read ADC1 between Vr and 2.5V
  2 - differential read ADC1 between Temp and 2.5V
  3 - differential read ADC1 between I_meas and 2.5V
  4 - not used
  5 - differential read ADC2 between Vh (grounded) and 2.5V
  6 - differential read ADC2 between Vh Ampl. and 2.5V
  7 - differential read ADC2 between the Gaussmeter and 2.5V
  8 - not used
  */

  mes = adc1->get_diff_read(lastRead,3);
  check(mes,lastRead+1);
  add_measure(measure,lastRead+1,mes);

  _delay_ms(10);

  mes = adc2->get_diff_read(lastRead,3);
  check(mes,lastRead+5);
  add_measure(measure,lastRead+5,mes);

  measure_ready=lastRead==2;
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
        datachan_register_measure(measure);
        measure_ready=false;
        return 0;
}

/*!
   \brief task2
   \details Triggers the heater transition evaluation and other similar tasks
   \param fromLastExecution milliseconds past the last time the task was executed
   \return 0 if executed, 2 if not execute, 1 if errored
 */
int task2(unsigned long fromLastExecution)
{
        heater->evaluate();
        led->evaluate();
        cgen->evaluate();
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

/*!
   \brief safety measures
   \details Checks whether all measures are in-parameter, otherwise shutdown down what needs to be shut down.
   \param mes the input measurement, in float
   \param ch the channel the measure refers to
   \return 0 if executed, 2 if not execute, 1 if errored
*/
bool check(float mes,uint8_t ch){
  switch (ch) {
    case 1:
    break;
    case 2://temp
      if(mes>0.7527){
        heater->set_duty_cycle(0);
        heater->disable();
        return 1;
      }
    break;
    case 3:
    break;
    case 4:
    break;
    case 5:
    break;
    case 6:
    break;
    case 7:
    break;
    case 8:
    break;
    return 0;
  }
}
