#include "StdAfx.h"
#include "Leaderboard.h"


Leaderboard::Leaderboard() : m_CurrentLeaderboard(NULL), m_nLeaderboardEntries(0)
{
	leaderboardReady = false;
	scoresReady = false;
}


Leaderboard::~Leaderboard(void)
{
}

void Leaderboard::FindLeaderboard( const char *pchLeaderboardName )
{
	leaderboardReady = false;
	scoresReady = false;
	m_CurrentLeaderboard = NULL;

	SteamAPICall_t hSteamAPICall = SteamUserStats()->FindLeaderboard(pchLeaderboardName);
	m_callFindLeaderboards.Set(hSteamAPICall, this, &Leaderboard::OnFindLeaderboard);
}

void Leaderboard::OnFindLeaderboard( LeaderboardFindResult_t *pCallback, bool bIOFailure)
{
	// see if we encountered an error during the call
	if ( !pCallback->m_bLeaderboardFound || bIOFailure ) {
		OutputDebugStringA("Leaderboard could not be found\n");
		leaderboardReady = false;
		return;
	}
	m_CurrentLeaderboard = pCallback->m_hSteamLeaderboard;
	leaderboardReady = true;
}

bool Leaderboard::UploadScore( int score )
{
	if (!m_CurrentLeaderboard) return false;
	SteamAPICall_t hSteamAPICall = SteamUserStats()->UploadLeaderboardScore( m_CurrentLeaderboard, k_ELeaderboardUploadScoreMethodKeepBest, score, NULL, 0 );
	m_callUploadScores.Set(hSteamAPICall, this, &Leaderboard::OnUploadScore);
	return true;
}

void Leaderboard::OnUploadScore(LeaderboardScoreUploaded_t *pCallback, bool bIOFailure)
{
	if ( !pCallback->m_bSuccess || bIOFailure )
	{
		OutputDebugStringA("Score could not be uploaded to Steam\n");
	}
}

bool Leaderboard::DownloadScoresAroundUser(int rangeMin = -4, int rangeMax = 5)
{
	scoresReady = false;
	if (!m_CurrentLeaderboard) return false;

	// load the specified leaderboard data around the current user
	SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries( 
		m_CurrentLeaderboard, k_ELeaderboardDataRequestGlobalAroundUser, rangeMin, rangeMax);
	m_callDownloadScores.Set(hSteamAPICall, this, 
	&Leaderboard::OnDownloadScore);

	return true;
}

bool Leaderboard::DownloadScoresGlobal(int rangeStart = 1, int rangeEnd = 10)
{
	scoresReady = false;
	if (!m_CurrentLeaderboard) return false;

	// load the specified leaderboard from rangeStart to rangeEnd
	SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(
		m_CurrentLeaderboard, k_ELeaderboardDataRequestGlobal, rangeStart, rangeEnd);
	m_callDownloadScores.Set(hSteamAPICall, this,
		&Leaderboard::OnDownloadScore);

	return true;
}

bool Leaderboard::DownloadScoresFriends()
{
	scoresReady = false;
	if (!m_CurrentLeaderboard) return false;

	// load the specified leaderboard for all friends
	SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(
		m_CurrentLeaderboard, k_ELeaderboardDataRequestFriends, 0, 0);
	m_callDownloadScores.Set(hSteamAPICall, this,
		&Leaderboard::OnDownloadScore);

	return true;
}

void Leaderboard::OnDownloadScore(LeaderboardScoresDownloaded_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure)
	{
		m_nLeaderboardEntries = min(pCallback->m_cEntryCount, 10);
   
		for (int index = 0; index < m_nLeaderboardEntries; index++)
		{
			SteamUserStats()->GetDownloadedLeaderboardEntry(
			pCallback->m_hSteamLeaderboardEntries,index,&m_leaderboardEntries[index],NULL,0);
			scoresReady = true;
		}
	}
}
