#include "scheduler.h"

static measure_t* measure;
static bool measure_ready=false;
static ADS1115* adc1;
static ADS1115* adc2;

void start_task(ADS1115* Adc1,ADS1115* Adc2){
  //unsigned long task = timer1_millis() % 8;
 adc1=Adc1;
 adc2=Adc2;
 static unsigned long executionCycleCounter = 0;
 executionCycleCounter ++;

 static unsigned long lastExecution[NUMBER_OF_TASKS] = {0};

  switch(executionCycleCounter % NUMBER_OF_TASKS){
    case 0:
    if (!task0(executionCycleCounter, lastExecution[executionCycleCounter % NUMBER_OF_TASKS]-timer1_millis()))
    {
    lastExecution[executionCycleCounter % NUMBER_OF_TASKS] = timer1_millis();
    }
    break;

    case 1:
    if (!task1(executionCycleCounter, lastExecution[executionCycleCounter % NUMBER_OF_TASKS]-timer1_millis()))
    {
    lastExecution[executionCycleCounter % NUMBER_OF_TASKS] = timer1_millis();
    }
    break;

    case 2:
    if (!task2(executionCycleCounter, lastExecution[executionCycleCounter % NUMBER_OF_TASKS]-timer1_millis()))
    {
    lastExecution[executionCycleCounter % NUMBER_OF_TASKS] = timer1_millis();
    }
    break;

    case 3:
    if (!task3(executionCycleCounter, lastExecution[executionCycleCounter % NUMBER_OF_TASKS]-timer1_millis()))
    {
    lastExecution[executionCycleCounter % NUMBER_OF_TASKS] = timer1_millis();
    }
    break;

    case 4:
    if (!task4(executionCycleCounter, lastExecution[executionCycleCounter % NUMBER_OF_TASKS]-timer1_millis()))
    {
    lastExecution[executionCycleCounter % NUMBER_OF_TASKS] = timer1_millis();
    }
    break;

    case 5:
    if (!task5(executionCycleCounter, lastExecution[executionCycleCounter % NUMBER_OF_TASKS]-timer1_millis()))
    {
    lastExecution[executionCycleCounter % NUMBER_OF_TASKS] = timer1_millis();
    }
    break;
}
}



//if executed
//return 0;
// if not executed
//return 2;
//error
//return 1;


//Read channels and create new measure
int task0(unsigned long  executionCycleCounter, unsigned long fromLastExecution)
{
  static uint8_t lastRead=0;
  if(fromLastExecution<fmax(adc1->getsp(),adc2->getsp())){
    return 2;
  }
  if(lastRead==0){
    measure=new_nonrealtime_measure(0xFF);
  }
  if(lastRead<4){
    add_measure(measure,lastRead+1,adc1->get_se_read(lastRead));
  }
  else{
    add_measure(measure,lastRead+1,adc2->get_se_read(lastRead-4));
  }
  measure_ready=lastRead==7;
  lastRead=(lastRead+1)%8 ;
  return 0;
}

//Add measure to data-chan queue
int task1(unsigned long  executionCycleCounter, unsigned long fromLastExecution)
{
  if(!measure_ready){
    return 2;
  }
  datachan_register_measure(measure);
  measure_ready=false;
  return 0;
}

int task2(unsigned long  executionCycleCounter, unsigned long fromLastExecution)
{

}

int task3(unsigned long  executionCycleCounter, unsigned long fromLastExecution)
{

}

int task4(unsigned long  executionCycleCounter, unsigned long fromLastExecution)
{

}

int task5(unsigned long  executionCycleCounter, unsigned long fromLastExecution)
{

}
