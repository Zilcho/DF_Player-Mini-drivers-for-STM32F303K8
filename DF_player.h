/*
* @file:        DF_player.h
* @author:      Rony Calderon - ronyc@nevada.unr.edu
* @board:       NUCLEO-F303K8 STM32F303K8T6 ARM®32-bit Cortex®-M4 CPU with FPU - Link: https://os.mbed.com/platforms/ST-Nucleo-F303K8/
* @compiler:    MBed OS 1.10.18.0
* @date:        October 7, 2018
* @version:     v1.01
* @ver. notes:  Header file used to implement the core method and functionality of the DF_player mini SD card module.
                Please look at http://www.picaxe.com/docs/spe033.pdf for the details on the datasheet used to implement
                the methods in this header file. This project will continue to evolve as I have more time to implement additional
                features and functionality. This includes testing a well.
                
* @NOTE:        Module does not seem to work as intended with the set_playback_mode function at the moment. Please be cautious.
                
* @NOTE:        The module usually need 100ms between commands to respond properly. If you are having issues returnng values or setting values
                then please implement a small delay between calls.                

* @copyright:   You are free to use, cite, change, or revise this header file as you see fit as this was tested and made for the board
                listed above. Please contact me at the email address above if you would like to adopt or revise this project or if you
                have any questions regarding the implementation of the DF_player methods.

* @Thanks:      Thanks to Frank Mascarich @ U.N.R for helping me figure out some of the annoying bugs early on. Always appreciate it.
*
*/

#ifndef DF_PLAYER_H
#define DF_PLAYER_H

#include "mbed.h"
#include "RawSerial.h"


// Macros commands used throughout the program
#define NEXT_SONG           0x01
#define PREVIOUS_SONG       0x02
#define TRACK_SELECT        0x03
#define VOLUME_UP           0x04
#define VOLUME_DOWN         0x05
#define VOLUME_SELECT       0x06
#define SET_EQ              0x07
#define PLAYBACK_MODE       0x08
#define DEVICE_SELECT       0x09
#define SLEEP_MODE          0x0A
#define NORMAL_MODE         0x0B
#define RESET_MODULE        0x0C
#define PLAY_COMMAND        0x0D
#define PAUSE               0x0E
#define FOLDER_SELECT       0x0F
#define VOLUME_ADJUST       0x10
#define REPEAT_PLAY         0x11
#define SELECT_TRACK        0x12
#define STOP_PLAYBACK       0x16
#define RETRANSMIT          0x40
#define REPLY               0x41
#define GET_FEEDBACK        0x42
#define GET_VOLUME          0x43

// Macros used to select the EQ options and device information
#define EQ_NORMAL           0x00
#define EQ_POP              0x01
#define EQ_ROCK             0x02
#define EQ_JAZZ             0x03
#define EQ_CLASSIC          0x04
#define EQ_BASS             0x05
#define GET_EQ              0x44
#define GET_MODE            0x45
#define GET_VERSION         0x46
#define GET_TRACKS          0x48

#define GET_CURRENT_SONG    0x4C

// Macros used to select playback mode
#define REPEAT          0x00
#define FOLDER_REPEAT   0x01
#define SINGLE_REPEAT   0x02
#define RANDOM          0x03


// Size of the packets sent and received from the device
const uint8_t SIZE = 10;

// Declare the packet that is transmitted to the device
extern uint8_t packet_out[SIZE];


// Class definition for the DF_player. Please see the source file for detailed explanations on each method.
class DF_player
{
    private:
        // Used to transmit the raw bytes to the uart pins
        RawSerial *serial_pointer;
        uint8_t* get_feedback(uint8_t option);
    
    public:
        // Constructor
        DF_player(RawSerial *stream);
        
        // sendCmd method
        void sendCmd(uint8_t command, uint8_t par1, uint8_t par2);
        

        // Basic methods to control the DF_player
        void play();
        void next_song();
        void previous_song();
        void volume_down();
        void volume_up();
        void play_track(uint16_t track);
        void set_eq(uint8_t option);
        void set_playback_mode(uint8_t option);

        void set_volume(uint8_t level);
        void set_device(uint8_t device);
        void pause();
        uint8_t get_volume();
        uint8_t get_EQ();
        uint16_t get_num_files();
        uint16_t get_current_track();
        
        
        
        void play_folder_and_track(uint8_t folder, uint8_t track);        

};

#endif
