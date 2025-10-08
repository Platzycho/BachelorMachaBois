// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "MidiReader.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ENoteInputType : uint8
{
	None	UMETA(DisplayName = "None"),
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right"),
	Up		UMETA(DisplayName = "Up"),
	Down	UMETA(DisplayName = "Down")
};

USTRUCT(BlueprintType)
struct FMidiNoteEvent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="MIDI")
	float TimeSeconds = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "MIDI")
	int32 NoteNumber = 0;

	UPROPERTY(BlueprintReadOnly, Category = "MIDI")
	ENoteInputType InputType = ENoteInputType::None;
};



UCLASS(Blueprintable)
class BACHELORMACHABOIS_API UMidiReader : public UObject
{
	GENERATED_BODY()

public:
	UMidiReader();
	~UMidiReader();

	bool LoadMidiFile(const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "MIDI")
	TArray<FMidiNoteEvent> GetParsedNotes() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MIDI")
	TMap<int32, ENoteInputType> NoteToInputMap;
protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<FMidiNoteEvent> ParsedNotes;

	ENoteInputType MapMidiNoteToInput(int32 NoteNumber);


private:
	struct MidiEvent
	{
		uint8 Status;
		uint8 Data1;
		uint8 Data2;
		uint32 DeltaTime;
		uint32 AbsoluteTime;
	};

	TArray<MidiEvent> Events;
	//uint16 Division = 480
	
	bool ParseMidiData(const TArray<uint8>& Data);
	uint32 ReadVariableLength(const uint8*& Ptr);
};
