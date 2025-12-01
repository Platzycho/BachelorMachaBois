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

enum class EHitQuality : uint8;

UCLASS()
class BACHELORMACHABOIS_API ARythmGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARythmGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Called from PlayerController when a lane key is pressed */
	UFUNCTION(BlueprintCallable, Category = "Rhythm|Hit")
	void HandleLaneInput(int32 LaneInputType);

	// -------- SCORE / COMBO / STATS (readable from UMG) --------

	/** Current score */
	UPROPERTY(BlueprintReadOnly, Category = "Rhythm|Score")
	int32 Score = 0;

	/** Current combo */
	UPROPERTY(BlueprintReadOnly, Category = "Rhythm|Score")
	int32 Combo = 0;

	/** Highest combo reached so far */
	UPROPERTY(BlueprintReadOnly, Category = "Rhythm|Score")
	int32 MaxCombo = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Rhythm|Score")
	int32 NumPerfect = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Rhythm|Score")
	int32 NumGreat = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Rhythm|Score")
	int32 NumGood = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Rhythm|Score")
	int32 NumMiss = 0;

	/** 0–100 (%), updated after every judgment */
	UPROPERTY(BlueprintReadOnly, Category = "Rhythm|Score")
	float AccuracyPercent = 0.f;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<URythmTrackWidget> TrackWidgetClass;

	UPROPERTY()
	URythmTrackWidget* TrackWidgetInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MIDI")
	TSubclassOf<UMidiReader> MidiReaderClass;

	float BeginPlayTime = 0.f;
	float TrackStartTime = 0.f;

	bool bTrackStarted = false;

	// How long to wait before spawning notes or starting music
	UPROPERTY(EditAnywhere, Category = "Timing")
	float StartDelay = 0.6f; // adjust as needed

	// -------- SCORE TUNING --------

	/** Score value for each hit quality */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm|Score")
	int32 PerfectScoreValue = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm|Score")
	int32 GreatScoreValue = 150;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm|Score")
	int32 GoodScoreValue = 50;

	/** Internals for accuracy calculation */
	int32 TotalPossibleScore = 0;   // if every judged note was Perfect
	int32 TotalAchievedScore = 0;   // what the player actually got

	/** Apply score/combo/accuracy changes from a hit result */
	void ApplyHitResult(EHitQuality Quality);
};