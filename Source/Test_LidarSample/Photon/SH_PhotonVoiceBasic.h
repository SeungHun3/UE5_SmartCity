// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Common-cpp/inc/Common.h"

class FString;

class SH_PhotonVoiceBasic
{

public:
	virtual ~SH_PhotonVoiceBasic() {};

	virtual void ErrorCheckMessage(const FString& message, int error) {}

	virtual void InitPlayers(void) {}
	virtual void AddPlayers(int playerNr, const ExitGames::Common::JString& playerName, bool local, const ExitGames::Common::Hashtable& Custom) {}
	virtual void RemovePlayer(int playerNr) {}

	// Connect
	virtual void Voice_ConnectComplete(void) {}
	// 
	virtual void Voice_UpdateFriend(void) {}
};
