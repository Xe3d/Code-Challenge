#include "VTPawn.h"
#include "Net/UnrealNetwork.h"

AVTPawn::AVTPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVTPawn::BeginPlay()
{
	Super::BeginPlay();	
}

void AVTPawn::Tick(float DeltaTime)
{
	if (MoveCharacter)
	{
		FVector Location(GetActorLocation());

		Location += CurrentDirection;

		SetActorLocation(Location);
	}
	Super::Tick(DeltaTime);
}

void AVTPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction<FVTInputDelegate>("Up", IE_Pressed, this, &AVTPawn::OnMovePressed, EDirection::UP);
	InputComponent->BindAction<FVTInputDelegate>("Down", IE_Pressed, this, &AVTPawn::OnMovePressed, EDirection::DOWN);
	InputComponent->BindAction<FVTInputDelegate>("Left", IE_Pressed, this, &AVTPawn::OnMovePressed, EDirection::LEFT);
	InputComponent->BindAction<FVTInputDelegate>("Right", IE_Pressed, this, &AVTPawn::OnMovePressed, EDirection::RIGHT);

	InputComponent->BindAction<FVTInputDelegate>("Up", IE_Released, this, &AVTPawn::OnMoveReleased, EDirection::UP);
	InputComponent->BindAction<FVTInputDelegate>("Down", IE_Released, this, &AVTPawn::OnMoveReleased, EDirection::DOWN);
	InputComponent->BindAction<FVTInputDelegate>("Left", IE_Released, this, &AVTPawn::OnMoveReleased, EDirection::LEFT);
	InputComponent->BindAction<FVTInputDelegate>("Right", IE_Released, this, &AVTPawn::OnMoveReleased, EDirection::RIGHT);


	InputComponent->BindAction("Action", IE_Pressed, this, &AVTPawn::OnActionPressed);
}

void AVTPawn::OnMoveReleased(EDirection Direction)
{
	CurrentDirection -= GetMovementVectorFromEnum(Direction);
	MoveOnServer(CurrentDirection);
}

void AVTPawn::OnMovePressed(EDirection Direction)
{
	CurrentDirection += GetMovementVectorFromEnum(Direction);
	MoveOnServer(CurrentDirection);
}

void AVTPawn::OnActionPressed()
{
	ActionPressedServer();
}

const FVector AVTPawn::GetMovementVectorFromEnum(EDirection Direction)
{
	switch (Direction)
	{
	case EDirection::UP:
		return FVector::ForwardVector;
		break;
	case EDirection::DOWN:
		return -FVector::ForwardVector;
		break;
	case EDirection::LEFT:
		return -FVector::RightVector;
		break;
	case EDirection::RIGHT:
		return FVector::RightVector;
		break;
	default:
		return FVector::ZeroVector;
		break;
	}
	
}

void AVTPawn::MoveOnServer_Implementation(FVector NewDir)
{
	CurrentDirection = NewDir;
	MoveCharacter = CurrentDirection != FVector::ZeroVector;	
}

bool AVTPawn::MoveOnServer_Validate(FVector NewDir)
{
	return true;
}

void AVTPawn::ActionPressedServer_Implementation()
{
	++ActionPressed;
	AddActorLocalRotation(FRotator(0.f, 5.f, 0.f));
}

bool AVTPawn::ActionPressedServer_Validate()
{
	return true;
}

void AVTPawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}