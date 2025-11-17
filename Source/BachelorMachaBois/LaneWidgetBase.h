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

    /* This lane's input type (Left, Right, Up, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rhythm")
    int32 LaneInputType = 0;

    /* Widget class to spawn for each note (set this to BP_NoteWidget in Blueprint) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm")
    TSubclassOf<UNoteWidgetBase> NoteWidgetClass;

    /* Spawn a note on this lane */
    UFUNCTION(BlueprintCallable)
    void SpawnNote(const FMidiNoteEvent& Event);

    /* Get the closest note to hit (optional; needed for actual gameplay) */
    UFUNCTION(BlueprintCallable)
    UNoteWidgetBase* GetClosestNoteToHit(float CurrentTime, float HitWindow = 0.15f);

protected:

    /* Panel where notes are spawned (CanvasPanel in Blueprint) */
    UPROPERTY(meta = (BindWidget))
    class UCanvasPanel* NoteCanvas;

    /* List of all active notes currently scrolling */
    UPROPERTY()
    TArray<UNoteWidgetBase*> ActiveNotes;
	
};
