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
	Up		UMETA(DisplayName = "Up"),
	Down	UMETA(DisplayName = "Down"),
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right"),
	W		UMETA(DisplayName = "W"),
	A		UMETA(DisplayName = "A"),
	S		UMETA(DisplayName = "S"),
	D		UMETA(DisplayName = "D")
};

USTRUCT(BlueprintType)
struct FMidiNoteEvent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="MIDI")
	float TimeSeconds = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float DurationSeconds = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "MIDI")
	int32 NoteNumber = 0;

	UPROPERTY(BlueprintReadOnly, Category = "MIDI")
	ENoteInputType InputType;
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
	//uint16 Division = 480
	
	bool ParseMidiData(const TArray<uint8>& Data);
};
