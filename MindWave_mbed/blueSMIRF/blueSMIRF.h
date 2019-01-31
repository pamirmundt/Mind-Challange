/**
 * @file    blueSMIRF.h
 * @brief   blueSMIRF
 * @author  Pamir Mundt
 * @version 0.0
 * @see
 */

#ifndef BLUESMIRF_H
#define BLUESMIRF

#include "mbed.h"
#include "BufferedSerial.h"

/** blueSMIRF class.
 */
class blueSMIRF{
public:
    /** Create blueSMIRF instance
     */
    blueSMIRF(PinName tx, PinName rx)
    :serial(tx,rx){
        serial.baud(115200);   
    };
    
    /** SET COMMANDS **********************************************************/
    
    /** 7-Bit Data Mode Enable/Disable
     * @param mode 0=Disable, 1=Enable
     * @returns
     *   1 on success
     *   0 on error
     */
    int set7BitMode(int mode);
    
    /** Authentication Enable/Disable
     * @param auth 0, 1, 2, 4
     * @returns
     *   1 on success
     *   0 on error
     */
    int setAuthentication(int auth);
    
    /** Set break values
     * @param length 1=37ms, 2=18.5ms, 3=12ms, 4=9ms, 5=7ms, 6=6ms
     * @returns
     *   1 on success
     *   0 on error
     */
    int setBreak(int length);
    
    /** Sets the service class field in the class of device (COD)
     * @param value
     * @returns
     *   1 on success
     *   0 on error
     */
    int setServiceClass(int value);
    
    /** Sets the class of device (COD) LSW
     * @param value
     * @returns
     *   1 on success
     *   0 on error
     */
    int setDeviceClass(int value);
    
    /** Factory Defaults
     * @returns
     *   1 on success
     *   0 on error
     */
    int factoryDefaults(void);
    
    /** Mode
     * @param mode 0:Slave, 1:Master, 2:Trigger, 3:Auto, 4:DTR, 5:Any 
     * @returns
     *   1 on success
     *   0 on error
     */
    int setMode(int mode);
    
    /** Mode
     * @param name of device
     * @returns
     *   1 on success
     *   0 on error
     */
    int setName(char * name);
    
    /** Connect/Disconnect Status String
     * @param str
     * @returns
     *   1 on success
     *   0 on error
     */
    int setStatusString(char * str);
    
    /** Connect/Disconnect Status String
     * $param str PinCode
     * @returns
     *   1 on success
     *   0 on error
     */
    int setPinCode(char * pinCode);
    
    /** Special Configuration Settings
     *  @param value 0, 4, 8, 16, 128, 256
     * @returns
     *   1 on success
     *   0 on error
     */
    int setSpecialConf(int value);
    
    /** Special Configuration Settings
     *  @param char array
     * @returns
     *   1 on success
     *   0 on error
     */
    int setRemoteAddress(char * address);
    
    /** This command sets the remote configuration timer
     * @param value
     * @returns
     *   1 on success
     *   0 on error
     */
    int setConfTimer(int value);
    
    /** This command sets the remote configuration timer
     * @param baud 1200, 2400, 4800, 9600, 19.2, 28.8, 38.4, 57.6, 115K, 230K, 460K, or 921K
     *  You only need to specify the first 2 characters of the desired baud rate.
     * @returns
     *   1 on success
     *   0 on error
     */
    int setUART(char * baud);
    
    /** GET COMMANDS **********************************************************/
    
    /** Get Basic Settings
     * @param  responsechar array to data to return
     */
    void getBasicSettings(char * response);
    
    /** Get Extended Settings
     * @param response char array to data to return
     */
    void getExtendedSettings(char * response);
    
    /** Get Bluetooth Address
     * @param response char array to data to return
     */
    void getBluetoothAddress(char * response);
    
    /** Get Connection Status
     * @param response char array to data to return
     */
    void getConnectionStatus(char * response);
    
    /** Get Bluetooth Address of Currently or Most Recently Connected Active Remote Device
     * @param response char array to data to return
     */
    void getConnectedAdress(char * response);
    
    /** get Remote Address
     * @param response char array to data to return
     */
    void getRemoteAdress(char * response);
    
    /**  get hex byte containing the value of the GPIO pins
     * @param response char array to data to return
     */
    void getGPIOHex(char * response);
    
    /**  get Help
     * @param response char array to data to return
     */
    void getHelp(char * response);
    
    /**  get Remote Modem Signal Status
     * @param response char array to data to return
     */
    void getSignalStatus(char * response);
    
    /**  get other setting
     * @param response char array to data to return
     */
    void getOtherSettings(char * response);
    
    /**  get firmware version
     * @param response char array to data to return
     */
    void getFirmwareVer(char * response);
    
    /** ACTION COMMANDS *******************************************************/
    
    /** Enter Command Mode
     * @returns
     *   1 on success
     *   0 on error
     */
    int enterCMD(void);
    
    /** Exit Command Mode
     * @returns
     *   1 on success
     *   0 on error
     */
    int exitCMD(void);
    
    /** Toggle the Local Echo of RX Characters in Command Mode
     * @returns
     *   1 on ECHO ON
     *   2 on ECHO OFF
     *   0 on error
     */
    int toggleLocalEcho(void);
    
    /** Return the Dipswitch Values
     * @param response char array to data to return
     */
    void returnDipswitchValues(char * response);
    
    /** Connect Immediately to the Stored Remote Address
     * @returns
     *   1 on success
     *   0 on error
     */
    int connectStoredAddress(void);
    
    /** Connect to Address
     * @param address in HEX
     * @returns
     *   1 on success
     *   0 on error
     */
    int connectAddress(int address);
    
    /** Connect to Address in Fast Mode
     */
    void connectAddressFast(char * address);
    
    /** Connect and Immediately Go into Fast Data Mode Using Last Address Found
     */
    void connectLastAddressFast(void);
    
    /** Connect to Stored Remote Address in Fast Mode
     */
    void connectStoredAddressFast(void);
    
    /** Connect, Address Required, Optional Disconnect Timer in ¼ Seconds
     * @param address in HEX
     * @param value disconnect after seconds (param/4)
     * Default: 28 - 7 Seconds
     * e.g: 255 is 64 seconds.
     * @returns
     *   1 on success
     *   0 on error
     */
    int connectAddressTimer(int address, int value);
    
    /** Enter Fast Data Mode, End Configuration Immediately
     * @returns
     *   1 on success
     *   0 on error
     */
    int enterFastMode(void);
    
    /** Device Scan Inquiry, Time in Seconds, Optional COD Filter (0 = All)
     * @param time is the scan time in seconds
     * @param response char array to data to return
     * @param classCOD is the optional COD of the device class for which you are scanning
     */
    void deviceScan(int time, char * response, int classCOD = 0);
    
    /** Device Scan Inquiry, Returns NAMEs
     * @param time is the scan time in seconds
     * @param response char array to data to return
     * @param classCOD is the optional COD of the device class for which you are scanning
     */
    void deviceScanWithNames(int time, char * response, int classCOD);
    
    /** Scans for Devices and Returns their RSSI
     * @param response char array to data to return
     */
    void deviceScanWithRSSI(char * response);
    
    /** Device Scan Inquiry, Fixed COD (0x001F00) to Find Roving Networks Devices
     * @param time is the scan time in seconds
     * @param response char array to data to return
     */
    void deviceScanRovingNetwork(int time, char * response);
    
    /** Device Scan Inquiry, Fixed COD (0x0055AA) to Find Instant Cable Pairs
     * @param time is the scan time in seconds
     * @param response char array to data to return
     */
    void deviceScanCablePair(int time, char * response);
    
    /** Hides the Device’s Pin Code
     * @returns
     *   1 on success
     *   0 on error
     */
    int hidePin(void);
    
    /** Kill (Disconnect) from Current Connection
     * @returns
     *   1 on success
     *   0 on error
     */
    int killConnection(void);
    
    /** Toggle Link Quality Readings
     * @param response char array to data to return
     * @returns
     *   1 on success
     *   0 on error
     */
    int toggleLinkQuality(char * response);
    
    /** Pass through Any Character up to a Carriage Return or Line Feed.
     */
    void passCharacter(char c);
    
    /** Quiet, Turn off Discovery and Connectability
     * @param mode NULL, 0, 1, 2
     * @returns
     *   1 on success
     *   0 on error
     */
    int quiteMode(int mode);
    
    /** Reboot
     * @returns
     *   1 on success
     *   0 on error
     */
    int reboot(void);
    
    /** Pass Received Data (from UART or Bluetooth) while in Command Mode
     * @param mode 0:Disable, 1:Enable
     * @returns
     *   1 on success
     *   0 on error
     */
    int passReceived(int mode);
    
    /** Temporary UART Change
     * @param baud  1200, 2400, 4800, 9600, 19.2K, 38.4K, 57.6K, 115K, 230K, 460K, or 921K
     * @param parity E:Even, O:Odd, N:None
     * @returns
     *   1 on success
     *   0 on error
     */
    int changeTempUART(char * baud, char parity);
    
    /** Re-Enable Discovery and Connectability
     * @returns
     *   1 on success
     *   0 on error
     */
    int enableDiscovery(void);
    
    /** Enter Low-Power Sleep Mode
     */
    void enterLowPowerSleep(void);
    
    /** OTHER COMMANDS ********************************************************/
    
    /** Read Byte Blocking Mode
     *  @param char array to send
     */
    int readByteBlocking(void);
    
    /** Read Byte Timeout Mode
     *  @param char array to send
     */
    int readByteTimeout(void);
    
    /** Print
     *  @param char array to send
     */
    void printf(char * msg);
    
    void putc(int test);
    
    /** Read Response
     *  @param char array to data to return
     */
    void readResponse(char* response);
    
    /** Flush Serial
     */
    void flushSerial(void);

    /** Set Baud
     */
    void setBaud(int baud);
    
private:
    BufferedSerial serial;
};

#endif
