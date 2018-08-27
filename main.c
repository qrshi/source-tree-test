#include "macro.h"

#define TEST3(name,ret) unsigned char name=ret
#define TEST2(name,ret) TEST3(name,ret)
#define TEST1(_name, _priority, _var) TEST2(CONCAT_2(_name, _priority),_var)

//BLE_LBS_DEF(m_lbs);

void ble_lbs_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context)
{
 
}


//static uint16_t g_dat_t __attribute__ ((section(".text"))) __attribute__((used))  ;
static uint16_t g_dat_t;


static unsigned char dat;
static char *pt=0;
int main(void)
{
 

static ble_lbs_t m_lbs; 
	

static nrf_sdh_ble_evt_observer_t m_lbs_obs __attribute__ ((section("sdh_ble_observers2"))) __attribute__((used))=
{
	    .handler   = ble_lbs_on_ble_evt,
		.p_context = &m_lbs,  
	} ;
	
	TEST1(namep1, part2, 3);
	
	 pt=STRINGIFY(sdh_ble_observers2);
	
	dat =MARCRO_USER_TEST;
	
	while(1){
  namep1part2++;
		
		dat++;
	};
	
	return 0;
}

