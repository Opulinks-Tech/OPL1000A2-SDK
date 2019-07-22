/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#ifndef _BLE_UUID_H_
#define _BLE_UUID_H_


enum 
{
    ATT_INVALID_UUID = 0,
    /*----------------- SERVICES ---------------------*/
    /// Generic Access Profile
    ATT_SVC_GENERIC_ACCESS                      = 0x1800,
    /// Attribute Profile
    ATT_SVC_GENERIC_ATTRIBUTE,
    /// Immediate alert Service
    ATT_SVC_IMMEDIATE_ALERT,
    /// Link Loss Service
    ATT_SVC_LINK_LOSS,
    /// Tx Power Service
    ATT_SVC_TX_POWER,
    /// Current Time Service Service
    ATT_SVC_CURRENT_TIME,
    /// Reference Time Update Service
    ATT_SVC_REF_TIME_UPDATE,
    /// Next DST Change Service
    ATT_SVC_NEXT_DST_CHANGE,
    /// Glucose Service
    ATT_SVC_GLUCOSE                             = 0x1808,
    /// Health Thermometer Service
    ATT_SVC_HEALTH_THERMOM                      = 0x1809,
    /// Device Information Service
    ATT_SVC_DEVICE_INFO                         = 0x180A,
    /// Heart Rate Service
    ATT_SVC_HEART_RATE                          = 0x180D,
    /// Phone Alert Status Service
    ATT_SVC_PHONE_ALERT_STATUS,
    /// Battery Service
    ATT_SVC_BATTERY_SERVICE,
    /// Blood Pressure Service
    ATT_SVC_BLOOD_PRESSURE                      = 0x1810,
    /// Alert Notification Service
    ATT_SVC_ALERT_NTF                           = 0x1811,
    /// HID Service
    ATT_SVC_HID                                 = 0x1812,
    /// Scan Parameters Service
    ATT_SVC_SCAN_PARAMETERS                     = 0x1813,
    /// Running Speed and Cadence Service
    ATT_SVC_RUNNING_SPEED_CADENCE               = 0x1814,
    /// Cycling Speed and Cadence Service
    ATT_SVC_CYCLING_SPEED_CADENCE               = 0x1816,
    /// Cycling Power Service
    ATT_SVC_CYCLING_POWER                       = 0x1818,
    /// Location and Navigation Service
    ATT_SVC_LOCATION_AND_NAVIGATION             = 0x1819,
    /// User Data Service
    ATT_SVC_USER_DATA                           = 0x181C,
    /// Weight Scale Service
    ATT_SVC_WEIGHT_SCALE                        = 0x181D,

    /*------------------- UNITS ---------------------*/
    /// No defined unit
    ATT_UNIT_UNITLESS                       	= 0x2700,
    /// Length Unit - Metre
    ATT_UNIT_METRE,
    //Mass unit - Kilogram
    ATT_UNIT_KG,
    ///Time unit - second
    ATT_UNIT_SECOND,
    ///Electric current unit - Ampere
    ATT_UNIT_AMPERE,
    ///Thermodynamic Temperature unit - Kelvin
    ATT_UNIT_KELVIN,
    /// Amount of substance unit - mole
    ATT_UNIT_MOLE,
    ///Luminous intensity unit - candela
    ATT_UNIT_CANDELA,
    ///Area unit - square metres
    ATT_UNIT_SQ_METRE                       	= 0x2710,
    ///Colume unit - cubic metres
    ATT_UNIT_CUBIC_METRE,
    ///Velocity unit - metres per second
    ATT_UNIT_METRE_PER_SECOND,
    ///Acceleration unit - metres per second squared
    ATT_UNIT_METRES_PER_SEC_SQ,
    ///Wavenumber unit - reciprocal metre
    ATT_UNIT_RECIPROCAL_METRE,
    ///Density unit - kilogram per cubic metre
    ATT_UNIT_DENS_KG_PER_CUBIC_METRE,
    ///Surface density unit - kilogram per square metre
    ATT_UNIT_KG_PER_SQ_METRE,
    ///Specific volume unit - cubic metre per kilogram
    ATT_UNIT_CUBIC_METRE_PER_KG,
    ///Current density unit - ampere per square metre
    ATT_UNIT_AMPERE_PER_SQ_METRE,
    ///Magnetic field strength unit - Ampere per metre
    ATT_UNIT_AMPERE_PER_METRE,
    ///Amount concentration unit - mole per cubic metre
    ATT_UNIT_MOLE_PER_CUBIC_METRE,
    ///Mass Concentration unit - kilogram per cubic metre
    ATT_UNIT_MASS_KG_PER_CUBIC_METRE,
    ///Luminance unit - candela per square metre
    ATT_UNIT_CANDELA_PER_SQ_METRE,
    ///Refractive index unit
    ATT_UNIT_REFRACTIVE_INDEX,
    ///Relative permeability unit
    ATT_UNIT_RELATIVE_PERMEABILITY,
    ///Plane angle unit - radian
    ATT_UNIT_RADIAN                         	= 0x2720,
    ///Solid angle unit - steradian
    ATT_UNIT_STERADIAN,
    ///Frequency unit - Hertz
    ATT_UNIT_HERTZ,
    ///Force unit - Newton
    ATT_UNIT_NEWTON,
    ///Pressure unit - Pascal
    ATT_UNIT_PASCAL,
    ///Energy unit - Joule
    ATT_UNIT_JOULE,
    ///Power unit - Watt
    ATT_UNIT_WATT,
    ///electric Charge unit - Coulomb
    ATT_UNIT_COULOMB,
    ///Electric potential difference - Volt
    ATT_UNIT_VOLT,
    ///Capacitance unit - Farad
    ATT_UNIT_FARAD,
    ///electric resistance unit - Ohm
    ATT_UNIT_OHM,
    ///Electric conductance - Siemens
    ATT_UNIT_SIEMENS,
    ///Magnetic flux unit - Weber
    ATT_UNIT_WEBER,
    ///Magnetic flux density unit - Tesla
    ATT_UNIT_TESLA,
    ///Inductance unit - Henry
    ATT_UNIT_HENRY,
    ///Temperature unit - degree Celsius
    ATT_UNIT_CELSIUS,
    ///Luminous flux unit - lumen
    ATT_UNIT_LUMEN,
    //Illuminance unit - lux
    ATT_UNIT_LUX,
    ///Activity referred to a radionuclide unit - becquerel
    ATT_UNIT_BECQUEREL,
    ///Absorbed dose unit - Gray
    ATT_UNIT_GRAY,
    ///Dose equivalent unit - Sievert
    ATT_UNIT_SIEVERT,
    ///Catalytic activity unit - Katal
    ATT_UNIT_KATAL,
    ///Synamic viscosity unit - Pascal second
    ATT_UNIT_PASCAL_SECOND                      = 0x2740,
    ///Moment of force unit - Newton metre
    ATT_UNIT_NEWTON_METRE,
    ///surface tension unit - Newton per metre
    ATT_UNIT_NEWTON_PER_METRE,
    ///Angular velocity unit - radian per second
    ATT_UNIT_RADIAN_PER_SECOND,
    ///Angular acceleration unit - radian per second squared
    ATT_UNIT_RADIAN_PER_SECOND_SQ,
    ///Heat flux density unit - Watt per square metre
    ATT_UNIT_WATT_PER_SQ_METRE,
    ///HEat capacity unit - Joule per Kelvin
    ATT_UNIT_JOULE_PER_KELVIN,
    ///Specific heat capacity unit - Joule per kilogram kelvin
    ATT_UNIT_JOULE_PER_KG_KELVIN,
    ///Specific Energy unit - Joule per kilogram
    ATT_UNIT_JOULE_PER_KG,
    ///Thermal conductivity - Watt per metre Kelvin
    ATT_UNIT_WATT_PER_METRE_KELVIN,
    ///Energy Density unit - joule per cubic metre
    ATT_UNIT_JOULE_PER_CUBIC_METRE,
    ///Electric field strength unit - volt per metre
    ATT_UNIT_VOLT_PER_METRE,
    ///Electric charge density unit - coulomb per cubic metre
    ATT_UNIT_COULOMB_PER_CUBIC_METRE,
    ///Surface charge density unit - coulomb per square metre
    ATT_UNIT_SURF_COULOMB_PER_SQ_METRE,
    ///Electric flux density unit - coulomb per square metre
    ATT_UNIT_FLUX_COULOMB_PER_SQ_METRE,
    ///Permittivity unit - farad per metre
    ATT_UNIT_FARAD_PER_METRE,
    ///Permeability unit - henry per metre
    ATT_UNIT_HENRY_PER_METRE,
    ///Molar energy unit - joule per mole
    ATT_UNIT_JOULE_PER_MOLE,
    ///Molar entropy unit - joule per mole kelvin
    ATT_UNIT_JOULE_PER_MOLE_KELVIN,
    ///Exposure unit - coulomb per kilogram
    ATT_UNIT_COULOMB_PER_KG,
    ///Absorbed dose rate unit - gray per second
    ATT_UNIT_GRAY_PER_SECOND,
    ///Radiant intensity unit - watt per steradian
    ATT_UNIT_WATT_PER_STERADIAN,
    ///Radiance unit - watt per square meter steradian
    ATT_UNIT_WATT_PER_SQ_METRE_STERADIAN,
    ///Catalytic activity concentration unit - katal per cubic metre
    ATT_UNIT_KATAL_PER_CUBIC_METRE,
    ///Time unit - minute
    ATT_UNIT_MINUTE                         	= 0x2760,
    ///Time unit - hour
    ATT_UNIT_HOUR,
    ///Time unit - day
    ATT_UNIT_DAY,
    ///Plane angle unit - degree
    ATT_UNIT_ANGLE_DEGREE,
    ///Plane angle unit - minute
    ATT_UNIT_ANGLE_MINUTE,
    ///Plane angle unit - second
    ATT_UNIT_ANGLE_SECOND,
    ///Area unit - hectare
    ATT_UNIT_HECTARE,
    ///Volume unit - litre
    ATT_UNIT_LITRE,
    ///Mass unit - tonne
    ATT_UNIT_TONNE,
    ///Pressure unit - bar
    ATT_UNIT_BAR                            	= 0x2780,
    ///Pressure unit - millimetre of mercury
    ATT_UNIT_MM_MERCURY,
    ///Length unit - angstrom
    ATT_UNIT_ANGSTROM,
    ///Length unit - nautical mile
    ATT_UNIT_NAUTICAL_MILE,
    ///Area unit - barn
    ATT_UNIT_BARN,
    ///Velocity unit - knot
    ATT_UNIT_KNOT,
    ///Logarithmic radio quantity unit - neper
    ATT_UNIT_NEPER,
    ///Logarithmic radio quantity unit - bel
    ATT_UNIT_BEL,
    ///Length unit - yard
    ATT_UNIT_YARD                           	= 0x27A0,
    ///Length unit - parsec
    ATT_UNIT_PARSEC,
    ///length unit - inch
    ATT_UNIT_INCH,
    ///length unit - foot
    ATT_UNIT_FOOT,
    ///length unit - mile
    ATT_UNIT_MILE,
    ///pressure unit - pound-force per square inch
    ATT_UNIT_POUND_FORCE_PER_SQ_INCH,
    ///velocity unit - kilometre per hour
    ATT_UNIT_KM_PER_HOUR,
    ///velocity unit - mile per hour
    ATT_UNIT_MILE_PER_HOUR,
    ///angular velocity unit - revolution per minute
    ATT_UNIT_REVOLUTION_PER_MINUTE,
    ///energy unit - gram calorie
    ATT_UNIT_GRAM_CALORIE,
    ///energy unit - kilogram calorie
    ATT_UNIT_KG_CALORIE,
    /// energy unit - kilowatt hour
    ATT_UNIT_KILOWATT_HOUR,
    ///thermodynamic temperature unit - degree Fahrenheit
    ATT_UNIT_FAHRENHEIT,
    ///percentage
    ATT_UNIT_PERCENTAGE,
    ///per mille
    ATT_UNIT_PER_MILLE,
    ///period unit - beats per minute)
    ATT_UNIT_BEATS_PER_MINUTE,
    ///electric charge unit - ampere hours
    ATT_UNIT_AMPERE_HOURS,
    ///mass density unit - milligram per decilitre
    ATT_UNIT_MILLIGRAM_PER_DECILITRE,
    ///mass density unit - millimole per litre
    ATT_UNIT_MILLIMOLE_PER_LITRE,
    ///time unit - year
    ATT_UNIT_YEAR,
    ////time unit - month
    ATT_UNIT_MONTH,


    /*---------------- DECLARATIONS -----------------*/
    /// Primary service Declaration
    ATT_DECL_PRIMARY_SERVICE                     = 0x2800,
    /// Secondary service Declaration
    ATT_DECL_SECONDARY_SERVICE,
    /// Include Declaration
    ATT_DECL_INCLUDE,
    /// Characteristic Declaration
    ATT_DECL_CHARACTERISTIC,


    /*----------------- DESCRIPTORS -----------------*/
    /// Characteristic extended properties
    ATT_DESC_CHAR_EXT_PROPERTIES                 = 0x2900,
    /// Characteristic user description
    ATT_DESC_CHAR_USER_DESCRIPTION,
    /// Client characteristic configuration
    ATT_DESC_CLIENT_CHAR_CFG,
    /// Server characteristic configuration
    ATT_DESC_SERVER_CHAR_CFG,
    /// Characteristic Presentation Format
    ATT_DESC_CHAR_PRES_FORMAT,
    /// Characteristic Aggregate Format
    ATT_DESC_CHAR_AGGREGATE_FORMAT,
    /// Valid Range
    ATT_DESC_VALID_RANGE,
    /// External Report Reference
    ATT_DESC_EXT_REPORT_REF,
    /// Report Reference
    ATT_DESC_REPORT_REF,


    /*--------------- CHARACTERISTICS ---------------*/
    /// Device name
    ATT_CHAR_DEVICE_NAME                        = 0x2A00,
    /// Appearance
    ATT_CHAR_APPEARANCE                         = 0x2A01,
    /// Privacy flag
    ATT_CHAR_PRIVACY_FLAG                       = 0x2A02,
    /// Reconnection address
    ATT_CHAR_RECONNECTION_ADDR                  = 0x2A03,
    /// Peripheral preferred connection parameters
    ATT_CHAR_PERIPH_PREF_CON_PARAM              = 0x2A04,
    /// Service handles changed
    ATT_CHAR_SERVICE_CHANGED                    = 0x2A05,
    /// Alert Level characteristic
    ATT_CHAR_ALERT_LEVEL                        = 0x2A06,
    /// Tx Power Level
    ATT_CHAR_TX_POWER_LEVEL                     = 0x2A07,
    /// Date Time
    ATT_CHAR_DATE_TIME                          = 0x2A08,
    /// Day of Week
    ATT_CHAR_DAY_WEEK                           = 0x2A09,
    /// Day Date Time
    ATT_CHAR_DAY_DATE_TIME                      = 0x2A0A,
    /// Exact time 256
    ATT_CHAR_EXACT_TIME_256                     = 0x2A0C,
    /// DST Offset
    ATT_CHAR_DST_OFFSET                         = 0x2A0D,
    /// Time zone
    ATT_CHAR_TIME_ZONE                          = 0x2A0E,
    /// Local time Information
    ATT_CHAR_LOCAL_TIME_INFO                    = 0x2A0F,
    /// Time with DST
    ATT_CHAR_TIME_WITH_DST                      = 0x2A11,
    /// Time Accuracy
    ATT_CHAR_TIME_ACCURACY                      = 0x2A12,
    ///Time Source
    ATT_CHAR_TIME_SOURCE                        = 0x2A13,
    /// Reference Time Information
    ATT_CHAR_REFERENCE_TIME_INFO                = 0x2A14,
    /// Time Update Control Point
    ATT_CHAR_TIME_UPDATE_CNTL_POINT             = 0x2A16,
    /// Time Update State
    ATT_CHAR_TIME_UPDATE_STATE                  = 0x2A17,
    /// Glucose Measurement
    ATT_CHAR_GLUCOSE_MEAS                       = 0x2A18,
    /// Battery Level
    ATT_CHAR_BATTERY_LEVEL                      = 0x2A19,
    /// Temperature Measurement
    ATT_CHAR_TEMPERATURE_MEAS                   = 0x2A1C,
    /// Temperature Type
    ATT_CHAR_TEMPERATURE_TYPE                   = 0x2A1D,
    /// Intermediate Temperature
    ATT_CHAR_INTERMED_TEMPERATURE               = 0x2A1E,
    /// Measurement Interval
    ATT_CHAR_MEAS_INTERVAL                      = 0x2A21,
    /// Boot Keyboard Input Report
    ATT_CHAR_BOOT_KB_IN_REPORT                  = 0x2A22,
    /// System ID
    ATT_CHAR_SYS_ID                             = 0x2A23,
    /// Model Number String
    ATT_CHAR_MODEL_NB                           = 0x2A24,
    /// Serial Number String
    ATT_CHAR_SERIAL_NB                          = 0x2A25,
    /// Firmware Revision String
    ATT_CHAR_FW_REV                             = 0x2A26,
    /// Hardware revision String
    ATT_CHAR_HW_REV                             = 0x2A27,
    /// Software Revision String
    ATT_CHAR_SW_REV                             = 0x2A28,
    /// Manufacturer Name String
    ATT_CHAR_MANUF_NAME                         = 0x2A29,
    /// IEEE Regulatory Certification Data List
    ATT_CHAR_IEEE_CERTIF                        = 0x2A2A,
    /// CT Time
    ATT_CHAR_CT_TIME                            = 0x2A2B,
    /// Scan Refresh
    ATT_CHAR_SCAN_REFRESH                       = 0x2A31,
    /// Boot Keyboard Output Report
    ATT_CHAR_BOOT_KB_OUT_REPORT                 = 0x2A32,
    /// Boot Mouse Input Report
    ATT_CHAR_BOOT_MOUSE_IN_REPORT               = 0x2A33,
    /// Glucose Measurement Context
    ATT_CHAR_GLUCOSE_MEAS_CTX                   = 0x2A34,
    /// Blood Pressure Measurement
    ATT_CHAR_BLOOD_PRESSURE_MEAS                = 0x2A35,
    /// Intermediate Cuff Pressure
    ATT_CHAR_INTERMEDIATE_CUFF_PRESSURE         = 0x2A36,
    /// Heart Rate Measurement
    ATT_CHAR_HEART_RATE_MEAS                    = 0x2A37,
    /// Body Sensor Location
    ATT_CHAR_BODY_SENSOR_LOCATION               = 0x2A38,
    /// Heart Rate Control Point
    ATT_CHAR_HEART_RATE_CNTL_POINT              = 0x2A39,
    /// Alert Status
    ATT_CHAR_ALERT_STATUS                       = 0x2A3F,
    /// Ringer Control Point
    ATT_CHAR_RINGER_CNTL_POINT                  = 0x2A40,
    /// Ringer Setting
    ATT_CHAR_RINGER_SETTING                     = 0x2A41,
    /// Alert Category ID Bit Mask
    ATT_CHAR_ALERT_CAT_ID_BIT_MASK              = 0x2A42,
    /// Alert Category ID
    ATT_CHAR_ALERT_CAT_ID                       = 0x2A43,
    /// Alert Notification Control Point
    ATT_CHAR_ALERT_NTF_CTNL_PT                  = 0x2A44,
    /// Unread Alert Status
    ATT_CHAR_UNREAD_ALERT_STATUS                = 0x2A45,
    /// New Alert
    ATT_CHAR_NEW_ALERT                          = 0x2A46,
    /// Supported New Alert Category
    ATT_CHAR_SUP_NEW_ALERT_CAT                  = 0x2A47,
    /// Supported Unread Alert Category
    ATT_CHAR_SUP_UNREAD_ALERT_CAT               = 0x2A48,
    /// Blood Pressure Feature
    ATT_CHAR_BLOOD_PRESSURE_FEATURE             = 0x2A49,
    /// HID Information
    ATT_CHAR_HID_INFO                           = 0x2A4A,
    /// Report Map
    ATT_CHAR_REPORT_MAP                         = 0x2A4B,
    /// HID Control Point
    ATT_CHAR_HID_CTNL_PT                        = 0x2A4C,
    /// Report
    ATT_CHAR_REPORT                             = 0x2A4D,
    /// Protocol Mode
    ATT_CHAR_PROTOCOL_MODE                      = 0x2A4E,
    /// Scan Interval Window
    ATT_CHAR_SCAN_INTV_WD                       = 0x2A4F,
    /// PnP ID
    ATT_CHAR_PNP_ID                             = 0x2A50,
    /// Glucose Feature
    ATT_CHAR_GLUCOSE_FEATURE                    = 0x2A51,
    /// Record access control point
    ATT_CHAR_REC_ACCESS_CTRL_PT                 = 0x2A52,
    /// RSC Measurement
    ATT_CHAR_RSC_MEAS                           = 0x2A53,
    /// RSC Feature
    ATT_CHAR_RSC_FEAT                           = 0x2A54,
    /// SC Control Point
    ATT_CHAR_SC_CNTL_PT                         = 0x2A55,
    /// CSC Measurement
    ATT_CHAR_CSC_MEAS                           = 0x2A5B,
    /// CSC Feature
    ATT_CHAR_CSC_FEAT                           = 0x2A5C,
    /// Sensor Location
    ATT_CHAR_SENSOR_LOC                         = 0x2A5D,
    /// CP Measurement
    ATT_CHAR_CP_MEAS                            = 0x2A63,
    /// CP Vector
    ATT_CHAR_CP_VECTOR                          = 0x2A64,
    /// CP Feature
    ATT_CHAR_CP_FEAT                            = 0x2A65,
    /// CP Control Point
    ATT_CHAR_CP_CNTL_PT                         = 0x2A66,
    /// Location and Speed
    ATT_CHAR_LOC_SPEED                          = 0x2A67,
    /// Navigation
    ATT_CHAR_NAVIGATION                         = 0x2A68,
    /// Position Quality
    ATT_CHAR_POS_QUALITY                        = 0x2A69,
    /// LN Feature
    ATT_CHAR_LN_FEAT                            = 0x2A6A,
    /// LN Control Point
    ATT_CHAR_LN_CNTL_PT                         = 0x2A6B,

    /// Weight Scale Feature
    ATT_CHAR_WS_FEAT                            = 0x2A9E,
    /// Weight Scale Measurement
    ATT_CHAR_WS_MEAS                            = 0x2A9D,
    /// User height
    ATT_CHAR_UDS_USER_HEIGHT                    = 0x2A8E,
    /// User age
    ATT_CHAR_UDS_USER_AGE                       = 0x2A80,
    /// User date of birth
    ATT_CHAR_UDS_USER_DATE_OF_BIRTH             = 0x2A85,
    /// User database change increment
    ATT_CHAR_UDS_USER_DB_CHANGE_INCR            = 0x2A99,
    /// User Data Index
    ATT_CHAR_UDS_USER_DATA_INDEX                = 0x2A9A,
    /// User Data Control Point
    ATT_CHAR_UDS_USER_CTRL_PT                   = 0x2A9F,
    /// Last define
    ATT_LAST
};

#endif
