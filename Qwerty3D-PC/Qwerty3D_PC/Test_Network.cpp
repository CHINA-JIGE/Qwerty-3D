
#include <iostream>
#include "Qwerty3D-PC.h"

Qwerty::NetworkModule networkModule;
void Mainloop();

//Main Entry
int main()
{
	//input user/client name
	std::string clientName;
	std::cout << "Please Enter user name:" << std::endl;
	std::cin >> clientName;
	networkModule.Init(
		"tcp://39.108.118.166:1883",
		clientName,
		"FuckingTestMqtt");

	//connect
	std::cout <<"Connecting to mqtt server..."<< std::endl;
	bool isConnected =networkModule.Connect();

	if (isConnected)
	{
		std::cout << "Connected." << std::endl;
	}
	else
	{
		std::cout << "Failed to connect!" << std::endl;
		system("pause");
		return 0;
	}

	while (1)
	{
		std::string s;
		std::cout << "please enter message to send:";
		std::cin >> s;
		if (s == "quit")break;
		networkModule.Send(s);
		std::cout << std::endl;
	}

	networkModule.Disconnect();

	return 0;
}
