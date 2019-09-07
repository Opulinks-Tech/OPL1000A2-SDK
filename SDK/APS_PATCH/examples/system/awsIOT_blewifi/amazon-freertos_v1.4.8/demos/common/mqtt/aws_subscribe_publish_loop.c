/*
 * Amazon FreeRTOS V1.4.8
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */


/**
 * @file aws_subscribe_publish_loop.c
 * @brief A simple MQTT pub sub example.
 *
 * A simple example that connects to an MQTT broker then sends strings and
 * unsigned integers to topics that it is also subscribed to - the result of
 * which is that every message published to a topic is also received from the
 * topic.  The MQTT callback checks the expected data is received on the
 * expected topics, flagging an error if that is not the case.
 */

/* Standard includes. */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "blewifi_ctrl.h"
#include "cmsis_os.h"


/* MQTT include. */
#include "aws_mqtt_agent.h"

/* This demo's configuration */
#include "aws_subscribe_publish_loop.h"
#include "aws_demo_config.h"

/* OPL1000 PWM for lighting*/
#include "hal_pwm.h"
#define MAX_LIGHT_BRIGHTNESS   95
#define MIN_LIGHT_BRIGHTNESS   0
#define DEAFAULT_PWM_FREQ      500

/* Required to get the broker address and port. */
#include "aws_clientcredential.h"

/* Strings are published to a topic that has the following path. */
#define subpubSTRING_TOPIC_PATH           "/string/"

/* Unsigned integers are published to a topic that has the following path. */
#define subpubUINT_TOPIC_PATH             "/uint/"

/* Max size for the name of tasks generated for MQTT. */
#define subpubCHAR_TASK_NAME_MAX_SIZE     10

/* Size of the topic string. */
#define subpubTopicSize                   ( subpubCHAR_TASK_NAME_MAX_SIZE + sizeof( subpubSTRING_TOPIC_PATH ) )

/* Task name. */
#define subpubCHAR_TASK_NAME              "IOT-%d"

/**
 * @brief Size of the IP address character string
 *
 * IP address is 15 character + the null termination.
 */
#define subpubIP_ADDRESS_STRING_SIZE      16

/* Unsecure broker details. */
#define subpubUNSECURE_BROKER_ENDPOINT    "Paste your unsecure MQTT Broker endpoint here."
/* Replace with the unsecure broker's listening port. */
#define subpubUNSECURE_BROKER_PORT        443


#define configPRINTF( X )                           printf X

#define nPublishInterval                  3600000



/**
 * @brief Contains the user data for callback processing.
 */
typedef struct SubpubUserData
{
    const char * pcExpectedString;        /**< Informs the MQTT callback of the next expected string. */
    uint32_t ulExpectedUint32;            /**< Informs the MQTT callback of the next expected integer. */
    BaseType_t xCallbackStatus;           /**< Used to communicate the success or failure of the callback function.
                                           *   xCallbackStatus is set to pdFALSE before the callback is executed, and is
                                           *   set to pdPASS inside the callback only if the callback receives the expected
                                           *   data. */
    StaticEventGroup_t xWakeUpEventGroup; /**< Event group used to synchronize tasks. */
    char cTopic[ subpubTopicSize ];       /**< Topic to subscribe and publish to. */
} SubpubUserData_t;

/**
 * @brief Sends strings and integers to topics that it is also subscribed to.
 *
 * @param[in] xMQTTClientHandle The MQTT client handle to use.
 * @param[in] xSecureConnection If pdTRUE it connects to the secure AWS IoT broker,
 * otherwise it connects to the unsecure broker.
 */
static void prvSubscribePublishDemo( MQTTAgentHandle_t xMQTTClientHandle,
                                     BaseType_t xSecureConnection );

/**
 * @brief Subscribes to a topic and then publishes strings on the same topic.
 *
 * Since it subscribes to the same topic before publishing the strings, the MQTT
 * client receives the published strings.  The MQTT callback checks that the expected
 * strings are received on the expected topics, flagging an error if that is not the
 * case.
 *
 * @param[in] pxConnectParams Parameters used to connect to the MQTT broker.
 * @param[in] xQOS Quality of Service used.
 * @param[in] pxUserData Data used to synchronize between this task and MQTT callback.
 *
 * @return pdPASS if test passes, pdFAIL otherwise.
 */
static BaseType_t prvStringPublishSubscribe( MQTTAgentConnectParams_t * pxConnectParams,
                                             MQTTQoS_t xQOS,
                                             MQTTAgentHandle_t xMQTTClientHandle,
                                             SubpubUserData_t * pxUserData );

/**
 * @brief Subscribes to a topic and then publishes unsigned integers on the same
 * topic.
 *
 * Since it subscribes to the same topic before publishing, the MQTT client receives
 * the published unsigned integers.  The MQTT callback checks that the expected integers
 * are received, flagging an error if that is not the case.
 *
 * @param[in] pxConnectParams Parameters used to connect to the MQTT broker.
 * @param[in] xQOS Quality of Service used.
 * @param[in] pxUserData Data used to synchronize between this task and MQTT callback.
 *
 * @return pdPASS if test passes, pdFAIL otherwise.
 */
static BaseType_t prvUint32PublishSubscribe( MQTTAgentConnectParams_t * pxConnectParams,
                                             MQTTQoS_t xQOS,
                                             MQTTAgentHandle_t xMQTTClientHandle,
                                             SubpubUserData_t * pxUserData );

/**
 * @brief The callback function registered with the MQTT client to get notified
 * of various events including publish messages received from the broker.
 *
 * This is executed in the context of the MQTT task so must be fast and not call
 * any blocking functions.
 *
 * @param[in] pvUserData User data as passed in the connect call. NULL in our case.
 * @param[in] pxCallbackParams Parameters containing the MQTT event and related data.
 *
 * @return The return value is significant only in the case of publish messages. We
 * always return pdFALSE to indicate that we are not taking the ownership of the provided
 * buffer.
 */
static BaseType_t prvMQTTCallback( void * pvUserData,
                                   const MQTTAgentCallbackParams_t * const pxCallbackParams );

/**
 * @brief The callback registered for receiving string messages.
 *
 * This is executed in the context of the MQTT task so must be fast and not call
 * any blocking functions.
 *
 * @param[in] pvCallbackContext User data as passed in the subscribe call.
 * @param[in] pxPublishData The publish data.
 *
 * @return We always return eMQTTFalse to indicate that we are not taking the
 * ownership of the provided buffer.
 */
static MQTTBool_t prvMQTTStringPublishCallback( void * pvCallbackContext,
                                                const MQTTPublishData_t * const pxPublishData );

/**
 * @brief The callback registered for receiving uint32 messages.
 *
 * This is executed in the context of the MQTT task so must be fast and not call
 * any blocking functions.
 *
 * @param[in] pvCallbackContext User data as passed in the subscribe call.
 * @param[in] pxPublishData The publish data.
 *
 * @return We always return eMQTTFalse to indicate that we are not taking the
 * ownership of the provided buffer.
 */
static MQTTBool_t prvMQTTUint32PublishCallback( void * pvCallbackContext,
                                                const MQTTPublishData_t * const pxPublishData );

/**
 * @brief The task that implements simple MQTT subscribe/publish example.
 *
 * @param[in] pvParameters If pdTRUE is passed, it connects to the secure
 * AWS IoT broker otherwise it connects to the unsecure broker.
 */
static void prvPublishSubscribeTask( void * pvParameters );


/*-----------------------------------------------------------*/

/*
 * Collection of strings published.
 */
static const char * const pcLongString = "OPL1000 MQTT string publush testing. This is a long string that requires a length encoding greater than one byte. This tests the encoding and decoding of Remaining Length field in a MQTT packet.";

static const char * const pcPeriodLightOFF = "light:OFF";
static const char * const pcPeriodLightON = "light:ON";

static int nTestLightSetting = 0;

/* The maximum time to wait for an MQTT operation to complete.  Needs to be
 * long enough for the TLS negotiation to complete. */
static const TickType_t xMaxCommandTime = pdMS_TO_TICKS( 1200000UL );

/*-----------------------------------------------------------*/


static BaseType_t prvMQTTCallback( void * pvUserData,
                                   const MQTTAgentCallbackParams_t * const pxCallbackParams )
{
    BaseType_t xTakeOwnership = pdFALSE;

    /* Remove compiler warnings about unused parameters. */
    ( void ) pvUserData;

    switch( pxCallbackParams->xMQTTEvent )
    {
        case eMQTTAgentDisconnect:
            configPRINTF( ( "MQTT Client got disconnected.\r\n" ) );
            break;

        case eMQTTAgentPublish:
            configPRINTF( ( "WARN: Should not have been called as we are registering topic specific callbacks.\r\n" ) );
            break;
    }

    /* We do not want to take the ownership of buffers in any case. */
    return xTakeOwnership;
}
/*-----------------------------------------------------------*/

static MQTTBool_t prvMQTTStringPublishCallback( void * pvCallbackContext,
                                                const MQTTPublishData_t * const pxPublishData )
{
    const char * pcReceivedString, * pcTopic;
    SubpubUserData_t * pxUserData;
    MQTTBool_t xTakeOwnership = eMQTTFalse;

    pxUserData = ( SubpubUserData_t * ) pvCallbackContext;

    /* The published data was a string. */
   configPRINTF( ( "Received %.*s on topic %.*s\r\n", pxPublishData->ulDataLength,
                    ( char * ) pxPublishData->pvData,
                    ( size_t ) pxPublishData->usTopicLength,
                    ( char * ) pxPublishData->pucTopic ) );

    /* Does the received string match the expected string? */
    pcReceivedString = ( char * ) pxPublishData->pvData;
    pcTopic = ( const char * ) pxPublishData->pucTopic;

    //if( strncmp( pcReceivedString, pxUserData->pcExpectedString, strlen( pxUserData->pcExpectedString ) ) == 0 )
    if( strncmp( pcReceivedString, "light:ON", strlen( "light:ON" ) ) == 0 )
    {
        if( strncmp( pcTopic, pxUserData->cTopic, strlen( pxUserData->cTopic ) ) == 0 )
        {
            /* The expected string was received on the expected topic so
             * set the status to pass to let the task know the callback
             * completed successfully. */
             
            Hal_Pwm_SimpleConfigSet(HAL_PWM_IDX_1,MAX_LIGHT_BRIGHTNESS,DEAFAULT_PWM_FREQ); // default bringtness level
            Hal_Pwm_Enable(HAL_PWM_IDX_1);
            
            printf("\033[42;31;4m [%s , %d]  light:ON  \033[0m\n\n", __FUNCTION__,__LINE__);
            
            pxUserData->xCallbackStatus = pdPASS;
        }
    }
    
    if( strncmp( pcReceivedString, "light:OFF", strlen( "light:OFF" ) ) == 0 )
    {
        if( strncmp( pcTopic, pxUserData->cTopic, strlen( pxUserData->cTopic ) ) == 0 )
        {
            /* The expected string was received on the expected topic so
             * set the status to pass to let the task know the callback
             * completed successfully. */
             
            Hal_Pwm_SimpleConfigSet(HAL_PWM_IDX_1,MIN_LIGHT_BRIGHTNESS,DEAFAULT_PWM_FREQ);
            Hal_Pwm_Enable(HAL_PWM_IDX_1);
            Hal_Pwm_Disable(HAL_PWM_IDX_1);

            printf("\033[42;31;4m [%s , %d]  light:OFF  \033[0m\n\n", __FUNCTION__,__LINE__);
            
            pxUserData->xCallbackStatus = pdPASS;
        }
    }

    /* Unblock the task as the callback has executed. */
    ( void ) xEventGroupSetBits( ( EventGroupHandle_t ) &( pxUserData->xWakeUpEventGroup ), 1 );

    /* We do not want to take the ownership of buffer. */
    return xTakeOwnership;
}
/*-----------------------------------------------------------*/

static MQTTBool_t prvMQTTUint32PublishCallback( void * pvCallbackContext,
                                                const MQTTPublishData_t * const pxPublishData )
{
    const char * pcTopic;
    SubpubUserData_t * pxUserData;
    MQTTBool_t xTakeOwnership = eMQTTFalse;

    pxUserData = ( SubpubUserData_t * ) pvCallbackContext;

    /* Does the value of the received uint32_t match that expected? */
    if( memcmp( ( void * ) &pxUserData->ulExpectedUint32, pxPublishData->pvData, sizeof( uint32_t ) ) == 0 )
    {
        configPRINTF( ( "Received %u on topic %.*s\r\n", pxUserData->ulExpectedUint32,
                        ( size_t ) pxPublishData->usTopicLength,
                        ( char * ) pxPublishData->pucTopic ) );

        /* Was the topic on which the uint32_t was received match
         * that expected? */
        pcTopic = ( char * ) pxPublishData->pucTopic;

        if( strncmp( pcTopic, pxUserData->cTopic, strlen( pxUserData->cTopic ) ) == 0 )
        {
            /* The expected uint32_t was received on the expected topic
             * so set the status to pass to let the task know the callback
             * completed successfully. */
            pxUserData->xCallbackStatus = pdPASS;
        }
    }

    /* Unblock the task as the callback has executed. */
    ( void ) xEventGroupSetBits( ( EventGroupHandle_t ) &( pxUserData->xWakeUpEventGroup ), 1 );

    /* We do not want to take the ownership of buffer. */
    return xTakeOwnership;
}
/*-----------------------------------------------------------*/

static BaseType_t prvUint32PublishSubscribe( MQTTAgentConnectParams_t * pxConnectParams,
                                             MQTTQoS_t xQOS,
                                             MQTTAgentHandle_t xMQTTClientHandle,
                                             SubpubUserData_t * pxUserData )
{
    MQTTAgentSubscribeParams_t xSubscribeParams;
    MQTTAgentPublishParams_t xPublishParams;
    MQTTAgentUnsubscribeParams_t xUnsubscribeParams;
    BaseType_t xResult;
    uint32_t ulData, ulPasses = 0UL;
    const uint32_t ulMaxMessages = 10UL;
    TickType_t xTickCount;

    xTickCount = xTaskGetTickCount();
    configPRINTF( ( "%s initiated connection to broker at time %u ticks.\r\n", __FUNCTION__, xTickCount ) );

    strncpy( ( char * ) pxUserData->cTopic, ( char * ) pcTaskGetName( NULL ), subpubCHAR_TASK_NAME_MAX_SIZE );
    strncat( ( char * ) pxUserData->cTopic, ( char * ) subpubUINT_TOPIC_PATH, sizeof( subpubSTRING_TOPIC_PATH ) );

    if( MQTT_AGENT_Connect( xMQTTClientHandle, pxConnectParams, xMaxCommandTime ) == eMQTTAgentSuccess )
    {
        configPRINTF( ( "%s connection to broker took %u ticks.\r\n", __FUNCTION__, xTaskGetTickCount() - xTickCount ) );

        /* Connected to the broker, subscribe to a topic that has the same name
         * as this function.  As this function sends unsigned integers the topic
         * name uses the subpubUINT_TOPIC_PATH path so the callback knows how to
         * interpret received data. */
        xSubscribeParams.pucTopic = ( const uint8_t * ) pxUserData->cTopic;
        xSubscribeParams.usTopicLength = ( uint16_t ) strlen( pxUserData->cTopic );
        xSubscribeParams.xQoS = xQOS;
        xSubscribeParams.pvPublishCallbackContext = ( void * ) pxUserData;
        xSubscribeParams.pxPublishCallback = &( prvMQTTUint32PublishCallback );

        if( MQTT_AGENT_Subscribe( xMQTTClientHandle, &xSubscribeParams, xMaxCommandTime ) == eMQTTAgentSuccess )
        {
            configPRINTF( ( "%s subscribed to topic %s.\r\n", __FUNCTION__, xSubscribeParams.pucTopic ) );

            /* Publish to the topic to which this task is subscribed in order
             * to receive back the data that was published. */
            xPublishParams.xQoS = xQOS;
            xPublishParams.pucTopic = ( const uint8_t * ) pxUserData->cTopic;
            xPublishParams.usTopicLength = ( uint16_t ) ( strlen( pxUserData->cTopic ) );
            xPublishParams.ulDataLength = ( uint16_t ) sizeof( ulData );

            /* ulMaxMessages unsigned integers are sent to the topic. */
            for( ulData = 0; ulData < ulMaxMessages; ulData++ )
            {
                xPublishParams.pvData = &ulData; /* Note this is pointing to a stack variable. */

                /* Used by the callback to check publishes received by this task
                 * match the publishes sent by this task - as the task both
                 * publishes to and subscribes to the same topic. */
                pxUserData->ulExpectedUint32 = ulData;
                pxUserData->xCallbackStatus = pdFAIL;

                if( MQTT_AGENT_Publish( xMQTTClientHandle, &xPublishParams, xMaxCommandTime ) == eMQTTAgentSuccess )
                {
                    /* The event callback will set bit 0 in this event group when it executes,
                     * until then wait here. At this time this demo does not have more than one
                     * message outstanding at a time. */
                    if( xEventGroupWaitBits( ( EventGroupHandle_t ) &( pxUserData->xWakeUpEventGroup ),
                                             1,
                                             pdTRUE,
                                             pdTRUE,
                                             xMaxCommandTime ) == 1 )
                    {
                        /* Did the callback execute and pass?  If so the callback
                         * will have set xCallbackStatus to pdPASS. */
                        if( pxUserData->xCallbackStatus == pdPASS )
                        {
                            ulPasses++;
                        }
                    }
                }
                else
                {
                    configPRINTF( ( "ERROR %s did not published to topic %s\r\n", __FUNCTION__, xPublishParams.pucTopic ) );
                }
            }

            /* Clean up subscriptions. */
            xUnsubscribeParams.pucTopic = ( const uint8_t * ) pxUserData->cTopic;
            xUnsubscribeParams.usTopicLength = ( uint16_t ) strlen( pxUserData->cTopic );

            if( MQTT_AGENT_Unsubscribe( xMQTTClientHandle, &xUnsubscribeParams, xMaxCommandTime ) == eMQTTAgentSuccess )
            {
                configPRINTF( ( "%s unsubscribed from topic %s\r\n", __FUNCTION__, xUnsubscribeParams.pucTopic ) );
            }
            else
            {
                configPRINTF( ( "ERROR:  %s did not unsubscribed from topic %s\r\n", __FUNCTION__, xUnsubscribeParams.pucTopic ) );

                /* Ensure the error is detected. */
                ulPasses = 0;
            }
        }
        else
        {
            configPRINTF( ( "ERROR %s did not subscribe to topic %s.\r\n", __FUNCTION__, xSubscribeParams.pucTopic ) );
        }

        /* Clean up connections. */
        configPRINTF( ( "Disconnecting from broker.\r\n" ) );

        if( MQTT_AGENT_Disconnect( xMQTTClientHandle, xMaxCommandTime ) == eMQTTAgentSuccess )
        {
            configPRINTF( ( "Disconnected from the broker.\r\n" ) );
        }
        else
        {
            configPRINTF( ( "ERROR:  Did not disconnected from the broker.\r\n" ) );

            /* Ensure the error is detected. */
            ulPasses = 0;
        }
    }

    if( ulPasses == ulMaxMessages )
    {
        /* All publishes to this task were as expected. */
        xResult = pdPASS;
    }
    else
    {
        xResult = pdFAIL;
    }

    configASSERT( xResult == pdPASS );

    return xResult;
}
/*-----------------------------------------------------------*/

static BaseType_t prvStringPublishSubscribe( MQTTAgentConnectParams_t * pxConnectParams,
                                             MQTTQoS_t xQOS,
                                             MQTTAgentHandle_t xMQTTClientHandle,
                                             SubpubUserData_t * pxUserData )
{
    MQTTAgentSubscribeParams_t xSubscribeParams;
    MQTTAgentPublishParams_t xPublishParams;
    //MQTTAgentUnsubscribeParams_t xUnsubscribeParams;
    BaseType_t xResult = pdPASS;
    extern bool bMQTTconnected;

    strncpy( ( char * ) pxUserData->cTopic, ( char * ) pcTaskGetName( NULL ), subpubCHAR_TASK_NAME_MAX_SIZE );
    strncat( ( char * ) pxUserData->cTopic, ( char * ) subpubSTRING_TOPIC_PATH, sizeof( subpubSTRING_TOPIC_PATH ) );

    printf(" [%s , %d] xResult is %d \n",__FUNCTION__,__LINE__,xResult);

    for(;;)
    {
        if( MQTT_AGENT_Connect( xMQTTClientHandle, pxConnectParams, xMaxCommandTime ) == eMQTTAgentSuccess )
        {
            printf(" [%s , %d] xResult is %d \n",__FUNCTION__,__LINE__,xResult);
            printf( ( " connected to broker.\r\n" ) );
            
            /* Connected to the broker, subscribe to a topic that has the same name
             * as this function.  As this function sends strings the topic name uses
             * the subpubSTRING_TOPIC_PATH path so the callback knows how to interpret
             * received data. */
            xSubscribeParams.pucTopic = ( const uint8_t * ) pxUserData->cTopic;
            xSubscribeParams.usTopicLength = ( uint16_t ) strlen( pxUserData->cTopic );
            xSubscribeParams.xQoS = xQOS;
            xSubscribeParams.pvPublishCallbackContext = ( void * ) pxUserData;
            xSubscribeParams.pxPublishCallback = &( prvMQTTStringPublishCallback );

            if( MQTT_AGENT_Subscribe( xMQTTClientHandle, &xSubscribeParams, xMaxCommandTime ) == eMQTTAgentSuccess )
            {
                printf("%s subscribed to topic %s.\r\n", __FUNCTION__,(char *) (xPublishParams.pucTopic)  );
                printf(" [%s , %d] xResult is %d \n",__FUNCTION__,__LINE__,xResult);

                /* Publish to the topic to which this task is subscribed in order
                 * to receive back the data that was published. */
                xPublishParams.xQoS = xQOS;
                xPublishParams.pucTopic = ( const uint8_t * ) pxUserData->cTopic;
                xPublishParams.usTopicLength = ( uint16_t ) ( strlen( pxUserData->cTopic ) );
                xPublishParams.pvData = __FUNCTION__;
                xPublishParams.ulDataLength = ( uint16_t ) strlen( __FUNCTION__ );

                /* Used by the callback to check publishes received by this task
                 * match the publishes sent by this task - as the task both publishes
                 * to and subscribes to the same topic.  The first published string is
                 * just the name of this function. */
                pxUserData->pcExpectedString = __FUNCTION__;
                pxUserData->xCallbackStatus = pdFAIL;
                

                if( MQTT_AGENT_Publish( xMQTTClientHandle, &xPublishParams, xMaxCommandTime ) == eMQTTAgentSuccess )
                {
                    printf( "%s published #[1] to topic %s\r\n", __FUNCTION__, (char *) (xPublishParams.pucTopic) );
                }
                else
                {
                    printf( "ERROR %s did not published to topic %s\r\n", __FUNCTION__, (char *) (xPublishParams.pucTopic) );
                    xResult = pdFAIL;
                }

                /* The event callback will set bit 0 in this event group when it executes,
                 * until then wait here. */
                if( xEventGroupWaitBits( ( EventGroupHandle_t ) &( pxUserData->xWakeUpEventGroup ),
                                         1,
                                         pdTRUE,
                                         pdTRUE,
                                         xMaxCommandTime ) != 1 )
                {
                    xResult = pdFAIL;
                }

                /* Did the callback execute and pass? */
                if( pxUserData->xCallbackStatus != pdPASS )
                {
                    xResult = pdFAIL;
                }

                /* Perform another publish, but this time using a string that is
                 * too long for the string's length to be encoded as a single byte in
                 * the MQTT header. */

                
                pxUserData->pcExpectedString = pcLongString;
                xPublishParams.pvData = pcLongString;
                xPublishParams.ulDataLength = ( uint16_t ) strlen( pcLongString );
                pxUserData->xCallbackStatus = pdFAIL;

                if( MQTT_AGENT_Publish( xMQTTClientHandle, &xPublishParams, xMaxCommandTime ) == eMQTTAgentSuccess )
                {
                    printf( "%s published to topic %s\r\n", __FUNCTION__,(char *) (xPublishParams.pucTopic) );
                }
                else
                {
                    printf( "ERROR %s did not published to topic %s\r\n", __FUNCTION__, (char *) (xPublishParams.pucTopic) );
                    xResult = pdFAIL;
                }

                    /* The event callback will set bit 0 in this event group when it executes,
                     * until then wait here. */
                if( xEventGroupWaitBits( ( EventGroupHandle_t ) &( pxUserData->xWakeUpEventGroup ),
                                             1,
                                             pdTRUE,
                                             pdTRUE,
                                        xMaxCommandTime ) != 1 )
                {
                    xResult = pdFAIL;
                }

                /* Did the callback execute and pass? */
                if( pxUserData->xCallbackStatus != pdPASS )
                {
                    xResult = pdFAIL;
                }
                 

                


                #if 0
                /* Clean up the subscription. */
                xUnsubscribeParams.pucTopic = ( const uint8_t * ) pxUserData->cTopic;
                xUnsubscribeParams.usTopicLength = ( uint16_t ) strlen( pxUserData->cTopic );

                if( MQTT_AGENT_Unsubscribe( xMQTTClientHandle, &xUnsubscribeParams, xMaxCommandTime ) == eMQTTAgentSuccess )
                {
                    printf(  "%s unsubscribed from topic %s\r\n", __FUNCTION__, (char *) (xPublishParams.pucTopic)  );
                }
                else
                {
                    printf(  "ERROR:  %s did not unsubscribed from topic %s\r\n", __FUNCTION__, (char *) (xPublishParams.pucTopic)  );
                    xResult = pdFAIL;
                }
                #endif
               

            }
            else
            {
                printf(  "ERROR %s did not subscribe to topic %s.\r\n", __FUNCTION__, (char *) (xPublishParams.pucTopic)  );
            }

            printf( "Enter While loop for patch AWS connection check %s\r\n", __FUNCTION__);


            
            while(1)
            {
                if(bMQTTconnected == true)
                {
                    if(nTestLightSetting % 2 == 0)
                    {
                        pxUserData->pcExpectedString = pcPeriodLightON;
                        xPublishParams.pvData = pcPeriodLightON;
                        xPublishParams.ulDataLength = ( uint16_t ) strlen( pcPeriodLightON );
                        pxUserData->xCallbackStatus = pdFAIL;
                    }
                    else
                    {
                        pxUserData->pcExpectedString = pcPeriodLightOFF;
                        xPublishParams.pvData = pcPeriodLightOFF;
                        xPublishParams.ulDataLength = ( uint16_t ) strlen( pcPeriodLightOFF );
                        pxUserData->xCallbackStatus = pdFAIL;
                    }

                    printf( " ++++++ Before publish to topic [%s = %s]  ++++++ \r\n",(char *) (xPublishParams.pucTopic), (char *) xPublishParams.pvData);

                    
                    if( MQTT_AGENT_Publish( xMQTTClientHandle, &xPublishParams, xMaxCommandTime ) == eMQTTAgentSuccess )
                    {
                        printf( "-------- published to topic [%s = %s] ---------\r\n", (char *) (xPublishParams.pucTopic), (char *) xPublishParams.pvData);
                    }
                    else
                    {
                        printf( "ERROR %s did not published to topic %s\r\n", __FUNCTION__, (char *) (xPublishParams.pucTopic) );
                    }
                    
                    
                    nTestLightSetting++;
                    
                    /* Perform peroid publish, but this time using a string that is
                    * too long for the string's length to be encoded as a single byte in
                    * the MQTT header. */
                    osDelay(nPublishInterval);
                }
                else 
                {
                    break;
                }
            }
                

            /* Clean up the connection. */
            configPRINTF( ( "Disconnecting from broker.\r\n" ) );

            if( MQTT_AGENT_Disconnect( xMQTTClientHandle, xMaxCommandTime ) == eMQTTAgentSuccess )
            {
                printf( "Disconnected from the broker.\r\n" );
            }
            else
            {
                printf(  "ERROR:  Did not disconnected from the broker.\r\n"  );
                xResult = pdFAIL;
            }
        }

    }
    
    //return xResult;
}
/*-----------------------------------------------------------*/

static void prvSubscribePublishDemo( MQTTAgentHandle_t xMQTTClientHandle,
                                     BaseType_t xSecureConnection )
{
    BaseType_t xResult;
    MQTTAgentConnectParams_t xConnectParams;
    SubpubUserData_t xUserData;

    /* Create the event group used to synchronize tasks and callback functions.
     * This function will not fail when its argument isn't NULL. */
    ( void ) xEventGroupCreateStatic( &( xUserData.xWakeUpEventGroup ) );

    /* Each function below connects then disconnects from the broker.  To save
     * each from having to setup the connect parameters the parameters are set up
     * here and passed into the functions. */
    xConnectParams.pucClientId = ( const uint8_t * ) pcTaskGetName( NULL );
    xConnectParams.usClientIdLength = ( uint16_t ) ( strlen( clientcredentialIOT_THING_NAME ) );

    xConnectParams.usPort = clientcredentialMQTT_BROKER_PORT;
    xConnectParams.xFlags = 0;
    xConnectParams.xURLIsIPAddress = pdFALSE;    /* Deprecated. */
    xConnectParams.xSecuredConnection = pdFALSE; /* Deprecated. */
    xConnectParams.pcCertificate = NULL;
    xConnectParams.ulCertificateSize = 0;
    xConnectParams.pvUserData = NULL;
    xConnectParams.pxCallback = &( prvMQTTCallback );

    if( xSecureConnection == pdTRUE )
    {
        xConnectParams.xFlags |= mqttagentREQUIRE_TLS;
        xConnectParams.pcURL = clientcredentialMQTT_BROKER_ENDPOINT;
        xConnectParams.usPort = clientcredentialMQTT_BROKER_PORT;
    }
    else
    {
        xConnectParams.pcURL = subpubUNSECURE_BROKER_ENDPOINT;
        xConnectParams.usPort = subpubUNSECURE_BROKER_PORT;
    }

    /* Call the functions that subscribe to then publish to topics.  These
     * include strings and integers, and at QoS 0 and QoS 1. */

    printf("\033[42;31;4m [%s , %d]  xSecureConnection is %d \033[0m\n\n", __FUNCTION__,__LINE__,xSecureConnection);

    
    //xResult = prvStringPublishSubscribe( &( xConnectParams ), eMQTTQoS0, xMQTTClientHandle, &( xUserData ) );

    //printf(" [%s , %d] prvStringPublishSubscribe  eMQTTQoS0 xResult is %d \n",__FUNCTION__,__LINE__,xResult);
    //configASSERT( xResult );

    //if( xResult != pdFAIL )
    {
        xResult = prvStringPublishSubscribe( &( xConnectParams ), eMQTTQoS1, xMQTTClientHandle, &( xUserData ) );
    }

    printf(" [%s , %d] prvStringPublishSubscribe  eMQTTQoS1 xResult is %d \n",__FUNCTION__,__LINE__,xResult);


    
    #if 0
    
    if( xResult != pdFAIL )
    {
        xResult = prvUint32PublishSubscribe( &( xConnectParams ), eMQTTQoS0, xMQTTClientHandle, &( xUserData ) );
    }

    printf(" [%s , %d] prvUint32PublishSubscribe  eMQTTQoS 0 xResult is %d \n",__FUNCTION__,__LINE__,xResult);

    if( xResult != pdFAIL )
    {
        xResult = prvUint32PublishSubscribe( &( xConnectParams ), eMQTTQoS1, xMQTTClientHandle, &( xUserData ) );
    }
    printf(" [%s , %d] prvUint32PublishSubscribe  eMQTTQoS 1 xResult is %d \n",__FUNCTION__,__LINE__,xResult);
   
    #endif
    
    if( xResult == pdPASS )
    {
        /*xResult = prvVerifySignedData();*/
        printf(" [%s , %d] prvStringPublishSubscribe Result is  OK\n",__FUNCTION__,__LINE__);
    }
   

    if( xResult == pdPASS )
    {
        #if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
            {
                TaskStatus_t xTaskStatus;
                /* Report on space efficiency of this demo task. */
                vTaskGetInfo( NULL, &xTaskStatus, pdTRUE, eInvalid );
                printf( ( "Heap low-water mark %u, Stack high-water mark %u.\r\n",
                                xPortGetMinimumEverFreeHeapSize(),
                                xTaskStatus.usStackHighWaterMark ) );
            }
        #endif
        printf(" [%s , %d] xResult is %d \n",__FUNCTION__,__LINE__,xResult);

    }
    else
    {
        printf(" [%s , %d] xResult is %d \n",__FUNCTION__,__LINE__,xResult);
    }

    /* All done.  FreeRTOS does not allow a task to run off the end of its
     * implementing function, so the task must be deleted. */
    printf( ( "Task MQTT_Functions completed and deleting itself.\r\n" ) );
    printf(" [%s , %d] xResult is %d \n",__FUNCTION__,__LINE__,xResult);

    configASSERT( xResult == pdPASS );
    ( void ) xResult;
}
/*-----------------------------------------------------------*/

void prvPublishSubscribeTask( void * pvParameters )
{
    BaseType_t xSecureConnection;
    //BaseType_t xResult;
    /* The MQTT client used for all the publish and subscribes. */
    MQTTAgentHandle_t xMQTTClientHandle;

    /* pdTRUE is passed as parameter for secure tasks and pdFALSE is
     * passed for unsecure tasks. */
    xSecureConnection = ( BaseType_t ) pvParameters;

    configPRINTF( ( "Creating MQTT Client...\r\n" ) );

    while (1)
    {
        if (true == BleWifi_Ctrl_EventStatusWait(BLEWIFI_CTRL_EVENT_BIT_GOT_IP, 0xFFFFFFFF))
        {
            // init behavior
            printf("==== Got ip ==== \n");
            Hal_Pwm_Init();
            Hal_Pwm_ClockSourceSet(HAL_PWM_CLK_22M);
            break;
         
        }

        else
        {
            printf("==== Not got ip ==== \n");
        }
        osDelay(1000);
    }

    
    /* Create MQTT Client.  This is used for all publishes and subscribes
     * performed by this demo. */
    if( MQTT_AGENT_Create( &( xMQTTClientHandle ) ) == eMQTTAgentSuccess )
    {
        configPRINTF( ( "MQTT Client Created.\r\n" ) );
        prvSubscribePublishDemo( xMQTTClientHandle, xSecureConnection );       
    }
    else
    {
        configPRINTF( ( "ERROR:  MQTT_AGENT_Create() Failed.\r\n" ) );
    }

    if( MQTT_AGENT_Delete( xMQTTClientHandle ) == eMQTTAgentSuccess )
    {
        configPRINTF( ( "Deleted Client.\r\n" ) );
    }
    else
    {
        configPRINTF( ( "ERROR:  MQTT_AGENT_Delete() Failed.\r\n" ) );
    }

    /* This task has finished.  FreeRTOS does not allow a task to run off the
     * end of its implementing function, so the task must be deleted. */
    vTaskDelete( NULL );
}
/*-----------------------------------------------------------*/

void vStartSubpubDemoTasks( void )
{
    int8_t x;
    char cTaskName[ subpubCHAR_TASK_NAME_MAX_SIZE ];
    /* Create the pub sub tasks which connect to the unsecure broker. */
    
    for( x = 0; x < democonfigMQTT_SUB_PUB_NUM_UNSECURE_TASKS; x++ )
    {
        snprintf( cTaskName, subpubCHAR_TASK_NAME_MAX_SIZE, subpubCHAR_TASK_NAME, x );
        xTaskCreate( prvPublishSubscribeTask,
                     cTaskName,
                     democonfigMQTT_SUB_PUB_TASK_STACK_SIZE,
                     ( void * ) pdFALSE, democonfigMQTT_SUB_PUB_TASK_PRIORITY,
                     NULL );
    }

    /* Create the pub sub tasks which connect to the secure AWS IoT broker. */
    for( x = democonfigMQTT_SUB_PUB_NUM_UNSECURE_TASKS;
         x < ( democonfigMQTT_SUB_PUB_NUM_UNSECURE_TASKS + democonfigMQTT_SUB_PUB_NUM_SECURE_TASKS );
         x++ )
    {
        snprintf( cTaskName, subpubCHAR_TASK_NAME_MAX_SIZE, subpubCHAR_TASK_NAME, x );
        xTaskCreate( prvPublishSubscribeTask,
                     cTaskName,
                     democonfigMQTT_SUB_PUB_TASK_STACK_SIZE,
                     ( void * ) pdTRUE, democonfigMQTT_SUB_PUB_TASK_PRIORITY, NULL );
    }

}
/*-----------------------------------------------------------*/
