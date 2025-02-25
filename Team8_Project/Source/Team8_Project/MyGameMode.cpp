#include "MyGameMode.h"
#include "MyPlayerController.h" 
#include "MyCharacter.h"



AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = AMyCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
}
