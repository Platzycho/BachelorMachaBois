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
	URythmTrackWidget();
	UPROPERTY(BlueprintReadOnly)
	TMap<ENoteInputType, ULaneWidgetBase*> Lanes;

	void InitFromParsedNotes(const TArray<FMidiNoteEvent>& Notes);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
