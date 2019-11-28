/*************************************************************************
* --------------------------- P4 - Timer ---------------------------------
* Created by: Tejesh Kalicharan, Nimmagadda (tnimmag@iu.edu id:2000432930)
* Team mate: Vijaya Krishna, Gopalakrishnan Partha
* Date: 13th Mar, 2019
*************************************************************************/

// A necessary header inclusion
    #include "application.h" 
    #include "nrfx.h"
    #include "nrfx_timer.h"
    // Defining 2 int variables which references the D0 and D7 pins.
    int ledD7 = D7;
	  int toggle=0;
    const nrfx_timer_t TIMER_LED = NRFX_TIMER_INSTANCE(3);

    const nrfx_timer_config_t TIMER_CFG = {
	.frequency = NRF_TIMER_FREQ_1MHz,
	.mode = NRF_TIMER_MODE_TIMER,
	.bit_width = NRF_TIMER_BIT_WIDTH_32,
	.interrupt_priority = NRFX_TIMER_DEFAULT_CONFIG_IRQ_PRIORITY,
	.p_context = NULL
    };

    void timer_event_handler(nrf_timer_event_t event_type, void *p_context)
    {
	    //if(event_type == NRF_TIMER_EVENT_COMPARE0){
	    if(toggle==0){
		    digitalWrite(ledD7, HIGH); toggle=1;
	    }
	    else if(toggle==1){
		    digitalWrite(ledD7, LOW); toggle=0;
	    }
	
    }

    // This will run the device in semi-automatic mode, and not connect to the
    // wifi, Particle cloud by default.
    //SYSTEM_MODE(SEMI_AUTOMATIC);

    nrfx_err_t init_msg = nrfx_timer_init(&TIMER_LED, &TIMER_CFG, timer_event_handler);
	  uint32_t time_ms = 500;
	  uint32_t time_ticks = nrfx_timer_ms_to_ticks(&TIMER_LED, time_ms);
	

    // setup() runs once, when the device is first turned on.
    void setup() {
        // Put initialization like pinMode and begin functions here.
	      nrfx_timer_extended_compare(&TIMER_LED, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
	      nrfx_timer_enable(&TIMER_LED);
        pinMode(ledD7, OUTPUT);
    }

    // loop() runs over and over again, as quickly as it can execute.
    void loop() {
	
        
    }
