// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MidiReader.h"
#include "NoteWidgetBase.h"

#include "Blueprint/UserWidget.h"
#include "LaneWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BACHELORMACHABOIS_API ULaneWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	ULaneWidgetBase();

	UPROPERTY(BlueprintReadOnly)
	ENoteInputType LaneType;

	UPROPERTY()
	TArray<UNoteWidgetBase*> ActiveNotes;

	void SpawnNote(const FMidiNoteEvent& Event);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UNoteWidgetBase* GetClosestNoteToHit();
	
};
