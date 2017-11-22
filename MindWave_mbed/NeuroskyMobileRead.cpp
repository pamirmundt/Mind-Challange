#include "NeuroskyMobileRead.h"
#include "slider.h"

blueSMIRF NeuroskyMobile_1(PC_10, PC_11);   //Serial3
blueSMIRF NeuroskyMobile_2(PA_0, PA_1);   //Serial4

bool NeuroskyMobile_1_bigPacket = false;
bool NeuroskyMobile_2_bigPacket = false;

int NS1_value = 0;
int NS2_value = 0;

//Disconnect Timer
Timer NS1_disconnect_timer;
Timer NS2_disconnect_timer;

int disconnectDuration = 5000;


void readNeuroskyMobile_1(){
    if(NS1_disconnect_timer.read_ms() > disconnectDuration){
        cartStop();
        pc.printf("Disconnected1\r\n");
        NS1_disconnect_timer.reset();
        NS1_disconnect_timer.stop();
    }
    
    // Look for sync bytes
    // 0xAA 0xAA
    if(NeuroskyMobile_1.readByteTimeout() == 170){
        if(NeuroskyMobile_1.readByteTimeout() == 170){
            int payloadLength = NeuroskyMobile_1.readByteTimeout();

            //Payload length can not be greater than 169
            if(payloadLength > 169 && payloadLength < 0)
                return;
            
            uint8_t generatedChecksum = 0;

            uint8_t payloadData[256] = {0};
            for(int i = 0; i < payloadLength; i++){
                //Read payload into memory
                int byte = NeuroskyMobile_1.readByteTimeout();
                if(byte < 0)
                    return;
                payloadData[i] = byte;
                generatedChecksum += payloadData[i];
            }   

            //Read checksum byte from stream
            int checksum = NeuroskyMobile_1.readByteTimeout();
            if(checksum < 0)
                return;
            //Take one's compliment of generated checksum
            generatedChecksum = 255 - generatedChecksum;

            if(checksum == generatedChecksum){  
                int NeuroskyMobile_1_poorQuality = 200;
                int NeuroskyMobile_1_attention = 0;
                int NeuroskyMobile_1_meditation = 0;

                for(int i = 0; i < payloadLength; i++) {
                    //Parse the payload
                    switch (payloadData[i]) {
                        case 2:{
                            i++;            
                            NeuroskyMobile_1_poorQuality = payloadData[i];
                            NeuroskyMobile_1_bigPacket = true;           
                            break;
                        }

                        case 4:{
                            i++;
                            NeuroskyMobile_1_attention = payloadData[i];                     
                            break;
                        }
          
                        case 5:{
                            i++;
                            NeuroskyMobile_1_meditation = payloadData[i];
                            break;
                        }
                        
                        case 0x80:{
                            i = i + 3;
                            break;
                        }
                        
                        case 0x83:{
                            i = i + 25;   
                            break;
                        }
                        
                        default:
                            break;
                    } //end of switch
                } //end of for loop
    
                if(NeuroskyMobile_1_bigPacket) { 
                    //pc.printf("%d, %d, %d\r\n", NeuroskyMobile_1_attention, NeuroskyMobile_1_meditation, NeuroskyMobile_1_poorQuality);
                    pc.printf("NS1,%d\r\n", NeuroskyMobile_1_attention);
                    NS1_value = NeuroskyMobile_1_attention;
                    
                    //For Checking Connection
                    NS1_disconnect_timer.reset();
                    NS1_disconnect_timer.start();
                }
                NeuroskyMobile_1_bigPacket = false;
                
            }
            else{
                // Checksum Error
            }  // end if else for checksum
        } // end if read 0xAA byte
    }// end if read 0xAA byte
}

void readNeuroskyMobile_2(){
    if(NS2_disconnect_timer.read_ms() > disconnectDuration){
        cartStop();
        pc.printf("Disconnected2\r\n");
        NS2_disconnect_timer.reset();
        NS2_disconnect_timer.stop();
    }
    
    // Look for sync bytes
    // 0xAA 0xAA
    if(NeuroskyMobile_2.readByteTimeout() == 170){
        if(NeuroskyMobile_2.readByteTimeout() == 170){
            int payloadLength = NeuroskyMobile_2.readByteTimeout();

            //Payload length can not be greater than 169
            if(payloadLength > 169 && payloadLength < 0)
                return;
            
            uint8_t generatedChecksum = 0;

            uint8_t payloadData[256] = {0};
            for(int i = 0; i < payloadLength; i++){
                //Read payload into memory
                int byte = NeuroskyMobile_2.readByteTimeout();
                if(byte < 0)
                    return;
                payloadData[i] = byte;
                generatedChecksum += payloadData[i];
            }   
            //Read checksum byte from stream
            int checksum = NeuroskyMobile_2.readByteTimeout();
            if(checksum < 0)
                return;
            //Take one's compliment of generated checksum
            generatedChecksum = 255 - generatedChecksum;

            if(checksum == generatedChecksum){  
                int NeuroskyMobile_2_poorQuality = 200;
                int NeuroskyMobile_2_attention = 0;
                int NeuroskyMobile_2_meditation = 0;

                for(int i = 0; i < payloadLength; i++) {
                    //Parse the payload
                    switch (payloadData[i]) {
                        case 2:{
                            i++;            
                            NeuroskyMobile_2_poorQuality = payloadData[i];
                            NeuroskyMobile_2_bigPacket = true;           
                            break;
                        }

                        case 4:{
                            i++;
                            NeuroskyMobile_2_attention = payloadData[i];                     
                            break;
                        }
          
                        case 5:{
                            i++;
                            NeuroskyMobile_2_meditation = payloadData[i];
                            break;
                        }
                        
                        case 0x80:{
                            i = i + 3;
                            break;
                        }
                        
                        case 0x83:{
                            i = i + 25;   
                            break;
                        }
                        
                        default:
                            break;
                    } //end of switch
                } //end of for loop
    
                if(NeuroskyMobile_2_bigPacket) { 
                    //pc.printf("%d, %d, %d\r\n", NeuroskyMobile_2_attention, NeuroskyMobile_2_meditation, NeuroskyMobile_2_poorQuality);
                    pc.printf("NS2,%d\r\n", NeuroskyMobile_2_attention);
                    NS2_value = NeuroskyMobile_2_attention;
                    
                    //For Checking Connection
                    NS2_disconnect_timer.reset();
                    NS2_disconnect_timer.start();
                }
                NeuroskyMobile_2_bigPacket = false;
                
            }
            else{
                // Checksum Error
            }  // end if else for checksum
        } // end if read 0xAA byte
    }// end if read 0xAA byte
}
