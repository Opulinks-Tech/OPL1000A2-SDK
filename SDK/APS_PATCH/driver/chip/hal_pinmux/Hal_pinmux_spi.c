/*
 * bsp_usart.c
 *
 * Copyright (c) 2018, Opulinks Technology Ltd. All rights reserved.
 */

//#include "hal_spi.h"

#include "Hal_pinmux_spi.h"

typedef struct
{
    E_OPLXXX_Periph cs;
    E_OPLXXX_Periph clk;
    E_OPLXXX_Periph mosi;
    E_OPLXXX_Periph miso;
}T_Spi_Map;

T_Spi_Map spi_map[]=
{
    {SPI0_CS_E,SPI0_CLK_E,SPI0_MOSI_E,SPI0_MISO_E},
    {SPI1_CS_E,SPI1_CLK_E,SPI1_MOSI_E,SPI1_MISO_E},
    {SPI2_CS_E,SPI2_CLK_E,SPI2_MOSI_E,SPI2_MISO_E},
};

ERR_CODE Hal_Pinmux_Spi_Init(T_OPL1000_Spi *spi)
{
    // Initial SPI 1/2 module
    if(spi->index < SPI_IDX_MAX)
    {
        Hal_Spi_Init(
            spi->index,
            spi->baudrate,
            spi->polar,
            spi->phase,
            spi->format,
            spi->size,
            spi->qMode
        );

        Hal_PinMux_OutPutConfig(Hal_Get_Point(spi->cs,spi_map[spi->index].cs));
        Hal_PinMux_OutPutConfig(Hal_Get_Point(spi->clk,spi_map[spi->index].clk));  
        Hal_PinMux_OutPutConfig(Hal_Get_Point(spi->mosi,spi_map[spi->index].mosi));
        Hal_PinMux_OutPutConfig(Hal_Get_Point(spi->miso,spi_map[spi->index].miso));

        Hal_PinMux_InPutConfig(Hal_Get_Point(spi->mosi,spi_map[spi->index].mosi));
        Hal_PinMux_InPutConfig(Hal_Get_Point(spi->miso,spi_map[spi->index].miso));

        if(spi->qMode != QMODE_ENABLE)
        {
            HAL_PINMUX_PRINT("QMODE_ENABLE TBD \r\n");
        }
    }
    else
    {
        return RET_FAIL;
    }
    return RET_SUCCESS;
}




