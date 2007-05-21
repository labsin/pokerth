/***************************************************************************
 *   Copyright (C) 2007 by Lothar May                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/* Network client thread. */

#ifndef _CLIENTTHREAD_H_
#define _CLIENTTHREAD_H_

#include <core/thread.h>
#include <guiinterface.h>
#include <playerdata.h>
#include <gamedata.h>
#include <string>
#include <memory>

class ClientContext;
class ClientState;
class SenderThread;
class ReceiverHelper;
class ClientSenderCallback;
class Game;

class ClientThread : public Thread
{
public:
	ClientThread(GuiInterface &gui);
	virtual ~ClientThread();

	// Set the parameters. Does not do any error checking.
	// Error checking will be done during connect
	// (i.e. after starting the thread).
	void Init(
		const std::string &serverAddress,
		unsigned serverPort,
		bool ipv6,
		const std::string &pwd,
		const std::string &playerName);

	ClientCallback &GetCallback();
	GuiInterface &GetGui();

protected:
	typedef std::map<unsigned, std::string> PlayerMap;

	// Main function of the thread.
	virtual void Main();

	const ClientContext &GetContext() const;
	ClientContext &GetContext();

	ClientState &GetState();
	void SetState(ClientState &newState);

	SenderThread &GetSender();
	ReceiverHelper &GetReceiver();

	const GameData &GetGameData() const;
	void SetGameData(const GameData &gameData);
	const StartData &GetStartData() const;
	void SetStartData(const StartData &startData);
	int GetGuiPlayerNum() const;
	void SetGuiPlayerNum(int guiPlayerNum);

	boost::shared_ptr<Game> GetGame();

	ClientSenderCallback &GetSenderCallback();

	void AddPlayerData(boost::shared_ptr<PlayerData> playerData);
	void RemovePlayerData(unsigned playerId);
	PlayerDataList GetMappedPlayerDataList() const;

private:

	std::auto_ptr<ClientContext> m_context;
	std::auto_ptr<ClientSenderCallback> m_senderCallback;
	ClientState *m_curState;
	GuiInterface &m_gui;

	std::auto_ptr<SenderThread> m_sender;
	std::auto_ptr<ReceiverHelper> m_receiver;

	GameData m_gameData;
	StartData m_startData;
	PlayerDataList m_playerDataList;

	boost::shared_ptr<Game> m_game;

	unsigned m_curGameId;
	int m_guiPlayerNum;

friend class ClientStateInit;
friend class ClientStateStartResolve;
friend class ClientStateResolving;
friend class ClientStateStartConnect;
friend class ClientStateConnecting;
friend class ClientStateStartSession;
friend class ClientStateWaitSession;
friend class ClientStateWaitGame;
friend class ClientStateWaitHand;
friend class ClientStateFinal;
};

#endif
