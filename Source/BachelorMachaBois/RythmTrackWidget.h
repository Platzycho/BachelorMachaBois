// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LaneWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "RythmTrackWidget.generated.h"

/**
 * 
 */
UCLASS()
class BACHELORMACHABOIS_API URythmTrackWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	URythmTrackWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	// We keep NativeTick but the game will drive TickTrackWidget from GameMode
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** Called by GameMode after loading MIDI */
	UFUNCTION(BlueprintCallable)
	void InitializeTrack(const TArray<FMidiNoteEvent>& Notes);

	/** Called by GameMode each frame to advance UI (GameMode-driven tick) */
	UFUNCTION(BlueprintCallable)
	void TickTrackWidget(float CurrentSongTime, float DeltaTime);

protected:
	/** Assign your HorizontalBox or UniformGridPanel in BP_RhythmTrackWidget */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* LaneContainer;

	/** One lane widget per input */
	UPROPERTY()
	TMap<int32, ULaneWidgetBase*> LaneMap;

	/** Notes waiting to be spawned */
	TArray<FMidiNoteEvent> PendingNotes;

	/** Start time recorded by GameMode (optional) */
	float StartTime = 0.f;

	/** Spawn lead time (seconds before hit to spawn) */
	float SpawnLeadTime = 2.0f;
};