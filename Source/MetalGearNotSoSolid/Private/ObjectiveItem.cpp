#include "ObjectiveItem.h"


#include "RikCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AObjectiveItem::AObjectiveItem()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComponent;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// Only collide with player
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComponent->SetupAttachment(MeshComponent);
}

void AObjectiveItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayParticleFX();

	const auto Rik = Cast<ARikCharacter>(OtherActor);
	if (Rik)
	{
		Rik->bIsObjectPickedUp = true;


		if (ParticlePickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ParticlePickupSound, GetActorLocation());
		}

		Destroy();
	}
}

void AObjectiveItem::PlayParticleFX()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ParticleFX, GetActorLocation());
}
