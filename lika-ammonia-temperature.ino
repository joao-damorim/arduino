#include <Wire.h>  // Arduino IDE
#include <SoftwareSerial.h>
#include <math.h>

#define         GAS_NH3                     (20) //Ammonia
#define         TGS2602_SENSOR               (2)

#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in

float           NH3_Curve[2]    =  {0.585030495,  -3.448654502}; //TGS2602   (0.8,1) (0.5,10) (0.3,30) 

//int gasSensor = 2; // Analog pin A2
//int gasval = 0;

float Ro6 = 2.511;
float RL6 = 0.893;


float MQResistanceCalculation(int raw_adc,float rl_value)
{
  Serial.println((float)raw_adc/100);
  Serial.println(rl_value);
  Serial.println((float)((float)(1024*1000*(float)rl_value)/raw_adc-rl_value));
  return ((float)((float)(1024*1000*(float)rl_value)/raw_adc-rl_value));
  //return  (long)((long)(1024*1000*(long)rl_value)/raw_adc-(long)rl_value);
;
}

float MQRead(int mq_pin,float rl_value)
{
  int i;
  float rs=0;
  Serial.println(analogRead(mq_pin));
  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += MQResistanceCalculation(analogRead(mq_pin),rl_value);
    delay(READ_SAMPLE_INTERVAL);
  }
  
  rs = rs/READ_SAMPLE_TIMES;
  Serial.println(rs); 
  return rs;  
}
 
int  MQGetPercentage(float rs_ro_ratio, float ro, float *pcurve)
{
  Serial.println(rs_ro_ratio);
  Serial.println(ro);
  Serial.println(pcurve[0]);
  Serial.println(pcurve[1]);


  //Serial.println(isnan((double)(pcurve[0] * pow(((float)rs_ro_ratio/ro), pcurve[1]))));
  return (isnan((double)(pcurve[0] * pow(((double)rs_ro_ratio/ro), pcurve[1]))));
}   

int MQGetGasPercentage(float rs_ro_ratio, float ro, int gas_id, int sensor_id){
  if (sensor_id == TGS2602_SENSOR ){
   if ( gas_id == GAS_NH3 ) {
      //Serial.print("AQUI"); 
      return MQGetPercentage(rs_ro_ratio,ro,NH3_Curve);  //TGS2602
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
}

void loop() {
  //TGS2602 C7H8
   Serial.print("TGS2602:"); 
   Serial.print("NH3  : "); 
   Serial.print(MQGetGasPercentage(MQRead(TGS2602_SENSOR,RL6),Ro6,GAS_NH3,TGS2602_SENSOR));
         //gw.send(msg_tgs2602.set((int)ceil(MQGetGasPercentage(MQRead(TGS2602_SENSOR,RL6),Ro6,GAS_NH3,TGS2602_SENSOR))));
   Serial.print( " ppm" );
   Serial.print("\n"); 
}
