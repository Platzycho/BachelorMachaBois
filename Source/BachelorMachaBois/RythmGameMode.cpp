// Fill out your copyright notice in the Description page of Project Settings.


#include "RythmGameMode.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "MidiReader.h"

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

		// Print all notes and timing
		for (const FMidiNoteEvent& Note : MidiReader->GetParsedNotes())
		{
			UE_LOG(LogTemp, Log, TEXT("Note %d | %.3f sec | Input: %s"),
				Note.NoteNumber,
				Note.TimeSeconds,
				*UEnum::GetValueAsString(Note.InputType));
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

	// Debug: print the running time every 2 seconds
	//if (FMath::Fmod(ElapsedTime, 2.f) < DeltaTime)
	//{
	//	if (MidiReader && MidiReader->LoadMidiFile(TEXT("C:/Users/Nesli/Documents/Bachelor/Midi Temp Stor/Dark 2.mid")))
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("MIDI loaded successfully!"));

	//		// Print all notes and timing
	//		for (const FMidiNoteEvent& Note : MidiReader->GetParsedNotes())
	//		{
	//			UE_LOG(LogTemp, Log, TEXT("Note %d | %.3f sec | Input: %s"),
	//				Note.NoteNumber,
	//				Note.TimeSeconds,
	//				*UEnum::GetValueAsString(Note.InputType));
	//		}
	//	}
	//}
}