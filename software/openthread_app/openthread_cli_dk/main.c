/** @file
 *
 * @defgroup cli_example_main main.c
 * @{
 * @ingroup cli_example
 * @brief An example presenting OpenThread CLI.
 *
 */

#include <math.h>
#include "arm_math.h"

#include "app_scheduler.h"
#include "app_timer.h"
#include "bsp_thread.h"
#include "nrf_log_ctrl.h"
#include "nrf_log.h"
#include "nrf_log_default_backends.h"
#include "nrf_drv_clock.h"

#include "thread_utils.h"

#include <openthread/thread.h>

#define SCHED_QUEUE_SIZE      32                              /**< Maximum number of events in the scheduler queue. */
#define SCHED_EVENT_DATA_SIZE APP_TIMER_SCHED_EVENT_DATA_SIZE /**< Maximum app_scheduler event size. */

// ---------------- ADDED BY EDWIN

//arm_gaussian_naive_bayes_instance_f32 S;

//#define NB_OF_CLASSES 3
//#define VECTOR_DIMENSION 6

// -----------------

const int buffer_size = 4;

int8_t beacon_buf1[4] = {0};
int8_t beacon_buf2[4] = {0};
int8_t beacon_buf3[4] = {0};
uint8_t counter1 = 0;
uint8_t counter2 = 0;
uint8_t counter3 = 0;

#define MEAN 1


/**@brief Function for predicting location.
 */
static void get_location() // TODO: add input params : x,y,z coordinate in float
{
  float32_t b1=0.0;
  float32_t b2=0.0;
  float32_t b3=0.0;

          NRF_LOG_INFO("b1 = % .f", b1);
        NRF_LOG_INFO("b2 = % .f", b2);
        NRF_LOG_INFO("b3 = % .f", b3);
  // -------------------------------------------------------------------------------------------------
  #ifdef MEAN
    for(int i=0;i<buffer_size;i++){
      b1 = b1 + (float32_t)beacon_buf1[i];
      b2 = b2 + (float32_t)beacon_buf2[i];
      b3 = b3 + (float32_t)beacon_buf3[i];
    }
    b1 = b1 / (float32_t)buffer_size;
    b2 = b2/ (float32_t)buffer_size;
    b3 = b3/ (float32_t)buffer_size;
  #endif

  // -------------------------------------------------------------------------------------------------

  float32_t in[3];
  //const float32_t theta[NB_OF_CLASSES*VECTOR_DIMENSION] = {
  //-52.906172839506176, -72.64444444444445, -80.73827160493828,-51.79925650557621,
  // -74.02973977695167, -84.38289962825279, -54.23622047244095, -71.48818897637796, 
  // -80.89763779527559, -74.36363636363636, -63.83116883116883, -78.83116883116882,-70.55833333333334,
  //  -54.925, -74.94166666666666, -69.1640625, -60.4765625, -79.859375
  //}; /**< Mean values for the Gaussians */

  //const float32_t sigma[NB_OF_CLASSES*VECTOR_DIMENSION] = {
  // 24.39613483391136, 7.666172937858941, 31.106806987546573, 9.885352705420123, 2.3634279116776375, 17.1507857425534,
  //  2.164672427697435, 20.391592881538326, 0.5328291640109084, 31.60802843365385, 1.880587043874784, 23.38708055036829,
  //   21.229930653908333, 11.586041765019429, 3.538263987241654, 4.809021094446527, 8.311950781946527, 2.683349707727777
  //}; /**< Variances for the Gaussians */

  //const float32_t classPriors[6] = {
  // 0.33665835411471323f, 0.22360764754779716f, 0.1055694098088113f, 0.12801330008312553f, 0.09975062344139651f, 
  // 0.10640066500415628f
  //}; /**< Class prior probabilities */

  //float32_t result[NB_OF_CLASSES];
  //  float32_t maxProba;
  //  uint32_t index;
  
  //  S.vectorDimension = VECTOR_DIMENSION; 
  //  S.numberOfClasses = NB_OF_CLASSES; 
  //  S.theta = theta;          
  //  S.sigma = sigma;         
  //  S.classPriors = classPriors;    
  //  S.epsilon=4.328939296523643e-09f; 

    in[0] = b1;
    in[1] = b2;
    in[2]= b3;

    //index = arm_gaussian_naive_bayes_predict_f32(&S, in, result);

    //maxProba = result[index];

    NRF_LOG_ERROR( "in[0] " NRF_LOG_FLOAT_MARKER "\r", NRF_LOG_FLOAT(in[0]));
    NRF_LOG_ERROR( "in[1] " NRF_LOG_FLOAT_MARKER "\r", NRF_LOG_FLOAT(in[1]));
    NRF_LOG_ERROR( "in[2] " NRF_LOG_FLOAT_MARKER "\r", NRF_LOG_FLOAT(in[2]));

    //NRF_LOG_INFO("in_1 = %f", in[0]);
    //NRF_LOG_INFO("in_2 = %f", in[0]);
    //NRF_LOG_INFO("in_3 = %f", in[0]);

    //NRF_LOG_ERROR( "Class = " NRF_LOG_FLOAT_MARKER "\r", NRF_LOG_FLOAT(result[index]));
 
}

/***************************************************************************************************
 * @section TIMER!!!!
 **************************************************************************************************/
APP_TIMER_DEF(m_repeated_timer_id);

static void lfclk_request(void)
{
  ret_code_t err_code = nrf_drv_clock_init();
  APP_ERROR_CHECK(err_code);
  nrf_drv_clock_lfclk_request(NULL);
}

static void start_timer()
{
  ret_code_t err_code;
  static uint32_t timeout=0;

  err_code = app_timer_start(m_repeated_timer_id, APP_TIMER_TICKS(100), NULL);
  APP_ERROR_CHECK(err_code);
}

static void repeated_timer_handler(void * p_context)
{
  // if there is a connection, do the rest, if not skip
 
  // do something
  otError ot_err_code;
  otNeighborInfoIterator aiterator = OT_NEIGHBOR_INFO_ITERATOR_INIT;
  otNeighborInfo my_neighbor_info;

  ot_err_code = otThreadGetNextNeighborInfo(thread_ot_instance_get(), &aiterator, &my_neighbor_info);
  NRF_LOG_INFO("last rssi from %d @ %d: %d", my_neighbor_info.mRloc16, my_neighbor_info.mAge, my_neighbor_info.mLastRssi);
  beacon_buf1[counter1%4] = my_neighbor_info.mLastRssi;
  
  aiterator++;
  ot_err_code = otThreadGetNextNeighborInfo(thread_ot_instance_get(), &aiterator, &my_neighbor_info);
  NRF_LOG_INFO("last rssi from %d @ %d: %d", my_neighbor_info.mRloc16, my_neighbor_info.mAge, my_neighbor_info.mLastRssi);
  beacon_buf2[counter2%4] = my_neighbor_info.mLastRssi;

  aiterator++;
  ot_err_code = otThreadGetNextNeighborInfo(thread_ot_instance_get(), &aiterator, &my_neighbor_info);
  NRF_LOG_INFO("last rssi from %d @ %d: %d", my_neighbor_info.mRloc16, my_neighbor_info.mAge, my_neighbor_info.mLastRssi);
  beacon_buf3[counter3%4] = my_neighbor_info.mLastRssi;

  get_location();

}

static void create_timers()
{
  ret_code_t err_code;

  //create timers
  err_code = app_timer_create(&m_repeated_timer_id,
                              APP_TIMER_MODE_REPEATED,
                              repeated_timer_handler);
  APP_ERROR_CHECK(err_code);
}

/***************************************************************************************************
 * @section State
 **************************************************************************************************/

static void thread_state_changed_callback(uint32_t flags, void * p_context)
{
    NRF_LOG_INFO("State changed! Flags: 0x%08x Current role: %d\r\n",
                 flags, otThreadGetDeviceRole(p_context));
}

/***************************************************************************************************
 * @section Initialization
 **************************************************************************************************/

/**@brief Function for initializing the Application Timer Module.
 */
static void timer_init(void)
{
    uint32_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the LEDs.
 */
static void leds_init(void)
{
    LEDS_CONFIGURE(LEDS_MASK);
    LEDS_OFF(LEDS_MASK);
}


/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


/**@brief Function for initializing the Thread Stack.
 */
static void thread_instance_init(void)
{
    thread_configuration_t thread_configuration =
    {
        .radio_mode        = THREAD_RADIO_MODE_RX_ON_WHEN_IDLE,
        .autocommissioning = false,
        .autostart_disable = true,
        .poll_period       = 20,
        .default_child_timeout = 1,
    };

    thread_init(&thread_configuration);
    thread_cli_init();
    thread_state_changed_callback_set(thread_state_changed_callback);

    uint32_t err_code = bsp_thread_init(thread_ot_instance_get());
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for deinitializing the Thread Stack.
 *
 */
static void thread_instance_finalize(void)
{
    bsp_thread_deinit(thread_ot_instance_get());
    thread_soft_deinit();
}


/**@brief Function for initializing scheduler module.
 */
static void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}


/***************************************************************************************************
 * @section Main
 **************************************************************************************************/

int main(int argc, char *argv[])
{
    log_init();
    scheduler_init();
    lfclk_request();
    timer_init();
    create_timers();
    leds_init();
    start_timer();

    uint32_t err_code = bsp_init(BSP_INIT_LEDS, NULL);
    APP_ERROR_CHECK(err_code);

    while (true)
    {
        thread_instance_init();

        while (!thread_soft_reset_was_requested())
        {
            thread_process();
            app_sched_execute();

            if (NRF_LOG_PROCESS() == false)
            {
                thread_sleep();
            }
        }

        thread_instance_finalize();
    }
}

/**
 *@}
 **/
