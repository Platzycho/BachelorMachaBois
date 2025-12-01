// Fill out your copyright notice in the Description page of Project Settings.

#include "RythmGameMode.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "MidiReader.h"
#include "RythmTrackWidget.h"

ARythmGameMode::ARythmGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARythmGameMode::BeginPlay()
{
	Super::BeginPlay();

	BeginPlayTime = GetWorld()->GetTimeSeconds();

	if (TrackWidgetClass)
	{
		TrackWidgetInstance = CreateWidget<URythmTrackWidget>(GetWorld(), TrackWidgetClass);
		TrackWidgetInstance->AddToViewport();
	}

	if (MidiReaderClass && TrackWidgetInstance)
	{
		UMidiReader* MidiReaderInstance = NewObject<UMidiReader>(this, MidiReaderClass);

		FString Path = TEXT("C:/Users/Nesli/Documents/Bachelor/Midi Temp Stor/Dark 2.mid");

		if (MidiReaderInstance->LoadMidiFile(Path))
		{
			TrackWidgetInstance->InitializeTrack(MidiReaderInstance->GetParsedNotes());
		}

		UE_LOG(LogTemp, Warning, TEXT("MidiReader parsed %d notes"),
			MidiReaderInstance->GetParsedNotes().Num());
	}
	UE_LOG(LogTemp, Log, TEXT("GameMode BeginPlay complete - waiting %.2f seconds before starting track"), StartDelay);
}

void ARythmGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TrackWidgetInstance)
		return;

	float Now = GetWorld()->GetTimeSeconds();

	// 1) DELAY UNTIL GAME IS READY
	if (!bTrackStarted)
	{
		if (Now - BeginPlayTime >= StartDelay)
		{
			TrackStartTime = Now;
			bTrackStarted = true;

			UE_LOG(LogTemp, Log, TEXT("Track STARTED at time %.3f"), TrackStartTime);
		}
		return; // Don't tick notes yet
	}

	// 2) NORMAL TRACK TIME
	float CurrentSongTime = Now - TrackStartTime;

	TrackWidgetInstance->TickTrackWidget(CurrentSongTime, DeltaTime);
}

void ARythmGameMode::HandleLaneInput(int32 LaneInputType)
{
	if (!TrackWidgetInstance || !bTrackStarted)
	{
		UE_LOG(LogTemp, Log, TEXT("HandleLaneInput: Track not ready yet"));
		return;
	}

	const float Now = GetWorld()->GetTimeSeconds();
	const float CurrentSongTime = Now - TrackStartTime;

	EHitQuality Result = TrackWidgetInstance->HandleLaneInput(LaneInputType, CurrentSongTime);

	UE_LOG(LogTemp, Log, TEXT("Input on lane %d -> %s"),
		LaneInputType,
		*UEnum::GetValueAsString(Result));
}

void ARythmGameMode::ApplyHitResult(EHitQuality Quality)
{
	// Every judged note (even Miss) contributes to "max possible"
	const int32 NoteMaxValue = PerfectScoreValue;
	TotalPossibleScore += NoteMaxValue;

	int32 DeltaScore = 0;

	switch (Quality)
	{
	case EHitQuality::Perfect:
		NumPerfect++;
		DeltaScore = PerfectScoreValue;
		Combo++;
		break;

	case EHitQuality::Great:
		NumGreat++;
		DeltaScore = GreatScoreValue;
		Combo++;
		break;

	case EHitQuality::Good:
		NumGood++;
		DeltaScore = GoodScoreValue;
		Combo++;
		break;

	case EHitQuality::Miss:
	default:
		NumMiss++;
		DeltaScore = 0;
		Combo = 0; // break combo on miss
		break;
	}

	Score += DeltaScore;
	TotalAchievedScore += DeltaScore;

	if (Combo > MaxCombo)
	{
		MaxCombo = Combo;
	}

	if (TotalPossibleScore > 0)
	{
		AccuracyPercent =
			static_cast<float>(TotalAchievedScore) /
			static_cast<float>(TotalPossibleScore) * 100.f;
	}
	else
	{
		AccuracyPercent = 0.f;
	}

	UE_LOG(LogTemp, Log, TEXT("Score=%d Combo=%d MaxCombo=%d Acc=%.2f%% (P:%d G:%d Gd:%d M:%d)"),
		Score, Combo, MaxCombo, AccuracyPercent,
		NumPerfect, NumGreat, NumGood, NumMiss);
}