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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitializeTrack(const TArray<FMidiNoteEvent>& Notes);

	UFUNCTION(BlueprintCallable)
	void TickTrackWidget(float CurrentSongTime, float DeltaTime);

protected:
	/** Assigned in BP_RhythmTrackWidget */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* LaneContainer;

	/** Found lanes, indexed by InputType */
	UPROPERTY()
	TMap<int32, ULaneWidgetBase*> LaneMap;

	/** Notes queued for spawning */
	TArray<FMidiNoteEvent> PendingNotes;

	/** Track start time */
	float StartTime = 0.f;

	/** How far ahead notes spawn */
	float SpawnLeadTime = 2.0f;

	/** Recursively search for lanes anywhere in the hierarchy */
	void FindLanesRecursive(UWidget* Widget);
};