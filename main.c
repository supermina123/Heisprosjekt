#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "door.h"
#include "time.h"


static void clear_all_order_lights(){
    HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < 3; i++){
            HardwareOrder type = order_types[i];
            hardware_command_order_light(f, type, 0);
        }
    }
}

static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

int main(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    signal(SIGINT, sigint_handler);
	
	
	
    // TIMER
    clock_t = door_opened_at_time;
    if (door_get_status()==1){
	door_opened_at_time = clock();	
	break; 					// forsøk på at den ikke skal byne å telle på nytt hele tiden døra er åpen
    }
	    
	    
	    
	    
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    hardware_command_movement(HARDWARE_MOVEMENT_UP);

    while(1){
        
        if(hardware_read_stop_signal()){
			hardware_command_movement(HARDWARE_MOVEMENT_STOP);
			while (hardware_read_stop_signal()) {
				hardware_command_stop_light(1);     //Lys på, ignorerer bestillinger mens lyset er tent
			}
			hardware_command_stop_light(0);          //Lys av
			for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++) {
				if (hardware_read_floor_sensor(f)) {
					door_open();                    //Åpner dør og oppdaterer klokken hvis heisen er i en etasje
				}
			}
            break;
        }
		/*
        if (door_get_status()) {						//Hvis døren er åpen
            if (hardware_read_obstruction_signal()) {
                door_open();						    //Sjekker obstruction, nullsstiller klokken hvis sant
                break;
            }
            if (!door_keep_open()) {					//Sjekker om det har gått mer enn 3 sek
                door_close();
                break;
            }

        }*/
		

        if(hardware_read_floor_sensor(0)){
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }
        if(hardware_read_floor_sensor(HARDWARE_NUMBER_OF_FLOORS - 1)){
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }

        /* All buttons must be polled, like this: */
        for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
                hardware_command_floor_indicator_on(f);
            }
        }

        /* Lights are set and cleared like this: */
        for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            /* Internal orders */
            if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
                hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 1);
            }

            /* Orders going up */
            if(hardware_read_order(f, HARDWARE_ORDER_UP)){
                hardware_command_order_light(f, HARDWARE_ORDER_UP, 1);
            }

            /* Orders going down */
            if(hardware_read_order(f, HARDWARE_ORDER_DOWN)){
                hardware_command_order_light(f, HARDWARE_ORDER_DOWN, 1);
            }
        }

        if(hardware_read_obstruction_signal()){
            hardware_command_stop_light(1);
            clear_all_order_lights();
        }
        else{
            hardware_command_stop_light(0);
        }
    }
	
    if (time() - door_opened_at_time > 3){
	    door_close();
    }
	

    return 0;
}

