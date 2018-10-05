/*
* @file:        DF_player.cpp
* @author:      Rony Calderon - ronyc@nevada.unr.edu
* @board:       NUCLEO-F303K8 STM32F303K8T6 ARM®32-bit Cortex®-M4 CPU with FPU - Link: https://os.mbed.com/platforms/ST-Nucleo-F303K8/
* @compiler:    MBed OS 1.10.18.0
* @date:        October 5, 2018
* @version:     v1.01
* @ver. notes:  Implementation source file for the DF_player.h methods.

* @copyright:   You are free to use, cite, change, or revise this header file as you see fit as this was tested and made for the board
                listed above. Please contact me at the email address above if you would like to adopt or revise this project or if you
                have any questions regarding the implementation of the DF_player methods.

* @Thanks:      Thanks to Frank Mascarich @ U.N.R for helping me figure out some of the annoying bugs early on. Always appreciate it.
*
*/


#include "DF_player.h"


// Initialize the outgoing pakcet with default values
uint8_t packet_out[SIZE] = {0x7E, 0xFF, 0x06, 0x0D, 0x00, 0x00, 0x00, 0xFE, 0xEE, 0xEF};


/*
* @brief:   Constructor for the DF_player class
* @details: Instantiates a DF_player object and uses the attributes of the RawSerial object pass to it
* @pre:     none
* @post:    A DF_player object is created
* @param:   RawSerial *stream
            The RawSerial object passed to the contructor that needs to be copied in order to send the byte
            stream to the DF_player module
            
* @return:  none (void)    
*/
DF_player::DF_player(RawSerial *stream)
{
    
    serial_pointer = stream;    
}


/*
* @brief:   sendCmd() method for the DF_player class
* @details: Send the byte stream packet including the command to the DF_player module. This is performed as follows:
            1) The command is placed in the command index
            2) The checksum is calculated by summing up indices 1 - 6 and then using a 2's complement
            3) The checksum is split into two uint8_t and placed into indices 7 and 8 respectively
            4) The packet is sent out to the DF_player module in ascending order
* @pre:     none
* @post:    A command is sent to the module
* @param:   uint8_t command
            The command to be sent to the DF_player module
            
* @return:  none (void)    
*/
void DF_player::sendCmd(uint8_t command, uint8_t par1, uint8_t par2)
{   
    uint16_t checksum = 0;
    packet_out[3] = command;
    packet_out[5] = par1;
    packet_out[6] = par2;
    
    // Sum up indices as per datasheet
    for (int i=1; i<7; i++) 
    {
        checksum += packet_out[i];
    }
    // 2's complement and store the value
    checksum = -checksum;
    packet_out[7] = checksum >> 8;
    packet_out[8] = checksum & 0xFF;
    
    // Send out the packet to the module on the TX line
    for (int i = 0; i < SIZE; i++) 
    {
        
        serial_pointer->putc(packet_out[i]);
    }   
}


/*
* @brief:   play() method for the DF_player class
* @details: Send the command to the sendCmd method to begin device playback
* @pre:     none
* @post:    A command is sent to the module
* @param:   none
            
* @return:  none (void)    
*/
void DF_player::play()
{
    sendCmd (PLAY_COMMAND, 0, 0);
}


/*
* @brief:   next_song() method for the DF_player class
* @details: Send the command to the sendCmd method to start the next song
* @pre:     none
* @post:    A command is sent to the module
* @param:   none
            
* @return:  none (void)    
*/
void DF_player::next_song()
{
    sendCmd(NEXT_SONG, 0, 0);
}


/*
* @brief:   volume_down() method for the DF_player class
* @details: Send the command to the sendCmd method to decrease the volume by 1 tick
* @pre:     none
* @post:    A command is sent to the module
* @param:   none
            
* @return:  none (void)    
*/
void DF_player::volume_down()
{
    sendCmd(VOLUME_DOWN, 0, 0);    
}


/*
* @brief:   volume_up() method for the DF_player class
* @details: Send the command to the sendCmd method to increase the volume by 1 tick
* @pre:     none
* @post:    A command is sent to the module
* @param:   none
            
* @return:  none (void)    
*/
void DF_player::volume_up()
{
    sendCmd(VOLUME_UP, 0, 0);    
}


/*
* @brief:   play_track() method for the DF_player class
* @details: Send the command to the sendCmd method to play the specified track
* @pre:     none
* @post:    A command is sent to play the specified track if available
* @param:   uint8_t track
            The track to play. Variable track must not exceed 2999 according to datasheet
            
* @return:  none (void)
*/
void DF_player::play_track(uint16_t track)
{
    // Out of bounds checking. Variable track must be 0 <= track <= 2999
    if(track > 3000)
    {
        return;
    }
    uint8_t par1, par2;
    par1 = track >> 8;
    par2 = track & 0xFF;

    sendCmd(TRACK_SELECT, par1, par2);
}


/*
* @brief:   get_feedback() method for the DF_player class
* @details: Send the command to the sendCmd method to retrieve device status information
* @pre:     none
* @post:    A command is sent to the module and the data is store in a message received array
* @param:   uint8_t option
            The desired feedback that the user of method would like a pointer to
            
* @return:  uint8_t *message
            A pointer to a uint8_t array holding the message retrieved from the device
*/
uint8_t* DF_player::get_feedback(uint8_t option)
{
    uint8_t *message = NULL;
    message = new uint8_t[SIZE];
    
    sendCmd(option, 0, 0);
    
    for(int i = 0; i < SIZE; i++)
    {
        *(message + i) = serial_pointer->getc();
    }
    
    return message;
}


/*
* @brief:   set_volume() method for the DF_player class
* @details: Send the command to the sendCmd method to set the volume output of the device
* @pre:     none
* @post:    The volume is set to the variable level from 0 - 30
* @param:   uint8_t level
            The desired volume level from 0 to 30.
            
* @return:  none (void)
*/
void DF_player::set_volume(uint8_t level)
{
    // Check for out of bounds max value
    if(level > 30)
    {
        level = 30;
    }
    
    sendCmd(VOLUME_SELECT, 0, level);    
}


/*
* @brief:   set_eq() method for the DF_player class
* @details: Sends the command to the sendCmd method to set the EQ option for the device.
            
* @pre:     none
* @post:    The device EQ is set to the desired setting.
* @param:   uint8_t option
            The desired EQ setting: 0 = Normal, 1 = Pop, 2 = Rock, 3 = Jazz, 4 = Classic, 5 = Bass
            Additionally, the preade macros can be used
            
* @return:  none (void)
*/
void DF_player::set_eq(uint8_t option)
{
    // Check for out of bounds range
    if(option > 5)
    {
        return;
    }
    
    sendCmd(SET_EQ, 0, option);
   
}


/*
* @brief:   set_playback_mode() method for the DF_player class
* @details: Sends the command to the sendCmd method to set the playback options

* @NOTE:    Method does not work as intended with the module. Testing and reimplementation is expected in the future.
            
* @pre:     none
* @post:    The device playback mode is set.
* @param:   uint8_t option
            The desired playback setting: 0 = Repeat, 1 = Repeat Current Folder, 2 = Single Repeat, 3 = Random
            Additionally, the preade macros can be used
            
* @return:  none (void)
*/
void DF_player::set_playback_mode(uint8_t option)
{
    // Check for out of bounds range
    if(option > 3)
    {
        return;
    }
    
    sendCmd(PLAYBACK_MODE, 0, option);
}


/*
* @brief:   set_playback_mode() method for the DF_player class
* @details: Sends the command to the sendCmd method to set the playback options
            
* @pre:     none
* @post:    The device playback mode is set.
* @param:   uint8_t option
            The desired device setting: 0 = U-Disk, 1 = TF (micro SD), 2 = Auxilary, 3 = SLEEP, 4 = FLASH
            
* @return:  none (void)
*/
void DF_player::set_device(uint8_t device)
{
    // Check for out of bounds range
    if(device > 4)
    {
        return;
    }
    
    sendCmd(DEVICE_SELECT, 0, device);
}


/*
* @brief:   pause() method for the DF_player class
* @details: Sends the command to the sendCmd method to pause the device
            
* @pre:     none
* @post:    The device is paused
* @param:   none
            
* @return:  none (void)
*/
void DF_player::pause()
{
    sendCmd(PAUSE, 0, 0);
}


/*
* @brief:   get_volume() method for the DF_player class
* @details: Sends the command to the sendCmd method to get the current volume setting
            
* @pre:     none
* @post:    none
* @param:   none
            
* @return:  uint8_t
            The volume is returned in an unsigned char
*/
uint8_t DF_player::get_volume()
{
    uint8_t *message = NULL;
    uint8_t volume;
    
    message = get_feedback(GET_VOLUME);
    volume = *(message + 6);
    delete [] message;
    
    return volume;
}


/*
* @brief:   pause() method for the DF_player class
* @details: Sends the command to the sendCmd method to get the current EQ status of the device
            
* @pre:     none
* @post:    none
* @param:   none
            
* @return:  uint8_t 
            The EQ value is returned to the user or method
*/
uint8_t DF_player::get_EQ()
{
    uint8_t *message = NULL;
    uint8_t EQ;
    
    message = get_feedback(GET_EQ);
    EQ = *(message + 6);
    delete [] message;
    
    return EQ;
}


/*
* @brief:   get_num_files() method for the DF_player class
* @details: Retrieves the number of files on the micro-SD card
            
* @pre:     none
* @post:    none
* @param:   none
            
* @return:  uint16_t 
            The total number of files on the SD card
*/
uint16_t DF_player::get_num_files()
{
    uint8_t *message = NULL;
    uint16_t num_of_files;
    
    message = get_feedback(GET_TRACKS);
    
    num_of_files = ((uint16_t)*(message + 5)) << 8 | (0xFF & *(message + 6));
    delete [] message;
    
    return num_of_files;
}


/*
* @brief:   get_current_track() method for the DF_player class
* @details: Retrieves the current track number playing
            
* @pre:     none
* @post:    none
* @param:   none
            
* @return:  uint16_t 
            The current track number that is playing
*/
uint16_t DF_player::get_current_track()
{
    uint8_t *message = NULL;
    uint16_t current_song;
    
    message = get_feedback(GET_CURRENT_SONG);
    
    current_song = ((uint16_t)*(message + 5)) << 8 | (0xFF & *(message + 6));
    delete [] message;
    
    return current_song;
}


/*
* @brief:   play_folder_and_track() method for the DF_player class
* @details: Set the folder and track to play
            
* @pre:     none
* @post:    none
* @param:   uint8_t folder
            The folder number to select. folder cannot exceed 99
            
            uint8_t track
            The track to select from the folder to start playback
            
* @return:  none (void)
*/
void DF_player::play_folder_and_track(uint8_t folder, uint8_t track)
{
    // Check for out of bounds values. folder < 100 and track <= 255
    if(folder > 100 || track > 255)
    {
        return;
    }
    
    sendCmd(FOLDER_SELECT, folder, track);
}

