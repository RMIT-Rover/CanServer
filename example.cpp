#define Phoenix_No_WPI // remove WPI dependencies
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <zmqpp/zmqpp.hpp>


#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "ctre/Phoenix.h"
#include "ctre/phoenix/platform/Platform.h"
#include "ctre/phoenix/unmanaged/Unmanaged.h"
#include "ctre/phoenix/cci/Unmanaged_CCI.h"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <SDL2/SDL.h>
#include <unistd.h>

using namespace ctre::phoenix;
using namespace ctre::phoenix::platform;
using namespace ctre::phoenix::motorcontrol;
using namespace ctre::phoenix::motorcontrol::can;

/* make some talons for drive train */
TalonSRX talLeft2(3);
TalonSRX talRght2(1);
TalonSRX talLeft(2);
TalonSRX talRght(0);


void initDrive()
{
	/* both talons should blink green when driving forward */
	talRght.SetInverted(true);
	talRght2.SetInverted(true);
}

void drive(double left, double rght)
{
	//double left = fwd - turn; /* min, max, -1, 1 (float)*/
	//double rght = fwd + turn; /* positive turn means turn robot LEFT */

	talLeft.Set(ControlMode::PercentOutput, left);
	talRght.Set(ControlMode::PercentOutput, rght);
	talLeft2.Set(ControlMode::PercentOutput, left);
	talRght2.Set(ControlMode::PercentOutput, rght);

}
/** simple wrapper for code cleanup */
void sleepApp(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main() {
	const std::string endpoint = "tcp://localhost:4242";

  	// initialize the 0MQ context
  	zmqpp::context context;

  	// generate a push socket
  	zmqpp::socket_type type = zmqpp::socket_type::push;
  	zmqpp::socket socketbig (context, type);

  	// open the connection
  	std::cout << "Opening connection to " << endpoint << "..." << std::endl;
  	socketbig.connect(endpoint);

	/* don't bother prompting, just use can0 */
	//std::cout << "Please input the name of your can interface: ";
	float left_speed = 0;
	float right_speed = 0;
	int s,s1, i;
	int nbytes, nbytes1;
	int autonomous;
	signed char left_speed_raw =0;
	signed char right_speed_raw=0;
	std::string interface;
	//std::cin >> interface;
	interface = "can0";

	ctre::phoenix::platform::can::SetCANInterface(interface.c_str());


	// Comment out the call if you would rather use the automatically running diag-server, note this requires uninstalling diagnostics from Tuner.
	//c_SetPhoenixDiagnosticsStartTime(-1); // disable diag server, instead we will use the diag server stand alone application that Tuner installs

	/* setup drive */
	initDrive();

	while (true) {
		/* we are looking for gamepad (first time or after disconnect),
			neutral drive until gamepad (re)connected. */
		drive(0, 0);

		// wait for gamepad
		//printf("Waiting for gamepad...\n");


		while (true) {

			//count += 1;
			/* poll for disconnects or bad things */
			//struct can_frame frame;
			//nbytes = read(s, &frame, sizeof(struct can_frame));

			//printf("can1 data[0] %d\n", frame.data[0]);
			//printf("can1 data[1] %d\n", frame.data[1]);
			//printf("can1 data[2] %d\n", frame.data[2]);
			//printf("can1 data[3] %d\n", frame.data[3]);



			//SDL_Event event;
			//if (SDL_PollEvent(&event)) {
				//if (event.type == SDL_QUIT) { break; }
				//if (event.jdevice.type == SDL_JOYDEVICEREMOVED) { break; }
			//}

			/* grab some stick values */
			struct sockaddr_can addr;
			struct ifreq ifr;
			struct can_frame frame;

			printf("CAN Sockets Receive Demo\r\n");

			if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
				perror("Socket");
				return 1;
			}

			strcpy(ifr.ifr_name, "can1" );
			ioctl(s, SIOCGIFINDEX, &ifr);

			memset(&addr, 0, sizeof(addr));
			addr.can_family = AF_CAN;
			addr.can_ifindex = ifr.ifr_ifindex;

			if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
				perror("Bind");
				return 1;
			}

			nbytes = read(s, &frame, sizeof(struct can_frame));
			if(frame.can_id == 0x11)
			{
				printf("ID is ARM. Reading from ARM\r\n");
				if(frame.data[0] == 0b10000100)
				{
					if(frame.data[1] & (1<<0))
					{

						printf("first bit is a 0\r\n");\
						// use a socket to send shit to ARM.
					}
				}
			}
			else if(frame.can_id == 0x001);
			{
				printf("ID is correct\r\n");
				unsigned int data1 = frame.data[0];
 				if (nbytes < 0)
				{
					perror("Read");
					return 1;
				}

				printf("0x%03X [%d] ",frame.can_id, frame.can_dlc);


				for (i = 0; i < frame.can_dlc; i++)
				printf("%02X ",frame.data[i]);

				//printf("\r\n");
				//printf("%02X ", frame.data[0]);
				printf("\r\n");

				if(frame.data[0] == 0b01100100)
				{
					printf("%02X ",data1);
					printf("\r\n");
					if(frame.data[1] & (1<<0))
					{

						printf("first bit is a 0\r\n");\
						left_speed=0; // set left speed to 0 vel
						right_speed=0; // set right speed to 0 vel
					}
					else if(frame.data[1] & (1<<1))
					{

						printf("bit 1 is a 1 therefore, remain in autonomous or switch to autonomous\r\n");
						//..... software code........//
						/*.............
						.............
						.............
						............
						.............
						.............*/
						// please set autonomous variable to 1 for writeback to CRM
						autonomous =1;
					}
					else if ((frame.data[1] & (1<<1))==0)
					{
						// switch from autonomous mode to manual or remain in manual mode.
						/*...................
						...................
						.......................
						.......................
						......................
						..................*/
						autonomous = 0;
					}
					//.......... Byte 2 and Byte 3 (left wheel and right wheel speed)...............//
					left_speed_raw = frame.data[2]; //
					left_speed = left_speed_raw;
					left_speed = left_speed/127;
					right_speed_raw = frame.data[3]; //
					right_speed = right_speed_raw;
					right_speed = right_speed/127;
					//.............................................//
				}


			}
			drive(left_speed, right_speed);
			printf("left speed = %f\n", left_speed);
			printf("right speed = %f\n", right_speed);
			//printf("Tel left Vel: %d\n", talLeft.GetSelectedSensorVelocity());
			printf("Tel left vel: %d\n", talLeft.GetSelectedSensorVelocity());
			printf("Tel left 2 vel: %d\n", talLeft2.GetSelectedSensorVelocity());
			printf("Tel right vel: %d\n", talRght.GetSelectedSensorVelocity());
			printf("Tel right 2 vel: %d\n", talRght2.GetSelectedSensorVelocity());
			//printf("Tel left out percent: %f\n", talLeft.GetMotorOutputPercent());




			/*printf("turn = %f\n", turn);
			/* [SAFETY] only enable drive if top left shoulder button is held down */
			//if (SDL_JoystickGetButton(joy, 4)) {
			//	ctre::phoenix::unmanaged::FeedEnable(100);
			//}
			ctre::phoenix::unmanaged::FeedEnable(200);
			/* loop yield for a bit */
			if (close(s) < 0) {
			perror("Close");
			return 1;
			}

			//..............writing to the BUS............//
			const char *ifname = "can1";
			if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
				perror("Socket");
				return 1;
			}

			strcpy(ifr.ifr_name, "can1" );
			ioctl(s, SIOCGIFINDEX, &ifr);
			addr.can_family = AF_CAN;
			addr.can_ifindex = ifr.ifr_ifindex;
		
			if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
				perror("Bind");
				return 1;
			}
			frame.can_id = 0x02; // as the JETSON
			frame.can_dlc=6;


			//printf("here\r\n");
			nbytes=write(s,&frame,sizeof(struct can_frame));
			
		        frame.data[0]= 0b00101100; // sending from the JETSON to the CRM with no more messages to come.
			if(autonomous == 1)
			{
					//............do autonomous things...........//
			}	
									frame.data[1]==1;
			/*
			TalonSRX talLeft2(3);
			TalonSRX talRght2(1);
			TalonSRX talLeft(2);
			TalonSRX talRght(0);
			*/
			printf("sending can message\r\n");
			int velocity1=int(talRght.GetSelectedSensorVelocity()/5);
			int velocity2=int(talRght2.GetSelectedSensorVelocity()/5);
			int velocity3=int(talLeft.GetSelectedSensorVelocity()/5);
			int velocity4=int(talLeft2.GetSelectedSensorVelocity()/5);
			frame.data[2]= velocity1;
			frame.data[3]= velocity2;
			frame.data[4]= velocity3;
			frame.data[5]= velocity4;
/*
			frame.data[1]= velocity1;
			frame.data[2]= velocity2;
			frame.data[3]= velocity3;
			frame.data[4]= velocity4;
*/
			nbytes = write(s, &frame, sizeof(struct can_frame));
			printf("Wrote %d bytes\n", nbytes);
			if (close(s) < 0) {
			perror("Close");
			return 1;
			}
			sleepApp(20);


		}

		/* we've left the loop, likely due to gamepad disconnect UNCOMMENT
		drive(0, 0);
		SDL_JoystickClose(joy);
		printf("gamepad disconnected\n");*/
	}

	SDL_Quit();
	return 0;
}
