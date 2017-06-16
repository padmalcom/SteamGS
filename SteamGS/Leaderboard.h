#pragma once

#include "steam_api.h"

class Leaderboard
{
private:
	SteamLeaderboard_t m_CurrentLeaderboard;
public:
	int m_nLeaderboardEntries; // How many entries do we have?
	LeaderboardEntry_t m_leaderboardEntries[10]; // The entries

	bool leaderboardReady = false;
	bool scoresReady = false;

	Leaderboard(void);
	~Leaderboard(void);

	void FindLeaderboard( const char *pchLeaderboardName );
	bool UploadScore( int score );
	bool DownloadScoresAroundUser(int rangeMin, int rangeMax);
	bool DownloadScoresGlobal(int rangeStart, int rangeEnd);
	bool DownloadScoresFriends();

	void OnFindLeaderboard( LeaderboardFindResult_t *pResult, bool bIOFailure);
	void OnUploadScore( LeaderboardScoreUploaded_t *pResult, bool bIOFailure);
	void OnDownloadScore( LeaderboardScoresDownloaded_t *pResult, bool bIOFailure);

	CCallResult<Leaderboard, LeaderboardFindResult_t> m_callFindLeaderboards;
	CCallResult<Leaderboard, LeaderboardScoresDownloaded_t> m_callDownloadScores;
	CCallResult<Leaderboard, LeaderboardScoreUploaded_t> m_callUploadScores;
};

