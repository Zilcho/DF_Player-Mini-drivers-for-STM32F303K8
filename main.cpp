/*
* This is a simple driver file designed to showcase some of the features implemented by the 
* DF_Player Mini when used with the Nucelo STM32F303K8T6 board. If using another board, please
* refer to the technical specs or datasheet to locate the appropriate RawSerial TX and RX lines.
*
* This program is intended to show a continuous playback mode using the DF_Player Mini's 'Busy'
  pin.
  
* NOTE: Please give the unit about a 100ms delay between calls to ensure proper operation that
        require the unit to respond with information about either volume, EQ, or number of files.
        I chose to wait either 250ms or 1000ms between calls to ensure proper operation but adjust
        for your project needs.
  

* Connections are as follows between the F303K8 and the DF_Player Mini:

    Nucleo-F303K8           DF_Player Mini
    --------------------------------------

    PA_9(UART1_TX)  ------->    RX 
    PA_10(UART1_RX) ------->    TX
    PF_1(D8)        ------->    Busy
    
    The speaker wires are connected to the DF_Player Mini SPK1 and SPK2 lines
*
*/


#include "DF_player.h"


// Create a RawSerial object to pass to the DF_player class. Tested bauds include 9600 and 19200
// according to the datasheet specifications
RawSerial uart1(PA_9, PA_10, 9600);


int main()
{
    // Create a DF_Player Mini object and pass the RawSerial object for initialization
    DF_player *jukebox = new DF_player(&uart1);
    
    // Busy pin connected to D8
    DigitalIn finished(D8);
    
    // Set the volume for the device and wait 1 second then play fourth track
    wait(1);
    jukebox->set_volume(15);
    wait(1);
    jukebox->play_track(4);
    
    // Test if the Busy pin is high signaling the current song has finished playing
    while(1)
    {  
        if(finished)
        {  
            // Play next song
            //wait(.25);
            jukebox->next_song();
            wait(.25);
        }
    }

	delete jukebox;
	jukebox = nullptr;
    
}// End


