// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ClipController.h"
#include "EditorFramework/AssetImportData.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimCurveTypes.h"
#include "ForwardKinematics.h"
#include "InverseKinematics.h"
#include "Animation/AnimSequence.h"
#include "Animation/PoseSnapshot.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimSequence.h"
#include "Animation/PoseSnapshot.h"
#include "Animation/AnimationPoseData.h"
#include "Animation/AnimTypes.h"
#include "Components/SkeletalMeshComponent.h"



ClipController::ClipController()
{
}

ClipController::~ClipController()
{
}	

void ClipController::GetCurvesFromUAsset(USkeletalMeshComponent* skelMeshComp, FString animName)
{
	FMemMark Mark(FMemStack::Get());
	if (!animationMap.Contains(animName))
	{
		animationMap.Add(animName, new AnimationDataController());
	}

	AnimationDataController* controller = animationMap[animName];

	FBoneContainer BoneContainer = skelMeshComp->GetAnimInstance()->GetRequiredBones();
	BoneContainer.SetUseSourceData(true);

	float sequenceLenght = currClip->GetPlayLength();
	float frameRate = currClip->GetSamplingFrameRate().AsDecimal();
	const int32 numFrames = currClip->GetNumberOfSampledKeys();
	
	for (int32 i = 0; i < numFrames ; ++i)
	{
		const float CurrentTime = i * (1.0f / frameRate);

		FCompactPose OutPose;
		FBlendedCurve OutCurve;
		FStackCustomAttributes OutAttributes;

		// Initialize OutPose with the bone container
		OutPose.SetBoneContainer(&BoneContainer);

		FAnimationPoseData AnimationPoseData(OutPose, OutCurve, OutAttributes);
		FAnimExtractContext ExtractionContext(CurrentTime, false); // false for not looping

		// Get the animation pose at the specific time bones in aprent space
		currClip->GetAnimationPose(AnimationPoseData, ExtractionContext);

		// Retrieve the bone transforms as a TArray<FTransform> (local/component space)
		//const TArray<FTransform> BoneTransforms = OutPose.GetBones();

		for (int j = 0; j < OutPose.GetBones().Num(); j++)
		{
			FTransform pose = OutPose.GetBones()[j];
			FString boneName = BoneContainer.GetReferenceSkeleton().GetBoneName(j).ToString();
			
			//not int he list of bone names
			if (!controller->bonesNames->Contains((FName)boneName))
			{
				controller->bonesNames->Add(boneName);
			}

			//not in the controller
			if (!controller->data.Contains(boneName))
			{
				controller->data.Add(boneName, new TArray<AnimationData*>());
			}

			//add new anim data
			AnimationData* animData = new AnimationData();
			animData->transform = pose;
			controller->data[boneName]->Add(animData);


		}

	}

	InitAnimationController(sequenceLenght, numFrames, controller);
}

void ClipController::InitAnimationController(float playLenght, int keysASecond, AnimationDataController* cont)
{
	keyFrames.clipduration = playLenght;
	keyFrames.speed = 1.0;
	
	FString rootName = cont->bonesNames->GetData()[0];
	int sample = cont->data[rootName]->Num();

	float distanceBetweenFames = playLenght / keysASecond;

	float totalTime = 0;

	for (int i = 0; i < sample-1; i++)
	{
		KeyFrames frame;
		frame.startTime = totalTime;
		frame.endTime = totalTime + distanceBetweenFames;
		frame.keyFrameParam = 0;
		frame.curTime = 0;

		keyFrames.frames.Add(frame);

		totalTime += distanceBetweenFames;
	}

	keyFrames.clipPeram = 0;
	keyFrames.keyFrameIndex = 0;
	keyFrames.curTime = 0;
	keyFrames.clipduration = totalTime;//this might be wrong
}

void ClipController::UpdateClipController(float dt, UModifiedPoseableMeshComponent* mesh, USkeletalMeshComponent* skelMeshComp, FTransform lookAtEffector)
{

	keyFrames.curTime += dt * keyFrames.speed;

	if (currClipTime > currClip->GetPlayLength())
	{
		//Handle looping
		currClipTime = 0;
	}

	if (keyFrames.curTime > keyFrames.clipduration)
	{
		float overflowTime = keyFrames.curTime - keyFrames.clipduration;

		//Accounts for overflow time steps that are longer than the clip duration
		while (overflowTime > keyFrames.clipduration)
		{
			overflowTime -= keyFrames.clipduration;
		}

		//just loops rn
		keyFrames.keyFrameIndex = 0;
		keyFrames.curTime = overflowTime;

	}
	else if (contrl->clipTime < 0)  //For reverse transition if clip time goes below 0
	{
		float overflowTime = -contrl->clipTime;

		//Accounts for overflow time steps that are longer than the clip duration
		while (overflowTime > clipDuration)
		{
			overflowTime -= clipDuration;
		}

		//pause if go past the start of the clip
		//contrl->speed = 0;
		contrl->clipTime = 0;
		contrl->currentkeyFrameIndex = 0;
	}

	////Finds current keyframe based on transitions and updated clip time

	//Picks a starting keyframe - 
	float keyFrameStartTime_T0 = contrl->keyFrames[contrl->currentkeyFrameIndex]->start;
	float keyFrameEndTime_T1 = contrl->keyFrames[contrl->currentkeyFrameIndex]->end;

	// keyFrameStartTime_T0 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex0].time_sec;
	//a3f64 keyFrameEndTime_T1 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex1].time_sec;

	//Makes sure it is the current keyframe Tristan created and refactored/imroved by Will and Tristan
	while (contrl->clipTime >= keyFrameEndTime_T1 || contrl->clipTime < keyFrameStartTime_T0)
	{
		//clipCtrl->clipPool->clip->keyframeDirection > 0 ? clipCtrl->keyframeIndex++ : clipCtrl->keyframeIndex--;

		contrl->currentkeyFrameIndex++;

		if (contrl->currentkeyFrameIndex < contrl->keyFrames.Num())
		{
			keyFrameStartTime_T0 = contrl->keyFrames[contrl->currentkeyFrameIndex]->start;
			keyFrameEndTime_T1 = contrl->keyFrames[contrl->currentkeyFrameIndex]->end;

			//keyFrameStartTime_T0 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex0].time_sec;
			//keyFrameEndTime_T1 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex1].time_sec;
		}

		////Don't need to account for if index goes out of range here since that is done in the transitions section

	}

	////Gets normalized time for current keyframe and clip - Base created by Tristan and refactored/imroved by Will and Tristan
	//clipCtrl->keyframeParam = (clipCtrl->clipTime_sec - keyFrameStartTime_T0) * clipCtrl->keyframe[clipCtrl->keyframeIndex].durationInv;
	contrl->keyFrames[contrl->currentkeyFrameIndex]->deltaKeyframe = (keyFrameEndTime_T1 - contrl->clipTime) / (contrl->keyFrames[contrl->currentkeyFrameIndex]->duration); //this needs to be keyframediation
	contrl->deltaClipTime = (contrl->clipDuration - contrl->clipTime) / contrl->clipDuration;
	//clipCtrl->clipParam = clipCtrl->clip->duration_sec * clipCtrl->clip->durationInv;

	FCompactPose outPose;
	FBlendedCurve outCurve;
	FStackCustomAttributes OutAttr;

	//Get bone container for required bones to get animation poses for
	outPose.SetBoneContainer(&skelMeshComp->GetAnimInstance()->GetRequiredBones());

	FAnimationPoseData poseData(outPose, outCurve, OutAttr);

	//Gets animation poses
	currClip->GetAnimationPose(poseData, FAnimExtractContext(currClipTime));

	//Updates poseable mesh bones with animation poses
	FCompactPose OutPose = poseData.GetPose();

	ForwardKinematics::UpdateFK(mesh, OutPose);
	InverseKinematics::UpdateEffectors(mesh, lookAtEffector);
	//ForwardKinematics::UpdateFK(mesh, OutPose);
}

BoneDataNode* ClipController::GetAssetData()
{
	//FMemMark Mark(FMemStack::Get());
	

	FString FilePath = FPaths::ProjectContentDir() + TEXT("/xbot_loco.htr");

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("File does not exist %s"), *FilePath));
		return NULL;
	}
	if (FilePath.IsEmpty())
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("NothingLoaded")));
		return NULL;
	}
	//file is real now we can read it 

	FString read;

	if (FFileHelper::LoadFileToString(read, *FilePath))
	{
		//UE_LOG(LogTemp, Warning, TEXT("File content: %s"), *read);

		//split into array
		TArray<FString> parsedData;
		TCHAR delimiter = '\n';

		read.ParseIntoArrayLines(parsedData);

		FString* array = parsedData.GetData();

		FString key = "basePose";
		animationMap.Add(key, new AnimationDataController());
		//use the count of the array then do the thinfs with it
		//depeneing on the set up might have to split the string even more

		//this is going to be a big loop
		int size = parsedData.Num();
		UE_LOG(LogTemp, Warning, TEXT("ParesedDataCount: %i"), size);

		for (int i = 0; i < parsedData.Num(); i++)
		{
			if (array[i].Contains("[Header]"))
			{
				if (parsedData.Num() < 12)
				{
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Problem")));
					return NULL;
				}

				TArray<FString> tempParsed;
				FString left;
				FString right;
				//get header data

				//File Type
				FString fileType = array[i + 1];
				fileType.Split(TEXT(" "), &left, &right);
				i++;//advance loop

				//DataType
				FString dataType = array[i + 1];
				dataType.Split(TEXT(" "), &left, &right);
				i++;

				//file version 
				FString fileVersion = array[i + 1];
				fileVersion.Split(TEXT(" "), &left, &right);
				int convertedFileVersion = FCString::Atoi(*right);
				i++;

				//NumSegments
				FString numSegments = array[i + 1];
				numSegments.Split(TEXT(" "), &left, &right);
				int convertedNumSegments = FCString::Atoi(*right);
				i++;

				//num frames
				FString numFrames = array[i + 1];
				numFrames.Split(TEXT(" "), &left, &right);
				int convertedNumFrames = FCString::Atoi(*right);
				i++;

				//data frame rate
				FString dataFrameRate = array[i + 1];
				dataFrameRate.Split(TEXT(" "), &left, &right);
				int convertedDataFrameRate = FCString::Atoi(*right);
				i++;

				//EulerOrderRotation
				FString eulerOrderRotation = array[i + 1];
				eulerOrderRotation.Split(TEXT(" "), &left, &right);
				i++;

				//Claibration units
				FString calibrationUnits = array[i + 1];
				calibrationUnits.Split(TEXT(" "), &left, &right);
				i++;

				//Rotation Units Degrees
				FString rotationalUnits = array[i + 1];
				rotationalUnits.Split(TEXT(" "), &left, &right);
				i++;

				//Golobal Axis Of Gravity
				FString globalAxisOfGravity = array[i + 1];
				rotationalUnits.Split(TEXT(" "), &left, &right);
				i++;

				//Bone Length Axis
				FString boneLengthAxis = array[i + 1];
				boneLengthAxis.Split(TEXT(" "), &left, &right);
				i++;

				//Scale Factor
				FString scaleFactor = array[i + 1];
				scaleFactor.Split(TEXT(" "), &left, &right);
				int convertedScaleFactor = FCString::Atoi(*right);
				i++;

			}

			//get the segment names --> only for the base pose
			if (array[i].Contains("[SegmentNames&Hierarchy]"))
			{
				i++;//progess pointer
				FString left;
				FString right;

				//add names to the animation map
				while (!array[i].Contains("["))
				{
					//THIS SHOULD BE A LIST OF NAMES
					FString nameData = array[i];
					TArray<FString> splitData;
					nameData.ParseIntoArray(splitData, TEXT("\t"), true);

					AnimationDataController* controller = animationMap[key];
					//this is used to scrape all the names for the basePosition
					if (controller != nullptr && !controller->bonesNames->Contains(splitData[0]))
					{
						controller->bonesNames->Add(splitData[0]);
					}

					//progress pointer
					i++;
				}
			}

			//base position
			if (array[i].Contains("[BasePosition]"))
			{
				//just read and do nothing with
				i++;

				while (!array[i].Contains("#")) //tells us we are at the end of a animation
				{
					i++;
				}

				i++;
			}

			if (array[i].Contains("[EndOfFile]"))
			{
				break;
			}

			if (array[i].Contains("#") && !array[i + 1].Contains("[EndOfFile]"))
			{
				i++;
				FString search1 = TEXT("# ");

				FString replace = TEXT("");

				//scrub line
				 key = array[i].Replace(*search1, *replace);
				//array[i].ReplaceInline(*search2, *replace);

				//set new key
				//key = array[i];

				//check if the key is contained
				if (!animationMap.Contains(key))
				{
					//add the new key
					animationMap.Add(key, new AnimationDataController());
				}


				
				UE_LOG(LogTemp, Warning, TEXT("EndOfAAnimation"));
			}
			
			
			//read in the animations
			if (array[i].Contains("[") && !array[i].Contains("[EndOfFile]"))
			{
				UE_LOG(LogTemp, Warning, TEXT("Looped to Read the Animations"));
				FString search1= TEXT("[");
				FString search2 = TEXT("]");

				FString replace = TEXT("");

				//scrub line
				array[i].ReplaceInline(*search1, *replace);
				array[i].ReplaceInline(*search2, *replace);


				AnimationDataController* controller = animationMap[key];
				controller->bonesNames->Add(array[i]);

				FString name = array[i];

				i++;
				//this is causing the crash fix it you dumb fuck
				while (!array[i].Contains("[") && !array[i].Contains("#"))//read until the next key
				{
					FString tempStore = array[i];
					
					//check to see if in the current animation we have
					if (!controller->data.Contains(name))
					{
						//add the new key
						controller->data.Add(name, new TArray<AnimationData*>());
					}
					
					TArray<AnimationData*>* list = controller->data[name];

					AnimationData* insertData = new AnimationData();

					//scrape string
					TArray<FString> splitData;
					array[i].ParseIntoArray(splitData, TEXT("\t"), true);

					insertData->location = new FVector(
						FCString::Atod(*splitData[1]), 
						FCString::Atod(*splitData[2]), 
						FCString::Atod(*splitData[3]));

					insertData->rotaion = new FVector(
						FCString::Atod(*splitData[4]),
						FCString::Atod(*splitData[5]),
						FCString::Atod(*splitData[6]));

					insertData->scale = FCString::Atod(*splitData[7]);

					list->Add(insertData);
					




					UE_LOG(LogTemp, Warning, TEXT("TempStore: %s"), *tempStore);
					i++;
				}
				i--;//need for when the loop progression happens
			}

			

		}

		//debug testing
		FString* p = animationMap[key]->bonesNames->GetData();
		UE_LOG(LogTemp, Warning, TEXT("boneName: %s"), *p[0]);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("First Element %s"), *array[0]));
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Could Not Load File")));

	}

	return NULL;
}
