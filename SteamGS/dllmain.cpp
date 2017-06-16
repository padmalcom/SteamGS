// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "stdafx.h"
#include <string>
#include "steam_api.h"
#include "Leaderboard.h"
#include "Achivements.h"

#define DLL_USE
#include "adll.h"


Leaderboard* g_Leaderboard = NULL;
Achivements* g_Achivements = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	engine_bind();
	return TRUE;
}

/*********************** GENERAL FUNCTIONS ***********************/

DLLFUNC var initSteam() {
	bool clientIsRunning = SteamAPI_Init();
	if (clientIsRunning) {
		g_Leaderboard = new Leaderboard();
		g_Achivements = new Achivements();
		return _VAR(1);
	} else {
		return _VAR(0);
	}
}

DLLFUNC var getPlayerName(STRING* _targetString) {
	const char* name = SteamFriends()->GetPersonaName();
	std::string strName(name);
	// char* finalName = new char[strName.length() + 1];
	strcpy(_targetString->chars, strName.c_str());
	return _VAR(1);
}

DLLFUNC void shutdownSteam() {
	SteamAPI_Shutdown();
}

// Has to be called in a loop
DLLFUNC void runSteamCallbacks() {
	SteamAPI_RunCallbacks();
}

/*********************** ACHIVEMENTS ***********************/
DLLFUNC void requestStats() {
	g_Achivements->RequestStats();
}

DLLFUNC var statsReceived() {
	if (g_Achivements->ReceivedStats()) {
		return _VAR(1);
	}
	return _VAR(0);
}

DLLFUNC var unlockAchievement(STRING* _achivement) {
	if (g_Achivements->SetAchievement(_CHR(_achivement))) {
		return _VAR(1);
	}
	return _VAR(0);
}


/*********************** LEADERBOARD ***********************/
DLLFUNC void findLeaderboard(STRING* _leaderboardName) {
	if (g_Leaderboard) {
		g_Leaderboard->FindLeaderboard(_CHR(_leaderboardName));
	}
}

DLLFUNC void uploadScore(var score) {
	if (g_Leaderboard) {
		g_Leaderboard->UploadScore(_INT(score));
	}
}

DLLFUNC var downloadScoresAroundUser(var rangeMin, var rangeMax) {
	if (g_Leaderboard) {
		bool downloadResult = g_Leaderboard->DownloadScoresAroundUser(_INT(rangeMin), _INT(rangeMax));
		if (downloadResult) {
			return _VAR(1);
		}
	}
	return _VAR(0);
}

DLLFUNC var downloadScoresGlobal(var rangeFrom, var rangeTo) {
	if (g_Leaderboard) {
		bool downloadResult = g_Leaderboard->DownloadScoresGlobal(_INT(rangeFrom), _INT(rangeTo));
		if (downloadResult) {
			return _VAR(1);
		}
	}
	return _VAR(0);
}

DLLFUNC var downloadScoresFriends() {
	if (g_Leaderboard) {
		bool downloadResult = g_Leaderboard->DownloadScoresFriends();
		if (downloadResult) {
			return _VAR(1);
		}
	}
	return _VAR(0);
}

DLLFUNC var getLeaderboardItem_AccountID(var index) {
	if (g_Leaderboard) {
		if (_INT(index) < 10) {
			AccountID_t id = g_Leaderboard->m_leaderboardEntries[_INT(index)].m_steamIDUser.GetAccountID();
			return _VAR((int)id);
		}
	}
	return _VAR(-1);
}

DLLFUNC var getLeaderboardItem_Score(var index) {
	if (g_Leaderboard) {
		if (_INT(index) < 10) {
			int32 score = g_Leaderboard->m_leaderboardEntries[_INT(index)].m_nScore;
			return _VAR((int)score);
		}
	}
	return _VAR(-1);
}

DLLFUNC var getLeaderboardItem_Rank(var index) {
	if (g_Leaderboard) {
		if (_INT(index) < 10) {
			int32 rank = g_Leaderboard->m_leaderboardEntries[_INT(index)].m_nGlobalRank;
			return _VAR((int)rank);
		}
	}
	return _VAR(-1);
}

DLLFUNC var getLeaderboardItem_Name(var index, STRING* _targetString) {
	if (g_Leaderboard) {
		if (_INT(index) < 10) {
			SteamFriends()->GetPlayerNickname(g_Leaderboard->m_leaderboardEntries[_INT(index)].m_steamIDUser);
			const char* name = SteamFriends()->GetPersonaName();
			std::string strName(name);
			char* finalName = new char[strName.length() + 1];
			strcpy(_targetString->chars, strName.c_str());
			return _VAR(1);
		}
	}
	return _VAR(-1);
}

DLLFUNC var isLeaderboardReady() {
	if (g_Leaderboard) {
		if (g_Leaderboard->leaderboardReady) {
			return _VAR(1);
		}
	}
	return _VAR(0);
}

DLLFUNC var isScoresReady() {
	if (g_Leaderboard) {
		if (g_Leaderboard->scoresReady) {
			return _VAR(1);
		}
	}
	return _VAR(0);
}