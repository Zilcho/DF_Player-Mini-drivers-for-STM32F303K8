# DF_Player-Mini-drivers-for-STM32F303K8
Base drivers to interface between the DF_Player Mini module and the STM32F303K8

This project repsitory includes base drivers to interface between the STM32F303K8 microcontroller and the DF_Player Mini mp3 module.

For more information on the board used, please refer to:
  https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f303k8.html

For more information on the DF_Player Mini module, please refer to:
  http://www.picaxe.com/docs/spe033.pdf
  
This project will continue to evolve as I see more need for additional features or as others request to revise and/or modify the current code.

NOTE: If you are new to Mbed and the STM32 boards, please look at the following resources for guidance:
  https://os.mbed.com/
  https://www.st.com/en/microcontrollers/stm32-32-bit-arm-cortex-mcus.html
  
Additionally, the module works independently from the microcontroller once a command is sent so uploading a new version of your code will need a
reset on the microcontroller first. Simply unplug the microcontroller and then drag and drop the binary file onto the STM32 board. This may help
with further frustrations later on :)

Please refer to the main.cpp file for a simple continuous play demonstration on how to use the drivers commited thus far and enjoy!
