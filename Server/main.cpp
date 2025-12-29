#include "main.h"
#include <iostream>
#include <string>
#include <smPacket.h>
#include <thread>

int main()
{
	std::cout << "Server Name: " << SERVER_NAME << std::endl;
	std::cout << "Server IP: "	 << SERVER_IP << std::endl;
	std::cout << "Server Port: " << SERVER_PORT << std::endl;
	std::cout << "Developer: "   << SERVER_DEVELOPER << " in " 
								 << SERVER_YEAR << std::endl;
	std::cout << "Servidor iniciado com sucesso!" << std::endl;

	std::string command;

	while (true) {

		std::cout << "> ";
		std::getline(std::cin, command);

		if (command == "status") {
			std::cout << "Server is running..." << std::endl;
		} 
		else if (command == "online") {
			std::cout << "Players online: " MAX_PLAYERS << std::endl;
		}
		else { 
			std::cout << "Commands invalid." << std::endl;
		}

	}

	return 0;
}