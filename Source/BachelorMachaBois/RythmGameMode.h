// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RythmGameMode.generated.h"

/**
 * 
 */
class UMidiReader;
class URythmTrackWidget;

UCLASS()
class BACHELORMACHABOIS_API ARythmGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARythmGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<URythmTrackWidget> TrackWidgetClass;

	UPROPERTY()
	URythmTrackWidget* TrackWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MIDI")
	TSubclassOf<UMidiReader> MidiReaderClass;

	float BeginPlayTime = 0.f;
	float TrackStartTime = 0.f;

	bool bTrackStarted = false;

	// How long to wait before spawning notes or starting music
	UPROPERTY(EditAnywhere, Category = "Timing")
	float StartDelay = 0.6f; // adjust as needed
};