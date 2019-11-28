/*****************************************************************
* Created by: Tejesh Kalicharan Nimmagadda
* tnimmag@iu.edu id: 2000432920
* Date: 1st April, 2019
*****************************************************************/
#include "nrfx.h"
#include "nrfx_ppi.h"
#include "nrfx_saadc.h"
#include "nrfx_timer.h"

#include "application.h"

#define TIMER_INSTANCE 3
#define TIMER_TICK_US 63 // this shoud contain the proper time for the timer
#define BUF0_SIZE 250

IPAddress remoteIP(18,218,51,247);
uint32_t remote_port = 44444;

UDP udp;

int button_ctrl = 0;
int process_buf = 0;
int read_pin = A4;

int gbufindex = 0;
int16_t GlobalBuf [16000];

int16_t BUF0 [BUF0_SIZE];
int16_t BUF1 [BUF0_SIZE];


const nrfx_timer_t TIMER_LED = NRFX_TIMER_INSTANCE(TIMER_INSTANCE);

void timer_event_handler(nrf_timer_event_t event_type, void *p_context){

}


uint32_t timer_init()
{

  uint32_t time_us = TIMER_TICK_US; // define the appropiate time for the timer triggers
  uint32_t err_code = NRF_SUCCESS;
  uint32_t time_ticks;

  const nrfx_timer_config_t TIMER_CFG = {
					 .frequency  = NRF_TIMER_FREQ_1MHz,
					 .mode       = NRF_TIMER_MODE_TIMER,
					 .bit_width  = NRF_TIMER_BIT_WIDTH_32,
					 .interrupt_priority = NRFX_TIMER_DEFAULT_CONFIG_IRQ_PRIORITY,
					 .p_context  = NULL
  };

  err_code = nrfx_timer_init(&TIMER_LED, &TIMER_CFG, timer_event_handler);

  time_ticks =  nrfx_timer_us_to_ticks( &TIMER_LED, time_us);

  nrfx_timer_extended_compare (&TIMER_LED, NRF_TIMER_CC_CHANNEL0, time_ticks,
			       NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
  nrfx_timer_enable(&TIMER_LED);

  return nrfx_timer_compare_event_address_get( &TIMER_LED, NRF_TIMER_CC_CHANNEL0);
}

/* function takes 2 arguments the address of the configured timer (timer_addr), */
/*   the address of the configured ADC (adc_addr) and configures the ADC sampling */
/*   to be driven by the timer output */
void ppi_init( const uint32_t timer_addr, const uint32_t adc_addr)
{
  uint32_t err_code = NRF_SUCCESS;
  nrf_ppi_channel_t ppi_channel;

  err_code = nrfx_ppi_channel_alloc (&ppi_channel);
  SPARK_ASSERT( err_code == NRF_SUCCESS);
      
  err_code = nrfx_ppi_channel_assign (ppi_channel, timer_addr, adc_addr);
  SPARK_ASSERT( err_code == NRF_SUCCESS);
  
  err_code =nrfx_ppi_channel_enable (ppi_channel);
  SPARK_ASSERT( err_code == NRF_SUCCESS);
}

void adc_handler( const nrfx_saadc_evt_t * event_type)
{
  if (event_type->type == NRFX_SAADC_EVT_DONE)
    {
      nrf_saadc_value_t * buf = event_type->data.done.p_buffer;
      uint16_t buf_sz = event_type->data.done.size;
      // buf contains the converted samples
      // buf_sz indicates the size.
      	for(int i=0; i<buf_sz; i++){
		GlobalBuf[gbufindex++] = buf[i];
	}
	if(gbufindex > 15999){
		 gbufindex = 0;
		 process_buf = 1;
	}
	nrfx_saadc_buffer_convert (event_type->data.done.p_buffer, BUF0_SIZE); 
    } else {
    SPARK_ASSERT(0);
  }
}

/* function for configuring the adc */
/* takes the input pin as input argument */
uint32_t adc_init( const uint16_t pin)
{
  uint32_t err_code = NRF_SUCCESS;
  
  const nrfx_saadc_config_t saadc_config =
    {
     .resolution         = NRF_SAADC_RESOLUTION_12BIT,
     .oversample         = NRF_SAADC_OVERSAMPLE_DISABLED,
     .interrupt_priority = NRFX_SAADC_CONFIG_IRQ_PRIORITY
    };

  NRF5x_Pin_Info *PIN_MAP = HAL_Pin_Map();

  nrf_saadc_input_t nrf_adc_channel = NRF_SAADC_INPUT_AIN0;
  switch (PIN_MAP[pin].adc_channel)
    {
    case 0: nrf_adc_channel = NRF_SAADC_INPUT_AIN0; break;
    case 1: nrf_adc_channel = NRF_SAADC_INPUT_AIN1; break;
    case 2: nrf_adc_channel = NRF_SAADC_INPUT_AIN2; break;
    case 3: nrf_adc_channel = NRF_SAADC_INPUT_AIN3; break;
    case 4: nrf_adc_channel = NRF_SAADC_INPUT_AIN4; break;
    case 5: nrf_adc_channel = NRF_SAADC_INPUT_AIN5; break;
    case 6: nrf_adc_channel = NRF_SAADC_INPUT_AIN6; break;
    case 7: nrf_adc_channel = NRF_SAADC_INPUT_AIN7; break;
    default: SPARK_ASSERT(0);
    }

  //make sure pin is configured for Analog Input
  if (PIN_MAP[pin].pin_func != PF_NONE && PIN_MAP[pin].pin_func != PF_DIO) {
    SPARK_ASSERT(0);
  }

  //Single ended, negative input to ADC shorted to GND.
  nrf_saadc_channel_config_t channel_config = {
					       .resistor_p = NRF_SAADC_RESISTOR_DISABLED,
					       .resistor_n = NRF_SAADC_RESISTOR_DISABLED,
					       .gain       = NRF_SAADC_GAIN1_4,
					       .reference  = NRF_SAADC_REFERENCE_VDD4,
					       .acq_time   = NRF_SAADC_ACQTIME_10US,
					       .mode       = NRF_SAADC_MODE_SINGLE_ENDED,
					       .burst      = NRF_SAADC_BURST_DISABLED,
					       .pin_p      = (nrf_saadc_input_t)(nrf_adc_channel),
					       .pin_n      = NRF_SAADC_INPUT_DISABLED
  };

  //setup adc itself
  err_code = nrfx_saadc_init (&saadc_config, adc_handler);
  SPARK_ASSERT( err_code == NRF_SUCCESS);

  //setup channel
  err_code = nrfx_saadc_channel_init(PIN_MAP[pin].adc_channel, &channel_config);
  SPARK_ASSERT( err_code == NRF_SUCCESS);

  // fix this to set up double buffering as opposed to a
  // single sampling buffer now
  err_code = nrfx_saadc_buffer_convert (BUF0, BUF0_SIZE);
  SPARK_ASSERT( err_code == NRF_SUCCESS);

  err_code = nrfx_saadc_buffer_convert (BUF1, BUF0_SIZE);
  SPARK_ASSERT( err_code == NRF_SUCCESS);
 
  nrf_saadc_enable();
  // timer starts the adc
  return nrfx_saadc_sample_task_get();
}

void button_handler(system_event_t event, int duration, void* )
{
    if (!duration) { // just pressed
	button_ctrl = 1-button_ctrl;
    }
    else { // just released
        //RGB.control(false);
    }
}

//Reference: https://devzone.nordicsemi.com/f/nordic-q-a/32982/can-t-stop-saadc
void disabiling()
{
	nrfx_timer_disable(&TIMER_LED);
	nrf_saadc_disable();
}

void enabling()
{
	nrfx_timer_enable(&TIMER_LED);
	nrf_saadc_enable();
}
void setup()
{
	Serial.begin(9600);
	Serial.printlnf("Connected adcdma\n");
	ppi_init(timer_init(), adc_init(read_pin));
	udp.begin(8888);
	System.on(button_status, button_handler);
}

void loop()
{
	if(button_ctrl == 1)
	{
		Serial.printlnf("Mode button on\n");
		if(process_buf == 1){
			disabiling();
			for(int j=0; j<16000; j++) { //GlobalBuf[j] = ((GlobalBuf[j]*3.3)/4095);
				 Serial.printf("%ld, ",GlobalBuf[j]); 
			}
			udp.sendPacket((byte*)GlobalBuf, sizeof(GlobalBuf), remoteIP, remote_port); 
			process_buf = 0;
			enabling();
		}
	}
	else{ Serial.printlnf("Mode button off\n"); }
}


