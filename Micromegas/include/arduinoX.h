#ifndef _ARDUINOY_H_
#define _ARDUINOY_H_

#include <cstdint>

#include "rs232ard.h"

enum class arduinoDIO { OUTPUT=0, INPUT=1 };

enum class arduinoCMD
 {
  DIGITAL_SETUP='a', ANALOG_IN='b', DIGITAL_OUT='c',
  PULSE_OUT='d', DIGITAL_IN='e', DELAYED_PULSE='f'
 };

double analogcalibration[16] = { 1, 1, 1, 1, 3482.0/3455.0, 2206.0/2182.0, 2288.0/2267.0, 1, 1, 1, 4240.0/4201.0, 6310.0/6208.0, 4272.0/4246.0, 5928.0/5819.0, 6130.0/6024.0, 1 };

class arduinoX : rs232ard
 {
  public:
	static arduinoX* create ( std::string tty );
	static void cleanup();

  protected:
	arduinoX( std::string tty );
	virtual ~arduinoX() {}

  public:
	void digitalSetup( int nchans, int* chans, arduinoDIO* way );
	void digitalOutput( int chan, bool val );
	int analogInput( int chan );
	bool digitalInput( int chan );
	void pulseOutput( int chan );
	void delayedPulse( int chan, int delay, int duration );

	void setPhysScale( uint16_t chan, double RFS, double RZS, double EFS, double EZS);

	double phyVal( uint16_t chan, int adcn )
	 {
	  double v=analogcalibration[chan]*10000.0*double(adcn)/1024.0;
	  return v;
	 }
	double getVal( uint16_t chan )
	 {
	  double v=analogcalibration[chan]*10000.0*double(this->analogInput(chan))/1024.0;
	  return v;
	 }
	double getPhyVal( uint16_t chan )
	 {
	  return m_slope[chan]*this->getVal(chan) + m_intcpt[chan];
	 }

  private:
	double m_RFS[16], m_RZS[16], m_EFS[16], m_EZS[16];
	double m_slope[16], m_intcpt[16];
	static arduinoX* m_arduino_inst;
 };

#endif // _ARDUINOY_H_
