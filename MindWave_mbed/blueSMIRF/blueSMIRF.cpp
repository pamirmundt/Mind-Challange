#include "blueSMIRF.h"

/** SET COMMANDS **************************************************************/

int blueSMIRF::set7BitMode(int mode){
    serial.printf("S7,%d\n", mode);
    char response [16] = {0};
    readResponse(response); //AOK
    if(strcmp(response, "AOK\r\n") == 0)
        return 1;
    else
        return 0;
}

int blueSMIRF::setAuthentication(int auth){
    serial.printf("SA,%d\n", auth);
    char response [16] = {0};
    readResponse(response); //AOK
    if(strcmp(response, "AOK\r\n") == 0)
        return 1;
    else
        return 0;
}

int blueSMIRF::setBreak(int length){
    serial.printf("SB,%d\n", length);
    char response [16] = {0};
    readResponse(response); //AOK
    if(strcmp(response, "AOK\r\n") == 0)
        return 1;
    else
        return 0;
}

int blueSMIRF::setServiceClass(int value){
    serial.printf("SC,%04x\n", value);
    char response [16] = {0};
    readResponse(response); //AOK
    if(strcmp(response, "AOK\r\n") == 0)
        return 1;
    else
        return 0; 
}

int blueSMIRF::setDeviceClass(int value){
    serial.printf("SD,%04x\n", value);
    char response [16] = {0};
    readResponse(response); //AOK
    if(strcmp(response, "AOK\r\n") == 0)
        return 1;
    else
        return 0; 
}

int blueSMIRF::factoryDefaults(void){
    serial.printf("SF,1\n");
    char response [16] = {0};
    readResponse(response); //AOK
    if(strcmp(response, "AOK\r\n") == 0)
        return 1;
    else
        return 0;
}

int blueSMIRF::setMode(int mode){
    serial.printf("SM,%d\n", mode);
    char response[16] = {0}; //AOK
    readResponse(response);
    if(strcmp(response, "AOK\r\n") == 0)
        return 1;
    else
        return 0; 
}

int blueSMIRF::setName(char * name){
    serial.printf("SN,%s\n", name);
    char response[16] = {0}; //AOK
    readResponse(response);
    if(strcmp(response, "AOK\r\n") == 0)
        return 1;
    else
        return 0; 
}

int blueSMIRF::setStatusString(char * str){
    serial.printf("SO,%s%\n", str);
    char response[16] = {0}; //AOK
    readResponse(response);
    if(strcmp(response, "AOK\r\n") == 0)
        return 1;
    else
        return 0;
}

int blueSMIRF::setPinCode(char * pinCode){
    serial.printf("SP,%s\n", pinCode);
    char response[16] = {0}; //AOK
    readResponse(response);
    if(strcmp(response, "AOK\r\n") == 0)
        return 1;
    else
        return 0;
}

int blueSMIRF::setSpecialConf(int value){
    serial.printf("SQ,%d\n", value);
    char response[16] = {0}; //AOK
    readResponse(response);
    if(strcmp(response, "AOK\r\n") == 0)
        return 1;
    else
        return 0;
}

int blueSMIRF::setRemoteAddress(char * address){
    serial.printf("SR,%s\n", address);
    char response[16] = {0}; //AOK
    readResponse(response);
    if(strcmp(response, "AOK\r\n") == 0)
        return 1;
    else
        return 0;
}

int blueSMIRF::setConfTimer(int value){
    serial.printf("ST,%d\n", value);
    char response[16] = {0}; //AOK
    readResponse(response);
    if(strcmp(response, "AOK\r\n") == 0)
        return 1;
    else
        return 0;
}

int blueSMIRF::setUART(char * baud){
    serial.printf("SU,%s\n", baud);
    char response[16] = {0};
    readResponse(response);
    if(strcmp(response, "AOK\r\n") == 0)
        return 1;
    else
        return 0;
}

/** GET COMMANDS **************************************************************/
void blueSMIRF::getBasicSettings(char * response){
     serial.printf("D\n");
     readResponse(response);
}

void blueSMIRF::getExtendedSettings(char * response){
     serial.printf("E\n");
     readResponse(response);
}

void blueSMIRF::getBluetoothAddress(char * response){
    serial.printf("GB\n");
    readResponse(response);   
}

void blueSMIRF::getConnectionStatus(char * response){
    serial.printf("GK\n");
    readResponse(response); 
}

void blueSMIRF::getConnectedAdress(char * response){
    serial.printf("GF\n");
    readResponse(response); 
}

void blueSMIRF::getRemoteAdress(char * response){
    serial.printf("GR\n");
    readResponse(response);
}

void blueSMIRF::getGPIOHex(char * response){
    serial.printf("G&\n");
    readResponse(response); 
}

void blueSMIRF::getHelp(char * response){
    serial.printf("H\n");
    readResponse(response);  
}

void blueSMIRF::getSignalStatus(char * response){
    serial.printf("M\n");
    readResponse(response); 
}

void blueSMIRF::getOtherSettings(char * response){
    serial.printf("O\n");
    readResponse(response); 
}

void blueSMIRF::getFirmwareVer(char * response){
    serial.printf("V\n");
    readResponse(response); 
}


/** ACTION COMMANDS ***********************************************************/
int blueSMIRF::enterCMD(void){
    serial.printf("$$$");
    char response[16] = {0};
    readResponse(response);
    if(strcmp(response, "CMD\r\n") == 0)
        return 1;
    else
        return 0;
}


int blueSMIRF::exitCMD(void){
    serial.printf("---\n");
    char response[16] = {0};
    readResponse(response);
    if(strcmp(response, "END\r\n") == 0)
        return 1;
    else
        return 0;
}
    
int blueSMIRF::toggleLocalEcho(void){
    serial.printf("+\n");
    char response[16] = {0};
    readResponse(response);
    if(strcmp(response, "ECHO ON\r\n") == 0)
        return 1;
    else if(strcmp(response, "ECHO OFF\r\n") == 0)
        return 2;
    else
        return 0;
}

void blueSMIRF::returnDipswitchValues(char * response){
    serial.printf("&\n");
    readResponse(response);
}

int blueSMIRF::connectStoredAddress(void){
    serial.printf("&\n");
    char response[16] = {0};
    readResponse(response);
    if(strcmp(response, "TRYING\r\n") == 0)
        return 1;
    else
        return 0;
}
//------------------------------------------>>>>
int blueSMIRF::connectAddress(int address){
    serial.printf("C,%12X\n", address);
    char response[16] = {0};
    readResponse(response);
    if(strcmp(response, "TRYING\r\n") == 0)
        return 1;
    else
        return 0;
}
 //------------------------------------------>>>>   
void blueSMIRF::connectAddressFast(char * address){
    serial.printf("CF,%s\n", address);
    //No Response
}

void blueSMIRF::connectLastAddressFast(void){
    serial.printf("CFI\n");
    //No Response
}

void blueSMIRF::connectStoredAddressFast(void){
    serial.printf("CFR\n");
    //No Response
}
    
int blueSMIRF::connectAddressTimer(int address, int value = 28){
    serial.printf("CT,%12X,%d\n", address, value);
    char response[16] = {0};
    readResponse(response);
    if(strcmp(response, "TRYING\r\n") == 0)
        return 1;
    else
        return 0;
}
    
int blueSMIRF::enterFastMode(void){
    serial.printf("F,1\n");
    char response[16] = {0};
    readResponse(response);
    if(strcmp(response, "TRYING\r\n") == 0)
        return 1;
    else
        return 0;
}

void blueSMIRF::deviceScan(int time, char * response, int classCOD){
    if(classCOD == 0)
        serial.printf("I,%d\n",time);
    else
        serial.printf("I,%d,%d\n",time ,classCOD);
    
    int delay = time + 10; //Extra 10seconds for BlueSmirf to print all devices
    wait(delay);
    
    readResponse(response);
}

void blueSMIRF::deviceScanWithNames(int time, char * response, int classCOD = 0){
    if(classCOD == 0)
        serial.printf("IN,%d\n",time);
    else
        serial.printf("IN,%d,%d\n",time ,classCOD);
    
    int delay = time + 10; //Extra 10seconds for BlueSmirf to print all devices
    wait(delay);
    
    readResponse(response);
}
    
void blueSMIRF::deviceScanWithRSSI(char * response){
    serial.printf("IQ\n");

    wait(17); // Wait 7 seconds + Extra 10 seconds for BlueSmirf to print all devices
    readResponse(response);
}
    
void blueSMIRF::deviceScanRovingNetwork(int time, char * response){
    serial.printf("IS%d\n",time);
    
    int delay = time + 10; //Extra 10seconds for BlueSmirf to print all devices
    wait(delay);
    readResponse(response);
}
    
void blueSMIRF::deviceScanCablePair(int time, char * response){
    serial.printf("IR%d\n",time);
    
    int delay = time + 10; //Extra 10seconds for BlueSmirf to print all devices
    wait(delay);
    readResponse(response);
}
    
int blueSMIRF::hidePin(void){
    serial.printf("J\n");
    char response[16] = {0};
    readResponse(response);
    if(strcmp(response, "Pin Hidden\r\n") == 0)
        return 1;
    else
        return 0;
}
    
int blueSMIRF::killConnection(void){
    serial.printf("K,\n");
    char response[16] = {0};
    readResponse(response);
    if(strcmp(response, "AOK\r\n") == 0)
        return 1;
    else
        return 0;
}

int blueSMIRF::toggleLinkQuality(char * response){
    serial.printf("L\n");
    readResponse(response);
    if(strcmp(response, "NOT Connected!\r\n") == 0)
        return 0;
    else
        return 1;
       
}
    
void blueSMIRF::passCharacter(char c){
    serial.printf("P,%c\n", c);
}

int blueSMIRF::quiteMode(int mode = -1){
    if(mode == -1)
        serial.printf("Q\n");
    else
        serial.printf("Q,%d\n",mode);
        
    char response[32] = {0};
    readResponse(response);
    if((strcmp(response, "Quiet\r\n") == 0) || (strcmp(response, "AOK\r\n") == 0))
        return 1;
    else
        return 0;
}

int blueSMIRF::reboot(void){
    serial.printf("R,1\n");
    wait_ms(100);
    char response[16] = {0}; //Reboot!
    readResponse(response);
    if(strcmp(response, "Reboot!\r\n") == 0)
        return 1;
    else
        return 0;
}

int blueSMIRF::passReceived(int mode){
    serial.printf("T,%d\n", mode);
    char response[16] = {0};
    readResponse(response);
    if((strcmp(response, "T=0!\r\n") == 0) || (strcmp(response, "T=0!\r\n") == 0))
        return 1;
    else
        return 0;
}
    
int blueSMIRF::changeTempUART(char * baud, char parity){
    serial.printf("U,%s,%c\n", baud, parity);
    char response[16] = {0};
    readResponse(response);
    if(strcmp(response, "AOK\r\n") == 0)
        return 1;
    else
        return 0;
}
    
int blueSMIRF::enableDiscovery(void){
    serial.printf("W\n");
    char response[16] = {0};
    readResponse(response);
    if(strcmp(response, "Wake\r\n") == 0)
        return 1;
    else
        return 0;
}

void blueSMIRF::enterLowPowerSleep(void){
    serial.printf("Z\n"); 
}

/** OTHER COMMANDS ************************************************************/

int blueSMIRF::readByteBlocking(void){
    while(!serial.readable());  //Block    
    return serial.getc();
}

int blueSMIRF::readByteTimeout(void){
    Timer timeOut;
    timeOut.start();
    while(!serial.readable()){
         if(timeOut.read_ms() > 2)
            return -1;
    }
    
    return serial.getc();
}

void blueSMIRF::printf(char * msg){
    serial.printf("%s",msg);   
}

void blueSMIRF::putc(int test){
    serial.putc(test);
}

void blueSMIRF::readResponse(char* response){
    wait_ms(100);
    
    Timer timeOut;
    timeOut.start();
    
    while(!serial.readable() && timeOut.read_ms() < 500);
    wait_ms(100); //Be sure to receive the response
    int index = 0;
    while(serial.readable()){
        response[index] = serial.getc();
        index++;
        wait_ms(1);
    }
}

void blueSMIRF::flushSerial(void){
    while(serial.readable())
        char c = serial.getc();
}

void blueSMIRF::setBaud(int baud){
    serial.baud(baud);
}
