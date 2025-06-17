# FreeModbus V1.6 Master Usage Guide

## 1. Overview

FreeModbus is an open-source Modbus protocol stack, but only the slave
is open-source; the master source code is **commercial**. There are no
good open-source Modbus master protocol stacks available online, so this
project was developed to support master mode for FreeModbus. The version
number is updated to V1.6, with the following features:

- The newly added master source code is consistent in style and
  interface with the original slave;
- Supports running master and slave in the same protocol stack;
- Supports both RTOS and bare-metal porting;
- Provides multiple request modes for applications, allowing users to
  choose blocking or non-blocking modes, customize timeout, etc., for
  flexible application layer calls;
- Supports all common Modbus functions.

### 1.1 File Structure

| Source File                                   | Description                                                                   |
|:----------------------------------------------|:------------------------------------------------------------------------------|
| FreeModbus\modbus\mb.c                        | Provides Modbus slave setup and polling interfaces for the application layer  |
| FreeModbus\modbus\mb_m.c                      | Provides Modbus master setup and polling interfaces for the application layer |
| FreeModbus\modbus\ascii\mbascii.c             | ASCII mode setup and state machine                                            |
| FreeModbus\modbus\functions\mbfunccoils.c     | Slave coil-related functions                                                  |
| FreeModbus\modbus\functions\mbfunccoils_m.c   | Master coil-related functions                                                 |
| FreeModbus\modbus\functions\mbfuncdisc.c      | Slave discrete input-related functions                                        |
| FreeModbus\modbus\functions\mbfuncdisc_m.c    | Master discrete input-related functions                                       |
| FreeModbus\modbus\functions\mbfuncholding.c   | Slave holding register-related functions                                      |
| FreeModbus\modbus\functions\mbfuncholding_m.c | Master holding register-related functions                                     |
| FreeModbus\modbus\functions\mbfuncinput.c     | Slave input register-related functions                                        |
| FreeModbus\modbus\functions\mbfuncinput_m.c   | Master input register-related functions                                       |
| FreeModbus\modbus\functions\mbfuncother.c     | Other Modbus functions                                                        |
| FreeModbus\modbus\functions\mbutils.c         | Utility functions used in the protocol stack                                  |
| FreeModbus\modbus\rtu\mbcrc.c                 | CRC check functions                                                           |
| FreeModbus\modbus\rtu\mbrtu.c                 | Slave RTU mode setup and state machine                                        |
| FreeModbus\modbus\rtu\mbrtu_m.c               | Master RTU mode setup and state machine                                       |
| FreeModbus\modbus\tcp\mbtcp.c                 | TCP mode setup and state machine                                              |
| FreeModbus\port\port.c                        | Implements hardware porting interfaces                                        |
| FreeModbus\port\portevent.c                   | Implements slave event porting interface                                      |
| FreeModbus\port\portevent_m.c                 | Implements master event and error handling porting interface                  |
| FreeModbus\port\portserial.c                  | Slave serial port porting                                                     |
| FreeModbus\port\portserial_m.c                | Master serial port porting                                                    |
| FreeModbus\port\porttimer.c                   | Slave timer porting                                                           |
| FreeModbus\port\porttimer_m.c                 | Master timer porting                                                          |
| FreeModbus\port\user_mb_app.c                 | Defines slave data buffer and implements slave Modbus callback interfaces     |
| FreeModbus\port\user_mb_app_m.c               | Defines master data buffer and implements master Modbus callback interfaces   |

> Note: All files with the _m suffix are required for master mode. If
> using slave mode, these files are not needed.

----------

## 2. Porting

Porting the protocol stack mainly involves two aspects: hardware and
software. Users need to choose according to their own needs.

> Note: All instructions below are mainly for Modbus master mode.

### 2.1 Software

Software supports both bare-metal and RTOS porting; supports running a
single master and a single slave independently at the same time. Users
can also modify the protocol stack's event callback interfaces to use
blocking or non-blocking modes for master requests; for master resource
waiting, users can set the wait timeout, etc. Many features will be
introduced one by one.

#### 2.1.1 RTOS and Bare-Metal

Both RTOS and bare-metal are supported, but using an RTOS is
recommended, as it makes interface calls and porting easier. Currently
supported RTOSes include [RT-Thread][1] (see project source code), UCOS,
and FreeRTOS.

Files involved in RTOS and bare-metal porting:
`FreeModbus\port\portevent_m.c`

The following interfaces in this file need to be ported by the user:

| Interface                       | Description                                   |
|:--------------------------------|:----------------------------------------------|
| xMBMasterPortEventInit          | Master event initialization                   |
| xMBMasterPortEventPost          | Master post event                             |
| xMBMasterPortEventGet           | Master get event                              |
| vMBMasterOsResInit              | Master OS resource initialization             |
| xMBMasterRunResTake             | Master resource acquire                       |
| vMBMasterRunResRelease          | Master resource release                       |
| vMBMasterErrorCBRespondTimeout  | Master response timeout callback              |
| vMBMasterErrorCBReceiveData     | Master receive data error callback            |
| vMBMasterErrorCBExecuteFunction | Master execute Modbus function error callback |
| vMBMasterCBRequestScuuess       | Master request success callback               |
| eMBMasterWaitRequestFinish      | Master wait for request completion callback   |

**For RTOS porting**, mainly use OS thread synchronization techniques.
The Modbus protocol stack itself needs to use the OS's event mechanism
for event notification and waiting. The user thread requesting Modbus
functions and the Modbus protocol stack's own thread (Modbus Poll
thread) need to synchronize via events. The master protocol stack also
needs a semaphore for master resource occupation, initialized to 1, to
ensure that only one thread can use the master when multiple threads
send requests simultaneously.

**For bare-metal porting**, software simulation is needed for event
notification, event waiting, and resource waiting, using user-defined
delays and flag variables. This is more complex than RTOS thread
synchronization.

#### 2.1.2 Data Buffer

The data buffer is defined at the top of
`FreeModbus\port\user_mb_app_m.c`, with **4 types** of data.

The FreeModbus slave uses a **one-dimensional array** as the buffer by
default. The master can store data for all slaves on the network, so the
master uses a **two-dimensional array** to store data for all slave
nodes. The column number represents the register, coil, or discrete
input address, and the row number represents the slave node ID (minus
one). For example, `usMRegHoldBuf[2][1]` represents the data for slave
ID 3, holding register address 1.

#### 2.1.3 Modbus Data Processing Callback Interfaces

There are 4 types of Modbus data, and all Modbus functions operate
around these types. Since user data buffer structures may differ, the
corresponding Modbus data processing methods may also differ, so users
need to implement each data type's operations according to their own
buffer structure.

All Modbus data processing callback interfaces are as follows:

| Interface              | Description               |
|:-----------------------|:--------------------------|
| eMBMasterRegInputCB    | Input register callback   |
| eMBMasterRegHoldingCB  | Holding register callback |
| eMBMasterRegCoilsCB    | Coil callback             |
| eMBMasterRegDiscreteCB | Discrete input callback   |

> For array-based buffer structures, the source code is already ported
> and can be used directly. You can also use the
> [EasyDataManager](https://github.com/armink/EasyDataManager) library,
> which uses a linked list as the buffer and supports event-driven
> notifications to the application layer on data change.

### 2.2 Hardware

When porting the FreeModbus master, you need to modify the serial port
and timer configuration in the port folder according to your CPU.

> Note: The protocol stack comes with STM32F103X porting files by
> default for reference.

For RTOS device driver framework porting, the protocol stack will later
add support for the [RT-Thread][1] device driver framework. As long as
the MCU is supported by RT-Thread BSP, users do not need to worry about
low-level porting, reducing porting costs.

#### 2.2.1 Serial Port

Serial port porting file: `FreeModbus\port\portserial_m.c`. In this
file, users need to modify the following interfaces:

| Interface                  | Description                                                                  |
|:---------------------------|:-----------------------------------------------------------------------------|
| vMBMasterPortSerialEnable  | Enable/disable serial port TX/RX; for RS485, handle TX/RX mode switching     |
| vMBMasterPortClose         | Close serial port                                                            |
| xMBMasterPortSerialInit    | Serial port initialization; for RS485, also initialize TX/RX mode pin        |
| xMBMasterPortSerialPutByte | Send a byte via serial port                                                  |
| xMBMasterPortSerialGetByte | Receive a byte via serial port                                               |
| prvvUARTTxReadyISR         | Serial TX complete ISR; by default, call `pxMBMasterFrameCBTransmitterEmpty` |
| prvvUARTRxISR              | Serial RX ISR; by default, call `pxMBMasterFrameCBByteReceived`              |

> At the end of the file, add your CPU's serial port ISRs, and call the
> above TX/RX ISR interfaces in the corresponding ISRs.

#### 2.2.2 Timer

Timer porting file: `FreeModbus\port\porttimer_m.c`. In this file, users
need to modify the following interfaces:

| Interface                               | Description                                                                                          |
|:----------------------------------------|:-----------------------------------------------------------------------------------------------------|
| xMBMasterPortTimersInit                 | Timer initialization; backup prescaler and T3.5 timeout to `usPrescalerValue` and `usT35TimeOut50us` |
| vMBMasterPortTimersT35Enable            | Start timer for T3.5 interval                                                                        |
| vMBMasterPortTimersConvertDelayEnable   | Start timer for broadcast frame conversion delay                                                     |
| vMBMasterPortTimersRespondTimeoutEnable | Start timer for response timeout                                                                     |
| vMBMasterPortTimersDisable              | Disable timer                                                                                        |
| prvvTIMERExpiredISR                     | Timer ISR; by default, call `pxMBMasterPortCBTimerExpired`                                           |

> Note:
> 1. `usPrescalerValue` and `usT35TimeOut50us` are defined at the top of
> the file.
> 2. Conversion delay and response timeout are set in
> `FreeModbus\modbus\include\mbconfig.h`; users can adjust as needed.

In addition, add your CPU's timer ISR at the end of the file and call
the above timer ISR interface.

## 3. API

The Modbus master is very different from the slave: the slave passively
waits for master requests, while the master actively sends requests and
processes slave responses. When the master sends a broadcast request,
the slave does not need to respond, so broadcast requests are suitable
for master write commands, not for read commands.

All master request API methods return the same type, with the following
meanings:

| Return Value       | Description                                              |
|:-------------------|:---------------------------------------------------------|
| MB_MRE_NO_ERR      | No error                                                 |
| MB_MRE_NO_REG      | Register, coil, or discrete input address error          |
| MB_MRE_ILL_ARG     | Invalid argument                                         |
| MB_MRE_REV_DATA    | Received data error                                      |
| MB_MRE_TIMEDOUT    | Response timeout: no response from slave within timeout  |
| MB_MRE_MASTER_BUSY | Master busy: request not sent within timeout             |
| MB_MRE_EXE_FUN     | Error executing Modbus function after receiving response |

> All master request methods are **thread-safe** and **blocking**. If
> the master resource is not acquired within the timeout, returns master
> busy; if acquired, waits for the request result before returning.

### 3.1 Write Single Holding Register

Write data to a holding register of a slave

```C
eMBMasterReqErrCode eMBMasterReqWriteHoldingRegister( UCHAR ucSndAddr, 
                                                      USHORT usRegAddr,
                                                      USHORT usRegData,
                                                      LONG lTimeOut );
```

| Parameter | Description                                                    |
|:----------|:---------------------------------------------------------------|
| ucSndAddr | Slave address; 0 for broadcast                                 |
| usRegAddr | Register address                                               |
| usRegData | Register data                                                  |
| lTimeOut  | Timeout; supports infinite wait (use OS's infinite wait param) |

### 3.2 Write Multiple Holding Registers

Write data to multiple holding registers of a slave

```C
eMBMasterReqErrCode eMBMasterReqWriteMultipleHoldingRegister( UCHAR ucSndAddr,
                                                              USHORT usRegAddr,
                                                              USHORT usNRegs, 
                                                              USHORT * pusDataBuffer,
                                                              LONG lTimeOut )
```

| Parameter     | Description                     |
|:--------------|:--------------------------------|
| ucSndAddr     | Slave address; 0 for broadcast  |
| usRegAddr     | Start register address          |
| usNRegs       | Number of registers             |
| pusDataBuffer | Register data                   |
| lTimeOut      | Timeout; supports infinite wait |

### 3.3 Read Multiple Holding Registers

Read data from multiple holding registers

```C
eMBMasterReqErrCode eMBMasterReqReadHoldingRegister( UCHAR ucSndAddr, 
                                                     USHORT usRegAddr,
                                                     USHORT usNRegs,
                                                     LONG lTimeOut );
```

| Parameter | Description                     |
|:----------|:--------------------------------|
| ucSndAddr | Slave address; 0 for broadcast  |
| usRegAddr | Register address                |
| usRegData | Number of registers             |
| lTimeOut  | Timeout; supports infinite wait |

### 3.4 Read/Write Multiple Holding Registers

Read then write multiple registers

```C
eMBMasterReqErrCode eMBMasterReqReadWriteMultipleHoldingRegister( UCHAR ucSndAddr,
                                                                  USHORT usReadRegAddr,
                                                                  USHORT usNReadRegs,
                                                                  USHORT * pusDataBuffer,
                                                                  USHORT usWriteRegAddr,
                                                                  USHORT usNWriteRegs,
                                                                  LONG lTimeOut )
```

| Parameter      | Description                     |
|:---------------|:--------------------------------|
| ucSndAddr      | Slave address; 0 for broadcast  |
| usReadRegAddr  | Read register address           |
| usNReadRegs    | Number of registers to read     |
| pusDataBuffer  | Register data to write          |
| usWriteRegAddr | Write register address          |
| usNWriteRegs   | Number of registers to write    |
| lTimeOut       | Timeout; supports infinite wait |

### 3.5 Read Multiple Input Registers

Read data from multiple input registers

```C
eMBMasterReqErrCode eMBMasterReqReadInputRegister( UCHAR ucSndAddr, 
                                                   USHORT usRegAddr,
                                                   USHORT usNRegs,
                                                   LONG lTimeOut );
```

| Parameter | Description                     |
|:----------|:--------------------------------|
| ucSndAddr | Slave address; 0 for broadcast  |
| usRegAddr | Register address                |
| usRegData | Number of registers             |
| lTimeOut  | Timeout; supports infinite wait |

### 3.6 Write Single Coil

Write data to a single coil of a slave

```C
eMBMasterReqErrCode eMBMasterReqWriteCoil( UCHAR ucSndAddr,
                                           USHORT usCoilAddr,
                                           USHORT usCoilData,
                                           LONG lTimeOut )
```

| Parameter  | Description                     |
|:-----------|:--------------------------------|
| ucSndAddr  | Slave address; 0 for broadcast  |
| usCoilAddr | Coil address                    |
| usCoilData | Coil data                       |
| lTimeOut   | Timeout; supports infinite wait |

### 3.7 Write Multiple Coils

Write data to multiple coils of a slave

```C
eMBMasterReqErrCode eMBMasterReqWriteMultipleCoils( UCHAR ucSndAddr,
                                                    USHORT usCoilAddr,
                                                    USHORT usNCoils,
                                                    UCHAR * pucDataBuffer,
                                                    LONG lTimeOut)
```

| Parameter     | Description                     |
|:--------------|:--------------------------------|
| ucSndAddr     | Slave address; 0 for broadcast  |
| usCoilAddr    | Start coil address              |
| usNCoils      | Number of coils                 |
| pucDataBuffer | Coil data                       |
| lTimeOut      | Timeout; supports infinite wait |

### 3.8 Read Multiple Coils

Read data from multiple coils

```C
eMBMasterReqErrCode eMBMasterReqReadCoils( UCHAR ucSndAddr,
                                           USHORT usCoilAddr,
                                           USHORT usNCoils ,
                                           LONG lTimeOut )
```

| Parameter  | Description                     |
|:-----------|:--------------------------------|
| ucSndAddr  | Slave address; 0 for broadcast  |
| usCoilAddr | Coil address                    |
| usNCoils   | Number of coils                 |
| lTimeOut   | Timeout; supports infinite wait |

### 3.9 Read Multiple Discrete Inputs

Read data from multiple discrete inputs

```C
eMBMasterReqErrCode eMBMasterReqReadDiscreteInputs( UCHAR ucSndAddr,
                                                    USHORT usDiscreteAddr,
                                                    USHORT usNDiscreteIn,
                                                    LONG lTimeOut )
```

| Parameter      | Description                     |
|:---------------|:--------------------------------|
| ucSndAddr      | Slave address; 0 for broadcast  |
| usDiscreteAddr | Discrete input address          |
| usNDiscreteIn  | Number of discrete inputs       |
| lTimeOut       | Timeout; supports infinite wait |

## 4. Workflow

### 4.1 Initialization Configuration Workflow

All configuration parameters are in
`FreeModbus\modbus\include\mbconfig.h`. The protocol stack supports both
master and slave modes, and both can be enabled at the same time. The
slave supports Modbus RTU, Modbus ASCII, and Modbus TCP; the master
currently only supports **Modbus RTU**. When using the master, configure
the broadcast conversion delay, command response timeout, and number of
slaves. Note: only **consecutive slave addresses** are supported,
starting from **1**.

### 4.2 Normal Usage Workflow

Only the master workflow is described here. Before using the master,
port the protocol stack to your project, including the software and
hardware parts above.

After porting, the usage workflow is:

1. Call `eMBMasterInit` to initialize the Modbus master protocol stack,
   including hardware initialization.
2. Call `eMBMasterEnable` to start the Modbus master.
3. In a thread or timer, periodically call `eMBMasterPoll`; the polling
   period determines command response time.
4. Call master request API methods, set a request timeout, and wait for
   the result. If successful and the command is a read, check the Modbus
   master's data buffer for the latest slave data.

### 4.3 Exception Handling Workflow

Exceptions mainly occur during normal master operation. All master
request API error codes are described at the start of Chapter 3. Users
should handle these errors according to their product requirements. It
is recommended to implement a retry mechanism for master requests,
especially for frame data errors and command response timeouts.
Increment the retry count automatically; if it exceeds the set value,
consider the slave offline and block further commands to it. If a retry
succeeds, **reset** the retry count for that slave.

All these features can be implemented using the master request methods
or the callback interfaces in `FreeModbus\port\portevent_m.c`, as
needed.

## 5. License

BSD license. See the LICENSE file for details.

[1]: http://www.rt-thread.org/
