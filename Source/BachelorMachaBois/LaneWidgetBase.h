// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MidiReader.h"
#include "NoteWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "LaneWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BACHELORMACHABOIS_API ULaneWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	ULaneWidgetBase(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SpawnNote(const FMidiNoteEvent& Event);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notes")
	TSubclassOf<UNoteWidgetBase> NoteWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* NoteCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane")
	int32 LaneInputType = 0;

	TArray<UNoteWidgetBase*> ActiveNotes;

	UNoteWidgetBase* GetClosestNoteToHit(float CurrentTime, float HitWindow);
};