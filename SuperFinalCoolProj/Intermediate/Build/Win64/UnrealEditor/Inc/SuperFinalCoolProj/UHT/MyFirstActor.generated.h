// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "MyFirstActor.h"

#ifdef SUPERFINALCOOLPROJ_MyFirstActor_generated_h
#error "MyFirstActor.generated.h already included, missing '#pragma once' in MyFirstActor.h"
#endif
#define SUPERFINALCOOLPROJ_MyFirstActor_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class AMyFirstActor ************************************************************
SUPERFINALCOOLPROJ_API UClass* Z_Construct_UClass_AMyFirstActor_NoRegister();

#define FID_Users_william_mansfield_Desktop_AdvAnim_SuperFinalCoolProj_Source_SuperFinalCoolProj_Public_MyFirstActor_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAMyFirstActor(); \
	friend struct Z_Construct_UClass_AMyFirstActor_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend SUPERFINALCOOLPROJ_API UClass* Z_Construct_UClass_AMyFirstActor_NoRegister(); \
public: \
	DECLARE_CLASS2(AMyFirstActor, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/SuperFinalCoolProj"), Z_Construct_UClass_AMyFirstActor_NoRegister) \
	DECLARE_SERIALIZER(AMyFirstActor)


#define FID_Users_william_mansfield_Desktop_AdvAnim_SuperFinalCoolProj_Source_SuperFinalCoolProj_Public_MyFirstActor_h_12_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	AMyFirstActor(AMyFirstActor&&) = delete; \
	AMyFirstActor(const AMyFirstActor&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AMyFirstActor); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AMyFirstActor); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AMyFirstActor) \
	NO_API virtual ~AMyFirstActor();


#define FID_Users_william_mansfield_Desktop_AdvAnim_SuperFinalCoolProj_Source_SuperFinalCoolProj_Public_MyFirstActor_h_9_PROLOG
#define FID_Users_william_mansfield_Desktop_AdvAnim_SuperFinalCoolProj_Source_SuperFinalCoolProj_Public_MyFirstActor_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_william_mansfield_Desktop_AdvAnim_SuperFinalCoolProj_Source_SuperFinalCoolProj_Public_MyFirstActor_h_12_INCLASS_NO_PURE_DECLS \
	FID_Users_william_mansfield_Desktop_AdvAnim_SuperFinalCoolProj_Source_SuperFinalCoolProj_Public_MyFirstActor_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class AMyFirstActor;

// ********** End Class AMyFirstActor **************************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_william_mansfield_Desktop_AdvAnim_SuperFinalCoolProj_Source_SuperFinalCoolProj_Public_MyFirstActor_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
