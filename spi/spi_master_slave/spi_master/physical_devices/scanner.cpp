#include "scanner.hpp"
#include <iostream>
#include "../loop/common_data/common_variables.hpp"
#include "../utilities/hardcoded_functions.hpp"
#include "../utilities/peripheral_functions.hpp"
#define _USE_MATH_DEFINES // for C++
#include <cmath>

#define PI 3.14159265 

Scanner::Scanner() : pos_({0, 0}), conf_({})
{}

Scanner::~Scanner()
{
  move_to({0, 0}, 10);
}

void Scanner::retract() //втянуть
{
 io3_1.enable();
}
void Scanner::protract() //вытянуть
{
 io3_1.disable();
}
void Scanner::start_scan(const Point &point)
{
  prev_point = pos_;
  vector_z.clear(); //231030;
  other_info.clear();
 
  if (!flgVirtual) move_to(point, 10);
  for (uint32_t i = 0; i < conf_.nPoints_y; ++i) //231030
  {
    for (uint32_t j = 0; j < conf_.nPoints_x; ++j)
    {
      for (uint16_t k = 0; k < conf_.betweenPoints_x; ++k)
      {
       if (!flgVirtual)  set_on_cap(1, ++pos_.x);
        sleep_us(conf_.delayF);
      }
      sleep_ms(50); // CONST 50ms
      if (!flgVirtual) 
      {
        getValuesFromAdc(); ///????? 231025
        getValuesFromAdc();
        vector_z.emplace_back((int16_t)spiBuf[0]);  // get Z from adc ?? 
        if (conf_.flag == 2) //231102
        {
         other_info.emplace_back((int16_t)spiBuf[conf_.flag]); //????
        }
      }
      else
      {
        vector_z.emplace_back(int16_t(10000.0*(sin(M_PI*j*0.1)+sin(M_PI*i*0.1))));  // get Z from adc
        if (conf_.flag == 2) //231102
        {
         other_info.emplace_back(int16_t(10000.0*(sin(M_PI*j*0.1)+sin(M_PI*i*0.1)))); //????
        }
      }   
    }
    for (uint16_t j = 0; j < conf_.betweenPoints_x * conf_.nPoints_x; ++j) // GET back
    {
      set_on_cap(1, --pos_.x);
      sleep_us(conf_.delayB);
    }
    afc.clear();
    afc="code50";
    for (size_t j = 0; j < vector_z.size(); j++)     // send info
    {
      afc+=','+std::to_string(vector_z[j] ); //231025
      if (conf_.flag == 2)  //231030
      {
        afc+=','+std::to_string(vector_z[j] )+','+std::to_string(other_info[j]); //231025
      };    
    }
      afc+="\n";
      std::cout<<afc;
      sleep_ms(100); //add 231107
      afc.clear();
      vector_z.clear();
      other_info.clear();
    if (CONFIG_UPDATE) //add mf 231101
    {
        CONFIG_UPDATE = false;
        red();
        conf_.delayF=vector[1];
        conf_.delayB=vector[2];
        set_io_value(2,vector[3]); //gain PID
        dark();
    }
    if (STOP_ALL)                     // is need to stop
    {
   /*   blue();
      STOP_ALL = false;
  //    stop_scan(); //????  231102
      MICRO_SCAN = false;
      green(); 
   */
      break;
    //  return; ///??? 231102
    }
    for (uint16_t j = 0; j < conf_.betweenPoints_y; ++j) // go next line
    {
      set_on_cap(2, ++pos_.y);
      sleep_us(conf_.delayF);
    }
    activateBlue();
    sleep_ms(100);
    activateDark();
  }//i
//  blue();
  STOP_ALL = false;
  SCANNING = false;
  if (!flgVirtual)  stop_scan();
  sleep_ms(200); //231030!!!
  while(not TheadDone) {sleep_ms(50); }
  TheadDone = false;    
  std::cout<<"end\n"; //add end scan 231025
  activateDark();
// green();
}

void Scanner::start_scan()
{
  start_scan(pos_);
}

void Scanner::stop_scan()
{
  move_to(prev_point, 10);
}

void Scanner::update(const Config &config)
{
  conf_ = config;
}

void Scanner::move_to(const Point &point, uint32_t delay)
{

  while (pos_.x < point.x)
  {
    set_on_cap(1, ++pos_.x);
    sleep_us(delay);
  }
  while (pos_.x > point.x)
  {
    set_on_cap(1, --pos_.x);
    sleep_us(delay);
  }
  while (pos_.y < point.y)
  {
    set_on_cap(2, ++pos_.y);
    sleep_us(delay);
  }
  while (pos_.y > point.y)
  {
    set_on_cap(2, --pos_.y);
    sleep_us(delay);
  }
  std::cout << "end\n";
}
void Scanner::moveLinearDriverUntilStop(int lid_name, int f, int p, int n, int dir, int gtmax,int gtmin) // n-nsteps
{
   //uint32_t SET_POINT;
  uint16_t GATE_Z_MAX, GATE_Z_MIN;
  int8_t   status;
  int16_t  Z0=32000;
  const int  none = 30;
  const int    ok = 3;
  const int touch = 2;

  int ln; //>0
  int ldir;

  ln=n;  ///????? sign abs?
  ldir=dir;
  GATE_Z_MAX = gtmax;
  GATE_Z_MIN = gtmin;
 
  red();
  if (flgVirtual) //add mf 
  {
    ZValue=Z0;
    SignalValue=Z0;
  }

  if (lid_name == 90 || lid_name == 95) //X,Y
  {
    while (LID_UNTIL_STOP)
    {  
     if (POSXYZ_CONFIG_UPDATE)
     { 
       green();
       sleep_ms(100);
       dark();
       ln         = vector[1]; // with sign 
       GATE_Z_MAX = vector[2];
       GATE_Z_MIN = vector[3];      
       ldir       = 0;
       if (ln>0) ldir=1;
       ln=abs(ln);
       POSXYZ_CONFIG_UPDATE = false;
     }
      status=none;
      if (!flgVirtual) //add mf
      {
        linearDriver.activate(lid_name, f, p, ln, ldir);
      }
      else
      {

      }
      afc.clear(); //231025
      afc="code"+std::to_string(lid_name)+','+std::to_string(status)+','+std::to_string(ZValue)+','+std::to_string(SignalValue) // Zval,SigVal??
      //+','+std::to_string(ln)
      +"\n";
      std::cout <<afc;
      sleep_ms(100); //231104 
      afc.clear();
    }    
  }
  if (lid_name == 99) //Z
  {
     status=none;
    while (LID_UNTIL_STOP)
    {  
      Z_STATE = true; 
      if (POSXYZ_CONFIG_UPDATE)
      {
       green();
       sleep_ms(100);
       dark();
       ln         = vector[1];
       GATE_Z_MAX = vector[2];
       GATE_Z_MIN = vector[3];
       ldir=0;
       if (ln>0) ldir=1;
       POSXYZ_CONFIG_UPDATE = false;
      }
      status=none;
      if (!flgVirtual) //add mf
      {
         getValuesFromAdc();   //test
         ZValue=(int16_t)getValuesFromAdc()[1];
         SignalValue=(int16_t)getValuesFromAdc()[0];
        // check if z > <
       if(ZValue<GATE_Z_MIN)
       {
         status=touch;
         break;
       }
       if(ZValue<GATE_Z_MAX)
       {
         status=ok;
         break;
       }  
       linearDriver.activate(lid_name, f, p, ln, ldir);
      }
      else
      {
       if (ldir==1) {ZValue-=ln;}
       else 
       {
        if ( ZValue<(Z0-ln)){ ZValue+=ln;}
                       else { ZValue=Z0; }
       }                 
       // check if z > <
       if(ZValue<GATE_Z_MIN)
       {
         status=touch;
         break;
       }
       if(ZValue<GATE_Z_MAX)
       {
         status=ok;
         break;
       }  
      }
      afc.clear();
      afc="code"+std::to_string(lid_name)+','+std::to_string(status)+','+std::to_string(ZValue)+','+std::to_string(SignalValue)+"\n";
      std::cout <<afc;
      sleep_ms(200); //need to adjust
      afc.clear();
    }
  }
      afc.clear();
      afc="code"+std::to_string(lid_name)+','+std::to_string(status)+','+std::to_string(ZValue)+','+std::to_string(SignalValue)
     // +','+std::to_string(GATE_Z_MIN)+','+std::to_string(GATE_Z_MAX)
      +"\n";
      std::cout <<afc;
      afc.clear();
      sleep_ms(200); //need to adjust
      while(not TheadDone) {sleep_ms(50); }
      TheadDone = false;    
      LID_UNTIL_STOP=false; 
      std::cout <<"end\n";   
    //  sleep_ms(100); //need to adjust
}

void Scanner::approacphm(const int16_t *const data) //uint16_t
{
 // const int waitsteps = 40;
  const int stopdone=5;
  const int none = 30;
  uint32_t SIGNAL,Z0, Z;
  uint32_t SET_POINT, GATE_Z_MAX, GATE_Z_MIN;
  int32_t  GAIN, NSTEPS;
  uint32_t INTDELAY, SCANNERDECAY;

  // SET VALUE FROM RX_CORE
    SET_POINT  = data[0];
    GATE_Z_MAX = data[1];
    GATE_Z_MIN = data[2];
        NSTEPS = data[3];
      INTDELAY = data[4];
          GAIN = data[5];
  SCANNERDECAY = data[6];
//  freq = data[7];
 // scv = data[8];
  afc.clear(); 
  afc= "debug," ;
  
    for (size_t j = 0; j < 7; j++)     // send info
    {
     afc+=','+ std::to_string(data[j]);
    }
    afc+=afc+"\n";  
  std::cout << afc;
  afc.clear();
  sleep_ms(200);

  dac8563.writeA(SET_POINT);
  std::vector<uint32_t> buf_params;
  buf_params.reserve(7);
  for (int i = 0; i < 7; ++i)
    buf_params.push_back(data[i]);

 if (!flgVirtual) //add mf
  {
    getValuesFromAdc();  
    uint16_t *ptr = getValuesFromAdc();
    SIGNAL = (int16_t) ptr[0];
    Z = (int16_t) ptr[1];
  }
  else
  { 
     SIGNAL = 32000;
         Z0 = 32000;
         Z  = Z0;
  }  

  std::vector<uint16_t> buf_status;
  buf_status.push_back(none);
  buf_status.push_back(Z);
  buf_status.push_back(SIGNAL);
  afc.clear(); 
  afc= "code75," + std::to_string(APPROACH)+',' +std::to_string(buf_status[1]) +',' +std::to_string(buf_status[2])+"\n";  
  std::cout << afc;
  afc.clear();
  sleep_ms(100);

  while (true)
  {
    buf_status[0] = none;
    if (!APPROACH)
    {
      red();
      sleep_ms(500);
      dark();
      buf_status[0] = stopdone;
      buf_status[1] = Z;
      buf_status[2] = SIGNAL;
      break;
    }
     if (APPROACH_CONFIG_UPDATE)
    {
      green();
      sleep_ms(100);
      dark();
     // log("config updated\n",flgDebugLevel);
      APPROACH_CONFIG_UPDATE = false; //add 231025 ???? Ilia
      SET_POINT  = vector[1];
      GATE_Z_MAX = vector[2];
      GATE_Z_MIN = vector[3];
      NSTEPS     = vector[4];
      INTDELAY   = vector[5];
      GAIN       = vector[6];
    SCANNERDECAY = vector[7]; 
    }

    dac8563.writeA(SET_POINT);
    set_io_value(2, GAIN);   
    sleep_ms(INTDELAY);

   if (!flgVirtual) 
   {
   //  getValuesFromAdc(); 
     /*uint16_t *ptr = getValuesFromAdc();  
     Z = (int16_t) ptr[1]; 
     SIGNAL = (int16_t) ptr[0];
     */
     Z=(int16_t)getValuesFromAdc()[1];
     SIGNAL=(int16_t)getValuesFromAdc()[0];
     if (flgDebugLevel<=DEBUG_LEVEL) ;//log("Z = " + std::to_string(Z) + '\n',flgDebugLevel);
   }
   else
   { 
     SIGNAL = 32000;
     if (NSTEPS >= 0)
     {
      Z = Z-500;
     } 
     else
     {       
       if ((Z0-Z)>0){Z+=500;}
       else {Z=Z0;}
     }  
     buf_status[1] = Z;
     buf_status[2] = SIGNAL;
   }  
    if (NSTEPS >= 0)
    {
      if (Z <= GATE_Z_MIN)
      {
        buf_status[0] = 2; // touch
        buf_status[1] = Z;
        buf_status[2] = SIGNAL;
        break;
      }
      if (Z <= GATE_Z_MAX)
      {
        int k = 0;
        for (int i = 0; i < 10; ++i)
        {
          if (!flgVirtual) 
           {
            //  getValuesFromAdc();
            //  auto pt = getValuesFromAdc();
             Z=(int16_t)getValuesFromAdc()[1];
           }
          if (Z <= GATE_Z_MAX)    k++;
          if (k == 3)
          {
           buf_status[0] = 3; // ok
           buf_status[1] = Z;
           buf_status[2] = SIGNAL;
           if (flgDebugLevel<=DEBUG_LEVEL);// log("success\n",flgDebugLevel);
           break;
          }
          sleep_ms(10);
        }
        if (buf_status[0] == 3) {break;}
      }
    } //NSTEPS>0
    if (NSTEPS < 0)
    {
    }
      afc.clear(); 
      afc= "code75," + std::to_string(buf_status[0])+',' +std::to_string(buf_status[1]) +',' +std::to_string(buf_status[2])+"\n";  
      std::cout << afc;
      afc.clear();
    if (!flgVirtual) 
    {
     retract(); //io3_1.enable(); // втянуть 231025
     sleep_ms(SCANNERDECAY);
     linearDriver.activate(99, 5000, 750, std::abs(NSTEPS), NSTEPS > 0);
     protract(); //io3_1.disable(); //вытянуть
    }
  }
      afc.clear(); 
      afc= "code75," + std::to_string(buf_status[0])+',' +std::to_string(buf_status[1]) +',' +std::to_string(buf_status[2])+"\n";  
      std::cout << afc;
      afc.clear();   
      APPROACH = false; 
      protract();// io3_1.disable();//вытянуть
      sleep_ms(INTDELAY); 
/*
      afc="done\n";
      std::cout<<afc;
      afc.clear();
*/
      while(not TheadDone) {sleep_ms(50); }
      TheadDone = false;
       std::cout << "end\n";
}
void Scanner::start_frqscan()
{
      static uint16_t inBuf[5]; // n, start_freq, step, channel, delay
      int16_t signalvalue; 
      int16_t res_freq=10000;
      int16_t a=10000;
 //   RESONANCE_ACTIVE = true;
      int16_t scan_index =0;
      int16_t current_freq=0;
      afc.clear();
      afc="debug";
      for (int j = 0; j < 5; ++j)
      {
        inBuf[j] = vector[1 + j];
        afc+=','+std::to_string(inBuf[j]);
      }
   /*  afc+=','+std::to_string(flgVirtual)+"\n";
      std::cout<<afc;
      sleep_ms(100);
    */  
      afc.clear();
  //     RESONANCE_ACTIVE=true;
      current_channel = inBuf[3] - 1;  //ampl=0;
      afc="code25";
      while ((scan_index++<inBuf[0]))// && !RESONANCE_STOP)
      {
        if (!flgVirtual) 
        {  
         set_freq(inBuf[1]);
         sleep_ms(inBuf[4]);
         afc +=','+ std::to_string(inBuf[1]) + ',' + std::to_string((int16_t)getValuesFromAdc()[current_channel]);//+ ',';
        } 
        else
        {
         current_freq=inBuf[1];//+inBuf[2]*scan_index;
         sleep_ms(inBuf[4]);
         signalvalue=std::round(a*(pow(M_E,-pow((current_freq-res_freq),2)/1000000)));
         afc +=','+std::to_string(current_freq) + ',' + std::to_string(signalvalue);//+',';
        }
       sleep_ms(10);
        inBuf[1] += inBuf[2];  
    //   scan_index++;     
     //   if (scan_index>=inBuf[0]) RESONANCE_ACTIVE=false;
        //std::cout << afc << '\n';   
      }
        std::cout << afc << '\n';
        sleep_ms(200);
        afc.clear();
      //  scan_index = current_freq = 0;
        current_channel = -1;
   //   RESONANCE_ACTIVE = false;
        RESONANCE = RESONANCE_STOP = false;        
}

