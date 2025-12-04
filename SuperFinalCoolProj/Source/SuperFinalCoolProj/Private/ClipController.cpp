// Fill out your copyright notice in the Description page of Project Settings.


#include "ClipController.h"
#include "EditorFramework/AssetImportData.h"
#include "ForwardKinematics.h"


ClipController::ClipController()
{
}

ClipController::~ClipController()
{
}

void ClipController::UpdateClipController(float dt)
{
	currClipTime += dt * currClip->RateScale; //Add reverse playback here too

	if (currClipTime > currClip->GetPlayLength()) 
	{
		//Handle looping
		currClipTime = 0;
	}

}

BoneDataNode* ClipController::GetAssetData(UPoseableMeshComponent* mesh, USkeletalMeshComponent* skelMeshComp)
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

	//Updates poseable mesh bones with animation poses
	FCompactPose OutPose = poseData.GetPose();
	for (FCompactPoseBoneIndex BoneIndex : OutPose.ForEachBoneIndex())
	{
		if (BoneIndex.GetInt() < currClip->GetSkeleton()->GetReferenceSkeleton().GetNum()) {
			const FTransform& BoneTransform = OutPose[BoneIndex];
			const FTransform& BaseBoneTransform = currClip->GetSkeleton()->GetReferenceSkeleton().GetBoneAbsoluteTransform(BoneIndex.GetInt());
			const FName BoneName = currClip->GetSkeleton()->GetReferenceSkeleton().GetBoneName(BoneIndex.GetInt());
			//mesh->SetBoneLocationByName(BoneName, BoneTransform.GetLocation() + BaseBoneTransform.GetLocation(), EBoneSpaces::ComponentSpace);
			//mesh->SetBoneRotationByName(BoneName, (BoneTransform.GetRotation() + BaseBoneTransform.GetRotation()).Rotator(), EBoneSpaces::ComponentSpace);
			//mesh->SetBoneTransformByName(BoneName, BoneTransform, EBoneSpaces::ComponentSpace);
			
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Bone: %s, Transform: %s"), *BoneName.ToString(), *BoneTransform.ToString()));*/
		}
	}

	ForwardKinematics::UpdateFK(mesh, OutPose);

	/*const TArray<FTransform>& boneTrans = reinterpret_cast<const TArray<FTransform>&>(outPose.GetBones());

	const FTransform* boneData = boneTrans.GetData();*/





	//if (GEngine)
	//	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Track NamesSES: %s"), currClip->GetSkeleton()->GetReferenceSkeleton().FindBoneIndex()));

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Num Ref Bones %i"), currClip->GetSkeleton()->GetReferenceSkeleton().GetNum()));

	//TArray<FAnimNotifyTrack> track = currClip->AnimNotifyTracks;
	//FAnimNotifyTrack* dat = track.GetData();

	//FCompactPose outPose;
	//FBlendedCurve outCurve;
	//FStackCustomAttributes OutAttr;
	//
	//if (!skelMeshComp)
	//{
	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Skel Mesh Comp does not exist")));
	//	return nullptr;
	//}

	//if (!skelMeshComp->GetAnimInstance())
	//{
	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Skel Mesh Comp AnimInstance does not exist")));
	//	return nullptr;
	//}

	////Get bone container for required bones to get animation poses for
	//outPose.SetBoneContainer(&skelMeshComp->GetAnimInstance()->GetRequiredBones());

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Num Required Bones %i"), skelMeshComp->GetAnimInstance()->GetRequiredBones().GetNumBones()));

	//FAnimationPoseData poseData(outPose, outCurve, OutAttr);

	////Gets animation poses
	//currClip->GetAnimationPose(poseData, FAnimExtractContext(currClipTime));

	////Updates poseable mesh bones with animation poses
	//FCompactPose OutPose = poseData.GetPose();
	//for (FCompactPoseBoneIndex BoneIndex : OutPose.ForEachBoneIndex())
	//{
	//	if (BoneIndex.GetInt() < currClip->GetSkeleton()->GetReferenceSkeleton().GetNum()) {
	//		const FTransform& BoneTransform = OutPose[BoneIndex];
	//		const FTransform& BaseBoneTransform = currClip->GetSkeleton()->GetReferenceSkeleton().GetBoneAbsoluteTransform(BoneIndex.GetInt());
	//		const FName BoneName = currClip->GetSkeleton()->GetReferenceSkeleton().GetBoneName(BoneIndex.GetInt());
	//		mesh->SetBoneTransformByName(BoneName, BoneTransform, EBoneSpaces::ComponentSpace);
	//	}
	//}
	//	
	//return new BoneDataNode();
}
