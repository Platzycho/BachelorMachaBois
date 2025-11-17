// Fill out your copyright notice in the Description page of Project Settings.


#include "RythmTrackWidget.h"
#include "Components/PanelWidget.h"
#include "Components/Widget.h"
#include "Kismet/GameplayStatics.h"

URythmTrackWidget::URythmTrackWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Do NOT call SetCanTick here (we'll drive updates from GameMode)
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

	const int32 ChildCount = LaneContainer->GetChildrenCount();
	for (int32 i = 0; i < ChildCount; i++)
	{
		UWidget* Child = LaneContainer->GetChildAt(i);
		if (ULaneWidgetBase* Lane = Cast<ULaneWidgetBase>(Child))
		{
			LaneMap.Add(Lane->LaneInputType, Lane);
			UE_LOG(LogTemp, Log, TEXT("TrackWidget: Registered Lane %d"), Lane->LaneInputType);
		}
		else
		{
			// not necessarily an error (could be spacer), but warn
			UE_LOG(LogTemp, Verbose, TEXT("TrackWidget: Non-lane widget found in LaneContainer (index %d)"), i);
		}
	}

	// StartTime is set by GameMode when it creates and starts the track
	StartTime = 0.f;
}

void URythmTrackWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// We do not rely on widget tick; leave empty or call Super
	Super::NativeTick(MyGeometry, InDeltaTime);
}

/** Initialize the pending notes (called once from GameMode) */
void URythmTrackWidget::InitializeTrack(const TArray<FMidiNoteEvent>& Notes)
{
	PendingNotes = Notes;
	PendingNotes.Sort([](const FMidiNoteEvent& A, const FMidiNoteEvent& B) { return A.TimeSeconds < B.TimeSeconds; });
	UE_LOG(LogTemp, Log, TEXT("TrackWidget: Received %d notes"), PendingNotes.Num());
}

/** GameMode-driven tick entry point — call this every frame from GameMode::Tick() */
void URythmTrackWidget::TickTrackWidget(float CurrentSongTime, float DeltaTime)
{
	if (PendingNotes.Num() == 0) return;

	int32 SpawnCount = 0;

	// Spawn while notes are within spawn lead time
	while (PendingNotes.Num() > 0)
	{
		const FMidiNoteEvent& Event = PendingNotes[0];

		// If event is in the future (beyond lead time) → stop
		if (Event.TimeSeconds > CurrentSongTime + SpawnLeadTime)
			break;

		// Dispatch note to correct lane
		if (ULaneWidgetBase** LanePtr = LaneMap.Find((int32)Event.InputType))
		{
			ULaneWidgetBase* Lane = *LanePtr;
			if (Lane)
			{
				Lane->SpawnNote(Event);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TrackWidget: No lane found for input %d"), (int32)Event.InputType);
		}

		// Remove from pending after spawning
		PendingNotes.RemoveAt(0);
		SpawnCount++;
	}

	if (SpawnCount > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("TrackWidget: Spawned %d notes at song time %.3f"), SpawnCount, CurrentSongTime);
	}
}