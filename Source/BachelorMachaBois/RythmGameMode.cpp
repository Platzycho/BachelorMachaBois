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

	MidiReader = NewObject<UMidiReader>(this);

	if (MidiReader && MidiReader->LoadMidiFile(TEXT("C:/Users/Nesli/Documents/Bachelor/Midi Temp Stor/Dark 2.mid")))
	{
		UE_LOG(LogTemp, Log, TEXT("MIDI loaded successfully!"));

		const TArray<FMidiNoteEvent>& Notes = MidiReader->GetParsedNotes();
		for (const FMidiNoteEvent& Note : Notes)
		{
			UE_LOG(LogTemp, Log, TEXT("Note %d | %.3f sec | Dur %.3f | Input: %s"),
				Note.NoteNumber, Note.TimeSeconds, Note.DurationSeconds, *UEnum::GetValueAsString(Note.InputType));
		}

		// Create and initialize track UI if we've set a widget class
		if (TrackWidgetClass)
		{
			TrackWidgetInstance = CreateWidget<URythmTrackWidget>(GetWorld(), TrackWidgetClass);
			if (TrackWidgetInstance)
			{
				TrackWidgetInstance->AddToViewport();
				TrackWidgetInstance->InitializeTrack(Notes);

				// record start time for the song/track (GameMode-driven clock)
				TrackStartTime = GetWorld()->GetTimeSeconds();
				UE_LOG(LogTemp, Log, TEXT("Track widget created and initialized at world time %.3f"), TrackStartTime);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TrackWidgetClass not set on GameMode. Set it in the GameMode defaults."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load MIDI file"));
	}
}

void ARythmGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;

	// Drive the UI via the track widget (if created)
	if (TrackWidgetInstance)
	{
		const float CurrentSongTime = GetWorld()->GetTimeSeconds() - TrackStartTime;
		TrackWidgetInstance->TickTrackWidget(CurrentSongTime, DeltaTime);
	}
}