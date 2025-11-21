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