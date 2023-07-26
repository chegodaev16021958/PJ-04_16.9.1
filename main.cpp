#include "Chat.h"

int main()
{
	Chat chat;
	chat.start();
	while (chat.chatWork())
	{
		chat.showLoginMenu();
		while (chat.mainMenuWork())
		{
			chat.showUserMenu();
		}
	}
	return 0;
}
