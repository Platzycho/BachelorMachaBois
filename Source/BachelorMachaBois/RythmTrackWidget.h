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

    /** Called by GameMode after loading MIDI */
    UFUNCTION(BlueprintCallable)
    void InitializeTrack(const TArray<FMidiNoteEvent>& Notes);

protected:

    /** Assign your HorizontalBox or UniformGridPanel in BP_RhythmTrackWidget */
    UPROPERTY(meta = (BindWidget))
    UPanelWidget* LaneContainer;

    /** One lane widget per input */
    UPROPERTY()
    TMap<int32, ULaneWidgetBase*> LaneMap;

    /** Notes waiting to be spawned */
    TArray<FMidiNoteEvent> PendingNotes;

    /** At what song time we started */
    float StartTime = 0.f;
};
