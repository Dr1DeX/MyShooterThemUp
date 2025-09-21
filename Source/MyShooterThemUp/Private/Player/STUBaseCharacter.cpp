// Shoot Them Up Game, All Rights Reserved.

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInt)
    : Super(ObjInt.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForwarder);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRighter);
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRun);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRun);
}

bool ASTUBaseCharacter::IsRunn() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero())
        return 0.0f;

    const auto VelocityNormal = GetVelocity().GetSafeNormal(); // нормаль скорости
    const auto AngleBetween = FMath::Acos(
        FVector::DotProduct(GetActorForwardVector(), VelocityNormal)); // скалярное произведение между вектором нормали и вектором движения
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal); // Ортогональный вектор
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::MoveForwarder(float Amount)
{
    IsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f)
        return;

    AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUBaseCharacter::MoveRighter(float Amount)
{
    if (Amount == 0.0f)
        return;

    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::OnStartRun()
{
    WantsToRun = true;
}

void ASTUBaseCharacter::OnStopRun()
{
    WantsToRun = false;
}
