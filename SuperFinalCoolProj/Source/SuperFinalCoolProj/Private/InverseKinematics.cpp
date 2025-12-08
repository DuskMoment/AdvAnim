// Fill out your copyright notice in the Description page of Project Settings.


#include "InverseKinematics.h"

int InverseKinematics::UpdateFullIK(UModifiedPoseableMeshComponent* mesh, FCompactPose& animPoses)
{
	InverseKinematics::SolvePartialIK(mesh, animPoses);
	return 1;
}

int InverseKinematics::SolvePartialIK(UModifiedPoseableMeshComponent* mesh, FCompactPose& animPoses)
{
	TArray<FName> names;
	mesh->GetBoneNames(names);

	for (int i = 0; i < names.Num(); i++)
	{
		FName parentName = mesh->GetParentBone(names[i]);
		if (parentName == NAME_None)
		{
			InverseKinematics::SolveRootIK(mesh, animPoses, names[i]);
		}
		else
		{
			InverseKinematics::SolveSingleIK(mesh, names[i], parentName);
		}
	}

	return 1;
}

int InverseKinematics::SolveRootIK(UModifiedPoseableMeshComponent* mesh, FCompactPose& animPoses, FName name)
{
	mesh->SetBoneSpaceTranformByName(mesh->GetBoneTransformByName(name, EBoneSpaces::ComponentSpace), name);
	return 1;
}

int InverseKinematics::SolveSingleIK(UModifiedPoseableMeshComponent* mesh, FName name, FName parentName)
{
	FTransform parentInvT = mesh->GetBoneTransformByName(parentName, EBoneSpaces::ComponentSpace).Inverse();
	FTransform currentT = mesh->GetBoneTransformByName(name, EBoneSpaces::ComponentSpace);

	mesh->SetBoneSpaceTranformByName(currentT * parentInvT, name);
	return 1;
}


int InverseKinematics::UpdateEffectors(UModifiedPoseableMeshComponent* mesh, FTransform effectorT)
{
	InverseKinematics::UpdateLookAt(mesh, effectorT);
	return 1;
}

void InverseKinematics::ResolvePostEffectorIK(UModifiedPoseableMeshComponent* mesh, FTransform jToObj, FName name)
{
	//Update object bone pose
	mesh->SetBoneRotationByName(name, jToObj.Rotator(), EBoneSpaces::ComponentSpace);

	//Solve for local bone pose
	SolveSingleIK(mesh, name, mesh->GetParentBone(name));

	FTransform localBone = mesh->SkeletalMesh->RefSkeleton.GetRefBonePose()[mesh->GetBoneIndex(name)];
	FTransform deconcatted = mesh->GetBoneSpaceTransforms()[mesh->GetBoneIndex(name)];

	//Deconcat for saving to anim pose for FK again if needed
	deconcatted.SetTranslation(deconcatted.GetTranslation() - localBone.GetTranslation());
	deconcatted.SetRotation(deconcatted.GetRotation() * localBone.GetRotation().Inverse());
	deconcatted.SetScale3D(deconcatted.GetScale3D() / localBone.GetScale3D());

	//mesh->SetBoneSpaceTranformByName(deconcatted, name);
}

void InverseKinematics::UpdateLookAt(UModifiedPoseableMeshComponent* mesh, FTransform effectorT)
{
	FName name("Neck");

	//Joint in world
	FTransform jToObj = mesh->GetBoneTransformByName(name, EBoneSpaces::WorldSpace);

	FTransform compBone = mesh->GetBoneTransformByName(name, EBoneSpaces::ComponentSpace);
	FTransform localBone = mesh->SkeletalMesh->RefSkeleton.GetRefBonePose()[mesh->GetBoneIndex(name)];

	//Hardcoded effector world position for now
	FVector effectorInWorld = effectorT.GetLocation();

	//Making the look at basis
	FVector fwd = effectorInWorld - jToObj.GetLocation();
	fwd.Normalize();

	FVector right = FVector::CrossProduct(-localBone.GetRotation().GetUpVector(), fwd);
	right.Normalize();

	FVector up = FVector::CrossProduct(fwd, right);
	up.Normalize();

	FMatrix lookAt_Basis(FPlane(right.X, right.Y, right.Z, 0.0),
		FPlane(up.X, up.Y, up.Z, 0.0),
		FPlane(fwd.X, fwd.Y, fwd.Z, 0.0),
		FPlane(0.0, 0.0, 0.0, 1.0)
	);

	/*FMatrix lookAt_Basis;
	lookAt_Basis.SetColumn(0, right);
	lookAt_Basis.SetColumn(1, fwd);
	lookAt_Basis.SetColumn(2, up);*/

	//jToObj.SetRotation(fwd.Rotation().Quaternion());

	//Make look at basis relative to bone basis
	jToObj.SetRotation((lookAt_Basis).Rotator().Quaternion());

	//Get the affected joint basis to get it's transpose (transpose is inverse for orthonormal basis)
	FVector jRight = compBone.GetRotation().GetRightVector();
	FVector jFwd = compBone.GetRotation().GetForwardVector();
	FVector jUp = compBone.GetRotation().GetUpVector();
	jRight.Normalize();
	jFwd.Normalize();
	jUp.Normalize();

	FMatrix basisT(FPlane(jFwd.X, jFwd.Y, jFwd.Z, 0.0),
		FPlane(jRight.X, jRight.Y, jRight.Z, 0.0),
		FPlane(jUp.X, jUp.Y, jUp.Z, 0.0),
		FPlane(0.0, 0.0, 0.0, 1.0));

	//Doing it this way with this look at basis has the head rotated to the right 90 degrees 
	// and we don't know why (probobly just missing one small thing that we REALLY want to know)
	// 	 
	/*lookAt_Basis = FMatrix(FPlane(fwd.X, fwd.Y, fwd.Z, 0.0),
		FPlane(right.X, right.Y, right.Z, 0.0),
		FPlane(up.X, up.Y, up.Z, 0.0),
		FPlane(0.0, 0.0, 0.0, 1.0)
	);
	jToObj.SetRotation((basisTL.GetTransposed() * lookAt_Basis).Rotator().Quaternion());*/

	//Solve post IK 
	InverseKinematics::ResolvePostEffectorIK(mesh, jToObj, name);
}


