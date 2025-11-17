// Fill out your copyright notice in the Description page of Project Settings.


#include "RythmTrackWidget.h"
#include "Components/PanelWidget.h"
#include "Components/Widget.h"
#include "Kismet/GameplayStatics.h"

URythmTrackWidget::URythmTrackWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    //SetCanTick(true);
}

void URythmTrackWidget::NativeConstruct()
{
    Super::NativeConstruct();

    LaneMap.Empty();

    if (!LaneContainer)
    {
        UE_LOG(LogTemp, Error, TEXT("RythmTrackWidget: LaneContainer is NOT bound!"));
        return;
    }

    // Find all lanes inside the container
    const int32 ChildCount = LaneContainer->GetChildrenCount();
    for (int32 i = 0; i < ChildCount; i++)
    {
        UWidget* Child = LaneContainer->GetChildAt(i);
        if (ULaneWidgetBase* Lane = Cast<ULaneWidgetBase>(Child))
        {
            LaneMap.Add(Lane->LaneInputType, Lane);

            UE_LOG(LogTemp, Log,
                TEXT("TrackWidget: Registered Lane %d"), Lane->LaneInputType);
        }
        else
        {
            UE_LOG(LogTemp, Warning,
                TEXT("TrackWidget: Non-lane widget found in LaneContainer"));
        }
    }

    StartTime = GetWorld()->GetTimeSeconds();
}

void URythmTrackWidget::InitializeTrack(const TArray<FMidiNoteEvent>& Notes)
{
    PendingNotes = Notes;

    // Sort by time (always good practice)
    PendingNotes.Sort([](const FMidiNoteEvent& A, const FMidiNoteEvent& B)
        {
            return A.TimeSeconds < B.TimeSeconds;
        });

    UE_LOG(LogTemp, Log,
        TEXT("TrackWidget: Received %d notes"), PendingNotes.Num());
}

void URythmTrackWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (PendingNotes.Num() == 0)
        return;

    const float CurrentTime = GetWorld()->GetTimeSeconds() - StartTime;

    // Spawn any notes whose time has arrived
    int32 SpawnCount = 0;

    while (PendingNotes.Num() > 0)
    {
        const FMidiNoteEvent& Event = PendingNotes[0];

        // If event is in the future → break
        if (Event.TimeSeconds > CurrentTime + 2.0f)
            break;

        // Dispatch note to correct lane
        if (ULaneWidgetBase** LanePtr = LaneMap.Find((int32)Event.InputType))
        {
            ULaneWidgetBase* Lane = *LanePtr;
            Lane->SpawnNote(Event);
        }

        // Remove from pending
        PendingNotes.RemoveAt(0);
        SpawnCount++;
    }

    if (SpawnCount > 0)
    {
        UE_LOG(LogTemp, Log,
            TEXT("Spawned %d notes at time %.3f"), SpawnCount, CurrentTime);
    }
}
