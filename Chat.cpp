#include <iostream>
#include "Chat.h"

bool Chat::checkLogin(const std::string& login)
{
	for (auto& user : users_)
	{
		if (login == user.getUserLogin())
			return true;
	}
	return false;
}

bool Chat::checkName(const std::string& name)
{
	for (auto& user : users_)
	{
		if (name == user.getUserName())
			return true;
	}
	return false;
}

bool Chat::checkPassword(const std::string& login, const std::string& password)
{
	for (auto& user : users_)
	{
		if (login == user.getUserLogin())
			return (password == user.getUserPassword()) ? true : false;
	}
	return false;
}

void Chat::signUp()
{
	std::string login, password, name;
	bool flag = false;

	do
	{
		std::cout << "Enter login: ";
		std::cin >> login;
		std::cout << "Enter password: ";
		std::cin >> password;
		std::cout << "Enter name: ";
		std::cin >> name;
		if (checkLogin(login))
		{
			std::cout << "ERROR: user login is busy! Try again" << "\n";
		}
		else if (checkName(name))
		{
			std::cout << "ERROR: user name is busy! Try again" << "\n";
		}
		else
		{
			flag = true;
			std::cout << std::endl;
			//std::cout << "All correct!" << "\n";
		}
	} while (flag == false);

	User user = User(login, password, name);
	user.setID(++idStorage_);
	users_.push_back(user);
	currentUser_ = std::make_shared<User>(user);
}

void Chat::login()
{
	std::string login, password;
	char operation;
	bool flag = false;
	do
	{
		std::cout << "Enter login: ";
		std::cin >> login;
		std::cout << "Enter password: ";
		std::cin >> password;

		if ((!checkLogin(login)) && (!checkPassword(login, password)))
		{
			std::cout << "Login or password failed! Try again(any key) or exit(0): " << "\n";
			std::cin >> operation;
			if (operation == '0')
				break;
		}
		else
		{
			for (auto& user : users_)
			{
				if (login == user.getUserLogin())
				{
					currentUser_ = std::make_shared<User>(user);
				}
			}
			flag = true;
			std::cout << "\n";
		}
	} while (flag == false);
}

void Chat::addCommonMessage()
{
	std::string text;
	std::cout << "Text for all: ";
	std::cin.ignore();
	getline(std::cin, text);
	std::cout << std::endl;
	commonChat_.emplace_back(Message{ currentUser_->getUserName(), "all", text });
}

void Chat::showCommonChat()
{
	std::cout << "--- Chat ---" << std::endl;
	for (auto& mess : commonChat_)
	{
		std::cout << "Message from " << mess.getFrom() << std::endl;
		std::cout << "text for all: " << mess.getText() << std::endl;
	}
	std::cout << "---------" << std::endl;
}

void Chat::addUserMessage()
{
	std::string to, text;

	std::cout << "To: ";
	std::cin >> to;
	std::cout << "Text: ";
	std::cin.ignore();
	getline(std::cin, text);

	if (!checkName(to))
	{
		std::cout << "Error send message: can't find " << to << std::endl;
		return;
	}
	else
		messages_.emplace_back(Message{ currentUser_->getUserName(), to, text });
}

void Chat::showUserChat()
{
	std::cout << "--- Chat ---" << std::endl;

	for (auto& mess : messages_)
	{
		if (currentUser_->getUserName() == mess.getFrom())
		{
			std::cout << "Message from " << currentUser_->getUserName() << " to " << mess.getTo() << std::endl;
			std::cout << "text: " << mess.getText() << std::endl;
		}
		if (currentUser_->getUserName() == mess.getTo())
		{
			std::cout << "Message from " << mess.getFrom() << " to " << currentUser_->getUserName() << std::endl;
			std::cout << "text: " << mess.getText() << std::endl;
		}
	}
	std::cout << "---------" << std::endl;
}

void Chat::changeUser()
{
	std::string newU;
	char operation;
	std::cout << "Change (1)name or (2)password: ";
	std::cin >> operation;
	switch (operation)
	{
	case '1':
		std::cout << "Enter new name: ";
		std::cin >> newU;
		if (checkName(newU))
			std::cout << "ERROR: user name is busy! Try again" << "\n";
		else
			currentUser_->setUserName(newU);
		break;
	case '2':
		std::cout << "Enter new password: ";
		std::cin >> newU;
		currentUser_->setUserPassword(newU);
		break;
	default:
		std::cout << "unknown choice" << std::endl;
		break;
	}
	std::cout << std::endl;
	users_.erase(users_.begin() + currentUser_->getID());
	users_.emplace(users_.begin() + currentUser_->getID(), User(currentUser_->getUserLogin(), currentUser_->getUserPassword(), currentUser_->getUserName(), currentUser_->getID()));
}

void Chat::showUsers()
{
	for (size_t inf = 1; inf < users_.size(); ++inf) 
	{
		std::cout << "User #" << inf << ": " << users_[inf].getUserName() << "\n";
	}
	std::cout << std::endl;
}

void Chat::admin() 
{
	User user = User("admin", "13579", "admin");
	user.setID(0);
	users_.push_back(user);
	currentUser_ = std::make_shared<User>(user);
}

void Chat::printAllInf()
{
	for (size_t inf = 1; inf < users_.size(); ++inf)
	{
		std::cout << "User ID: " << users_[inf].getID()
			<< " Name: " << users_[inf].getUserName()
			<< " Login: " << users_[inf].getUserLogin()
			<< " Password: " << users_[inf].getUserPassword() << std::endl;
	}
	std::cout << std::endl;
}

void Chat::showPrivateChat()
{
	std::string name;

	std::cout << "Enter verifiable name: ";
	std::cin >> name;
	if (!checkName(name))
		std::cout << "ERROR: user name is missing! Try again" << "\n";
	else
	{
		std::cout << "--- Check Chat ---" << std::endl;

		for (auto& mess : messages_)
		{
			if (name == mess.getFrom())
			{
				std::cout << "Message from " << name << " to " << mess.getTo() << std::endl;
				std::cout << "text: " << mess.getText() << std::endl;
			}
			if (name == mess.getTo())
			{
				std::cout << "Message from " << mess.getFrom() << " to " << name << std::endl;
				std::cout << "text: " << mess.getText() << std::endl;
			}
		}
		std::cout << "---------" << std::endl;
	}
}

void Chat::start()
{
	chatWork_ = true;
	admin();
}

void Chat::showLoginMenu()
{
	currentUser_ = nullptr;
	char operation;

	do
	{
		std::cout << "Login menu \n"
			" (1)Login\n"
			" (2)Signup\n"
			" (0)Shutdown\n"
			"select operation: ";
		std::cin >> operation;
		std::cout << std::endl;
		switch (operation)
		{
		case '1':
			login();
			mainMenuWork_ = true;
			break;
		case '2':
			signUp();
			mainMenuWork_ = true;
			break;
		case '0':
			chatWork_ = false;
			break;
		default:
			std::cout << "1 or 2..." << std::endl;
			break;
		}
	} while (chatWork_ && !mainMenuWork_);
}

void Chat::showUserMenu()
{
	char operation;
	std::cout << "Hello, " << currentUser_->getUserName() << std::endl;
	if (currentUser_->getUserName() != "admin")
	{
		while (mainMenuWork_)
		{
			std::cout << "User menu \n"
				" (1)Show common chat \n"
				" (2)Add common message \n"
				" (3)Show user chat \n"
				" (4)Add user message \n"
				" (5)Change user name or password \n"
				" (6)Show all users \n"
				" (0)Logout \n"
				"select operation: ";
			std::cin >> operation;
			std::cout << std::endl;
			switch (operation)
			{
			case '1':
				showCommonChat();
				break;
			case '2':
				addCommonMessage();
				break;
			case '3':
				showUserChat();
				break;
			case '4':
				addUserMessage();
				break;
			case '5':
				changeUser();
				break;
			case '6':
				showUsers();
				break;
			case '0':
				mainMenuWork_ = false;
				currentUser_ = nullptr;
				break;
			default:
				std::cout << "unknown choice..." << std::endl;
				break;
			}
		}
	}
	else
	{
		while (mainMenuWork_)
		{
			std::cout << "User menu \n"
				" (1)Show common chat \n"
				" (2)Add common message \n"
				" (3)Show user chat \n"
				" (4)Add user message \n"
				" (5)Print all information about users \n"
				" (6)Show private chat \n"
				" (0)Logout \n"
				"select operation: ";
			std::cin >> operation;
			std::cout << std::endl;
			switch (operation)
			{
			case '1':
				showCommonChat();
				break;
			case '2':
				addCommonMessage();
				break;
			case '3':
				showUserChat();
				break;
			case '4':
				addUserMessage();
				break;
			case '5':
				printAllInf();
				break;
			case '6':
				showPrivateChat();
				break;
			case '0':
				mainMenuWork_ = false;
				currentUser_ = nullptr;
				break;
			default:
				std::cout << "unknown choice..." << std::endl;
				break;
			}
		}
	}
}