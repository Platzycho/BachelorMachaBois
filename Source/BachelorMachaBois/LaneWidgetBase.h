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
	ULaneWidgetBase(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rhythm")
	int32 LaneInputType = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm")
	TSubclassOf<UNoteWidgetBase> NoteWidgetClass;

	UFUNCTION(BlueprintCallable)
	void SpawnNote(const FMidiNoteEvent& Event);

	UFUNCTION(BlueprintCallable)
	UNoteWidgetBase* GetClosestNoteToHit(float CurrentTime, float HitWindow = 0.15f);

protected:
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* NoteCanvas;

	UPROPERTY()
	TArray<UNoteWidgetBase*> ActiveNotes;
};