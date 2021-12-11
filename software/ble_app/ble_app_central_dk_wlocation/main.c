/**tested
 * @brief BLE LED Button Service central and client application main file.
 *
 * This example can be a central for up to 8 peripherals.
 * The peripheral is called ble_app_blinky and can be found in the ble_peripheral
 * folder.
 */
#include <math.h>
// #include <stdio.h>
#include "arm_math.h"


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "app_timer.h"
#include "bsp_btn_ble.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_db_discovery.h"
#include "ble_lbs_c.h"
#include "ble_conn_state.h"
#include "nrf_ble_gatt.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_ble_scan.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf_ringbuf.h"


#define APP_BLE_CONN_CFG_TAG      1                                     /**< Tag that refers to the BLE stack configuration that is set with @ref sd_ble_cfg_set. The default tag is @ref APP_BLE_CONN_CFG_TAG. */
#define APP_BLE_OBSERVER_PRIO     3                                     /**< BLE observer priority of the application. There is no need to modify this value. */

#define CENTRAL_SCANNING_LED      BSP_BOARD_LED_0
#define CENTRAL_CONNECTED_LED     BSP_BOARD_LED_1
#define LEDBUTTON_LED             BSP_BOARD_LED_2                       /**< LED to indicate a change of state of the Button characteristic on the peer. */

#define LEDBUTTON_BUTTON          BSP_BUTTON_0                          /**< Button that writes to the LED characteristic of the peer. */
#define BUTTON_DETECTION_DELAY    APP_TIMER_TICKS(50)                   /**< Delay from a GPIOTE event until a button is reported as pushed (in number of timer ticks). */

// ---------------- ADDED BY EDWIN

arm_gaussian_naive_bayes_instance_f32 S;

#define NB_OF_CLASSES 3
#define VECTOR_DIMENSION 6

// -----------------

NRF_BLE_GATT_DEF(m_gatt);                                               /**< GATT module instance. */
BLE_LBS_C_ARRAY_DEF(m_lbs_c, NRF_SDH_BLE_CENTRAL_LINK_COUNT);           /**< LED button client instances. */
BLE_DB_DISCOVERY_ARRAY_DEF(m_db_disc, NRF_SDH_BLE_CENTRAL_LINK_COUNT);  /**< Database discovery module instances. */
NRF_BLE_SCAN_DEF(m_scan);                                               /**< Scanning Module instance. */
NRF_BLE_GQ_DEF(m_ble_gatt_queue,                                        /**< BLE GATT Queue instance. */
               NRF_SDH_BLE_CENTRAL_LINK_COUNT,
               NRF_BLE_GQ_QUEUE_SIZE);

// -----------------------------------------------------------------------------  ADDED STUFF #1:

#define SCAN_DURATION_LIMITED 3000

static ble_gap_addr_t const m_addr_1 = 
{
  .addr = {0x7b,0xf2,0xc2,0x36,0xf3,0xd6}, //LSB format
  .addr_type = BLE_GAP_ADDR_TYPE_RANDOM_STATIC,
};

static ble_gap_addr_t const m_addr_2 = 
{
  .addr = {0x8b,0xe1,0xe5,0x20,0x43,0xe6}, //LSB format
  .addr_type = BLE_GAP_ADDR_TYPE_RANDOM_STATIC,
};

static ble_gap_addr_t const m_addr_3 = 
{
  .addr = {0xe7,0xad,0xe9,0x03,0x6b,0xd4}, //LSB format
  .addr_type = BLE_GAP_ADDR_TYPE_RANDOM_STATIC,
};

/**< Scan parameters requested for scanning and connection. */
static ble_gap_scan_params_t const m_scan_param =
{
    .active        = 0x01,
    .interval      = NRF_BLE_SCAN_SCAN_INTERVAL,
    .window        = NRF_BLE_SCAN_SCAN_WINDOW, // scan window in 625us units
    .filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL,
    .timeout       = BLE_GAP_SCAN_TIMEOUT_UNLIMITED,
    .scan_phys     = BLE_GAP_PHY_1MBPS,
};

// NRF_RINGBUF_DEF(m_ringbuf_beacon1, 64);
// NRF_RINGBUF_DEF(m_ringbuf_beacon2, 64);
// NRF_RINGBUF_DEF(m_ringbuf_beacon3, 64);

const int buffer_size = 4;

int8_t beacon_buf1[4] = {0};
int8_t beacon_buf2[4] = {0};
int8_t beacon_buf3[4] = {0};
uint8_t counter1 = 0;
uint8_t counter2 = 0;
uint8_t counter3 = 0;

//#define MEDIAN 1
#define MEAN 1

// ---------------------------------------------  ADDED STUFF


/**@brief Function for handling asserts in the SoftDevice.
 *
 * @details This function is called in case of an assert in the SoftDevice.
 *
 * @warning This handler is only an example and is not meant for the final product. You need to analyze
 *          how your product is supposed to react in case of an assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num     Line number of the failing assert call.
 * @param[in] p_file_name  File name of the failing assert call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}


/**@brief Function for handling the LED Button Service Client errors.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void lbs_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}



/**@brief Function for the LEDs initialization.
 *
 * @details Initializes all LEDs used by the application.
 */
static void leds_init(void)
{
    bsp_board_init(BSP_INIT_LEDS);
}

// ------------------------------------------------------------------------ custom filter handler func

static void scan_evt_handler(scan_evt_t const * p_scan_evt)
{
    ret_code_t err_code;
    ble_gap_evt_adv_report_t const * p_adv = 
                   p_scan_evt->params.filter_match.p_adv_report;
    ble_gap_scan_params_t    const * p_scan_param = 
                   p_scan_evt->p_scan_params;

    switch(p_scan_evt->scan_evt_id)
    {
        case NRF_BLE_SCAN_EVT_FILTER_MATCH: 
        {
            //NRF_LOG_INFO("ADDRESS !! %02X:%02X:%02X:%02X:%02X:%02X", p_adv->peer_addr.addr[5],p_adv->peer_addr.addr[4],p_adv->peer_addr.addr[3],p_adv->peer_addr.addr[2],p_adv->peer_addr.addr[1],p_adv->peer_addr.addr[0]);
            //NRF_LOG_INFO("NRF_BLE_SCAN_EVT_FILTER_MATCH !! %d", (int8_t)p_adv->rssi);
            switch(p_adv->peer_addr.addr[5])
            {
              case 0xd6:
              {
                beacon_buf1[counter1%4] = p_adv->rssi;
                counter1++;
                //NRF_LOG_INFO("found beacon1");
                //int8_t* cur_rssi = (int8_t*)&p_adv->rssi;
                //size_t len_in = sizeof(*cur_rssi);
                //err_code = nrf_ringbuf_cpy_put(&m_ringbuf_beacon1, cur_rssi, &len_in);
              } break;
              case 0xe6:
              {
                //NRF_LOG_INFO("found beacon2");
                beacon_buf2[counter2%4] = p_adv->rssi;
                counter2++;
                //int8_t* cur_rssi = (int8_t*)&p_adv->rssi;
                //size_t len_in = sizeof(*cur_rssi);
                //err_code = nrf_ringbuf_cpy_put(&m_ringbuf_beacon2, cur_rssi, &len_in);
              } break;
              case 0xd4:
              {
                //NRF_LOG_INFO("found beacon3");
                beacon_buf3[counter3%4] = p_adv->rssi;
                counter3++;
                //int8_t* cur_rssi = (int8_t*)&p_adv->rssi;
                //size_t len_in = sizeof(*cur_rssi);
                //err_code = nrf_ringbuf_cpy_put(&m_ringbuf_beacon3, cur_rssi, &len_in);
              } break;
            }
            // --------------------------
            //APP_ERROR_CHECK(err_code);
        } break;

        default:
            break;
    }
}

// ------------------------------------------------------------------------ custom filter handler func

/**@brief Function for initializing the scanning and setting the filters.
 */
static void scan_init(void)
{
    ret_code_t          err_code;
    nrf_ble_scan_init_t init_scan;

    memset(&init_scan, 0, sizeof(init_scan));

    init_scan.p_scan_param     = &m_scan_param;
    init_scan.connect_if_match = false;
    init_scan.conn_cfg_tag     = APP_BLE_CONN_CFG_TAG;

    err_code = nrf_ble_scan_init(&m_scan, &init_scan, scan_evt_handler);
    APP_ERROR_CHECK(err_code);

    // ------------------------------------------------------------------------ FILTERING BY ADDRESS: 

    err_code = nrf_ble_scan_filter_set(&m_scan, SCAN_ADDR_FILTER, m_addr_1.addr);
    err_code = nrf_ble_scan_filter_set(&m_scan, SCAN_ADDR_FILTER, m_addr_2.addr);
    err_code = nrf_ble_scan_filter_set(&m_scan, SCAN_ADDR_FILTER, m_addr_3.addr);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_ble_scan_filters_enable(&m_scan, NRF_BLE_SCAN_ADDR_FILTER, false);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for starting scanning. */
static void scan_start(void)
{
    ret_code_t ret;

    //NRF_LOG_INFO("Start scanning for device w address %s.", (uint32_t)m_addr_1.addr);
    NRF_LOG_INFO("Start scanning for device w address %s.", "(uint32_t)m_addr_1.addr)");
    ret = nrf_ble_scan_start(&m_scan);
    APP_ERROR_CHECK(ret);
    // Turn on the LED to signal scanning.
    bsp_board_led_on(CENTRAL_SCANNING_LED);
}


/**@brief Handles events coming from the LED Button central module.
 *
 * @param[in] p_lbs_c     The instance of LBS_C that triggered the event.
 * @param[in] p_lbs_c_evt The LBS_C event.
 */
static void lbs_c_evt_handler(ble_lbs_c_t * p_lbs_c, ble_lbs_c_evt_t * p_lbs_c_evt)
{
    switch (p_lbs_c_evt->evt_type)
    {
        case BLE_LBS_C_EVT_DISCOVERY_COMPLETE:
        {
            ret_code_t err_code;

            NRF_LOG_INFO("LED Button Service discovered on conn_handle 0x%x",
                         p_lbs_c_evt->conn_handle);

            err_code = app_button_enable();
            APP_ERROR_CHECK(err_code);

            // LED Button Service discovered. Enable notification of Button.
            err_code = ble_lbs_c_button_notif_enable(p_lbs_c);
            APP_ERROR_CHECK(err_code);
        } break; // BLE_LBS_C_EVT_DISCOVERY_COMPLETE

        case BLE_LBS_C_EVT_BUTTON_NOTIFICATION:
        {
            NRF_LOG_INFO("Link 0x%x, Button state changed on peer to 0x%x",
                         p_lbs_c_evt->conn_handle,
                         p_lbs_c_evt->params.button.button_state);

            if (p_lbs_c_evt->params.button.button_state)
            {
                bsp_board_led_on(LEDBUTTON_LED);
            }
            else
            {
                bsp_board_led_off(LEDBUTTON_LED);
            }
        } break; // BLE_LBS_C_EVT_BUTTON_NOTIFICATION

        default:
            // No implementation needed.
            break;
    }
}

/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    ret_code_t err_code;

    // For readability.
    ble_gap_evt_t const * p_gap_evt = &p_ble_evt->evt.gap_evt;

    switch (p_ble_evt->header.evt_id)
    {
        
        // Upon connection, check which peripheral is connected, initiate DB
        // discovery, update LEDs status, and resume scanning, if necessary.
        case BLE_GAP_EVT_CONNECTED:
        {
            NRF_LOG_INFO("Connection 0x%x established, starting DB discovery.",
                         p_gap_evt->conn_handle);

            APP_ERROR_CHECK_BOOL(p_gap_evt->conn_handle < NRF_SDH_BLE_CENTRAL_LINK_COUNT);

            err_code = ble_lbs_c_handles_assign(&m_lbs_c[p_gap_evt->conn_handle],
                                                p_gap_evt->conn_handle,
                                                NULL);
            APP_ERROR_CHECK(err_code);

            err_code = ble_db_discovery_start(&m_db_disc[p_gap_evt->conn_handle],
                                              p_gap_evt->conn_handle);
            APP_ERROR_CHECK(err_code);

            // Update LEDs status and check whether it is needed to look for more
            // peripherals to connect to.
            bsp_board_led_on(CENTRAL_CONNECTED_LED);
            if (ble_conn_state_central_conn_count() == NRF_SDH_BLE_CENTRAL_LINK_COUNT)
            {
                bsp_board_led_off(CENTRAL_SCANNING_LED);
            }
            else
            {
                // Resume scanning.
                bsp_board_led_on(CENTRAL_SCANNING_LED);
                scan_start();
            }
        } break; // BLE_GAP_EVT_CONNECTED

        // Upon disconnection, reset the connection handle of the peer that disconnected, update
        // the LEDs status and start scanning again.
        case BLE_GAP_EVT_DISCONNECTED:
        {
            NRF_LOG_INFO("LBS central link 0x%x disconnected (reason: 0x%x)",
                         p_gap_evt->conn_handle,
                         p_gap_evt->params.disconnected.reason);

            if (ble_conn_state_central_conn_count() == 0)
            {
                err_code = app_button_disable();
                APP_ERROR_CHECK(err_code);

                // Turn off the LED that indicates the connection.
                bsp_board_led_off(CENTRAL_CONNECTED_LED);
            }

            // Start scanning.
            scan_start();

            // Turn on the LED for indicating scanning.
            bsp_board_led_on(CENTRAL_SCANNING_LED);

        } break;

        case BLE_GAP_EVT_TIMEOUT:
        {
            // Timeout for scanning is not specified, so only the connection requests can time out.
            if (p_gap_evt->params.timeout.src == BLE_GAP_TIMEOUT_SRC_CONN)
            {
                NRF_LOG_DEBUG("Connection request timed out.");
            }
        } break;

        case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
        {
            NRF_LOG_DEBUG("BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST.");
            // Accept parameters requested by peer.
            err_code = sd_ble_gap_conn_param_update(p_gap_evt->conn_handle,
                                        &p_gap_evt->params.conn_param_update_request.conn_params);
            APP_ERROR_CHECK(err_code);
        } break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            NRF_LOG_DEBUG("PHY update request.");
            ble_gap_phys_t const phys =
            {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
        } break;

        case BLE_GATTC_EVT_TIMEOUT:
        {
            // Disconnect on GATT client timeout event.
            NRF_LOG_DEBUG("GATT client timeout.");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
        } break;

        case BLE_GATTS_EVT_TIMEOUT:
        {
            // Disconnect on GATT server timeout event.
            NRF_LOG_DEBUG("GATT server timeout.");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
        } break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief LED Button collector initialization. */
static void lbs_c_init(void)
{
    ret_code_t       err_code;
    ble_lbs_c_init_t lbs_c_init_obj;

    lbs_c_init_obj.evt_handler   = lbs_c_evt_handler;
    lbs_c_init_obj.p_gatt_queue  = &m_ble_gatt_queue;
    lbs_c_init_obj.error_handler = lbs_error_handler;

    for (uint32_t i = 0; i < NRF_SDH_BLE_CENTRAL_LINK_COUNT; i++)
    {
        err_code = ble_lbs_c_init(&m_lbs_c[i], &lbs_c_init_obj);
        APP_ERROR_CHECK(err_code);
    }
}


/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupts.
 */
static void ble_stack_init(void)
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}


/**@brief Function for writing to the LED characteristic of all connected clients.
 *
 * @details Based on whether the button is pressed or released, this function writes a high or low
 *          LED status to the server.
 *
 * @param[in] button_action The button action (press or release).
 *            Determines whether the LEDs of the servers are ON or OFF.
 *
 * @return If successful, NRF_SUCCESS is returned. Otherwise, returns the error code from @ref ble_lbs_led_status_send.
 */
static ret_code_t led_status_send_to_all(uint8_t button_action)
{
    ret_code_t err_code;

    for (uint32_t i = 0; i< NRF_SDH_BLE_CENTRAL_LINK_COUNT; i++)
    {
        err_code = ble_lbs_led_status_send(&m_lbs_c[i], button_action);
        if (err_code != NRF_SUCCESS &&
            err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
            err_code != NRF_ERROR_INVALID_STATE)
        {
            return err_code;
        }
    }
        return NRF_SUCCESS;
}


/**@brief Function for handling events from the button handler module.
 *
 * @param[in] pin_no        The pin that the event applies to.
 * @param[in] button_action The button action (press or release).
 */
static void button_event_handler(uint8_t pin_no, uint8_t button_action)
{
    ret_code_t err_code;

    switch (pin_no)
    {
        case LEDBUTTON_BUTTON:
            err_code = led_status_send_to_all(button_action);
            if (err_code == NRF_SUCCESS)
            {
                NRF_LOG_INFO("LBS write LED state %d", button_action);
            }
            break;

        default:
            APP_ERROR_HANDLER(pin_no);
            break;
    }
}


/**@brief Function for initializing the button handler module.
 */
static void buttons_init(void)
{
    ret_code_t err_code;

   // The array must be static because a pointer to it is saved in the button handler module.
    static app_button_cfg_t buttons[] =
    {
        {LEDBUTTON_BUTTON, false, BUTTON_PULL, button_event_handler}
    };

    err_code = app_button_init(buttons, ARRAY_SIZE(buttons), BUTTON_DETECTION_DELAY);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling database discovery events.
 *
 * @details This function is a callback function to handle events from the database discovery module.
 *          Depending on the UUIDs that are discovered, this function forwards the events
 *          to their respective services.
 *
 * @param[in] p_event  Pointer to the database discovery event.
 */
static void db_disc_handler(ble_db_discovery_evt_t * p_evt)
{
    NRF_LOG_DEBUG("call to ble_lbs_on_db_disc_evt for instance %d and link 0x%x!",
                  p_evt->conn_handle,
                  p_evt->conn_handle);

    ble_lbs_on_db_disc_evt(&m_lbs_c[p_evt->conn_handle], p_evt);
}


/** @brief Database discovery initialization.
 */
static void db_discovery_init(void)
{
    ble_db_discovery_init_t db_init;

    memset(&db_init, 0, sizeof(ble_db_discovery_init_t));

    db_init.evt_handler  = db_disc_handler;
    db_init.p_gatt_queue = &m_ble_gatt_queue;

    ret_code_t err_code = ble_db_discovery_init(&db_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling the idle state (main loop).
 *
 * @details This function handles any pending log operations, then sleeps until the next event occurs.
 */
static void idle_state_handle(void)
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}


/** @brief Function for initializing the log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


/** @brief Function for initializing the timer.
 */
static void timer_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the GATT module.
 */
static void gatt_init(void)
{
    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
    APP_ERROR_CHECK(err_code);
}

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
  const float32_t theta[NB_OF_CLASSES*VECTOR_DIMENSION] = {
  -52.906172839506176, -72.64444444444445, -80.73827160493828,-51.79925650557621,
   -74.02973977695167, -84.38289962825279, -54.23622047244095, -71.48818897637796, 
   -80.89763779527559, -74.36363636363636, -63.83116883116883, -78.83116883116882,-70.55833333333334,
    -54.925, -74.94166666666666, -69.1640625, -60.4765625, -79.859375
  }; /**< Mean values for the Gaussians */

  const float32_t sigma[NB_OF_CLASSES*VECTOR_DIMENSION] = {
   24.39613483391136, 7.666172937858941, 31.106806987546573, 9.885352705420123, 2.3634279116776375, 17.1507857425534,
    2.164672427697435, 20.391592881538326, 0.5328291640109084, 31.60802843365385, 1.880587043874784, 23.38708055036829,
     21.229930653908333, 11.586041765019429, 3.538263987241654, 4.809021094446527, 8.311950781946527, 2.683349707727777
  }; /**< Variances for the Gaussians */

  const float32_t classPriors[6] = {
   0.33665835411471323f, 0.22360764754779716f, 0.1055694098088113f, 0.12801330008312553f, 0.09975062344139651f, 
   0.10640066500415628f
  }; /**< Class prior probabilities */


    // for(int i=0;i<4;i++){
    //   NRF_LOG_INFO("%d, %d, %d", beacon_buf1[i], beacon_buf2[i], beacon_buf3[i]);
    // }

    float32_t result[NB_OF_CLASSES];
    float32_t maxProba;
    uint32_t index;
  
    S.vectorDimension = VECTOR_DIMENSION; 
    S.numberOfClasses = NB_OF_CLASSES; 
    S.theta = theta;          
    S.sigma = sigma;         
    S.classPriors = classPriors;    
    S.epsilon=4.328939296523643e-09f; 

    in[0] = b1;
    in[1] = b2;
    in[2]= b3;
    //in[0] = -1.5f;
    //in[1] = 1.0f;
    //in[2] = 0.0f;

    index = arm_gaussian_naive_bayes_predict_f32(&S, in, result);

    maxProba = result[index];

    NRF_LOG_ERROR( "in[0] " NRF_LOG_FLOAT_MARKER "\r", NRF_LOG_FLOAT(in[0]));
    NRF_LOG_ERROR( "in[1] " NRF_LOG_FLOAT_MARKER "\r", NRF_LOG_FLOAT(in[1]));
    NRF_LOG_ERROR( "in[2] " NRF_LOG_FLOAT_MARKER "\r", NRF_LOG_FLOAT(in[2]));

    //NRF_LOG_INFO("in_1 = %f", in[0]);
    //NRF_LOG_INFO("in_2 = %f", in[0]);
    //NRF_LOG_INFO("in_3 = %f", in[0]);


    //NRF_LOG_INFO("Class = %d\n", result[index]);
    NRF_LOG_ERROR( "Class = " NRF_LOG_FLOAT_MARKER "\r", NRF_LOG_FLOAT(result[index]));
 

}


int main(void)
{
    // Initialize.
    log_init();
    timer_init();
    leds_init();
    buttons_init();
    power_management_init();

    ble_stack_init();
    gatt_init();
    db_discovery_init();
    lbs_c_init();
    ble_conn_state_init();
    scan_init();

    // Start execution.
    NRF_LOG_INFO("Multilink example started.");
    scan_start();

    for (;;)
    {
	    for(int i=0;i<20;i++)
	    {
	        idle_state_handle();
	    }

	    // todo - call this via timer/interrupt
	    get_location();

    }
}
