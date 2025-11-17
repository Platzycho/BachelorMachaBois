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

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/** MIDI reader instance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm")
	UMidiReader* MidiReader;

	/** Widget class to create (set this in the BP subclass of your GameMode) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm")
	TSubclassOf<URythmTrackWidget> TrackWidgetClass;

	/** Runtime instance of the track widget created at BeginPlay */
	UPROPERTY()
	URythmTrackWidget* TrackWidgetInstance = nullptr;

	/** When the track was started (GetWorld()->GetTimeSeconds()) */
	float TrackStartTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rhythm")
	float ElapsedTime = 0.f;
};