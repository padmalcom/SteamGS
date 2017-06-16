#pragma once

#include "steam_api.h"

#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }
/*struct Achievement_t {
	int m_eAchievementID;
	const char *m_pchAchievementID;
	char m_rgchName[128];
	char m_rgchDescription[256];
	bool m_bAchieved;
	int m_iIconImage;
};*/

class Achivements
{
	private:
		int m_iAppID; // Our current AppID
		//Achievement_t *m_pAchievements; // Achievements data
		int m_iNumAchievements; // The number of Achievements
		bool m_bInitialized; // Have we called Request stats and received the callback?
	public:
		//Achivements(Achievement_t *Achievements, int NumAchievements);
		Achivements();
		~Achivements();
		bool RequestStats();
		bool ReceivedStats();
		bool SetAchievement(const char* ID);
		STEAM_CALLBACK(Achivements, OnUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived );
		STEAM_CALLBACK(Achivements, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored );
		STEAM_CALLBACK(Achivements, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored );
};

