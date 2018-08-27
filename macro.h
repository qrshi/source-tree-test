#ifndef _MACRO_H_
#define	_MACRO_H_

#define _MARCRO_USER_TEST 10
#define MARCRO_USER_TEST _MARCRO_USER_TEST



#define BLE_LBS_BLE_OBSERVER_PRIO 2

typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;


// Forward declaration of the ble_lbs_t type.
typedef struct ble_lbs_s ble_lbs_t;
typedef void (*ble_lbs_led_write_handler_t) (uint16_t conn_handle, ble_lbs_t * p_lbs, uint8_t new_state);

/**@brief BLE Event header. */
typedef struct
{
  uint16_t evt_id;                /**< Value from a BLE_<module>_EVT series. */
  uint16_t evt_len;               /**< Length in octets including this header. */
} ble_evt_hdr_t;

/**@brief Event structure for @ref BLE_EVT_USER_MEM_REQUEST. */
typedef struct
{
  uint8_t                     type;     /**< User memory type, see @ref BLE_USER_MEM_TYPES. */
} ble_evt_user_mem_request_t;

/**@brief User Memory Block. */
typedef struct
{
  uint8_t          *p_mem;      /**< Pointer to the start of the user memory block. */
  uint16_t          len;        /**< Length in bytes of the user memory block. */
} ble_user_mem_block_t;


/**@brief Event structure for @ref BLE_EVT_USER_MEM_RELEASE. */
typedef struct
{
  uint8_t                     type;       /**< User memory type, see @ref BLE_USER_MEM_TYPES. */
  ble_user_mem_block_t        mem_block;  /**< User memory block */
} ble_evt_user_mem_release_t;

/**@brief Event structure for events not associated with a specific function module. */
typedef struct
{
  uint16_t conn_handle;                                 /**< Connection Handle on which this event occurred. */
  union
  {
    ble_evt_user_mem_request_t      user_mem_request;    /**< User Memory Request Event Parameters. */
    ble_evt_user_mem_release_t      user_mem_release;    /**< User Memory Release Event Parameters. */
  } params;                                              /**< Event parameter union. */
} ble_common_evt_t;

/**@brief Common BLE Event type, wrapping the module specific event reports. */
typedef struct
{
  ble_evt_hdr_t header;           /**< Event header. */
  union
  {
    ble_common_evt_t  common_evt; /**< Common Event, evt_id in BLE_EVT_* series. */
 
  } evt;                          /**< Event union. */
} ble_evt_t;

/**@brief   BLE stack event handler. */
typedef void (*nrf_sdh_ble_evt_handler_t)(ble_evt_t const * p_ble_evt, void * p_context);
/**@brief   BLE event observer. */
typedef struct
{
    nrf_sdh_ble_evt_handler_t handler;      //!< BLE event handler.
    void *                    p_context;    //!< A parameter to the event handler.
} const nrf_sdh_ble_evt_observer_t;

/**@brief GATT Characteristic Definition Handles. */
typedef struct
{
  uint16_t          value_handle;       /**< Handle to the characteristic value. */
  uint16_t          user_desc_handle;   /**< Handle to the User Description descriptor, or @ref BLE_GATT_HANDLE_INVALID if not present. */
  uint16_t          cccd_handle;        /**< Handle to the Client Characteristic Configuration Descriptor, or @ref BLE_GATT_HANDLE_INVALID if not present. */
  uint16_t          sccd_handle;        /**< Handle to the Server Characteristic Configuration Descriptor, or @ref BLE_GATT_HANDLE_INVALID if not present. */
} ble_gatts_char_handles_t;

// Forward declaration of the ble_lbs_t type.
typedef struct ble_lbs_s ble_lbs_t;

/**@brief LED Button Service structure. This structure contains various status information for the service. */
struct ble_lbs_s
{
    uint16_t                    service_handle;      /**< Handle of LED Button Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t    led_char_handles;    /**< Handles related to the LED Characteristic. */
    ble_gatts_char_handles_t    button_char_handles; /**< Handles related to the Button Characteristic. */
    uint8_t                     uuid_type;           /**< UUID type for the LED Button Service. */
    ble_lbs_led_write_handler_t led_write_handler;   /**< Event handler to be called when the LED Characteristic is written. */
};

#define STRINGIFY_(val) #val
/** Converts a macro argument into a character constant.
 */
#define STRINGIFY(val)  STRINGIFY_(val)
/*
(sdh_ble_observers2,m_lbs_obs)
static nrf_sdh_ble_evt_observer_t m_lbs_obs __attribute__ ((section(#sdh_ble_observers2))) __attribute__((used))
*/
#define NRF_SECTION_ITEM_REGISTER(section_name, section_var) \
    section_var __attribute__ ((section(STRINGIFY(section_name)))) __attribute__((used))

#define CONCAT_2(p1, p2)      CONCAT_2_(p1, p2)
/** Auxiliary macro used by @ref CONCAT_2 */
#define CONCAT_2_(p1, p2)     p1##p2

/*
(sdh_ble_observers, BLE_LBS_BLE_OBSERVER_PRIO, static nrf_sdh_ble_evt_observer_t m_lbs_obs)

(sdh_ble_observers2,m_lbs_obs)
*/
#define NRF_SECTION_SET_ITEM_REGISTER(_name, _priority, _var)                                       \
    NRF_SECTION_ITEM_REGISTER(CONCAT_2(_name, _priority), _var)

/*
(m_lbs_obs,BLE_LBS_BLE_OBSERVER_PRIO,ble_lbs_on_ble_evt,&m_lbs)
static nrf_sdh_ble_evt_observer_t m_lbs_obs
(sdh_ble_observers, BLE_LBS_BLE_OBSERVER_PRIO, static nrf_sdh_ble_evt_observer_t m_lbs_obs)

*/
#define NRF_SDH_BLE_OBSERVER(_name, _prio, _handler, _context)                                      \
NRF_SECTION_SET_ITEM_REGISTER(sdh_ble_observers, _prio, static nrf_sdh_ble_evt_observer_t _name) =  \
{                                                                                                   \
    .handler   = _handler,                                                                          \
    .p_context = _context                                                                           \
}
/*_name==m_lbs
static ble_lbs_t m_lbs;  
(m_lbs_obs,BLE_LBS_BLE_OBSERVER_PRIO,ble_lbs_on_ble_evt,&m_lbs)
*/
#define BLE_LBS_DEF(_name)                                                                          \
static ble_lbs_t _name;                                                                             \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                                 \
                     BLE_LBS_BLE_OBSERVER_PRIO,                                                     \
                     ble_lbs_on_ble_evt, &_name)


void ble_lbs_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);

#endif



