// Fill out your copyright notice in the Description page of Project Settings.


#include "CH8_GameState.h"

ACH8_GameState::ACH8_GameState() 
	: Score(0)
	, Gold(0)
	, CurrentWaveIndex(0)
	, WaveDuration(5.0f)
{
}

int32 ACH8_GameState::GetScore()
{
	return Score;
}

int32 ACH8_GameState::GetGold()
{
	return Gold;
}

void ACH8_GameState::SpawnWave(TArray<AActor*> MinionToSpawnPerWave)
{
}
