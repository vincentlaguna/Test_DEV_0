#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

// #define LedPin 0
#define ButtonPin 0

int  main(void)
{
 // setenv("WIRINGPI_GPIOMEM", "1", 1);
  wiringPiSetup();
  
  pullUpDnControl(ButtonPin, PUD_UP); // Pull up to 3.3V, make GPIO1 a stable level

//  pinMode(LedPin, OUTPUT);
  pinMode(ButtonPin, INPUT);
  // int status;

  while(1)
  {
    // digitalWrite(LedPin, HIGH); // LED is OFF 
    if (digitalRead(ButtonPin) == 1)
    {
      execl("/home/pi/Desktop/xmas_player1/run2.sh", "run2.sh", (char *) NULL);    
      // system("./run1.sh");	    
      // system("cvlc hp4.mp4 --volume-step 128.0");
      // system("cvlc ghostfinishfail.mp4 --volume-step 192.0");
      // printf("Shell 1 should be running...\n");
    }
    //else // (digitalRead(ButtonPin) == 0)
    //{ 
      //execl("/home/pi/Desktop/xmas_player1/run2.sh", "run2.sh", (char *) NULL); 
      // fork(); 	  
      // system("ps -axc | grep -i cvlc | awk '{print $1}' | tr -d '\n' | xargs -0 kill");
      // system("run2.sh &");  
      // system("cvlc ghostfinishfail.mp4 --volume-step 192.0");
      // printf("Shell 2 should be running...\n");
    //}
    
    //break;

  }

  return(0);

}

